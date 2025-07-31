/**
 * vga.c
 */

#include "stdbool.h"
#include <kernel.h>
#include <sys/types.h>
#include <sys/io.h>
#include <string.h>
#include <vga.h>

u16	VGA_tmp[VGA_WIDTH * VGA_HEIGHT] = {0};
u16	VGA0_screen[VGA_WIDTH * VGA_HEIGHT] = {0};
u16	VGA1_screen[VGA_WIDTH * VGA_HEIGHT] = {0};

VGA_ctx		VGA_CTX = {0};
VGA_screen	VGA0 = { .screen = (u16 *)VGA0_screen };
VGA_screen	VGA1 = { .screen = (u16 *)VGA1_screen };

__inline void
vga_cursor_update(void)
{
	u16 pos = (VGA_CTX.row * VGA_WIDTH) + VGA_CTX.col;

	outb(VGA_INDEX_BYTE, 0x0F);
	outb(VGA_DATA_BYTE,  pos & 0xFF);
	outb(VGA_INDEX_BYTE, 0x0E);
	outb(VGA_DATA_BYTE,  pos >> 8);
}

__inline void
vga_scroll(void)
{
	u32	offset = VGA_WIDTH * (VGA_HEIGHT - 1);

	memcpy((void *)VGA_SCREEN, (void *)VGA_SCREEN + VGA_WIDTH, offset * 2);
	memset((void *)VGA_SCREEN + offset, 0, VGA_WIDTH * 2);
}

__inline void
vga_newline(void)
{
	VGA_CTX.col = 0;
	if (VGA_CTX.row + 1 == VGA_HEIGHT)
		vga_scroll();
	else
		VGA_CTX.row++;
	((u16 *)VGA_SCREEN)[VGA_CTX.row * VGA_WIDTH + VGA_CTX.col] = (VGA_CTX.attr << 8) | ' ';
	vga_cursor_update();
}

#define	TAB_SIZE	4

__inline void
vga_tabulation(void)
{
	if (VGA_CTX.col > VGA_WIDTH - 4 - 1)
	{
		vga_newline();
		return ;
	}
	u8 spacing = TAB_SIZE - (VGA_CTX.col & (TAB_SIZE - 1));
	for (u8 i = 0; i < spacing; vga_putc(' '), ++i);
}

__inline u32
vga_go_back(void)
{
	if (!VGA_CTX.col)
	{
		if (!VGA_CTX.row)
			return (1);
		VGA_CTX.row--;
		VGA_CTX.col = VGA_WIDTH;
	}
	VGA_CTX.col--;
	return (0);
}

__inline void
vga_backspace(void)
{
	while (!vga_go_back())
	{
		u16	*current = &((u16 *)VGA_SCREEN)[VGA_CTX.row * VGA_WIDTH + VGA_CTX.col];

		if ((*current & 0xFF) != 0)
		{
			*current = VGA_CTX.attr << 8;
			break ;
		}
	}
	vga_cursor_update();
}

void
vga_screen_clear(u16 *vga)
{
	for (u32 i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
		vga[i] = (VGA_CTX.attr << 8) | ' ';
}

void
vga_init(void)
{
	VGA_CTX.attr = VGA_INIT_ATTR;
	VGA0.ctx.attr = VGA_INIT_ATTR;
	VGA1.ctx.attr = VGA_INIT_ATTR;
	
	vga_screen_clear((u16 *)VGA_SCREEN);
	vga_screen_clear(VGA0.screen);
	vga_screen_clear(VGA1.screen);
}

void
vga_bg_set(u8 bg)
{
	VGA_CTX.attr &= 0xF;
	VGA_CTX.attr |= bg << 4;
}

void
vga_fg_set(u8 fg)
{
	VGA_CTX.attr &= 0xF0;
	VGA_CTX.attr |= fg;
}

void
vga_attr_set(u8 fg, u8 bg)
{
	VGA_CTX.attr = bg << 4 | fg;
}

void
vga_cursor_set(u8 x, u8 y)
{
	VGA_CTX.col = x;
	VGA_CTX.row = y;
	vga_cursor_update();
}

void
vga_screen_shift(void)
{
	static bool	n = false;
	VGA_ctx		tmp;
	VGA_ctx		*ctx = n ? &VGA0.ctx : &VGA1.ctx;
	void		*screen = n ? VGA0_screen : VGA1_screen;

	tmp = VGA_CTX;
	VGA_CTX = *ctx;
	*ctx = tmp;

	memcpy(VGA_tmp, (u16 *)VGA_SCREEN, VGA_WIDTH * VGA_HEIGHT * 2);
	memcpy((u16 *)VGA_SCREEN, screen, VGA_WIDTH * VGA_HEIGHT * 2);
	memcpy(screen, VGA_tmp, VGA_HEIGHT * VGA_WIDTH * 2);

	n = !n;
}

void
vga_putc(char c)
{
	switch (c)
	{
		case '\n':
			vga_newline();
			return ;
		case '\t':
			vga_tabulation();
			return ;
		case '\b':
			vga_backspace();
			return ;
		default:
			break ;
	}
	((u16 *)VGA_SCREEN)[VGA_CTX.row * VGA_WIDTH + VGA_CTX.col] = (VGA_CTX.attr << 8) | c;
	VGA_CTX.col++;
	if (VGA_CTX.col == VGA_WIDTH)
		vga_newline();
	vga_cursor_update();
}

void 
vga_puts(const char *str)
{
	while(*str)
		vga_putc(*(str++));
}

void 
vga_write(const char *str, u32 len)
{
	u32 i;

	i = 0;
	while(i < len)
		vga_putc(str[i++]);
}
