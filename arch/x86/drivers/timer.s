global timer_handler_wrapper
timer_handler_wrapper:
	; Push for gen_register_set struct
	pusha

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
	popa

	; IRET
	iret

; Extern stuff
extern dump_registers
extern timer_handler
