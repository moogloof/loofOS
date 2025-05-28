#include <stdint.h>
#include <string.h>
#include <mm/paging.h>
#include <mm/alloc.h>
#include <core/isr.h>
#include <core/idt.h>
#include <core/panic.h>

// Page free list stack
// This specifies top of stack
// Bottom of stack can point to any address above the kernel base
static page_block* page_freelist;

// Page directory for the kernel
// 4MiB
pde_4mib kernel_memory[PAGE_LENGTH_4M] __attribute__((aligned(4096)));

// Initialize 32 bit paging
void init_paging(void) {
	// Zero out the page directory for the time being
	for (int i = 0; i < PAGE_LENGTH_4M; i++) {
		kernel_memory[i] = (pde_4mib){.present = 0, .rw = 0, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 0, .g = 0, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = 0};
	}

	// Shift actually useable physical memory back to fill the hole of the kernel
	for (int i = 0; i < 3*PAGE_LENGTH_4M / 4; i++) {
		kernel_memory[i] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 0, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i + (PAGE_LENGTH_4M / 4)};
	}
	// Swap low and high memory for the kernel addr
	for (int i = 0; i < PAGE_LENGTH_4M / 4; i++) {
		kernel_memory[i + (3*PAGE_LENGTH_4M / 4)] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 1, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i};
	}

	// Setup page faults
	set_id(14, &pagefault_handler_wrapper, 0x08, IDT_PROT_INTR, 0, 1);

	// Enable paging
	enable_paging((pde_4mib*)((uint32_t)kernel_memory - KERNEL_BASE));

	// Setup freelist stack below high memory
	for (uint32_t i = 0; i < KERNEL_BASE; i += PAGE_SIZE_4K) {
		page_freelist = (page_block*)i;
		page_freelist->next = (page_block*)(i + PAGE_SIZE_4K);
	}
	page_freelist = 0;
}

// Page fault handler
void pagefault_handler(seg_register_set seg_regs, gen_register_set gen_regs, uint32_t eid, interrupt_frame frame) {
	kernel_panic("Page fault.", seg_regs, gen_regs, frame, eid);
}

// Allocate a page
void allocate_page(pde_4kib* page_dir, uint32_t addr) {
	// See if any pages are in stack
	// Deny allocation into kernel space
	if ((uint32_t)page_freelist >= KERNEL_BASE || addr >= KERNEL_BASE) {
		return;
	}

	// Get page directory entry
	int dir_index = addr >> 22;
	// Get page table entry
	int table_index = (addr >> 12) & 0x3ff;
	// Page table
	pte_4kib* page_table;
	// Allocated page block
	page_block* available_page;

	// Check if the table exists, if not, make one
	if (page_dir[dir_index].present) {
		// If there is already a 4MiB page entry, then there is no point in allocating a table
		if (page_dir[dir_index].ps) {
			return;
		}

		page_table = (pte_4kib*)((page_dir[dir_index].addr << 12) + KERNEL_BASE);
	} else {
		page_table = kernel_allocate(sizeof(pte_4kib) * 1024);
		page_dir[dir_index] = (pde_4kib){.present = 1, .rw = 1, .us = 1, .pwt = 0, .pcd = 0, .a = 0, .ignored = 0, .ps = 0, .ignored2 = 0, .addr = ((uint32_t)page_table - KERNEL_BASE) >> 12};
	}

	// Check if page already exists
	// TOO SIMPLE, FIX LATER
	if (page_table[table_index].present) {
		return;
	}

	// Get available page physical addr
	available_page = (page_block*)((uint32_t)page_freelist + PAGE_OFFSET);
	// Pop stack
	page_freelist = page_freelist->next;

	// Set page
	page_table[table_index] = (pte_4kib){.present = 1, .rw = 1, .us = 1, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .pat = 0, .g = 0, .ignored = 0, .addr = (uint32_t)available_page >> 12};
}

// Free a page
// Does nothing for 4MiB pages
void free_page(pde_4kib* page_dir, uint32_t addr) {
	// Get page directory entry
	int dir_index = addr >> 22;
	// Get page table entry
	int table_index = (addr >> 12) & 0x3ff;
	// Page table
	pte_4kib* page_table;

	// Check if the table exists
	if (page_dir[dir_index].present) {
		page_table = (pte_4kib*)((page_dir[dir_index].addr << 12) - KERNEL_BASE);

		// Check if the page exists
		if (page_table[table_index].present) {
			// Push page back on stack
			page_block* freed_page = (page_block*)((page_table[table_index].addr << 12) - PAGE_OFFSET);
			freed_page->next = page_freelist;
			page_freelist = freed_page;
			// Erase the page from the table
			page_table[table_index] = (pte_4kib){0};
		}
	}
}

// Map two pages, virtual -> physical
// The length is in bytes
// TODO: Implement
