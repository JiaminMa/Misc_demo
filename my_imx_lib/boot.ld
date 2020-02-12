ENTRY(reset)
SECTIONS
{
	. = 0x007E0000;
	.startup . : { start.o(.text) }
	.text : {
        _text = .;
        *(.text*)
        *(.rodata*)
        _etext = .;
    }

    . = 0x00900000;
    .data :
    {
        _data = .;
        *(.data)
        _edata = .;
    }
	.bss :
    {
        _bss = .;
        *(.bss COMMON)
        _ebss = .;
    }
	. = ALIGN(8);

    . = (0x00900000 + 0x10000 - 0x4000);
	. = . + 0x4000; /* 4kB of stack memory */
	stack_top = .;
}
