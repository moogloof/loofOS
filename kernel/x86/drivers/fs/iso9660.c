#include <stdint.h>
#include <common.h>
#include <drivers/fs/iso9660.h>
#include <drivers/ata.h>
#include <mm/alloc.h>

// Obtain a file to read
// Reads will always start at the root directory
file_info* iso_read_file(char* path) {
	// Parse the path
	// The path must end with a null terminating character
	// The beginning of the directory name
	char* folder_start = path;
	// Length of the directory name
	int folder_len = 0;
	// Allocate a buffer for stuff
	uint16_t* vol_buffer = (uint16_t*)kernel_allocate(sizeof(generic_volume_record));
	// LBA to start at
	uint32_t vol_lba = 0x10;
	uint32_t vol_lbs;

	// Search for primary volume descriptor
	while (1) {
		// Load
		read_sectors(vol_lba++, 1, vol_buffer);

		// Check le type of record
		if (((generic_volume_record*)vol_buffer)->volume_type == 1) {
			// If primary volume descriptor go to root
			// Calculate size and lba first
			vol_lbs = (((directory_record*)(((primary_volume_descriptor*)vol_buffer)->root_directory_record))->data_length + 2047) / 2048;
			vol_lba = ((directory_record*)(((primary_volume_descriptor*)vol_buffer)->root_directory_record))->extent_lba;
			// Allocate new space
			kernel_free(vol_buffer);
			vol_buffer = (uint16_t*)kernel_allocate(vol_lbs * 2048);
			// Read sectors
			read_sectors(vol_lba, vol_lbs, vol_buffer);
			break;
		} else if (((generic_volume_record*)vol_buffer)->volume_type == 0xff) {
			// Volume not found, so exit with failure
			return 0;
		}
	}

	while (1) {
		// Get folder
		folder_len = 0;
		while (*(char*)(folder_start + folder_len) != '/' && *(char*)(folder_start + folder_len) != '\0')
			folder_len++;

		// No empty string folders sorry
		if (folder_len > 0) {
			// Search for directory
			// The start of the directory record
			directory_record* dir_start = vol_buffer;
			// Skip first two, which are . and ..
			dir_start = (uint8_t*)dir_start + 0x44;

			// Search for directory
			while (1) {
				// Check if names match up
				if (dir_start->record_length == 0) {
					// No file of that path
					return 0;
				} else if (verify_string(dir_start->identifier, folder_start, folder_len)) {
					// Load the dir
					vol_lba = dir_start->extent_lba;
					vol_lbs = (dir_start->data_length + 2047) / 2048;
					// Allocate new space
					kernel_free(vol_buffer);
					vol_buffer = (uint16_t*)kernel_allocate(vol_lbs * 2048);
					read_sectors(vol_lba, vol_lbs, vol_buffer);
					break;
				}
				// Go to next dir record
				dir_start = (uint8_t*)dir_start + dir_start->record_length;
			}
		} else {
			// Invalid string
			return 0;
		}

		// Exit if done
		if (*(char*)(folder_start + folder_len) == '\0') {
			file_info* returned_file = (file_info*)kernel_allocate(sizeof(file_info));
			returned_file->byte_length = vol_lbs * 2048;
			returned_file->file_buffer = (uint8_t*)vol_buffer;
			return returned_file;
		}

		// Go forward
		folder_start += folder_len + 1;
	}
}
