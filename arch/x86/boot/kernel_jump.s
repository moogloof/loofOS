section .text
global jump_to_kernel
jump_to_kernel:
	; Setup kernel stack
	mov esp, 0x10000000
	add esp, 0xc0000000
	; DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP DO THE JUMP 
	jmp 0x8:0xc0100000
