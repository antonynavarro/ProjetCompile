#ifndef SEMANTIQUE_H
#define SEMANTIQUE_H

#include "tree.h"
#include "tabsymbole.h"

extern int sem_error;

void verifyIdentifiers(Node*, const TableSymbole*, const TableSymbole*);
const char *exprType(Node*, const TableSymbole*, const TableSymbole*);
void checkAssignments(Node*, const TableSymbole*, const TableSymbole*);

void verifyReturns(Node *node, const TableSymbole *global);
void verifyMainExists(const TableSymbole *global) ;

#endif
