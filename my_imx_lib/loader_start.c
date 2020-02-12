#include <stdint.h>
#include <stddef.h>
#include "loader.h"
#include "imx_flexspi.h"

extern uint64_t _bss;
extern uint64_t _ebss;
extern uint64_t _data;
extern uint64_t _edata;

uint32_t test_global_data = 0;

void hw_init()
{

}


void loader_start(void)
{

    uint8_t *start = (uint8_t *)NULL;
    size_t length = 0;

    /* Clear the BSS section */
    start = (uint8_t *)(&_bss);
    length = (size_t)(&_ebss) - (size_t)(&_bss);
    memclr(start, length);

    trace("Hello BootROM\n");
    hw_init();

    while (1);
}
