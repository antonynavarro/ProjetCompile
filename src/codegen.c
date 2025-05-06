// src/codegen.c
#include "codegen.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void emitNASMHeader(FILE *out, TableSymbole *globals) {
    fprintf(out, "section .bss\n");
    for (int i = 0; i < globals->count; i++) {
        Symbole *s = &globals->symb[i];
        if (strcmp(s->type, "function") == 0) continue;
        if (strcmp(s->type, "char") == 0)
            fprintf(out, "%s: resb 1\n", s->ident);
        else
            fprintf(out, "%s: resq 1\n", s->ident);
    }

    fprintf(out,
        "\nsection .text\n"
        "    global _start\n"
        "_start:\n"
        "    call main\n"
        "    mov rax, 60\n"
        "    xor rdi, rdi\n"
        "    syscall\n\n");
}

void generateNASM(Node *node, TableSymbole *globals, FILE *out) {
    static int depth = 0;
    //static bool rightmost[128];

    if (!node) return;

    // DEBUG VISUEL
    // for (int i = 1; i < depth; i++)
    //     printf(rightmost[i] ? "    " : "│   ");
    // if (depth > 0)
    //     printf(rightmost[depth] ? "└── " : "├── ");
    // printf("Processing node: %s\n", node->label);

    // === Début de fonction ===
    if (strcmp(node->label, "Function") == 0) {
        const char *fn_name = SECONDCHILD(node->firstChild)->label;
        fprintf(out, "%s:\n", fn_name);

        // Préambule standard
        fprintf(out,
            "    ; save stack return address\n"
            "    push rbp\n"
            "    mov rbp, rsp\n");

        // Gestion des paramètres (rdi, rsi, etc.)
        Node *paramNode = node->firstChild->firstChild->nextSibling->nextSibling;
        int param_offset = -8;
        if (paramNode && strcmp(paramNode->label, "Parameter") == 0) {
            int regIndex = 0;
            const char *paramRegs[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
            for (Node *typeNode = paramNode->firstChild; typeNode; typeNode = typeNode->nextSibling) {
                if (typeNode->firstChild && regIndex < 6) {
                    fprintf(out, "    ; push parameter %s\n", typeNode->firstChild->label);
                    fprintf(out, "    push %s\n", paramRegs[regIndex++]);
                    param_offset -= 8;
                }
            }
        }

        // Réserve de l’espace local minimal
        fprintf(out, "    sub rsp, 8\n\n");

        // Poursuit le corps de la fonction
        generateNASM(node->firstChild->nextSibling, globals, out);

        // Épilogue par défaut (utile pour fonctions vides)
        fprintf(out,
            "    ; stack alignement before exiting the function\n"
            "    mov rsp, rbp\n"
            "    pop rbp\n"
            "    ret\n\n");
        return;
    }

    // === return Exp ===
    if (strcmp(node->label, "Return") == 0 && node->firstChild) {
        generateNASM(node->firstChild, globals, out);
        fprintf(out,
            "    ; return value loading\n"
            "    pop rax\n"
            "    mov rsp, rbp\n"
            "    pop rbp\n"
            "    ret\n");
        return;
    }

    // === Constante littérale ===
    if (isdigit(node->label[0]) || (node->label[0] == '-' && isdigit(node->label[1]))) {
        fprintf(out,
            "    ; literal value\n"
            "    push %s\n", node->label);
        return;
    }

    // === Opérations Add/Sub ===
    if (strcmp(node->label, "AddSub") == 0) {
        generateNASM(node->firstChild, globals, out);
        generateNASM(node->firstChild->nextSibling, globals, out);
        if (strcmp(node->value, "+") == 0) {
            fprintf(out,
                "    pop rcx\n"
                "    pop rax\n"
                "    add rax, rcx\n"
                "    push rax\n");
        } else {
            fprintf(out,
                "    pop rcx\n"
                "    pop rax\n"
                "    sub rax, rcx\n"
                "    push rax\n");
        }
        return;
    }

    // === Accès variable paramètre locale (sur la pile) ===
    if (node->firstChild == NULL && isalpha(node->label[0])) {
        fprintf(out,
            "    ; accessing to '%s'\n"
            "    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres\n"
            "    push rax\n",
            node->label);
        return;
    }

    // Appel récursif
    depth++;
    for (Node *child = node->firstChild; child; child = child->nextSibling) {
        //rightmost[depth] = (child->nextSibling == NULL);
        generateNASM(child, globals, out);
    }
    depth--;
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