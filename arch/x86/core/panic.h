#ifndef _PANIC_H
#define _PANIC_H

#include <core/isr.h>
#include <core/registers.h>

// Do kernel panic function
void kernel_panic(const char*, seg_register_set, gen_register_set, interrupt_frame, uint32_t);

#endif
