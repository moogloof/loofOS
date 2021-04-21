#ifndef _P_IRQ_H
#define _P_IRQ_H

// Define interrupt registers
// Base address of registers
#define IRQ_BASE 0x7e00b000

// IRQ basic pending
#define IRQ_BASIC_PENDING (IRQ_BASE + 0x200)
// IRQ pending 1 and 2
#define IRQ_PENDING1 (IRQ_BASE + 0x204)
#define IRQ_PENDING2 (IRQ_BASE + 0x208)
// FIQ control
#define FIQ_CONTROL (IRQ_BASE + 0x20c)
// Enable IRQs 1 and 2
#define IRQ_ENABLE1 (IRQ_BASE + 0x210)
#define IRQ_ENABLE2 (IRQ_BASE + 0x214)
// Enable basic IRQs
#define IRQ_BASIC_ENABLE (IRQ_BASE + 0x218)
// Disable IRQs 1 and 2
#define IRQ_DISABLE1 (IRQ_BASE + 0x21c)
#define IRQ_DISABLE2 (IRQ_BASE + 0x220)
// Disable basic IRQs
#define IRQ_BASIC_DISABLE (IRQ_BASE + 0x224)

// IRQ types
// IRQ Timer 1 and 3 interrupt
#define IRQ_TIMER1 (1 << 1)
#define IRQ_TIMER2 (1 << 3)

#endif
