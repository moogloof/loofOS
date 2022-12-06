#include <stdint.h>
#include <core/panic.h>
#include <core/isr.h>
#include <core/registers.h>
#include <drivers/vga_text.h>

// Do kernel panic function
void kernel_panic(const char* msg, seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame, uint32_t ecode) {
	reset_display();
	set_color(0, 4);
	// Display kernel panic message
	kernel_print(msg);
	kernel_print("\r\n\r\n");
	// Display error code
	kernel_print("ECODE: %b\r\n\r\n", ecode);
	// Display the registers
	// Seg regs
	kernel_print("seg_regs {\r\n");
	kernel_print("    DS=%x    ES=%x\r\n", seg_regs.ds, seg_regs.es);
	kernel_print("    FS=%x    GS=%x\r\n", seg_regs.fs, seg_regs.gs);
	kernel_print("}\r\n");
	// Gen regs
	kernel_print("gen_regs {\r\n");
	kernel_print("    EDI=%x    ESI=%x    EBP=%x\r\n", gen_regs.edi, gen_regs.esi, gen_regs.ebp);
	kernel_print("    EBX=%x    EDX=%x    ECX=%x\r\n", gen_regs.ebx, gen_regs.edx, gen_regs.ecx);
	kernel_print("    EAX=%x\r\n", gen_regs.eax);
	kernel_print("}\r\n");
	// Interrupt frame
	kernel_print("interrupt_frame {\r\n");
	kernel_print("    EIP=%x    CS=%x\r\n", frame.eip, frame.cs);
	kernel_print("    EFLAGS=%b\r\n", frame.eflags);
	kernel_print("    ESP=%x    SS=%x\r\n", frame.esp, frame.ss);
	kernel_print("}\r\n");

	// Halt
	while (1) {}
}
