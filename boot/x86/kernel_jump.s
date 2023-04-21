[bits 32]
section .text
global kernel_jump
kernel_jump:
	jmp 0x8:0xc0100000
