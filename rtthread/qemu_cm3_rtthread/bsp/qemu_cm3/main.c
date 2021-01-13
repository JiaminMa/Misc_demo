#include <rtthread.h>

void main()
{
    while(1) {
        rt_kprintf("main entry\n");
        rt_thread_mdelay(1);
    }
}
