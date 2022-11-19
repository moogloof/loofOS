%macro START_ISR 0
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
%endmacro

%macro END_ISR 0
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

	; Enable inetrrupts after done
	sti

	; IRET
	iret
%endmacro

section .text
; Exceptions
global divbyzero_handler_wrapper
divbyzero_handler_wrapper:
START_ISR
	call divbyzero_handler
END_ISR

global debug_handler_wrapper
debug_handler_wrapper:
START_ISR
	call debug_handler
END_ISR

global nonmask_handler_wrapper
nonmask_handler_wrapper:
START_ISR
	call nonmask_handler
END_ISR

global breakpoint_handler_wrapper
breakpoint_handler_wrapper:
START_ISR
	call breakpoint_handler
END_ISR

global overflow_handler_wrapper
overflow_handler_wrapper:
START_ISR
	call overflow_handler
END_ISR

global boundrange_handler_wrapper
boundrange_handler_wrapper:
START_ISR
	call boundrange_handler
END_ISR

global invalop_handler_wrapper
invalop_handler_wrapper:
START_ISR
	call invalop_handler
END_ISR

global nodevice_handler_wrapper
nodevice_handler_wrapper:
START_ISR
	call nodevice_handler
END_ISR

global doublefault_handler_wrapper
doublefault_handler_wrapper:
START_ISR
	call doublefault_handler
END_ISR

extern divbyzero_handler
extern debug_handler
extern nonmask_handler
extern breakpoint_handler
extern overflow_handler
extern boundrange_handler
extern invalop_handler
extern nodevice_handler
extern doublefault_handler

; Page fault
global pagefault_handler_wrapper
pagefault_handler_wrapper:
START_ISR
	call pagefault_handler
END_ISR

extern pagefault_handler
