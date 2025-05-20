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

void addFun(TableSymbole* table){
    addSymbol(table, "getint",    "int", GLOBAL, 0, 1);
    addSymbol(table, "putint",    "void",GLOBAL, 0, 1);
    addSymbol(table, "getchar",   "char",GLOBAL, 0, 1);
    addSymbol(table, "putchar",   "void",GLOBAL, 0, 1);

}

void addSymbol(TableSymbole* table, const char* ident, const char* type, Scope scope, int address, int isFunction, int isStatic, int isParam) {
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
    table->symb[table->count].isStatic = isStatic;
    table->symb[table->count].isParam = isParam;
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
                addSymbol(table, fn_name, fn_type, GLOBAL, 0, 1, 0, 0);
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
                if (identExisteDansScope(table, var->value, GLOBAL)) {
                    fprintf(stderr,
                            "Erreur sémantique : Variable globale \"%s\" redéclarée\n", var->label);
                    sem_error = 2;
                } else {
                    addSymbol(table, var->value, decl->label, GLOBAL, offset, 0, 0, 0);
                    offset += size;
                }
            }
            decl = decl->nextSibling;
        }
    }

    generateGlobalSymbolTable(node->firstChild,  table);
    generateGlobalSymbolTable(node->nextSibling, table);
}


void calculateLocalVariableAddresses(TableSymbole* localTable) {
    int currentOffset = 0;
    
    // First, process parameters (if needed)
    for (int i = 0; i < localTable->count; i++) {
        if (localTable->symb[i].isFunction == 0) {
            size_t varSize;
            
            // Determine variable size based on type
            if (strcmp(localTable->symb[i].type, "int") == 0) {
                varSize = 8;  // 64-bit integer
            } else if (strcmp(localTable->symb[i].type, "char") == 0) {
                varSize = 1;  // 8-bit character
            } else {
                // Default to 8 bytes for unknown types
                varSize = 8;
            }
            
            // Align to 8-byte boundary
            currentOffset = (currentOffset + 7) & ~7;
            
            // Assign negative offset from rbp
            localTable->symb[i].address = -currentOffset - varSize;
            
            // Increment offset
            currentOffset += varSize;
        }
    }
    
    // Ensure total space is a multiple of 16 for stack alignment
    currentOffset = (currentOffset + 15) & ~15;
}

void generateLocalSymbolTable(Node *node, TableSymbole* globalTable) {
    if (!node) return;

    if (strcmp(node->label, "Function") == 0) {
        Node *head = node->firstChild;
        Node *body = head->nextSibling;

        // Create a new local symbol table
        TableSymbole *local = malloc(sizeof(TableSymbole));
        local->count = 0;
        node->localTable = local; // Attach table to this function node

        // Add parameters
        Node *paramNode = head->firstChild->nextSibling->nextSibling;
        if (paramNode && strcmp(paramNode->label, "Parameter") == 0) {
            Node *paramTypeNode = paramNode->firstChild;
            while (paramTypeNode) {
                if (paramTypeNode->firstChild) {
                    // Check for parameter conflicts
                    if (identExiste(local, paramTypeNode->firstChild->value)) {
                        fprintf(stderr,
                                "Semantic Error: Parameter conflict \"%s\"\n", 
                                paramTypeNode->firstChild->label);
                        sem_error = 2;
                    }
                    addSymbol(local, paramTypeNode->firstChild->value, paramTypeNode->label, LOCAL, 0, 0, 0, 1);
                }
                paramTypeNode = paramTypeNode->nextSibling;
            }
        }

        // Add local variables
        if (body && strcmp(body->label, "Body") == 0) {
            Node *varNode = body->firstChild;
            while (varNode) {
                if (strcmp(varNode->label, "Vars") == 0 && varNode->firstChild) {
                    Node *varTypeNode = varNode->firstChild;
                    Node *typeNode = strcmp(varTypeNode->label, "Static") == 0 
                        ? varNode->firstChild->firstChild 
                        : varNode->firstChild;
                    
                    while (typeNode) {
                        Node *identNode = typeNode->firstChild;
                        while (identNode) {
                            // Check for local variable conflicts
                            if (identExiste(local, identNode->value)) {
                                fprintf(stderr,
                                    "Semantic Error: Local variable conflict \"%s\"\n", 
                                    identNode->value);
                                sem_error = 2;
                            }
                            addSymbol(local, identNode->value, typeNode->label, LOCAL, 0, 0, strcmp(varTypeNode->label, "Static") == 0, 0);
                            identNode = identNode->nextSibling;
                        }
                        
                        // Handle static and non-static type nodes
                        if (strcmp(varTypeNode->label, "Static") == 0) {
                            varTypeNode = varTypeNode->nextSibling;
                            typeNode = varTypeNode ? varTypeNode->firstChild : NULL;
                        } else {
                            typeNode = typeNode->nextSibling;
                        }
                    }
                }
                varNode = varNode->nextSibling;
            }
        }

        // Calculate addresses for local variables
        calculateLocalVariableAddresses(local);
    }

    generateLocalSymbolTable(node->firstChild);
    generateLocalSymbolTable(node->nextSibling);
}



void printSymbolTable(TableSymbole* table) {
    for (int i = 0; i < table->count; i++) {
        printf("Nom: %s, Type: %s, Portée: %s, Statique: %s, Paramètre: %s, Adresse: %d\n",
               table->symb[i].ident,
               table->symb[i].type,
               (table->symb[i].scope == GLOBAL) ? "Global" : "Local",
               table->symb[i].isStatic ? "Oui" : "Non",
               table->symb[i].isParam ? "Oui" : "Non",
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
