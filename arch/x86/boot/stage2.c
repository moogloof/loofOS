#include <stdint.h>
#include <boot/vbe.h>
#include <boot/iso9660.h>
#include <boot/idt.h>
#include <boot/isr.h>
#include <boot/ata.h>
#include <boot/kernel_jump.h>

// The directories and files to search for
char system_path[6] = "SYSTEM";
char kernel_path[6] = "KERNEL";

// Get the saved mode info
extern struct vbe_cib __attribute__((packed)) vbe_info_block;
extern struct vbe_mib __attribute__((packed)) saved_mode_info_block;
extern char* logo_image;
extern uint32_t primary_volume_descriptor_position;

// Small utility function for checking strings
int verify_string(char*, char*, int);

// The main function for the second stage boot
void stage2_boot() {
	// The record to read
	primary_volume_descriptor vol_desc;
	uint8_t read_success;
	// Folder blocks to load
	uint8_t directory_blocks[2048];
	// The pointer inside the directory block
	uint8_t* directory_block_pointer = directory_blocks;
	// Flag whether the system folder or kernel has been found yet
	int dir_found = 0;

	// Init devices needed for boot stage 2
	init_pic();
	init_idt();
	init_ata();

	enable_interrupts();

	// Load the primary volume descriptor
	read_success = read_sectors(primary_volume_descriptor_position, 1, &vol_desc);

	// Get the root directory block
	read_success = read_sectors(((directory_record*)(vol_desc.root_directory_record))->extent_lba, 1, (uint16_t*)(directory_blocks));

	// Parse the root directory
	// Search for the SYSTEM folder first
	while (!dir_found && (*directory_block_pointer != 0) && (directory_block_pointer - directory_blocks < 2048)) {
		// Check whether it is a folder or not
		if (((directory_record*)directory_block_pointer)->flags.is_directory) {
			// Check whether the name matches up
			if (verify_string(system_path, ((directory_record*)directory_block_pointer)->identifier, 6)) {
				// If matching up, then go into
				read_success = read_sectors(((directory_record*)directory_block_pointer)->extent_lba, 1, (uint16_t*)(directory_blocks));
				dir_found = 1;
			}
		}

		// Go further
		directory_block_pointer += *directory_block_pointer;
	}

	// Check if dir found
	if (dir_found) {
		dir_found = 0;
		directory_block_pointer = directory_blocks;

		// Check for the root
		while (!dir_found && (*directory_block_pointer != 0) && (directory_block_pointer - directory_blocks < 2048)) {
			// Check whether it is a file or not
			if (!((directory_record*)directory_block_pointer)->flags.is_directory) {
				// Check whether the name matches up
				if (verify_string(kernel_path, ((directory_record*)directory_block_pointer)->identifier, 6)) {
					// If matching up, then go into
					// Get the lbs in sectors
					uint32_t kernel_lbs = ((directory_record*)directory_block_pointer)->data_length;
					if (kernel_lbs % 2048 > 0)
						kernel_lbs = (kernel_lbs / 2048) + 1;
					else
						kernel_lbs /= 2048;
					// Load kernel
					read_success = read_sectors(((directory_record*)directory_block_pointer)->extent_lba, kernel_lbs, 0x100000);
					dir_found = 1;
				}
			}

			// Go further
			directory_block_pointer += *directory_block_pointer;
		}
	}

	// When the kernel is found and read, jump to it
	if (dir_found) {
		// First setup some initial paging
		init_paging();
		// Then put some info in the 0x600 address to be used by kernel
		uint8_t* kernel_eggs = (uint8_t*)0x600;
		for (int i = 0; i < sizeof(struct vbe_cib); i++) {
			kernel_eggs[i] = (uint8_t*)(&vbe_info_block) + i;
		}
		kernel_eggs += 0x400;
		for (int i = 0; i < sizeof(struct vbe_mib); i++) {
			kernel_eggs[i] = (uint8_t*)(&saved_mode_info_block) + i;
		}
		// Then jump
		jump_to_kernel();
	}

	while (1);
}

int verify_string(char* truth, char* guess, int len) {
	// Check until length
	for (int i = 0; i < len; i++) {
		if (truth[i] != guess[i])
			return 0;
	}
	// The guess is correct
	return 1;
}
