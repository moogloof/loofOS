#include <stdint.h>
#include <core/idt.h>
#include <core/isr.h>
#include <core/port.h>
#include <core/panic.h>

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
	outportb(PIC_DATA1, 0);
	outportb(PIC_DATA2, 0);
}

// Enable interrupts
void enable_interrupts() {
	__asm__("sti");
}

// Disable interrupts
void disable_interrupts() {
	__asm__("cli");
}

// Enable and add exceptions
void init_exceptions() {
	// Set the divide-by-zero exception
	set_id(0, divbyzero_handler, 0x08, IDT_PROT_TRAP, 0, 1);
	set_id(1, debug_handler, 0x08, IDT_PROT_TRAP, 0, 1);
	set_id(2, nonmask_handler, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(3, breakpoint_handler, 0x08, IDT_PROT_TRAP, 0, 1);
	set_id(4, overflow_handler, 0x08, IDT_PROT_TRAP, 0, 1);
	set_id(5, boundrange_handler, 0x08, IDT_PROT_TRAP, 0, 1);
	set_id(6, invalop_handler, 0x08, IDT_PROT_TRAP, 0, 1);
	set_id(7, nodevice_handler, 0x08, IDT_PROT_TRAP, 0, 1);
	set_id(8, doublefault_handler, 0x08, IDT_PROT_TRAP, 0, 1);
}

// Exception handlers
// Divide-by-zero error
__attribute__((interrupt)) void divbyzero_handler(struct interrupt_frame* frame) {
	kernel_panic("Division-by-zero exception occured.");
}

// Debug
__attribute__((interrupt)) void debug_handler(struct interrupt_frame* frame) {
	kernel_panic("Debug exception.");
}

// Non-maskable interrupt
__attribute__((interrupt)) void nonmask_handler(struct interrupt_frame* frame) {
	kernel_panic("Non-maskable interrupt.");
}

// Breakpoint
__attribute__((interrupt)) void breakpoint_handler(struct interrupt_frame* frame) {
	kernel_panic("Breakpoint exception.");
}

// Overflow
__attribute__((interrupt)) void overflow_handler(struct interrupt_frame* frame) {
	kernel_panic("Overflow exception.");
}

// Bound range exceeded
__attribute__((interrupt)) void boundrange_handler(struct interrupt_frame* frame) {
	kernel_panic("Bound range exceeded exception.");
}

// Invalid opcode
__attribute__((interrupt)) void invalop_handler(struct interrupt_frame* frame) {
	kernel_panic("Invalid opcode exception.");
}

// Device not available
__attribute__((interrupt)) void nodevice_handler(struct interrupt_frame* frame) {
	kernel_panic("Device not available exception.");
}

// Double fault
__attribute__((interrupt)) void doublefault_handler(struct interrupt_frame* frame) {
	// Pop error code
	uint32_t error_code;
	__asm__("pop %0" : "=r"(error_code));
	kernel_panic("Double fault exception.");
}
