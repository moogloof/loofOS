#ifndef SCREEN_DRIVER_H
#define SCREEN_DRIVER_H

#include <stdint.h>

// Video buffer start address
#define VIDEO_ADDRESS 0xB8000
#define VIDEO_COLS 80
#define VIDEO_ROWS 25

// Clear screen
void screen_reset();

// Display char on screen at (x, y)
void screen_putc(char, uint8_t, uint8_t);

// Display string on screen, starting at (x, y)
void screen_print(char*, uint8_t, uint8_t);

#endif
