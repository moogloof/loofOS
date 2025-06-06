[bits 32]
section .text
global kernel_jump
kernel_jump:
	; Set stack
	mov ebp, 0xcffffffc
	mov esp, 0xcffffffc
	jmp 0x8:0xc0100000
