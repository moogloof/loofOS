#include <stdint.h>
#include <bios.h>

extern uint16_t drive_bytes_per_sector;
extern uint8_t drive_number;

// BIOS ext read
void bios_ext_read(uint8_t* output_address, uint32_t lba_offset, uint32_t length) {
	// Make buffer
	uint8_t temp_buffer[drive_bytes_per_sector];
	// Make packet
	bios_dap temp_dap = (bios_dap){.dap_size = 0x10, .unused = 0, .load_size = 1, .load_offset = (int)temp_buffer, .load_segment = 0, .lba_low = lba_offset, .lba_high = 0};
	// Make registers
	bios_registers registers = (bios_registers){.eax = 0x4200, .ebx = 0, .ecx = 0, .edx = drive_number, .esi = (int)(&temp_dap), .edi = 0};

	// Read loop
	for (int i = 0; i < length; i++) {
		// Read using bios calls
		_bios_int(registers, 0x13, 0);

		// Copy the buffer to correct place
		for (int j = 0; j < drive_bytes_per_sector; j++) {
			output_address[j + i*drive_bytes_per_sector] = temp_buffer[j];
		}

		if (++temp_dap.lba_low == 0)
			temp_dap.lba_high++;
	}
}

// BIOS memory scan
void bios_mem_scan(address_range* buffer, int* length) {
	// Make command registers
	bios_registers registers = (bios_registers){
		.eax = 0xe820,
		.ebx = 0,                     // 0 at first, don't change after
		.ecx = sizeof(address_range), // Minimum size must be 20
		.edx = 0x534d4150,            // Must be SMAP
		.esi = 0,
		.edi = (uint32_t)buffer
	};
	// Result register buffer
	bios_registers result_registers = {0};

	// Scan loop
	for (int i = 0; i < *length; i++) {
		// Scan one block
		_bios_int(registers, 0x15, &result_registers);

		if (result_registers.ebx == 0) {
			// Set actual length
			*length = i + 1;
			break;
		}

		// Go to next block
		registers.ebx = result_registers.ebx;
		registers.edi += sizeof(bios_registers);
	}
}
