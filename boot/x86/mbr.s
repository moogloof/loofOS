; ----BOOT1----
[bits 16]
;[org 0x7c00]

section .text.boot

global _start
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
	mov sp, 0x7c00
	mov bp, sp

	; Save the drive number stored at DL
	mov [drive_number], dl

	; Send message boot
	lea si, [hello]
	call print_string

	; Get the sector size
	mov ah, 0x48
	mov dl, [drive_number]
	lea si, [drive_parameters]
	int 0x13
	; Check whether info read succeeded
	jnc read_info_success
	; If info read failed, then halt
	lea si, [read_info_fail_msg]
	call print_string
	jmp halt

; Info read succeeded
read_info_success:
	; Load second stage bootloader using extended read
	; Loading second bootloader message
	lea si, [next_stage_msg]
	call print_string
	; Set number of sectors to read
	mov ax, 63
	; Use the number of bytes in a sector
	; The algorithm assumes that the number of bytes in a sector is a power of 2
	; Set cx to sector size
	mov cx, [drive_bytes_per_sector]
	; Loop
	sector_size_loop:
		; Check if size is reduced
		cmp cx, 512
		je sector_size_done
		shr ax, 1
		shr cx, 1
		jmp sector_size_loop
	; Loop finished
	sector_size_done:
		inc ax
	mov [dap_read_length], ax
	; Set segment:offset for loading
	mov ax, 0
	mov [dap_segment], ax
	mov ax, 0x7e00
	mov [dap_offset], ax
	; Set the low and high parts of the LBA for the drive
	mov ax, 1
	mov [dap_lba_low], ax
	; Set the drive number
	mov dl, [drive_number]
	; Do read
	lea si, [dap_block]
	mov ah, 0x42
	int 0x13
	; Check if read worked
	jnc stage2_load_success
	; If read not worked, then halt
	lea si, [stage2_load_fail_msg]
	jmp halt

; Read worked
stage2_load_success:
	lea si, [stage2_load_success_msg]
	call print_string
	; Jump to second bootloader
	jmp stage2

; Halting
halt:
	hlt
	jmp halt

; Print string
print_string:
	; Push registers to stack
	pusha
	pushf

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
		popf
		popa
		ret

; The drive number for boot
drive_number: db 0
; Messages
hello: db "LoofOS booting...", 0x0d, 0x0a, 0
read_info_fail_msg: db "Reading drive info failed. Halting.", 0x0d, 0x0a, 0
next_stage_msg: db "Loading next stage boot...", 0x0d, 0x0a, 0
stage2_load_fail_msg: db "Failed to load stage2 boot. Halting.", 0x0d, 0x0a, 0
stage2_load_success_msg: db "Loaded stage2 boot. Jumping.", 0x0d, 0x0a, 0

; The read sectors from drive with extended read block
dap_block:
	dap_block_size: db 0x10
	db 0
	dap_read_length: dw 0
	dap_offset: dw 0
	dap_segment: dw 0
	dap_lba_low: dd 0
	dap_lba_high: dw 0
	dw 0

; The result buffer of the drive parameters
drive_parameters:
	drive_parameters_size: dw 0x1e
	dw 0
	dd 0
	dd 0
	dd 0
	dd 0
	dd 0
	drive_bytes_per_sector: dw 0
	dd 0

times 440-($-$$) db 0
; Some mbr stuff
dd 0
dw 0
; The first partition
db 0x80
dw 0
db 0
db 7
dw 0
db 0
dd 0
dd 0x100
; The second
db 0x80
dw 0
db 0
db 0x96
dw 0
db 0
dd 0x100
dd 0xffffffff
times 8 dd 0
dw 0xaa55

extern stage2
