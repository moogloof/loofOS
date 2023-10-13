[bits 32]
section .text
; _bios_int(registers, code)
global _bios_int
_bios_int:
	push ebp
	mov ebp, esp
	push eax
	push ebx
	push ecx
	push edx
	push esi
	push edi

	; Prepare for the interrupt
	jmp 0x18:_bios_int_trans
[bits 16]
_bios_int_trans:
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

	jmp 0:_bios_int_real
_bios_int_real:
	mov ax, 0
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	; Load registers
	mov al, [ebp + 32]
	mov [_bios_int_number], al
	mov eax, [ebp + 8]
	mov ebx, [ebp + 12]
	mov ecx, [ebp + 16]
	mov edx, [ebp + 20]
	mov esi, [ebp + 24]
	mov edi, [ebp + 28]

	; Do the interrupt
	sti

	db 0xcd
	_bios_int_number: db 0

	; Reenter protected mode
	cli
	lgdt [gdtr]
	mov eax, cr0
	or al, 1
	mov cr0, eax

	jmp 0x8:_bios_int_prot
[bits 32]
_bios_int_prot:
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	pop edi
	pop esi
	pop edx
	pop ecx
	pop ebx
	pop eax
	pop ebp
	ret

bios_idtr:
	dw 0x3ff
	dd 0

extern gdtr
