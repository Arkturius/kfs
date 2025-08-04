/**
 * string.c
 */

#include <stdbool.h>
#include <sys/cdefs.h>
#include <sys/types.h>

/* MEMORY *********************************************************************/

void
*memset(void *s, int c, u32 n)
{
	for (u32 i = 0; i < n; ((u8 *)s)[i] = (u8)c, ++i);
	return (s);
}

void
*memcpy(void *dst, const void *src, u32 n)
{
	for (u32 i = 0; i < n; ((u8 *)dst)[i] = ((u8*)src)[i], ++i);
	return (dst);
}

/* STRING *********************************************************************/
char *
strchr(const char *s, int c)
{
	int		i;
	char	*res;

	i = 0;
	res = (char *)s;
	while (res[i])
	{
		if (res[i] == (unsigned char)c)
			return (res + i);
		i++;
	}
	if (res[i] == (unsigned char)c)
		return (res + i);
	return (NULL);
}

u32
strlen(const char *str)
{
	const char *start;

	start = str;
	while(*str)
		str++;
	return(str - start);
}

i32
strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

i32
strncmp(const char *s1, const char *s2, u32 n)
{
	while (*s1 && *s1 == *s2 && n--)
	{
		s1++;
		s2++;
	}
	if (!*s1 || !*s2)
		return (0);
	return (*s1 - *s2);
}

char	
*strcat(char *dst, const char *src)
{
	char	*tmp = dst;

	while (*tmp)
		tmp++;
	while (*src)
		*tmp++ = (char)*src++;
	*tmp = 0;
	return (dst);
}

/* ANY_TO_STR *****************************************************************/

const char
*lltoa(i32 l)
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

const char
*ulltoa(u32 l)
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

const char
*ulxtoa(u32 l, bool upcase)
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
		if (upcase)
			c |= 0x10;
		*cursor-- = hex[(int)c];
		l >>= 4;
	}
	cursor++;
	return (cursor);
}

const char
*ptrtoa(u32 l)
{
	static char	buff[19] = {0};

	if (l != 0)
	{
		strcat(buff, "0x");
		strcat(buff, ulxtoa(l, 0));
	}
	else
		strcat(buff, "(nil)");
	return (buff);
}
