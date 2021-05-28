#include <stdint.h>

void kernel_main() {
	*((uint16_t*)0xb8000) = 0b1000111101001100;
}

