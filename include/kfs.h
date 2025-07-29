#ifndef _KFS_H
# define _KFS_H

// types.h

typedef unsigned int	u32;
typedef unsigned short	u16;
typedef unsigned char	u8;
typedef int				i32;
typedef short			i16;
typedef char			i8;

// cdefs.h

# define	INLINE	static inline __attribute__((always_inline))
# define	UNUSED	__attribute__((unused))

// VGA.h

# define	VGA_SCREEN	0xB8000
# define	VGA_WIDTH	80
# define	VGA_HEIGHT	25

void	kwait(void);

typedef struct _VGA_ctx
{
	u16	*screen;
	u8	col;
	u8	row;
	u8	attr;
}	VGA_ctx;

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

#endif // _KFS_H
