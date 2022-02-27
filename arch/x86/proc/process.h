#ifndef PROC_PROCESS_H
#define PROC_PROCESS_H

#include <core/isr.h>
#include <core/registers.h>

// Process descriptor type
typedef struct process_desc {
	interrupt_frame frame; // All register sets
	gen_register_set gen_regs;
	seg_register_set seg_regs;
	struct process_desc* next;
	uint8_t privilege;
} __attribute__((packed)) process_desc;

// Switch context
void switch_process(seg_register_set*, gen_register_set*, interrupt_frame*);

#endif
