#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include <core/isr.h>

// Initialize the timer interrupt
void init_timer();

// Timer handler
__attribute__((interrupt)) void timer_handler(struct interrupt_frame*);

#endif
