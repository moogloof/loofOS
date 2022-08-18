#include <stdint.h>
#include <mm/paging.h>
#include <mm/alloc.h>
#include <core/isr.h>
#include <core/idt.h>
#include <core/panic.h>

// Create a bitmap for each page in 4 GiB ram
// 0 == available
// 1 == used
static uint8_t* pagelist_bitmap;

// Page directory for the kernel
// 4MiB
volatile pde_4mib* kernel_memory = (pde_4mib*)KERNEL_PAGE_DIRECTORY;

// Initialize 32 bit paging
void init_paging() {
	// Allocate a pagelist_bitmap
	pagelist_bitmap = kernel_allocate(PAGE_LENGTH_4K / 8);

	// Make everything available first
	for (int i = 0; i < PAGE_LENGTH_4K / 8; i++) {
		pagelist_bitmap[i] = 0;
	}

	// Reserve until pagelist start for kernel and hardware
	for (int i = 0; i < PAGE_LENGTH_4K / 4 / 8; i++) {
		pagelist_bitmap[i] = 0xff;
	}

	// Set pages for kernel to cover entire RAM
	for (int i = 0; i < PAGE_LENGTH_4M; i++) {
		kernel_memory[i] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 0, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i};
	}

	// Swap low and high memory
	for (int i = 0; i < PAGE_LENGTH_4M / 4; i++) {
		kernel_memory[i] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 0, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i + 768};
	}
	for (int i = 0; i < PAGE_LENGTH_4M / 4; i++) {
		kernel_memory[i + 768] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 1, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i};
	}

	// Setup page faults
	set_id(14, pagefault_handler_wrapper, 0x08, IDT_PROT_TRAP, 0, 1);

	// Enable paging
	enable_paging();
}

// Page fault handler
void pagefault_handler(uint32_t eid) {
	kernel_panic("Page fault.");
}

// Allocate a page
void allocate_page(pde_4kib* page_dir, uint32_t addr) {
	// Get page directory entry
	int dir_index = addr >> 22;
	// Get page table entry
	int table_index = (addr << 10) >> 22;
	// Page table
	pte_4kib* page_table;
	// Address of new page
	uint32_t new_page = 0;

	// Check if the table exists, if not, make one
	if (page_dir[dir_index].present) {
		page_table = page_dir[dir_index].addr << 12;
	} else {
		page_table = kernel_allocate(sizeof(pte_4kib) * 1024);
		page_dir[dir_index] = (pde_4kib){.present = 1, .rw = 1, .us = 1, .pwt = 0, .pcd = 0, .a = 0, .ignored = 0, .ps = 0, .ignored2 = 0, .addr = (uint32_t)page_table >> 12};
	}

	// Search for an available page
	for (int i = 0; i < PAGE_LENGTH_4K; i++) {
		if (!((pagelist_bitmap[i / 8] >> (i % 8)) & 1)) {
			// Get the new page
			new_page = i;
			// Set the new page as used
			pagelist_bitmap[i / 8] |= 1 << (i % 8);
			break;
		}
	}

	// Allocate a page if none exists
	if (!page_table[table_index].present) {
		page_table[table_index] = (pte_4kib){.present = 1, .rw = 1, .us = 1, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .pat = 0, .g = 0, .ignored = 0, .addr = new_page};
	}
}

// Free a page
void free_page(pde_4kib* page_dir, uint32_t addr) {
	// Get page directory entry
	int dir_index = addr >> 22;
	// Get page table entry
	int table_index = (addr << 10) >> 22;
	// Page table
	pte_4kib* page_table;

	// Checck if the table exists
	if (page_dir[dir_index].present) {
		page_table = page_dir[dir_index].addr << 12;

		// Check if the page exists
		if (page_table[table_index].present) {
			// Set the page as available
			int i = page_table[table_index].addr;
			pagelist_bitmap[i / 8] &= ~(1 << (i % 8));
			// Erase the page from the table
			page_table[table_index] = (pte_4kib){0};
		}
	}
}
