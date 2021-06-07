#include <stdint.h>
#include <drivers/vga_text.h>
#include <core/port.h>

// The VGA text buffer is at 0xb8000
volatile uint16_t* text_buffer = (uint16_t*)0xb8000;
// The color of the writing
uint8_t color = 0x0f;

// Display a character on screen
// The position of the char (row, col)
void display_char(char c, int row, int col) {
	// Write the character to the corresponding position in the buffer with the color
	text_buffer[80*row + col] = ((uint16_t)color << 8) | (uint16_t)c;
}

// Display a string on screen
// The start position of the string (row, col)
void display_string(const char* str, int row, int col) {
	// Offset from the string pointer
	int offset = 0;

	// Write the string sequentially
	while (*(str + offset)) {
		// Display the char
		display_char(*(str + offset++), row, col);

		// Update the position for the next char
		row += (++col) / 80;
		col %= 80;
	}
}

// Clear the screen and set the vga stuff
void reset_display() {
	// Clear the screen
	for (int i = 0; i < 2000; i++) {
		display_char(' ', i / 80, i % 80);
	}
}

// Set the background and foreground color
// The color follows VGA color tables
void set_color(int b, int f) {
	// Set color
	color = (uint8_t)((b << 4) | f);
}

// Enable the cursor with vga ports
void enable_cursor(uint8_t start, uint8_t end) {
	// Set the start of the cursor
	// Select the 0Ah cursor start register
	outportb(0x3d4, 0x0a);
	// Enable the cursor and set the start scanline
	outportb(0x3d5, (0b000 << 5) | start);

	// Set the end of the cursor
	// Select the 0Ah cursor end register
	outportb(0x3d4, 0x0b);
	// Set the end scanline
	outportb(0x3d5, (0b000 << 5) | end);
}

// Disable the cursor with vga ports
void disable_cursor() {
	// Set the start of the cursor
	// Select the 0Ah cursor start register
	outportb(0x3d4, 0x0a);
	// Write to cursor start register to disable
	outportb(0x3d5, 0b00100000);
}

// Set the cursor position with vga ports
void cursor_pos(int row, int col) {
	// Calculate the cursor pos
	int pos = row*80 + col;

	// Set the low cursor location
	// Select the 0Fh cursor low pos register
	outportb(0x3d4, 0x0f);
	// Set the low position bits
	outportb(0x3d5, (uint8_t)(pos & 0xff));

	// Set the high cursor location
	// Select the 0Eh cursor high pos register
	outportb(0x3d4, 0x0e);
	// Set the high position bits
	outportb(0x3d5, (uint8_t)((pos >> 8) & 0xff));
}
