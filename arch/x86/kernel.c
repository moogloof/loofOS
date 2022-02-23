#include <sys/kernel_print.h>
#include <drivers/vga_text.h>
#include <drivers/timer.h>
#include <drivers/rtc.h>
#include <drivers/ps2/controller.h>
#include <drivers/ps2/keyboard.h>
#include <core/isr.h>
#include <core/idt.h>
#include <mm/paging.h>

__attribute__((section(".kernel"), noreturn)) void kernel_main() {
	// Reset the vga and display stuff
	reset_display();

	// Set outb for printing
	set_outb(output_char);

	// Start screen stuff
	kernel_print("Setup VGA text mode display.\r\n");

	// Load the interrupt descriptor table
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

	// Unmask interrupts
	enable_interrupts();
	kernel_print("Interrupts enabled.\r\n");

	// Initialize the RTC
	init_rtc();
	kernel_print("Initialized RTC.\r\n");
	// Get time
	time_struct cur_time = read_time();
	// Display time
	kernel_print("It's %d:%d, on %d/%d/%d\r\n", (int)cur_time.hours, (int)cur_time.minutes, (int)cur_time.month, (int)cur_time.day_month, (int)cur_time.year);

	while (1) {
	}
}
