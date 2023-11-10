[bits 64]

section .text

LoadTSS:
	mov ax, 0x28
	ltr ax
	ret

global LoadTSS