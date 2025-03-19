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

/* Generate NASM Assembly from the syntax tree */
void generateNASM(Node *node, FILE *out) {
    if (!node) return;
    
    // Process numeric values
    if (isdigit(node->label[0]) || (node->label[0] == '-' && isdigit(node->label[1]))) {
        fprintf(out, " push %s\n", node->label);
        return;
    }
    
    // Process operations
    if (strcmp(node->label, "AddSub") == 0) {
        // Process children first
        generateNASM(node->firstChild, out);
        generateNASM(node->firstChild->nextSibling, out);
        
        // Check value to determine operation
        if (node->value && strcmp(node->value, "+") == 0) {
            fprintf(out, " pop rbx\n pop rax\n add rax, rbx\n push rax\n");
        } else {
            fprintf(out, " pop rbx\n pop rax\n sub rax, rbx\n push rax\n");
        }
        return;
    }
    
    if (strcmp(node->label, "DivStar") == 0) {
        // Process children first
        generateNASM(node->firstChild, out);
        generateNASM(node->firstChild->nextSibling, out);
        
        // Check value to determine operation
        if (node->value && strcmp(node->value, "*") == 0) {
            fprintf(out, " pop rbx\n pop rax\n imul rax, rbx\n push rax\n");
        } else {
            fprintf(out, " pop rbx\n pop rax\n xor rdx, rdx\n idiv rbx\n push rax\n");
        }
        return;
    }
    
    if (strcmp(node->label, "Or") == 0) {
        // Process children first
        generateNASM(node->firstChild, out);
        generateNASM(node->firstChild->nextSibling, out);
        
        fprintf(out, " pop rbx\n pop rax\n or rax, rbx\n push rax\n");
        return;
    }
    
    if (strcmp(node->label, "And") == 0) {
        // Process children first
        generateNASM(node->firstChild, out);
        generateNASM(node->firstChild->nextSibling, out);
        
        fprintf(out, " pop rbx\n pop rax\n and rax, rbx\n push rax\n");
        return;
    }
    
    if (strcmp(node->label, "Eq") == 0) {
        // Process children first
        generateNASM(node->firstChild, out);
        generateNASM(node->firstChild->nextSibling, out);
        
        // Check if it's a comparison or assignment
        if (node->value) {
            // It's a comparison
            if (strcmp(node->value, "==") == 0) {
                fprintf(out, " pop rbx\n pop rax\n cmp rax, rbx\n sete al\n movzx rax, al\n push rax\n");
            } else if (strcmp(node->value, "!=") == 0) {
                fprintf(out, " pop rbx\n pop rax\n cmp rax, rbx\n setne al\n movzx rax, al\n push rax\n");
            }
        } else {
            // It's an assignment
            fprintf(out, " pop rbx\n pop rax\n mov [rax], rbx\n push rbx\n");
        }
        return;
    }
    
    if (strcmp(node->label, "Order") == 0) {
        // Process children first
        generateNASM(node->firstChild, out);
        generateNASM(node->firstChild->nextSibling, out);
        
        if (node->value) {
            if (strcmp(node->value, "<") == 0) {
                fprintf(out, " pop rbx\n pop rax\n cmp rax, rbx\n setl al\n movzx rax, al\n push rax\n");
            } else if (strcmp(node->value, ">") == 0) {
                fprintf(out, " pop rbx\n pop rax\n cmp rax, rbx\n setg al\n movzx rax, al\n push rax\n");
            } else if (strcmp(node->value, "<=") == 0) {
                fprintf(out, " pop rbx\n pop rax\n cmp rax, rbx\n setle al\n movzx rax, al\n push rax\n");
            } else if (strcmp(node->value, ">=") == 0) {
                fprintf(out, " pop rbx\n pop rax\n cmp rax, rbx\n setge al\n movzx rax, al\n push rax\n");
            }
        }
        return;
    }
    
    // Process identifiers
    if (strcmp(node->label, "IDENT") == 0) {
        // Load identifier address
        fprintf(out, " lea rax, [%s]\n push rax\n", node->value);
        return;
    }
    
    // Process function calls
    if (node->firstChild && strcmp(node->firstChild->label, "Args") == 0) {
        // Process arguments
        generateNASM(node->firstChild, out);
        
        // Call function
        fprintf(out, " call %s\n", node->label);
        
        // Clean up stack and push return value
        fprintf(out, " push rax\n");
        return;
    }
    
    // Process control flow
    if (strcmp(node->label, "If") == 0) {
        static int if_count = 0;
        int current_if = if_count++;
        
        // Generate condition code
        generateNASM(node->firstChild, out);
        
        fprintf(out, " pop rax\n test rax, rax\n jz else_%d\n", current_if);
        
        // Generate "then" block
        generateNASM(node->firstChild->nextSibling, out);
        fprintf(out, " jmp endif_%d\n", current_if);
        
        fprintf(out, "else_%d:\n", current_if);
        
        // Check for "else" block
        if (node->nextSibling && strcmp(node->nextSibling->label, "Else") == 0) {
            generateNASM(node->nextSibling->firstChild, out);
        }
        
        fprintf(out, "endif_%d:\n", current_if);
        return;
    }
    
    if (strcmp(node->label, "While") == 0) {
        static int while_count = 0;
        int current_while = while_count++;
        
        fprintf(out, "while_%d:\n", current_while);
        
        // Generate condition code
        generateNASM(node->firstChild, out);
        
        fprintf(out, " pop rax\n test rax, rax\n jz endwhile_%d\n", current_while);
        
        // Generate loop body
        generateNASM(node->firstChild->nextSibling, out);
        
        fprintf(out, " jmp while_%d\n", current_while);
        fprintf(out, "endwhile_%d:\n", current_while);
        return;
    }
    
    if (strcmp(node->label, "Return") == 0) {
        // If returning a value
        if (node->firstChild) {
            generateNASM(node->firstChild, out);
            fprintf(out, " pop rax\n");
        }
        
        fprintf(out, " ret\n");
        return;
    }
    
    // Continue with children
    generateNASM(node->firstChild, out);
    generateNASM(node->nextSibling, out);
}

/* Translate AST to NASM */
void translate(Node* root) {
    FILE *out = fopen("_anonymous.asm", "w");
    if (!out) {
        perror("Error opening _anonymous.asm");
        exit(EXIT_FAILURE);
    }
    fprintf(out, "section .text\n    global _start\n_start:\n");
    generateNASM(root, out);
    fprintf(out, "    mov rax, 60\n    xor rdi, rdi\n    syscall\n");
    fclose(out);
}
