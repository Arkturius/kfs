BITS 32
ALIGN 4

%define MAGIC	0x1BADB002

section .multiboot
grub_magic:		dd MAGIC
grub_flags:		dd 0
grub_checksum:	dd -(MAGIC)

section .text

extern kmain

global kstart
kstart:
	mov		esp, STACK_TOP
	call	kmain
	cli
.hlt:
	hlt
	jmp .hlt

section .bss
STACK_TOP:	resb 4096
STACK_BTM:
