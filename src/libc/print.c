#include <stdint.h>
#include <print.h>


// Int to string
static int inttostr(long it, uint8_t base, char* str) {
	char* pstr = str;
	uint8_t neg = 0;
	int len;

	// No support for weird bases
	if (base > 16 || base < 2) {
		return -1;
	}

	// Negative sign
	if (it < 0) {
		neg = 1;
		it = -it;
	}

	// Int to string by base
	do {
		// Find digit
		long digit = it;
		long quot = 0;
		while (digit >= base) {
			quot++;
			digit -= base;
		}

		// Add to string
		*(pstr++) = (digit < 10) ? ('0' + digit) : ('A' + digit - 10);

		// Shift by base
		it = quot;
	} while (it != 0);

	// Negative sign add to string
	if (neg) {
		*(pstr++) = '-';
	}

	// Reverse string output
	len = pstr - str + neg;
	for (int i = 0; i < len/2; i++) {
		char dup = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = dup;
	}

	// Return string length
	return len;
}

// Print a string
void print(void (*_puts)(char* c), char* str) {
	(*_puts)(str);
}

// Print an int
void printint(void (*_puts)(char* c), long it, int len, uint8_t base) {
	char str[len];
	inttostr(it, base, str);
	print(_puts, str);
}

