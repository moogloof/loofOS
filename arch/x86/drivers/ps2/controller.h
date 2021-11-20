#ifndef PS2_CONTROLLER_DRIVER_H
#define PS2_CONTROLLER_DRIVER_H

#include <stdint.h>

// PS/2 Controller IO ports
// Data port
#define PS2_DATA 0x60
// Status port
#define PS2_STATUS 0x64
// Command port
#define PS2_COMMAND 0x64

// Initialize the PS/2 Controller
void init_ps2_controller();

// Poll from the data port
uint8_t poll_ps2_dataport();

// Write to the ps2 dataport
void write_ps2_dataport(uint8_t);

#endif
