#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include <core/isr.h>

// Peripheral ports for timer
// PIT channels
#define TIMER_CHANNEL0 0x40
#define TIMER_CHANNEL1 0x41
#define TIMER_CHANNEL2 0x42
// PIT mode
#define TIMER_MODE 0x43

// Initialize the timer interrupt
void init_timer();

// Timer handler
__attribute__((interrupt)) void timer_handler(interrupt_frame*);

#endif
