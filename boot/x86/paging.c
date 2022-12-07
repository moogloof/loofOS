#include <stdint.h>
#include <common.h>
#include <boot/paging.h>

// Create a bitmap for each page in 4 GiB ram
// 0 == available
// 1 == used
static uint8_t* pagelist_bitmap;

// Page directory for the kernel
// 4MiB
volatile pde_4mib* kernel_memory = (pde_4mib*)KERNEL_PAGE_DIRECTORY;

// Initialize 32 bit paging
void init_paging() {
	// Make page directory 1-to-1 for the time being
	for (int i = 0; i < PAGE_LENGTH_4M; i++) {
		kernel_memory[i] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 1, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i};
	}

	// Swap low and high memory for the kernel addr
	for (int i = 0; i < PAGE_LENGTH_4M / 4; i++) {
		kernel_memory[i + (3*PAGE_LENGTH_4M / 4)] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 1, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i};
	}

	// Enable paging
	enable_paging();
}
