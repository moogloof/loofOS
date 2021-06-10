#ifndef _ISR_H
#define _ISR_H

#include <stdint.h>

// The interrupt frame struct
struct interrupt_frame {
	uint16_t ip;
	uint16_t cs;
	uint16_t flags;
	uint16_t sp;
	uint16_t ss;
};

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
__attribute__((interrupt)) void divbyzero_handler(struct interrupt_frame*);

#endif
