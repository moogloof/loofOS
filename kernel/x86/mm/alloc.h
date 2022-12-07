#ifndef MM_ALLOC_H
#define MM_ALLOC_H

#include <common.h>

// Size of smallest block
#define BLOCK_SIZE 16
// Start of the kernel heap
#define HEAP_START (KERNEL_BASE + 0x10000000)
// End of the kernel heap
#define HEAP_END (KERNEL_BASE + 0x3ffffff0)
// Length of the kernel heap
#define HEAP_LENGTH (HEAP_END - HEAP_START)

// Initialize kernel heap
void init_kernel_heap();

// Allocate a kernel object
void* kernel_allocate(int);

// Free kernel object
void kernel_free(void*);

#endif
