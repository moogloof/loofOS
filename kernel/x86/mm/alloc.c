#include <stdint.h>
#include <mm/alloc.h>
#include <core/panic.h>

// Create a bitmap for each block in the heap
// 0 == unused
// 1 == used
// 2 == fill
// 3 == reserved
static uint8_t* heap_bitmap = HEAP_START;

// Size of heap bitmap
static int heap_bitmap_size = HEAP_LENGTH / (BLOCK_SIZE * 4);

// Bitmap set helper
void bitmap_set(int i, int val) {
	heap_bitmap[i / 4] &= ~(3 << (2 * (i % 4)));
	heap_bitmap[i / 4] |= (val & 3) << (2 * (i % 4));
}

// Bitmap get helper
int bitmap_get(int i) {
	return (heap_bitmap[i / 4] >> (2 * (i % 4))) & 3;
}

// Initialize the kernel heap
void init_kernel_heap() {
	// Clear the bitmap first
	for (int i = 0; i < heap_bitmap_size; i++) {
		bitmap_set(i, 0);
	}
	// First part of heap reserved for the heap bitmap
	for (int i = 0; i < heap_bitmap_size / BLOCK_SIZE; i++) {
		bitmap_set(i, 3);
	}
}

// Allocate a kernel object
void* kernel_allocate(int size) {
	// Find level of the object
	int size_level = BLOCK_SIZE;

	// No invalid sizes
	if (size > HEAP_LENGTH) {
		return 0;
	}

	// Calculate size level as power of 2
	while (size_level < size) {
		size_level *= 2;
	}

	// Search by level
	for (int i = 0; i < heap_bitmap_size * 4; i += size_level / BLOCK_SIZE) {
		// Check for availability
		if (!bitmap_get(i)) {
			// Check if the size fits
			uint8_t mem_found = 1;

			for (int j = i; j < i + (size_level / BLOCK_SIZE); j++) {
				// Check if unused memory
				if (bitmap_get(j)) {
					// Exit and continue on finding used or filled
					mem_found = 0;
					break;
				}
			}

			// Check if correct memory found
			if (mem_found) {
				// Return address and fill memory on found
				bitmap_set(i, 1);
				for (int j = i + 1; j < i + (size_level / BLOCK_SIZE); j++) {
					bitmap_set(j, 2);
				}
				return (void*)(i * BLOCK_SIZE + HEAP_START);
			}
		}
	}

	// No memory so we panic
	return 0;
}

// Free kernel object
void kernel_free(void* addr) {
	// Get the bitmap addr
	int bitmap_addr = ((int)addr - HEAP_START) / BLOCK_SIZE;

	// Make sure the address is within the heap
	if ((uint32_t)addr < HEAP_START || (uint32_t)addr > HEAP_END - 1) {
		return;
	} else if (bitmap_get(bitmap_addr) == 3) {
		// Make sure the allocated address is not reserved
		return;
	}

	// Set it free
	bitmap_set(bitmap_addr, 0);
	for (int i = bitmap_addr + 1; bitmap_get(i) == 2; i++) {
		bitmap_set(i, 0);
	}
}