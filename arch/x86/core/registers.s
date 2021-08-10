; Save current registers to the register set passed
global dump_registers
dump_registers:
	; Set stack frame
	push ebp
	mov ebp, esp

	; Save used registers
	push ecx
	; Save eax register
	push eax

	; Get address of struct
	mov eax, [ebp + 8]

	; Save general registers except eax
	mov [eax + 4], ebx
	mov [eax + 8], ecx
	mov [eax + 12], edx
	; Save string operation registers
	mov [eax + 16], esi
	mov [eax + 20], edi
	; Save base pointer
	mov ecx, [ebp]
	mov [eax + 24], ecx
	; Save eax register
	mov ecx, eax
	pop eax
	mov [ecx], eax

	; Restore ecx
	pop ecx

	; Return pointer to struct
	lea eax, [ebp + 8]

	; Exit stack frame
	pop ebp

	ret
