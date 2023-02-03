#include <bios.h>
#include <vesa/vbe.h>

extern uint16_t drive_bytes_per_sector;

void stage2() {
	init_vbe();
	bios_ext_read(0x100000, 0x100 * 512 / drive_bytes_per_sector, 1);

	while (1) {}
}
