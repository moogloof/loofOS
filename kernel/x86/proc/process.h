#ifndef PROC_PROCESS_H
#define PROC_PROCESS_H

#include <stdint.h>
#include <core/isr.h>
#include <core/registers.h>

// Process priv level
#define RING_KERNEL 0
#define RING_USER 3

// Process states
// Running state
#define PROCESS_RUNNING 0
// Halted state
#define PROCESS_HALTED 1

// Define number of pages to allocate for a process stack
#define PROCESS_STACK_PAGES 1

// Process descriptor type
typedef struct process_desc {
	interrupt_frame frame; // All register sets
	gen_register_set gen_regs;
	seg_register_set seg_regs;
	uint32_t esp0; // For kernel stack
	uint32_t page_directory; // Page directory address
	struct process_desc* prev; // Prev process in queue
	struct process_desc* next; // Next process in queue
	uint8_t state; // State of the process, 0 == running, 1 == halted, 2 == ended
} __attribute__((packed)) process_desc;

// Init processes
void init_processes(void);

// Force enter userland
void force_enter_userland(void);

// Switch context
void switch_process(seg_register_set, gen_register_set, interrupt_frame);

// Switch process context
void switch_context(seg_register_set, gen_register_set, interrupt_frame, uint32_t);

// Create a process
void create_process(uint32_t);

// End a process
void update_process(uint8_t);

#endif
