#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include <stdint.h>

// Clear timer statuses
void clear_timer(int);
// Initialize timer
void init_timer();
// Get system timer counter
uint64_t count_timer();
// Timer interrupt handler
void irq_timer_handler();

#endif
