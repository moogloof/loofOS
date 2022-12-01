#include <stdint.h>
#include <common.h>
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

	// Zero out the page directory for the time being
	for (int i = 0; i < PAGE_LENGTH_4M; i++) {
		kernel_memory[i] = (pde_4mib){.present = 0, .rw = 0, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 0, .g = 0, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = 0};
	}

	// Shift actually useable physical memory back to fill the hole of the kernel
	for (int i = 0; i < (TOTAL_MEMORY / PAGE_SIZE_4M) - (PAGE_LENGTH_4M / 4); i++) {
		kernel_memory[i] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 0, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i + (PAGE_LENGTH_4M / 4)};
	}
	// Swap low and high memory for the kernel addr
	for (int i = 0; i < PAGE_LENGTH_4M / 4; i++) {
		kernel_memory[i + (3*PAGE_LENGTH_4M / 4)] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 1, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i};
	}

	// Setup page faults
	set_id(14, &pagefault_handler_wrapper, 0x08, IDT_PROT_TRAP, 0, 1);

	// Enable paging
	enable_paging();
}

// Page fault handler
void pagefault_handler(seg_register_set seg_regs, gen_register_set gen_regs, uint32_t eid, interrupt_frame frame) {
	kernel_panic("Page fault.", seg_regs, gen_regs, frame, eid);
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

	// Deny allocation into kernel space
	if (addr > (3*PAGE_LENGTH_4M / 4) * PAGE_SIZE_4M) {
		return;
	}

	// Check if the table exists, if not, make one
	if (page_dir[dir_index].present) {
		// If there is already a 4MiB page entry, then there is no point in allocating a table
		if (page_dir[dir_index].ps) {
			return;
		}

		page_table = page_dir[dir_index].addr << 12;
	} else {
		page_table = kernel_allocate(sizeof(pte_4kib) * 1024);
		page_dir[dir_index] = (pde_4kib){.present = 1, .rw = 1, .us = 1, .pwt = 0, .pcd = 0, .a = 0, .ignored = 0, .ps = 0, .ignored2 = 0, .addr = ((uint32_t)page_table - KERNEL_BASE) >> 12};
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

	// Check if the table exists
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

// Map two pages, virtual -> physical
// The length is in bytes
// TODO: Map more than just 4mib pages
void map_page(pde_4mib* page_dir, uint32_t virt_addr, uint32_t phys_addr, int len, int isuser) {
	// Get page directory entry
	int dir_index = virt_addr >> 22;
	// Get the amount of 4MiB pages
	int pde_4mib_count = len / PAGE_SIZE_4M;
	pde_4mib_count += (pde_4mib_count % PAGE_SIZE_4M > 0) ? 1 : 0;

	// Map the 4mib pages
	for (int i = 0; i < pde_4mib_count; i++) {
		page_dir[dir_index + i] = (pde_4mib){.present = 1, .rw = 1, .us = isuser, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 0, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = (phys_addr >> 22) + i};
	}
}
