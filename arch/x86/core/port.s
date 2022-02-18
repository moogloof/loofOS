; Input port byte function
; Read byte from port
global inportb
inportb:
	push ebp
	mov ebp, esp

	mov edx, [ebp + 8]
	in al, dx

	pop ebp
	ret

; Output port byte function
; Write byte to port
global outportb
outportb:
	push ebp
	mov ebp, es

	mov al, BYTE [ebp + 12]
	mov edx, [ebp + 8]
	out dx, al

	pop ebp
	ret
