ENTRY(reset)
SECTIONS
{
/*	. = 0x60010000;*/
	. = 0xe0000000;
 	.startup . : { neon_start.o(.text) }
 	.text : { *(.text) }
 	.data : { *(.data) }
 	.bss : { *(.bss COMMON) }
 	. = ALIGN(8);
 	. = . + 0x1000; /* 4kB of stack memory */
 	svc_stack_top = .;
}
