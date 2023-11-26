#include <fs/exfat.h>
#include <stdint.h>
#include <string.h>
#include <bios.h>

extern uint16_t drive_bytes_per_sector;

// The boot record
struct {
	char jumpboot[3];
	char fs_name[8];
	uint8_t zero[53];
	uint64_t partition_offset;
	uint64_t volume_length;
	uint32_t fat_offset;
	uint32_t fat_length;
	uint32_t cluster_heap_offset;
	uint32_t cluster_count;
	uint32_t root_first_cluster;
	uint32_t volume_serial;
	uint16_t fs_revision;
	uint16_t volume_flags;
	uint8_t bytes_per_sector_shift;
	uint8_t sectors_per_cluster_shift;
	uint8_t number_of_fats;
	uint8_t drive_select;
	uint8_t percent_used;
	uint8_t reserved[7];
	char bootcode[390];
	char boot_signature[2];
} __attribute__((packed)) exfat_boot_record;

// The upcase table
uint16_t upcase_table[128];

// Get the heap LBA
static uint32_t get_heap_lba(uint32_t index) {
	return (0x100 + exfat_boot_record.cluster_heap_offset + ((index - 2) << exfat_boot_record.sectors_per_cluster_shift)) * 512 / drive_bytes_per_sector;
}

// Get the LBA that the FAT entry resides in
static uint32_t get_fat_lba(uint32_t index) {
	return (0x100 + exfat_boot_record.fat_offset + (index * 4 / 512)) * 512 / drive_bytes_per_sector;
}

// Get cluster size in sectors
static uint32_t get_cluster_size(uint32_t size) {
	return (size << exfat_boot_record.sectors_per_cluster_shift) * 512 / drive_bytes_per_sector;
}

// Initialize the exfat filesystem
void init_exfat() {
	// Load buffer
	uint8_t load_buffer[drive_bytes_per_sector];
	// Buffer index
	int buffer_i;

	// Load boot record
	bios_ext_read(load_buffer, 0x100 * 512 / drive_bytes_per_sector, 1);
	memcpy(&exfat_boot_record, load_buffer, sizeof(exfat_boot_record));

	// Fill out upcase default
	for (int i = 0; i < 128; i++)
		upcase_table[i] = i;

	// Load root directory
	bios_ext_read(load_buffer, get_heap_lba(exfat_boot_record.root_first_cluster), 1);
	// Search for and load upcase table
	for (int i = 0; i < drive_bytes_per_sector; i += 32) {
		if (load_buffer[i] == 0x82) {
			exfat_upcase_dir_entry* upcase_entry = &(load_buffer[i]);

			// Load the upcase cluster
			bios_ext_read(load_buffer, get_heap_lba(upcase_entry->first_cluster), 1);
			memcpy(upcase_table, load_buffer, 128);

			break;
		}
	}
}

// Read a file
int _read_file_exfat_helper(const char* name, int name_len, exfat_generic_dir_entry* dir_buffer, int clusters, uint8_t* dest_buffer) {
	// Temp name buffer
	uint16_t filename[255] = {0};

	// Number of clusters written
	int clusters_written = 0;

	// Loop through the directory entries
	for (int i = 0; i < drive_bytes_per_sector * get_cluster_size(clusters) / 32; i++) {
		// Check for file directory entries
		if (dir_buffer[i].entry_type == 0x85) {
			// Store number of name entries
			int name_entry_count = ((exfat_file_dir_entry*)(dir_buffer + i))->secondary_count - 1;
			// Get the stream extension entry
			exfat_extension_dir_entry extension_entry;
			memcpy(&extension_entry, dir_buffer + i + 1, sizeof(exfat_extension_dir_entry));

			// Get the name of the file
			for (int j = 0; j < name_entry_count; j++) {
				// Get name entry
				exfat_name_dir_entry name_entry;
				memcpy(&name_entry, dir_buffer + i + j + 2, sizeof(exfat_name_dir_entry));
				// Copy over the characters
				for (int x = 0; x < 15; x++) {
					filename[j*15 + x] = name_entry.name[x];
				}
			}
			// Apply upcase table to the filename
			for (int j = 0; j < 255; j++) {
				filename[j] = upcase_table[filename[j]];
			}

			// Make sure lengths for filename and name are equal
			if (extension_entry.name_length == name_len) {
				// Compare the filename
				// Flag for equal name
				int name_equal = 1;
				for (int j = 0; j < name_len; j++) {
					if (filename[j] != name[j]) {
						name_equal = 0;
						break;
					}
				}

				// If names equal, then load file
				if (name_equal == 1) {
					// First cluster and current cluster
					int current_cluster_id = extension_entry.first_cluster;

					// indexes per cluster
					int id_per_cluster = drive_bytes_per_sector * get_cluster_size(1) / 4;

					// Temp FAT storage
					uint32_t fat_buffer[id_per_cluster];

					// Write first FAT to buffer
					bios_ext_read((uint8_t*)fat_buffer, get_fat_lba(current_cluster_id), get_cluster_size(1));

					// Check whether file is fragmented or not
					if (fat_buffer[current_cluster_id % id_per_cluster] == 0) {
						// Simply loop through unfragmented
						for (int j = 0; j < (int)(extension_entry.data_length + get_cluster_size(1) * drive_bytes_per_sector - 1) / (get_cluster_size(1) * drive_bytes_per_sector); j++) {
							bios_ext_read(dest_buffer + (j * drive_bytes_per_sector * get_cluster_size(1)), get_heap_lba(current_cluster_id + j), get_cluster_size(1));
						}
					} else {
						// Number of clusters read
						int clusters_read = 0;

						// Link through FAT
						while (1) {
							// Load the cluster in data
							bios_ext_read(dest_buffer + drive_bytes_per_sector * get_cluster_size(1) * (clusters_read++), get_heap_lba(current_cluster_id), get_cluster_size(1));

							current_cluster_id = fat_buffer[current_cluster_id % id_per_cluster];

							if (current_cluster_id == 0xffffffff || current_cluster_id == 0xfffffff7) {
								// End or bad cluster
								break;
							} else {
								// Load FAT sector
								bios_ext_read((uint8_t*)fat_buffer, get_fat_lba(current_cluster_id), get_cluster_size(1));
							}
						}
					}

					// Return clusters read
					return (int)(extension_entry.data_length + get_cluster_size(1) * drive_bytes_per_sector - 1) / (get_cluster_size(1) * drive_bytes_per_sector);
				}
			}

			// Increment stuff
			i += name_entry_count + 1;
		}
	}

	// File not found, so return 0
	return 0;
}

// Read the exfat path
// NOTE: This assumes the path ends with a \0 character
int read_file_exfat(const char* path, uint8_t* dest) {
	// Length of one path block
	int path_length = 0;
	// Length in clusters of dir block
	int dirs_clusters = 1;

	// Load the root block
	bios_ext_read(dest, get_heap_lba(exfat_boot_record.root_first_cluster), get_cluster_size(1));

	// Loop through path string
	for (int i = 0; path[i] != '\0'; i++) {
		// Skip separators
		if (path[i] != '/') {
			// Get the path name length
			for (path_length = 0; path[i + path_length] != '/' && path[i + path_length] != '\0'; path_length++);

			// Load the stuff in the destination
			dirs_clusters = _read_file_exfat_helper(path + i, path_length, (exfat_generic_dir_entry*)dest, dirs_clusters, dest);

			// No file discovered then exit
			if (dirs_clusters == 0) {
				return 0;
			}

			// Go forward
			i += path_length - 1;
		}
	}

	return dirs_clusters * get_cluster_size(1) * drive_bytes_per_sector;
}
