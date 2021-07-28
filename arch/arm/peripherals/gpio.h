#ifndef _P_GPIO_H
#define _P_GPIO_H

#include <peripherals/base.h>

// Define GPIO registers
// Base address of registers
#define GPIO_BASE (P_BASE + 0x200000)

// GPIO Function select registers
#define GPFSEL0 (GPIO_BASE)
#define GPFSEL1 (GPIO_BASE + 0x4)
#define GPFSEL2 (GPIO_BASE + 0x8)
#define GPFSEL3 (GPIO_BASE + 0xc)
#define GPFSEL4 (GPIO_BASE + 0x10)
#define GPFSEL5 (GPIO_BASE + 0x14)

// GPIO Pin Output Set
#define GPSET0 (GPIO_BASE + 0x1c)
#define GPSET1 (GPIO_BASE + 0x20)

// GPIO Pin Output Clear
#define GPCLR0 (GPIO_BASE + 0x28)
#define GPCLR1 (GPIO_BASE + 0x2c)

// GPIO Pin Level
#define GPLEV0 (GPIO_BASE + 0x34)
#define GPLEV1 (GPIO_BASE + 0x38)

// GPIO Pin Event Detect Status
#define GPEDS0 (GPIO_BASE + 0x40)
#define GPEDS1 (GPIO_BASE + 0x44)

// GPIO Pin Rising Edge Detect Enable
#define GPREN0 (GPIO_BASE + 0x4c)
#define GPREN1 (GPIO_BASE + 0x50)

// GPIO Pin Falling Edge Detect Enable
#define GPFEN0 (GPIO_BASE + 0x58)
#define GPFEN1 (GPIO_BASE + 0x5c)

// GPIO Pin High Detect Enable
#define GPHEN0 (GPIO_BASE + 0x64)
#define GPHEN1 (GPIO_BASE + 0x68)

// GPIO Pin Low Detect Enable
#define GPLEN0 (GPIO_BASE + 0x70)
#define GPLEN1 (GPIO_BASE + 0x74)

// GPIO Pin Async. Rising Edge Detect
#define GPAREN0 (GPIO_BASE + 0x7c)
#define GPAREN1 (GPIO_BASE + 0x80)

// GPIO Pin Async. Falling Edge Detect
#define GPAFEN0 (GPIO_BASE + 0x88)
#define GPAFEN1 (GPIO_BASE + 0x8c)

// GPIO Pin Pull-up/down Enable
#define GPPUD (GPIO_BASE + 0x94)
// GPIO Pin PUD enable clock
#define GPPUDCLK0 (GPIO_BASE + 0x98)
#define GPPUDCLK1 (GPIO_BASE + 0x9c)

#endif
