#include <boot/vbe.h>

// Get the saved mode info
extern struct vbe_mib __attribute__((packed)) saved_mode_info_block;
extern char* logo_image;

// The main function for the second stage boot
void stage2_boot() {
	// Draw the logo
	draw_xpm(saved_mode_info_block.xresolution / 2 - 49, saved_mode_info_block.yresolution / 2 - 49, 100, 100, logo_image);

	while (1) {}
}
