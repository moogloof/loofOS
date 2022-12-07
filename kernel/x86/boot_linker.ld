ENTRY(_start)

SECTIONS {
	. = 0x7c00;
	.text : {
		KEEP(*(.text.boot))
		*(.text .text.*)
		. = ALIGN(2048);
	}

	.rodata : {
		*(.rodata .rodata.*)
		. = ALIGN(2048);
	}

	.data : {
		*(.data .data.*)
		. = ALIGN(2048);
	}

	.bss : {
		*(.bss .bss.*)
		. = ALIGN(2048);
	}

	.gdb_index : {
		KEEP(*(.gdb_index .gdb_index.*))
		. = ALIGN(2048);
	}
}
