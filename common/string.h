#ifndef _COMMON
#define _COMMON

#include <stdint.h>

void* memset(void*, uint8_t, uint32_t);

void* memcpy(void*, void*, uint32_t);

int memcmp(void*, void*, uint32_t);

#endif
