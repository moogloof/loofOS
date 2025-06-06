#include <string.h>
#include <stdint.h>

void* memset(void* b, uint8_t c, uint32_t len) {
	for (uint32_t i = 0; i < len; i++) {
		*((uint8_t*)b + i) = c;
	}

	return b;
}

void* memcpy(void* dst, void* src, uint32_t num) {
	for (uint32_t i = 0; i < num; i++) {
		*((uint8_t*)dst + i) = *((uint8_t*)src + i);
	}

	return dst;
}

// Small utility function for checking strings
int memcmp(void* truth, void* guess, uint32_t len) {
	// Char stuff
	unsigned char* truth_arr = truth;
	unsigned char* guess_arr = guess;

	// Check until length
	for (uint32_t i = 0; i < len; i++) {
		if (truth_arr[i] != guess_arr[i])
			return truth_arr[i] - guess_arr[i];
	}

	// The guess is correct
	return 0;
}
