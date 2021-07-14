#include <stdint.h>
#include <core/port.h>
#include <drivers/timer.h>
#include <core/idt.h>
#include <core/isr.h>
#include <proc/queue.h>

static uint8_t cur_char = 'A';

// Initialize the timer interrupt
void init_timer() {
	// Add IRQ0
	set_id(IRQ_OFFSET, timer_handler, 0x08, IDT_PROT_INTR, 0, 1);

	// Set mode register
	outportb(TIMER_MODE, 0b00110100);

	//  Set channel0 divider for timer interrupts
	//  Low byte
	outportb(TIMER_CHANNEL0, 0xff);
	// High byte
	outportb(TIMER_CHANNEL0, 0x00);
}

// Timer handler
__attribute__((interrupt)) void timer_handler(struct interrupt_frame* frame) {
	display_char(cur_char++, 0, 0);
	cur_char %= 128;
	// Call switch process for stuff
//	switch_process();

	// Send EOI
	outportb(PIC_COMMAND1, PIC_EOI);
}
