#include <common.h>
#include <stdint.h>

void* memcpy(void* dst, const void* src, uint32_t num) {
	for (uint32_t i = 0; i < num; i++) {
		*((uint8_t*)dst + i) = *((uint8_t*)src + i);
	}

	return dst;
}

// Small utility function for checking strings
int verify_string(char* truth, char* guess, int len) {
	// Check until length
	for (int i = 0; i < len; i++) {
		if (truth[i] != guess[i])
			return 0;
	}
	// The guess is correct
	return 1;
}
