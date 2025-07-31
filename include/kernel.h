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

#endif // _KERNEL_H
