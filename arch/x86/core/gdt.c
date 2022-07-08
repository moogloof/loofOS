#include <stdint.h>
#include <core/gdt.h>

// The segment descriptor structure
struct seg_desc {
	uint16_t limit1; // Low limit
	uint32_t base1:24; // Low base
	uint8_t a:1; // Accessed bit
	uint8_t rw:1; // Read write bit; 0 == readable, 1 == writable
	uint8_t dc:1; // Expansion bit/conforming bit; 0 == expand up/code must be of dpl ring, 1 == expand down/code can be lower priv levels
	uint8_t e:1; // Executable bit; 0 == not executable, 1 == executable
	uint8_t s:1; // Descriptor type; 0 == system, 1 == code/data
	uint8_t dpl:2; // Descriptor privilege level bit
	uint8_t p:1; // Present bit
	uint8_t limit2:4; // High limit
	uint8_t zero:1; // Reserved
	uint8_t l:1; // 64-bit code segment
	uint8_t db:1; // Operating size bit; 0 == 16-bit, 1 == 32-bit
	uint8_t g:1; // Granularity; 0 == byte, 1 == 4kib
	uint8_t base2; // High base
} __attribute__((packed)) *gdt; // GDT buffer

// The GDTR structure
struct gdtr_desc {
	uint16_t size;
	uint32_t offset;
} __attribute__((packed)) gdtr;

// The TSS block
struct tss_block {
	uint16_t link;
	uint16_t zero1;
	uint32_t esp0;
	uint16_t ss0;
	uint16_t zero2;
	uint32_t esp1;
	uint16_t ss1;
	uint16_t zero3;
	uint32_t esp2;
	uint16_t ss2;
	uint16_t zero4;
	uint32_t cr3;
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint16_t es;
	uint16_t zero5;
	uint16_t cs;
	uint16_t zero6;
	uint16_t ss;
	uint16_t zero7;
	uint16_t ds;
	uint16_t zero8;
	uint16_t fs;
	uint16_t zero9;
	uint16_t gs;
	uint16_t zero10;
	uint16_t ldtr;
	uint32_t zero11;
	uint16_t iopb;
	uint32_t ssp;
} __attribute__((packed)) *tss;

// Initialize GDT structures
void init_gdt() {
	int gdt_len = sizeof(struct seg_desc) * GDT_LIMIT;
	gdt = kernel_allocate(gdt_len);
	gdtr = (struct gdtr_desc){.size = gdt_len - 1, .offset = (uint32_t)(gdt)};

	// Add the required GDT structures
	// Set null descriptor
	gdt[0] = (struct seg_desc){0};
	// Set kernel code
	gdt[1] = (struct seg_desc){.limit1 = 0xffff, .base1 = 0, .a = 0, .rw = 1, .dc = 0, .e = 1, .s = 1, .dpl = 0, .p = 1, .limit2 = 0xf, .zero = 0, .l = 0, .db = 1, .g = 1, .base2 = 0};
	// Set kernel data
	gdt[2] = (struct seg_desc){.limit1 = 0xffff, .base1 = 0, .a = 0, .rw = 1, .dc = 0, .e = 0, .s = 1, .dpl = 0, .p = 1, .limit2 = 0xf, .zero = 0, .l = 0, .db = 1, .g = 1, .base2 = 0};
	// Set user code
	gdt[3] = (struct seg_desc){.limit1 = 0xffff, .base1 = 0, .a = 0, .rw = 1, .dc = 0, .e = 1, .s = 1, .dpl = 3, .p = 1, .limit2 = 0xf, .zero = 0, .l = 0, .db = 1, .g = 1, .base2 = 0};
	// Set user data
	gdt[4] = (struct seg_desc){.limit1 = 0xffff, .base1 = 0, .a = 0, .rw = 1, .dc = 0, .e = 0, .s = 1, .dpl = 3, .p = 1, .limit2 = 0xf, .zero = 0, .l = 0, .db = 1, .g = 1, .base2 = 0};

	// Set TSS block
	tss = kernel_allocate(sizeof(struct tss_block));
	tss->ss0 = 0x10;
	tss->esp0 = 0xc8000000;
	// Set the TSS gdt segment (the access byte is of a different form)
	gdt[5] = (struct seg_desc){.limit1 = sizeof(struct tss_block), .base1 = (uint32_t)tss & 0xffffff, .a = 1, .rw = 0, .dc = 0, .e = 1, .s = 0, .dpl = 0, .p = 1, .limit2 = 0, .zero = 0, .l = 0, .db = 1, .g = 0, .base2 = (uint32_t)tss >> 24};
}

// Load the global descriptor table
void load_gdt() {
	// Load the gdtr
	__asm__("lgdt %0" : : "m"(gdtr));
}
