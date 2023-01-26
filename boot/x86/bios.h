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

// BIOS ext read
void bios_ext_read(uint8_t*, uint32_t, uint32_t);

// BIOS ext read helper
void _bios_ext_read_helper(bios_dap);

#endif
