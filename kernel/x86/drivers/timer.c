#include <stdint.h>
#include <core/port.h>
#include <drivers/timer.h>
#include <core/idt.h>
#include <core/isr.h>
#include <proc/process.h>
#include <mm/paging.h>
#include <string.h>

// Switching context enable flag
static uint8_t switching_enabled;

// Initialize the timer interrupt
void init_timer(void) {
	// Zero out vars
	switching_enabled = 0;

	// Divider for the interrputs
	uint16_t divider = TIMER_FREQ / TIMER_INT_FREQ;

	// Add IRQ0
	set_id(IRQ_OFFSET, &timer_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);

	// Set mode register
	outportb((uint8_t*)TIMER_MODE, 0x34);

	//  Set channel0 divider for timer interrupts
	//  Low byte
	outportb((uint8_t*)TIMER_CHANNEL0, (uint8_t)(divider & 0xff));
	// High byte
	outportb((uint8_t*)TIMER_CHANNEL0, (uint8_t)((divider >> 8) & 0xff));
}

// Enable switching (YOU CAN'T GO BACK)
void enable_switching(void) {
	switching_enabled = 1;
}

// Timer handler
void timer_handler(seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame) {
	// Switch context if enabled
	if (switching_enabled) {
		switch_process(seg_regs, gen_regs, frame);
	}

	// Send EOI
	outportb((uint8_t*)PIC_COMMAND1, PIC_EOI);
}
