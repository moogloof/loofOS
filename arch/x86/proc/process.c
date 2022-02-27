#include <proc/process.h>

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
	current_process->gen_regs.esp = gen_regs->esp;
	current_process->gen_regs.ebx = gen_regs->ebx;
	current_process->gen_regs.edx = gen_regs->edx;
	current_process->gen_regs.ecx = gen_regs->ecx;
	current_process->gen_regs.eax = gen_regs->eax;

	// Update current process
	current_process = current_process->next;

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
	gen_regs->esp = current_process->gen_regs.esp;
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

	// Update previous context
	previous_process = current_process;
}
