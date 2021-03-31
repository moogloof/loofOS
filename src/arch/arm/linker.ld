OUTPUT_FORMAT(elf32-littlearm)
ENTRY(_start)

SECTIONS {
	. = 0x8000;

	.text : ALIGN(0x1000) {
		KEEP(*(.text.boot))
		*(.text)
	}

	.data : ALIGN(0x1000) {
		*(.data)
	}

	.rodata : ALIGN(0x1000) {
		*(.rodata)
	}

	.bss : ALIGN(0x1000) {
		_sbss = .;
		*(COMMON)
		*(.bss)
	}
	_ebss = .;

	STACK_BASE = .;
	. += 0x10000;
	STACK_TOP = .;
}
