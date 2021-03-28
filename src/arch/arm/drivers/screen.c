#include "screen.h"
#include <stdint.h>

// VGA text buffer
// VGA text buffer bit data format is BBBBFFFFCCCCCCCC
// B is background color, F is foreground color, and C is the char value
volatile uint16_t* vga_buffer = (uint16_t*)VIDEO_ADDRESS;
// Default color
const uint8_t default_color = 0x0F;

// Clear screen func
void screen_reset() {
	// Set entire buffer to space
	for (int row = 0; row < VIDEO_ROWS; row++) {
		for (int col = 0; col < VIDEO_COLS; col++) {
			// Set screen buffer space to empty space char
			vga_buffer[row * VIDEO_COLS + col] = ((uint16_t)default_color << 8) | ' ';
		}
	}
}

// Display char on screen func
// Use coordinates (x, y) and char
void screen_putc(char c, uint8_t x, uint8_t y) {
	// Replace text at (x, y) with c if available
	if (x < VIDEO_COLS && y < VIDEO_ROWS)
		vga_buffer[y * VIDEO_COLS + x] = ((uint16_t)default_color << 8) | c;
}

// Display string on screen func
// Use coordinates (x, y) and str
void screen_print(char* str, uint8_t x, uint8_t y) {
	// Copy str pointer for non-updating indexing
	char* strp = str;
	// Coords for char output
	uint8_t cx = x;
	uint8_t cy = y;

	// Display all chars in str
	do {
		// Display at coords
		screen_putc(*strp, cx, cy);

		// Calculate next coords
		cy += (++cx) / VIDEO_ROWS;
		cx %= VIDEO_ROWS;
	} while (*(++strp) && cy < VIDEO_ROWS); // Check if terminating char or overflow
}
