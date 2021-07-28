#ifndef _ISR_H
#define _ISR_H

#include <stdint.h>

// Define EOI code
#define PIC_EOI 0x20

// Define command and data ports
// Master PIC command port
#define PIC_COMMAND1 0x20
// Master PIC data port
#define PIC_DATA1 0x21
// Slave PIC command port
#define PIC_COMMAND2 0xa0
// Slave PIC data port
#define PIC_DATA2 0xa1

// The interrupt frame struct
typedef struct {
	uint32_t ss;
	uint32_t esp;
	uint32_t eflags;
	uint32_t cs;
	uint32_t eip;
} interrupt_frame;

// Initialize the PIC
void init_pic();

// Enable interrupts
void enable_interrupts();

// Disable interrupts
void disable_interrupts();

// Enable and add exceptions
void init_exceptions();

// Exception handlers
// Divide-by-zero error
__attribute__((interrupt)) void divbyzero_handler(interrupt_frame*);
// Debug
__attribute__((interrupt)) void debug_handler(interrupt_frame*);
// Non-maskable interrupt
__attribute__((interrupt)) void nonmask_handler(interrupt_frame*);
// Breakpoint
__attribute__((interrupt)) void breakpoint_handler(interrupt_frame*);
// Overflow
__attribute__((interrupt)) void overflow_handler(interrupt_frame*);
// Bound range exceeded
__attribute__((interrupt)) void boundrange_handler(interrupt_frame*);
// Invalid opcode
__attribute__((interrupt)) void invalop_handler(interrupt_frame*);
// Device not available
__attribute__((interrupt)) void nodevice_handler(interrupt_frame*);
// Double fault
__attribute__((interrupt)) void doublefault_handler(interrupt_frame*);

#endif
