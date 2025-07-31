/**
 * kmain.c
 */

#include <kernel.h>
#include <printk.h>
#include <vga.h>
#include <driver/keyboard.h>

u8
kb_key_get(void);

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
		u8	key = kb_key_get();

		if (!key)
			continue ;

		if ((kbd_mod & MOD_ALT_L) && key == '\t')
		{
 			vga_screen_shift();
 			vga_puts("screen change\n");
			continue;
		}
	
		vga_putc(key);

	}

	return (0);
}
