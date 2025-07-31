#
# Makefile
#

include sources.mk

PREFIX		:=	i686-elf

CC			:=	$(PREFIX)-gcc
CFLAGS		:=	-ffreestanding -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs -Wall -Wextra -Werror -O2 -Iinclude

AS			:=	nasm
ASFLAGS		:=	-f elf32

LDSCRIPT	:=	./kfs.ld
LDFLAGS		:=	-T $(LDSCRIPT)

SRCS_DIR	:=	src

SRCS_S		:=	$(addprefix $(SRCS_DIR)/, $(SRCS_S))
SRCS_C		:=	$(addprefix $(SRCS_DIR)/, $(SRCS_C))

OBJS_DIR	:=	.build

OBJS_S		:=	$(addprefix $(OBJS_DIR)/, $(SRCS_S:%.s=%.o))
OBJS_C		:=	$(addprefix $(OBJS_DIR)/, $(SRCS_C:%.c=%.o))
OBJS		:=	$(OBJS_C) $(OBJS_S)

BINARY		:=	kfs.bin
ISO			:=	kfs.iso

ifeq ($(BONUS), 1)
	CFLAGS	+= -DKFS_BONUS
endif

all:	$(BINARY)

run:	$(BINARY)
	cp kfs.bin iso/boot/
	grub-mkrescue -o $(ISO) iso
	qemu-system-i386 -cdrom $(ISO)

$(BINARY):	$(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o kfs.bin -lgcc

$(OBJS_DIR)/src/%.o:	src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/src/%.o:	src/%.s
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

fclean:
	@rm -rf $(OBJS_DIR)
	@rm -rf $(BINARY) $(ISO)

re:		fclean run
