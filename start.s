	.set EXIT, 60

	.text
	.globl	_start
_start:
	mov	(%rsp), %rdi
	mov	%rsp, %rsi
	add	$8, %rsi
	call	main

	mov	%rax, %rdi
	mov	$EXIT, %rax
	syscall

