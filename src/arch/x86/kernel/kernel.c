#include <drivers/vga_text.h>

__attribute__((section(".text.kernel"))) void kernel_main() {
	// Reset the vga and display stuff
	reset_display();
	display_string("Hello, world!", 0, 0);

	set_color(0, 0xb);
	display_string("Bello, borld!", 24, 20);

	while (1) {
	}
}
