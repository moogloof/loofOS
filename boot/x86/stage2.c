#include <stdint.h>

void foo(uint64_t a) {
	uint64_t b = a + 4;
}

void stage2() {
	foo(0xffffffffff);

	while (1) {}
}
