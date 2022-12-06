#include <stdint.h>
#include <drivers/ps2/controller.h>
#include <drivers/ps2/keyboard.h>
#include <drivers/vga_text.h>
#include <sys/kernel_print.h>
#include <core/isr.h>
#include <core/idt.h>

// Scancode set detected
static uint8_t scancode_set;

// Modifiers for pressed key
static struct {
	uint8_t shift:1;
	uint8_t ctrl:1;
} __attribute__((packed)) key_modifier;

// Scancodes for key processing
// Scancode set 1
static const char scancodes1[] = {
	NOK, NOK, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t', // 0x00
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', NOK, 'a', 's', // 0x10
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', NOK, '\\', 'z', 'x', 'c', 'v', // 0x20
	'b', 'n', 'm', ',', '.', '/', NOK, '*', NOK, ' ', NOK, NOK, NOK, NOK, NOK, NOK, // 0x30
	NOK, NOK, NOK, NOK, NOK, NOK, NOK, '7', '8', '9', '-', '4', '5', '6', '+', '1', // 0x40
	'2', '3', '0', '.', NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK // 0x50
};
// Scancode set 1 shifted
static const char scancodes1_shift[] = {
	NOK, NOK, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', // 0x00
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', NOK, 'A', 'S', // 0x10
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', NOK, '|', 'Z', 'X', 'C', 'V', // 0x20
	'B', 'N', 'M', '<', '>', '?', NOK, '*', NOK, ' ', NOK, NOK, NOK, NOK, NOK, NOK, // 0x30
	NOK, NOK, NOK, NOK, NOK, NOK, NOK, '7', '8', '9', '-', '4', '5', '6', '+', '1', // 0x40
	'2', '3', '0', '.', NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK // 0x50
};

// Scancode set 2
static const char scancodes2[] = {
	NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, '\t', '`', NOK, // 0x00
	NOK, NOK, NOK, NOK, NOK, 'q', '1', NOK, NOK, NOK, 'z', 's', 'a', 'w', '2', NOK, // 0x10
	NOK, 'c', 'x', 'd', 'e', '4', '3', NOK, NOK, ' ', 'v', 'f', 't', 'r', '5', NOK, // 0x20
	NOK, 'n', 'b', 'h', 'g', 'y', '6', NOK, NOK, NOK, 'm', 'j', 'u', '7', '8', NOK, // 0x30
	NOK, ',', 'k', 'i', 'o', '0', '9', NOK, NOK, '.', '/', 'l', ';', 'p', '-', NOK, // 0x40
	NOK, NOK, '\'', NOK, '[', '=', NOK, NOK, NOK, NOK, '\n', ']', NOK, '\\', NOK, NOK, // 0x50
	NOK, NOK, NOK, NOK, NOK, NOK, '\b', NOK, NOK, '1', NOK, '4', '7', NOK, NOK, NOK, // 0x60
	'0', '.', '2', '5', '6', '8', NOK, NOK, NOK, '+', '3', '-', '*', '9', NOK, NOK // 0x70
};
// Scancode set 2 shifted
static const char scancodes2_shift[] = {
	NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, NOK, '\t', '~', NOK, // 0x00
	NOK, NOK, NOK, NOK, NOK, 'Q', '!', NOK, NOK, NOK, 'Z', 'S', 'A', 'W', '@', NOK, // 0x10
	NOK, 'C', 'X', 'D', 'E', '$', '#', NOK, NOK, ' ', 'V', 'F', 'T', 'R', '%', NOK, // 0x20
	NOK, 'N', 'B', 'H', 'G', 'Y', '^', NOK, NOK, NOK, 'M', 'J', 'U', '&', '*', NOK, // 0x30
	NOK, '<', 'K', 'I', 'O', ')', '(', NOK, NOK, '>', '?', 'L', ':', 'P', '_', NOK, // 0x40
	NOK, NOK, '"', NOK, '{', '+', NOK, NOK, NOK, NOK, '\n', '}', NOK, '|', NOK, NOK, // 0x50
	NOK, NOK, NOK, NOK, NOK, NOK, '\b', NOK, NOK, '1', NOK, '4', '7', NOK, NOK, NOK, // 0x60
	'0', '.', '2', '5', '6', '8', NOK, NOK, NOK, '+', '3', '-', '*', '9', NOK, NOK // 0x70
};

// Initialize the PS/2 Keyboard device
void init_ps2_keyboard() {
	// Zero out all necessary vars
	key_modifier.shift = 0;
	key_modifier.ctrl = 0;
	scancode_set = 0;

	// TODO: Better handling of ACK byte
	// Enable keyboard scanning
	write_ps2_dataport(0xf4);
	// Get ACK
	poll_ps2_dataport();

	// Check and get scancodes
	// Send get command
	write_ps2_dataport(0xf0);
	// Get ACK
	poll_ps2_dataport();
	// Get scancode set
	write_ps2_dataport(0);
	poll_ps2_dataport();
	scancode_set = poll_ps2_dataport() & 0x0f;
	// Check whether scancode set is available
	if (scancode_set > 2) {
		// Set scancode set to 2
		// Send set command
		write_ps2_dataport(0xf0);
		// Get ACK
		poll_ps2_dataport();
		// Set scancode set
		write_ps2_dataport(2);
		poll_ps2_dataport();

		scancode_set = 2;
	}

//	kernel_print("Keyboard Scancode Set: %d\r\n", scancode_set);

	// Set keyboard interrupt handler
	set_id(IRQ_OFFSET + 1, &keyboard_handler, 0x08, IDT_PROT_INTR, 0, 1);
}

// PS/2 Keyboard handler
__attribute__((interrupt)) void keyboard_handler(interrupt_frame* frame) {
	// Scancode
	uint8_t scancode1 = poll_ps2_dataport();

	switch (scancode_set) {
		case 1:
			// Process scancodes for 1
			if (scancode1 < 0x60) {
				// Check if pressed key is shift
				if (scancode1 == 0x2a || scancode1 == 0x36) {
					key_modifier.shift = 1;
				}

				// Check if pressed key is special or not
				if (scancodes1[scancode1] == NOK) {
					break;
				}

				// Handle pressed key
				if (key_modifier.shift) {
					output_char(scancodes1_shift[scancode1]);
				} else {
					output_char(scancodes1[scancode1]);
				}
			} else {
				// Check if released key is shift
				if (scancode1 == 0xaa || scancode1 == 0xb6) {
					key_modifier.shift = 0;
				}
			}
			break;
		case 2:
			// Process scancodes for 2
			if (scancode1 < 0x80) {
				// Check if pressed key is shift
				if (scancode1 == 0x12 || scancode1 == 0x59) {
					key_modifier.shift = 1;
				}

				// Check if pressed key is special or not
				if (scancodes2[scancode1] == NOK) {
					break;
				}

				// Handle pressed key
				if (key_modifier.shift) {
					output_char(scancodes2_shift[scancode1]);
				} else {
					output_char(scancodes2[scancode1]);
				}
			} else if (scancode1 == 0xf0) {
				// Get released key
				scancode1 = poll_ps2_dataport();

				// Check if released key is shift
				if (scancode1 == 0x12 || scancode1 == 0x59) {
					key_modifier.shift = 0;
				}
			}
			break;
		default:
			break;
	}

	// Send EOI
	outportb(PIC_COMMAND1, PIC_EOI);
}
