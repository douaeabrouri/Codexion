	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 15	sdk_version 10, 15
	.globl	_typing1                ## -- Begin function typing1
	.p2align	4, 0x90
_typing1:                               ## @typing1
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	_value(%rip), %eax
	addl	$1, %eax
	movl	%eax, _value(%rip)
	movq	-8(%rbp), %rax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_typing                 ## -- Begin function typing
	.p2align	4, 0x90
_typing:                                ## @typing
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	leaq	L_.str(%rip), %rdi
	movb	$0, %al
	callq	_printf
	movl	$3, %edi
	movl	%eax, -12(%rbp)         ## 4-byte Spill
	callq	_sleep
	leaq	L_.str.1(%rip), %rdi
	movl	%eax, -16(%rbp)         ## 4-byte Spill
	movb	$0, %al
	callq	_printf
	xorl	%ecx, %ecx
	movl	%ecx, %edi
	movl	%eax, -20(%rbp)         ## 4-byte Spill
	movq	%rdi, %rax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$64, %rsp
	xorl	%eax, %eax
	movl	%eax, %ecx
	leaq	_typing1(%rip), %rdx
	movl	$0, -4(%rbp)
	movl	$0, -8(%rbp)
	leaq	-16(%rbp), %rdi
	movq	%rcx, %rsi
	callq	_pthread_create
	xorl	%r8d, %r8d
	movl	%r8d, %ecx
	leaq	_typing1(%rip), %rdx
	leaq	-24(%rbp), %rdi
	movq	%rcx, %rsi
	movl	%eax, -28(%rbp)         ## 4-byte Spill
	callq	_pthread_create
	leaq	L_.str.2(%rip), %rdi
	movl	%eax, -32(%rbp)         ## 4-byte Spill
	movb	$0, %al
	callq	_printf
	xorl	%r8d, %r8d
	movl	%r8d, %esi
	movq	-16(%rbp), %rdi
	movl	%eax, -36(%rbp)         ## 4-byte Spill
	callq	_pthread_join
	xorl	%r8d, %r8d
	movl	%r8d, %esi
	movq	-24(%rbp), %rdi
	movl	%eax, -40(%rbp)         ## 4-byte Spill
	callq	_pthread_join
	leaq	L_.str.3(%rip), %rdi
	movl	%eax, -44(%rbp)         ## 4-byte Spill
	movb	$0, %al
	callq	_printf
	movl	_value(%rip), %esi
	leaq	L_.str.4(%rip), %rdi
	movl	%eax, -48(%rbp)         ## 4-byte Spill
	movb	$0, %al
	callq	_printf
	xorl	%esi, %esi
	movl	%eax, -52(%rbp)         ## 4-byte Spill
	movl	%esi, %eax
	addq	$64, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_value                  ## @value
.zerofill __DATA,__common,_value,4,2
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"THREAD 2 is working too \n"

L_.str.1:                               ## @.str.1
	.asciz	"ending ...\n"

L_.str.2:                               ## @.str.2
	.asciz	"Waiting ...\n"

L_.str.3:                               ## @.str.3
	.asciz	"Main continues"

L_.str.4:                               ## @.str.4
	.asciz	"value = %d"


.subsections_via_symbols
