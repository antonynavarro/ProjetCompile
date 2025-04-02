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
	
    TableSymbole globalTable = {.count = 0};
    TableSymbole localTable = {.count = 0};

    // Générer la table des variables globales
    generateGlobalSymbolTable(root, &globalTable);
    printSymbolTable(&globalTable);

    // Générer les tables des fonctions
    generateLocalSymbolTable(root, &localTable);
    printSymbolTable(&localTable);
    
    
    FILE *out = fopen("_anonymous.asm", "w");
    if (!out) {
        perror("Error opening _anonymous.asm");
        exit(EXIT_FAILURE);
    }
    fprintf(out, "section .text\n    global _start\n_start:\n");
    
    if (root) generateNASM(root, out);

    fprintf(out, "    mov rax, 60\n    xor rdi, rdi\n    syscall\n");
    fclose(out);
}



/* POUR LA TABLE DES SYMBOLES */
int identExiste(const TableSymbole* table, const char* ident) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symb[i].ident, ident) == 0) {
            return 1;
        }
    }
    return 0;
}


void addSymbol(TableSymbole* table, const char* ident, const char* type, Scope scope, void * address) {
    if (identExiste(table, ident) || table->count >= MAX_SYMBOLES) {
        return;
    }
    strncpy(table->symb[table->count].ident, ident, sizeof(table->symb[table->count].ident) - 1);
    strncpy(table->symb[table->count].type, type, sizeof(table->symb[table->count].type) - 1);
    table->symb[table->count].scope = scope;
    table->symb[table->count].address = address;
    table->count++;
}

void generateGlobalSymbolTable(Node *node, TableSymbole* table) {
    if (!node) return;

    // Vérifie si le nœud représente une déclaration globale
    if (strcmp(node->label, "Global") == 0) {
        Node *decl = node->firstChild;
        while (decl) {
            // Vérifie si le nœud est un type (int, char, etc.)
            if (strcmp(decl->label, "int") == 0 || strcmp(decl->label, "char") == 0) {
                // Parcourt les enfants pour trouver les identifiants associés
                Node *var = decl->firstChild;
                while (var) {
                    addSymbol(table, var->label, decl->label, GLOBAL, 0);  // Utilise le type du parent
                    var = var->nextSibling;
                }
            }
            decl = decl->nextSibling;
        }
    }

    // Parcours récursif des enfants
    generateGlobalSymbolTable(node->firstChild, table);
    generateGlobalSymbolTable(node->nextSibling, table);
}


void generateLocalSymbolTable(Node *node, TableSymbole* table) {
    if (!node) return;

    // Vérifie si le nœud représente une déclaration de fonction
    if (strcmp(node->label, "Function") == 0) {
        Node *head = node->firstChild;
        char *name = head->firstChild->nextSibling->label;
        Node *body = head->nextSibling;

        addSymbol(table, name, head->firstChild->label, LOCAL, head);  // Utilise le type du parent

        // Ajouter les paramètres
        Node *paramNode = head->firstChild->nextSibling->nextSibling;
        if (paramNode && strcmp(paramNode->label, "Parameter") == 0) {
            Node *param = paramNode->firstChild;
            while (param) {
                if (strcmp(param->label, "void") != 0){
                    addSymbol(table, name, param->label, LOCAL, param);
                }
                param = param->nextSibling;
            }
        }

        // Ajouter les variables locales
        if (body && strcmp(body->label, "Body") == 0) {
            Node *varNode = body->firstChild;
            while (varNode) {
                if (strcmp(varNode->label, "Vars") == 0) {
                    Node *var = varNode->firstChild;
                    while (var) {
                        Node *ident = var->firstChild;
                        while (ident) {
                            addSymbol(table, ident->label, var->label, LOCAL, &(ident->value));
                            ident = ident->nextSibling;
                        }
                        var = var->nextSibling;
                    }
                }
                varNode = varNode->nextSibling;
            }
        }
    }

    // Parcours récursif
    generateLocalSymbolTable(node->firstChild, table);
    generateLocalSymbolTable(node->nextSibling, table);
}


void printSymbolTable(TableSymbole* table) {
    printf("\nTable des Symboles:\n");
    for (int i = 0; i < table->count; i++) {
        printf("Nom: %s, Type: %s, Portée: %s, Adresse: %p\n", 
               table->symb[i].ident, 
               table->symb[i].type,
               (table->symb[i].scope == GLOBAL) ? "Global" : "Local",
               table->symb[i].address);
        printf("Valeur : %s\n", (char*)table->symb[i].address);
    }
}


