#ifndef _SYS_CDEFS_H
# define _SYS_CDEFS_H

# define	NULL	(void *)0

# define	INLINE	static inline __attribute__((always_inline))
# define	UNUSED	__attribute__((unused))

# define	__asm	__asm__ volatile

#endif // _SYS_CDEFS_H
