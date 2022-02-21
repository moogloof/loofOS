ENTRY(kernel_main)

SECTIONS {
	. = 0x100000;
	.text : {
		KEEP(*(.kernel))
		*(.text .text.*)
		. = ALIGN(4096);
	}

	.data : {
		*(.rodata .rodata.*)
		*(.data .data.*)
		. = ALIGN(4096);
	}

	.bss : {
		*(.bss .bss.*)
		. = ALIGN(4096);
	}
}
