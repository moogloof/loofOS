#ifndef CORE_ISR_H
#define CORE_ISR_H

#include <stdint.h>
#include <common.h>
#include <core/registers.h>

// Define EOI code
#define PIC_EOI 0x20

// Define command and data ports
// Master PIC command port
#define PIC_COMMAND1 (KERNEL_BASE + 0x20)
// Master PIC data port
#define PIC_DATA1 (KERNEL_BASE + 0x21)
// Slave PIC command port
#define PIC_COMMAND2 (KERNEL_BASE + 0xa0)
// Slave PIC data port
#define PIC_DATA2 (KERNEL_BASE + 0xa1)

// The interrupt frame struct
// IMPORTANT: This is only valid if the interrupt happens while the CPU is in ring 3.
typedef struct {
	uint32_t eip;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;
} interrupt_frame;

// Initialize the PIC
void init_pic();

// Enable interrupts
#define enable_interrupts() __asm__("sti");

// Disable interrupts
#define disable_interrupts() __asm__("cli");

// Enable and add exceptions
void init_exceptions();

// Exception handlers
// Divide-by-zero error
__attribute__((interrupt)) void divbyzero_handler_wrapper(interrupt_frame*);
void divbyzero_handler(seg_register_set, gen_register_set, interrupt_frame);
// Debug
__attribute__((interrupt)) void debug_handler_wrapper(interrupt_frame*);
void debug_handler(seg_register_set, gen_register_set, interrupt_frame);
// Non-maskable interrupt
__attribute__((interrupt)) void nonmask_handler_wrapper(interrupt_frame*);
void nonmask_handler(seg_register_set, gen_register_set, interrupt_frame);
// Breakpoint
__attribute__((interrupt)) void breakpoint_handler_wrapper(interrupt_frame*);
void breakpoint_handler(seg_register_set, gen_register_set, interrupt_frame);
// Overflow
__attribute__((interrupt)) void overflow_handler_wrapper(interrupt_frame*);
void overflow_handler(seg_register_set, gen_register_set, interrupt_frame);
// Bound range exceeded
__attribute__((interrupt)) void boundrange_handler_wrapper(interrupt_frame*);
void boundrange_handler(seg_register_set, gen_register_set, interrupt_frame);
// Invalid opcode
__attribute__((interrupt)) void invalop_handler_wrapper(interrupt_frame*);
void invalop_handler(seg_register_set, gen_register_set, interrupt_frame);
// Device not available
__attribute__((interrupt)) void nodevice_handler_wrapper(interrupt_frame*);
void nodevice_handler(seg_register_set, gen_register_set, interrupt_frame);
// Double fault
__attribute__((interrupt)) void doublefault_handler_wrapper(interrupt_frame*);
void doublefault_handler(seg_register_set, gen_register_set, uint32_t, interrupt_frame);
// Invalid TSS fault
__attribute__((interrupt)) void invaltss_handler_wrapper(interrupt_frame*);
void invaltss_handler(seg_register_set, gen_register_set, uint32_t, interrupt_frame);
// Segment not present fault
__attribute__((interrupt)) void nosegment_handler_wrapper(interrupt_frame*);
void nosegment_handler(seg_register_set, gen_register_set, uint32_t, interrupt_frame);
// Stack segment fault
__attribute__((interrupt)) void stackseg_handler_wrapper(interrupt_frame*);
void stackseg_handler(seg_register_set, gen_register_set, uint32_t, interrupt_frame);
// General protection fault
__attribute__((interrupt)) void genprotection_handler_wrapper(interrupt_frame*);
void genprotection_handler(seg_register_set, gen_register_set, uint32_t, interrupt_frame);

#endif
