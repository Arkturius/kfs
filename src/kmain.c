/**
 * kmain.c
 */

#include <kernel.h>
#include <printk.h>
#include <vga.h>
#include <driver/keyboard.h>

int kmain(void)
{
	vga_init();
	printk("%d\n", 42);

 	while (1)
 	{
 		u8	key = kb_key_get();
 
 		if (!key)
 			continue ;
 		if ((kbd_mod & MOD_ALT_L) && key == '\t')
 		{
  			vga_screen_shift();
 			continue;
 		}
 		vga_putc(key);
 	}

	return (0);
}
