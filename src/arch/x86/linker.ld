OUTPUT_FORMAT(binary)
ENTRY(halt)

SECTIONS {
	.text : ALIGN(0x1000) {
		KEEP(*(.text.boot))
		*(.text)
	}
}
