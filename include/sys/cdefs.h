#ifndef _SYS_CDEFS_H
# define _SYS_CDEFS_H

# define	NULL		(void *)0

# define	__inline	static inline __attribute__((always_inline))
# define	__unused	__attribute__((unused))
# define	__asm		__asm__ volatile

#endif // _SYS_CDEFS_H
