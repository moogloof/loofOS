#ifndef _REGISTERS_H
#define _REGISTERS_H

#include <stdint.h>

typedef struct {
	uint32_t edi; // String operation registers
	uint32_t esi;
	uint32_t ebp; // Base pointer
	uint32_t ebx; // General registers
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
} gen_register_set; // Symmetric of pusha

typedef struct {
	uint32_t ds;
	uint32_t es;
	uint32_t fs;
	uint32_t gs;
} seg_register_set; // Segment registers not included in the interrupt frame

#endif
