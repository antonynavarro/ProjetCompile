// src/codegen.c
#include "codegen.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
  Émet le header NASM (bss + _start).
  À appeler une fois AVANT l’appel à generateNASM().
*/
void emitNASMHeader(FILE *out, TableSymbole *globals) {
    // .bss pour les globals
    fprintf(out, "section .bss\n");
    for (int i = 0; i < globals->count; i++) {
        Symbole *s = &globals->symb[i];
        if (s->isFunction) continue;
        if (strcmp(s->type, "char") == 0)
            fprintf(out, "%s: resb 1\n", s->ident);
        else
            fprintf(out, "%s: resq 1\n", s->ident);
    }
    // .text, point d’entrée
    fprintf(out,
        "\nsection .text\n"
        "    global _start\n"
        "_start:\n"
        "    call main\n"
        "    mov rax, 60\n"
        "    xor rdi, rdi\n"
        "    syscall\n\n");
}

/*
  generateNASM : génère le code d’un nœud AST.
  - fonctons : prologue / corps / épilogue
  - Return : pop rax + épilogue + ret
  - expressions : pile + opérateurs séparés
*/
void generateNASM(Node *node, TableSymbole *globals, FILE *out) {
    if (!node) return;

    // ── fonction ─────────────────────────────────────────────
    if (strcmp(node->label, "Function") == 0) {
        // nom et prologue
        const char *fn = SECONDCHILD(node->firstChild)->label;
        fprintf(out, "%s:\n", fn);
        fprintf(out,
            "    push rbp\n"
            "    mov rbp, rsp\n");

        // empiler paramètres (rdi, rsi, ...)
        {
            Node *p = node->firstChild->firstChild->nextSibling->nextSibling;
            const char *regs[] = {"rdi","rsi","rdx","rcx","r8","r9"};
            int i = 0;
            if (p && strcmp(p->label, "Parameter") == 0) {
                for (Node *t = p->firstChild; t; t = t->nextSibling) {
                    if (i < 6 && t->firstChild) {
                        fprintf(out, "    push %s    ; param %s\n",
                                regs[i], t->firstChild->label);
                        i++;
                    }
                }
            }
        }

        // réserver au moins 8 octets pour le spool local
        fprintf(out, "    sub rsp, 8\n\n");

        // générer le corps (Body)
        generateNASM(node->firstChild->nextSibling, globals, out);

        // épilogue par défaut (si pas de Return rencontré)
        fprintf(out,
            "    mov rsp, rbp\n"
            "    pop rbp\n"
            "    ret\n\n");
        return;
    }

    // ── Return Exp; ────────────────────────────────────────────
    if (strcmp(node->label, "Return") == 0 && node->firstChild) {
        // expr → pile
        generateNASM(node->firstChild, globals, out);
        // pop dans rax puis épilogue + ret
        fprintf(out,
            "    pop rax    ; return value\n"
            "    mov rsp, rbp\n"
            "    pop rbp\n"
            "    ret\n");
        return;
    }

    // ── littéral entier ───────────────────────────────────────
    if (isdigit(node->label[0]) ||
       (node->label[0]=='-' && isdigit(node->label[1]))) {
        fprintf(out,
            "    push %s    ; literal\n",
            node->label);
        return;
    }

    // ── Add/Sub ───────────────────────────────────────────────
    if (strcmp(node->label, "AddSub") == 0) {
        generateNASM(node->firstChild, globals, out);
        generateNASM(node->firstChild->nextSibling, globals, out);
        fprintf(out,
            "    pop rcx\n"
            "    pop rax\n");
        if (strcmp(node->value, "+") == 0)
            fprintf(out, "    add rax, rcx\n");
        else
            fprintf(out, "    sub rax, rcx\n");
        fprintf(out, "    push rax\n");
        return;
    }

    // ── accès variable locale/paramètre ───────────────────────
    if (node->firstChild == NULL && isalpha(node->label[0])) {
        // il faut calculer l'offset réel : ici on suppose 8 pour premier param
        // et 8 octets de réserve locale. À adapter avec votre layout !
        fprintf(out,
            "    mov rax, [rbp-8]    ; var %s\n"
            "    push rax\n",
            node->label);
        return;
    }

    // ── récursion ─────────────────────────────────────────────
    for (Node *c = node->firstChild; c; c = c->nextSibling)
        generateNASM(c, globals, out);
}




/*
void translate(Node* root) {
	
    TableSymbole globalTable = {.count = 0};
    //TableSymbole localTable = {.count = 0};

    // Générer la table des variables globales
    generateGlobalSymbolTable(root, &globalTable);
    printSymbolTable(&globalTable);

    // Générer les tables des fonctions
    generateLocalSymbolTable(root, NULL);
    //printSymbolTable(&localTable);

    //printSymbolTable(&globalTable);
    printAllLocalSymbolTables(root);

    
    verifyIdentifiers(root, &globalTable, NULL);

    // **Vérification de type**
    checkAssignments(root, &globalTable, NULL);


    // Ouverture du fichier de sortie
    FILE *out = fopen("_anonymous.asm", "w");
    if (!out) {
        perror("Error opening _anonymous.asm");
        exit(EXIT_FAILURE);
    }

    // Section BSS : on réserve un mot (8 octets) pour chaque int, un octet pour chaque char
    fprintf(out, "section .bss\n");
    for (int i = 0; i < globalTable.count; i++) {
        Symbole *s = &globalTable.symb[i];
        if (strcmp(s->type, "function") == 0) {
            // on ne réserve rien pour les fonctions 
            continue;
        }
        if (strcmp(s->type, "char") == 0) {
            fprintf(out, "%s: resb 1    ; at offset %d\n", s->ident, s->address);
        } else {  // on suppose int => 8 octets 
            fprintf(out, "%s: resq 1    ; at offset %d\n", s->ident, s->address);
        }
    }
    fprintf(out, "\n");

    // Section code 
    fprintf(out, "section .text\n"
                 "    global _start\n"
                 "_start:\n");

    // Génération du corps de _start (votre code)
    if (root) generateNASM(root, out);

    // Exit syscall
    fprintf(out,
            "    mov rax, 60\n"
            "    xor rdi, rdi\n"
            "    syscall\n");

    fclose(out);
}*/