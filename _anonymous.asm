section .text
    global _start
_start:
 push 0
 pop rax
 ret
 push 10
 push 10
 pop rbx
 pop rax
 mov [rax], rbx
 push rbx
 push 10
 push 10
 pop rbx
 pop rax
 mov [rax], rbx
 push rbx
 push 10
 push 10
 pop rbx
 pop rax
 mov [rax], rbx
 push rbx
 pop rbx
 pop rax
 add rax, rbx
 push rax
 pop rax
 ret
 pop rax
 ret
 pop rbx
 pop rax
 cmp rax, rbx
 sete al
 movzx rax, al
 push rax
 push 0
 push 0
 pop rbx
 pop rax
 cmp rax, rbx
 sete al
 movzx rax, al
 push rax
 pop rbx
 pop rax
 and rax, rbx
 push rax
 pop rax
 test rax, rax
 jz else_0
 jmp endif_0
else_0:
endif_0:
    mov rax, 60
    xor rdi, rdi
    syscall
