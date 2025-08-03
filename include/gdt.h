/**
 * gdt.h
 */

#ifndef _GDT_H
# define _GDT_H

# include <sys/types.h>
# include <sys/cdefs.h>

# define    SEG_LONG(x)         ((x) << 0x01)
# define    SEG_SIZE(x)         ((x) << 0x02)
# define    SEG_GRAN(x)         ((x) << 0x03)

# define    SEG_PRIV(x)         (((x) &  0x03) << 0x05)
# define    SEG_DESCTYPE(x)     ((x) << 0x04)
# define    SEG_PRES(x)         ((x) << 0x07)
  
# define    GDT_PLEVEL_KERNEL   0b00
# define    GDT_PLEVEL_USER     0b11

typedef enum    _gdt_access
{
    SEG_DATA_RD        = 0x00,
    SEG_DATA_RDA       = 0x01,
    SEG_DATA_RDWR      = 0x02,
    SEG_DATA_RDWRA     = 0x03,
    SEG_DATA_RDEXPD    = 0x04,
    SEG_DATA_RDEXPDA   = 0x05,
    SEG_DATA_RDWREXPD  = 0x06,
    SEG_DATA_RDWREXPDA = 0x07,
    SEG_CODE_EX        = 0x08,
    SEG_CODE_EXA       = 0x09,
    SEG_CODE_EXRD      = 0x0A,
    SEG_CODE_EXRDA     = 0x0B,
    SEG_CODE_EXC       = 0x0C,
    SEG_CODE_EXCA      = 0x0D,
    SEG_CODE_EXRDC     = 0x0E,
    SEG_CODE_EXRDCA    = 0x0F,
}   GDT_access;

typedef struct  _gdt_entry
{
    u8  limit[2];
    u8  base[3];
    u8  access;
    u8  limitMS4b   :4;
    u8  flags       :4;
    u8  baseMSB;
}   __packed GDT_entry;

typedef struct  _gdt_desc
{
    u16         size;
    GDT_entry   *table;
}   __packed GDT_desc;

# define    GDT                         __section(".gdt")
# define    GDT_ENTRY(_b, _l, _a, _f)   (GDT_entry)                         \
    {                                                                       \
        .access = _a,                                                       \
        .flags = (_f),                                                      \
        .base[0] = (_b) & 0xFF,                                             \
        .base[1] = ((_b) >> 8) & 0xFF,                                      \
        .base[2] = ((_b) >> 16) & 0xFF,                                     \
        .baseMSB = (_b) >> 24,                                              \
        .limit[0] = _l & 0xFF,                                              \
        .limit[1] = ((_l) >> 8) & 0xFF,                                     \
        .limitMS4b = ((_l) >> 16) & 0x0F                                    \
    }                                                                       \

# define    GDT_DATA_ACCESS(_p)                                             \
    SEG_PRES(1) | SEG_PRIV(_p) | SEG_DESCTYPE(1) | SEG_DATA_RDWR

# define    GDT_CODE_ACCESS(_p)                                             \
    SEG_PRES(1) | SEG_PRIV(_p) | SEG_DESCTYPE(1) | SEG_CODE_EXRD

# define    GDT_STACK_ACCESS(_p)                                            \
    SEG_PRES(1) | SEG_PRIV(_p) | SEG_DESCTYPE(1) | SEG_DATA_RDWREXPD

# define    GDT_BYTE_32         SEG_GRAN(0) | SEG_SIZE(1)
# define    GDT_BYTE_16         SEG_GRAN(0)
# define    GDT_PAGE_32         SEG_GRAN(1) | SEG_SIZE(1)
# define    GDT_PAGE_16         SEG_GRAN(1)

# define    GDT_NULL            GDT_ENTRY(0, 0, 0, 0)

extern GDT_entry    GDT_start[7];
extern GDT_desc     GDT_descriptor;

#endif
