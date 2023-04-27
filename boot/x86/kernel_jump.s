[bits 32]
section .text
global kernel_jump
kernel_jump:
	; Set stack
	mov ebp, 0xd0000000
	mov esp, 0xd0000000
	jmp 0x8:0xc0100000
