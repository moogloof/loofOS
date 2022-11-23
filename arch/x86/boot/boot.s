; ----BOOT1----
[bits 16]
;[org 0x7c00]

section .text.boot

_start:
	; Set CS
	jmp 0:next
; Pad to 0x8 offset
times 8-($-$$) db 0
; El Torito block
; The primary volume descriptor LBA
primary_volume_descriptor: dd 0
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
	jmp next_boot

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
a20_disable_msg: db "Disabling A20 line...", 0x0d, 0x0a, 0
a20_fail_msg: db "A20 line disabling failed.", 0x0d, 0x0a, 0
a20_success_msg: db "A20 line disabled.", 0x0d, 0x0a, 0

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

times 510-($-$$) db 0
dw 0xaa55

; ----BOOT1.5----
next_boot:
	; Success load message
	lea si, [load_success_msg]
	call print_string

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
	sti

; Set VBE modes
vbe_set_display:
	; Get VBE Controller Info
	mov ax, 0x4f00
	lea di, [vbe_info_block]
	; Write signature
	mov cl, 'V'
	mov [vbe_info_block], cl
	mov cl, 'B'
	mov [vbe_info_block + 1], cl
	mov cl, 'E'
	mov [vbe_info_block + 2], cl
	mov cl, '2'
	mov [vbe_info_block + 3], cl
	; Write version
	mov cx, 0x300
	mov [vbe_info_block + 4], cx
	int 0x10

	; Browse the available modes
	; We specifically want a 640x480 24 bit mode
	; ^ Ignore above, we want whatever we can get
	; Initially, the selected width and height are 0s
	mov ax, 0
	mov [selected_width], ax
	mov [selected_height], ax
	mov [selected_vbe_mode], ax
	; Initial mode
	mov si, [vbe_info_block + 14]
	vbe_search_mode:
		; Presets for vbe function calls
		mov cx, [si]
		mov ax, 0x4f01
		lea di, [mode_info_block]
		; Check if it's the end of the modes
		cmp cx, 0xffff
		je vbe_choose_mode
		; Get the info block for the mode
		int 0x10
		; Check if depth matches
		mov al, [mode_info_block + 25]
		cmp al, 24
		jne vbe_try_again
		; Get the highest resolution below 1920
		mov ax, [mode_info_block + 18]
		cmp ax, [selected_width]
		jbe vbe_try_again
		cmp ax, 1920
		ja vbe_try_again
		mov [selected_width], ax
		mov ax, [mode_info_block + 20]
		mov [selected_height], ax
		mov [selected_vbe_mode], cx
		; If matched, then return with the thing
		; ^ Ignore above, try again
		;jmp vbe_choose_mode
	vbe_try_again:
		; Increment and retry
		add si, 2
		jmp vbe_search_mode

	vbe_failed:
		; TODO still jump but with different graphics
		jmp halt

	vbe_choose_mode:
		; Choose the selected mode
		mov ax, 0x4f02
		mov bx, [selected_vbe_mode]
		mov di, 0x600
		int 0x10

		jmp halt

; Enter protected mode and then stage2 boot
protected_mode:
	cli

	; Enable protected mode by setting protected mode enable bit
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	; Set all segment registers
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; Jump to set CS
	jmp (gdt_code_kernel - gdt_start):stage2_boot

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
load_success_msg: db "Next stage boot successfully loaded.", 0x0d, 0x0a, 0
gdt_load_msg: db "Loading the GDT...", 0x0d, 0x0a, 0
interrupt_disable_msg: db "Disabling interrupts...", 0x0d, 0x0a, 0
pmode_enable_msg: db "Enabling protected mode...", 0x0d, 0x0a, 0
stage2_jump_msg: db "Jumping to the stage2 boot...", 0x0d, 0x0a, 0

; GDTR
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
	; User code descriptor
	gdt_code_user:
		dw 0xffff
		dw 0
		db 0
		db 1111_1010b
		db 1100_1111b
		db 0
	; User data descriptor
	gdt_data_user:
		dw 0xffff
		dw 0
		db 0
		db 1111_0010b
		db 1100_1111b
		db 0
gdt_end:

; All externs
extern stage2_boot
extern vbe_info_block
extern mode_info_block
extern selected_width
extern selected_height
extern selected_vbe_mode
