#include <sys/cdefs.h>
#include <sys/types.h>

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

char	
*strcat(char *dst, const char *src)
{
	char	*tmp = dst;

	while (*src)
		*dst++ = (char)*src++;
	*dst = 0;
	return (tmp);
}

int
strcmp(const char *s1, const char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

int
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

u32
strlen(const char *str)
{
	const char *start;

	start = str;
	while(*str)
		str++;
	return(str - start);
}
