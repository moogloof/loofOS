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

	// Stuff needed
	int sign_integer, reversed_int, integer_len;
	uint32_t sub_integer;
	char* nstr;

	// Print all characters
	while (cur_char) {
		if (substituting) {
			// Process substitution
			switch (cur_char) {
				case 'c':
					// Substitute a character
					outb(va_arg(arglist, char));
					break;
				case 'd':
					// Substitute a signed integer in decimal
					sign_integer = va_arg(arglist, int);
					reversed_int = 0;
					integer_len = 0;

					// Output sign if present
					if (sign_integer < 0) {
						outb('-');
						sign_integer = -sign_integer;
					}

					// Get length of integer and reverse integer
					while (sign_integer > 0) {
						reversed_int *= 10;
						reversed_int += sign_integer % 10;
						sign_integer /= 10;
						integer_len++;
					}

					integer_len = integer_len ? integer_len : 1;

					// Output in decimal
					for (int dec_place = integer_len-1; dec_place >= 0; dec_place--) {
						outb(hex_chars[reversed_int % 10]);
						reversed_int /= 10;
					}
					break;
				case 's':
					// Substitute a string
					nstr = va_arg(arglist, char*);

					// Print the string
					while (nstr) {
						outb(*(nstr++));
					}
					break;
				case 'x':
					// Substitute an unsigned integer in hex
					// Get integer arg
					sub_integer = va_arg(arglist, uint32_t);

					// Output in hex
					for (int hex_place = 7; hex_place >= 0; hex_place--) {
						// Output digit
						outb(hex_chars[(sub_integer >> (hex_place * 4)) & 0xf]);
					}
					break;
				case 'b':
					// Substitute an unsigned integer in binary
					// Get integer arg
					sub_integer = va_arg(arglist, uint32_t);

					// Output in binary
					for (int bin_place = 31; bin_place >= 0; bin_place--) {
						// Output digit
						outb(((sub_integer >> bin_place) & 1) ? '1' : '0');
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
