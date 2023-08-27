/*
 * Linked list allocator
 * The allocator stores the free blocks as a linked list. Free blocks come in multiple sizes. The sizes are powers of two.
 */

#include <stdint.h>
#include <mm/alloc.h>

// TODO: add, remove, merge, split, how tf will these work

// Get minimum block size for some size
// Blocks of that size may not be available
// Block size is returned as the log base two of a power of two
static int round_block_size(uint32_t size) {
	uint32_t block_size = MIN_BLOCK_SIZE - 1;

	// Loop block size until smallest block size that contains size
	while (size >> ++block_size != 0);

	// Return the block size
	return block_size;
}

// Allocate an object
void* memory_allocate(uint32_t size, heap_block blocks[]) {
	// Just return 0 on size 0
	if (size == 0) return 0;

	// Check which size is the min block we want
	int target_block_size = round_block_size(size);
	int min_block_index = target_block_size - MIN_BLOCK_SIZE;
	heap_block* allocated_block;

	// Check if the size is available
	// Also get the minimum size that is available
	while (min_block_index < TOTAL_BLOCK_LISTS && blocks[min_block_index].next == blocks[min_block_index].prev)
		min_block_index++;

	// Exit if no size available
	if (min_block_index >= TOTAL_BLOCK_LISTS) return 0;

	// Split the blocks until we get target block size
	while (min_block_index + MIN_BLOCK_SIZE > target_block_size) {
		// Split the block
		// Note that the min_block_index is an index of blocks[]
		split_block(min_block_index, blocks);

		// Decrease block size to split
		min_block_index--;
	}

	// Remove block from free list
	allocated_block = remove_block(min_block_index, blocks);

	// Return allocated block
	return allocated_block;
}
