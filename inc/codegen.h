#ifndef CODEGEN_H
#define CODEGEN_H

#include "tree.h"
#include "tabsymbole.h"

void generateNASM(Node *node,TableSymbole *globals,TableSymbole *locals,FILE *out);
int tryEmitGlobalConstructs(Node *node,TableSymbole *globals,FILE *out);
void emitNASMHeader(FILE *out, TableSymbole *globals);
void add_fun_asm(FILE *out);
//void translate(Node *root,const TableSymbole *global);

#endif
