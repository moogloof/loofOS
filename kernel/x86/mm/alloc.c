/*
 * Linked list allocator
 * The allocator stores the free blocks as a linked list. Free blocks come in multiple sizes. The sizes are powers of two.
 */

#include <stdint.h>
#include <mm/alloc.h>
#include <presets.h>

// Kernel alloc config
static heap_spec kernel_heap = {.start = KERNEL_BASE + 0x10000000 - sizeof(heap_header), .end = KERNEL_BASE + 0x30000000 - sizeof(heap_header)};

// Split block
static void split_block(heap_header* block) {
	// New size
	uint32_t new_size = block->size - 1;
	// Create new buddy
	heap_header* new_buddy = (heap_header*)((int)block + (1 << new_size));
	new_buddy->size = new_size;
	new_buddy->used = 0;

	// Edit old block
	block->size = new_size;
}

// Coalesce block
static void coalesce_block(heap_header* block, uint32_t offset) {
	// Get buddy blocks
	heap_header* buddy_block1 = block;
	heap_header* buddy_block2;

	// Keep coalescing
	while (1) {
		// Set buddys
		buddy_block2 = (heap_header*)((char*)buddy_block1 - offset);
		buddy_block2 = (heap_header*)((int)buddy_block2 ^ (1 << buddy_block1->size));
		buddy_block2 = (heap_header*)((char*)buddy_block2 + offset);

		// Check if buddy is used
		if (!buddy_block2->used && buddy_block2->size == buddy_block1->size) {
			// Get the one that starts and set it to the correct
			if (buddy_block2 > buddy_block1) {
				buddy_block1->size++;
			} else {
				buddy_block2->size++;
				buddy_block1 = buddy_block2;
			}
		} else {
			return;
		}
	}
}

// Get minimum block size for some size
// Blocks of that size may not be available
// Block size is returned as the log base two of a power of two
static uint32_t round_block_size(uint32_t size) {
	uint32_t block_size = MIN_BLOCK_SIZE - 1;

	// Loop block size until smallest block size that contains size
	while ((size >> ++block_size) != 0);

	// Return the block size rounded
	return (size > (1 << block_size)) ? (block_size + 1) : block_size;
}

// Allocate an object
void* memory_allocate(heap_spec alloc_info, uint32_t size) {
	heap_header* alloc_addr = (heap_header*)alloc_info.start;
	uint32_t actual_size = round_block_size(size + sizeof(heap_header));

	// Search for available that fits size
	while ((uint32_t)alloc_addr <= alloc_info.end) {
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
		alloc_addr = (heap_header*)((char*)alloc_addr + (1 << alloc_addr->size));
	}

	// No matches
	return 0;
}

// Free object
void memory_free(heap_spec alloc_info, void* addr) {
	heap_header* free_addr = (heap_header*)((uint32_t)addr - sizeof(heap_header));

	// Free it
	free_addr->used = 0;

	// Coalesce the block
	coalesce_block(addr, alloc_info.start);
}

// Initialize a heap given heap_spec
void init_memory(heap_spec alloc_info) {
	heap_header* block = (heap_header*)alloc_info.start;
	uint32_t heap_length = alloc_info.end - alloc_info.start + 1;
	uint8_t alloc_size = MIN_BLOCK_SIZE;

	// Pre truncate heap_length
	heap_length >>= alloc_size;

	// Fill the entire range with blocks
	while ((uint32_t)block < alloc_info.end && heap_length > 0) {
		// Fill the powers of two blocks
		// We start with least significant bc usually our first allocs will be small
		if (heap_length & 1) {
			// Create new block
			block->size = alloc_size;
			block->used = 0;
			// Move to next
			block = (heap_header*)((char*)block + (1 << alloc_size));
		}
		heap_length >>= 1;
		alloc_size++;
	}
}

// Kernel allocate function
void* kernel_allocate(uint32_t size) {
	return memory_allocate(kernel_heap, size);
}

// Kernel free function
void kernel_free(void* addr) {
	memory_free(kernel_heap, addr);
}

// Initialize kernel heap
void init_kernel_heap(void) {
	init_memory(kernel_heap);
}
