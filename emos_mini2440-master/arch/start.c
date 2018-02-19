#include "types.h"
#include "s3c2440.h"
#include "printk.h"
#include "krlinit.h"
#include "mm.h"

LKHEAD_T int kernel_start()
{
    char *s = "OS Version:";
	
	s3c2440_init_platform();
    printk("%s %s\n", s, "1.0.2");	
	kernel_init();
    for(;;);
    return 0;
}
