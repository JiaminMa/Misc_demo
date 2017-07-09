ENTRY(reset)
SECTIONS
{
	. = 0x60010000;
/*	. = 0x04000000;*/
 	.startup . : { smc_start.o(.text) }
 	.text : { *(.text) }
 	.data : { *(.data) }
 	.bss : { *(.bss COMMON) }
 	. = ALIGN(8);
 	. = . + 0x1000; /* 4kB of stack memory */
	smc_stack_top = .;
 	. = . + 0x1000; /* 4kB of stack memory */
 	svc_stack_top = .;
}
