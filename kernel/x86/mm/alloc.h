#ifndef MM_ALLOC_H
#define MM_ALLOC_H

#include <stdint.h>

// Size of smallest block (2^x)
#define MIN_BLOCK_SIZE 4
// Size of the largest block (2^x)
#define MAX_BLOCK_SIZE 30
// Number of block lists
#define TOTAL_BLOCK_LISTS (MAX_BLOCK_SIZE - MIN_BLOCKSIZE + 1)

// Heap blocks in a doubly linked list
// These are stored within the free blocks
// They are located at the head of the free blocks
// These will be erased when freed blocks are used for security
typedef struct heap_block {
	struct heap_block* prev, next;
} heap_block;

// Allocate an object
void* memory_allocate(uint32_t, heap_block[]);

// Free object
void memory_free(void*);

#endif
