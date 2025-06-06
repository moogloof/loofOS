#ifndef MM_ALLOC_H
#define MM_ALLOC_H

#include <stdint.h>
#include <string.h>

// Size of smallest block (2^x)
#define MIN_BLOCK_SIZE 3

// Heap headers stored at the beginning of each thing
typedef struct {
	uint8_t size:7;
	uint8_t used:1;
} __attribute__((packed)) heap_header;

// Heap specification, contains metadata
typedef struct {
	uint32_t start;
	uint32_t end; // Inclusive (end = last byte addr)
} heap_spec;

// Allocate an object
void* memory_allocate(heap_spec, uint32_t);

// Free object
void memory_free(heap_spec, void*);

// Initialize a heap given heap_spec
void init_memory(heap_spec);

// Kernel allocate function
void* kernel_allocate(uint32_t);

// Kernel free function
void kernel_free(void*);

// Initialize kernel heap
void init_kernel_heap(void);

#endif
