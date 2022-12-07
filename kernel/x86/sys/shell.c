#include <sys/shell.h>
#include <drivers/vga_text.h>
#include <drivers/rtc.h>

// The command buffer
static char command_buffer[256];
static int command_index;
static int command_passed;

// Run the shell
void shell_app() {
	// Stuff
	reset_display();
	set_color(0, 0xa);
	kernel_print("Started shell.\r\n");
	set_color(0, 0xf);

	// Go run the shell loop
	while (1) {
		command_index = 0;
		command_passed = 0;
		for (int i = 0; i < 256; i++)
			command_buffer[command_index] = 0;

		kernel_print("> ");

		while (1) {
			if (command_passed) {
				int i;
				for (i = 0; command_buffer[i] != 0; i++);
				if (i == 0) {
				} else if (verify_string("date", command_buffer, i)) {
					time_struct cur_time = read_time();
					kernel_print("It's %d/%d/%d\r\n", (int)cur_time.month, (int)cur_time.day_month, (int)cur_time.year);
				} else if (verify_string("time", command_buffer, i)) {
					time_struct cur_time = read_time();
					kernel_print("It's %d:%d\r\n", (int)cur_time.hours, (int)cur_time.minutes);
				} else if (verify_string("reset display", command_buffer, i)) {
					reset_display();
				} else {
					kernel_print("That's not a command.\r\n");
				}
				break;
			}
		}
	}
}

// Handle key input
void handle_input(char c) {
	if (c == '\n' || c == 13) {
		output_char('\r');
		output_char('\n');
		command_passed = 1;
	}
	else if (c == 8 || c == 127) {
		if (command_index > 0) {
			output_char(c);
			command_buffer[--command_index] = 0;
		}
	} else {
		if (command_index < 255) {
			output_char(c);
			command_buffer[command_index++] = c;
		}
	}
}
