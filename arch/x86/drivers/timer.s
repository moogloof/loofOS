section .text
global timer_handler_wrapper
timer_handler_wrapper:
	; Push for gen_register_set struct
	push eax
	push ecx
	push edx
	push ebx
	push ebp
	push esi
	push edi

	; Push for seg_register_set struct
	push gs
	push fs
	push es
	push ds

	; Push page directory
	mov eax, cr3
	push eax

	; Call the handler
	call timer_handler

	; Pop page directory
	pop eax
	mov cr3, eax

	; Pop all the stuff
	pop ds
	pop es
	pop fs
	pop gs
	pop edi
	pop esi
	pop ebp
	pop ebx
	pop edx
	pop ecx
	pop eax

	; IRET
	iret

; Extern stuff
extern dump_registers
extern timer_handler
