	.file "defer_x86_64.s"
	.section .text

	.global defer_call
	.type	defer_call, @function
	# void defer_call(void *fn %rdi, void *args[6] %rsi)
defer_call:
	test	%rsi, %rsi
	je		.do_call

    # save the function pointer and args in temporary registers
	movq	%rdi, %r10
	movq	%rsi, %r11

    # assume sizeof(void*) == 8 bytes
	movq	0x00(%r11), %rdi
	movq	0x08(%r11), %rsi
	movq	0x10(%r11), %rdx
	movq	0x18(%r11), %rcx
	movq	0x20(%r11), %r8
	movq	0x28(%r11), %r9

.do_call:
	call	*%r10
	ret
