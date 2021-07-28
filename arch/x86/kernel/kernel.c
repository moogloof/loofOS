#include <sys/kernel_print.h>
#include <drivers/vga_text.h>
#include <drivers/timer.h>
#include <drivers/rtc.h>
#include <core/isr.h>
#include <core/idt.h>

__attribute__((section(".text.kernel"))) void kernel_main() {
	// Reset the vga and display stuff
	reset_display();
	// Set cursor as max height and position at origin
	enable_cursor(0, 15);
	// Set outb for printing
	set_outb(output_char);

	// Start screen stuff
	display_string("Hello, world!", 0, 0);
	set_color(0, 0xb);
	display_string("Bello, borld!", 24, 20);

	// Load the interrupt descriptor table
	// Initialize the timer (IRQ0)
	init_timer();
	// Initialize PIC
	init_pic();
	// Initialize exception handlers
	init_exceptions();
	// Load the IDT
	load_idt();

	// Unmask interrupts
	enable_interrupts();

	// Initialize the RTC
	init_rtc();
	// Get time
	time_struct cur_time = read_time();
	// Display time
	set_cursor_pos(1, 0);
	kernel_print("It's %d:%d\r\n", (int)cur_time.hours, (int)cur_time.minutes);

	while (1) {
	}
}
