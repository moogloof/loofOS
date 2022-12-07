#ifndef PS2_KEYBOARD_DRIVER_H
#define PS2_KEYBOARD_DRIVER_H

#include <core/isr.h>

// Empty/not processed key
#define NOK 0

// Initialize the PS/2 Keyboard device
void init_ps2_keyboard();

// PS/2 Keyboard handler
void keyboard_handler();
__attribute__((interrupt)) void keyboard_handler_wrapper(interrupt_frame*);

#endif
