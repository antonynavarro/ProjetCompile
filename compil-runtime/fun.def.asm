section .text
global my_putchar
global my_getchar
global my_getint
global my_putint

;--------------------------------------------------------------
; void my_putchar(char c)
;--------------------------------------------------------------
my_putchar:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16          ; réserve 16 octets (alignement & tampon)

    mov  byte [rsp], dil  ; place le caractère dans le tampon

    mov  rax, 1           ; syscall: write
    mov  rdi, 1           ; fd = stdout
    lea  rsi, [rsp]       ; adresse du tampon
    mov  rdx, 1           ; longueur
    syscall

    leave                 ; restaure RBP et remet la pile
    ret

;--------------------------------------------------------------
; char my_getchar(void)
;         → AL (zéro‑ext. vers RAX) contient l’octet lu
;         En cas d’erreur/EOF: exit 5
;--------------------------------------------------------------
my_getchar:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    lea  rsi, [rbp-1]     ; buffer d’un octet
    mov  rax, 0           ; syscall: read
    mov  rdi, 0           ; fd = stdin
    mov  rdx, 1           ; 1 octet
    syscall
    cmp  rax, 1
    jne  .io_error        ; si EOF ou erreur
    mov  al,  [rbp-1]     ; caractère lu
    leave
    ret
.io_error:
    mov  rdi, 5           ; code 5
    mov  rax, 60          ; syscall exit
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

    xor  rax, rax                ; résultat
    mov  rbx, 1                  ; signe = +1

    ; Ignorer les caractères non numériques jusqu'au premier signe ou chiffre
.skip_non_numeric:
    call my_getchar              ; AL := caractère
    
    cmp  al, '-'
    je   .process_negative
    cmp  al, '+'
    je   .process_positive
    
    ; Vérifier si c'est un chiffre
    cmp  al, '0'
    jb   .skip_non_numeric       ; Si < '0', continuer à ignorer
    cmp  al, '9'
    ja   .skip_non_numeric       ; Si > '9', continuer à ignorer
    
    ; C'est un chiffre, commencer à traiter
    sub  al, '0'
    movzx rax, al                ; résultat = 1er chiffre
    jmp  .read_digits

.process_negative:
    mov  rbx, -1                 ; signe négatif
    jmp  .read_first_digit

.process_positive:
    mov  rbx, 1                  ; signe positif (déjà initialisé, mais pour clarté)
    ; continuer pour lire le premier chiffre

.read_first_digit:
    call my_getchar              ; lire le premier chiffre
    
    cmp  al, '0'
    jb   .input_err
    cmp  al, '9'
    ja   .input_err
    
    sub  al, '0'
    movzx rax, al                ; résultat = 1er chiffre

.read_digits:
    call my_getchar
    
    cmp  al, '0'
    jb   .check_term
    cmp  al, '9'
    ja   .check_term
    
    sub  al, '0'
    movzx rcx, al
    imul rax, rax, 10
    add  rax, rcx
    jmp  .read_digits

.check_term:
    cmp  al, ' '
    je   .apply_sign
    cmp  al, 10                  ; '\n'
    je   .apply_sign
    
    ; Si ce n'est ni un espace ni un saut de ligne, on a une entrée invalide
    jmp  .input_err

.input_err:
    mov  rdi, 5
    mov  rax, 60                 ; exit(5)
    syscall

.apply_sign:
    imul rax, rbx                ; applique le signe
    add  rsp, 16
    pop  rbx
    leave
    ret

;--------------------------------------------------------------
; void my_putint(int i)
; Imprime l’entier signé passé dans RDI sur stdout.
;--------------------------------------------------------------
my_putint:
    push rbp
    mov  rbp, rsp
    push rbx              ; on utilisera RBX pour diviseur 10
    sub  rsp, 64          ; buffer local suffisant pour 20 chiffres + signe

    mov  rax, rdi         ; valeur à convertir
    lea  rsi, [rbp-1]     ; pointeur de construction (on remplit à l’envers)
    mov  rcx, 0           ; nombre de caractères

    ; cas spécial zéro
    cmp  rax, 0
    jne  .conv_start
    mov  byte [rsi], '0'
    inc  rcx
    jmp  .write_digits

.conv_start:
    mov  rbx, 10
    ; gérer le signe négatif : on mettra '-' plus tard
    cmp  rax, 0
    jge  .conv_loop
    neg  rax
    mov  byte [rbp-64], '-' ; stocke '-' tout au début du buffer réservé
    mov  rdx, 0
    mov  rcx, 1             ; comptera le signe aussi plus tard
    jmp  .conv_loop

.conv_loop:
    xor  rdx, rdx
    div  rbx               ; RDX = digit, RAX = reste
    add  dl, '0'
    mov  [rsi], dl         ; stocke le chiffre
    dec  rsi
    inc  rcx
    test rax, rax
    jne  .conv_loop

    ; si on avait un signe négatif enregistré :
    cmp  byte [rbp-64], '-'
    jne  .write_digits
    inc  rcx               ; compte le caractère '-'
    mov  [rsi], byte '-'

.write_digits:
    ; RDI = stdout, RSI pointe sur premier caractère à écrire, RCX = len
    inc  rsi               ; on s’est arrêté un caractère trop loin
    mov  rax, 1            ; syscall write
    mov  rdi, 1
    mov  rdx, rcx
    syscall

    add  rsp, 64
    pop  rbx
    leave
    ret

