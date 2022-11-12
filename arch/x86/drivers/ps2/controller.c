#include <stdint.h>
#include <core/port.h>
#include <drivers/ps2/controller.h>

// Flag for second PS/2 port
static uint8_t port2_exists;
// Flag for initialization success
static uint8_t init_success;

// Initialize the PS/2 Controller
void init_ps2_controller() {
	// Zero out vars
	port2_exists = 0;
	init_success = 0;

	// Temporarily disable PS/2 Devices
	// Disable first port
	outportb(PS2_COMMAND, 0xad);
	// Disable second port
	outportb(PS2_COMMAND, 0xa7);

	// Flush the data port buffer
	inportb(PS2_DATA);

	// Set controller configuration byte for initialization
	// Poll configuration byte
	outportb(PS2_COMMAND, 0x20);
	uint8_t config_byte = poll_ps2_dataport();
	// Check if the second port exists
	port2_exists = (config_byte >> 5) & 1;
	// PS/2 port interrupts and translation disabled
	outportb(PS2_COMMAND, 0x60);
	write_ps2_dataport(0b00000100);

	// Perform controller test
	outportb(PS2_COMMAND, 0xaa);
	// Check response
	uint8_t test_response = poll_ps2_dataport();
	if (test_response != 0x55) {
		// If the test does not pass, return with error
		init_success = 0;
		return;
	}
	// Set the configuration byte again in case of device restart
	// PS/2 port interrupts and translation disabled
	outportb(PS2_COMMAND, 0x60);
	write_ps2_dataport(0b00000100);

	// Test PS/2 ports
	// Test first port
	outportb(PS2_COMMAND, 0xab);
	// Check if first port passed test
	test_response = poll_ps2_dataport();
	if (test_response != 0) {
		// If the test does not pass, return with error
		init_success = 0;
		return;
	}
	// Test second port
	outportb(PS2_COMMAND, 0xa9);
	// Check if second port passed test
	test_response = poll_ps2_dataport();
	port2_exists = test_response == 0;

	// Enable ports
	// Enable first port
	outportb(PS2_COMMAND, 0xae);
	// Enable second port
	if (port2_exists) {
		outportb(PS2_COMMAND, 0xa8);
	}

	// Enable interrupts in configuration byte
	// Read configuration byte
	outportb(PS2_COMMAND, 0x20);
	config_byte = poll_ps2_dataport();
	// Enable first port interrupt
	config_byte |= 1;
	// Disable second port interrupt
	config_byte &= 0b11111101;
	// Update configuration byte
	outportb(PS2_COMMAND, 0x60);
	write_ps2_dataport(config_byte);

	/* TODO: WTF is wrong with the resets????
	// Reset devices
	// Reset first port device
	write_ps2_dataport(0xff);
	// Get test result
	// Get ACK
	if (!(poll_ps2_dataport() == 0xfa && poll_ps2_dataport() == 0xaa)) {
		// If the test does not pass, return with error
		init_success = 0;
		return;
	}
	// Reset second port device
	outportb(PS2_COMMAND, 0xd4);
	write_ps2_dataport(0xff);
	// Get test result
	// Get ACK
	if (!(poll_ps2_dataport() == 0xfa && poll_ps2_dataport() == 0xaa)) {
		// If the test does not pass, return with error
		init_success = 0;
		return;
	}
	*/

	init_success = 1;
	// TODO: Make configuration more airtight
}

uint8_t poll_ps2_dataport() {
	// Wait for output buffer flag to be set
	while (!(inportb(PS2_STATUS) & 1));

	// Read from data port
	return inportb(PS2_DATA);
}

void write_ps2_dataport(uint8_t data) {
	// Wait for input buffer flag to be set
	while (inportb(PS2_STATUS) & 2);

	// Write to data port
	outportb(PS2_DATA, data);
}
