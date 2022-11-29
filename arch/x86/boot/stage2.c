#include <stdint.h>
#include <boot/iso9660.h>

// Get the saved mode info
extern struct vbe_mib __attribute__((packed)) saved_mode_info_block;
extern char* logo_image;
extern uint32_t primary_volume_descriptor_position;

// The main function for the second stage boot
void stage2_boot() {
	// The record to read
	primary_volume_descriptor vol_desc;
	uint8_t read_success;

	while (1);
}
