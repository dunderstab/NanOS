
ENTRY(_start)
 
SECTIONS
{

	. = 1M;
 
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}
 
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}
 
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
 
    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
        *(.note*)
        *(.iplt)
        *(.igot)
    }

    end = .; _end = .; __end = .;
}