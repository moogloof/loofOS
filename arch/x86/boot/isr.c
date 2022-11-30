#include <stdint.h>
#include <boot/idt.h>
#include <boot/isr.h>
#include <boot/port.h>

// Initialize the PIC
void init_pic() {
	// ICW1
	// Start PIC initialization
	outportb(PIC_COMMAND1, 0x11);
	outportb(PIC_COMMAND2, 0x11);

	// ICW2
	// Set the PIC vector offsets
	outportb(PIC_DATA1, IRQ_OFFSET);
	outportb(PIC_DATA2, IRQ_OFFSET);

	// ICW3
	// Tell master PIC to commuicate through IRQ2 with slave PIC
	outportb(PIC_DATA1, 0b00000100);
	// Set cascade
	outportb(PIC_DATA2, 2);

	// ICW4
	// Set 8086 mode
	outportb(PIC_DATA1, 1);
	outportb(PIC_DATA2, 1);

	// Clear masks on all lines
	outportb(PIC_DATA1, 0xff);
	outportb(PIC_DATA2, 0b00111111);
}
