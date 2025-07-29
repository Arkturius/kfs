#ifndef _STRING_H
# define _STRING_H

# include <kfs.h>

INLINE void
*memset(void *s, int c, u32 n)
{
	for (u32 i = 0; i < n; ((u8 *)s)[i] = (u8)c, ++i);
	return (s);
}

INLINE void
*memcpy(void *dst, const void *src, u32 n)
{
	for (u32 i = 0; i < n; ((u8 *)dst)[i] = ((u8*)src)[i], ++i);
	return (dst);
}

#endif // _STRING_H
