#include <common.h>
#include <stdint.h>

void* memcpy(void* dst, const void* src, uint32_t num) {
	for (uint32_t i = 0; i < num; i++) {
		*((uint8_t*)dst + i) = *((uint8_t*)src + i);
	}

	return dst;
}
