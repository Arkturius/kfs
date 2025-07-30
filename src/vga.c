/**
 * vga.c
 */

#include <sys/types.h>
#include <sys/io.h>
#include <string.h>
#include <vga.h>

VGA_screen	VGA1 = { .screen = (u16 *)VGA_SCREEN };
VGA_screen	VGA2 = {0};
VGA_screen	VGA3 = {0};
VGA_screen	*VGA = &VGA1;

u16	VGA2_screen[VGA_WIDTH * VGA_HEIGHT] = {0};
u16	VGA3_screen[VGA_WIDTH * VGA_HEIGHT] = {0};

void
vga_cursor_update(void)
{
	u16 pos = (VGA->ctx.row * VGA_WIDTH) + VGA->ctx.col;

	outb(VGA_INDEX_BYTE, 0x0F);
	outb(VGA_DATA_BYTE,  pos);
	outb(VGA_INDEX_BYTE, 0x0E);
	outb(VGA_DATA_BYTE,  pos >> 8);
}

void
vga_cursor_set(u8 x, u8 y)
{
	VGA->ctx.col = x;
	VGA->ctx.row = y;
	vga_cursor_update();
}

void
vga_attr_set(u8 fg, u8 bg)
{
	VGA->ctx.attr = bg << 4 | fg;
}

INLINE void
vga_bg_set(u8 bg)
{
	VGA->ctx.attr &= 0xF;
	VGA->ctx.attr |= bg << 4;
}

INLINE void
vga_fg_set(u8 fg)
{
	VGA->ctx.attr &= 0xF0;
	VGA->ctx.attr |= fg;
}

void
vga_scroll(void)
{
	u32	offset = VGA_WIDTH * (VGA_HEIGHT - 1);

	memcpy(VGA->screen, VGA->screen + VGA_WIDTH, offset * 2);
	memset(VGA->screen + offset, 0, VGA_WIDTH * 2);
}

INLINE void
vga_newline(void)
{
	VGA->ctx.col = 0;
	if (VGA->ctx.row + 1 == VGA_HEIGHT)
		vga_scroll();
	else
		VGA->ctx.row++;
}

void
vga_putc(char c)
{
	if(c == '\n')
		vga_newline();
	else
	{
		VGA->screen[VGA->ctx.row * VGA_WIDTH + VGA->ctx.col] = VGA->ctx.attr << 8 | c;
		VGA->ctx.col++;
		if (VGA->ctx.col == VGA_WIDTH)
			vga_newline();
	}
	vga_cursor_update();
}

void 
vga_puts(char *str)
{
	while(*str)
		vga_putc(*(str++));
}

void 
vga_write(char *str, u32 len)
{
	u32 i;

	i = 0;
	while(i < len)
		vga_putc(str[i++]);
}
