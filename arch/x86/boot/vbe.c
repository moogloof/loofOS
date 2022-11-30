#include <stdint.h>
#include <drivers/vbe.h>

// The VBE info block
struct vbe_cib __attribute__((packed)) vbe_info_block;

// The VBE mode info block
struct vbe_mib __attribute__((packed)) mode_info_block;
// The saved one
struct vbe_mib __attribute__((packed)) saved_mode_info_block;

// The dimensions of the thing
// The selected dimensions
uint16_t selected_width;
uint16_t selected_height;
uint16_t selected_vbe_mode;
