/**
 * kmain.c
 */

#include <kernel.h>
#include <printk.h>
#include <vga.h>
#include <keyboard.h>
#include <string.h>

void
kdump_gdt(void);

# define    KERNEL_STACK    0x10000

__inline u32
make_ispmask(u32 ptr)
{
    u32 mask = 0;

    for (u32 i = 0; i < 4; ++i)
    {
        u8 c = (ptr >> 8 * i) & 0xFF;

        mask >>= 8;
        if (c >= 32 && c < 127)
            mask |= 0xFF000000;
    }
    return (mask);
}

void
kdump_stack(void)
{
    u32 esp;
    __asm ("mov %%esp, %0" : "=r" (esp));

    for (u32 addr = KERNEL_STACK; addr >= esp; addr -= 16)
    {
        u8  *sptr = (u8 *)addr;
        u32 *ad1 = (u32 *)addr;
        u32 *ad2 = ad1 + 1;
        u32 *ad3 = ad1 + 2;
        u32 *ad4 = ad1 + 3;

        vga_fg_set(VGA_COLOR_DARK_GREY);
        printk("%08x| ", sptr);
        printk_fflush();
        
        vga_fg_set(VGA_COLOR_LIGHT_GREY);
        if (*ad1 >= esp && *ad1 <= KERNEL_STACK)
            vga_fg_set(VGA_COLOR_LIGHT_RED);
        else if (*ad1 >= 0x00100000 && *ad1 <= 0x00102000)
            vga_fg_set(VGA_COLOR_RED);

        for (u32 i = 0; i < 4; ++i)
            printk("%02x ", sptr[i]);
        printk_fflush();
        vga_putc(' ');

        vga_fg_set(VGA_COLOR_LIGHT_GREY);
        if (*ad2 >= esp && *ad2 <= KERNEL_STACK)
            vga_fg_set(VGA_COLOR_LIGHT_RED);
        else if (*ad2 >= 0x00100000 && *ad2 <= 0x00102000)
            vga_fg_set(VGA_COLOR_RED);

        for (u32 i = 0; i < 4; ++i)
            printk("%02x ", sptr[i + 4]);
        printk_fflush();
        vga_putc(' ');
        
        vga_fg_set(VGA_COLOR_LIGHT_GREY);
        if (*ad3 >= esp && *ad3 <= KERNEL_STACK)
            vga_fg_set(VGA_COLOR_LIGHT_RED);
        else if (*ad3 >= 0x00100000 && *ad3 <= 0x00102000)
            vga_fg_set(VGA_COLOR_RED);

        for (u32 i = 0; i < 4; ++i)
            printk("%02x ", sptr[i + 8]);
        printk_fflush();
        vga_putc(' ');

        vga_fg_set(VGA_COLOR_LIGHT_GREY);
        if (*ad4 >= esp && *ad4 <= KERNEL_STACK)
            vga_fg_set(VGA_COLOR_LIGHT_RED);
        else if (*ad4 >= 0x00100000 && *ad4 <= 0x00102000)
            vga_fg_set(VGA_COLOR_RED);

        for (u32 i = 0; i < 4; ++i)
            printk("%02x ", sptr[i + 12]);
        printk_fflush();

        vga_fg_set(VGA_COLOR_LIGHT_GREY);
        u32 mask = 0x2e2e2e2e;

        u32 data[5] = {*ad1, *ad2, *ad3, *ad4, 0};
        u32 isp;

        isp = make_ispmask(data[0]);
        data[0] ^= mask;
        data[0] &= isp;
        data[0] ^= mask;

        isp = make_ispmask(data[1]);
        data[1] ^= mask;
        data[1] &= isp;
        data[1] ^= mask;

        isp = make_ispmask(data[2]);
        data[2] ^= mask;
        data[2] &= isp;
        data[2] ^= mask;

        isp = make_ispmask(data[3]);
        data[3] ^= mask;
        data[3] &= isp;
        data[3] ^= mask;

        printk("|%s|\n", (char *)data);
    }
}

int
kshell_exec(const char *cmd)
{
    if (!strncmp(cmd, "stack", 6))
        kdump_stack();
    if (!strncmp(cmd, "echo ", 5))
    {
        const char    *msg = cmd + 5;

        while (*msg == ' ' || (*msg >= 9 && *msg <= 13))
            msg++;
        printk("%s\n", msg);
    }
    if (!strncmp(cmd, "halt", 5))
        __asm ("hlt");
    if (!strncmp(cmd, "reboot", 7))
        kreboot();
    if (!strncmp(cmd, "clear", 6))
    {
        VGA_CTX.col = 0;
        VGA_CTX.row = 0;
        vga_screen_clear((u16 *)VGA_SCREEN);
    }
    return (0);
}

int
kshell(void)
{
    while (1)
    {
        memset(VGA_BUF, 0, sizeof(VGA_BUF));
        vga_attr_set(VGA_COLOR_RED, VGA_COLOR_BLACK);
        printk("kfs $ ");
        printk_fflush();
        vga_attr_set(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

        i32 r = kb_read(VGA_BUF, sizeof(VGA_BUF));

        if (r == 0)
        {
            vga_putc('\n');
            continue;
        }
        if (r < 0)
            panic("kb_read");

        vga_putc('\n');
        if (kshell_exec(VGA_BUF))
            panic("kshell_exec");
    }
}

int
kmain(void)
{
    vga_init();

    log("kernel started.");
    log("screen cleared.");
	log("%d", 42);

    log("Global Descriptor Table:");
    kdump_gdt();
    log("launching shell...");
    kshell();

    return (0);
}
