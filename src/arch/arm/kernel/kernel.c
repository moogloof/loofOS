// Include drivers
#include <drivers/uart.h>
#include <util.h>
#include <stdint.h>

// Main kernel func
void kernel_main() {
	// Hello world thing
	uart_init();

	// Get Mode
	uint8_t el = get_mode();

	// Print Mode
	uart_sendstr("Processor Mode: ");
	uart_sendc((char)(el + 48));
	uart_sendstr("\r\n");

	while (1) {
		uart_sendc(uart_recv());
	}
}
