#ifndef CORE_PORT_H
#define CORE_PORT_H

#include <stdint.h>

// Input port byte function
uint8_t inportb(uint8_t*);

// Input port word function
uint16_t inportw(uint16_t*);

// Output port byte function
void outportb(uint8_t*, uint8_t);

// Output port word function
void outportw(uint16_t*, uint16_t);

#endif
