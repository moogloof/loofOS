// Include drivers
#include <drivers/uart.h>

// Main kernel func
void kernel_main() {
	// Hello world thing
	uart_init();

	uart_sendstr("Hello, world!\r\n");

	while (1) {
		uart_sendc(uart_recv());
	}
}
