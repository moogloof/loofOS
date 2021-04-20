#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>

// Conversion functions
// Int to string
static int inttostr(long, uint8_t, char*);

// Print functions
// General print string
void print(void (*_puts)(char*), char*);
// Print int
void printint(void (*_puts)(char*), long, int, uint8_t);

#endif
