#ifndef PROC_PROCESS_H
#define PROC_PROCESS_H

#include <stdint.h>
#include <core/isr.h>
#include <core/registers.h>

// Define number of pages to allocate for a process stack
#define PROCESS_STACK_PAGES 20

// Process descriptor type
typedef struct process_desc {
	interrupt_frame frame; // All register sets
	gen_register_set gen_regs;
	seg_register_set seg_regs;
	uint32_t page_directory; // Page directory address
	struct process_desc* next; // Next process in queue
	uint8_t state; // State of the process, 0 == running, 1 == halted, 2 == ended
} __attribute__((packed)) process_desc;

// Init processes
void init_processes();

// Switch context
void switch_process(seg_register_set, gen_register_set, interrupt_frame);

// Switch process context
void switch_context(seg_register_set, gen_register_set, interrupt_frame, uint32_t);

// Create a process
void create_process(uint32_t);

// End a process
void end_process();

#endif
