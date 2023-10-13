%macro START_ISR 0
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

	; Change segs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
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

	; IRET
	iret
%endmacro

%macro END_ISR_ECODE 0
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

	; Get rid of ecode
	add esp, 4

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

global invaltss_handler_wrapper
invaltss_handler_wrapper:
START_ISR
	call invaltss_handler
END_ISR_ECODE

global nosegment_handler_wrapper
nosegment_handler_wrapper:
START_ISR
	call nosegment_handler
END_ISR_ECODE

global stackseg_handler_wrapper
stackseg_handler_wrapper:
START_ISR
	call stackseg_handler
END_ISR_ECODE

global genprotection_handler_wrapper
genprotection_handler_wrapper:
START_ISR
	call genprotection_handler
END_ISR_ECODE

extern divbyzero_handler
extern debug_handler
extern nonmask_handler
extern breakpoint_handler
extern overflow_handler
extern boundrange_handler
extern invalop_handler
extern nodevice_handler
extern doublefault_handler
extern invaltss_handler
extern nosegment_handler
extern stackseg_handler
extern genprotection_handler

; Page fault
global pagefault_handler_wrapper
pagefault_handler_wrapper:
START_ISR
	call pagefault_handler
END_ISR_ECODE

extern pagefault_handler
