#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include <core/isr.h>
#include <core/registers.h>

// Peripheral ports for timer
// PIT channels
#define TIMER_CHANNEL0 0x40
#define TIMER_CHANNEL1 0x41
#define TIMER_CHANNEL2 0x42
// PIT mode
#define TIMER_MODE 0x43

// Frequency for the timer
#define TIMER_FREQ 1193182
#define TIMER_INT_FREQ 2000

// Initialize the timer interrupt
void init_timer();

// Wrapper for timer handler
__attribute__((interrupt)) void timer_handler_wrapper(interrupt_frame*);

// Timer handler
void timer_handler(seg_register_set, gen_register_set, interrupt_frame);

#endif
