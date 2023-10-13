#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define KERNEL_BASE 0xc0000000

#define TOTAL_MEMORY 0xc0000000

// For memcpy
void* memcpy(void*, const void*, uint32_t);

// Small utility function for checking strings
int verify_string(char*, char*, int);

#endif
