#include <stdint.h>
#include <bios.h>

extern uint16_t drive_bytes_per_sector;
extern uint8_t drive_number;

// BIOS ext read
void bios_ext_read(uint8_t* output_address, uint32_t lba_offset, uint32_t length) {
	// Make buffer
	uint8_t temp_buffer[drive_bytes_per_sector];
	// Make packet
	bios_dap temp_dap = (bios_dap){.dap_size = 0x10, .unused = 0, .load_size = 1, .load_offset = temp_buffer, .load_segment = 0, .lba_low = lba_offset, .lba_high = 0};
	// Make registers
	bios_registers registers = (bios_registers){.eax = 0x4200, .ebx = 0, .ecx = 0, .edx = drive_number, .esi = &temp_dap, .edi = 0};

	// Read loop
	for (int i = 0; i < length; i++) {
		// Read using bios calls
		_bios_int(registers, 0x13);

		// Copy the buffer to correct place
		for (int j = 0; j < drive_bytes_per_sector; j++) {
			output_address[j + i*drive_bytes_per_sector] = temp_buffer[j];
		}

		if (++temp_dap.lba_low == 0)
			temp_dap.lba_high++;
	}
}
