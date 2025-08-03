#
# Makefile
#

include sources.mk

PREFIX		:=	i686-elf

AS			:=	nasm
ASFLAGS		:=	-f elf32

LD			:=	$(PREFIX)-ld
LDSCRIPT	:=	./kfs.ld
LDFLAGS		:=	-T $(LDSCRIPT) -m elf_i386

CC			:=	$(PREFIX)-gcc
CFLAGS		:=	-ffreestanding			\
				-fno-builtin			\
				-fno-exceptions			\
				-fno-stack-protector	\
				-nostdlib				\
				-nodefaultlibs			\
				-Wall -Wextra -Werror	\

INC_DIR		:=	include

INC_DIRS	:=	drivers lib sys
INC_DIRS	:=	$(addprefix $(INC_DIR)/, $(INC_DIRS)) $(INC_DIR)

CFLAGS		+=	$(foreach dir, $(INC_DIRS), -I$(dir))

OBJS_DIR	:=	.build

OBJS_S		:=	$(addprefix $(OBJS_DIR)/, $(SRCS_S:%.s=%.o))
OBJS_C		:=	$(addprefix $(OBJS_DIR)/, $(SRCS_C:%.c=%.o))
OBJS		:=	$(OBJS_C) $(OBJS_S)

ELF			:=	kfs.elf
ISO			:=	kfs.iso

ifeq ($(BONUS), 1)
	CFLAGS	+= -DKFS_BONUS
endif

DEBUG		?=	1

ifeq ($(DEBUG), 1)
	CFLAGS	+= -D__KFS_DEBUG__
endif


all:	$(ELF)

run:	$(ELF)
	cp $< iso/boot/
	grub-mkrescue -o $(ISO) iso
	qemu-system-i386 -cdrom $(ISO) -no-reboot -no-shutdown

$(ELF):	$(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

$(OBJS_DIR)/%.o:	%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -lgcc

$(OBJS_DIR)/%.o:	%.s
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@

fclean:
	@rm -rf $(OBJS_DIR)
	@rm -rf $(ELF) $(ISO)

re:		fclean run
