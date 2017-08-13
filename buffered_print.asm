; do I/O only with one core because of context switch cost
section .data
	output_buffer_size: resdq 1
	output_buffer_max equ 512
	output_buffer: resdw output_buffer_max

_3: ; print
	mov rax, [rsp+8]
	mov rdi, [rax]
	cmp rdi, output_buffer_max
	jge _print_long
	add rdi, [output_buffer_size]
	cmp rdi, output_buffer_max
	cmovge rdi, 512


	mov rdi, [output_buffer]
	add rdi, [rsp+8]
	cmp rdi, 

	lea rsi, [rax+8]

_print_long:
	mov rsi, output_buffer
	mov rdx, [output_buffer_size]
	mov rax, 1
	mov rdi, 1
	syscall
	mov rax, [rsp+8]
	lea rsi, [rax+8]
	mov rdx, [rax]
	mov rax, 1
	mov rdi, 1
	mov [output_buffer_size], 0
	syscall
	ret 8