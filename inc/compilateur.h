#ifndef COMPILATEUR_H
#define COMPILATEUR_H

#include "tree.h"
#include "tabsymbole.h"
#include "semantique.h"
#include "codegen.h"
#include <string.h>
#include <stdbool.h>

void translate(Node* root, bool show);

#endif


/*
#ifndef COMPILATEUR_H
#define COMPILATEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

// Définition des constantes 
#define MAX_SYMBOLES 100
#define MAX_IDENT_LEN 50

typedef enum { GLOBAL, LOCAL } Scope;

typedef struct {
    char ident[MAX_IDENT_LEN];  // Nom variable ou fonction
    char type[50];            
    Scope scope;                // Portée : globale ou locale
    int address;                //adresse de la variable 
} Symbole;



typedef struct TableSymbole{
    Symbole symb[MAX_SYMBOLES];
    int count;
} TableSymbole;



//Lors du parcours et que on rencontre une fonction on aloue de la memoire pour une nouvelle table des symboles 
//et ajoute un pointeur vers cette table des symboles

//faire autre strcuture pour les fonction

//Une table avec les variables globales
//Une table avec les fontions et variables statics de chaque fonction

//premier parcours tables des symboles
//deuxième erreur semantiques (pour plus tard)
//troisième traduction (pour plus tard)


// Prototypes des fonctions
int identExiste(const TableSymbole* table, const char* ident);
void rempliTable(TableSymbole* table, const char* ident, const char* type);
void generateNASM(Node *node, FILE *out);
void translate(Node* root);
void generateGlobalSymbolTable(Node *node, TableSymbole* table);
void addSymbol(TableSymbole* table, const char* ident, const char* type, Scope scope, int address);
void generateLocalSymbolTable(Node *node, TableSymbole* table) ;
void printSymbolTable(TableSymbole* table);
void printAllLocalSymbolTables(Node *node);


int identifierDeclared(const char *name, const TableSymbole *local, const TableSymbole *global);
void verifyIdentifiers(Node *node, const TableSymbole *global, const TableSymbole *local);
#endif //COMPILATEUR_H 
*/


