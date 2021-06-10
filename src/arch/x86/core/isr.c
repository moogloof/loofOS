#include <core/idt.h>
#include <core/isr.h>
#include <core/port.h>

// Initialize the PIC
void init_pic() {
	// ICW1
	// Start PIC initialization
	outportb(0x20, 0x11);
	outportb(0xa0, 0x11);

	// ICW2
	// Set the PIC vector offsets
	outportb(0x21, 0x20);
	outportb(0xa1, 0x28);

	// ICW3
	// Tell master PIC to commuicate through IRQ2 with slave PIC
	outportb(0x21, 0b00000100);
	// Set cascade
	outportb(0xa1, 2);

	// ICW4
	// Set 8086 mode
	outportb(0x21, 1);
	outportb(0xa1, 1);

	// Clear masks on all lines
	outportb(0x21, 0);
	outportb(0xa1, 0);
}

// Enable interrupts
void enable_interrupts() {
	__asm__("sti");
}

// Disable interrupts
void disable_interrupts() {
	__asm__("cli");
}
