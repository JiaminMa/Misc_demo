ENTRY(_Reset)
SECTIONS
{
     . = 0x60010000;
      .startup . : { start.o(.text) }
       .text : { *(.text) }
        .data : { *(.data) }
         .bss : { *(.bss COMMON) }
          . = ALIGN(8);
        . = . + 0x1000; /* 4kB of stack memory */
        stack_top = .;
}
