#ifndef PRINT
#define PRINT

/// Set writeb
void set_outb(void (*)(char));

// Print for kernel
void print(const char*, ...);

#endif
