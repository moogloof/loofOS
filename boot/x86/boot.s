; ----BOOT1----
[bits 16]
;[org 0x7c00]

section .text.boot

global _start
_start:
	; Set CS
	jmp 0:next
; Pad to 0x8 offset
times 8-($-$$) db 0
; El Torito block
; The primary volume descriptor LBA
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
	mov [dap_read_length], ax
	; Set segment:offset for loading
	mov ax, 0
	mov [dap_segment], ax
	mov ax, 0x7c00
	add ax, [drive_bytes_per_sector]
	mov [dap_offset], ax
	; Set the low and high parts of the LBA for the drive
	mov ax, [boot_location]
	inc ax
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
	jmp stage2_setup

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
global drive_number
drive_number: db 0
; Messages
hello: db "LoofOS booting...", 0x0d, 0x0a, 0
read_info_fail_msg: db "Reading drive info failed. Halting.", 0x0d, 0x0a, 0
next_stage_msg: db "Loading next stage boot...", 0x0d, 0x0a, 0
stage2_load_fail_msg: db "Failed to load stage2 boot. Halting.", 0x0d, 0x0a, 0
stage2_load_success_msg: db "Loaded stage2 boot. Jumping.", 0x0d, 0x0a, 0

; The read sectors from drive with extended read block
global dap_block
dap_block:
	dap_block_size: db 0x10
	db 0
	global dap_read_length
	dap_read_length: dw 0
	global dap_offset
	dap_offset: dw 0
	global dap_segment
	dap_segment: dw 0
	global dap_lba_low
	dap_lba_low: dd 0
	global dap_lba_high
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
	global drive_bytes_per_sector
	drive_bytes_per_sector: dw 0
	dd 0

times 510-($-$$) db 0
dw 0xaa55

stage2_setup:
	; A20 disable
	; Send A20 disable message
	lea si, [a20_disable_msg]
	call print_string
	; Disable A20
	; Check A20
	call a20_test
	cmp ax, 0
	je a20_0
	jmp a20_success
	; If disabled, try to enable
	; Int 15h method
a20_0:
	mov ax, 0x2403
	int 0x15
	call a20_test

	; Check A20
	call a20_test
	cmp ax, 0
	je a20_1
	jmp a20_success
a20_1:
	mov ax, 0x2402
	int 0x15
	call a20_test

	; Check A20
	call a20_test
	cmp ax, 0
	je a20_2
	jmp a20_success
a20_2:
	mov ax, 0x2401
	int 0x15
	call a20_test

	; Check A20
	call a20_test
	cmp ax, 0
	je a20_fail
	jmp a20_success
; The A20 failed to enable
a20_fail:
	lea si, [a20_fail_msg]
	call print_string
	jmp halt
; The A20 succeeded to enable
a20_success:
	lea si, [a20_success_msg]
	call print_string

; Load the gdt
load_gdt:
	lea si, [gdt_load_msg]
	call print_string
	cli
	lgdt [gdtr]
protected_mode:
	; Switch to protected mode
	mov eax, cr0
	or al, 1
	mov cr0, eax

	; Jump to the stage2 main
	jmp 0x8:protected_mode_jmp
[bits 32]
protected_mode_jmp:
	; Set environment vars
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	jmp stage2

[bits 16]
; Test if A20 line is enabled
a20_test:
	; Push used registers
	pushf
	push ds
	push es
	push di
	push si

	; Setup addresses
	; Lower
	mov ax, 0
	mov es, ax
	mov di, 0x7dfe
	; Upper
	mov ax, 0xffff
	mov ds, ax
	mov si, 0xfe0e

	; Save data at addresses
	mov ax, word [es:di]
	push ax
	mov ax, word [ds:si]
	push ax

	; Change two addresses
	mov word [es:di], 0
	mov word [ds:si], 0x4e69

	; Test
	cmp word [es:di], 0x4e69

	; Cleanup memory
	pop ax
	mov word [ds:si], ax
	pop ax
	mov word [es:di], ax

	; If equal, branch to handler
	mov ax, 0
	je a20_test_exit
	mov ax, 1

	; Cleanup and return
	a20_test_exit:
		pop si
		pop di
		pop es
		pop ds
		popf
		ret

; Messages
a20_disable_msg: db "Disabling A20 line...", 0x0d, 0x0a, 0
a20_fail_msg: db "A20 line disabling failed.", 0x0d, 0x0a, 0
a20_success_msg: db "A20 line disabled.", 0x0d, 0x0a, 0
gdt_load_msg: db "Loading the GDT...", 0x0d, 0x0a, 0

; GDTR
global gdtr
gdtr:
	dw gdt_end - gdt_start - 1
	dd gdt_start

; GDT
; Flat memory model
gdt_start:
	; Null descriptor.
	gdt_null:
		dd 0
		dd 0
	; Kernel code descriptor
	gdt_code_kernel:
		dw 0xffff
		dw 0
		db 0
		db 1001_1010b
		db 1100_1111b
		db 0
	; Kernel data descriptor
	gdt_data_kernel:
		dw 0xffff
		dw 0
		db 0
		db 1001_0010b
		db 1100_1111b
		db 0
	; Kernel code descriptor
	gdt_code_real:
		dw 0xffff
		dw 0
		db 0
		db 1001_1010b
		db 1000_1111b
		db 0
	; Kernel data descriptor
	gdt_data_real:
		dw 0xffff
		dw 0
		db 0
		db 1001_0010b
		db 1000_1111b
		db 0
gdt_end:

extern stage2
