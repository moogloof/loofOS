.global get32
get32:
	// Get 32bit info from address parameter
	ldr r0, [r0]
	bx lr

.global put32
put32:
	// Set 32bit info at address
	str r1, [r0]
	bx lr

.global delay
delay:
	// Delay for amount of clock ticks
	subs r0, r0, #1
	bne delay
	bx lr

.global get_mode
get_mode:
	// Get EL of process
	mrs r0, cpsr
	and r0, #0x1f
	bx lr
