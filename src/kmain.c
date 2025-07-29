#include <kfs.h>
#include <string.h>

INLINE u8
inb(u16 port)
{
    u8 ret;
    __asm__ volatile (
		"inb %w1, %b0" 
		: "=a"(ret) 
		: "Nd"(port) 
		: "memory"
	);
    return (ret);
}

INLINE void
outb(u16 port, u8 val)
{
    __asm__ volatile (
		"outb %b0, %w1" 
		: 
		: "a"(val), "Nd"(port) 
		: "memory"
	);
}

#define	VGA_INDEX_BYTE	0x3D4
#define VGA_DATA_BYTE	(VGA_INDEX_BYTE + 1)

VGA_screen	VGA1 = { .screen = (u16 *)VGA_SCREEN };
VGA_screen	VGA2 = { 0 };
VGA_screen	VGA3 = { 0 };
VGA_screen	*VGA = NULL;

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

INLINE void
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

	memcpy(VGA1.screen, VGA1.screen + VGA_WIDTH, offset * 2);

	memset(VGA1.screen + offset, 0, VGA_WIDTH * 2);
}

INLINE void
vga_putc(char c)
{
	VGA1.screen[VGA->ctx.row * VGA_WIDTH + VGA->ctx.col] = VGA->ctx.attr << 8 | c;
	VGA->ctx.col++;
	if (VGA->ctx.col == VGA_WIDTH)
	{
		VGA->ctx.col = 0;
		if (VGA->ctx.row + 1 == VGA_HEIGHT)
			vga_scroll();
		else
			VGA->ctx.row++;
	}
	vga_cursor_update();
}

int kmain(void)
{
	VGA2.screen = VGA2_screen;
	VGA3.screen = VGA3_screen;

	memset(VGA->screen, 0, VGA_WIDTH * VGA_HEIGHT * 2);

	vga_attr_set(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);

	vga_cursor_set(39, 12);
	vga_putc('4');
	vga_putc('2');

	return (0);
}
