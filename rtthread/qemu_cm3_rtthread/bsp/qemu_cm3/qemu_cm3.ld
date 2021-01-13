MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 256K
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 256K
}

SECTIONS
{
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
        _etext = .;
    } > FLASH

    /DISCARD/ :
    {
        *(.ARM.exidx*)
        *(.gnu.linkonce.armexidx.*)
    }

    .data : AT(_etext)
    {
        _flash_data = LOADADDR(.data);
        _data = .;
        KEEP(*(.rti_fn.*))
        *(vtable)
        *(.data*)
        _edata = .;
    } > SRAM 

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM

    . = ALIGN(32);           /*Not sure if this needs to be done, but why not.*/
    _stack_bottom = .;       /*Address of the bottom of the stack.*/
    . = . + 0x4000;          /*Allocate 4K for the Stack.*/
    _stack_top = 0x20010000;  /*Address of the top of the heap, also end of RAM.*/
}
