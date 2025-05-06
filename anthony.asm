global _start
section .bss
globals: resb 0

section .text

_start:
	call	main
	mov 	rdi, rax
	mov 	rax, 60
	syscall

; function add
add:
	; save stack return address
	push	rbp
	mov 	rbp, rsp

	; push parameters on the stack
	push	rdi
	push	rsi

	; allocate memory for local variables
	sub 	rsp, 8

	; function's body

	; accessing to 'a' in parameters
	push	qword [rbp - 8]

	; accessing to 'b' in parameters
	push	qword [rbp - 16]

	; binary operator (+)
	pop 	rcx
	pop 	rax
	add 	rax, rcx
	push	rax

	; return value loading
	pop 	rax

	; stack alignement before exiting the function
	mov 	rsp, rbp
	pop 	rbp
	ret

	; stack alignement before exiting the function
	mov 	rsp, rbp
	pop 	rbp
	ret

; function text
text:
	; save stack return address
	push	rbp
	mov 	rbp, rsp

	; push parameters on the stack
	push	rdi

	; allocate memory for local variables
	sub 	rsp, 8

	; function's body

	; accessing to 't' in parameters
	push	qword [rbp - 8]

	; return value loading
	pop 	rax

	; stack alignement before exiting the function
	mov 	rsp, rbp
	pop 	rbp
	ret

	; stack alignement before exiting the function
	mov 	rsp, rbp
	pop 	rbp
	ret

; function print
print:
	; save stack return address
	push	rbp
	mov 	rbp, rsp

	; push parameters on the stack
	push	rdi

	; allocate memory for local variables
	sub 	rsp, 8

	; function's body

	; stack alignement before exiting the function
	mov 	rsp, rbp
	pop 	rbp
	ret

; function main
main:
	; save stack return address
	push	rbp
	mov 	rbp, rsp

	; push parameters on the stack

	; allocate memory for local variables
	sub 	rsp, 8

	; function's body

	; pushing integer
	push	0

	; return value loading
	pop 	rax

	; stack alignement before exiting the function
	mov 	rsp, rbp
	pop 	rbp
	ret

	; stack alignement before exiting the function
	mov 	rsp, rbp
	pop 	rbp
	ret
