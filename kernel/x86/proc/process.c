#include <proc/process.h>
#include <common.h>
#include <mm/alloc.h>
#include <mm/paging.h>

// Flag whether entering usermode for the first time or not
static uint8_t entering_user = 1;

// Current process in context
static process_desc* current_process;

// Previous process in context
static process_desc* previous_process;

// Init processes
void init_processes() {
	// Zero out vars
	current_process = 0;
	previous_process = 0;
}

// Switch context
void switch_process(seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame) {
	// No process is no switch
	if (!current_process) {
		return;
	}

	if (!entering_user) {
		// Check if current process is running
		if (current_process->state == 0) {
			// Freeze registers
			// Frame registers
			current_process->frame = frame;
			// Segment registers
			current_process->seg_regs = seg_regs;
			// General registers
			current_process->gen_regs = gen_regs;
		} else if (current_process->state == 2) {
			// On state ended, delete process
			previous_process->next = current_process->next;
			kernel_free(current_process->frame.esp);
			kernel_free(current_process);
		}

		// Update current process
		// Skip all halted processes
		do {
			// Update previous context
			previous_process = current_process;
			current_process = current_process->next;
		} while (current_process->state == 1);
	} else {
		entering_user = 0;
	}

	switch_context(current_process->seg_regs, current_process->gen_regs, current_process->frame, current_process->page_directory - KERNEL_BASE);
}

// Create a process
void create_process(uint32_t eip) {
	process_desc* new_process = kernel_allocate(sizeof(process_desc));
	uint32_t code_segment = 0x18 | 3;
	uint32_t data_segment = 0x20 | 3;

	// Set address of process
	new_process->frame.eip = eip;
	// Set code segment
	new_process->frame.cs = code_segment;
	// Set eflags
	// IMPORTANT: Make sure to set the interrupt enable flag
	new_process->frame.eflags = 1 << 9;
	// Setup stack
	new_process->frame.esp = KERNEL_BASE - 4;
	new_process->frame.ss = data_segment;

	// Set data segments
	new_process->seg_regs.ds = data_segment;
	new_process->seg_regs.es = data_segment;
	new_process->seg_regs.fs = data_segment;
	new_process->seg_regs.gs = data_segment;

	// Set general registers
	new_process->gen_regs.edi = 0;
	new_process->gen_regs.esi = 0;
	new_process->gen_regs.ebp = new_process->frame.esp;
	new_process->gen_regs.ebx = 0;
	new_process->gen_regs.edx = 0;
	new_process->gen_regs.ecx = 0;
	new_process->gen_regs.eax = 0;

	// Set the page directory
	new_process->page_directory = kernel_allocate(sizeof(pde_4kib) * 1024);
	// Copy kernel space to user space
	for (int i = 0; i < 256; i++) {
		((pde_4mib*)new_process->page_directory)[i + 768] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 0, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i};
	}
	// Allocate stack page
	// Calculate base address of stack
	uint32_t base_esp_addr = KERNEL_BASE - (PAGE_SIZE_4K * PROCESS_STACK_PAGES);
	for (int i = 0; i < PROCESS_STACK_PAGES; i++) {
		allocate_page((pde_4kib*)new_process->page_directory, base_esp_addr + i*PAGE_SIZE_4K);
	}

	allocate_page((pde_4kib*)new_process->page_directory, 0);

	// Set state of process as running
	new_process->state = 0;

	// Insert process into process queue
	if (previous_process) {
		previous_process->next = new_process;
	}
	if (!current_process) {
		current_process = new_process;
		current_process->next = new_process;
	}
	previous_process = new_process;
	new_process->next = current_process;
}

// End a process
void end_process() {
	current_process->state = 2;
}
