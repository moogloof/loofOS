#ifndef MM_ALLOC_H
#define MM_ALLOC_H

// Size of smallest block
#define BLOCK_SIZE 32
// Start of the kernel heap
#define HEAP_START 0x10000000
// End of the kernel heap
#define HEAP_END 0x40000000

// Initialize kernel heap
void init_kernel_heap();

// Allocate a kernel object
void* kernel_allocate(int);

// Free kernel object
void kernel_free(void*);

#endif
