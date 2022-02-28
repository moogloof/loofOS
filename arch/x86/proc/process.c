#include <proc/process.h>
#include <mm/alloc.h>

// Flag whether entering usermode for the first time or not
static uint8_t entering_user = 1;

// Current process in context
static process_desc* current_process = 0;

// Previous process in context
static process_desc* previous_process = 0;

// Switch context
void switch_process(seg_register_set* seg_regs, gen_register_set* gen_regs, interrupt_frame* frame) {
	// No process is no switch
	if (!current_process) {
		return;
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
}

// Create a process
void create_process(uint32_t eip) {
	process_desc* new_process = kernel_allocate(sizeof(process_desc));

	// Set address of process
	new_process->frame.eip = eip;
	// Set code segment
	new_process->frame.cs = 0x8;
	// Set eflags
	// IMPORTANT: Make sure to set the interrupt enable flag
	new_process->frame.eflags = 1 << 9;
	new_process->frame.esp = kernel_allocate(4096) + 4092;
	new_process->frame.ss = 0x10;

	// Set data segments
	new_process->seg_regs.ds = 0x10;
	new_process->seg_regs.es = 0x10;
	new_process->seg_regs.fs = 0x10;
	new_process->seg_regs.gs = 0x10;

	new_process->gen_regs.edi = 0;
	new_process->gen_regs.esi = 0;
	new_process->gen_regs.ebp = new_process->frame.esp;
	new_process->gen_regs.ebx = 0;
	new_process->gen_regs.edx = 0;
	new_process->gen_regs.ecx = 0;
	new_process->gen_regs.eax = 0;

	new_process->state = 0;

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
