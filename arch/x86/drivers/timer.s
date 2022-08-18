section .text
global timer_handler_wrapper
timer_handler_wrapper:
	; Disable interrupts while handling
	cli

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

	; Call the handler
	call timer_handler

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

	; Enable interrupts after done
	sti

	; IRET
	iret

; Extern stuff
extern dump_registers
extern timer_handler
