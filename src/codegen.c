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
    /* Écrit l’en‑tête requis pour l’assembleur NASM */
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
 * Génère le code NASM pour l'AST.
 * globals : table globale
 * locals  : table locale actuelle (mise à jour au niveau Function)
 */
/*
 * Génère le code NASM pour l'AST.
 * - globals : table globale
 * - locals  : table locale actuelle (mise à jour au niveau Function)
 */
void generateNASM(Node *node,
                  TableSymbole *globals,
                  TableSymbole *locals,
                  FILE *out) {
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

    // ── Appels prédéfinis (I/O) ──────────────────────────
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

    // ── Affectation : Eq (lhs = rhs) ───────────────────────
if (!strcmp(node->label, "Eq")) {
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
                fprintf(out,
                    "    mov  [rbp%+d], rax    ; %s = getint()\n\n",
                    sym->address, sym->ident);
                return;
            }
            if (!strcmp(fs->ident, "getchar")) {
                fputs("    call my_getchar\n", out);
                fprintf(out,
                    "    mov  [rbp%+d], rax    ; %s = getchar()\n\n",
                    sym->address, sym->ident);
                return;
            }

            // --- identity(a) ou tout appel à 1 argument
            if (!strcmp(fs->ident, "identity")) {
                // 1) évaluer a → résultat dans RAX
                generateNASM(rhs->firstChild->firstChild, globals, locals, out);
                // 2) passer a dans RDI
                fputs("    pop  rdi            ; préparer argument for identity\n", out);
                // 3) appeler
                fprintf(out, "    call identity\n");
                // 4) stocker dans lhs
                fprintf(out,
                    "    mov  [rbp%+d], rax    ; %s = identity(a)\n\n",
                    sym->address, sym->ident);
                return;
            }
        }
    }

    // Cas général : évaluer rhs → push, pop une fois + mov
    generateNASM(rhs, globals, locals, out);
    fputs("    pop  rax    ; valeur à affecter\n", out);
    fprintf(out,
        "    mov  [rbp%+d], rax    ; %s\n\n",
        sym->address, sym->ident);
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
        generateNASM(node->firstChild, globals, locals, out);
        generateNASM(node->firstChild->nextSibling,
                     globals, locals, out);
        fputs(
          "    pop  rcx\n"
          "    pop  rax\n", out);
        fputs(strcmp(node->value,"+")==0
              ? "    add  rax, rcx\n"
              : "    sub  rax, rcx\n",
              out);
        fputs("    push rax\n", out);
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

        /* a. empiler tous les arguments (gauche → droite) */
        int n = 0;
        Node *args = node->firstChild;                /* nœud Args      */
        for (Node *a = args ? args->firstChild : NULL; a; a = a->nextSibling) {
            generateNASM(a->firstChild, globals, locals, out);  /* push rax */
            ++n;
        }

        /* b. dépiler dans RDI, RSI, RDX, … (droite → gauche) */
        static const char *reg[] = {"rdi","rsi","rdx","rcx","r8","r9"};
        for (int i = n-1; i >= 0 && i < 6; --i)
            fprintf(out,"    pop  %s        ; arg%d pour %s\n", reg[i], i, sym->ident);
        if (n > 6)
            fprintf(stderr,"⚠️  plus de 6 arguments non gérés (fonction « %s »)\n", sym->ident);

        /* c. appel + empiler la valeur de retour */
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
    if (!strcmp(sym->type,"char")) {  /* char => zéro-extension */
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



    // ── Récursion par défaut ────────────────────────────────
    for (Node *c = node->firstChild; c; c = c->nextSibling) {
        generateNASM(c, globals, locals, out);
    }
}


void add_fun_asm(FILE *out){

    fputs(runtimeAsm, out);
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