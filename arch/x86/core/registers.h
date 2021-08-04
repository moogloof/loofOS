#ifndef _REGISTERS_H
#define _REGISTERS_H

#include <stdint.h>

typedef struct {
	uint32_t eax; // General registers
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t esi; // String operation registers
	uint32_t edi;
	uint32_t ebp; // Base pointer
} gen_register_set;

// Save current registers to the register set passed
gen_register_set dump_registers();

// Load passed register set to current register set
void load_registers(gen_register_set);

#endif
