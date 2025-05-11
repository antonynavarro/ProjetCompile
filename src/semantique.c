// src/semantique.c
#include "semantique.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Recherche un symbole par nom dans la table (locale puis globale). */
static const Symbole* lookupSymbol(const TableSymbole *g,
                                   const TableSymbole *l,
                                   const char *ident)
{
    if (l) {
        for (int i = 0; i < l->count; i++)
            if (strcmp(l->symb[i].ident, ident) == 0)
                return &l->symb[i];
    }
    if (g) {
        for (int i = 0; i < g->count; i++)
            if (strcmp(g->symb[i].ident, ident) == 0)
                return &g->symb[i];
    }
    return NULL;
}

/* Retourne le type (chaîne) d’un ident ou NULL si non trouvé. */
const char* lookupType(const TableSymbole *g,
                       const TableSymbole *l,
                       const char *ident)
{
    const Symbole *sym = lookupSymbol(g, l, ident);
    return sym ? sym->type : NULL;
}

/* Vérifie que tous les identifiants sont déclarés avant utilisation. */
void verifyIdentifiers(Node *node,
                       const TableSymbole *global,
                       const TableSymbole *local)
{
    if (!node) return;
    
    printf("verifyIdentifiers: %s\n", node->label);

    if (strcmp(node->label, "Ident") == 0) {
        if (!lookupSymbol(global, local, node->value)) {
            fprintf(stderr,
                "error (line %d): identifier '%s' not declared\n",
                node->lineno, node->value);
            sem_error = 2;
        }
    }

    verifyIdentifiers(node->firstChild, global, local);
    verifyIdentifiers(node->nextSibling, global, local);
}

/* Calcule le type d’une expression. */
const char* exprType(Node *node,
                     const TableSymbole *g,
                     const TableSymbole *l)
{
    if (!node) return NULL;

    if (isdigit(node->label[0]) ||
       (node->label[0]=='-' && isdigit(node->label[1])))
        return "int";

    if (node->label[0]=='\'' && node->label[strlen(node->label)-1]=='\'')
        return "char";

    if (strcmp(node->label, "Call")==0 && node->firstChild) {
        const char *fn_name = node->firstChild->label;
        const Symbole *sym = lookupSymbol(g, l, fn_name);
        if (!sym) {
            return NULL;
        }
        if (!sym->isFunction) {
            fprintf(stderr,
                "error (line %d): '%s' is not a function\n",
                node->lineno, fn_name);
            sem_error = 2;
            return NULL;
        }
        if (strcmp(sym->type, "void")==0) {
            fprintf(stderr,
                "error (line %d): void function '%s' used in expression\n",
                node->lineno, fn_name);
            sem_error = 2;
        }
        return sym->type;
    }

    if (node->firstChild==NULL && isalpha(node->label[0])) {
        const char *t = lookupType(g, l, node->label);
        return t ? t : "unknown";
    }

    if (!strcmp(node->label,"AddSub") ||
        !strcmp(node->label,"DivStar")||
        !strcmp(node->label,"Order")  ||
        !strcmp(node->label,"Or")     ||
        !strcmp(node->label,"And"))
    {
        return "int";
    }

    return exprType(node->firstChild, g, l);
}

/* Vérifie les assignments LValue = RHS, warning si char ← int. */
void checkAssignments(Node *node,
                      const TableSymbole *g,
                      const TableSymbole *l)
{
    if (!node) return;

    if (strcmp(node->label, "Function")==0 && node->localTable) {
        TableSymbole *myLocals = node->localTable;
        Node *body = node->firstChild->nextSibling;
        checkAssignments(body, g, myLocals);
        checkAssignments(node->nextSibling, g, l);
        return;
    }

    if (strcmp(node->label, "Eq")==0 &&
        node->firstChild && node->firstChild->nextSibling)
    {
        Node *lhs = node->firstChild;
        Node *rhs = lhs->nextSibling;
        const char *t_lhs = lookupType(g, l, lhs->label);
        const char *t_rhs = exprType(rhs, g, l);
        if (t_lhs && t_rhs &&
            strcmp(t_lhs, "char")==0 &&
            strcmp(t_rhs, "int")==0)
        {
            fprintf(stderr,
                "warning (line %d): assigning ‘int’ to ‘char’ may lose data: %s = …\n",
                node->lineno, lhs->label);
        }
    }

    checkAssignments(node->firstChild, g, l);
    checkAssignments(node->nextSibling, g, l);
}

/* Vérifie la présence d’une fonction main() renvoyant int. */
void verifyMainExists(const TableSymbole *global)
{
    int found = 0;
    for (int i = 0; i < global->count; i++) {
        const Symbole *s = &global->symb[i];
        if (s->isFunction &&
            strcmp(s->ident, "main")==0 &&
            strcmp(s->type, "int")==0)
        {
            found = 1;
            break;
        }
    }
    if (!found) {
        fprintf(stderr,
            "error: undefined reference to ‘main’\n");
            sem_error = 2;
    }
}

/* Vérifie les return conformes au type de la fonction. */
static void checkReturnsInSubtree(Node *n,
    const char *expectedType,
    const TableSymbole *global,
    const TableSymbole *local,
    int *hasReturn)
{
    if (!n) return;

    if (strcmp(n->label, "Return") == 0) {
        *hasReturn = 1;
        if (n->firstChild) {
            const char *actualType = exprType(n->firstChild, global, local);
            if (strcmp(expectedType, "char") == 0 && strcmp(actualType, "int") == 0) {
                fprintf(stderr,
                    "warning (line %d): return from ‘int’ to ‘char’ may lose data\n",
                    n->lineno);
            } else if (strcmp(expectedType, actualType) != 0) {
                fprintf(stderr,
                    "error (line %d): return type ‘%s’ does not match function return type ‘%s’\n",
                    n->lineno,
                    actualType ? actualType : "unknown",
                    expectedType);
                sem_error = 2;
            }
        }
    }

    checkReturnsInSubtree(n->firstChild, expectedType, global, local, hasReturn);
    checkReturnsInSubtree(n->nextSibling, expectedType, global, local, hasReturn);
}

void verifyReturns(Node *node, const TableSymbole *global) {
    if (!node) return;

    if (strcmp(node->label, "Function") == 0) {
        const char *expectedType = FIRSTCHILD(node->firstChild)->label;
        const TableSymbole *local = node->localTable;
        Node *body = node->firstChild->nextSibling;

        int hasReturn = 0;
        checkReturnsInSubtree(body, expectedType, global, local, &hasReturn);

        if (strcmp(expectedType, "void") != 0 && !hasReturn) {
            fprintf(stderr,
                "error: function '%s' with non-void return type must return a value\n",
                node->firstChild->label);
            sem_error = 2;
        }

        verifyReturns(node->nextSibling, global);
        return;
    }

    verifyReturns(node->firstChild, global);
    verifyReturns(node->nextSibling, global);
}
