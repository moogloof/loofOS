#ifndef KERNEL_PRINT_H
#define KERNEL_PRINT_H

#include <stdint.h>

/// Set writeb
void set_writeb(void (*)(char));

// Print for kernel
void kernel_print(const char*, ...);

#endif
