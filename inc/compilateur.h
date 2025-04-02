#ifndef COMPILATEUR_H
#define COMPILATEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"


/* Définition des constantes */
#define MAX_SYMBOLES 100
#define MAX_IDENT_LEN 50

typedef enum { GLOBAL, LOCAL } Scope;

typedef struct {
    char ident[MAX_IDENT_LEN];  // Nom variable ou fonction
    char type[50];            
    Scope scope;                // Portée : globale ou locale
    void * address;                
} Symbole;

typedef struct {
    Symbole symb[MAX_SYMBOLES];
    int count;
} TableSymbole;


/* Prototypes des fonctions */
int identExiste(const TableSymbole* table, const char* ident);
void rempliTable(TableSymbole* table, const char* ident, const char* type);
void generateNASM(Node *node, FILE *out);
void translate(Node* root);
void generateGlobalSymbolTable(Node *node, TableSymbole* table);
void addSymbol(TableSymbole* table, const char* ident, const char* type, Scope scope, void * address);
void generateLocalSymbolTable(Node *node, TableSymbole* table) ;
void printSymbolTable(TableSymbole* table);

#endif /* COMPILATEUR_H */


