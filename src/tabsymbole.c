#include "tabsymbole.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int sem_error = 0;

/* POUR LA TABLE DES SYMBOLES */
int identExiste(const TableSymbole* table, const char* ident) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symb[i].ident, ident) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Vérifie si ident existe dans table avec un scope donné */
int identExisteDansScope(const TableSymbole* table, const char* ident, Scope scope) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symb[i].ident, ident) == 0 && table->symb[i].scope == scope) {
            return 1;
        }
    }
    return 0;
}

void addSymbol(TableSymbole* table, const char* ident, const char* type, Scope scope, int address, int isFunction) {
    if (identExiste(table, ident)) {
        fprintf(stderr,
                "Erreur sémantique : \"%s\" déjà déclaré\n", ident);
        sem_error = 2;
        return;
    }
    if (table->count >= MAX_SYMBOLES) {
        fprintf(stderr,
                "Erreur interne : table des symboles pleine\n");
        return;
    }
    strncpy(table->symb[table->count].ident, ident, sizeof(table->symb[table->count].ident) - 1);
    table->symb[table->count].ident[sizeof(table->symb[table->count].ident) - 1] = '\0';
    strncpy(table->symb[table->count].type, type, sizeof(table->symb[table->count].type) - 1);
    table->symb[table->count].type[sizeof(table->symb[table->count].type) - 1] = '\0';
    table->symb[table->count].scope = scope;
    table->symb[table->count].address = address;
    table->symb[table->count].isFunction = isFunction;
    table->count++;
}

void generateGlobalSymbolTable(Node *node, TableSymbole* table) {
    static size_t offset = 0;  // décalage cumulatif en octets pour les variables

    if (!node) return;

    if (strcmp(node->label, "Function") == 0) {
        if (SECONDCHILD(node->firstChild)) { // Sécuriser
            const char *fn_name = SECONDCHILD(node->firstChild)->label;
            const char *fn_type = FIRSTCHILD(node->firstChild)->label;
            if (identExiste(table, fn_name)) {
                fprintf(stderr,
                        "Erreur sémantique : Conflit nom global/fonction pour \"%s\"\n", fn_name);
                sem_error = 2;
            } else {
                addSymbol(table, fn_name, fn_type, GLOBAL, 0,1);
            }
        }
    }

    // Déclarations globales (inchangé)
    if (strcmp(node->label, "Global") == 0) {
        Node *decl = node->firstChild;
        while (decl) {
            size_t size;
            if      (strcmp(decl->label, "int")  == 0) size = 8;
            else if (strcmp(decl->label, "char") == 0) size = 1;
            else { decl = decl->nextSibling; continue; }

            for (Node *var = decl->firstChild; var; var = var->nextSibling) {
                if (identExisteDansScope(table, var->label, GLOBAL)) {
                    fprintf(stderr,
                            "Erreur sémantique : Variable globale \"%s\" redéclarée\n", var->label);
                    sem_error = 2;
                } else {
                    addSymbol(table, var->label, decl->label, GLOBAL, offset,0);
                    offset += size;
                }
            }
            decl = decl->nextSibling;
        }
    }

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
                    if (identExiste(local, paramTypeNode->firstChild->label)) {
                        fprintf(stderr,
                                "Erreur sémantique : Conflit paramètre/variable locale \"%s\"\n", paramTypeNode->firstChild->label);
                        sem_error = 2;
                    }
                    addSymbol(local, paramTypeNode->firstChild->label, paramTypeNode->label, LOCAL, 0,0);
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
                            if (identExiste(local, identNode->label)) {
                                fprintf(stderr,
                                        "Erreur sémantique : Conflit paramètre/variable locale \"%s\"\n", identNode->label);
                                sem_error = 2;
                            }
                            addSymbol(local, identNode->label, typeNode->label, LOCAL, 0,0);
                            identNode = identNode->nextSibling;
                        }
                        typeNode = typeNode->nextSibling;
                    }
                }
                varNode = varNode->nextSibling;
            }
        }
    }

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
