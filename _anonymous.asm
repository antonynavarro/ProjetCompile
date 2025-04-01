section .text
    global _start
_start:
 push 2
 push 4
 pop rbx
 pop rax
 add rax, rbx
 push rax
    mov rax, 60
    xor rdi, rdi
    syscall
