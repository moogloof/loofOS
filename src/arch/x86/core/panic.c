#include <stdint.h>
#include <core/panic.h>
#include <core/isr.h>
#include <core/registers.h>

// Do kernel panic function
void kernel_panic(const char* msg, seg_register_set seg_regs, gen_register_set gen_regs, interrupt_frame frame, uint32_t ecode) {
	// Halt
	while (1) {}
}
