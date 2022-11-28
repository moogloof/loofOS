section .text
; Input port byte function
; Read byte from port
global inportb
inportb:
	push ebp
	mov ebp, esp
	push edx

	mov edx, [ebp + 8]
	in al, dx

	pop edx
	pop ebp
	ret

; Input port word function
; Read word from port
global inportw
inportw:
	push ebp
	mov ebp, esp
	push edx

	mov edx, [ebp + 8]
	in ax, dx

	pop edx
	pop ebp
	ret

; Output port byte function
; Write byte to port
global outportb
outportb:
	push ebp
	mov ebp, esp
	push eax
	push edx

	mov al, BYTE [ebp + 12]
	mov edx, [ebp + 8]
	out dx, al

	pop edx
	pop eax
	pop ebp
	ret
