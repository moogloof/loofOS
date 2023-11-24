ENTRY(main)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386)

SECTIONS {
	. = 0;
	.text : {
		*(.text .text.*)
		. = ALIGN(512);
	}

	.rodata : {
		*(.rodata .rodata.*)
		. = ALIGN(512);
	}

	.data : {
		*(.data .data.*)
		. = ALIGN(512);
	}

	.bss : {
		*(.bss .bss.*)
		. = ALIGN(512);
	}

	.gdb_index : {
		KEEP(*(.gdb_index .gdb_index.*))
		. = ALIGN(512);
	}
}
