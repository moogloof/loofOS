#include <stdint.h>
#include <peripherals/gpio.h>
#include <peripherals/aux.h>
#include <drivers/mini_uart.h>
#include <util.h>

void mini_uart_init() {
	// Set function select register to setup GPIO Pins 14 and 15 for mini uart
	// Alternate functions 5 will be used (TXD1, RXD1)
	uint32_t selector = get32(GPFSEL1);
	// Clear function select for FSEL14 and FSEL15
	selector &= ~(7 << 12);
	selector &= ~(7 << 15);
	// Set function select to alt5
	selector |= ~(2 << 12);
	selector |= ~(2 << 15);
	// Update select register
	put32(GPFSEL1, selector);

	// Disable GPIO PUD for GPIO Pins 14 and 15
	// Set PUD signal
	put32(GPPUD, 0);
	// Wait 150 cycles for set-up time
	delay(150);
	// Clock the control signal for the pins
	put32(GPPUDCLK0, (1 << 14) | (1 << 15));
	// Wait 150 cycles for the control signal
	delay(150);
	// Remove clock
	put32(GPPUDCLK0, 0);

	// Initialize mini Uart
	// Set mode to mini Uart
	put32(AUX_ENABLES, 1);
	// Temporarily disable receiver and transmitter
	put32(AUX_MU_CNTL_REG, 0);
	// Disable receive and transmit interrupts
	put32(AUX_MU_IER_REG, 0);
	// Set Uart 8-bit mode
	put32(AUX_MU_LCR_REG, 3);
	// Ignore and set RTS line to always high
	put32(AUX_MU_MCR_REG, 0);
	// Set to 115200 baud rate
	put32(AUX_MU_BAUD_REG, 270);
	// Enable transmitter and receiver
	put32(AUX_MU_CNTL_REG, 3);
}

void mini_uart_sendc(char c) {
	// Wait until transmitter is empty
	while (get32(AUX_MU_LSR_REG) & 0x20) {}
	// Write to transmitter
	put32(AUX_MU_IO_REG, c);
}

void mini_uart_sendstr(char* str) {
	// Create pointer index
	char* strptr = str;

	// Send all chars in string
	do {
		mini_uart_sendc(*strptr);
	} while (*(++strptr));
}

char mini_uart_recv() {
	// Wait until transmitter is full
	while (get32(AUX_MU_LSR_REG) & 0x1) {}
	// Return transmitted char
	return (char)get32(AUX_MU_IO_REG);
}
