.section .text.boot
.global halt
halt:
	jmp halt

.fill 510-(.-halt), 1, 0
.word 0xaa55
