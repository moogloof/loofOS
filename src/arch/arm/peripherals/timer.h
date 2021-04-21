#ifndef _P_TIMER_H
#define _P_TIMER_H

// Define system timer registers
// Base address of registers
#define TIMER_BASE 0x7e003000

// System timer control/status
#define TIMER_CS (TIMER_BASE)
// System timer counter lower 32 bits
#define TIMER_CLO (TIMER_BASE + 0x4)
// System timer counter higher 32 bits
#define TIMER_CHI (TIMER_BASE + 0x8)
// System timer compare 0-3
#define TIMER_C0 (TIMER_BASE + 0xc)
#define TIMER_C1 (TIMER_BASE + 0x10)
#define TIMER_C2 (TIMER_BASE + 0x14)
#define TIMER_C3 (TIMER_BASE + 0x18)

#endif
