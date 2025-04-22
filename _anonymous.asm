section .bss
FOO: resq 1    ; at offset 0
BAR: resb 1    ; at offset 8
MIN: resq 1    ; at offset 9
MAX: resq 1    ; at offset 17
FILE: resb 1    ; at offset 25
TEXT: resb 1    ; at offset 26

section .text
    global _start
_start:
    mov rax, [FOO]
    push rax
    mov rax, [BAR]
    push rax
    mov rax, [MIN]
    push rax
    mov rax, [MAX]
    push rax
    mov rax, [FILE]
    push rax
    mov rax, [TEXT]
    push rax
    mov rax, [main]
    push rax
    mov rax, 60
    xor rdi, rdi
    syscall
