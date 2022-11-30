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

#endif
