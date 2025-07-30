#include <kfs.h>
#include <string.h>
#include <stdarg.h>

void vga_write(char *str, u32 len);
void vga_putstr(char *str);

static const char	*lltoa(long l)
{
	static char	buff[22] = {0};
	char		*cursor;

	memset(buff, 0, sizeof(buff));
	cursor = &buff[20];
	if (!l)
	{
		*cursor = '0';
		return (cursor);
	}
	while (l)
	{
		char	c = l % 10;
		if (l < 0)
			c = -c;
		*cursor-- = c + 48;
		if (l > -10 && l < 0)
			*cursor-- = '-';
		l /= 10;
	}
	cursor++;
	return (cursor);
}

static const char	*ulltoa(unsigned long l)
{
	static char	buff[21] = {0};
	char		*cursor;

	memset(buff, 0, sizeof(buff));
	cursor = &buff[19];
	if (!l)
	{
		*cursor = '0';
		return (cursor);
	}
	while (l)
	{
		char	c = l % 10;
		*cursor-- = c + 48;
		l /= 10;
	}
	cursor++;
	return (cursor);
}

const char	*ulxtoa(unsigned long l, int upper)
{
	static char	buff[17] = {0};
	static char	*hex = "0123456789abcdef0123456789ABCDEF";
	char		*cursor;

	memset(buff, 0, sizeof(buff));
	cursor = &buff[15];
	if (!l)
	{
		*cursor = '0';
		return (cursor);
	}
	while (l)
	{
		char	c = l & 0xF;
		if (upper)
			c |= 0x10;
		*cursor-- = hex[(int)c];
		l >>= 4;
	}
	cursor++;
	return (cursor);
}

static const char	*ptrtoa(unsigned long l)
{
	static char	buff[19] = {0};

	if (l != 0)
	{
		strcat(buff, "0x");
		strcat(buff + 2, ulxtoa(l, 0));
	}
	else
		strcat(buff, "(nil)");
	return (buff);
}

/* CONTEXT ********************************************************************/

typedef struct	printk_ctx
{
	u32	len;
	char		out[1024];
}	printf_ctx;

printf_ctx	printk_ctx = {0};

static inline void
printk_fflush(void)
{
	vga_write(printk_ctx.out, printk_ctx.len);
	printk_ctx.len = 0;
}

static inline void
printk_ctx_cat(const char *src, u32 len)
{
	if (printk_ctx.len + len >= 1024)
		printk_fflush();

	while (len--)
	{
		printk_ctx.out[printk_ctx.len++] = *src++;
		if (printk_ctx.len == 1023)
		{
			printk_ctx.out[1023] = 0;
			printk_fflush();
			printk_ctx.len = 0;
		}
	}
	printk_ctx.out[printk_ctx.len] = 0;
}

/*
__attribute__((destructor))
static void	printk_ctx_destroy(void)
{
	if (printk_ctx.out)
	{
		printk_fflush();
	}
}
*/

/* PRINTER ********************************************************************/

enum	uprintf_flags
{
	U_LONG		= 1 << 0,
	U_ZERO		= 1 << 2,
	U_MINUS		= 1 << 3,
	U_PADDED	= 1 << 4,
	U_INVALID	= 1 << 31
};

static inline u32
printk_switch_len(const char **fmt_ptr)
{
	const char	*fmt = *fmt_ptr;
	u32	z = 0;

	while (*fmt && *fmt >= '0' && *fmt <= '9')
	{
		z = z * 10 + (*fmt - 48);
		fmt++;
	}
	*fmt_ptr = fmt;
	return (z);
}

static inline u32
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

static inline void
printk_switch_flags(const char **fmt_ptr, va_list ap)
{
	const char	*fmt = *fmt_ptr;
	const char	*to_add;
	u32	flags = 0;
	u32	pad = 0;
	u32	len = 0;
	char		c;

	flags = printk_switch_prefix(&fmt, &pad);
	if (flags & U_INVALID)
		return ; //TODO: we need a exit
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
			vga_putstr("Unsupported format.\n");
			//exit(1); //TODO: we need a exit
			return ;
	}
	len = strlen(to_add);
	int	diff = pad - len - (*to_add == '-' && *fmt != 's');
	
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
	printk_ctx_cat(to_add, len);
//	if (flags & U_MINUS)
//	{
//		while (diff-- > 0)
//			printk_ctx_cat(&c, 1);
//	}
	*fmt_ptr = fmt;
}

void	vdprintk(const char *fmt, va_list ap)
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

void	printk(const char *fmt, ...)
{
	va_list	ap;

	if (!fmt)
		return ;
	va_start(ap, fmt);
	vdprintk(fmt, ap);
	va_end(ap);
}
