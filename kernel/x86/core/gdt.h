#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>

// Define GDT constraints
#define GDT_LIMIT 256

// Initialize GDT structures
void init_gdt();

// Set the tss stack
void set_tss_stack(uint32_t);

// Load the global descriptor table
void load_gdt();

// Do GDT segments
void load_gdt_segments();

#endif
