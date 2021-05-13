[bits 16]
[org 0x7c00]

section .text.boot

_start:
	jmp 0:next
next:
	; Setup segment registers
	mov ax, 0
	mov ss, ax
	mov ds, ax
	mov es, ax

	; Setup stack
	mov sp, 0x9000
	mov bp, sp

	lea si, [hello]
	call print_string

	lea si, [load_msg]
	call print_string

	jmp halt

; Halting
halt:
	hlt
	jmp halt

; Print string
print_string:
	pusha

	mov bh, 0
	mov bl, 0
	mov ah, 0x0e

	print_string_loop:
		lodsb

		cmp al, 0
		je exit_print

		int 0x10

		jmp print_string_loop

	exit_print:
		popa
		ret

hello: db "LoofOS booting...", 0x0d, 0x0a, 0
load_msg: db "Loading kernel into memory at 1MiB...", 0x0d, 0x0a, 0

times 510-($-$$) db 0
dw 0xaa55
