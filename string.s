	.set STDOUT, 1
	.set WRITE, 1

	.text
	.globl	strlen
strlen:
	xor	%rax, %rax

loop:
	cmpb	$0, (%rdi)
	je	end

	add	$1, %rdi
	add	$1, %rax

	jmp	loop

end:
	ret

	.globl	print
print:
	push	%rdi
	call	strlen
	pop	%rdi

	mov	%rdi, %rsi
	mov	%rax, %rdx
	mov	$STDOUT, %rdi
	mov	$WRITE, %rax
	syscall
	ret
