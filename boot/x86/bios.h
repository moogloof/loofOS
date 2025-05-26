#ifndef BIOS
#define BIOS

#include <stdint.h>

// DAP Disk Address Packet
typedef struct {
	uint8_t dap_size;
	uint8_t unused;
	uint16_t load_size;
	uint16_t load_offset;
	uint16_t load_segment;
	uint32_t lba_low;
	uint32_t lba_high;
} __attribute__((packed)) bios_dap;

// Registers
typedef struct {
	uint32_t eax;
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t esi;
	uint32_t edi;
} __attribute__((packed)) bios_registers;

// BIOS ext read
void bios_ext_read(uint8_t*, uint32_t, uint32_t);

// BIOS int
void _bios_int(bios_registers, uint8_t, bios_registers*);

#endif
