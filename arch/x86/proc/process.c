#include <proc/process.h>
#include <common.h>
#include <mm/alloc.h>
#include <mm/paging.h>

// Flag whether entering usermode for the first time or not
static uint8_t entering_user = 1;

// Current process in context
static process_desc* current_process = 0;

// Previous process in context
static process_desc* previous_process = 0;

// Switch context
uint32_t switch_process(seg_register_set* seg_regs, gen_register_set* gen_regs, interrupt_frame* frame) {
	// No process is no switch
	if (!current_process) {
		return KERNEL_PAGE_DIRECTORY;
	}

	if (!entering_user) {
		// Check if current process is running
		if (current_process->state == 0) {
			// Freeze registers
			// Frame registers
			current_process->frame.eip = frame->eip;
			current_process->frame.cs = frame->cs;
			current_process->frame.eflags = frame->eflags;
			current_process->frame.esp = frame->esp;
			current_process->frame.ss = frame->ss;
			// Segment registers
			current_process->seg_regs.ds = seg_regs->ds;
			current_process->seg_regs.es = seg_regs->es;
			current_process->seg_regs.fs = seg_regs->fs;
			current_process->seg_regs.gs = seg_regs->gs;
			// General registers
			current_process->gen_regs.edi = gen_regs->edi;
			current_process->gen_regs.esi = gen_regs->esi;
			current_process->gen_regs.ebp = gen_regs->ebp;
			current_process->gen_regs.ebx = gen_regs->ebx;
			current_process->gen_regs.edx = gen_regs->edx;
			current_process->gen_regs.ecx = gen_regs->ecx;
			current_process->gen_regs.eax = gen_regs->eax;
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

	// Switch to next context
	// Segment registers
	seg_regs->ds = current_process->seg_regs.ds;
	seg_regs->es = current_process->seg_regs.es;
	seg_regs->fs = current_process->seg_regs.fs;
	seg_regs->gs = current_process->seg_regs.gs;
	// General registers
	gen_regs->edi = current_process->gen_regs.edi;
	gen_regs->esi = current_process->gen_regs.esi;
	gen_regs->ebp = current_process->gen_regs.ebp;
	gen_regs->ebx = current_process->gen_regs.ebx;
	gen_regs->edx = current_process->gen_regs.edx;
	gen_regs->ecx = current_process->gen_regs.ecx;
	gen_regs->eax = current_process->gen_regs.eax;
	// Frame registers
	frame->eip = current_process->frame.eip;
	frame->cs = current_process->frame.cs;
	frame->eflags = current_process->frame.eflags;
	frame->esp = current_process->frame.esp;
	frame->ss = current_process->frame.ss;
	// Page directory
	return current_process->page_directory - KERNEL_BASE;
}

// Create a process
void create_process(uint32_t eip, uint8_t ring) {
	process_desc* new_process = kernel_allocate(sizeof(process_desc));
	uint32_t code_segment = ((ring == 0) ? 0x8 : 0x18) | ring;
	uint32_t data_segment = ((ring == 0) ? 0x10 : 0x20) | ring;

	// Set process ring
	new_process->ring = ring;
	// Set address of process
	new_process->frame.eip = eip;
	// Set code segment
	new_process->frame.cs = code_segment;
	// Set eflags
	// IMPORTANT: Make sure to set the interrupt enable flag
	new_process->frame.eflags = 1 << 9;
	// Setup stack
	new_process->frame.esp = kernel_allocate(4096) + 4092;
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
	if (ring == 0) {
		new_process->page_directory = KERNEL_PAGE_DIRECTORY;
	} else {
		new_process->page_directory = kernel_allocate(sizeof(pde_4kib) * 1024);
		// Copy kernel space to user space
		for (int i = 0; i < PAGE_LENGTH_4M / 4; i++) {
			((pde_4mib*)new_process->page_directory)[i + 768] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 1, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i};
		}
		// Allow access to the stack in the kernel heap
		// Calculate base address of stack
		uint32_t base_esp_addr = new_process->frame.esp - 4092;
		// Create new PTE for specifying 4KiB block stack
		void* new_pte = (uint32_t)kernel_allocate(sizeof(pte_4kib) * 1024);
		// Set PDE
		((pde_4kib*)new_process->page_directory)[base_esp_addr >> 22] = (pde_4kib){.present = 1, .rw = 1, .us = 1, .pwt = 0, .pcd = 0, .a = 0, .ignored = 0, .ps = 0, .ignored2 = 0, .addr = (uint32_t)new_pte >> 12};
		// Set all PTEs to invalid except allowed stack
		for (int i = 0; i < 1024; i++) {
			((pte_4kib*)new_pte)[i] = (pte_4kib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .pat = 0, .g = 1, .ignored = 0, .addr = ((base_esp_addr >> 22) << 10) + i};
		}
		// Set the PTE for the allowed stack
		((pte_4kib*)new_pte)[(base_esp_addr << 10) >> 22] = (pte_4kib){.present = 1, .rw = 1, .us = 1, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .pat = 0, .g = 1, .ignored = 0, .addr = base_esp_addr >> 12};
	}

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
