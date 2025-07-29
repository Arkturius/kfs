#define VGA_SCREEN	0xB8000
#define VGA_WIDTH	80
#define VGA_HEIGHT	25

typedef unsigned short	u16;
typedef unsigned char	u8;

static inline u8 inb(u16 port)
{
    u8 ret;
    __asm__ volatile ( "inb %w1, %b0" : "=a"(ret) : "Nd"(port) : "memory");
    return ret;
}

static inline void outb(u16 port, u8 val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static void set_cursor(u8 x, u8 y)
{
	u16 pos = (y * VGA_WIDTH) + x;
	outb(0x3D4, 0x0F);
	outb(0x3D5, pos);
	outb(0x3D4, 0x0E);
	outb(0x3D5, pos >> 8);
}

int kmain(void)
{
	u16	*screen = (u16 *)VGA_SCREEN;
	
	screen[0] = (9 << 8) | '4';
	screen[1] = (9 << 8) | '2';

	set_cursor(2, 0);


	return (0);
}
