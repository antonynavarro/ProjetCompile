section .bss
FOO: resq 1    ; at offset 0
BAR: resb 1    ; at offset 8
MIN: resq 1    ; at offset 9
MAX: resq 1    ; at offset 17
FILE: resb 1    ; at offset 25
TEXT: resb 1    ; at offset 26
vars: resq 1    ; at offset 0
add: resq 1    ; at offset 0

section .text
    global _start
_start:
    ; accessing to 'FOO'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'BAR'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'MIN'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'MAX'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'FILE'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'TEXT'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
vars:
    ; save stack return address
    push rbp
    mov rbp, rsp
    sub rsp, 8

    ; accessing to 'a'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'b'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'e'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'd'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'c'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'h'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'g'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'f'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'Suite_instr'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; stack alignement before exiting the function
    mov rsp, rbp
    pop rbp
    ret

add:
    ; save stack return address
    push rbp
    mov rbp, rsp
    ; push parameter a
    push rdi
    ; push parameter b
    push rsi
    sub rsp, 8

    ; accessing to 'Vars'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'a'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    ; accessing to 'b'
    mov rax, [rbp - 8]  ; à adapter selon l’ordre réel des paramètres
    push rax
    pop rcx
    pop rax
    add rax, rcx
    push rax
    ; return value loading
    pop rax
    mov rsp, rbp
    pop rbp
    ret
    ; stack alignement before exiting the function
    mov rsp, rbp
    pop rbp
    ret

    mov rax, 60
    xor rdi, rdi
    syscall
