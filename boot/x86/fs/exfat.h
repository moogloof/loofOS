#ifndef FS_EXFAT
#define FS_EXFAT

#include <stdint.h>

// The generic directory entry form
typedef struct {
	uint8_t entry_type;
	uint8_t custom[31];
} __attribute__((packed)) exfat_generic_dir_entry;

// Up-case table directory entry
typedef struct {
	uint8_t entry_type; // 0x82
	uint8_t reserved[3];
	char checksum[4];
	uint8_t reserved2[12];
	uint32_t first_cluster;
	uint64_t data_length;
} __attribute__((packed)) exfat_upcase_dir_entry;

// Volume label directory entry
typedef struct {
	uint8_t entry_type; // 0x83
	uint8_t character_count;
	uint16_t volume_label[11];
	uint8_t reserved[8];
} __attribute__((packed)) exfat_volume_label_dir_entry;

// File attributes
typedef struct {
	uint8_t readonly:1;
	uint8_t hidden:1;
	uint8_t system:1;
	uint8_t reserved:1;
	uint8_t directory:1;
	uint8_t archive:1;
	uint16_t reserved2:10;
} __attribute__((packed)) exfat_file_attributes;

// Timestamp format
typedef struct {
	uint8_t double_seconds:5;
	uint8_t minute:6;
	uint8_t hour:5;
	uint8_t day:5;
	uint8_t month:4;
	uint8_t year:7;
} __attribute__((packed)) exfat_timestamp;

// File directory entry
typedef struct {
	uint8_t entry_type; // 0x85
	uint8_t secondary_count;
	char checksum[2];
	exfat_file_attributes attributes;
	uint16_t reserved;
	exfat_timestamp create_time;
	exfat_timestamp last_modified_time;
	exfat_timestamp last_accessed_time;
	uint8_t create_time_10ms;
	uint8_t last_modified_10ms;
	uint8_t create_timezone;
	uint8_t last_modified_timezone;
	uint8_t last_accessed_timezone;
	uint8_t reserved2[7];
} __attribute__((packed)) exfat_file_dir_entry;

// General flags attribute
typedef struct {
	uint8_t allocation_possible:1;
	uint8_t nofatchain:1;
	uint8_t custom:6;
} __attribute__((packed)) exfat_general_flags;

// File stream extension entry
typedef struct {
	uint8_t entry_type; // 0xc0
	exfat_general_flags flags;
	uint8_t reserved;
	uint8_t name_length;
	char name_hash[2];
	uint8_t reserved2[2];
	uint64_t valid_data_length;
	uint8_t reserved3[4];
	uint32_t first_cluster;
	uint64_t data_length;
} __attribute__((packed)) exfat_extension_dir_entry;

// File name directory entry
typedef struct {
	uint8_t entry_type; // 0xc1
	exfat_general_flags flags;
	uint16_t name[15];
} __attribute__((packed)) exfat_name_dir_entry;

// Bitmap flags attribute
typedef struct {
	uint8_t id:1;
	uint8_t reserved:7;
} __attribute__((packed)) exfat_bitmap_flags;

// Bitmap allocation entry
typedef struct {
	uint8_t entry_type; // 0x81
	exfat_bitmap_flags flags;
	uint8_t reserved[18];
	uint32_t first_cluster;
	uint64_t data_length;
} __attribute__((packed)) exfat_bitmap_dir_entry;

// Volume GUID directory entry
typedef struct {
	uint8_t entry_type; // 0xa0
	uint8_t secondary_count;
	char checksum[2];
	exfat_general_flags flags;
	char guid[16];
	uint8_t reserved[10];
} __attribute__((packed)) exfat_volume_guid_dir_entry;

// Initialize the exfat filesystem
void init_exfat(void);

// Read the exfat file
int _read_file_exfat_helper(const char*, int, exfat_generic_dir_entry*, int, uint8_t*);

// Read the exfat path
int read_file_exfat(const char*, uint8_t*);

#endif
