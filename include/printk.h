/**
 * printk.h
 */

#ifndef _PRINTK_H
# define _PRINTK_H

# include <sys/types.h>

i32
printk(const char *fmt, ...);

void
printk_fflush(void);

#endif // _PRINTK_H
