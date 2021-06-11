#include <drivers/vga_text.h>
#include <drivers/timer.h>
#include <core/isr.h>
#include <core/idt.h>

__attribute__((section(".text.kernel"))) void kernel_main() {
	// Reset the vga and display stuff
	reset_display();
	// Set cursor as max height and position at origin
	enable_cursor(0, 15);

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

	while (1) {
	}
}
