#include <stdint.h>
#include <common.h>
#include <drivers/vbe.h>
#include <mm/paging.h>

// The VBE info block
static vbe_cib* vbe_info_block = 0x600 + KERNEL_BASE;

// The saved one
static vbe_mib* saved_mode_info_block = 0xa00 + KERNEL_BASE;

// The framebuffer in kernel memory
volatile uint8_t* vbe_framebuffer;

// The secondary framebuffer
volatile uint8_t* vbe_framebuffer_double;

// Initialize the driver
void init_vbe() {
	// Allocate some space to be the second buffer
	vbe_framebuffer_double = (uint8_t*)kernel_allocate(saved_mode_info_block->bytes_per_line * saved_mode_info_block->yresolution);
	// Allocate some kernel space to be the framebuffer
	vbe_framebuffer = (uint8_t*)kernel_allocate(saved_mode_info_block->bytes_per_line * saved_mode_info_block->yresolution);
	// Map space to actual physical memory
	map_page(KERNEL_PAGE_DIRECTORY, vbe_framebuffer, saved_mode_info_block->phys_base_ptr, saved_mode_info_block->bytes_per_line * saved_mode_info_block->yresolution, 0);

	// Clear the screen
	for (int i = 0; i < saved_mode_info_block->bytes_per_line * saved_mode_info_block->yresolution; i++) {
		vbe_framebuffer[i] = 0;
	}
}

// Obtain the secondary framebuffer
uint8_t* obtain_framebuffer() {
	return vbe_framebuffer_double;
}

// Update the primary framebuffer
void update_framebuffer() {
	memcpy(vbe_framebuffer, vbe_framebuffer_double, saved_mode_info_block->bytes_per_line * saved_mode_info_block->yresolution);
}

// Get info about winddow
vbe_mib* get_window_info() {
	return saved_mode_info_block;
}
