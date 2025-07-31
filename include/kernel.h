/**
 * kernel.h
 */

#ifndef _KERNEL_H
# define _KERNEL_H

# include <sys/cdefs.h>
# include <vga.h>

__inline void
panic(const char *msg)
{
	vga_puts(msg);
	__asm ("hlt");
}

#endif // _KERNEL_H
