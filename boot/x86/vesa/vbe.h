#ifndef BOOT_VBE
#define BOOT_VBE

#include <stdint.h>

typedef struct {
	uint16_t mode_attributes; // The attributes of the mode
	uint8_t windowa_attributes;
	uint8_t windowb_attributes;
	uint16_t window_granularity;
	uint16_t window_size;
	uint16_t windowa_segment;
	uint16_t windowb_segment;
	uint32_t window_function_ptr;
	uint16_t bytes_per_line;
	uint16_t xresolution;
	uint16_t yresolution;
	uint8_t xchar_size;
	uint8_t ychar_size;
	uint8_t number_of_planes;
	uint8_t bits_per_pixel;
	uint8_t number_of_banks;
	uint8_t memory_model;
	uint8_t bank_size;
	uint8_t number_of_image_pages;
	uint8_t reserved;
	uint8_t red_mask_size;
	uint8_t red_field_position;
	uint8_t green_mask_size;
	uint8_t green_field_position;
	uint8_t blue_mask_size;
	uint8_t blue_field_position;
	uint8_t rsvd_mask_size;
	uint8_t rsvd_field_position;
	uint8_t direct_color_mode_info;
	uint32_t phys_base_ptr;
	uint32_t reserved2;
	uint16_t reserved3;
	uint16_t lin_bytes_per_scan_line;
	uint8_t bnk_number_of_image_pages;
	uint8_t lin_number_of_image_pages;
	uint8_t lin_red_mask_size;
	uint8_t lin_red_field_position;
	uint8_t lin_green_mask_size;
	uint8_t lin_green_field_position;
	uint8_t lin_blue_mask_size;
	uint8_t lin_blue_field_position;
	uint8_t lin_rsvd_mask_size;
	uint8_t lin_rsvd_field_position;
	uint32_t max_pixel_clock;
	uint8_t reserved4[189];
} __attribute__((packed)) vbe_mib;

// Initialize vbe
void init_vbe(void);

#endif
