#include <bios.h>

extern uint16_t drive_bytes_per_sector;

volatile uint8_t* output;

void stage2() {
	output = 0x100000;

	bios_ext_read(output, 0, 1);

	while (1) {}
}
