[bits 32]
section .text
global switch_context
switch_context:
	add esp, 4
	; Get the needed registers from the stack
	mov ebp, esp
	; The frame
;	push dword [ebp + 68]
;	push dword [ebp + 64]
;	push dword [ebp + 60]
;	push dword [ebp + 56]
;	push dword [ebp + 52]

	; The general registers
;	push dword [ebp + 48]
;	push dword [ebp + 44]
;	push dword [ebp + 40]
;	push dword [ebp + 36]
;	push dword [ebp + 32]
;	push dword [ebp + 28]
;	push dword [ebp + 24]
;	push dword [ebp + 20]

	; The segments
;	push dword [ebp + 16]
;	push dword [ebp + 12]
;	push dword [ebp + 8]
;	push dword [ebp + 4]

	; Notify of finished interrupt
	mov al, 0x20
	out 0xc0000020, al

	; Change page directory
	mov eax, [ebp + 68]
	mov cr3, eax

	; Pop segments
	pop ds
	pop es
	pop fs
	pop gs

	; Pop general registers
	pop edi
	pop esi
	pop ebp
	pop ebx
	pop edx
	pop ecx
	pop eax

	cmp dword [esp], 0
	jne switch_to_kernel
switch_to_user:
	add esp, 4
	jmp switch_end
switch_to_kernel:
	pop esp
switch_end:
	iret
