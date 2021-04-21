#include <drivers/timer.h>
#include <drivers/uart.h>
#include <peripherals/timer.h>
#include <util.h>
#include <print.h>
#include <stdint.h>


// Timer interrupt interval
const uint32_t interval = 10;
uint32_t cmpval = 0;

// Clear timer statuses
void clear_timer(int tid) {
	put32(TIMER_CS, (1 << tid));
}

// Initialize timer
void init_timer() {
	cmpval = get32(TIMER_CLO);
	cmpval += interval;
	put32(TIMER_C1, cmpval);
}

// Get system timer counter
uint64_t count_timer() {
	return ((uint64_t)get32(TIMER_CHI) << 32) | get32(TIMER_CLO);
}

// Handle timer interrupt
void irq_timer_handler() {
	cmpval += interval;
	put32(TIMER_C1, cmpval);
	clear_timer(1);
	print(&uart_sendstr, "Timer interrupt handled.");
}

