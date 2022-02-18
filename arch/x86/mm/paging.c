#include <stdint.h>
#include <mm/paging.h>
#include <sys/kernel_print.h>

// Create a bitmap for each page in 4 GiB ram
// 0 == available
// 1 == used
static uint8_t pagelist_bitmap[(PAGELIST_END / PAGE_SIZE_4K) / 8];

// Page directory for the kernel
// 4MiB
pde_4mib* kernel_memory = (pde_4mib*)KERNEL_PAGE_DIRECTORY;

// Initialize 32 bit paging
void init_paging() {
	// Make everything available first
	for (int i = 0; i < PAGELIST_END / PAGE_SIZE_4K / 8; i++) {
		pagelist_bitmap[i] = 0;
	}

	// Reserve until pagelist start for kernel and hardware
	for (int i = 0; i < PAGELIST_START / PAGE_SIZE_4K / 8; i++) {
		pagelist_bitmap[i] = 0xff;
	}

	// Set pages for kernel to cover entire RAM
	for (int i = 0; i < 1024; i++) {
		kernel_memory[i] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 0, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i};
	}

	// Enable paging
//	enable_paging();
}
