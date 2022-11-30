section .text
global jump_to_kernel
jump_to_kernel:
	jmp 0x8:0xc0100000
