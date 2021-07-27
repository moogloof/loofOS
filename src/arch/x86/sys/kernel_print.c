#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/kernel_print.h>

// Output char function
static void (*outb)(char);
// Hex code characters
static const char* hex_chars = "0123456789ABCDEF";

// Set writeb
void set_outb(void (*func)(char)) {
	outb = func;
}

// Print for kernel
void kernel_print(const char* format, ...) {
	va_list arglist;

	// Initialize arguments
	va_start(arglist, format);

	// Whether currently processing substitution or not
	bool substituting = false;
	// Offset
	int offset = 0;
	// Character to analyze
	char cur_char = *format;

	// Print all characters
	while (cur_char) {
		if (substituting) {
			// Process substitution
			switch (cur_char) {
				case 'c':
					// Substitute a character
					outb(va_arg(arglist, char));
					break;
				case 's':
					// Substitute a string
					char* nstr = va_arg(arglist, char*);

					// Print the string
					while (nstr) {
						outb(*(nstr++));
					}
					break;
				case 'x':
					// Substitute an unsigned integer in hex
					// Get integer arg
					uint32_t sub_integer = va_arg(arglist, uint32_t);

					// Output hex header
					outb('0');
					outb('x');

					// Output in hex
					for (int hex_place = 7; hex_place >= 0; hex_place--) {
						// Output digit
						outb(hex_chars[(sub_integer >> (hex_place * 4)) & 0xf]);
					}
					break;
				case '%':
					// Substitute with %
					outb('%');
					break;
				default:
					// Just ignore
					break;
			}

			// Reset substituting
			substituting = false;
		} else if (cur_char == '%') {
			// Switch to substitution mode
			substituting = true;
		} else {
			// Just print the char
			outb(cur_char);
		}

		// Change cur_char
		cur_char = *(format + ++offset);
	}

	// Cleanup variable argument list
	va_end(arglist);
}
