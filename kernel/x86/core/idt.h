#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>
#include <core/isr.h>

// Define IDT constraints
#define IDT_LIMIT 256
// Define IDT types
#define IDT_PROT_TASK 0x5
#define IDT_REAL_INTR 0x6
#define IDT_REAL_TRAP 0x7
#define IDT_PROT_INTR 0xe
#define IDT_PROT_TRAP 0xf
// Define IRQ vector offsets
#define IRQ_OFFSET 0x20

// Initialize IDT structures
void init_idt();

// Add interrupt descriptor to the IDT
void set_id(uint8_t, void (*)(interrupt_frame*), uint16_t, uint8_t, uint8_t, uint8_t);

// Load the IDT
void load_idt();

#endif
