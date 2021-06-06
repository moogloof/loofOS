#ifndef VGA_TEXT_DRIVER_H
#define VGA_TEXT_DRIVER_H

// Display a character on screen
void display_char(char, int, int);

// Display a string on screen
void display_string(const char*, int, int);

// Clear the screen
void reset_display();

// Set the color of the background and foreground
void set_color(int, int);

#endif
