#include <drivers/vga_text.h>

__attribute__((section(".text.kernel"))) void kernel_main() {
	// Reset the vga and display stuff
	reset_display();
	// Set cursor as max height
	enable_cursor(0, 15);

	cursor_pos(0, 0);

	display_string("Hello, world!", 0, 0);

	set_color(0, 0xb);
	display_string("Bello, borld!", 24, 20);

	while (1) {
	}
}
