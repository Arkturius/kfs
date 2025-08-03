#
# sources.mk
#

SRCS_S		:=	boot/kstart.s

SRCS_C		:=	boot/kmain.c		\
				boot/gdt.c			\
				lib/string.c		\
				lib/printk.c		\
				drivers/vga.c		\
				drivers/keyboard.c
