#include <stdint.h>
#include <core/idt.h>

// The interrupt descriptor structure
struct idt_desc {
	uint16_t offset1;
	uint16_t selector;
	uint8_t unused;
	uint8_t type:4;
	uint8_t s:1;
	uint8_t dpl:2;
	uint8_t present:1;
	uint16_t offset2;
} __attribute__((packed)) idt[IDT_LIMIT]; // IDT buffer

// Add interrupt descriptor to the IDT
void set_id(uint8_t idt_pos, uint32_t offset, uint16_t selector, uint8_t type, uint8_t dpl, uint8_t present) {
	// Set the ID at position in the IDT
	idt[idt_pos] = (struct idt_desc){.offset1 = offset & 0xffff,
			.selector = selector,
			.unused = 0,
			.type = type,
			.s = 0,
			.dpl = dpl,
			.present = present,
			.offset2 = (offset >> 16) & 0xffff};
}

// Load the interrupt descriptor table
void load_idt() {
	// The IDTR structure
	struct {
		uint16_t size;
		uint32_t offset;
	} __attribute__((packed)) idtr = {.size = sizeof(idt) - 1, .offset = &idt};

	// Load the idtr
	__asm__("lidt %0" : : "m"(idtr));
}
