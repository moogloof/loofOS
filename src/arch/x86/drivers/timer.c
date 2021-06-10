#include <stdint.h>
#include <drivers/timer.h>
#include <core/idt.h>
#include <core/isr.h>

// Initialize the timer interrupt
void init_timer() {
	// Add IRQ0
	set_id(IRQ_OFFSET, timer_handler, 0x08, IDT_PROT_INTR, 0, 1);
}

// Timer handler
__attribute__((interrupt)) void timer_handler(struct interrupt_frame* frame) {
}
