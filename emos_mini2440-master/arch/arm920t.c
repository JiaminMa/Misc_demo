#include "arm920t.h"

void disable_cache()
{
    __asm__ __volatile__(
         "mrc p15,0,r0,c1,c0,0  \n\t"
         "orr r0,r0,%[tmpdata] \n\t"
         "mcr p15,0,r0,c1,c0,0 \n\t"
         : 
         :[tmpdata] "r" (0x1004)
         :"r0", "cc","memory"
    );    

}


unsigned int cp15_read_c5()
{
    unsigned int tmp;

    __asm__ __volatile__(
         "mrc p15,0,%[tmpreg],c5,c0,0  \n\t"
         : [tmpreg] "=r" (tmp)
         :
         : "cc","memory"
    );
    return tmp;
}

unsigned int cp15_read_c6()
{
    unsigned int tmp;

    __asm__ __volatile__(
         "mrc p15,0,%[tmpreg],c6,c0,0  \n\t"
         : [tmpreg] "=r" (tmp)
         :
         : "cc","memory"
    );
    return tmp;
}

unsigned int read_cp15regs(unsigned int regnr)
{
    switch (regnr) {
    case CP15_C5R:
        return cp15_read_c5();
        break;
    case CP15_C6R:
        return cp15_read_c6();
        break;    
    default:
        break;
    }
    return 0;
}
