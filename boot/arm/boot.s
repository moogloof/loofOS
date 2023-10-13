.section .vectors
.global _start
_start:
	// Vectors
	b _reset
	b _undefined_handler
	b _supervisor_call
	b _prefetch_abort_handler
	b _data_abort_handler
	nop
	b _irq_handler

.section .text.boot
_reset:
	// Shutoff extra cores
	mrc p15, 0, r5, c0, c0, 5
	and r5, r5, #3
	cmp r5, #0
	bne halt

	// Setup stack
	ldr sp, =STACK_TOP

	// BSS clear
	ldr r0, =_sbss
	ldr r1, =_ebss
	bl memzero

	// Call kernel
	bl kernel_main

halt:
	b halt
