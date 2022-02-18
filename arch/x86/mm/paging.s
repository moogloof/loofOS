global enable_paging
enable_paging:
	mov eax, 0xf00000
	mov cr3, eax

	; Enable 4 MiB pages
	mov eax, cr4
	or eax, 0x10
	mov cr4, eax

	; Set paging bit of cr0 register
	mov eax, cr0
	or eax, 0x80000001
	mov cr0, eax

	jmp short_jump
short_jump:
	jmp 0x8:long_jump
long_jump:

	ret
