default rel
section .text
global _start
_start:
global start
start:
_0_: ; -NONE-
	mov r8, 0
	mov r9, r8
_0_1:
	mov r8, 10
	mov r10, r9
	cmp r10, r8
	setne r10b
	cmp r10, 0
	jne _0_2
	mov r8, 5
	mov r10, r9
	cmp r10, r8
	sete r10b
	cmp r10, 0
	jne _0_3
	mov r8, 2
	mov r10, r9
	add r10, r8
	mov r9, r10
_0_3:
	mov r8, 1
	mov r10, r9
	add r10, r8
	mov r9, r10
	jmp _0_1
_0_2:
	mov rax, 60
	mov rdi, 0
	syscall