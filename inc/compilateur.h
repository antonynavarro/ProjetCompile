#ifndef COMPILATEUR_H
#define COMPILATEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"


/* Définition des constantes */
#define MAX_SYMBOLES 100
#define MAX_FONCTIONS 100

/* Structures */
typedef struct {
    char * ident;
    char * type;
    char * value;
} Symbole;

typedef struct {
    Symbole symb[MAX_SYMBOLES];
    int count;
} TableSymbole;

typedef struct fonction{
    char * nom;
    TableSymbole table;
    Fonction * suivant;
} Fonction;


/* Prototypes des fonctions */
int identExiste(const TableSymbole* table, const char* ident);
void rempliTable(TableSymbole* table, const char* ident, const char* type);
void generateNASM(Node *node, FILE *out);
void translate(Node* root);

#endif /* COMPILATEUR_H */
