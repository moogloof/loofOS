// Include drivers
#include <drivers/mini_uart.h>

// Main kernel func
void kernel_main() {
	// Hello world thing
	mini_uart_init();

	mini_uart_sendstr("Hello, world!\r\n");

	while (1) {
		mini_uart_sendc(mini_uart_recv());
	}
}
