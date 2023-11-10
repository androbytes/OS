[bits 64]

LoadGDT:
	; load gdt
	lgdt [rdi]

   ; Reload data segment registers
	mov   ax, 0x10 ; 0x10 is a stand-in for your data segment
	mov   ds, ax
	mov   es, ax
	mov   fs, ax
	mov   ss, ax
	mov   gs, ax

	pop rdi ; Pop return address
	mov rax, 0x8 ; Kernel code segment

	push rax ; New code segment
	push rdi ; Push back the return address
	retfq

global LoadGDT