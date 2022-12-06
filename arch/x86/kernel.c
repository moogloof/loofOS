#include <sys/kernel_print.h>
#include <sys/kernel_console.h>
#include <drivers/timer.h>
#include <drivers/rtc.h>
#include <drivers/ps2/controller.h>
#include <drivers/ps2/keyboard.h>
#include <drivers/vbe.h>
#include <core/isr.h>
#include <core/idt.h>
#include <mm/paging.h>
#include <mm/alloc.h>
#include <proc/process.h>

extern uint8_t context_switching;

__attribute__((section(".kernel"), noreturn)) void kernel_main() {
	// Console placeholder
	set_outb(&console_write);

	// Setup the kernel heap
	init_kernel_heap();

	// Allocate test values
	int* test1 = (int*)kernel_allocate(sizeof(int));
	int* test2 = (int*)kernel_allocate(sizeof(int));
	*test1 = 0x11223344;
	*test2 = 0xabcdef12;

	// Load the GDT
	init_gdt();
	load_gdt();

	// Load the interrupt descriptor table
	init_idt();
	// Initialize the timer (IRQ0)
	init_timer();
	// Initialize the PS/2 controller
	init_ps2_controller();
	// Initialize the PS/2 keyboard
	init_ps2_keyboard();
	// Initialize the ATA
	init_ata();
	// Initialize PIC
	init_pic();
	// Initialize exception handlers
	init_exceptions();
	// Load the IDT
	load_idt();

	// Setup paging
	init_paging();

	// Initialize processes
	init_processes();

	init_vbe();

	// Unmask interrupts
	enable_interrupts();

	// Reset the vbe and display stuff
	init_kernel_console();
	kernel_print("Interrupts enabled.\r\n");

	// Initialize the RTC
	init_rtc();
	kernel_print("Initialized RTC.\r\n\r\n");
	// Get time
	time_struct cur_time = read_time();
	// Display time
	kernel_print("It's %d:%d, on %d/%d/%d\r\n", (int)cur_time.hours, (int)cur_time.minutes, (int)cur_time.month, (int)cur_time.day_month, (int)cur_time.year);

	// Start context switching and go into usermode
	kernel_print("Enabling context switching and entering user mode.\r\n");

	create_process(0x00000000, 3);

	context_switching = 1;

	while (1) {
	}
}
