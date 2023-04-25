#include <stdint.h>
#include <core/idt.h>
#include <core/isr.h>
#include <mm/alloc.h>

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
} __attribute__((packed)) *idt; // IDT buffer

// The IDTR structure
struct idtr_desc {
	uint16_t size;
	uint32_t offset;
} __attribute__((packed)) idtr;

// Initialize IDT structures
void init_idt() {
	int idt_len = sizeof(struct idt_desc) * IDT_LIMIT;
	idt = kernel_allocate(idt_len);
	idtr = (struct idtr_desc){.size = idt_len - 1, .offset = (int32_t)(idt)};
}

// Add interrupt descriptor to the IDT
void set_id(uint8_t idt_pos, void (*offset)(interrupt_frame*), uint16_t selector, uint8_t type, uint8_t dpl, uint8_t present) {
	// Set the ID at position in the IDT
	idt[idt_pos] = (struct idt_desc){.offset1 = (uint32_t)offset & 0xffff,
			.selector = selector,
			.unused = 0,
			.type = type,
			.s = 0,
			.dpl = dpl,
			.present = present,
			.offset2 = ((uint32_t)offset >> 16) & 0xffff};
}

// Load the interrupt descriptor table
void load_idt() {
	// Load the idtr
	__asm__("lidt %0" : : "m"(idtr));
}
