/* compilateur.c */

#include "compilateur.h"
#include "tree.h"
#include <ctype.h>

#define MAX_SYMBOLES 100


/* Function Prototypes */
int identExiste(const TableSymbole* table, const char* ident);
void rempliTable(TableSymbole* table, const char* ident, const char* type);
void generateNASM(Node *node, FILE *out);
void translate(Node* root);

/* Check if an identifier exists in the symbol table */
int identExiste(const TableSymbole* table, const char* ident) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symb[i].ident, ident) == 0) {
            return 1;
        }
    }
    return 0;
}

/* Add an identifier to the symbol table */
void rempliTable(TableSymbole* table, const char* ident, const char* type) {
    if (identExiste(table, ident) || table->count >= MAX_SYMBOLES) {
        return;
    }
    strncpy(table->symb[table->count].ident, ident, sizeof(table->symb[table->count].ident) - 1);
    strncpy(table->symb[table->count].type, type, sizeof(table->symb[table->count].type) - 1);
    table->count++;
}

void generateNASM(Node *node, FILE *out) {
    static int depth = 0;  
    static bool rightmost[128];  

    if (!node) return; // Vérification de NULL

    // Affichage pour debug
    for (int i = 1; i < depth; i++) {
        printf(rightmost[i] ? "    " : "\u2502   ");
    }
    if (depth > 0) {
        printf(rightmost[depth] ? "\u2514\u2500\u2500 " : "\u251c\u2500\u2500 ");
    }
    printf("Processing node: %s\n", node->label);

    // Vérification avant d'accéder à node->label
    if (node->label && (isdigit(node->label[0]) || (node->label[0] == '-' && isdigit(node->label[1])))) {
        fprintf(out, " push %s\n", node->label);
        return;
    }

    // Vérification des opérations arithmétiques AddSub
    if (node->label && strcmp(node->label, "AddSub") == 0) {
        if (node->firstChild) generateNASM(node->firstChild, out);
        if (node->firstChild && node->firstChild->nextSibling) generateNASM(node->firstChild->nextSibling, out);

        if (node->value && strcmp(node->value, "+") == 0) {
            fprintf(out, " pop rbx\n pop rax\n add rax, rbx\n push rax\n");
        } else {
            fprintf(out, " pop rbx\n pop rax\n sub rax, rbx\n push rax\n");
        }
        return;
    }

    // Vérification de Eq
    if (node->label && strcmp(node->label, "Eq") == 0) {
        if (node->firstChild) generateNASM(node->firstChild, out);
        if (node->firstChild && node->firstChild->nextSibling) generateNASM(node->firstChild->nextSibling, out);

        if (node->value && strcmp(node->value, "==") == 0) {
            fprintf(out, " pop rbx\n pop rax\n cmp rax, rbx\n sete al\n movzx rax, al\n push rax\n");
        } else if (node->value && strcmp(node->value, "!=") == 0) {
            fprintf(out, " pop rbx\n pop rax\n cmp rax, rbx\n setne al\n movzx rax, al\n push rax\n");
        }
        return;
    }

    // Parcours récursif des enfants
    depth++;
    for (Node *child = node->firstChild; child != NULL; child = child->nextSibling) {
        if (depth < 128) // Vérifier pour éviter dépassement de tableau
            rightmost[depth] = (child->nextSibling == NULL);
        generateNASM(child, out);
    }
    depth--;
}

void translate(Node* root) {
    FILE *out = fopen("_anonymous.asm", "w"); // Correction du nom du fichier
    if (!out) {
        perror("Error opening _anonymous.asm");
        exit(EXIT_FAILURE);
    }
    fprintf(out, "section .text\n    global _start\n_start:\n");
    
    if (root) generateNASM(root, out); // Vérifie que root n'est pas NULL

    fprintf(out, "    mov rax, 60\n    xor rdi, rdi\n    syscall\n");
    fclose(out);
}

