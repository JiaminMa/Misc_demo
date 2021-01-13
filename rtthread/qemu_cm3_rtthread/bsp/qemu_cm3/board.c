#include <stdint.h>

#define SCS_BASE            (0xE000E000)                 /*System Control Space Base Address */
#define SYSTICK_BASE        (SCS_BASE +  0x0010)         /*SysTick Base Address*/
#define SCB_BASE            (SCS_BASE +  0x0D00)
#define HSI_CLK             12000000UL
#define SYSTICK_PRIO_REG    (0xE000ED23)

typedef struct systick_tag {
    volatile uint32_t ctrl;
    volatile uint32_t load;
    volatile uint32_t val;
    volatile uint32_t calrb;
}systick_t;

volatile uint32_t * const UART0DR = (uint32_t *)0x4000C000;

static void init_systick()
{
    systick_t *systick_p = (systick_t *)SYSTICK_BASE;
    uint8_t *sys_prio_p = (uint8_t *)SYSTICK_PRIO_REG;
    *sys_prio_p = 0xf0;
    systick_p->load = (HSI_CLK & 0xffffffUL) - 1;
    systick_p->val = 0;
    systick_p->ctrl = 0x7;
}

// Symbols Created by the Linker
extern uint32_t _etext;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;
extern uint32_t _stack_bottom;
extern uint32_t _stack_top;
extern uint32_t _flash_data;

void rt_hw_copy_data_bss()
{
    uint32_t *src = (uint32_t *)&_flash_data;
    uint32_t *dst = (uint32_t *)&_data;
    uint32_t len = (uint32_t )&_edata - (uint32_t)&_data;

    while(len > 0) {
        *dst = *src;
        dst++;
        src++;
        len -= 4;
    }

}

void rt_hw_board_init(void)
{
    init_systick();
}

void rt_hw_console_output(const char *str)
{
    while(*str != 0) {
        *UART0DR = *str++;
    }
}

void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

