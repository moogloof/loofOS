OUTPUT_FORMAT(binary)
ENTRY(kernel_main)

SECTIONS {
	.text : {
		. = 0x100000;
		KEEP(*(.text.kernel))
		*(.text*)
	} =0x0000

	.data : {
		*(.data*)
	} =0x0000

	.bss : {
		*(.bss*)
	} =0x0000
}
