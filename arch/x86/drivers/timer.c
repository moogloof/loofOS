#include <stdint.h>
#include <core/port.h>
#include <drivers/timer.h>
#include <drivers/vga_text.h>
#include <core/idt.h>
#include <core/isr.h>
#include <sys/kernel_print.h>

// Initialize the timer interrupt
void init_timer() {
	// Divider for the interrputs
	uint16_t divider = TIMER_FREQ / TIMER_INT_FREQ;

	// Add IRQ0
	set_id(IRQ_OFFSET, timer_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);

	// Set mode register
	outportb(TIMER_MODE, 0b00110100);

	//  Set channel0 divider for timer interrupts
	//  Low byte
	outportb(TIMER_CHANNEL0, (uint8_t)(divider & 0xff));
	// High byte
	outportb(TIMER_CHANNEL0, (uint8_t)((divider >> 8) & 0xff));
}

// Timer handler
void timer_handler(seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame) {
	set_cursor_pos(2, 0);
	kernel_print("EAX=%x, EBX=%x, ECX=%x, EDX=%x\r\n", gen_regs.eax, gen_regs.ebx, gen_regs.ecx, gen_regs.edx);
	kernel_print("ESI=%x, EDI=%x, EBP=%x, CS=%x\r\n", gen_regs.esi, gen_regs.edi, gen_regs.ebp, frame.cs);
	kernel_print("EIP=%x, EFLAGS=%x\r\n", frame.eip, frame.eflags);
	kernel_print("DS=%x, ES=%x, FS=%x, GS=%x", seg_regs.ds, seg_regs.es, seg_regs.fs, seg_regs.gs);

	// Send EOI
	outportb(PIC_COMMAND1, PIC_EOI);
}
