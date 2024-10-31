	.file	"pip.c"
	.text
	.globl	pnpoly
	.type	pnpoly, @function
pnpoly:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	movq	%rdx, -40(%rbp)
	movss	%xmm0, -24(%rbp)
	movss	%xmm1, -44(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -12(%rbp)
	movl	-20(%rbp), %eax
	subl	$1, %eax
	movl	%eax, -8(%rbp)
	jmp	.L2
.L5:
	movl	-12(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movss	(%rax), %xmm0
	comiss	-44(%rbp), %xmm0
	seta	%cl
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movss	(%rax), %xmm0
	comiss	-44(%rbp), %xmm0
	seta	%al
	xorl	%ecx, %eax
	testb	%al, %al
	je	.L3
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movss	(%rax), %xmm0
	movl	-12(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movss	(%rax), %xmm2
	movaps	%xmm0, %xmm1
	subss	%xmm2, %xmm1
	movl	-12(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movss	(%rax), %xmm2
	movss	-44(%rbp), %xmm0
	subss	%xmm2, %xmm0
	mulss	%xmm1, %xmm0
	movl	-8(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movss	(%rax), %xmm1
	movl	-12(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-40(%rbp), %rax
	addq	%rdx, %rax
	movss	(%rax), %xmm3
	movaps	%xmm1, %xmm2
	subss	%xmm3, %xmm2
	movaps	%xmm0, %xmm1
	divss	%xmm2, %xmm1
	movl	-12(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-32(%rbp), %rax
	addq	%rdx, %rax
	movss	(%rax), %xmm0
	addss	%xmm1, %xmm0
	comiss	-24(%rbp), %xmm0
	jbe	.L3
	cmpl	$0, -4(%rbp)
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -4(%rbp)
.L3:
	movl	-12(%rbp), %eax
	leal	1(%rax), %edx
	movl	%edx, -12(%rbp)
	movl	%eax, -8(%rbp)
.L2:
	movl	-12(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jl	.L5
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	pnpoly, .-pnpoly
	.ident	"GCC: (GNU) 14.2.1 20240910"
	.section	.note.GNU-stack,"",@progbits
