section .text
    global _start
_start:
 push 12
 push 12
 pop rbx
 pop rax
 mov [rax], rbx
 push rbx
    mov rax, 60
    xor rdi, rdi
    syscall
