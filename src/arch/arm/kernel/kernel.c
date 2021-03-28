// Include drivers
#include <drivers/screen.h>

// Main kernel func
void kernel_main() {
	// Reset screen
	screen_reset();

	// Hello world thing
	screen_print("Hello, world!", 0, 0);
}
