#ifndef PAGING_H
#define PAGING_H

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

// Enable paging
void enable_paging(pde_4mib*);

#endif
