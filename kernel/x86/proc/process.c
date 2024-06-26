#include <proc/process.h>
#include <string.h>
#include <presets.h>
#include <mm/alloc.h>
#include <mm/paging.h>
#include <core/gdt.h>

// All externs
extern pde_4mib kernel_memory[PAGE_LENGTH_4M];

// Current process in context
static process_desc* current_process;

// Init processes
void init_processes(void) {
	// Zero out vars
	current_process = 0;
}

// Force enter userland
void force_enter_userland(void) {
	set_tss_stack(current_process->esp0);
	switch_context(current_process->seg_regs, current_process->gen_regs, current_process->frame, current_process->page_directory - KERNEL_BASE);
}

// Switch context
// Round robin
// PLANS: WE NEED TO DO STUFF
void switch_process(seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame) {
	// No process is no switch
	if (!current_process || current_process->next == current_process) {
		return;
	}

	// Get priv level of interrupted thread
	uint8_t priv_level = frame.cs & 3;

	// Check if current process is running
	if (current_process->state == PROCESS_RUNNING) {
		// Freeze registers
		// Frame registers
		current_process->frame = frame;
		// Segment registers
		current_process->seg_regs = seg_regs;
		// General registers
		current_process->gen_regs = gen_regs;
	} else if (current_process->state == 2) {
		// On state ended, delete process
		kernel_free((void*)current_process->frame.esp);
		kernel_free((void*)current_process);

		// Clear current_process
		if (current_process == current_process->next) {
			// TODO: Properly handle this case
			current_process = 0;
		} else {
			current_process->prev->next = current_process->next;
			current_process->next->prev = current_process->prev;
		}
	}

	// Update stack for stuf bc stack won't change
	if (priv_level == RING_KERNEL) {
		// This is super important to remember
		// SUPER IMPORTANT
		// When the context switches from a kernel thread, the stack doesn't change
		// But when we go back, we'll have to change back to the stack
		// It's tricky to do that, but we can use a little magic trick
		// The interrupt arguments will be preserved on the stack
		// This is super hacky in my opinion, but for now it works
		// So yeah
		current_process->frame.ss = 0x10;
		current_process->frame.esp = current_process->gen_regs.esp;
	} else {
		current_process->gen_regs.esp = 0;
	}

	// Update current process
	// Skip all halted processes
	do {
		// Update previous context
		current_process = current_process->next;
	} while (current_process->state == PROCESS_HALTED);

	// Set TSS
	set_tss_stack(current_process->esp0);

	switch_context(current_process->seg_regs, current_process->gen_regs, current_process->frame, current_process->page_directory - KERNEL_BASE);
}

// Create a process
// Kernel proceses should be explicitly defined within the kernel code
// No spawning kernel processes through some user interface
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
	new_process->gen_regs.esp = 0;

	// Get new kernel stack
	new_process->esp0 = (uint32_t)kernel_allocate(4096) + 4092;
	// Set the page directory
	new_process->page_directory = (uint32_t)kernel_allocate(sizeof(pde_4kib) * 1024);
	// Copy kernel space to user space
	for (int i = 0; i < 256; i++) {
		((pde_4mib*)new_process->page_directory)[i + 768] = kernel_memory[i + 768];
	}
	// Allocate stack page
	// Calculate base address of stack
	allocate_page((pde_4kib*)new_process->page_directory, KERNEL_BASE - PAGE_SIZE_4K);

	// Allocate first page
	allocate_page((pde_4kib*)new_process->page_directory, 0);

	enable_paging((pde_4mib*)(new_process->page_directory - KERNEL_BASE));

	volatile char* a = 0;
	a[0] = 0xe9;
	a[1] = 0xfb;
	a[2] = 0xff;
	a[3] = 0xff;
	a[4] = 0xff;

	enable_paging((pde_4mib*)((uint32_t)kernel_memory - KERNEL_BASE));

	// Set state of process as running
	new_process->state = 0;

	// Insert process into process queue
	if (!current_process) {
		current_process = new_process;
		current_process->next = new_process;
		current_process->prev = new_process;
	} else {
		new_process->next = current_process;
		new_process->prev = current_process->prev;
		current_process->prev->next = new_process;
		current_process->prev = new_process;
	}
}

// End a process
void update_process(uint8_t state) {
	current_process->state = state;
}
