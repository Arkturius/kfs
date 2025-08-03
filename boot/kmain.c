/**
 * kmain.c
 */

#include <kernel.h>
#include <printk.h>
#include <vga.h>
#include <keyboard.h>

void
kdump_gdt(void);

int
kmain(void)
{
    vga_init();

    log("kernel started.");
    log("screen cleared.");
	log("%d", 42);

    log("Global Descriptor Table:");
    kdump_gdt();

    while (1)
    {
        u8 key = kb_key_get();
 
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
