#include "cfg.h"
//#define APP_LEVEL
static cfg_bits m_cfg_bitmap[CFG_BITMAP_SIZE];
void board_puts(const char *s);
void board_puthex(const unsigned int num);
#define GET_BITMAP_INDEX(addr) ((addr - MEM_BASE) >> 8)
#define GET_BITMAP_OFFSET(addr) (((addr - MEM_BASE) & 0xfc) >> 2)

#if 0
void cfg_register_bak(void *fun_addr)
{
    unsigned int addr = (unsigned int) fun_addr;
    unsigned int i = (addr - MEM_BASE) >> CFG_RESOLUTION;
    unsigned int z = sizeof(cfg_bits) * 8;

#ifdef APP_LEVEL 
    printf("i/z :%d, i%z:%d\n", i/z, i%z);    
#endif

#ifdef DEBUG
    board_puts("i/z:");
    board_puthex(i / z);
    board_puts("i%z:");
    board_puthex(i % z);
#endif
    m_cfg_bitmap[i / z] |= 1 << (i % z);
}

void cfg_check_bak(void *fun_addr)
{
    unsigned int addr = (unsigned int) fun_addr;
    unsigned int i = (addr - MEM_BASE) >> CFG_RESOLUTION;
    unsigned int z = sizeof(cfg_bits) * 8;
    if ( !((m_cfg_bitmap[i / z] >> (i % z)) & 1 )) {
#ifndef APP_LEVEL 
        board_puts("cfg_check: the illegal jump, should reset\n");
#else
        printf("cfg_check: the illegal jump, should reset\n");
#endif
        while(1);
    }
}

void cfg_move_bak(void *fun_addr)
{
    unsigned int addr = (unsigned int) fun_addr;
    unsigned int i = (addr - MEM_BASE) >> CFG_RESOLUTION;
    unsigned int z = sizeof(cfg_bits) * 8;
    m_cfg_bitmap[i / z] &= 0 << (i % z);
}
#endif

void cfg_register(void *fun_addr)
{
    unsigned int addr = (unsigned int) fun_addr;
    unsigned int index = GET_BITMAP_INDEX(addr);
    unsigned int offset = GET_BITMAP_OFFSET(addr);
    m_cfg_bitmap[index] |= 1 << offset;
}

void cfg_check(void *fun_addr)
{
    unsigned int addr = (unsigned int) fun_addr;
    unsigned int index = GET_BITMAP_INDEX(addr);
    unsigned int offset = GET_BITMAP_OFFSET(addr);
    if ( !((m_cfg_bitmap[index] >> offset) & 1 )) {
        board_puts("cfg_check: the illegal jump, should reset\n");
        while(1);
    }
}

void cfg_move(void *fun_addr)
{
    unsigned int addr = (unsigned int) fun_addr;
    unsigned int index = GET_BITMAP_INDEX(addr);
    unsigned int offset = GET_BITMAP_OFFSET(addr);
    m_cfg_bitmap[index] &= 0 << offset;
}

#ifdef APP_LEVEL
#include <stdio.h>
int main()
{
    printf("cfg bitmap size:%d\n", CFG_BITMAP_SIZE );
    cfg_register(0x60010034);
    cfg_register(0x60345678);
    cfg_register(0x6200ffff);
    return 0;
}
#endif
