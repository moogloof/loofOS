ENTRY(kernel_main)
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(x86-64)

SECTIONS {
	. = 0x80000000;
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
