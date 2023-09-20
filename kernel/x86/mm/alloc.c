/*
 * Linked list allocator
 * The allocator stores the free blocks as a linked list. Free blocks come in multiple sizes. The sizes are powers of two.
 */

#include <stdint.h>
#include <mm/alloc.h>

// Split block
static void split_block(heap_header* block) {
	// New size
	uint32_t new_size = block->size - 1;
	// Create new buddy
	heap_header* new_buddy = (char*)block + (1 << new_size);
	new_buddy.size = new_size;
	new_buddy.used = 0;

	// Edit old block
	block->size = new_size;
}

// Coalesce block
static void coalesce_block(heap_header* block, uint32_t offset) {
	// Get buddy block
	heap_header* buddy_block = (char*)block - offset;
	buddy_block ^= 1 << buddy_block->size;
	buddy_block = (char*)buddy_block + offset;

	// Check if buddy is used
	if (!buddy_block->used) {
		// Get the one that starts and set it to the correct
		if (buddy_block > block) {
			block->size++;
		} else {
			buddy_block->size++;
		}
	}
}

// Get minimum block size for some size
// Blocks of that size may not be available
// Block size is returned as the log base two of a power of two
static uint32_t round_block_size(uint32_t size) {
	uint32_t block_size = MIN_BLOCK_SIZE - 1;

	// Loop block size until smallest block size that contains size
	while (((size + (1 << ++block_size) - 1) >> block_size) != 0);

	// Return the block size
	return block_size;
}

// Allocate an object
void* memory_allocate(heap_spec alloc_info, uint32_t size) {
	heap_header* alloc_addr = alloc_info.start;
	uint32_t actual_size = round_block_size(size + sizeof(heap_header));

	// Search for available that fits size
	while (alloc_addr <= alloc_info.end) {
		if (!alloc_addr->used && alloc_addr->size >= actual_size) {
			// Split to accomodate size
			while (alloc_addr->size > actual_size) {
				split_block(alloc_addr);
			}

			// Mark as used
			alloc_addr->used = 1;

			// Return the final split block
			return (char*)alloc_addr + sizeof(heap_header);
		}

		// Go to next block
		alloc_addr = (char*)alloc_addr + (1 << alloc_addr->size);
	}

	// No matches
	return 0;
}

// Free object
void memory_free(heap_spec alloc_info, void* addr) {
	heap_header* free_addr = addr;

	// Free it
	free_addr->used = 0;

	// Coalesce the block
	coalesce_block(addr, alloc_info.start);
}
