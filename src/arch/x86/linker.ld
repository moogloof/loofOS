OUTPUT_FORMAT(binary)
ENTRY(_start)

SECTIONS {
	.text : ALIGN(0x1000) {
		KEEP(*(.text.boot))
		*(.text)
	}
	.data : ALIGN(0x1000) {
		*(.data)
	}
	.bss : ALIGN(0x1000) {
		_sbss = .;
		*(.bss)
	}
	_ebss = .;
}
