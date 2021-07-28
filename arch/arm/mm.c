#include <stdint.h>
#include <mm.h>

void memzero(uint32_t saddr, uint32_t eaddr) {
	uint8_t* ptr = (uint8_t*)saddr;

	for (; ptr < (uint8_t*)eaddr; ptr++) {
		*ptr = 0;
	}
}
