[bits 32]
section .text
; GDT segment load
global load_gdt_segments
load_gdt_segments:
	; Load data segments
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov fs, ax
	mov gs, ax

	; Load code segment
	jmp 0x8:cs_load
cs_load:
	ret
