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
    vga_puts("\nKF    KFS   KFSKFSKFS    SKFSKF \n");
    vga_puts("KF   SKF    KFS        KFS   KFS\n");
    vga_puts("KF  FSK     KF         KFS      \n");
    vga_puts("KF KFS      KFSKFSK     FSKF    \n");
    vga_puts("KFSKF       KFS           KFS   \n");
    vga_puts("KF KFS      KF             FSKF \n");
    vga_puts("KF  FSK     KF               KFS\n");
    vga_puts("KF   SKF    KF         KFS   KFS\n");
    vga_puts("KF    KFS   KF          FSKFSK  \n\n");
}

int
kmain(void)
{
    vga_init();
    log("kernel started.");
    log("screen cleared.");

    kfs_logo();
	log("%d", 42);

    for (u32 i = 0; i < 7; ++i)
    {
        u32 addr = 0x800 + 8 * i;
        printk("[%d] ", i);
        printk("0x%08x%08x\n", *((u32 *)addr + 1),*((u32 *)addr));
    }

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
