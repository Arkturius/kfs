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
    call    gdt_set
    call    gdt_segment_reload
    mov     esp, 0x10000
	call	kmain
	cli
.hlt:
	hlt
	jmp .hlt

gdt_set:
    lgdt    [GDT_DESCRIPTOR]
    ret

gdt_segment_reload:
    jmp     0x08:.reload
.reload:
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ax, 0x18
    mov     ss, ax
    ret

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

section .gdt
global GDT_START
global GDT_END
GDT_START:
GDT_TABLE:
    dq  0x0000000000000000 ; NULL descriptor
    dq  0x00CF9A000000FFFF
    dq  0x00CF92000000FFFF
    dq  0x00C096000000FFFF
    dq  0x00CFFA000000FFFF
    dq  0x00CFF2000000FFFF
    dq  0x00C0F6000000FFFF
GDT_END:

section .data
GDT_DESCRIPTOR:
    dw  GDT_END - GDT_START - 1
    dd  GDT_START
