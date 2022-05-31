#ifndef RTC_DRIVER_H
#define RTC_DRIVER_H

#include <stdint.h>
#include <common.h>

// Define ports of RTC
// Index register port
#define RTC_INDEX (KERNEL_BASE + 0x70)
// Register data port
#define RTC_DATA (KERNEL_BASE + 0x71)

// Define registers of RTC
// Current second
#define RTC_REG_SECOND 0
// Current minute
#define RTC_REG_MINUTE 0x2
// Current hour
#define RTC_REG_HOUR 0x4
// Current week day
#define RTC_REG_WEEKDAY 0x6
// Current day of month
#define RTC_REG_MONTHDAY 0x7
// Current month
#define RTC_REG_MONTH 0x8
// Current year (0-99)
#define RTC_REG_YEAR 0x9
// Current century (19-20)
#define RTC_REG_CENTURY 0x32
// Status A
#define RTC_REG_STATUSA 0xa
// Status B
#define RTC_REG_STATUSB 0xb

// Define NMI status bit
#define RTC_NMI_DISABLE 0x80

// Struct to describe time
typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t weekday;
	uint8_t day_month;
	uint8_t month;
	uint16_t year;
} time_struct;

// Initialize the RTC and reset NMI
void init_rtc();

// Read the current time from CMOS
time_struct read_time();

#endif
