#ifndef TABSYMBOLE_H
#define TABSYMBOLE_H

#include "tree.h"

#define MAX_SYMBOLES   100
#define MAX_IDENT_LEN   50

extern int sem_error;

typedef enum { GLOBAL, LOCAL } Scope;


typedef struct {
  char ident[MAX_IDENT_LEN];
  char type[50];                                           
  Scope scope;
  int address;   
  int isFunction;           
} Symbole;


typedef struct TableSymbole {
  Symbole      symb[MAX_SYMBOLES];
  int          count;
} TableSymbole;


void addFun(TableSymbole* table);
int  identExiste(const TableSymbole*, const char*);
void addSymbol(TableSymbole*, const char*, const char*, Scope, int address, int isFunction);
void generateGlobalSymbolTable(Node*, TableSymbole*);
void generateLocalSymbolTable(Node*, TableSymbole*);
void printSymbolTable(TableSymbole* table);
void printAllLocalSymbolTables(Node *node);

#endif
