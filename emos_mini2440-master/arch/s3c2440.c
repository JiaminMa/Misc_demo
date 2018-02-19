#include "types.h"
#include "s3c2440.h"
#include "arm920t.h"
#include "link.h"
#include "cpuctrl.h"
#include "mach.h"
#include "global.h"
#include "mm.h"
#include "interrupt.h"
#include "printk.h"

extern void vector();

LKHEAD_T static void s3c2440mmu_set_domain(unsigned int domain)
{
    __asm__ __volatile__(
        "mcr p15,0,%[domval],c3,c0,0 \n\t"
        :
        : [domval]"r"(domain)
        : "cc","memory"
        
    );
    return;
}

LKHEAD_T static void s3c2440mmu_set_tblbass(unsigned int tblbphyadr)
{
    __asm__ __volatile__(
        "mcr p15,0,%[tbass],c2,c0,0 \n\t"
        :
        : [tbass]"r"(tblbphyadr)
        : "cc","memory"
        
    );
    return;
}

LKHEAD_T static void s3c2440mmu_enable()
{
    __asm__ __volatile__(
        "mrc p15,0,r0,c1,c0,0 \n\t"
        "orr r0, r0, #1 \n\t"
        "mcr p15,0,r0,c1,c0,0 \n\t"
        "nop \n\t"
        "nop \n\t"
        "nop \n\t"
        "nop \n\t"
        "nop \n\t"
        "nop \n\t"
        "nop \n\t"
        :
        :
        : "r0","cc","memory"
        
    );
    return;
}

LKHEAD_T static void s3c2440mmu_invalid_dicache()
{
    __asm__ __volatile__(
         "mov    r0, #0 \n\t"
         "mcr    p15, 0, r0, c7, c7, 0 \n\t" 
         "mcr    p15, 0, r0, c7, c10, 4 \n\t"
         "mcr    p15, 0, r0, c8, c7, 0 \n\t"
         :
         :
         :"cc","memory","r0" 
    );
    
    return;
}

LKHEAD_T static void s3c2440_mmu_init()
{
	int i;
	unsigned int vaddr = 0;
	unsigned int pgdsc = 0;
	unsigned int *pgdiraddr = (unsigned int *)PAGE_TLB_DIR;
	for(i = 0; i < PAGE_TLB_SIZE; i++) {
        pgdsc = vaddr | PTE_SECT_AP | PTE_SECT_DOMAIN | 
				PTE_SECT_NOCW| PTE_SECT_BIT;
        pgdiraddr[i] = pgdsc;
        vaddr += 0x100000;
	}
	
	pgdiraddr[0] = SDRAM_MAPVECTPHY_ADDR | PTE_SECT_AP | PTE_SECT_DOMAIN |
					PTE_SECT_NOCW | PTE_SECT_BIT;
					
	s3c2440mmu_set_tblbass(PAGE_TLB_DIR);
    s3c2440mmu_set_domain(~0);
    s3c2440mmu_invalid_dicache();
    disable_cache();
    s3c2440mmu_enable();
}

LKHEAD_T static void s3c2440_vector_copy()
{
    unsigned int *src = (unsigned int *)(&vector);
    unsigned int *det = (unsigned int *)(CPU_VECTOR_VIRADR);
    int i;
    for(i = 0; i < 4096; i++) {
        det[i] = 0;
    }
    for(i = 0;i < 4096;i+=4, src++, det++) {
        *det = *src;
    }   
}

LKHEAD_T static void s3c2440_vector_init()
{
	s3c2440_vector_copy();
}

static void s3c2440_uart_init()
{
	hal_io32_write(ULCON0_R, 3);
    hal_io32_write(UCON0_R, 5);
    hal_io32_write(UFCON0_R, 0);
    hal_io32_write(UMCON0_R, 0);
    hal_io32_write(UBRDIV0_R, 26);
}

void s3c2440_init_platform() 
{
	s3c2440_mmu_init();
	s3c2440_vector_init();
	s3c2440_uart_init();
	init_mach();
    init_mm();
	init_intupt();
#if 0
	print_mach(&osmach);
	print_mmapdsc(&osmach);
#endif
}
