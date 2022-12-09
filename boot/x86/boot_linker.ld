ENTRY(_start)

SECTIONS {
	. = 0x7c00;
	.text : {
		KEEP(*(.text.boot))
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
