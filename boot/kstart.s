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
extern GDT_descriptor

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
    lgdt    [GDT_descriptor]
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

global kreboot
kreboot:
    jmp     0xFFFF:0
    hlt

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
