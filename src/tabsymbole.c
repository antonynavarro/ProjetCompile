#include "tabsymbole.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int sem_error = 0;

// Taille totale de la mémoire statique allouée pour les variables globales
size_t totalGlobalMemory = 0;

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

// Obtenir la taille en octets d'un type
size_t getSizeOfType(const char* type) {
    if (strcmp(type, "int") == 0) return 8;      // 64 bits
    else if (strcmp(type, "char") == 0) return 1; // 8 bits
    else return 8; // Taille par défaut
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

    // Déclarations globales avec gestion d'adresses
    if (strcmp(node->label, "Global") == 0) {
        Node *decl = node->firstChild;
        while (decl) {
            // Vérifier si c'est un type valide
            if (strcmp(decl->label, "int") != 0 && strcmp(decl->label, "char") != 0) {
                decl = decl->nextSibling;
                continue;
            }
            
            size_t size = getSizeOfType(decl->label);
            
            // Traiter chaque variable de ce type
            for (Node *var = decl->firstChild; var; var = var->nextSibling) {
                if (identExisteDansScope(table, var->value, GLOBAL)) {
                    fprintf(stderr,
                            "Erreur sémantique : Variable globale \"%s\" redéclarée\n", var->value);
                    sem_error = 2;
                } else {
                    // Aligner l'adresse si nécessaire (pour l'alignement des types)
                    if (size > 1) {
                        // Alignement sur la taille du type (8 octets pour int)
                        offset = (offset + size - 1) & ~(size - 1);
                    }
                    
                    // Ajouter le symbole avec l'adresse calculée
                    addSymbol(table, var->value, decl->label, GLOBAL, offset, 0, 0, 0);
                    
                    // Mettre à jour l'offset et la taille totale
                    offset += size;
                    totalGlobalMemory += size;
                    
                    printf("Variable globale '%s' de type '%s' à l'adresse %zu\n", 
                           var->value, decl->label, offset - size);
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
    
    for (int i = 0; i < localTable->count; i++) {
        if (localTable->symb[i].isFunction == 0) {
            size_t varSize;
            
            
            if (strcmp(localTable->symb[i].type, "int") == 0) {
                varSize = 8;  // 64-bit integer
            } else if (strcmp(localTable->symb[i].type, "char") == 0) {
                varSize = 1;  // 8-bit char
            } else {
                // Default 8 bytes
                varSize = 8;
            }
                      
            currentOffset = (currentOffset + 7) & ~7;
                   
            localTable->symb[i].address = -currentOffset - varSize;
            
            currentOffset += varSize;
        }
    }
    
    currentOffset = (currentOffset + 15) & ~15;
}

void generateLocalSymbolTable(Node *node) {
    if (!node) return;

    if (strcmp(node->label, "Function") == 0) {
        Node *head = node->firstChild;
        Node *body = head->nextSibling;

        TableSymbole *local = malloc(sizeof(TableSymbole));
        local->count = 0;
        node->localTable = local; // attacher a fonction

        // Ajoute parametres
        Node *paramNode = head->firstChild->nextSibling->nextSibling;
        if (paramNode && strcmp(paramNode->label, "Parameter") == 0) {
            Node *paramTypeNode = paramNode->firstChild;
            while (paramTypeNode) {
                if (paramTypeNode->firstChild) {
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
                            // Check conflicts
                            if (identExiste(local, identNode->value)) {
                                fprintf(stderr,
                                    "Semantic Error: Local variable conflict \"%s\"\n", 
                                    identNode->value);
                                sem_error = 2;
                            }
                            addSymbol(local, identNode->value, typeNode->label, LOCAL, 0, 0, strcmp(varTypeNode->label, "Static") == 0, 0);
                            identNode = identNode->nextSibling;
                        }
                        
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
        calculateLocalVariableAddresses(local);
    }

    // Recursive traversal
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

// Fonction pour obtenir la taille totale de la mémoire statique
size_t getTotalGlobalMemorySize() {
    return totalGlobalMemory;
}

// Génère le code assembleur pour la section .data
void generateGlobalDataSection(FILE* out, TableSymbole* table) {
    fprintf(out, "\n.section .data\n");
    
    // Parcourir toute la table des symboles pour générer le code pour les variables globales
    for (int i = 0; i < table->count; i++) {
        // Ignorer les fonctions
        if (table->symb[i].isFunction) continue;
        
        // Traiter uniquement les variables globales
        if (table->symb[i].scope == GLOBAL) {
            if (strcmp(table->symb[i].type, "int") == 0) {
                fprintf(out, "    %s: .quad 0\n", table->symb[i].ident);
            } else if (strcmp(table->symb[i].type, "char") == 0) {
                fprintf(out, "    %s: .byte 0\n", table->symb[i].ident);
            }
        }
    }
}