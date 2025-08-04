/**
 * sys/io.h
 */

#ifndef _SYS_IO_H
# define _SYS_IO_H

# include <sys/cdefs.h>
# include <sys/types.h>

__inline u8
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

__inline void
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

__inline void
outw(u16 value, u16 port)
{
    __asm
	(
		"outw %0, %1"
		:
		: "a"(value), "Nd"(port)
	);
}	
__inline u16
inw(u16 port)
{
	u16	ret;
	__asm
	(
		"inw %1, %0"
		: "=a"(ret)
		: "Nd"(port)
	);
	return (ret);
}

#endif // _SYS_IO_H
