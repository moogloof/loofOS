#ifndef BIOS
#define BIOS

#include <stdint.h>

// DAP Disk Address Packet
typedef struct {
	uint8_t dap_size;
	uint8_t unused;
	uint16_t load_offset;
	uint16_t load_segment;
	uint32_t lba_low;
	uint16_t lba_high;
	uint16_t reserved;
} __attribute__((packed)) bios_dap;

void bios_ext_read(uint16_t, uint8_t, );

void _bios_ext_read_helper(uint16_t, uint8_t, 

#endif
