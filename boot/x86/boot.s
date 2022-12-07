; ----BOOT1----
[bits 16]
[org 0x7c00]

section .text.boot

_start:
	; Set CS
	jmp 0:next
; Pad to 0x8 offset
times 8-($-$$) db 0
; El Torito block
; The primary volume descriptor LBA
global primary_volume_descriptor_position
primary_volume_descriptor_position: dd 0
; The location of the boot file LBA
boot_location: dd 0
; The length of the boot file in bytes
boot_length: dd 0
; A little checksum
torito_checksum: dd 0
; Some reserved stuff
times 40 db 0

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

	; Loading stage 2 boot
	; Bruh why did we do this anyway
	; Reset drive
	; The drive ID is already loaded at the DL register
	; But load again anyway
	; Resetting drive message
	;lea si, [reset_drive_msg]
	;call print_string
	; Reset drive
	;mov dl, [drive_number]
	;mov ah, 0
	;int 0x13

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
	mov ax, [boot_length]
	; Use the number of bytes in a sector
	; The algorithm assumes that the number of bytes in a sector is a power of 2
	; Set cx to sector size
	mov cx, [drive_bytes_per_sector]
	; Loop
	sector_size_loop:
		; Check if size is reduced
		cmp cx, 1
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
	mov ax, 0x7c00
	mov [dap_offset], ax
	; Set the low and high parts of the LBA for the drive
	mov ax, [boot_location]
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
	; Jump to second bootloader
	jmp halt

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
times 16 dd 0
dw 0xaa55
