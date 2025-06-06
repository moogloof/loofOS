#ifndef BOOT_VGA
#define BOOT_VGA

#include <stdint.h>

// VGA text ports
// VGA text index port
#define VGA_TEXT_INDEX 0x3d4
// VGA text data port
#define VGA_TEXT_DATA 0x3d5

// Display a character on screen
void display_char(char, int, int);

// Display a string on screen
void display_string(const char*, int, int);

// Clear the screen
void reset_display(void);

// Set the color of the background and foreground
void set_color(int, int);

// Enable the cursor
void enable_cursor(uint8_t, uint8_t);

// Disable the cursor
void disable_cursor(void);

// Set the cursor position
void set_cursor_pos(int, int);

// Get the cursor position
uint16_t get_cursor_pos(void);

// Output method for prints
void vga_output_char(char);

#endif
