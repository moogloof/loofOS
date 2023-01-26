[bits 32]
section .text
; _bios_ext_read_helper(output_address, lba_address)
global _bios_ext_read_helper
_bios_ext_read_helper:
	push ebp
	mov ebp, esp
	push esi
	push edx

	; Prepare for the read
	jmp 0x18:_bios_ext_read_helper_real
[bits 16]
_bios_ext_read_helper_real:
	mov ax, 0x20
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	lidt [bios_idtr]

	; Temporarily enter real mode
	mov eax, cr0
	and al, 0xfe
	mov cr0, eax

	jmp 0:_bios_ext_read_helper_int
_bios_ext_read_helper_int:
	mov ax, 0
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Do the read
	sti
	mov dl, byte [drive_number]
	lea si, [ebp + 8]
	mov ah, 0x42
	int 0x13

	; Reenter protected mode
	cli
	lgdt [gdtr]
	mov eax, cr0
	or al, 1
	mov cr0, eax

	jmp 0x8:_bios_ext_read_helper_prot
[bits 32]
_bios_ext_read_helper_prot:
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	pop edx
	pop esi
	pop ebp
	ret

bios_idtr:
	dw 0x3ff
	dd 0

extern drive_number
extern dap_block
extern dap_offset
extern dap_lba_low
extern dap_lba_high
extern dap_read_length
extern gdtr
