#include <peripherals/gpio.h>
#include <peripherals/aux.h>
#include <peripherals/uart.h>
#include <drivers/uart.h>
#include <util.h>

void uart_init() {
	// Set function select register to setup GPIO Pins 32 and 33 for uart (PL011)
	// Alternate functions 3 will be used (TXD0, RXD0)
	uint32_t selector = get32(GPFSEL3);
	// Set function select to alt3
	selector |= 7 << 6;
	selector |= 7 << 9;
	// Update select register
	put32(GPFSEL3, selector);

	// Disable GPIO PUD for GPIO pins 32 and 33
	// Set PUD signal
	put32(GPPUD, 0);
	// Wait 150 cycles for set-up time
	delay(150);
	// Clock the control signal for the pins
	put32(GPPUDCLK1, 3);
	// Wait 150 cycles for the control signal
	delay(150);
	// Remove clock
	put32(GPPUDCLK1, 0);

	// Initialize UART (PL011)
	// Disable mini uart
	put32(AUX_ENABLES, 0);
	// Clear all interrupts
	put32(UART_ICR, 0xff);
	// Temporarily disable UART
	put32(UART_CR, 0);
	// Set to 115200 baudrate
	put32(UART_IBRD, 510);
	// Set to 8-bit mode
	put32(UART_LCRH, 3 << 5);
	// UART enable and transmission/receive enable
	put32(UART_CR, 1 | (1 << 8) | (1 << 9));
}

void uart_sendc(char c) {
	// Wait until transmitter is empty
	while (get32(UART_FR) & 0x40) {}
	// Write to transmitter
	put32(UART_DR, c);
}

void uart_sendstr(char* str) {
	// Create pointer index
	char* strptr = str;

	// Send all chars in string
	do {
		uart_sendc(*strptr);
	} while (*(++strptr));
}

char uart_recv() {
	// Wait until transmitter is full
	while (get32(UART_FR) & 0x20) {}
	// Return transmitted char
	return (char)get32(UART_DR);
}
