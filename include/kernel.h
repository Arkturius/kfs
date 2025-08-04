/**
 * kernel.h
 */

#ifndef _KERNEL_H
# define _KERNEL_H

# include <sys/cdefs.h>
# include <printk.h>
# include <vga.h>

__inline void
panic(const char *msg)
{
	vga_attr_set(VGA_COLOR_RED, VGA_COLOR_BLACK);
	printk("kfs 01 panicked: %s\n", msg);
	__asm ("hlt");
}

# if defined(__KFS_DEBUG__)
#  define    log(X, ...)                                                    \
    do                                                                      \
    {                                                                       \
        u8 _attrsave = VGA_CTX.attr;                                        \
        vga_attr_set(VGA_COLOR_DARK_GREY, VGA_COLOR_BLACK);                 \
        vga_puts("kfs $ ");                                                 \
        vga_attr_set(_attrsave & 0xF, _attrsave >> 4);                      \
        printk(X"\n", ##__VA_ARGS__);                                       \
    } while (0);
# else
#  define   log(X, ...) 
# endif

void
kreboot(void);

void
nop_loop(u32 n);

#endif // _KERNEL_H
