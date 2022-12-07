#ifndef BOOT_ISO9660
#define BOOT_ISO9660

#include <stdint.h>

// The datetime record in the directory
typedef struct {
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t timezone;
} __attribute__((packed)) dir_datetime_record;

// The datetime record in the volume record
typedef struct {
	char year[4];
	char month[2];
	char day_of_month[2];
	char hour[2];
	char minute[2];
	char second[2];
	char centisecond[2];
	uint8_t timezone;
} __attribute__((packed)) vol_datetime_record;

// A generic volume record to cast into one of the 3 below
typedef struct {
	uint8_t volume_type;
	char standard_id[5];
	uint8_t version;
	uint8_t data[2041];
} __attribute__((packed)) generic_volume_record;

// The boot record
typedef struct {
	uint8_t volume_type;
	char standard_id[5]; // Always CD001
	uint8_t version;
	char boot_system_id[32];
	char boot_id[32];
	uint8_t reserved[1977];
} __attribute__((packed)) boot_record;

// The volume record terminator
typedef struct {
	uint8_t volume_type;
	char standard_id[5]; // Always CD001
	uint8_t version;
	uint8_t reserved[2041];
} __attribute__((packed)) volume_descriptor_terminator;

// The primary volume record
typedef struct {
	uint8_t volume_type;
	char standard_id[5];
	uint8_t version;
	uint8_t unused;
	char system_id[32];
	char volume_id[32];
	uint8_t unused2[8];
	uint32_t volume_space_size;
	uint32_t volume_space_size_big;
	uint8_t unused3[32];
	uint16_t volume_set_size;
	uint16_t volume_set_size_big;
	uint16_t volume_sequence_number;
	uint16_t volume_sequence_number_big;
	uint16_t lbs;
	uint16_t lbs_big;
	uint32_t path_table_size;
	uint32_t path_table_size_big;
	uint32_t path_table_lba;
	uint32_t path_table_lba_optional;
	uint32_t path_table_big_lba;
	uint32_t path_table_big_lba_optional;
	uint8_t root_directory_record[34]; // Note that the type is not put here. Instead, simply cast the array to the directory record struct.
	char volume_set_id[128];
	char publisher_id[128];
	char data_preparer_id[128];
	char application_id[128];
	char copyright_file_id[37];
	char abstract_file_id[37];
	char bibliography_file_id[37];
	vol_datetime_record volume_creation_date;
	vol_datetime_record volue_modification_date;
	vol_datetime_record volume_expiration_date;
	vol_datetime_record volume_effective_date;
	uint8_t file_structure_version;
	uint8_t unused4;
	uint8_t reserved[1165];
} __attribute__((packed)) primary_volume_descriptor;

// The file flags for directory entry
typedef struct {
	uint8_t hidden:1;
	uint8_t is_directory:1;
	uint8_t is_associated:1;
	uint8_t is_extended:1;
	uint8_t has_permissions:1;
	uint8_t reserved:2;
	uint8_t not_final:1;
} __attribute__((packed)) file_flags;

// The directory record
typedef struct {
	uint8_t record_length;
	uint8_t attribute_length;
	uint32_t extent_lba;
	uint32_t extent_lba_big;
	uint32_t data_length;
	uint32_t data_length_big;
	dir_datetime_record recording_date;
	file_flags flags;
	uint8_t unit_size;
	uint8_t interleave_gap;
	uint16_t volume_sequence_number;
	uint16_t volume_sequence_number_big;
	uint8_t identifier_length;
	char identifier[];
} __attribute__((packed)) directory_record;

// File read info
typedef struct {
	uint32_t byte_length;
	uint8_t* file_buffer;
} file_info;

// Obtain a file to read
file_info* iso_read_file(char*);

#endif
