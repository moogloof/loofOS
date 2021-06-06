OUTPUT_FORMAT(binary)
ENTRY(kernel_main)

KERNEL_VMA = 0x100000;

SECTIONS {
	. = KERNEL_VMA;

	.text : AT(ADDR(.text) - KERNEL_VMA) {
		KEEP(*(.text.kernel))
		*(.text*)
		. = ALIGN(0x200);
	} =0x0000

	.data : AT(ADDR(.data) - KERNEL_VMA) {
		*(.data*)
		. = ALIGN(0x200);
	} =0x0000

	.bss : AT(ADDR(.bss) - KERNEL_VMA) {
		*(.bss*)
		. = ALIGN(0x200);
	} =0x0000
}
