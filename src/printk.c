/**
 * printk.c
 */

#include <kernel.h>
#include <string.h>
#include <stdarg.h>
#include <vga.h>

/* CONTEXT ********************************************************************/

#define		PRINTK_BUFSIZE	1024

typedef struct	printk_ctx
{
	u32			len;
	char		out[PRINTK_BUFSIZE];
}	printf_ctx;

printf_ctx	printk_ctx = {0};

void
printk_fflush(void)
{
	vga_write(printk_ctx.out, printk_ctx.len);
	printk_ctx.len = 0;
}

__inline void
printk_ctx_cat(const char *src, u32 len)
{
	if (printk_ctx.len + len >= PRINTK_BUFSIZE)
		printk_fflush();

	while (len--)
	{
		printk_ctx.out[printk_ctx.len++] = *src++;
		if (printk_ctx.len == PRINTK_BUFSIZE - 1)
		{
			printk_ctx.out[PRINTK_BUFSIZE - 1] = 0;
			printk_fflush();
			printk_ctx.len = 0;
		}
	}
	printk_ctx.out[printk_ctx.len] = 0;
}

/* PRINTER ********************************************************************/

enum	uprintf_flags
{
	U_LONG		= 1 << 0,
	U_ZERO		= 1 << 2,
	U_MINUS		= 1 << 3,
	U_PADDED	= 1 << 4,
	U_INVALID	= 1 << 31
};

__inline u32
printk_switch_len(const char **fmt_ptr)
{
	const char	*fmt = *fmt_ptr;
	u32			z = 0;

	while (*fmt && *fmt >= '0' && *fmt <= '9')
	{
		z = z * 10 + (*fmt - 48);
		fmt++;
	}
	*fmt_ptr = fmt;
	return (z);
}

__inline u32
printk_switch_prefix(const char **fmt_ptr, u32 *pad_ptr)
{
	const char	*fmt = *fmt_ptr;
	u32	flags = 0;

	fmt++;
	if (*fmt == '-')
	{
		fmt++;
		flags |= U_MINUS;
	}
	if (*fmt >= '0' && *fmt <= '9')
	{
		if (*fmt == '0' && !(flags & U_MINUS))
			flags |= U_ZERO;
		flags |= U_PADDED;
		*pad_ptr = printk_switch_len(&fmt);
	}
	if ((flags & U_MINUS) && !(flags & (U_PADDED)))
		flags |= U_INVALID;
	if (*fmt == 'l')
	{
		flags |= U_LONG;
		fmt++;
	}
	*fmt_ptr = fmt;
	return (flags);
}

__inline void
printk_switch_flags(const char **fmt_ptr, va_list ap)
{
	const char	*fmt = *fmt_ptr;
	const char	*to_add;
	u32			flags = 0;
	u32			pad = 0;
	u32			len = 0;
	char		c;

	flags = printk_switch_prefix(&fmt, &pad);
	if (flags & U_INVALID)
		panic("Invalid printk prefix.");
	
	switch (*fmt)
	{
		case 'c':
			c = va_arg(ap, int);
			to_add = &c;
			len = 1;
			goto end;
		case 's':
			to_add = va_arg(ap, char *);
			break ;
		case 'i': 
		case 'd':
			if (flags & U_LONG)
				to_add = lltoa(va_arg(ap, int));
			else
				to_add = lltoa(va_arg(ap, long));
			break ;
		case 'u':
			if (flags & U_LONG)
				to_add = ulltoa(va_arg(ap, unsigned int));
			else
				to_add = ulltoa(va_arg(ap, unsigned long));
			break ;
		case 'x':
			to_add = ulxtoa(va_arg(ap, unsigned int), 0);
			break ;
		case 'X':
			to_add = ulxtoa(va_arg(ap, unsigned int), 1);
			break ;
		case 'p':
			to_add = ptrtoa(va_arg(ap, unsigned long));
			break ;
		default:
			panic("Invalid printk format.");
			return ;
	}
	len = strlen(to_add);
	int	diff;

	diff = pad - len - (*to_add == '-' && *fmt != 's');
	
	if (*fmt != 'p' && (flags & (U_ZERO | U_PADDED)))
	{
		c = flags & U_ZERO ? '0' : ' ';

		if (!(flags & U_MINUS))
		{
			while (diff-- > 0)
				printk_ctx_cat(&c, 1);
		}
	}

end:
	diff = pad - len - (*to_add == '-' && *fmt != 's');
	printk_ctx_cat(to_add, len);
	if (flags & U_MINUS)
	{
		while (diff-- > 0)
			printk_ctx_cat(&c, 1);
	}
	*fmt_ptr = fmt;
}

void
vdprintk(const char *fmt, va_list ap)
{
	if (!fmt)
		return ;
	while (*fmt)
	{
		switch (*fmt)
		{
			case '%':
				printk_switch_flags(&fmt, ap);
				break ;
			case '\n':
				printk_ctx_cat(fmt, 1);
				printk_fflush();
				break ;
			default:
				printk_ctx_cat(fmt, 1);
				break ;
		}
		fmt++;
	}
}

void
printk(const char *fmt, ...)
{
	va_list	ap;

	if (!fmt)
		return ;
	va_start(ap, fmt);
	vdprintk(fmt, ap);
	va_end(ap);
}
