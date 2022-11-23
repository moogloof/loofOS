#include <stdint.h>
#include <boot/vbe.h>

// The VBE info block
struct vbe_cib __attribute__((packed)) vbe_info_block;

// The VBE mode info block
struct vbe_mib __attribute__((packed)) mode_info_block;
// The saved one
struct vbe_mib __attribute__((packed)) saved_mode_info_block;

// The dimensions of the thing
// The selected dimensions
uint16_t selected_width;
uint16_t selected_height;
uint16_t selected_vbe_mode;

// Draw xpm simple monocolor
void draw_xpm(int x, int y, int w, int h, char* image) {
	// The pixel mask
	char pixel_mask;
	// The physical base ptr to go to
	char* phys_base_ptr = saved_mode_info_block.phys_base_ptr;
	// The position address
	int rel_pos;

	// Loop through
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			// Set the mask depending on color
			pixel_mask = (image[j + i*w] != ' ') ? 0xde : 0;
			rel_pos = (x + j)*3 + ((i + y)*saved_mode_info_block.bytes_per_line);

			phys_base_ptr[rel_pos] = pixel_mask;
			phys_base_ptr[rel_pos + 1] = pixel_mask;
			phys_base_ptr[rel_pos + 2] = pixel_mask;
		}
	}
}
