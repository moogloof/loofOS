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
; Enter huge unreal mode
; So that we can load the kernel above the 16 bit limit
unreal_mode:
	; Switch to pmode for cache
	mov eax, cr0
	or al, 1
	mov cr0, eax
	; Save DS segment register before
	push ds
	; Load the gdt data into segment register, which will save it to cache
	mov bx, 0x10
	mov ds, bx
	mov es, bx
	; Swtich back to real mode, now in unreal mode
	and al, 0xfe
	mov cr0, eax
	; Restore DS segment register
	pop ds
	; Enable interrupts
	sti
; Load the kernel at 0x100000
load_kernel:
	lea si, [kernel_load_msg]
	call print_string

	; Set ES:BX for loading at 0x9200 initially
	mov ax, 0x920
	mov es, ax

	mov edi, 0x100000

	; Load the kernel
	load_kernel_loop:
		; Load a sector into 0x9200
		mov ah, 0x2
		mov al, 1
		mov ch, 0 ; Cylinder
		mov cl, 6 ; Sector
		add cl, byte [kernel_load_offset]
		mov dh, 0 ; Head
		mov bx, 0x0 ; ES:BX sector destination

		; Modulo the loaded sector by 18
		; 18 sectors per head in a 1.44 MB thingy
		; 80 heads per cylinder
		; 2 cylinders
		mod_loop:
			cmp cl, 18
			jg inc_cylinder
			jmp mod_loop2

			inc_cylinder:
				inc dh
				sub cl, 18
		mod_loop2:
			cmp ch, 79
			jg inc_head
			jmp load_sector_int

			inc_head:
				inc ch
				sub dh, 80

		load_sector_int:
		; Load sector BIOS interrupt
		int 0x13

		; Update sector position to load
		inc word [kernel_load_offset]
		; Update sectors to load
		dec word [kernel_size]

		; Relocate kernel sector to 0x100000 + pos
		push ds
		push es
		; Set ES and DS for memory access
		mov ax, 0x0
		mov ds, ax
		mov es, ax
		; Setup registers for copying bytes
		mov ecx, 128
		mov esi, 0x9200
		; Copy over bytes
		cld
		a32 rep movsd

		pop es
		pop ds
		; Check if there are sectors left to load
		cmp word [kernel_size], 0
		jne load_kernel_loop
; Set the VGA 80x25 video text mode
vga_mode_set:
	mov ah, 0
	mov al, 0x3
	int 0x10
; Enter protected mode and then kernel
protected_mode:
	; Disable interrupts
	lea si, [interrupt_disable_msg]
	call print_string
	lea si, [pmode_enable_msg]
	call print_string
	lea si, [kernel_jump_msg]
	call print_string
	cli

	; Enable protected mode by setting protected mode enable bit
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	; Jump to set CS
	jmp (gdt_code_kernel - gdt_start):kernel_jmp

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
a20_disable_msg: db "Disabling A20 line...", 0x0d, 0x0a, 0
a20_fail_msg: db "A20 line disabling failed.", 0x0d, 0x0a, 0
a20_success_msg: db "A20 line disabled.", 0x0d, 0x0a, 0
gdt_load_msg: db "Loading the GDT...", 0x0d, 0x0a, 0
kernel_load_msg: db "Loading the kernel...", 0x0d, 0x0a, 0
interrupt_disable_msg: db "Disabling interrupts...", 0x0d, 0x0a, 0
pmode_enable_msg: db "Enabling protected mode...", 0x0d, 0x0a, 0
kernel_jump_msg: db "Jumping to the kernel...", 0x0d, 0x0a, 0
; Size of the kernel to load
kernel_size: dw 0xb40
; Position of kernel load offset
kernel_load_offset: dw 0

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
	; TSS section
	gdt_tss:
		dw 0x6c
		dw tss_block
		db 0
		db 0x89
		db 0100_0000b
		db 0
gdt_end:

tss_block:
	dd 0 ; Link
	dd 0x8000000 ; ESP0
	dd 0x10 ; SS0
	dd 0 ; ESP1
	dd 0 ; SS1
	dd 0 ; ESP2
	dd 0 ; SS2
	dd 0 ; CR3
	dd 0 ; EIP
	dd 0 ; EFLAGS
	dd 0 ; EAX
	dd 0 ; ECX
	dd 0 ; EDX
	dd 0 ; EBX
	dd 0 ; ESP
	dd 0 ; EBP
	dd 0 ; ESI
	dd 0 ; EDI
	dd 0 ; ES
	dd 0 ; CS
	dd 0 ; SS
	dd 0 ; DS
	dd 0 ; FS
	dd 0 ; GS
	dd 0 ; LDTR
	dw 0
	dw 0 ; IOPB
	dd 0 ; SSP

[bits 32]
; Jump to the kernel
kernel_jmp:
	; Setup for protected mode
	; Update segment registers
	mov ax, (gdt_data_kernel - gdt_start)
	mov ds, ax
	mov es, ax
	mov ss, ax
	; Set new stack
	mov esp, 0x10000000
	sub esp, 0x4
	mov ebp, esp
	; Load TSS
	mov ax, (gdt_tss - gdt_start)
	ltr ax

	jmp 0x100000
