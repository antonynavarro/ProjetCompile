#ifndef CODEGEN_H
#define CODEGEN_H

#include "tree.h"
#include "tabsymbole.h"

void generateNASM(Node *node,TableSymbole *globals,FILE *out);
int tryEmitGlobalConstructs(Node *node,TableSymbole *globals,FILE *out);
//void translate(Node *root,const TableSymbole *global);

#endif
