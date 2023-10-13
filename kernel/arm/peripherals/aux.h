#ifndef _P_AUX_H
#define _P_AUX_H

// Define Auxiliary registers
// Base address of registers
#define AUX_BASE 0x7e215000

// Auxiliary interrupt status
#define AUX_IRQ (AUX_BASE)
// Auxiliary enables
#define AUX_ENABLES (AUX_BASE + 0x4)

// Mini Uart I/O data
#define AUX_MU_IO_REG (AUX_BASE + 0x40)
// Mini Uart interrupt enable
#define AUX_MU_IER_REG (AUX_BASE + 0x44)
// Mini Uart interrupt identify
#define AUX_MU_IIR_REG (AUX_BASE + 0x48)
// Mini Uart line control
#define AUX_MU_LCR_REG (AUX_BASE + 0x4c)
// Mini Uart modem control
#define AUX_MU_MCR_REG (AUX_BASE + 0x50)
// Mini Uart line status
#define AUX_MU_LSR_REG (AUX_BASE + 0x54)
// Mini Uart modem status
#define AUX_MU_MSR_REG (AUX_BASE + 0x58)
// Mini Uart scratch
#define AUX_MU_SCRATCH (AUX_BASE + 0x5c)
// Mini Uart extra control
#define AUX_MU_CNTL_REG (AUX_BASE + 0x60)
// Mini Uart extra status
#define AUX_MU_STAT_REG (AUX_BASE + 0x64)
// Mini Uart baudrate
#define AUX_MU_BAUD_REG (AUX_BASE + 0x68)

// SPI 1 control register 0
#define AUX_SPI1_CTNL0_REG (AUX_BASE + 0x80)
// SPI 1 control register 1
#define AUX_SPI1_CNTL1_REG (AUX_BASE + 0x84)
// SPI 1 status
#define AUX_SPI1_STAT_REG (AUX_BASE + 0x88)
// SPI 1 data
#define AUX_SPI1_IO_REG (AUX_BASE + 0x90)
// SPI 1 peek
#define AUX_SPI1_PEEK_REG (AUX_BASE + 0x94)

// SPI 2 control register 0
#define AUX_SPI2_CNTL0_REG (AUX_BASE + 0xc0)
// SPI 2 control register 1
#define AUX_SPI2_CNTL1_REG (AUX_BASE + 0xc4)
// SPI 2 status
#define AUX_SPI2_STAT_REG (AUX_BASE + 0xc8)
// SPI 2 data
#define AUX_SPI2_IO_REG (AUX_BASE + 0xd0)
// SPI 2 peek
#define AUX_SPI2_PEEK_REG (AUX_BASE + 0xd4)

#endif
