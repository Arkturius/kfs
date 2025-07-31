/**
 * kmain.c
 */

#include <kernel.h>
#include <printk.h>
#include <vga.h>
#include <driver/keyboard.h>

void
kfs_logo(void)
{
    vga_cursor_set(26, 7);
    vga_puts("KF    KFS   KFSKFSKFS    SKFSKF \n");
    vga_cursor_set(26, 8);
    vga_puts("KF   SKF    KFS        KFS   KFS\n");
    vga_cursor_set(26, 9);
    vga_puts("KF  FSK     KF         KFS      \n");
    vga_cursor_set(26, 10);
    vga_puts("KF KFS      KFSKFSK     FSKF    \n");
    vga_cursor_set(26, 11);
    vga_puts("KFSKF       KFS           KFS   \n");
    vga_cursor_set(26, 12);
    vga_puts("KF KFS      KF             FSKF \n");
    vga_cursor_set(26, 13);
    vga_puts("KF  FSK     KF               KFS\n");
    vga_cursor_set(26, 14);
    vga_puts("KF   SKF    KF        KFS    KFS\n");
    vga_cursor_set(26, 15);
    vga_puts("KF    KFS   KF          FSKFSK  \n");
}

int
kmain(void)
{
    vga_init();
    kfs_logo();
	vga_attr_set(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK);

    vga_cursor_set(0, 0);

	printk("%d\n", 42);

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
