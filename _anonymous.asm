section .bss

section .text
    global _start
_start:
    call main
    mov rax, 60
    xor rdi, rdi
    syscall

add:
    push rbp
    mov rbp, rsp
    push rdi    ; param a
    push rsi    ; param b
    sub rsp, 8

    mov rax, [rbp-8]    ; var Vars
    push rax
    mov rax, [rbp-8]    ; var a
    push rax
    mov rax, [rbp-8]    ; var b
    push rax
    pop rcx
    pop rax
    add rax, rcx
    push rax
    pop rax    ; return value
    mov rsp, rbp
    pop rbp
    ret
    mov rsp, rbp
    pop rbp
    ret

text:
    push rbp
    mov rbp, rsp
    push rdi    ; param t
    sub rsp, 8

    mov rax, [rbp-8]    ; var Vars
    push rax
    mov rax, [rbp-8]    ; var t
    push rax
    pop rax    ; return value
    mov rsp, rbp
    pop rbp
    ret
    mov rsp, rbp
    pop rbp
    ret

print:
    push rbp
    mov rbp, rsp
    push rdi    ; param s
    sub rsp, 8

    mov rax, [rbp-8]    ; var Vars
    push rax
    mov rax, [rbp-8]    ; var Suite_instr
    push rax
    mov rsp, rbp
    pop rbp
    ret

main:
    push rbp
    mov rbp, rsp
    sub rsp, 8

    mov rax, [rbp-8]    ; var Vars
    push rax
    push 0    ; literal
    pop rax    ; return value
    mov rsp, rbp
    pop rbp
    ret
    mov rsp, rbp
    pop rbp
    ret

