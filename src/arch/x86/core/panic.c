#include <stdint.h>
#include <core/panic.h>
#include <drivers/vga_text.h>

// Do kernel panic function
void kernel_panic(char* msg) {
	// Display kernel panic message
	reset_display();
	set_color(0, 4);
	display_string(msg, 0, 0);

	// Halt
	while (1) {}
}
