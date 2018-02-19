#ifndef _ARM920T_H_
#define _ARM920T_H_
#include "types.h"
void disable_cache(void);
unsigned int cp15_read_c5(void);
unsigned int cp15_read_c6(void);
unsigned int read_cp15regs(unsigned int regnr);

#define CP15_C5R 5
#define CP15_C6R 6
#define CPU_VECTOR_VIRADR 0

typedef struct s_INTSTKREGS
{
    reg_t s_spsr;
	reg_t c_lr;
    reg_t r0;
    reg_t r1;
    reg_t r2;
    reg_t r3;
    reg_t r4;
    reg_t r5;
    reg_t r6;
    reg_t r7;
    reg_t r8;
    reg_t r9;
    reg_t r10;
    reg_t r11;
    reg_t r12;
    reg_t r13;
    reg_t r14;
    reg_t s_lr;
}intstkregs_t;

typedef struct s_ARMREGS {
    reg_t r0;
    reg_t r1;
    reg_t r2;
    reg_t r3;
    reg_t r4;
    reg_t r5;
    reg_t r6;
    reg_t r7;
    reg_t r8;
    reg_t r9;
    reg_t r10;
    reg_t r11;
    reg_t r12;
    reg_t r13;
    reg_t r14;	
}armregs_t;

#endif
