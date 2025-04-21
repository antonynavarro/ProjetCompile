#include "tabsymbole.h"
#include <string.h>
#include <stdlib.h>


/* POUR LA TABLE DES SYMBOLES */
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

    // Detecter déclarations de fonctions
    if (strcmp(node->label, "Function") == 0) {
        const char *fn_name = SECONDCHILD(node->firstChild)->label;
        addSymbol(table, fn_name, "function", GLOBAL, 0); //faudra changer adresse apres
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
                          var->label,      // nom
                          decl->label,     // type
                          GLOBAL,          // scope
                          offset           // adresse
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
    
        node->localTable = local; // Attache la table a ce nœud de fonction
    
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