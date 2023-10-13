#include <stdint.h>
#include <common.h>

void* memset(void* b, uint8_t c, int len) {
	for (int i = 0; i < len; i++) {
		*((uint8_t*)b + i) = c;
	}
}

void* memcpy(void* dst, void* src, int len) {
	for (int i = 0; i < len; i++) {
		*((uint8_t*)dst + i) = *((uint8_t*)src + i);
	}
}
