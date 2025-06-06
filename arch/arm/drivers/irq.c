#include <drivers/irq.h>
#include <peripherals/irq.h>
#include <stdint.h>
#include <drivers/uart.h>
#include <drivers/timer.h>
#include <print.h>
#include <util.h>

// IRQ initializer
void irq_init() {
	uint32_t irq1 = 0x0;
	uint32_t irq2 = 0x0;

	// Enable IRQs
	irq1 |= IRQ_TIMER1;

	// Set IRQ enable registers
	put32(IRQ_ENABLE1, irq1);
	put32(IRQ_ENABLE2, irq2);
}

// IRQ handler
void _irq_handler() {
	print(&uart_sendstr, "Some IRQ.\r\n");
	// Get pending IRQs
	uint64_t irqs = ((uint64_t)get32(IRQ_PENDING2) << 32) | get32(IRQ_PENDING1);

	print(&uart_sendstr, "Some IRQ.\r\n");

	// Handle IRQs
	switch (irqs) {
		case IRQ_TIMER1: // Timer 1 irq
			irq_timer_handler();
			break;
		default: // Unknown irq
			print(&uart_sendstr, "Unknown IRQ.\r\n");
	}
}

