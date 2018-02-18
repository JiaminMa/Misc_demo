#include "printk.h"
#include "spinlock.h"
#include "cpuctrl.h"

cpuflg_t cpufg;
spinlock_t lock;
void task1_main()
{
	int i = 0;
	for (;;) {
		i++;
		printk("task1 run %d\n", i);
	}
}

void task0_main()
{
	int i = 0;
	for (;;) {
		i++;
		printk("task0 run %d\n", i);
	}
}
