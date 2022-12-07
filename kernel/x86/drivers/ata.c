#include <stdint.h>
#include <boot/ata.h>
#include <boot/port.h>
#include <boot/isr.h>
#include <boot/idt.h>

// ATA command done
static uint8_t ata_done_flag;

// The type of ATA port
// 0 == Does not exist
// 1 == ATA PIO
// 2 == ATAPI
static uint8_t ata_type;

// Data read from IDENTIFY command
static uint16_t identify_block[256];

// Init the ATA driver
void init_ata() {
	// Select master
	outportb(ATA_DRIVE, 0xa0);

	// Reset ports
	outportb(ATA_SECTOR_COUNT, 0);
	outportb(ATA_SECTOR_NUMBER, 0);
	outportb(ATA_CYLINDER_LOW, 0);
	outportb(ATA_CYLINDER_HIGH, 0);

	// Send identify command
	outportb(ATA_COMMAND, 0xec);

	// Get status
	uint8_t ata_status = inportb(ATA_STATUS);
	// Check if drive exists
	if (ata_status == 0) {
		ata_type = 0;
		return;
	}

	// Wait until not busy
	while ((inportb(ATA_STATUS) >> 7) & 1);

	// Check if error
	if (ata_status & 1) {
		// Check whether ATAPI or not
		if (inportb(ATA_CYLINDER_LOW) == 0x14 && inportb(ATA_CYLINDER_HIGH) == 0xeb) {
			ata_type = 2;
		} else {
			ata_type = 1;
		}
	} else {
		ata_type = 1;

		// Wait until DRQ set
		while (!((inportb(ATA_STATUS) >> 3) & 1));
	}

	// Read data
	for (int i = 0; i < 256; i++) {
		identify_block[i] = inportw(ATA_DATA);
	}

	// Set interrupts
	set_id(IRQ_OFFSET + 14, &ata_handler, 0x08, IDT_PROT_INTR, 0, 1);
	set_id(IRQ_OFFSET + 15, &ata_handler, 0x08, IDT_PROT_INTR, 0, 1);
}

// ATA interrupts
__attribute__((interrupt)) void ata_handler(interrupt_frame*) {
	ata_done_flag = 1;
	outportb(PIC_COMMAND2, PIC_EOI);
	outportb(PIC_COMMAND1, PIC_EOI);
}

// Wait while ATA is doing stuff
void ata_wait() {
	ata_done_flag = 0;
	while (!ata_done_flag);
}

// Read some sectors in PIO
uint8_t read_sectors(uint32_t lba, uint32_t lbs, uint16_t* dest) {
	if (ata_type == 1) {
		// Select master or slave
		outportb(ATA_DRIVE, 0xe0 | ((lba >> 24) & 0xf));

		// Send sectorcount
		outportb(ATA_SECTOR_COUNT, (uint8_t)(lbs & 0xff));

		// Send low 8 of lba
		outportb(ATA_SECTOR_NUMBER, (uint8_t)(lba & 0xff));

		// Send mid 8 of lba
		outportb(ATA_CYLINDER_LOW, (uint8_t)((lba >> 8) & 0xff));

		// Send high 8 of lba
		outportb(ATA_CYLINDER_HIGH, (uint8_t)((lba >> 16) & 0xff));

		// Send read command
		outportb(ATA_COMMAND, 0x20);

		// Read loop
		for (int i = 0; i < lbs; i++) {
			// Wait until the DRQ bit is set
			while (!((inportb(ATA_STATUS) >> 3) & 1));

			// Perform a transfer
			for (int j = 0; j < 1024; j++) {
				dest[j] = inportw(ATA_DATA);
			}
		}

		return 1;
	} else if (ata_type == 2) {
		// Status for ATA to not inport twice
		uint8_t ata_status;
		// The packet
		uint8_t atapi_packet[12];
		// The amount of words that is transferred in one moment
		uint32_t words_to_fetch = 0;
		// The current write destination for multireads
		uint16_t* dest_track = dest;

		// Select master
		outportb(ATA_DRIVE, 0);

		// Wait until ready
		while (!((inportb(ATA_STATUS) >> 6) & 1));

		// No DMA
		outportb(ATA_FEATURES, 0);

		// Misc st uff
		outportb(ATA_SECTOR_COUNT, 0);
		outportb(ATA_SECTOR_NUMBER, 0);

		// Stuff
		outportb(ATA_CYLINDER_LOW, (lbs * 2048) & 0xff);
		outportb(ATA_CYLINDER_HIGH, (lbs * 2048) >> 8);

		// Send packets
		// Send packet command
		outportb(ATA_COMMAND, 0xa0);

		while (!((inportb(ATA_STATUS) >> 3) & 1));

		// READ 12
		// Command byte
		atapi_packet[0] = 0xa8;
		// Byte 1 is reserved
		atapi_packet[1] = 0;
		// Bytes of LBA from greatest to least
		atapi_packet[2] = (lba >> 24) & 0xff;
		atapi_packet[3] = (lba >> 16) & 0xff;
		atapi_packet[4] = (lba >> 8) & 0xff;
		atapi_packet[5] = lba & 0xff;
		// Bytes of LBS from greatest to least
		atapi_packet[6] = (lbs >> 24) & 0xff;
		atapi_packet[7] = (lbs >> 16) & 0xff;
		atapi_packet[8] = (lbs >> 8) & 0xff;
		atapi_packet[9] = lbs & 0xff;
		// Rest is reserved
		atapi_packet[10] = 0;
		atapi_packet[11] = 0;

		// Send the actual command packet now
		for (int i = 0; i < 6; i++) {
			outportw(ATA_DATA, ((uint16_t*)atapi_packet)[i]);
		}

		// Wait for DRQ
		ata_wait();

		// Loop for getting data
		while (1) {
			// Once done waiting, get
			words_to_fetch = ((uint32_t)inportb(ATA_CYLINDER_HIGH) << 8) | (uint32_t)inportb(ATA_CYLINDER_LOW);
			words_to_fetch /= 2;

			// Loop get
			for (uint32_t i = 0; i < words_to_fetch; i++) {
				dest_track[i] = inportw(ATA_DATA);
			}

			dest_track += words_to_fetch;

			// Get status
			ata_status = inportb(ATA_STATUS);

			if ((ata_status >> 3) || (ata_status >> 7)) {
				break;
			}

			// Wait
			ata_wait();
		}

		return 1;
	}
}
