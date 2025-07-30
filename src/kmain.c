/**
 * kmain.c
 */

#include <string.h>
#include <vga.h>

void printk(const char *fmt, ...);
void printk_fflush(void);

int kmain(void)
{
	memset(VGA->screen, 0, VGA_WIDTH * VGA_HEIGHT * 2);

	vga_attr_set(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);

	printk("%x\n", 0x45);
	printk("salut %x\n", 0x45);
	printk("salut %u\n", -25);

	return (0);
}
