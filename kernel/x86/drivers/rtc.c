#include <stdint.h>
#include <drivers/rtc.h>
#include <core/port.h>

// Initialize the RTC and reset NMI
void init_rtc() {
	// Reset NMI
	// Select 0 register (seconds)
	outportb(RTC_INDEX, RTC_NMI_DISABLE);
	// Clear action pending
	inportb(RTC_DATA);

	// Select 24 hour mode and binary mode and daylight savings time
	outportb(RTC_INDEX, RTC_NMI_DISABLE | RTC_REG_STATUSB);
	outportb(RTC_DATA, 0b00000110);
}

// Read the current time from CMOS
time_struct read_time() {
	// Time struct to return
	time_struct cur_time;

	// Wait for toggle of status register A
	// Wait for RTC to update
/*	do {
		// Set to read status A register
		outportb(RTC_INDEX, RTC_NMI_DISABLE | RTC_REG_STATUSA);
	} while (~(inportb(RTC_DATA) & 0b10000000));
	// Wait for RTC to finish updating
*/	do {
		// Set to read status A register
		outportb(RTC_INDEX, RTC_NMI_DISABLE | RTC_REG_STATUSA);
	} while (inportb(RTC_DATA) & 0b10000000);

	// Read the current time
	// YOU MUST DO THIS SUPAH FAST
	// Read current seconds
	outportb(RTC_INDEX, RTC_NMI_DISABLE | RTC_REG_SECOND);
	cur_time.seconds = inportb(RTC_DATA);
	// Read current minutes
	outportb(RTC_INDEX, RTC_NMI_DISABLE | RTC_REG_MINUTE);
	cur_time.minutes = inportb(RTC_DATA);
	// Read current hour
	outportb(RTC_INDEX, RTC_NMI_DISABLE | RTC_REG_HOUR);
	cur_time.hours = inportb(RTC_DATA);
	// Read current weekday
	outportb(RTC_INDEX, RTC_NMI_DISABLE | RTC_REG_WEEKDAY);
	cur_time.weekday = inportb(RTC_DATA);
	// Read current day of month
	outportb(RTC_INDEX, RTC_NMI_DISABLE | RTC_REG_MONTHDAY);
	cur_time.day_month = inportb(RTC_DATA);
	// Read current month
	outportb(RTC_INDEX, RTC_NMI_DISABLE | RTC_REG_MONTH);
	cur_time.month = inportb(RTC_DATA);
	// Read current year
	// Get year
	outportb(RTC_INDEX, RTC_NMI_DISABLE | RTC_REG_YEAR);
	cur_time.year = inportb(RTC_DATA);
	// Get century
	outportb(RTC_INDEX, RTC_NMI_DISABLE | RTC_REG_CENTURY);
	cur_time.year += 100 * (uint16_t)inportb(RTC_DATA);

	// Return the time
	return cur_time;
}
