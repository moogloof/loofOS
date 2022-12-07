#include <stdint.h>
#include <drivers/fs/iso9660.h>
#include <sys/kernel_console.h>
#include <drivers/vbe.h>

// The console text framebuffer
static volatile uint8_t* console_framebuffer;

// The buffer for fonts
static uint8_t* kernel_font_buffer;
// The console grid
static int console_width;
static int console_height;
static int console_pos;

// Initialize the kernel console
void init_kernel_console() {
	// Get framebuffer
	console_framebuffer = obtain_framebuffer();

	// Load the fonts
	kernel_font_buffer = iso_read_file("SYSTEM/FONT.PSF\0")->file_buffer;

	// Get all the window info
	console_pos = 0;
	console_width = get_window_info()->xresolution / 8;
	console_height = get_window_info()->yresolution / 16;
}

// The function to write some char to the console
void console_write(char c) {
	// Write stuff
	if (c == '\n') {
		console_pos += console_width;
	} else if (c == '\r') {
		console_pos -= console_pos % console_width;
	} else if (c == '\t') {
		console_pos += 4;
	} else {
		// Write char from stuff
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 8; j++) {
				uint8_t pixel_fill = ((kernel_font_buffer[4 + 16 * c + i] >> (7 - j)) & 1) ? 0xff : 0;
				int console_index = (i + 16*(console_pos / console_width)) * get_window_info()->bytes_per_line + (j + 8*(console_pos % console_width))*3;
				console_framebuffer[console_index] = pixel_fill;
				console_framebuffer[console_index + 1] = pixel_fill;
				console_framebuffer[console_index + 2] = pixel_fill;
			}
		}

		// Wrap around
		console_pos++;
		console_pos %= console_height * console_width;
	}
}
