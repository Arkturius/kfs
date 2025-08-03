/**
 * gdt.c
 */

#include <gdt.h>
#include <printk.h>

#define GDT_SIZE    7

__section(".gdt") GDT_entry   GDT_start[GDT_SIZE] = 
{
    GDT_NULL,
    GDT_ENTRY(
        0, 0xFFFFF,
        GDT_CODE_ACCESS(GDT_PLEVEL_KERNEL),
        GDT_PAGE_32
    ),
    GDT_ENTRY(
        0, 0xFFFFF,
        GDT_DATA_ACCESS(GDT_PLEVEL_KERNEL),
        GDT_PAGE_32
    ),
    GDT_ENTRY(
        0, 0xFFFF,
        GDT_STACK_ACCESS(GDT_PLEVEL_KERNEL),
        GDT_PAGE_32
    ),
    GDT_ENTRY(
        0, 0xFFFFF,
        GDT_CODE_ACCESS(GDT_PLEVEL_USER),
        GDT_PAGE_32
    ),
    GDT_ENTRY(
        0, 0xFFFFF,
        GDT_DATA_ACCESS(GDT_PLEVEL_USER),
        GDT_PAGE_32
    ),
    GDT_ENTRY(
        0, 0xFFFF,
        GDT_STACK_ACCESS(GDT_PLEVEL_USER),
        GDT_PAGE_32
    ),
};

__section(".data") GDT_desc   GDT_descriptor = 
{
    .size = GDT_SIZE * sizeof(GDT_entry) - 1,
    .table = GDT_start,
};

void
kdump_gdt(void)
{
    for (u32 i = 1; i < GDT_SIZE; ++i)
    {
        GDT_entry   *entry = &GDT_start[i];

        u32 base = entry->base[0];
        base += (entry->base[1] << 8);
        base += (entry->base[2] << 16);
        base += (entry->baseMSB << 24);

        u32 limit = *(u16 *)&entry->limit;
        limit += entry->limitMS4b << 16;

        printk("GDT[%d] { 0x%08x to 0x%08x, ", i, base, base + limit);

        if (entry->flags & 0x02)
            printk("long ");
        if (entry->flags & 0x04)
            printk("32-bit, ");
        else
            printk("16-bit, ");
        if (entry->flags & 0x08)
            printk("pages, ");
        else
            printk("bytes, ");

        printk("%s%s%s%s, ", 
            entry->access & 0x08 ? "E" : ".",
            entry->access & 0x04 ? "C" : ".",
            entry->access & 0x02 ? "RW" : ".",
            entry->access & 0x01 ? "A" : "."
        );

        printk("ring %d}\n", (entry->access >> 5) & 0x3);
    }
}
