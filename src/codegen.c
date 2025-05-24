// src/codegen.c
#include "codegen.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>


static const char *runtimeAsm =
"; ————————————————————————————————————————————————————————————\n"
"; void my_putchar(char c)\n"
";————————————————————————————————————————————————————————————\n"
"my_putchar:\n"
"    push rbp\n"
"    mov  rbp, rsp\n"
"    sub  rsp, 8                 ; garde l'alignement 16 octets\n"
"\n"
"    mov  byte [rsp], dil        ; place le caractère\n"
"    mov  rax, 1                 ; syscall : write\n"
"    mov  rdi, 1                 ; fd = stdout\n"
"    lea  rsi, [rsp]\n"
"    mov  rdx, 1\n"
"    syscall\n"
"\n"
"    leave\n"
"    ret\n"
"\n"
"; ————————————————————————————————————————————————————————————\n"
"; char my_getchar(void)\n"
";   AL (puis RAX) = octet lu ; exit 5 si EOF/erreur.\n"
";————————————————————————————————————————————————————————————\n"
"my_getchar:\n"
"    push rbp\n"
"    mov  rbp, rsp\n"
"    sub  rsp, 8                 ; tampon 1 octet, pile alignée\n"
"\n"
"    lea  rsi, [rsp]\n"
"    mov  rax, 0                 ; read\n"
"    mov  rdi, 0                 ; stdin\n"
"    mov  rdx, 1\n"
"    syscall\n"
"\n"
"    cmp  rax, 1\n"
"    jne  .io_err\n"
"\n"
"    mov  al, [rsp]\n"
"    leave\n"
"    ret\n"
"\n"
".io_err:\n"
"    mov  rdi, 5\n"
"    mov  rax, 60                ; exit(5)\n"
"    syscall\n"
"\n"
"; ————————————————————————————————————————————————————————————\n"
"; int my_getint(void)\n"
";   Lit  [+|-]?[0-9]+  suivi d'espace ou « \\n ».\n"
";   RAX = valeur signée  (64 bits, l'appelant tronquera si besoin).\n"
";————————————————————————————————————————————————————————————\n"
"my_getint:\n"
"    push rbp\n"
"    mov  rbp, rsp\n"
"    push rbx                     ; signe dans RBX\n"
"    sub  rsp, 16                 ; alignement + scratch\n"
"\n"
"    ; Initialisation\n"
"    xor  rbx, rbx                ; rbx = 0 (utilisé pour stocker le résultat)\n"
"    mov  r9, 10                  ; r9 = 10 (constante pour multiplication)\n"
"\n"
"    ; Lire le premier caractère\n"
"    call my_getchar              ; AL := premier char\n"
"\n"
"    ; Vérifier si c'est un signe\n"
"    cmp  al, '+'\n"
"    je   .read_first_digit\n"
"    cmp  al, '-'\n"
"    je   .negative\n"
"\n"
"    ; Sinon, on attend directement un chiffre\n"
"    cmp  al, '0'\n"
"    jb   .input_err\n"
"    cmp  al, '9'\n"
"    ja   .input_err\n"
"\n"
"    ; Traiter le premier chiffre\n"
"    sub  al, '0'                 ; Convertir ASCII -> valeur numérique\n"
"    movzx rbx, al                ; rbx = premier chiffre\n"
"    jmp  .read_next_digit\n"
"\n"
".read_first_digit:\n"
"    ; Après avoir lu un signe +, on lit le premier chiffre\n"
"    call my_getchar\n"
"    cmp  al, '0'\n"
"    jb   .input_err\n"
"    cmp  al, '9'\n"
"    ja   .input_err\n"
"    sub  al, '0'\n"
"    movzx rbx, al                ; rbx = premier chiffre\n"
"    jmp  .read_next_digit\n"
"\n"
".negative:\n"
"    ; Après avoir lu un signe -, on lit le premier chiffre\n"
"    call my_getchar\n"
"    cmp  al, '0'\n"
"    jb   .input_err\n"
"    cmp  al, '9'\n"
"    ja   .input_err\n"
"    sub  al, '0'\n"
"    movzx rbx, al                ; rbx = premier chiffre\n"
"    neg  rbx                     ; Appliquer le signe négatif\n"
"    jmp  .read_next_digit_negative\n"
"\n"
".read_next_digit:\n"
"    ; Lire le prochain caractère\n"
"    call my_getchar\n"
"    cmp  al, '0'\n"
"    jb   .end_of_number\n"
"    cmp  al, '9'\n"
"    ja   .end_of_number\n"
"    ; C'est un chiffre, l'ajouter au résultat\n"
"    sub  al, '0'                 ; Convertir ASCII -> valeur\n"
"    movzx rcx, al                ; rcx = chiffre\n"
"    ; Calculer résultat = résultat * 10 + chiffre\n"
"    imul rbx, r9                 ; rbx *= 10\n"
"    add  rbx, rcx                ; rbx += chiffre\n"
"    jmp  .read_next_digit\n"
"\n"
".read_next_digit_negative:\n"
"    ; Même chose pour négatif\n"
"    call my_getchar\n"
"    cmp  al, '0'\n"
"    jb   .end_of_number\n"
"    cmp  al, '9'\n"
"    ja   .end_of_number\n"
"    sub  al, '0'\n"
"    movzx rcx, al\n"
"    imul rbx, r9\n"
"    sub  rbx, rcx                ; rbx -= chiffre\n"
"    jmp  .read_next_digit_negative\n"
"\n"
".end_of_number:\n"
"    ; Validate terminator\n"
"    cmp  al, ' '\n"
"    je   .valid_terminator\n"
"    cmp  al, 10                  ; '\\n'\n"
"    je   .valid_terminator\n"
"    jmp  .input_err\n"
"\n"
".valid_terminator:\n"
"    mov  rax, rbx\n"
"    add  rsp, 16\n"
"    pop  rbx\n"
"    leave\n"
"    ret\n"
"\n"
".input_err:\n"
"    mov  rdi, 5                  ; Code d'erreur\n"
"    mov  rax, 60                 ; syscall exit\n"
"    syscall\n"
"\n"
"; ————————————————————————————————————————————————————————————\n"
"; void my_putint(int i)\n"
";   Affiche l'entier signé RDI sur stdout.\n"
";————————————————————————————————————————————————————————————\n"
"my_putint:\n"
"    push rbp\n"
"    mov  rbp, rsp\n"
"    push rbx                     ; RBX utilisé comme diviseur 10\n"
"    sub  rsp, 64                 ; buffer\n"
"\n"
"    lea  rsi, [rsp+63]           ; pointeur fin de buffer\n"
"    mov  rcx, 0                  ; compteur longueur\n"
"    mov  rax, rdi                ; valeur\n"
"\n"
"    cmp  rax, 0\n"
"    jne  .conv_start\n"
"    mov  byte [rsi], '0'\n"
"    inc  rcx\n"
"    jmp  .write\n"
"\n"
".conv_start:\n"
"    mov  rbx, 10\n"
"    mov  r8b, 0                  ; flag négatif ?\n"
"    cmp  rax, 0\n"
"    jge .conv_loop\n"
"    neg  rax\n"
"    mov  r8b, 1                  ; nombre négatif\n"
"\n"
".conv_loop:\n"
"    xor  rdx, rdx\n"
"    div  rbx                     ; RDX = digit\n"
"    add  dl, '0'\n"
"    dec  rsi\n"
"    mov  [rsi], dl\n"
"    inc  rcx\n"
"    test rax, rax\n"
"    jne  .conv_loop\n"
"\n"
"    cmp  r8b, 1\n"
"    jne  .write\n"
"    dec  rsi\n"
"    mov  byte [rsi], '-'\n"
"    inc  rcx\n"
"\n"
".write:\n"
"    mov  rax, 1                  ; write\n"
"    mov  rdi, 1                  ; stdout\n"
"    mov  rdx, rcx                ; len\n"
"    syscall\n"
"\n"
"    add  rsp, 64\n"
"    pop  rbx\n"
"    leave\n"
"    ret\n"
"\n"
;



/*
  Header NASM
*/
void emitNASMHeader(FILE *out, TableSymbole *globals) {
    fprintf(out, "section .data\n");
    for (int i = 0; i < globals->count; i++) {
        Symbole *s = &globals->symb[i];
        if (s->isFunction) continue;
        if (strcmp(s->type, "char") == 0)
            fprintf(out, "%s: db 0\n", s->ident);  
        else
            fprintf(out, "%s: dq 0\n", s->ident); 
    }
    
    fputs("section .text\n",             out);
    fputs("global _start\n",          out);
    fputs("global my_putchar\n",          out);
    fputs("global my_getchar\n",          out);
    fputs("global my_getint\n",           out);
    fputs("global my_putint\n\n",        out);

    fprintf(out,
        "\n_start:\n"
        "    call main\n"
        "    mov rax, 60\n"
        "    xor rdi, rdi\n"
        "    syscall\n\n");
}


/*
 * Recherche un symbole par nom dans la table (locale puis globale).
 */
Symbole* lookupSymbol2(TableSymbole *g,
                       TableSymbole *l,
                       const char *ident) {
    if (l) {
        for (int i = 0; i < l->count; i++)
            if (strcmp(l->symb[i].ident, ident) == 0)
                return &l->symb[i];
    }
    if (g) {
        for (int i = 0; i < g->count; i++)
            if (strcmp(g->symb[i].ident, ident) == 0)
                return &g->symb[i];
    }
    return NULL;
}

static int label_count = 0;


/*
 * Génère le code pour une comparaison, avec création d'étiquettes pour
 * les cas vrai et faux avec l'évaluation paresseuse.
 * Retourne les étiquettes générées via les pointeurs true_label et false_label.
 */
void generateCompare(Node *node, TableSymbole *globals, TableSymbole *locals, FILE *out, const char *true_label, const char *false_label) {
    
    if (!strcmp(node->label, "Eq") && node->value) {
        // Évaluer les deux expressions
        generateNASM(node->firstChild, globals, locals, out);
        generateNASM(node->firstChild->nextSibling, globals, locals, out);
        
        // Extraire les valeurs pour la comparaison
        fputs("    pop  rcx    ; deuxième opérande\n", out);
        fputs("    pop  rax    ; premier opérande\n", out);
        fputs("    cmp  rax, rcx\n", out);
        
        // Brancher selon l'opérateur
        if (!strcmp(node->value, "==")) {
            fprintf(out, "    je   %s\n", true_label);
            fprintf(out, "    jmp  %s\n", false_label);
        } else if (!strcmp(node->value, "!=")) {
            fprintf(out, "    jne  %s\n", true_label);
            fprintf(out, "    jmp  %s\n", false_label);
        }
    } 
    else if (!strcmp(node->label, "Order") && node->value) {
        // Évaluer les deux expressions
        generateNASM(node->firstChild, globals, locals, out);
        generateNASM(node->firstChild->nextSibling, globals, locals, out);
        
        // Extraire les valeurs pour la comparaison
        fputs("    pop  rcx    ; deuxième opérande\n", out);
        fputs("    pop  rax    ; premier opérande\n", out);
        fputs("    cmp  rax, rcx\n", out);
        
        // Brancher selon l'opérateur
        if (!strcmp(node->value, "<")) {
            fprintf(out, "    jl   %s\n", true_label);
            fprintf(out, "    jmp  %s\n", false_label);
        } else if (!strcmp(node->value, ">")) {
            fprintf(out, "    jg   %s\n", true_label);
            fprintf(out, "    jmp  %s\n", false_label);
        } else if (!strcmp(node->value, "<=")) {
            fprintf(out, "    jle  %s\n", true_label);
            fprintf(out, "    jmp  %s\n", false_label);
        } else if (!strcmp(node->value, ">=")) {
            fprintf(out, "    jge  %s\n", true_label);
            fprintf(out, "    jmp  %s\n", false_label);
        }
    }
    else if (!strcmp(node->label, "Not")) {
        // inverser les étiquettes true et false
        generateCompare(node->firstChild, globals, locals, out, false_label, true_label);
    }
    else if (!strcmp(node->label, "And")) {
        int label_id = label_count++;
        char left_true[20];
        sprintf(left_true, ".L_and_next_%d", label_id);
        
        // Evaluation du premier opérande
        generateCompare(node->firstChild, globals, locals, out, left_true, false_label);
        
        // etiquette pour le deuxième opérande
        fprintf(out, "%s:\n", left_true);
        
        // Evaluation du deuxième opérande
        generateCompare(node->firstChild->nextSibling, globals, locals, out, true_label, false_label);
    }
    else if (!strcmp(node->label, "Or")) {
        // evalue le premier opérande d'abord
        int label_id = label_count++;
        char left_false[20];
        sprintf(left_false, ".L_or_next_%d", label_id);
        
        // Evaluation du premier opérande
        generateCompare(node->firstChild, globals, locals, out, true_label, left_false);
        
        // etiquette pour le deuxième opérande
        fprintf(out, "%s:\n", left_false);
        
        // Evaluation du deuxième opérande
        generateCompare(node->firstChild->nextSibling, globals, locals, out, true_label, false_label);
    }
    else if (!strcmp(node->label, "Ident")) {
        // Évaluer l'expression
        generateNASM(node, globals, locals, out);
        
        // Extraire la valeur et faire la comparaison avec zéro
        fputs("    pop  rax\n", out);
        fputs("    cmp  rax, 0\n", out);
        
        // Sauter si non-zéro (considéré comme vrai)
        fprintf(out, "    jnz  %s\n", true_label);
        fprintf(out, "    jmp  %s\n", false_label);
    }
    else if (!strcmp(node->label, "DivStar")) {
        // Évaluer l'expression arithmétique
        generateNASM(node, globals, locals, out);
        
        // Interpréter le résultat comme une valeur booléenne (non-zéro = vrai)
        fputs("    pop  rax    ; résultat expression arithmétique\n", out);
        fputs("    cmp  rax, 0  ; comparer avec zéro\n", out);
        
        // Sauter si non-zéro (considéré comme vrai)
        fprintf(out, "    jnz  %s    ; valeur différente de zéro = vrai\n", true_label);
        fprintf(out, "    jmp  %s    ; valeur égale à zéro = faux\n", false_label);
    }
    else if (!strcmp(node->label, "AddSub") || !strcmp(node->label, "MulDiv") || 
             !strcmp(node->label, "ModDiv")) {
        // Évaluer l'expression arithmétique
        generateNASM(node, globals, locals, out);
        
        // Interpréter le résultat comme une valeur booléenne (non-zéro = vrai)
        fputs("    pop  rax    ; résultat expression arithmétique\n", out);
        fputs("    cmp  rax, 0  ; comparer avec zéro\n", out);
        
        // Sauter si non-zéro (considéré comme vrai)
        fprintf(out, "    jnz  %s    ; valeur différente de zéro = vrai\n", true_label);
        fprintf(out, "    jmp  %s    ; valeur égale à zéro = faux\n", false_label);
    }
    else {
        // Cas par défaut: évaluer comme valeur booléenne
        generateNASM(node, globals, locals, out);
        
        // Comparer avec zéro
        fputs("    pop  rax\n", out);
        fputs("    cmp  rax, 0\n", out);
        
        // Sauter si non-zéro (considéré comme vrai)
        fprintf(out, "    jnz  %s\n", true_label);
        fprintf(out, "    jmp  %s\n", false_label);
    }
}

/*
 * Génère le code NASM pour l'AST.
 * globals : table globale
 * locals  : table locale actuelle
 */
void generateNASM(Node *node,TableSymbole *globals,TableSymbole *locals,FILE *out) {
    if (!node) return;

    // sauter les nœuds de déclaration pure
    if (!strcmp(node->label,"Global") ||
        !strcmp(node->label,"Vars")   ||
        !strcmp(node->label,"Declaration")) {
        return;
    }

    if (!strcmp(node->label,"Function")) {
        const char *fn = SECONDCHILD(node->firstChild)->label;
        // prologue
        fprintf(out,
            "%s:\n"
            "    push rbp\n"
            "    mov  rbp, rsp\n",
            fn);

        // calcul de la taille de frame (16-alignée)
        int frame = 0;
        locals = node->localTable;
        if (locals) {
            for (int i = 0; i < locals->count; i++) {
                Symbole *s = &locals->symb[i];
                if (!s->isFunction) {
                    int top = -s->address;
                    if (top > frame) frame = top;
                }
            }
            frame = (frame + 15) & ~15;
        }
        if (frame > 0) {
            fprintf(out,
                "    sub  rsp, %d   ; réserve %d octets\n\n",
                frame, frame);
        } else {
            fputs("\n", out);
        }

        // Ajout de code pour stocker les paramètres de fonction depuis les registres vers la pile
        if (locals) {
            // Identifions d'abord les paramètres de la fonction
            Node *params = THIRDCHILD(node->firstChild);
            if (params) {
                // Tableau des registres pour les paramètres selon la convention System V
                static const char *reg[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
                int paramIndex = 0;
                
                // Parcourir les paramètres et générer le code pour les stocker
                for (Node *param = params->firstChild; param && paramIndex < 6; param = param->nextSibling) {
                    // Ignorer le param void
                    if (!strcmp(param->label, "void")) {
                        continue;
                    }
                    
                    // Pour chaque paramètre, cherchons le symbole correspondant
                    Node *paramIdent = param->firstChild;
                    if (paramIdent) {
                        for (int i = 0; i < locals->count; i++) {
                            Symbole *s = &locals->symb[i];
                            // Vérifier si le symbole correspond au paramètre
                            if (!s->isFunction && 
                                s->scope == LOCAL && 
                                strcmp(s->ident, paramIdent->value) == 0) {
                                
                                fprintf(out,
                                    "    mov  [rbp%+d], %s    ; stocke le paramètre '%s' depuis %s\n",
                                    s->address, reg[paramIndex], s->ident, reg[paramIndex]);
                                break;
                            }
                        }
                    }
                    paramIndex++;
                }
                if (paramIndex > 0) {
                    fputs("\n", out); // Ajouter une ligne vide après les stockages de paramètres
                }
            }
        }

        // corps
        generateNASM(node->firstChild->nextSibling,
                     globals, locals, out);

        // épilogue si dernier nœud != Return
        {
            Node *suite = node->firstChild->nextSibling;
            Node *last  = NULL;
            if (suite && !strcmp(suite->label,"Suite_instr")) {
                for (Node *c = suite->firstChild; c; c = c->nextSibling)
                    last = c;
            }
            if (!last || strcmp(last->label,"Return") != 0) {
                fputs(
                  "    mov  rsp, rbp\n"
                  "    pop  rbp\n"
                  "    ret\n\n", out);
            }
        }
        return;
    }

    // ── While ────────────────────────────────────────
    if (!strcmp(node->label, "While")) {
        // Générer des étiquettes uniques pour le début et la fin de la boucle
        int while_id = label_count++;
        char start_label[20], body_label[20], end_label[20];
        sprintf(start_label, ".L_while_start_%d", while_id);
        sprintf(body_label, ".L_while_body_%d", while_id);  // Nouvelle étiquette pour le corps
        sprintf(end_label, ".L_while_end_%d", while_id);
        
        // Étiquette de début de boucle (évaluation de la condition)
        fprintf(out, "%s:  ; début de la boucle while (évaluation condition)\n", start_label);
        
        // Générer le code pour la condition
        // Si la condition est vraie -> aller au corps de la boucle
        // Si la condition est fausse -> sortir de la boucle
        Node *condition = node->firstChild;
        generateCompare(condition, globals, locals, out, body_label, end_label);
        
        // Étiquette pour le corps de la boucle
        fprintf(out, "%s:  ; corps de la boucle while\n", body_label);
        
        // Code pour le corps de la boucle
        Node *body = node->firstChild->nextSibling;
        if (body) {
            generateNASM(body, globals, locals, out);
        }
        
        // Retour au début de la boucle pour réévaluer la condition
        fprintf(out, "    jmp %s  ; retour à l'évaluation de la condition\n", start_label);
        
        // Étiquette de fin de boucle
        fprintf(out, "%s:  ; fin de la boucle while\n", end_label);
        return;
    }

    // ── If-Then-Else ────────────────────────────────────
    if (!strcmp(node->label, "If")) {
        // Générer des étiquettes uniques
        int if_id = label_count++;
        char true_label[20], false_label[20], end_label[20];
        sprintf(true_label, ".L_then_%d", if_id);
        sprintf(false_label, ".L_else_%d", if_id);
        sprintf(end_label, ".L_endif_%d", if_id);
        
        // Générer le code pour la condition avec les étiquettes
        Node *condition = node->firstChild;
        generateCompare(condition, globals, locals, out, true_label, false_label);
        
        // Code pour le bloc 'then'
        fprintf(out, "%s:  ; début bloc then\n", true_label);
        Node *thenNode = node->firstChild->nextSibling;
        if (thenNode) {
            generateNASM(thenNode, globals, locals, out);
        } else {
            // Si pas de bloc 'then' direct, chercher un nœud Then suivant
            Node *nextNode = node->nextSibling;
            if (nextNode && !strcmp(nextNode->label, "Then")) {
                generateNASM(nextNode->firstChild, globals, locals, out);
                // Marquer le nœud Then comme traité
                node->nextSibling = nextNode->nextSibling;
            }
        }
        fprintf(out, "    jmp %s  ; sortie de if\n", end_label);
        
        // Code pour le bloc 'else' s'il existe
        fprintf(out, "%s:  ; début bloc else\n", false_label);
        Node *elseNode = node->nextSibling;
        if (elseNode && !strcmp(elseNode->label, "Else")) {
            generateNASM(elseNode->firstChild, globals, locals, out);
            // On a traité le nœud Else, on le marque comme traité
            node->nextSibling = elseNode->nextSibling;
        }
        
        // Étiquette de fin
        fprintf(out, "%s:  ; fin if-else\n", end_label);
        return;
    }

    // ── Else (déjà traité par If) ────────────────────────
    if (!strcmp(node->label, "Else")) {
        return;
    }

    // ── Then (bloc de code du if) ────────────────────────
    if (!strcmp(node->label, "Then")) {
        return;
    }

    // ── Suite_instr (bloc d'instructions) ────────────────
    if (!strcmp(node->label, "Suite_instr")) {
        // Traiter toutes les instructions dans le bloc
        for (Node *instr = node->firstChild; instr; instr = instr->nextSibling) {
            generateNASM(instr, globals, locals, out);
        }
        return;
    }

    // ── Appels prédéfinis──────────────────────────
    if (node->value) {
        if (!strcmp(node->value,"getint")) {
            fputs(
              "    call my_getint\n"
              "    push rax         ; résultat getint\n",
              out);
            return;
        }
        if (!strcmp(node->value,"getchar")) {
            fputs(
              "    call my_getchar\n"
              "    push rax         ; résultat getchar\n",
              out);
            return;
        }
        if (!strcmp(node->value,"putint") && node->firstChild) {
            generateNASM(node->firstChild->firstChild,
                         globals, locals, out);
            fputs(
              "    pop  rdi\n"
              "    call my_putint\n",
              out);
            return;
        }
        if (!strcmp(node->value,"putchar") && node->firstChild) {
            generateNASM(node->firstChild->firstChild,
                         globals, locals, out);
            fputs(
              "    pop  rdi\n"
              "    call my_putchar\n",
              out);
            return;
        }
    }

    // ── Return Exp ───────────────────────────────────────
    if (!strcmp(node->label,"Return") && node->firstChild) {
        generateNASM(node->firstChild, globals, locals, out);
        fputs(
          "    pop  rax    ; return value\n"
          "    mov  rsp, rbp\n"
          "    pop  rbp\n"
          "    ret\n",
          out);
        return;
    }

    // ── Affectation : Eq  ───────────────────────
    if (!strcmp(node->label, "Eq") && !node->value) {
        Node *lhs = node->firstChild;
        Node *rhs = lhs->nextSibling;
        Symbole *sym = lookupSymbol2(globals, locals, lhs->value);
        if (!sym) {
            fprintf(stderr, "Erreur: variable '%s' non trouvée\n", lhs->value);
            return;
        }

        // Cas spécial : rhs est un Ident qui désigne une fonction prédéfinie
        if (!strcmp(rhs->label, "Ident")) {
            Symbole *fs = lookupSymbol2(globals, locals, rhs->value);
            if (fs && fs->isFunction) {
                // --- getint() ou getchar() à zéro argument
                if (!strcmp(fs->ident, "getint")) {
                    fputs("    call my_getint\n", out);
                    if (sym->scope == GLOBAL) {
                        fprintf(out, "    mov  [%s], rax    ; %s = getint()\n\n", 
                               sym->ident, sym->ident);
                    } else {
                        fprintf(out,
                            "    mov  [rbp%+d], rax    ; %s = getint()\n\n",
                            sym->address, sym->ident);
                    }
                    return;
                }
                if (!strcmp(fs->ident, "getchar")) {
                    fputs("    call my_getchar\n", out);
                    if (sym->scope == GLOBAL) {
                        fprintf(out, "    mov  [%s], rax    ; %s = getchar()\n\n", 
                               sym->ident, sym->ident);
                    } else {
                        fprintf(out,
                            "    mov  [rbp%+d], rax    ; %s = getchar()\n\n",
                            sym->address, sym->ident);
                    }
                    return;
                }

                // --- identity(a) ou tout appel à 1 argument
                if (!strcmp(fs->ident, "identity")) {
                    
                    generateNASM(rhs->firstChild->firstChild, globals, locals, out);
                    
                    fputs("    pop  rdi            ; préparer argument for identity\n", out);
                   
                    fprintf(out, "    call identity\n");
                    
                    if (sym->scope == GLOBAL) {
                        fprintf(out, "    mov  [%s], rax    ; %s = identity(a)\n\n", 
                               sym->ident, sym->ident);
                    } else {
                        fprintf(out,
                            "    mov  [rbp%+d], rax    ; %s = identity(a)\n\n",
                            sym->address, sym->ident);
                    }
                    return;
                }
            }
        }

        // Cas général 
        generateNASM(rhs, globals, locals, out);
        fputs("    pop  rax    ; valeur à affecter\n", out);
        
        // Vérifier si c'est une variable globale ou locale
        if (sym->scope == GLOBAL) {
            if (!strcmp(sym->type, "char")) {
                fprintf(out, "    mov  byte [%s], al  ; %s (global char)\n\n", 
                       sym->ident, sym->ident);
            } else {
                fprintf(out, "    mov  [%s], rax    ; %s (global)\n\n", 
                       sym->ident, sym->ident);
            }
        } else {
            fprintf(out, "    mov  [rbp%+d], rax    ; %s (local)\n\n",
                   sym->address, sym->ident);
        }
        return;
    }
    
    // ── Comparaison et opérations logiques ────────────────
    // Cas spécial pour les nœuds de comparaison et opérations logiques utilisés hors If
    if ((!strcmp(node->label, "Eq") && node->value) || 
        !strcmp(node->label, "Order") || 
        !strcmp(node->label, "And") || 
        !strcmp(node->label, "Or") ||
        !strcmp(node->label, "Not")) {
        // Générer des étiquettes uniques pour pousser 1 (vrai) ou 0 (faux) sur la pile
        int cmp_id = label_count++;
        char true_label[20], false_label[20], end_label[20];
        sprintf(true_label, ".L_cmp_true_%d", cmp_id);
        sprintf(false_label, ".L_cmp_false_%d", cmp_id);
        sprintf(end_label, ".L_cmp_end_%d", cmp_id);
        
        // Générer le code pour l'évaluation de la condition
        generateCompare(node, globals, locals, out, true_label, false_label);
        
        // Cas où la condition est vraie
        fprintf(out, "%s:\n", true_label);
        fprintf(out, "    push 1    ; condition vraie\n");
        fprintf(out, "    jmp  %s\n", end_label);
        
        // Cas où la condition est fausse
        fprintf(out, "%s:\n", false_label);
        fprintf(out, "    push 0    ; condition fausse\n");
        
        // Fin de l'évaluation
        fprintf(out, "%s:\n", end_label);
        return;
    }

    if (!strcmp(node->label, "Not")) {
        // Évaluer l'expression à nier
        generateNASM(node->firstChild, globals, locals, out);
        
        // Nier le résultat
        fputs("    pop  rax    ; valeur à nier\n", out);
        fputs("    test rax, rax\n", out);
        fputs("    setz al     ; al = (rax == 0) ? 1 : 0\n", out);
        fputs("    movzx rax, al\n", out);
        fputs("    push rax    ; !expression\n", out);
        return;
    }

    // ── Littéral entier ───────────────────────────────────
    if (isdigit(node->label[0]) ||
       (node->label[0]=='-' && isdigit(node->label[1]))) {
        fprintf(out,"    push %s    ; literal\n", node->label);
        return;
    }

    // ── Add/Sub ───────────────────────────────────────────
    if (!strcmp(node->label,"AddSub")) {
        // Cas spécial pour l'opération unaire (négation)
        if (!node->firstChild->nextSibling) {
            // Opération unaire (comme -1)
            generateNASM(node->firstChild, globals, locals, out);
            if (strcmp(node->value, "-") == 0) {
                fputs("    pop  rax    ; opérande\n", out);
                fputs("    neg  rax    ; négation\n", out);
                fputs("    push rax    ; résultat de l'opération\n", out);
            }
            return;
        }
        
        // Opération binaire normale
        generateNASM(node->firstChild, globals, locals, out);
        generateNASM(node->firstChild->nextSibling, globals, locals, out);
        
        // Les résultats sont sur la pile, les récupérer
        fputs("    pop  rcx    ; deuxième opérande\n", out);
        fputs("    pop  rax    ; premier opérande\n", out);
        
        // Exécuter l'opération
        if (strcmp(node->value,"+") == 0) {
            fputs("    add  rax, rcx    ; addition\n", out);
        } else if (strcmp(node->value,"-") == 0) {
            fputs("    sub  rax, rcx    ; soustraction\n", out);
        } else {
            fprintf(stderr, "Opération arithmétique non supportée: %s\n", node->value);
        }
        
        // Empiler le résultat
        fputs("    push rax    ; résultat de l'opération\n", out);
        return;
    }

    // ── MulDiv ───────────────────────────────────────────
    if (!strcmp(node->label,"MulDiv")) {
        // Évaluer les expressions des deux côtés
        generateNASM(node->firstChild, globals, locals, out);
        generateNASM(node->firstChild->nextSibling, globals, locals, out);
        
        // Les résultats sont sur la pile, les récupérer
        fputs("    pop  rcx    ; deuxième opérande\n", out);
        fputs("    pop  rax    ; premier opérande\n", out);
        
        // Exécuter l'opération
        if (strcmp(node->value,"*") == 0) {
            fputs("    imul rax, rcx    ; multiplication\n", out);
        } else if (strcmp(node->value,"/") == 0) {
            fputs("    cqo              ; étendre RAX en RDX:RAX\n", out);
            fputs("    idiv rcx         ; division (résultat dans RAX)\n", out);
        } else {
            fprintf(stderr, "Opération arithmétique non supportée: %s\n", node->value);
        }
        
        // Empiler le résultat
        fputs("    push rax    ; résultat de l'opération\n", out);
        return;
    }

    // ── DivStar operations (*, /, %) ─────────────────────
    if (!strcmp(node->label, "DivStar")) {
        // Évaluer les expressions des deux côtés
        generateNASM(node->firstChild, globals, locals, out);
        generateNASM(node->firstChild->nextSibling, globals, locals, out);
        
        // Les résultats sont sur la pile, les récupérer
        fputs("    pop  rcx    ; deuxième opérande (diviseur/multiplicateur)\n", out);
        fputs("    pop  rax    ; premier opérande (dividende/multiplicande)\n", out);
        
        // Exécuter l'opération selon l'opérateur
        if (!strcmp(node->value, "*")) {
            fputs("    imul rax, rcx    ; multiplication\n", out);
        } else if (!strcmp(node->value, "/")) {
            fputs("    cqo              ; étendre RAX en RDX:RAX\n", out);
            fputs("    idiv rcx         ; division (quotient dans RAX)\n", out);
        } else if (!strcmp(node->value, "%")) {
            fputs("    cqo              ; étendre RAX en RDX:RAX\n", out);
            fputs("    idiv rcx         ; division (quotient dans RAX, reste dans RDX)\n", out);
            fputs("    mov  rax, rdx    ; déplacer le reste dans RAX\n", out);
        } else {
            fprintf(stderr, "Opération DivStar non supportée: %s\n", node->value);
        }
        
        // Empiler le résultat
        fputs("    push rax    ; résultat de l'opération\n", out);
        return;
    }


    /* ── Ident : variable, globale ou appel fonction ─────────────── */
    if (!strcmp(node->label, "Ident")) {
        Symbole *sym = lookupSymbol2(globals, locals, node->value);
        if (!sym) {
            fprintf(stderr,"Erreur : symbole « %s » introuvable\n", node->value);
            return;
        }

        /* ---------- 1. Appel de fonction ---------- */
        if (sym->isFunction) {
            /* a. Compter et collecter les arguments */
            int n = 0;
            Node **argNodes = NULL;
            Node *args = node->firstChild;                /* nœud Args */
            
            if (args) {
                /* Compter le nombre d'arguments */
                for (Node *a = args->firstChild; a; a = a->nextSibling) {
                    n++;
                }
                
                /* Collecter les nœuds d'arguments dans un tableau */
                if (n > 0) {
                    argNodes = (Node **)malloc(n * sizeof(Node *));
                    if (!argNodes) {
                        fprintf(stderr, "Erreur d'allocation mémoire\n");
                        return;
                    }
                    
                    int i = 0;
                    for (Node *a = args->firstChild; a; a = a->nextSibling) {
                        argNodes[i++] = a;
                    }
                }
            }
            
            /* b. Traiter les arguments dans l'ordre inverse pour la convention d'appel */
            static const char *reg[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
            
            /* Génération du code pour les arguments dans l'ordre inverse */
            for (int i = n - 1; i >= 0; i--) {
                Node *argNode = argNodes[i];
                
                /* Cas 1: Argument est un littéral numérique */
                if (isdigit(argNode->label[0]) || 
                    (argNode->label[0] == '-' && isdigit(argNode->label[1]))) {
                    fprintf(out, "    push %s    ; argument littéral\n", argNode->label);
                }
                /* Cas 2: Argument est un identificateur ou autre nœud */
                else {
                    generateNASM(argNode, globals, locals, out);
                }
            }
            
            /* Récupération des arguments dans les registres appropriés */
            for (int i = 0; i < n && i < 6; i++) {
                fprintf(out, "    pop  %s        ; arg%d pour %s\n", reg[i], i, sym->ident);
            }
            
            if (n > 6) {
                fprintf(stderr, "plus de 6 arguments non gérés (fonction « %s »)\n", sym->ident);
            }
            
            /* Libérer la mémoire utilisée pour les nœuds d'arguments */
            if (argNodes) {
                free(argNodes);
            }
            
            /* c. Appel de la fonction + empiler la valeur de retour */
            fprintf(out,
                "    call %s\n"
                "    push rax           ; résultat %s\n",
                sym->ident, sym->ident);
            return;
        }

        /* ---------- 2. Variable locale (ou paramètre) ---------- */
        if (sym->scope == LOCAL) {
            if (sym->address >= 0) {      /* paramètre */
                fprintf(out,
                  "    mov  rax, [rbp+%d]    ; param %s\n"
                  "    push rax\n",
                  sym->address, sym->ident);
            } else {                      /* variable locale */
                fprintf(out,
                  "    mov  rax, [rbp%d]     ; var %s\n"
                  "    push rax\n",
                  sym->address, sym->ident);
            }
            return;
        }

        /* ---------- 3. Variable globale ---------- */
        if (!strcmp(sym->type,"char")) {  
            fprintf(out,
              "    movzx rax, byte [%s] ; global %s (char)\n"
              "    push rax\n",
              sym->ident, sym->ident);
        } else {
            fprintf(out,
              "    mov  rax, [%s]      ; global %s\n"
              "    push rax\n",
              sym->ident, sym->ident);
        }
        return;
    }

    // Récursion par défaut 
    for (Node *c = node->firstChild; c; c = c->nextSibling) {
        generateNASM(c, globals, locals, out);
    }
}

// Ajoute les fonctions predefini en NASM
void add_fun_asm(FILE *out){

    fputs(runtimeAsm, out);
}

