// Include drivers
#include <drivers/uart.h>
#include <drivers/irq.h>
#include <print.h>
#include <util.h>

// Main kernel func
void kernel_main() {
	// Setup IRQs
	irq_enable();
	irq_init();

	// Setup uart
	uart_init();

	// Get Mode
	long el = get_mode();

	// Print Mode
	print(&uart_sendstr, "Processor Mode: ");
	printint(&uart_sendstr, el, 10, 16);
	print(&uart_sendstr, "\r\n");

	while (1) {
		uart_sendc(uart_recv());
	}
}
