#ifndef _VGA_H
# define _VGA_H

# include <sys/io.h>
# include <sys/cdefs.h>
# include <sys/types.h>

# define	VGA_SCREEN	0xB8000
# define	VGA_WIDTH	80
# define	VGA_HEIGHT	25

# define	VGA_INDEX_BYTE	0x3D4
# define	VGA_DATA_BYTE	(VGA_INDEX_BYTE + 1)

typedef struct _VGA_ctx
{
	u8	col;
	u8	row;
	u8	attr;
}	VGA_ctx;

typedef struct _VGA_screen
{
	VGA_ctx	ctx;
	u16		*screen;
}	VGA_screen;

enum vga_color
{
	VGA_COLOR_BLACK			= 0,
	VGA_COLOR_BLUE			= 1,
	VGA_COLOR_GREEN			= 2,
	VGA_COLOR_CYAN			= 3,
	VGA_COLOR_RED			= 4,
	VGA_COLOR_MAGENTA		= 5,
	VGA_COLOR_BROWN			= 6,
	VGA_COLOR_LIGHT_GREY	= 7,
	VGA_COLOR_DARK_GREY		= 8,
	VGA_COLOR_LIGHT_BLUE	= 9,
	VGA_COLOR_LIGHT_GREEN	= 10,
	VGA_COLOR_LIGHT_CYAN	= 11,
	VGA_COLOR_LIGHT_RED		= 12,
	VGA_COLOR_LIGHT_MAGENTA	= 13,
	VGA_COLOR_LIGHT_BROWN	= 14,
	VGA_COLOR_WHITE			= 15,
};

extern VGA_screen	VGA1;
extern VGA_screen	VGA2;
extern VGA_screen	VGA3;
extern VGA_screen	*VGA;

extern u16	VGA2_screen[VGA_WIDTH * VGA_HEIGHT];
extern u16	VGA3_screen[VGA_WIDTH * VGA_HEIGHT];

void
vga_init(void);

void
vga_attr_set(u8 fg, u8 bg);

void
vga_cursor_set(u8 x, u8 y);

void
vga_putc(char c);

void 
vga_puts(char *str);

void
vga_write(char *str, u32 len);

#endif // _VGA_H
