#ifndef BOOT_ATA_PIO
#define BOOT_ATA_PIO

#include <stdint.h>
#include <core/isr.h>

// Ports for the ATA bus
// The base of the ports
#define ATA_BASE 0x170
// Data port
#define ATA_DATA (ATA_BASE)
// Error register
#define ATA_ERROR (ATA_BASE + 1)
// Features register
#define ATA_FEATURES (ATA_BASE + 1)
// Sector count register
#define ATA_SECTOR_COUNT (ATA_BASE + 2)
// Sector number register, also the bottom third of LBA
#define ATA_SECTOR_NUMBER (ATA_BASE + 3)
// Cylinder low register, also the middle third of LBA
#define ATA_CYLINDER_LOW (ATA_BASE + 4)
// Cylinder high register, also the upper third of LBA
#define ATA_CYLINDER_HIGH (ATA_BASE + 5)
// The drive or head register
#define ATA_DRIVE (ATA_BASE + 6)
// The status register
#define ATA_STATUS (ATA_BASE + 7)
// The command register
#define ATA_COMMAND (ATA_BASE + 7)

// Init the ATA driver
void init_ata();

// ATA interrups
__attribute__((interrupt)) void ata_handler(interrupt_frame*);

// Wait while ATA is doing stuff
void ata_wait();

// Read some sectors
uint8_t read_sectors(uint32_t, uint32_t, uint16_t*);

#endif
