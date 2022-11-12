#include <sys/kernel_print.h>
#include <drivers/vga_text.h>
#include <drivers/timer.h>
#include <drivers/rtc.h>
#include <drivers/ps2/controller.h>
#include <drivers/ps2/keyboard.h>
#include <core/isr.h>
#include <core/idt.h>
#include <mm/paging.h>
#include <mm/alloc.h>
#include <proc/process.h>

extern uint8_t context_switching;

__attribute__((section(".kernel"), noreturn)) void kernel_main() {
	// Reset the vga and display stuff
	reset_display();

	// Set outb for printing
	set_outb(output_char);

	// Start screen stuff
	kernel_print("Setup VGA text mode display.\r\n");
	set_color(0, 5);
	kernel_print("loofOS v1.0\r\n\r\n");
	set_color(0, 0xf);

	// Setup the kernel heap
	init_kernel_heap();
	kernel_print("Initialized kernel heap.\r\n\r\n");

	// Allocate test values
	int* test1 = (int*)kernel_allocate(sizeof(int));
	int* test2 = (int*)kernel_allocate(sizeof(int));
	*test1 = 0x11223344;
	*test2 = 0xabcdef12;
	kernel_print(" TEST %x %x\r\n\r\n", test1, test2);

	// Load the GDT
	init_gdt();
	load_gdt();

	// Load the interrupt descriptor table
	init_idt();
	// Initialize the timer (IRQ0)
	init_timer();
	kernel_print("Initialized timer.\r\n");
	// Initialize the PS/2 controller
	init_ps2_controller();
	kernel_print("Initialized PS/2 controller.\r\n");
	// Initialize the PS/2 keyboard
	init_ps2_keyboard();
	kernel_print("Initialized PS/2 keyboard.\r\n");
	// Initialize PIC
	init_pic();
	kernel_print("Initialized PIC.\r\n");
	// Initialize exception handlers
	init_exceptions();
	kernel_print("Initialized exception handlers.\r\n");
	// Load the IDT
	load_idt();
	kernel_print("Loaded IDT.\r\n");

	// Setup paging
	init_paging();
	kernel_print("Initialized paging.\r\n\r\n");
	kernel_print(" KDIR %x USTART %x UEND %x UTOT %x\r\n\r\n", KERNEL_PAGE_DIRECTORY, PAGELIST_START, PAGELIST_END - 1, PAGELIST_END - PAGELIST_START);

	// Initialize processes
	init_processes();

	// Unmask interrupts
	enable_interrupts();
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
