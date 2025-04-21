#include "codegen.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void generateNASM(Node *node, TableSymbole *globals, FILE *out) {
    static int depth = 0;
    static bool rightmost[128];

    if (!node) return;

    // Affichage pour debug
    for (int i = 1; i < depth; i++) {
        printf(rightmost[i] ? "    " : "│   ");
    }
    if (depth > 0) {
        printf(rightmost[depth] ? "└── " : "├── ");
    }
    printf("Processing node: %s\n", node->label);


    // Pour les condition if, else ...
    if (tryEmitGlobalConstructs(node, globals, out)) return;

    // Accès aux variables globales
    if (node->label && isalpha(node->label[0]) && identExiste(globals, node->label)) {
        fprintf(out,
                "    mov rax, [%s]\n"
                "    push rax\n",
                node->label);
        return;
    }

    // Constantes numériques
    if (node->label && (isdigit(node->label[0]) || (node->label[0] == '-' && isdigit(node->label[1])))) {
        fprintf(out, "    push %s\n", node->label);
        return;
    }

    // Opérations AddSub
    if (node->label && strcmp(node->label, "AddSub") == 0) {
        if (node->firstChild)
            generateNASM(node->firstChild, globals, out);
        if (node->firstChild && node->firstChild->nextSibling)
            generateNASM(node->firstChild->nextSibling, globals, out);

        if (node->value && strcmp(node->value, "+") == 0) {
            fprintf(out, "    pop rbx\n    pop rax\n    add rax, rbx\n    push rax\n");
        } else {
            fprintf(out, "    pop rbx\n    pop rax\n    sub rax, rbx\n    push rax\n");
        }
        return;
    }

    // Comparaisons Eq
    if (node->label && strcmp(node->label, "Eq") == 0) {
        if (node->firstChild)
            generateNASM(node->firstChild, globals, out);
        if (node->firstChild && node->firstChild->nextSibling)
            generateNASM(node->firstChild->nextSibling, globals, out);

        if (node->value && strcmp(node->value, "==") == 0) {
            fprintf(out, "    pop rbx\n    pop rax\n    cmp rax, rbx\n    sete al\n    movzx rax, al\n    push rax\n");
        } else if (node->value && strcmp(node->value, "!=") == 0) {
            fprintf(out, "    pop rbx\n    pop rax\n    cmp rax, rbx\n    setne al\n    movzx rax, al\n    push rax\n");
        }
        return;
    }

    // Parcours récursif des enfants
    depth++;
    for (Node *child = node->firstChild; child; child = child->nextSibling) {
        rightmost[depth] = (child->nextSibling == NULL);
        generateNASM(child, globals, out);
    }
    depth--;
}

int tryEmitGlobalConstructs(Node *node,TableSymbole *globals,FILE *out){
// 1) Affectation à une variable globale : LHS = RHS
if (strcmp(node->label, "Eq")==0 && !node->value
&& node->firstChild && node->firstChild->nextSibling
&& isalpha(node->firstChild->label[0])
&& identExiste(globals, node->firstChild->label))
{
Node *lhs =       node->firstChild;
Node *rhs = lhs->nextSibling;
// Génère le RHS
generateNASM(rhs, globals, out);
fprintf(out, "    pop rax\n");
// Stocke en mémoire globale
fprintf(out, "    mov [%s], rax\n", lhs->label);
return 1;
}

// 2) if (var_globale) then ...
if (strcmp(node->label, "If")==0 && node->firstChild
&& isalpha(node->firstChild->label[0])
&& identExiste(globals, node->firstChild->label))
{
static int if_id = 0;
int id = if_id++;
// charge et teste
fprintf(out, "    mov rax, [%s]\n", node->firstChild->label);
fprintf(out, "    test rax, rax\n");
fprintf(out, "    jz else_%d\n", id);

// bloc then
generateNASM(node->firstChild->nextSibling, globals, out);

// bloc else ?
Node *maybeElse = node->firstChild->nextSibling->nextSibling;
if (maybeElse) {
fprintf(out, "    jmp endif_%d\n", id);
fprintf(out, "else_%d:\n", id);
// le fils unique de ce noeud Else est son firstChild
generateNASM(maybeElse->firstChild, globals, out);
}
fprintf(out, "endif_%d:\n", id);
return 1;
}

return 0;
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