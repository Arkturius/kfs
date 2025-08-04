/**
 * string.h
 */

#ifndef _STRING_H
# define _STRING_H

# include <sys/io.h>
# include <sys/cdefs.h>
# include <stdbool.h>

void
*memset(void *s, int c, u32 n);

void
*memcpy(void *dst, const void *src, u32 n);


char
*strcat(char *dst, const char *src);

int
strcmp(const char *s1, const char *s2);

int
strncmp(const char *s1, const char *s2, u32 n);

u32
strlen(const char *str);

char *
strchr(const char *s, int c);


const char
*lltoa(i32 i);

const char
*ulltoa(u32 u);

const char
*ulxtoa(u32 u, bool upcase);

const char
*ptrtoa(u32 p);

#endif // _STRING_H
