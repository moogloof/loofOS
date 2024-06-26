#include <stdint.h>
#include <core/idt.h>
#include <core/isr.h>
#include <core/port.h>
#include <core/panic.h>
#include <core/registers.h>

// Initialize the PIC
void init_pic(void) {
	// ICW1
	// Start PIC initialization
	outportb((uint8_t*)PIC_COMMAND1, 0x11);
	outportb((uint8_t*)PIC_COMMAND2, 0x11);

	// ICW2
	// Set the PIC vector offsets
	outportb((uint8_t*)PIC_DATA1, IRQ_OFFSET);
	outportb((uint8_t*)PIC_DATA2, IRQ_OFFSET);

	// ICW3
	// Tell master PIC to commuicate through IRQ2 with slave PIC
	outportb((uint8_t*)PIC_DATA1, 0x04);
	// Set cascade
	outportb((uint8_t*)PIC_DATA2, 2);

	// ICW4
	// Set 8086 mode
	outportb((uint8_t*)PIC_DATA1, 1);
	outportb((uint8_t*)PIC_DATA2, 1);

	// Clear masks on all lines
	outportb((uint8_t*)PIC_DATA1, 0);
	outportb((uint8_t*)PIC_DATA2, 0);
}

// Enable and add exceptions
void init_exceptions(void) {
	// Set the divide-by-zero exception
	set_id(0, &divbyzero_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(1, &debug_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(2, &nonmask_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(3, &breakpoint_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(4, &overflow_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(5, &boundrange_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(6, &invalop_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(7, &nodevice_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(8, &doublefault_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(10, &invaltss_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(11, &nosegment_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(12, &stackseg_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(13, &genprotection_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(39, &divbyzero_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);
}

// Exception handlers
// Divide-by-zero error
void divbyzero_handler(seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame) {
	kernel_panic("Division-by-zero exception occured.", seg_regs, gen_regs, frame, 0);
}

// Debug
void debug_handler(seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame) {
	kernel_panic("Debug exception.", seg_regs, gen_regs, frame, 0);
}

// Non-maskable interrupt
void nonmask_handler(seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame) {
	kernel_panic("Non-maskable interrupt.", seg_regs, gen_regs, frame, 0);
}

// Breakpoint
void breakpoint_handler(seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame) {
	kernel_panic("Breakpoint exception.", seg_regs, gen_regs, frame, 0);
}

// Overflow
void overflow_handler(seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame) {
	kernel_panic("Overflow exception.", seg_regs, gen_regs, frame, 0);
}

// Bound range exceeded
void boundrange_handler(seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame) {
	kernel_panic("Bound range exceeded exception.", seg_regs, gen_regs, frame, 0);
}

// Invalid opcode
void invalop_handler(seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame) {
	kernel_panic("Invalid opcode exception.", seg_regs, gen_regs, frame, 0);
}

// Device not available
void nodevice_handler(seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame) {
	kernel_panic("Device not available exception.", seg_regs, gen_regs, frame, 0);
}

// Double fault
void doublefault_handler(seg_register_set seg_regs, gen_register_set gen_regs, uint32_t ecode, interrupt_frame frame) {
	kernel_panic("Double fault exception.", seg_regs, gen_regs, frame, ecode);
}

// Invalid TSS fault
void invaltss_handler(seg_register_set seg_regs, gen_register_set gen_regs, uint32_t ecode, interrupt_frame frame) {
	kernel_panic("Invalid TSS exception.", seg_regs, gen_regs, frame, ecode);
}

// Segment not present fault
void nosegment_handler(seg_register_set seg_regs, gen_register_set gen_regs, uint32_t ecode, interrupt_frame frame) {
	kernel_panic("Segment not present exception.", seg_regs, gen_regs, frame, ecode);
}

// Stack segment fault
void stackseg_handler(seg_register_set seg_regs, gen_register_set gen_regs, uint32_t ecode, interrupt_frame frame) {
	kernel_panic("Stack segment exception.", seg_regs, gen_regs, frame, ecode);
}

// General protection fault
void genprotection_handler(seg_register_set seg_regs, gen_register_set gen_regs, uint32_t ecode, interrupt_frame frame) {
	kernel_panic("General protection exception.", seg_regs, gen_regs, frame, ecode);
}
