#include <stdint.h>
#include <boot/vbe.h>

// The VBE info block
vbe_cib vbe_info_block;

// The VBE mode info block
vbe_mib mode_info_block;
// The saved one
vbe_mib saved_mode_info_block;

// The dimensions of the thing
// The selected dimensions
uint16_t selected_width;
uint16_t selected_height;
uint16_t selected_vbe_mode;
