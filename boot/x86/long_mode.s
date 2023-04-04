[bits 32]
section .text
; Long mode transition
global long_mode
long_mode:
	; Jump to the kernel
	jmp 0x80000000
