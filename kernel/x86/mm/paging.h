#ifndef MM_PAGING_H
#define MM_PAGING_H

#include <stdint.h>
#include <core/isr.h>
#include <string.h>

// Section of physical memory set aside for users
// Size of the page frames
#define PAGE_SIZE_4K 0x1000
#define PAGE_SIZE_4M 0x400000
// Number of 4K pages
#define PAGE_LENGTH_4K 0x100000
// Number of 4M pages
#define PAGE_LENGTH_4M 1024
// Physical page offset
#define PAGE_OFFSET 0x40000000

// Header for each page
typedef struct page_block {
	struct page_block* next;
} __attribute__((packed)) page_block;

// Page directory entry for 4 MiB
typedef struct {
	uint8_t present:1; // Present bit; must be 1
	uint8_t rw:1; // Read/write bit; 0 == ro, 1 == rw allowed
	uint8_t us:1; // User/supervisor; 0 == supervisor, 1 == user
	uint8_t pwt:1; // Page-level write-through bit
	uint8_t pcd:1; // Page-level cache disable bit
	uint8_t a:1; // Accessed bit
	uint8_t d:1; // Dirty bit
	uint8_t ps:1; // Page size; must be 1 for 4 MiB
	uint8_t g:1; // Global bit
	uint8_t ignored:3; // Ignored bits
	uint8_t pat:1; // PAT bit; 0
	uint16_t highaddr:9; // Reserved; Bits 39:32 of physical address
	uint16_t lowaddr:10; // Bits 31:22 of physical address
} __attribute__((packed)) pde_4mib;

typedef struct {
	uint8_t present:1; // Present bit; must be 1
	uint8_t rw:1; // Read/write bit; 0 == ro, 1 == rw allowed
	uint8_t us:1; // User/supervisor; 0 == supervisor, 1 = user
	uint8_t pwt:1; // Page-level write-through bit
	uint8_t pcd:1; // Page-level cache disable bit
	uint8_t a:1; // Accessed bit
	uint8_t ignored:1; // Ignored bit
	uint8_t ps:1; // Page size; must be 0 for 4 KiB pages
	uint8_t ignored2:4; // Ignored bits
	uint32_t addr:20; // Physical address of page table
} __attribute__((packed)) pde_4kib;

typedef struct {
	uint8_t present:1; // Present bit; must be 1
	uint8_t rw:1; // Read/write bit; 0 == ro, 1 == rw allowed
	uint8_t us:1; // User/supervisor; 0 == supervisor, 1 = user
	uint8_t pwt:1; // Page-level write-through bit
	uint8_t pcd:1; // Page-level cache disable bit
	uint8_t a:1; // Accessed bit
	uint8_t d:1; // Dirty bit
	uint8_t pat:1; // PAT bit; 0
	uint8_t g:1; // Global bit
	uint8_t ignored:3; // Ignored bits
	uint32_t addr:20; // Physical address of page
} __attribute__((packed)) pte_4kib;

// Initialize 32 bit paging
void init_paging();

// Page fault handler wrapper
__attribute__((interrupt)) void pagefault_handler_wrapper(interrupt_frame*);

// Page fault handler
void pagefault_handler(seg_register_set, gen_register_set, uint32_t, interrupt_frame);

// Enable paging
void enable_paging(pde_4mib*);

// Allocate a page
void allocate_page(pde_4kib*, uint32_t);

// Free a page
void free_page(pde_4kib*, uint32_t);

// Map two pages, virtual -> physical
//void map_page(pde_4mib*, uint32_t, uint32_t, int, int);

#endif
