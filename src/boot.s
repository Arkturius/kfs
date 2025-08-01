BITS 32
ALIGN 4

%define MAGIC	0x1BADB002
%define FLAGS	0x0

section .multiboot
grub_magic:		dd MAGIC
grub_flags:		dd FLAGS
grub_checksum:	dd -(MAGIC + FLAGS)

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

global nop_loop
nop_loop:
    mov     ecx, dword [esp + 8]
.loop:
    dec     ecx
    test    ecx, ecx
    jne     .loop
    ret

section .bss
STACK_BTM:	resb 4096
STACK_TOP:
