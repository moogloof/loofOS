.section .text.boot
.global _start
_start:
	// Shutoff extra cores
	mrc p15, 0, r5, c0, c0, 5
	and r5, r5, #3
	cmp r5, #0
	bne halt

	// Setup stack
	ldr sp, =STACK_TOP

	// BSS clear
	ldr r4, =_sbss
	ldr r9, =_ebss
	mov r5, #0
	mov r6, #0
	mov r7, #0
	mov r8, #0

// bss clear loop
bss_clear_loop:
	// Store multiple
	stmia r4!, {r5-r8}

bss_loop_check:
	// Exit once done
	cmp r4, r9
	blo bss_clear_loop

	// Call kernel
	bl kernel_main

halt:
	b halt
