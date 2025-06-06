#ifndef _P_UART_H
#define _P_UART_H

#include <peripherals/base.h>

// UART registers
// Base UART address
#define UART_BASE (P_BASE + 0x201000)

// Data register
#define UART_DR (UART_BASE)
// Nothing
#define UART_RSRECR (UART_BASE + 0x4)
// Flag register
#define UART_FR (UART_BASE + 0x18)
// Not used
#define UART_ILPR (UART_BASE + 0x20)
// Integer baud rate divisor
#define UART_IBRD (UART_BASE + 0x24)
// Fractional baud rate divisor
#define UART_FBRD (UART_BASE + 0x28)
// Line control register
#define UART_LCRH (UART_BASE + 0x2c)
// Control register
#define UART_CR (UART_BASE + 0x30)
// Interrupt FIFO level select register
#define UART_IFLS (UART_BASE + 0x34)
// Interrupt mask set clear register
#define UART_IMSC (UART_BASE + 0x38)
// Raw interrupt status register
#define UART_RIS (UART_BASE + 0x3c)
// Masked interrupt status register
#define UART_MIS (UART_BASE + 0x40)
// Interrupt clear register
#define UART_ICR (UART_BASE + 0x44)
// DMA control register
#define UART_DMACR (UART_BASE + 0x48)
// Test control register
#define UART_ITCR (UART_BASE + 0x80)
// Integration test input register
#define UART_ITIP (UART_BASE + 0x84)
// Integration test output register
#define UART_ITOP (UART_BASE + 0x88)
// Test data register
#define UART_TDR (UART_BASE + 0x8c)

#endif
