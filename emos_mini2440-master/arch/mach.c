#include "mach.h"
#include "global.h"
#include "spinlock.h"
#include "cpuctrl.h"
#include "list.h"
#include "link.h"
#include "s3c2440.h"
#include "printk.h"

static void mach_t_init(mach_t* initp) 
{
	hal_spinlock_init(&initp->mh_lock);
	list_init(&initp->mh_list);
	initp->mh_kerinramstart = KRNL_INRAM_START;
	initp->mh_kerinramend = (unsigned int)(&__end_kernel);
    initp->mh_mmapdscadr = (mmapdsc_t*)(ALIGN(((unsigned int)(&__end_kernel)),4096));
    initp->mh_mmapdscnr = 0;
	initp->mh_spedscnr = PLFM_ADRSPCE_NR;
	initp->mh_spedsc = machadrspce;
	initp->mh_intfltdsc = machintflt;
	initp->mh_intfltnr = IRQ_MAX;
	
#if 0
	printk("kernel start address:%x\n", initp->mh_kerinramstart);
	printk("kernel end address:%x\n", initp->mh_kerinramend);
	printk("bitmap start address:%x\n", initp->mh_mmapdscadr);
#endif

}

void print_mach(mach_t *mach_p)
{
	printk("mach.mh_kerinramstart:%x\n", (unsigned int)mach_p->mh_kerinramstart);
	printk("mach.mh_kerinramend:%x\n", (unsigned int)mach_p->mh_kerinramend);\
	printk("mach.mh_mmapdscadr:%x\n", (unsigned int)mach_p->mh_mmapdscadr);
	printk("mach.mh_mmapdscnr:%x\n", (unsigned int)mach_p->mh_mmapdscnr);
	printk("mach.mh_spedsc:%x\n", (unsigned int)mach_p->mh_spedsc);
	printk("mach.mh_spedscnr:%x\n", (unsigned int)mach_p->mh_spedscnr);
	printk("mach.mh_intfltdsc:%x\n", (unsigned int)mach_p->mh_intfltdsc);
	printk("mach.mh_intfltnr:%x\n", (unsigned int)mach_p->mh_intfltnr);
}

void init_mach()
{
	mach_t_init(&osmach);
}


