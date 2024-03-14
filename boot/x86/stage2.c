#include <stdint.h>
#include <bios.h>
#include <print.h>
#include <vesa/vga.h>
#include <vesa/vbe.h>
#include <fs/exfat.h>
#include <paging.h>

extern uint16_t drive_bytes_per_sector;
extern void kernel_jump(void);

// Page directory table
pde_4mib kernel_memory[1024] __attribute__((aligned(4096)));

void stage2(void) {
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
	kernel_size = read_file_exfat("/system/kernel", (uint8_t*)0x100000);
	if (kernel_size == 0) {
		// No kernel exists so halt
		print("No kernel detected at /system/kernel\r\n");
		while (1) {}
	}

	// Load system processes
	print("Loading system processes...\r\n");
	print("Loading VGA driver\r\n");
	print("Kernel Size: %x\r\n", kernel_size);
	if (read_file_exfat("/system/vga", (uint8_t*)(0x100000 + kernel_size)) == 0) {
		// No initial process detected
		print("System process does not exist\r\n");
	} else {
		// Initial process detected
		print("System process detected at %x. Passing info\r\n", 0x100000 + kernel_size);
	}

	// Initialize paging
	// First do a linear map
	for (int i = 0; i < 768; i++) {
		kernel_memory[i] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 0, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i};
	}
	// Map high memory
	for (int i = 0; i < 256; i++) {
		kernel_memory[i + 768] = (pde_4mib){.present = 1, .rw = 1, .us = 0, .pwt = 0, .pcd = 0, .a = 0, .d = 0, .ps = 1, .g = 0, .ignored = 0, .pat = 0, .highaddr = 0, .lowaddr = i};
	}

	// Enable paging
	enable_paging(kernel_memory);

	// Initialize VBE
	init_vbe();

	// Jump to kernel
	kernel_jump();

	while (1) {}
}
