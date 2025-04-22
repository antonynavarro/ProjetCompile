#include "semantique.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// lookupType, verifyIdentifiers, exprType, checkAssignments

// implémentation de verifyIdentifiers, exprType, lookupType, checkAssignments


int identifierDeclared(const char *name,const TableSymbole *local,const TableSymbole *global){
    if (local && identExiste(local, name)) return 1; //c'est bon
    if (global && identExiste(global, name)) return 1;
    return false; // Il y a pas
    }
    
    // Parcours semantique pour verifier identificateurs
    void verifyIdentifiers(Node *node,const TableSymbole *global,const TableSymbole *local){
        if (!node) return;
    
        //Si on arrive sur un nœud de fonction, on change la table locale
        const TableSymbole *curLocal = local;
        if (strcmp(node->label, "Function") == 0 && node->localTable) {
        curLocal = node->localTable;
        }
    
        if (strcmp(node->label, "IDENT") == 0) {
            if (!identifierDeclared(node->value, curLocal, global)) {
                fprintf(stderr,
                    "Erreur sémantique : '%s' non déclaré (ligne %d)\n",
                    node->value, node->lineno);
            }
        }
    
        verifyIdentifiers(node->firstChild, global, curLocal);
        verifyIdentifiers(node->nextSibling, global, local);
    }
    
    
    
    /*DETERMINER TYPE EXPRESSION */
    
    // Cherche dans globale puis locale
    const char* lookupType(const TableSymbole *g, const TableSymbole *l, const char *ident) {
        for(int i=0; i<g->count; i++)
            if(strcmp(g->symb[i].ident, ident)==0) return g->symb[i].type;
        for(int i=0; i<l->count; i++)
            if(strcmp(l->symb[i].ident, ident)==0) return l->symb[i].type;
        return NULL; // non déclaré
    }
    
    // évalue le type d’une expression
    const char* exprType(Node *node,
                         const TableSymbole *g,
                         const TableSymbole *l)
    {
        if (!node) return NULL;
    
        // littéraux numériques → int
        if (isdigit(node->label[0]) || (node->label[0]=='-' && isdigit(node->label[1])))
            return "int";
    
        // littéraux caractère → char
        if (node->label[0]=='\'' && node->label[strlen(node->label)-1]=='\'')
            return "char";
    
        // variable ou paramètre
        if (node->firstChild==NULL && isalpha(node->label[0])) {
            const char *t = lookupType(g, l, node->label);
            return t ? t : "unknown";
        }
    
        // opérations arithmétiques/addition/soustraction → int
        if (strcmp(node->label,"AddSub")==0 ||
            strcmp(node->label,"DivStar")==0 ||
            strcmp(node->label,"Order")==0 ||
            strcmp(node->label,"Or")==0 ||
            strcmp(node->label,"And")==0)
        {
            // on pourrait vérifier la cohérence des types des sous‑expressions,
            // mais pour l’instant on retourne toujours int
            return "int";
        }
    
        // appel récursif : si vous voulez gérer plus de cas
        return exprType(node->firstChild, g, l);
    }
    
    
    void checkAssignments(Node *node,
        const TableSymbole *g,
        const TableSymbole *l)
    {
    if (!node) return;
    
    // Si on rentre dans une fonction, on change de contexte local
    if (strcmp(node->label, "Function") == 0) {
    // on suppose que vous avez fait : node->localTable = malloc(...)
    TableSymbole *myLocals = node->localTable;
    // On veut vérifier tout le corps de la fonction avec cette table,
    // **sans** retomber ensuite sur l'ancien l.
    // body est le 2ᵉ enfant (HEAD puis BODY)
    Node *body = node->firstChild->nextSibling;
    checkAssignments(body, g, myLocals);
    // On ne do it again for HEAD, HEAD ne contient pas d’Eq
    // Puis on continue la recursion sur les frères de ce Function
    checkAssignments(node->nextSibling, g, l);
    return;
    }
    
    // Vérifier si c'est une affectation LHS = RHS
    if (strcmp(node->label, "Eq") == 0 &&
    node->firstChild &&
    node->firstChild->nextSibling)
    {
    Node *lhs = node->firstChild;
    Node *rhs = lhs->nextSibling;
    
    // lookupType cherche d’abord dans global puis dans la table locale l
    const char *t_lhs = lookupType(g, l, lhs->label);
    const char *t_rhs = exprType(rhs, g, l);
    if (t_lhs && t_rhs &&
    strcmp(t_lhs, "char")==0 &&
    strcmp(t_rhs, "int")==0)
    {
    fprintf(stderr,
    "Warning (ligne %d) : affectation int → char pour '%s'\n",
    node->lineno, lhs->label);
    }
    }
    
    // sinon, on continue la recursion en gardant toujours la même table locale
    checkAssignments(node->firstChild,  g, l);
    checkAssignments(node->nextSibling, g, l);
    }
    