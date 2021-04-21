// Include drivers
#include <drivers/uart.h>
#include <drivers/irq.h>
#include <drivers/timer.h>
#include <peripherals/timer.h>
#include <print.h>
#include <util.h>

// Main kernel func
void kernel_main() {
	// Setup uart
	print(&uart_sendstr, "Setting up UART.\r");
	uart_init();
	print(&uart_sendstr, "Setup UART.-----\r\n");

	// Setup timer
	print(&uart_sendstr, "Setting up timer.\r");
	init_timer();
	print(&uart_sendstr, "Setup timer.-----\r\n");

	// Setup IRQs
	print(&uart_sendstr, "Setting up IRQs.\r");
	irq_enable();
	irq_init();
	print(&uart_sendstr, "Setup IRQs.-----\r\n");

	// Get Mode
	long el = get_mode();

	// Print Mode
	print(&uart_sendstr, "Processor Mode: ");
	printint(&uart_sendstr, el, 10, 16);
	print(&uart_sendstr, "\r\n");

	while (1) {
	}
}
