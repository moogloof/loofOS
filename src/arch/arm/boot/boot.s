.text
.global _start
_start:
	// Setup stack
	ldr sp, =_start

	// Call kernel
	bl kernel_main

halt:
	b halt
