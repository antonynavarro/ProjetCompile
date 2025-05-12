/* compilateur.c */

#include "compilateur.h"
#include "tree.h"
#include <ctype.h>

#define MAX_SYMBOLES 100

// Grosse fonction qui fait tout (à séparer plus tard)
void translate(Node* root, bool show) {
	
    TableSymbole globalTable = {.count = 0};

    //table des variables globales
    generateGlobalSymbolTable(root, &globalTable);
    if (show) {printSymbolTable(&globalTable);}

    // tables des fonctions et vars locals
    generateLocalSymbolTable(root, NULL);
    if (show) {printAllLocalSymbolTables(root);}


    //verifyIdentifiers(root, &globalTable, NULL);
    verifyMainExists(&globalTable); // <- après la table des symboles
    verifyReturns(root, &globalTable);
    checkAssignments(root, &globalTable, NULL);



    FILE *out = fopen("_anonymous.asm", "w");
    if (!out) {
        perror("Error opening _anonymous.asm");
        exit(EXIT_FAILURE);
    }

    emitNASMHeader(out, &globalTable);
    generateNASM(root, &globalTable, out);

    //executer code nasm avec : nasm -felf64 _anonymous.asm && ld _anonymous.o -o prog && ./prog

    

    fclose(out);
}

/*
//Function Prototypes
int identExiste(const TableSymbole* table, const char* ident);
void rempliTable(TableSymbole* table, const char* ident, const char* type);
void generateNASM(Node *node, FILE *out);
void translate(Node* root);



void generateNASM(Node *node, FILE *out) {
    static int depth = 0;  
    static bool rightmost[128];  

    if (!node) return; // Vérification de NULL

    // Affichage pour debug
    for (int i = 1; i < depth; i++) {
        printf(rightmost[i] ? "    " : "\u2502   ");
    }
    if (depth > 0) {
        printf(rightmost[depth] ? "\u2514\u2500\u2500 " : "\u251c\u2500\u2500 ");
    }
    printf("Processing node: %s\n", node->label);

    // Vérification avant d'accéder à node->label
    if (node->label && (isdigit(node->label[0]) || (node->label[0] == '-' && isdigit(node->label[1])))) {
        fprintf(out, " push %s\n", node->label);
        return;
    }

    // Vérification des opérations arithmétiques AddSub
    if (node->label && strcmp(node->label, "AddSub") == 0) {
        if (node->firstChild) generateNASM(node->firstChild, out);
        if (node->firstChild && node->firstChild->nextSibling) generateNASM(node->firstChild->nextSibling, out);

        if (node->value && strcmp(node->value, "+") == 0) {
            fprintf(out, " pop rbx\n pop rax\n add rax, rbx\n push rax\n");
        } else {
            fprintf(out, " pop rbx\n pop rax\n sub rax, rbx\n push rax\n");
        }
        return;
    }

    // Vérification de Eq
    if (node->label && strcmp(node->label, "Eq") == 0) {
        if (node->firstChild) generateNASM(node->firstChild, out);
        if (node->firstChild && node->firstChild->nextSibling) generateNASM(node->firstChild->nextSibling, out);

        if (node->value && strcmp(node->value, "==") == 0) {
            fprintf(out, " pop rbx\n pop rax\n cmp rax, rbx\n sete al\n movzx rax, al\n push rax\n");
        } else if (node->value && strcmp(node->value, "!=") == 0) {
            fprintf(out, " pop rbx\n pop rax\n cmp rax, rbx\n setne al\n movzx rax, al\n push rax\n");
        }
        return;
    }

    // Parcours récursif des enfants
    depth++;
    for (Node *child = node->firstChild; child != NULL; child = child->nextSibling) {
        if (depth < 128) // Vérifier pour éviter dépassement de tableau
            rightmost[depth] = (child->nextSibling == NULL);
        generateNASM(child, out);
    }
    depth--;
}

void translate(Node* root) {
	
    TableSymbole globalTable = {.count = 0};
    //TableSymbole localTable = {.count = 0};

    // Générer la table des variables globales
    generateGlobalSymbolTable(root, &globalTable);
    printSymbolTable(&globalTable);

    // Générer les tables des fonctions
    generateLocalSymbolTable(root, NULL);
    //printSymbolTable(&localTable);

    //printSymbolTable(&globalTable);
    printAllLocalSymbolTables(root);

    
    verifyIdentifiers(root, &globalTable, NULL);

    // **Vérification de type**
    checkAssignments(root, &globalTable, NULL);


    // 3) Ouverture du fichier de sortie 
    FILE *out = fopen("_anonymous.asm", "w");
    if (!out) {
        perror("Error opening _anonymous.asm");
        exit(EXIT_FAILURE);
    }

    // Section BSS : on réserve un mot (8 octets) pour chaque int, un octet pour chaque char
    fprintf(out, "section .bss\n");
    for (int i = 0; i < globalTable.count; i++) {
        Symbole *s = &globalTable.symb[i];
        if (strcmp(s->type, "function") == 0) {
            // on ne réserve rien pour les fonctions
            continue;
        }
        if (strcmp(s->type, "char") == 0) {
            fprintf(out, "%s: resb 1    ; at offset %d\n", s->ident, s->address);
        } else {  // on suppose int => 8 octets 
            fprintf(out, "%s: resq 1    ; at offset %d\n", s->ident, s->address);
        }
    }
    fprintf(out, "\n");

    // Section code 
    fprintf(out, "section .text\n"
                 "    global _start\n"
                 "_start:\n");

    // Génération du corps de _start (votre code)
    if (root) generateNASM(root, out);

    // Exit syscall
    fprintf(out,
            "    mov rax, 60\n"
            "    xor rdi, rdi\n"
            "    syscall\n");

    fclose(out);
}



// POUR LA TABLE DES SYMBOLES
int identExiste(const TableSymbole* table, const char* ident) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symb[i].ident, ident) == 0) {
            return 1;
        }
    }
    return 0;
}


void addSymbol(TableSymbole* table, const char* ident, const char* type, Scope scope, int address) {
    if (identExiste(table, ident) || table->count >= MAX_SYMBOLES) {
        return;
    }
    strncpy(table->symb[table->count].ident, ident, sizeof(table->symb[table->count].ident) - 1);
    strncpy(table->symb[table->count].type, type, sizeof(table->symb[table->count].type) - 1);
    table->symb[table->count].scope = scope;
    table->symb[table->count].address = address;
    table->count++;
}

void generateGlobalSymbolTable(Node *node, TableSymbole* table) {
    static size_t offset = 0;  // décalage cumulatif en octets pour les variables

    if (!node) return;

    // D’abord, repérer les déclarations de fonctions
    if (strcmp(node->label, "Function") == 0) {
        // le nom de la fonction est le 2ᵉ fils de "Head"
        const char *fn_name = SECONDCHILD(node->firstChild)->label;
        addSymbol(table, fn_name, "function", GLOBAL,  0);
    }

    //  Puis, les déclarations globales
    if (strcmp(node->label, "Global") == 0) {
        Node *decl = node->firstChild;
        while (decl) {
            size_t size;
            if      (strcmp(decl->label, "int")  == 0) size = 8;
            else if (strcmp(decl->label, "char") == 0) size = 1;
            else { decl = decl->nextSibling; continue; }

            // pour chaque ident d’une même ligne `int A, B, C;`
            for (Node *var = decl->firstChild; var; var = var->nextSibling) {
                addSymbol(table,
                          var->label,      // nom, ex. "FOO"
                          decl->label,     // type, ex. "int"
                          GLOBAL,
                          offset           // adresse relative
                );
                offset += size;
            }
        decl = decl->nextSibling;
        }
    }

    // récursion
    generateGlobalSymbolTable(node->firstChild,  table);
    generateGlobalSymbolTable(node->nextSibling, table);
}



void generateLocalSymbolTable(Node *node, TableSymbole* table) {
    if (!node) return;

    if (strcmp(node->label, "Function") == 0) {
        Node *head = node->firstChild;
        Node *body = head->nextSibling;
    
        // Création d'une nouvelle table locale
        TableSymbole *local = malloc(sizeof(TableSymbole));
        local->count = 0;
    
        node->localTable = local; // <-- Attache la table à ce nœud de fonction
    
        // Ajouter les paramètres
        Node *paramNode = head->firstChild->nextSibling->nextSibling;
        if (paramNode && strcmp(paramNode->label, "Parameter") == 0) {
            Node *paramTypeNode = paramNode->firstChild;
            while (paramTypeNode) {
                if (paramTypeNode->firstChild) {
                    addSymbol(local, paramTypeNode->firstChild->label, paramTypeNode->label, LOCAL, 0);
                }
                paramTypeNode = paramTypeNode->nextSibling;
            }
        }

    
        // Ajouter les variables locales
        if (body && strcmp(body->label, "Body") == 0) {
            Node *varNode = body->firstChild;
            while (varNode) {
                if (strcmp(varNode->label, "Vars") == 0) {
                    Node *typeNode = varNode->firstChild;
                    while (typeNode) {
                        Node *identNode = typeNode->firstChild;
                        while (identNode) {
                            addSymbol(local, identNode->label, typeNode->label, LOCAL, 0);
                            identNode = identNode->nextSibling;
                        }
                        typeNode = typeNode->nextSibling;
                    }
                }
                varNode = varNode->nextSibling;
            }
        }
    }
    

    // Parcours récursif
    generateLocalSymbolTable(node->firstChild, table);
    generateLocalSymbolTable(node->nextSibling, table);
}


void printSymbolTable(TableSymbole* table) {
    printf("\nTable des Symboles:\n");
    for (int i = 0; i < table->count; i++) {
        printf("Nom: %s, Type: %s, Portée: %s, Adresse: %d\n", 
               table->symb[i].ident, 
               table->symb[i].type,
               (table->symb[i].scope == GLOBAL) ? "Global" : "Local",
               table->symb[i].address);
    }
}

void printAllLocalSymbolTables(Node *node) {
    if (!node) return;

    if (strcmp(node->label, "Function") == 0 && node->localTable != NULL) {
        printf("\nTable des symboles de la fonction : %s\n", SECONDCHILD(node->firstChild)->label);
        printSymbolTable(node->localTable);
    }

    printAllLocalSymbolTables(node->firstChild);
    printAllLocalSymbolTables(node->nextSibling);
}



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



// DETERMINER TYPE EXPRESSION

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
*/
