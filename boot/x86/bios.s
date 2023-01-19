[bits 16]
section .text
global _bios_ext_read_helper
_bios_ext_read_helper:
	push bp
	mov bp, sp
	push si
	push dx

	; BIOS params
	mov dl, byte [drive_number]
	lea si, [dap_block]
	; Read in output address
	mov ax, word [bp + 8]
	mov word [dap_offset], ax
	; Read in the lba offset
	mov eax, [bp + 12]
	mov [dap_lba_low], eax
	mov eax, [bp + 16]
	mov [dap_lba_high], eax
	; Read in the lba read length
	mov ax, word [bp + 20]
	mov word [dap_read_length], ax

	; Do the read
	mov ah, 0x42
	int 0x13

	pop dx
	pop si
	pop bp
	ret

global prot_memcpy
prot_memcpy:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push ecx

	; Copy loop
	mov ecx, [ebp + 16]
	mov edi, [ebp + 12]
	mov esi, [ebp + 8]
	copy_loop:
		cmp ecx, 0
		je copy_done

		mov al, byte [ds:esi]
		mov byte [es:edi], al
		inc esi
		inc edi
		jmp copy_loop
	copy_done:

	pop ecx
	pop edi
	pop esi
	pop ebp
	ret

extern drive_number
extern dap_block
extern dap_offset
extern dap_lba_low
extern dap_lba_high
extern dap_read_length
