#include <stdint.h>
#include <core/port.h>

// Input port byte function
// Read byte from port
uint8_t inportb(uint8_t* port) {
	uint8_t ret;

	// Read port
	__asm__("in %1, %0" : "=a"(ret) : "Nd"(port));

	return ret;
}

// Output port byte function
// Write byte to port
void outportb(uint8_t* port, uint8_t val) {
	// Write to port
	__asm__("out %0, %1" : : "a"(val), "Nd"(port));
}
