#include "cpuctrl.h"
#include "printk.h"
#include "krlpagempol.h"
#include "krldevice.h"
#include "cpuctrl.h"
#include "s3c24xx.h"
#include "printk.h"
#include "krlcpuidle.h"
#include "krlsched.h"

void task1_main();
void task0_main();
static void task0();
static void task1();
static void init_task();

void kernel_init()
{
	
	init_krlpagempol();
	init_krldevice();
	init_krldriver();
	init_krlsched();
//	init_task();

//	init_krlcpuidle();
//	hal_enable_irq();
	printk("LMOSEM RUN!!");
	hal_sysdie("LMOSEM RUN!!");
}

static void task1()
{

    thread_t* thp= krlnew_thread((void*)task1_main,0,PRILG_USR,PRITY_MIN,0,0);
	thp->name = "task1";
	debug("task1 addr:%x\n", (unsigned int )thp);
    if(thp==NULL)
    {
        hal_sysdie("task1 err");
    }

    return;
}

static void task0()
{
    thread_t* thp= krlnew_thread((void*)task0_main,0,PRILG_USR,PRITY_MIN,0,0);
    thp->name = "task0";
	debug("task0 addr:%x\n", (unsigned int )thp);
    if(thp==NULL)
    {
        hal_sysdie("task0 err");
    }

    return;
}

static void init_task() {
	task0();
	task1();
}

