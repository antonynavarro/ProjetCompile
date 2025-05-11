; fun.asm – minimal I/O routines sans libc
; Plate‑forme : Linux x86‑64, assembleur NASM, convention System V AMD64 ABI
; Implémente :
;   char  my_getchar(void);
;   void  my_putchar(char c);
;   int   my_getint(void);
;   void  my_putint(int i);
;
; Les quatre symboles sont exportés pour un appel depuis du C.
; Chaque fonction respecte l'ABI : pile 16‑octets alignée avant tout CALL,
; RBX (registre callee‑saved) est sauvegardé/restauré dès qu'il est modifié.
;
; my_getchar – lit 1 octet sur stdin, retourne AL étendu en RAX ; sinon exit 5.
; my_putchar – écrit le caractère DIL sur stdout.
; my_getint  – lit « [+|-]?[0‑9]+ », séparé par espace ou saut de ligne, retourne RAX.
; my_putint  – affiche l'entier signé passé dans RDI.
;--------------------------------------------------------------------------

section .text

; ————————————————————————————————————————————————————————————
; exports

global my_putchar
global my_getchar
global my_getint
global my_putint

; ————————————————————————————————————————————————————————————
; void my_putchar(char c)
;————————————————————————————————————————————————————————————
my_putchar:
    push rbp
    mov  rbp, rsp
    sub  rsp, 8                 ; garde l'alignement 16 octets

    mov  byte [rsp], dil        ; place le caractère
    mov  rax, 1                 ; syscall : write
    mov  rdi, 1                 ; fd = stdout
    lea  rsi, [rsp]
    mov  rdx, 1
    syscall

    leave
    ret

; ————————————————————————————————————————————————————————————
; char my_getchar(void)
;   AL (puis RAX) = octet lu ; exit 5 si EOF/erreur.
;————————————————————————————————————————————————————————————
my_getchar:
    push rbp
    mov  rbp, rsp
    sub  rsp, 8                 ; tampon 1 octet, pile alignée

    lea  rsi, [rsp]
    mov  rax, 0                 ; read
    mov  rdi, 0                 ; stdin
    mov  rdx, 1
    syscall

    cmp  rax, 1
    jne  .io_err

    mov  al, [rsp]
    leave
    ret

.io_err:
    mov  rdi, 5
    mov  rax, 60                ; exit(5)
    syscall

; ————————————————————————————————————————————————————————————
; int my_getint(void)
;   Lit  [+|-]?[0‑9]+  suivi d'espace ou « \n ».
;   RAX = valeur signée  (64 bits, l'appelant tronquera si besoin).
;————————————————————————————————————————————————————————————
my_getint:
    push rbp
    mov  rbp, rsp
    push rbx                     ; signe dans RBX
    sub  rsp, 16                 ; alignement + scratch

    ; Initialisation
    xor  rbx, rbx                ; rbx = 0 (utilisé pour stocker le résultat)
    mov  r9, 10                  ; r9 = 10 (constante pour multiplication)

    ; Lire le premier caractère
    call my_getchar              ; AL := premier char

    ; Vérifier si c'est un signe
    cmp  al, '+'
    je   .read_first_digit
    cmp  al, '-'
    je   .negative

    ; Sinon, on attend directement un chiffre
    cmp  al, '0'
    jb   .input_err
    cmp  al, '9'
    ja   .input_err

    ; Traiter le premier chiffre
    sub  al, '0'                 ; Convertir ASCII -> valeur numérique
    movzx rbx, al                ; rbx = premier chiffre
    jmp  .read_next_digit

.read_first_digit:
    ; Après avoir lu un signe +, on lit le premier chiffre
    call my_getchar
    cmp  al, '0'
    jb   .input_err
    cmp  al, '9'
    ja   .input_err
    sub  al, '0'
    movzx rbx, al                ; rbx = premier chiffre
    jmp  .read_next_digit

.negative:
    ; Après avoir lu un signe -, on lit le premier chiffre
    call my_getchar
    cmp  al, '0'
    jb   .input_err
    cmp  al, '9'
    ja   .input_err
    sub  al, '0'
    movzx rbx, al                ; rbx = premier chiffre
    neg  rbx                     ; Appliquer le signe négatif
    jmp  .read_next_digit_negative

.read_next_digit:
    ; Lire le prochain caractère
    call my_getchar
    
    ; Vérifier si c'est un chiffre
    cmp  al, '0'
    jb   .end_of_number
    cmp  al, '9'
    ja   .end_of_number
    
    ; C'est un chiffre, l'ajouter au résultat
    sub  al, '0'                 ; Convertir ASCII -> valeur
    movzx rcx, al                ; rcx = chiffre
    
    ; Calculer résultat = résultat * 10 + chiffre
    imul rbx, r9                 ; rbx *= 10
    add  rbx, rcx                ; rbx += chiffre
    
    jmp  .read_next_digit

.read_next_digit_negative:
    ; Lire le prochain caractère
    call my_getchar
    
    ; Vérifier si c'est un chiffre
    cmp  al, '0'
    jb   .end_of_number
    cmp  al, '9'
    ja   .end_of_number
    
    ; C'est un chiffre, l'ajouter au résultat (négatif)
    sub  al, '0'                 ; Convertir ASCII -> valeur
    movzx rcx, al                ; rcx = chiffre
    
    ; Calculer résultat = résultat * 10 - chiffre (pour les nombres négatifs)
    imul rbx, r9                 ; rbx *= 10
    sub  rbx, rcx                ; rbx -= chiffre pour conserver le signe négatif
    
    jmp  .read_next_digit_negative

.end_of_number:
    ; Le caractère n'est pas un chiffre, vérifier qu'il est valide comme terminateur
    cmp  al, ' '
    je   .valid_terminator
    cmp  al, 10                  ; '\n'
    je   .valid_terminator
    jmp  .input_err              ; Caractère non valide

.valid_terminator:
    ; Mettre le résultat final dans rax et retourner
    mov  rax, rbx
    add  rsp, 16
    pop  rbx
    leave
    ret

.input_err:
    ; Erreur: caractère invalide
    mov  rdi, 5                  ; Code d'erreur
    mov  rax, 60                 ; syscall exit
    syscall

; ————————————————————————————————————————————————————————————
; void my_putint(int i)
;   Affiche l'entier signé RDI sur stdout.
;————————————————————————————————————————————————————————————
my_putint:
    push rbp
    mov  rbp, rsp
    push rbx                     ; RBX utilisé comme diviseur 10
    sub  rsp, 64                 ; buffer

    lea  rsi, [rsp+63]           ; pointeur fin de buffer (on remplit en arrière)
    mov  rcx, 0                  ; compteur longueur
    mov  rax, rdi                ; valeur

    ; gérer zéro directement
    cmp  rax, 0
    jne  .conv_start
    mov  byte [rsi], '0'
    inc  rcx
    jmp  .write

.conv_start:
    mov  rbx, 10
    mov  r8b, 0                  ; flag négatif ?
    cmp  rax, 0
    jge .conv_loop
    neg  rax
    mov  r8b, 1                  ; nombre négatif

.conv_loop:
    xor  rdx, rdx
    div  rbx                     ; RDX = digit, RAX = reste
    add  dl, '0'
    dec  rsi
    mov  [rsi], dl
    inc  rcx
    test rax, rax
    jne  .conv_loop

    ; préfixer '-' si besoin
    cmp  r8b, 1
    jne  .write
    dec  rsi
    mov  byte [rsi], '-'
    inc  rcx

.write:
    mov  rax, 1                  ; write
    mov  rdi, 1                  ; stdout
    mov  rdx, rcx                ; len
    syscall

    add  rsp, 64
    pop  rbx
    leave
    ret
