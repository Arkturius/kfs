#ifndef _SYS_IO_H
# define _SYS_IO_H

# include <sys/cdefs.h>
# include <sys/types.h>

INLINE u8
inb(u16 port)
{	
	u8 ret;

    __asm
	(
		"inb %w1, %b0" 
		: "=a"(ret) 
		: "Nd"(port) 
		: "memory"
	);
    return (ret);
}

INLINE void
outb(u16 port, u8 val)
{
    __asm
	(
		"outb %b0, %w1" 
		: 
		: "a"(val), "Nd"(port) 
		: "memory"
	);
}

#endif // _SYS_IO_H
