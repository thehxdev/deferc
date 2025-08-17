	.file "defer.s"
	.section .text

	.global defer_call
	.type	defer_call, @function
	# void defer_call(void *fn %rdi, void *args[6] %rsi, int args_count %rdx)
defer_call:
    # save args in temporary registers
	movq	%rdi, %r12
	movq	%rsi, %r14

    # TODO: this is not the correct way to setup arguments. must use the
    # `args_count` parameter and a jump table to setup the arguements as
    # needed.
    #
    # assume sizeof(void*) == 8 bytes
	movq	0x00(%r14), %rdi
	movq	0x08(%r14), %rsi
	movq	0x10(%r14), %rdx
	movq	0x18(%r14), %rcx
	movq	0x20(%r14), %r8
	movq	0x28(%r14), %r9

	call	%r12
	ret

