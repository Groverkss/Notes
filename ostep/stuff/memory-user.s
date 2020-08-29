	.file	"memory-user.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB50:
	.cfi_startproc
	endbr64
	cmpl	$2, %edi
	je	.L12
	movl	$1, %eax
	ret
.L12:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	$10, %edx
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	pushq	%rax
	.cfi_def_cfa_offset 32
	movq	8(%rsi), %rdi
	xorl	%esi, %esi
	call	strtol@PLT
	movl	%eax, %ebx
	sall	$20, %ebx
	movslq	%ebx, %rdi
	call	malloc@PLT
	movq	%rax, %rbp
	call	getpid@PLT
	leaq	.LC0(%rip), %rsi
	movl	$1, %edi
	movl	%eax, %edx
	xorl	%eax, %eax
	call	__printf_chk@PLT
	leal	-1(%rbx), %eax
	leaq	1(%rbp,%rax), %rdx
	.p2align 4,,10
	.p2align 3
.L5:
	testl	%ebx, %ebx
	jle	.L5
	movq	%rbp, %rax
.L4:
	movb	$0, (%rax)
	addq	$1, %rax
	cmpq	%rdx, %rax
	jne	.L4
	jmp	.L5
	.cfi_endproc
.LFE50:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.3.0-10ubuntu2) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
