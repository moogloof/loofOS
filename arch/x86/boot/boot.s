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
	; Reset drive
	; The drive ID is already loaded at the DL register
	; But load again anyway
	; Resetting drive message
	lea si, [reset_drive_msg]
	call print_string
	; Reset drive
	mov dl, [drive_number]
	mov ah, 0
	int 0x13

	; Load second stage bootloader
	; Loading second bootloader message
	lea si, [next_stage_msg]
	call print_string
	; Set ES:BX for loading
	mov ax, 0x7e0
	mov es, ax
	mov bx, 0
	; Load 2 sectors of next stage bootloader
	mov ax, [boot_length]
	shr ax, 9
	mov ah, 0x2
	mov ch, 0
	mov cl, [boot_location]
	inc cl
	mov dh, 0
	mov dl, [drive_number]
	int 0x13

	; Do some other stuff before moving to second bootloader
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

; The drive number for boot
drive_number: db 0
; Messages
hello: db "LoofOS booting...", 0x0d, 0x0a, 0
reset_drive_msg: db "Resetting drive...", 0x0d, 0x0a, 0
next_stage_msg: db "Loading next stage boot...", 0x0d, 0x0a, 0
a20_disable_msg: db "Disabling A20 line...", 0x0d, 0x0a, 0
a20_fail_msg: db "A20 line disabling failed.", 0x0d, 0x0a, 0
a20_success_msg: db "A20 line disabled.", 0x0d, 0x0a, 0

times 510-($-$$) db 0
dw 0xaa55

; ----BOOT2----
next_boot:
	; Success load message
	lea si, [load_success_msg]
	call print_string

; Load the gdt
load_gdt:
	lea si, [gdt_load_msg]
	call print_string
	cli
	lgdt [gdtr]
	sti

; Enter protected mode and then stage2 boot
protected_mode:
	; Disable interrupts
	lea si, [interrupt_disable_msg]
	call print_string
	lea si, [pmode_enable_msg]
	call print_string
	lea si, [stage2_jump_msg]
	call print_string
	cli

	; Enable protected mode by setting protected mode enable bit
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	; Jump to set CS
	jmp (gdt_code_kernel - gdt_start):stage2_jmp

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
