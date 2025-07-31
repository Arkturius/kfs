/**
 * kmain.c
 */

#include <kernel.h>
#include <printk.h>
#include <vga.h>

u8
kb_key_get(void)
{
	return (0);
}

int kmain(void)
{
	vga_init();

	vga_attr_set(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);
	printk("%d\n", 0);
	vga_screen_shift();

	vga_attr_set(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
	printk("%d\n", 1);
	vga_screen_shift();

	vga_attr_set(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
	printk("%d\n", 2);
	vga_screen_shift();

	while (1)
	{
		for (i32 i = 0; i < 0x1fffffff; ++i);
		vga_screen_shift();
	}

	return (0);
}
