#define VGA_SCREEN	0xB8000

typedef unsigned short	u16;

int kmain(void)
{
	u16	*screen = (u16 *)VGA_SCREEN;
	
	screen[0] = (9 << 8) | '4';
	screen[1] = (9 << 8) | '2';

	return (0);
}
