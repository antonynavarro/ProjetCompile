/* compilateur.c */

#include "compilateur.h"
#include "tree.h"
#include <ctype.h>

// Appelle toutes les fonctions necessaire pour la semantique et la traduction en nasm
void translate(Node* root, bool show) {
	
    TableSymbole globalTable = {.count = 0};

    addSymbol(&globalTable, "getchar", "char", GLOBAL, 0, 1, 0, 0);
    addSymbol(&globalTable, "putchar", "void", GLOBAL, 0, 1, 0, 0);
    addSymbol(&globalTable, "getint", "int", GLOBAL, 0, 1, 0, 0);
    addSymbol(&globalTable, "putint", "void", GLOBAL, 0, 1, 0, 0);

    //table des variables globales
    generateGlobalSymbolTable(root, &globalTable);
    if (show) {printSymbolTable(&globalTable);}

    // tables des fonctions et vars locals
    generateLocalSymbolTable(root);
    if (show) {printAllLocalSymbolTables(root);}

    verifyIdentifiers(root, &globalTable, NULL);
    //verifyFunctions(root, root, &globalTable, NULL);
    verifyMainExists(&globalTable);
    verifyReturns(root, &globalTable);
    checkAssignments(root, &globalTable, NULL);


    if (sem_error == 2) {
        fprintf(stderr, "Compilation failed due to semantic errors.\n");
        return;
    }


    FILE *out = fopen("_anonymous.asm", "w");
    if (!out) {
        perror("Error opening _anonymous.asm");
        exit(EXIT_FAILURE);
    }

    emitNASMHeader(out, &globalTable);
    generateNASM(root, &globalTable,NULL, out);
    add_fun_asm(out);
    //executer code nasm avec : nasm -felf64 _anonymous.asm && ld _anonymous.o -o prog && ./prog

    

    fclose(out);
}
