; ----BOOT0----
[bits 16]
[org 0x7c00]

section .text.boot

_start:
	; Set CS
	jmp 0:next
next:
	; Setup segment registers for bootloader
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

	; Loading stage 1 boot
	; Reset drive
	; The drive ID is already loaded at the DL register
	; Resetting drive message
	lea si, [reset_drive_msg]
	call print_string
	; Reset drive
	mov ah, 0
	int 0x13

	; Load second stage bootloader
	; Loading second bootloader message
	lea si, [next_stage_msg]
	call print_string
	; Set ES:BS for loading
	mov ax, 0x7e0
	mov es, ax
	mov bx, 0
	; Load 2 sectors of next stage bootloader
	mov ah, 0x2
	mov al, 2
	mov ch, 0
	mov cl, 2
	mov dh, 0
	int 0x13

	; Jump to second bootloader
	jmp next_boot

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
reset_drive_msg: db "Resetting drive...", 0x0d, 0x0a, 0
next_stage_msg: db "Loading next stage boot...", 0x0d, 0x0a, 0

times 510-($-$$) db 0
dw 0xaa55

; ----BOOT1----
next_boot:
	; Success load message
	lea si, [load_success_msg]
	call print_string

; Messages
load_success_msg: db "Next stage boot successfully loaded.", 0x0d, 0x0a, 0
