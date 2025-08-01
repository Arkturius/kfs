ENTRY(kstart)

SECTIONS
{
    . = 0x800;
    
    .gdt :
    {
        *(.gdt)
    }

	. = 0x100000;

	.text ALIGN(4K):
	{
		*(.multiboot)
		*(.text)
	}
	.data ALIGN(4K):
	{
		*(.data)
	}
	.bss ALIGN(4K):
	{
		*(.bss)
	}
}
