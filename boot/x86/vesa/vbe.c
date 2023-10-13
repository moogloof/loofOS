#include <vesa/vbe.h>
#include <common.h>
#include <bios.h>

struct {
	char vbe_signature[4]; // The VBE signature, it must be VBE2
	uint16_t version; // The version, make it 0x300
	uint16_t* oem_string_ptr; // Who cares
	uint8_t capabilities[4]; // Who cares, just make sure bit 1 is set
	uint16_t* video_mode_ptr; // List of video modes
	uint16_t total_memory; // The total amount of memory in 64KB blocks
	uint8_t reserved[492]; // Other stuff
} __attribute__((packed)) vbe_cib;

// Initialize vbe
void init_vbe() {
	// Set stuff
	vbe_cib.vbe_signature[0] = 'V';
	vbe_cib.vbe_signature[1] = 'B';
	vbe_cib.vbe_signature[2] = 'E';
	vbe_cib.vbe_signature[3] = '2';
	vbe_cib.version = 0x300;
	// Trash buffer to use for CRTC block
	uint8_t crtc[64] = {0};
	// Registers to use for the int
	bios_registers registers = (bios_registers){.eax = 0x4f00, .ebx = 0, .ecx = 0, .edx = 0, .esi = 0, .edi = &vbe_cib};
	// Get the controller information
	_bios_int(registers, 0x10);

	// Iterate through the list of modes
	for (int i = 0; vbe_cib.video_mode_ptr[i] != 0xffff; i++) {
		// Get the video mode info
		vbe_mib video_mode_info = {0};
		registers.eax = 0x4f01;
		registers.ecx = vbe_cib.video_mode_ptr[i];
		registers.edi = &video_mode_info;
		_bios_int(registers, 0x10);

		// Check if video mode info is compatible
		// First check for linear frame buffer
		if ((video_mode_info.mode_attributes >> 7) & 1) {
			// Check for width
			if (video_mode_info.xresolution == 1280) {
				// Check for pixel depth
				if (video_mode_info.bits_per_pixel == 24) {
					// Switch to the video mode
					registers.eax = 0x4f02;
					registers.ebx = vbe_cib.video_mode_ptr[i];
					registers.edi = crtc;
					_bios_int(registers, 0x10);
					break;
				}
			}
		}
	}
}
