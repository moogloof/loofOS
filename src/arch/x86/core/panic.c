#include <stdint.h>
#include <core/panic.h>
#include <core/isr.h>
#include <drivers/vga_text.h>

// Do kernel panic function
void kernel_panic(char* msg) {
	// Disable interrupts
	disable_interrupts();

	// Display kernel panic message
	reset_display();
	set_color(0, 4);
	display_string(msg, 0, 0);

	// Halt
	while (1) {}
}
