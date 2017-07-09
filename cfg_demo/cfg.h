#ifndef _CFG_H_
#define _CFG_H_
#define MEM_BASE 0x60010000
#define MEM_SIZE 32 * 1024 * 1024

typedef unsigned long long cfg_bits;
#define CFG_RESOLUTION 2
#define CFG_BITS 24
//#define CFG_BITMAP_SIZE ((MEM_SIZE >> 8) / (sizeof(cfg_bits) * 8))
#define CFG_BITMAP_SIZE (MEM_SIZE >> 8)

void cfg_register(void *fun_addr);
void cfg_check(void *fun_addr);
void cfg_move(void *fun_addr);

#endif
