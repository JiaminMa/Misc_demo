#include <rtthread.h>
#include <stdint.h>


extern void test_thread1_entry(void *arg);
extern void test_thread2_entry(void *arg);

static struct rt_thread test_thread1;
static struct rt_thread test_thread2;
static uint8_t thread1_stack[256];
static uint8_t thread2_stack[256];

void main()
{
    rt_thread_init(&test_thread1, "test thread1", test_thread1_entry, RT_NULL, &thread1_stack[0], 
                    sizeof(thread1_stack), 0, 10);
    rt_thread_startup(&test_thread1);

    rt_thread_init(&test_thread2, "test thread2", test_thread2_entry, RT_NULL, &thread2_stack[0], 
                    sizeof(thread2_stack), 0, 10);
    rt_thread_startup(&test_thread2);
}
