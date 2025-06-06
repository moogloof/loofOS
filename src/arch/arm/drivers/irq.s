.global irq_enable
irq_enable:
	// Enable irq
	cpsie i
	bx lr

.global irq_disable
irq_disable:
	// Disable irq
	cpsid i
	bx lr
