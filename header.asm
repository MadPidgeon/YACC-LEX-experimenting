section .text
_3: ; print
	mov rax, [rsp+8]
	lea rsi, [rax+8]
	mov rdx, [rax]
	mov rax, 1
	mov rdi, 1
	syscall
	ret 8
global _start
_start:
global start
start:
global main
main: