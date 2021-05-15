[bits 16]
[org 0x7c00]

section .text.boot

_start:
	; Set CS
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

	; Send message boot
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
	; Push registers to stack
	pusha

	; Set background, foreground, and mode
	mov bh, 0
	mov bl, 0
	mov ah, 0x0e

	print_string_loop:
		; Load byte at the address in the SI register to AL register and increment SI
		lodsb

		; Check whether there is a terminating byte
		cmp al, 0
		je exit_print

		; Call teletype bios interrupt
		int 0x10

		jmp print_string_loop

	exit_print:
		; Pop previously saved registers from stack and return
		popa
		ret

; Messages
hello: db "LoofOS booting...", 0x0d, 0x0a, 0
a20_msg: db "Enabling A20 line", 0x0d, 0x0a, 0

times 510-($-$$) db 0
dw 0xaa55
