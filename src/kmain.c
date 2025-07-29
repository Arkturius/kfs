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

VGA_ctx	VGA = { .screen = (u16 *)VGA_SCREEN };

void
vga_cursor_update(void)
{
	u16 pos = (VGA.row * VGA_WIDTH) + VGA.col;

	outb(VGA_INDEX_BYTE, 0x0F);
	outb(VGA_DATA_BYTE,  pos);
	outb(VGA_INDEX_BYTE, 0x0E);
	outb(VGA_DATA_BYTE,  pos >> 8);
}

void
vga_cursor_set(u8 x, u8 y)
{
	VGA.col = x;
	VGA.row = y;
	vga_cursor_update();
}

INLINE void
vga_attr_set(u8 fg, u8 bg)
{
	VGA.attr = fg << 4 | (bg & 0xF);
}

INLINE void
vga_bg_set(u8 bg)
{
	VGA.attr &= 0xF0;
	VGA.attr |= bg & 0xF;
}

INLINE void
vga_fg_set(u8 fg)
{
	VGA.attr &= 0xF;
	VGA.attr |= fg << 4;
}

void
vga_scroll(void)
{
	u32	offset = VGA_WIDTH * (VGA_HEIGHT - 1);

	memcpy(VGA.screen, VGA.screen + VGA_WIDTH, offset * 2);

	memset(VGA.screen + offset, 0, VGA_WIDTH * 2);
}

INLINE void
vga_putc(char c)
{
	VGA.screen[VGA.row * VGA_WIDTH + VGA.col] = VGA.attr << 8 | c;
	VGA.col++;
	if (VGA.col == VGA_WIDTH)
	{
		VGA.col = 0;
		if (VGA.row + 1 == VGA_HEIGHT)
			vga_scroll();
		else
			VGA.row++;
	}
	vga_cursor_update();
}

int kmain(void)
{
	vga_attr_set(VGA_COLOR_BLACK, VGA_COLOR_LIGHT_BLUE);

	vga_cursor_set(39, 12);
	vga_putc('4');
	vga_putc('2');

	return (0);
}
