#include <bios.h>
#include <print.h>
#include <vesa/vga.h>
#include <vesa/vbe.h>
#include <fs/exfat.h>

extern uint16_t drive_bytes_per_sector;

void stage2() {
	// Size of kernel in bytes
	int kernel_size;

	// Initializing the VGA display
	reset_display();
	set_cursor_pos(0, 0);

	// Set color and print
	set_color(0, 0xf);
	set_outb(vga_output_char);

	// Do a VGA test print
	print("Starting stage2 boot...\r\n");

	// Initialize exfat
	print("Initializing the exFAT filesystem...\r\n");
	init_exfat();

	// Load kernel and check if kernel exists
	print("Loading kernel...\r\n");
	kernel_size = read_file_exfat("/system/kernel", 0x100000);
	if (kernel_size == 0) {
		// No kernel exists so halt
		print("No kernel detected at /system/kernel\r\n");
		while (1) {}
	}

	// Load system processes
	print("Loading system processes...\r\n");
	if (read_file_exfat("/system/proc0", 0x100000 + kernel_size) == 0) {
		// No initial process detected
		print("System process does not exist\r\n");
	} else {
		// Initial process detected
		print("System process detected at %x. Passing info\r\n", kernel_size);
	}

	// Initialize VBE
	init_vbe();

	while (1) {}
}
