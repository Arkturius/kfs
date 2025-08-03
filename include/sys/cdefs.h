/**
 * sys/cdefs.h
 */

#ifndef _SYS_CDEFS_H
# define _SYS_CDEFS_H

# define	NULL		    (void *)0

# define	__inline	    static inline __attribute__((always_inline))
# define	__unused	    __attribute__((unused))
# define	__asm		    __asm__ volatile

# define    __packed        __attribute__((packed))
# define    __section(X)   __attribute__((section(X)))

#endif // _SYS_CDEFS_H
