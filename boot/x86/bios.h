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

// Address range descriptor
typedef struct {
	uint32_t base_addr_low;
	uint32_t base_addr_high;
	uint32_t size_low;
	uint32_t size_high;
	uint32_t type; // 1 = usable
		       // 2 = reserved
		       // 3 = ACPI reclaimable
		       // 4 = ACPI NVS
		       // 5 = bad
	uint32_t acpi_attributes; // Bit 0 is clear if whole entry should be ignored
				  // Bit 1 is set if entry is non-volatile
				  // Might be 0 in case
} __attribute__((packed)) address_range;

// BIOS ext read
void bios_ext_read(uint8_t*, uint32_t, uint32_t);

// BIOS mem scan
void bios_mem_scan(address_range*, int*);

// BIOS int
void _bios_int(bios_registers, uint8_t, bios_registers*);

#endif
