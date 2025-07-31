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

	printk("%d\n", 42);

	while (1)
	{
		u8	key = kb_key_get();

		if (!key)
			continue ;

// 		if (KB_KEY_ALT && key == KB_KEYCODE_TAB)
// 			vga_screen_shift();

		vga_putc(key);
	}

	return (0);
}
