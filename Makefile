#
# Makefile
#

PREFIX		:= i686-elf

CC			:= $(PREFIX)-gcc
CFLAGS		:= -ffreestanding -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs -Wall -Wextra -Werror

AS			:= nasm
ASFLAGS		:= -f elf32

LDSCRIPT	:= ./kfs.ld
LDFLAGS		:= -T $(LDSCRIPT)

SRCS_S		:= src/boot.s
SRCS_C		:= src/kmain.c

OBJS_DIR	:= .build

OBJS_S		:= $(addprefix $(OBJS_DIR)/, $(SRCS_S:%.s=%.o))
OBJS_C		:= $(addprefix $(OBJS_DIR)/, $(SRCS_C:%.c=%.o))
OBJS		:= $(OBJS_C) $(OBJS_S)

all:	build

build:	$(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o kfs.bin -lgcc

run:	build
	cp kfs.bin iso/boot/
	grub-mkrescue -o kfs.iso iso
	qemu-system-i386 -cdrom kfs.iso

$(OBJS_DIR)/src/%.o:	src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -Iinclude

$(OBJS_DIR)/src/%.o:	src/%.s
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) $< -o $@
