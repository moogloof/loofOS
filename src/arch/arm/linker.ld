ENTRY(_start)

SECTIONS {
	. = 0x8000;
	__text_start = .;
	.text : {
		*(.text)
	}
	. = ALIGN(4096);
	__text_end = .;

	__data_start = .;
	.data : {
		*(.data)
	}
	. = ALIGN(4096);
	__data_end = .;

	__bss_start = .;
	.bss : {
		*(.bss)
	}
	. = ALIGN(4096);
	__bss_end = .;
	__end = .;
}
