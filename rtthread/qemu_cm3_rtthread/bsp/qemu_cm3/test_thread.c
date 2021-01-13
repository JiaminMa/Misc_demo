#include <rtthread.h>
#include <stdint.h>

void test_thread1_entry(void *arg)
{
    static uint32_t count = 0;
    while(1) {
        rt_kprintf("test_thread1 entry:%d\n", count++);
        rt_thread_mdelay(1);
    }
}

void test_thread2_entry(void *arg)
{
    static uint32_t count = 0;
    while(1) {
        rt_kprintf("test_thread2 entry:%d\n", count++);
        rt_thread_mdelay(1);
    }
}
