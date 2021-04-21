.global irq_enable
irq_enable:
	// Enable irq
	mrs r0, cpsr
	orr r0, #128
	msr cpsr, r0
	bx lr

.global irq_disable
irq_disable:
	// Disable irq
	mrs r0, cpsr
	and r0, #0xffffff80
	msr cpsr, r0
	bx lr
