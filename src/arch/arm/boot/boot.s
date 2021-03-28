.text
.global _start
_start:
	// Setup stack
	ldr r5, =_start
	mov sp, r5

	// Call kernel
	bl kernel_main

halt:
	b halt
