#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#define KERNEL_BASE 0xc0000000

// For memcpy
void* memcpy(void*, const void*, uint32_t);

#endif
