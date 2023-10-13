#include <drivers/timer.h>
#include <core/isr.h>
#include <core/idt.h>
#include <core/gdt.h>
#include <mm/paging.h>
#include <mm/alloc.h>
#include <proc/process.h>

__attribute__((section(".kernel"), noreturn)) void kernel_main() {
	// Load and init the gdt
	init_gdt();
	load_gdt();

	// Setup the kernel heap
	init_kernel_heap();

	// Allocate test values
	int* test1 = (int*)kernel_allocate(sizeof(int));
	int* test2 = (int*)kernel_allocate(sizeof(int));
	*test1 = 0x11223344;
	*test2 = 0xabcdef12;

	// Load the interrupt descriptor table
	init_idt();
	// Initialize the timer (IRQ0)
	init_timer();
	// Initialize PIC
	init_pic();
	// Load the IDT
	load_idt();

	// Setup paging
	init_paging();

	// Initialize processes
	init_processes();

	// Unmask interrupts
//	enable_interrupts();

	// IMPORTANT: STUFF THIS BEFORE ENABLING SWITCHING
	create_process(0);
	create_process(0);

	// Enable switching
	enable_switching();

	// Force enter into user
	// This should be the only time this is used
	force_enter_userland();

	// Stuff
	while (1) {}
}
