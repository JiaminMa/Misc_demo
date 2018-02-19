#include "krlthread.h"
#include "krlmm.h"
#include "arm920t.h"
#include "cpuctrl.h"
#include "printk.h"
#include "krlsched.h"

static void context_t_init(context_t *initp)
{
	initp->ctx_usrsp = 0;
	initp->ctx_svcsp = 0;
	initp->ctx_svcspsr = 0;
	initp->ctx_cpsr = 0;
	initp->ctx_lr = 0;
}

unsigned int krlretn_thread_id(thread_t *tdp)
{
	return (unsigned int)tdp;
}

void thread_t_init(thread_t *initp)
{
	unsigned int hand;
	hal_spinlock_init(&initp->td_lock);
	list_init(&initp->td_list);
	initp->td_flgs = 0;
    initp->td_stus = TDSTUS_NEW;
    initp->td_cpuid = hal_retn_cpuid();
	initp->td_id = krlretn_thread_id(initp);
	initp->td_tick = 0;
	initp->td_privilege = PRILG_USR;
    initp->td_priority = PRITY_MIN;
    initp->td_runmode = 0;
    initp->td_krlstktop = NULL;
    initp->td_krlstkstart = NULL;
    initp->td_usrstktop = NULL;
    initp->td_usrstkstart = NULL;	
	initp->td_mmdsc = NULL;
	initp->td_resdsc = NULL;
	initp->td_privtep = NULL;
	initp->td_extdatap = NULL;
	context_t_init(&initp->td_context);
	for (hand = 0; hand < TD_HAND_MAX; hand++) {
		initp->td_handtbl[hand] = NULL;
	}
}

thread_t* krlnew_thread_dsc() {
	thread_t* rettdp = (thread_t*)(krlnew((size_t)(sizeof(thread_t))));
	if(rettdp == NULL) {
		return NULL;
	}
	thread_t_init(rettdp);
	return rettdp;
}


void krlthreadkrlsatck_init(thread_t *thdp, void *runadr, reg_t cpsr, reg_t spsr)
{
	thdp->td_krlstktop &= (~0xf);
	thdp->td_usrstktop &= (~0xf);
	armregs_t *arp = (armregs_t *)(thdp->td_krlstktop - sizeof(armregs_t));
	arp->r0 = 0;
	arp->r1 = 0;
	arp->r2 = 0;
	arp->r3 = 0;
	arp->r4 = 0;
	arp->r5 = 0;
	arp->r6 = 0;
	arp->r7 = 0;
	arp->r8 = 0;
	arp->r9 = 0;
	arp->r10 = 0;
	arp->r11 = 0;
	arp->r12 = 0;
	arp->r13 = (reg_t)(thdp->td_usrstktop);
	arp->r14 = (reg_t)runadr;
	thdp->td_context.ctx_usrsp = (reg_t)(thdp->td_usrstktop);
	thdp->td_context.ctx_svcsp = (reg_t)(arp);
	thdp->td_context.ctx_svcspsr =spsr;
	thdp->td_context.ctx_cpsr = cpsr;
	thdp->td_context.ctx_lr = (reg_t)runadr;
}

static thread_t *krlnew_thread_core(void *filerun, unsigned int flg, unsigned int prilg,
				unsigned int prity, size_t usrstksz, size_t krlstksz)
{
	thread_t *ret_td = NULL;
	bool_t acs = FALSE;
	adr_t usrstkadr = NULL, krlstkadr = NULL;
	
	usrstkadr = krlnew(usrstksz);
	if (usrstksz == NULL) {
		return NULL;
	}
	
	krlstkadr = krlnew(krlstksz);
	if (krlstkadr == NULL) {
        if(krldelete(usrstkadr, usrstksz)==FALSE)
        {
            return NULL;
        }
        return NULL;
	}
	
	ret_td = krlnew_thread_dsc();
	if(ret_td == NULL) {
        acs = krldelete(usrstkadr,usrstksz);
        acs = krldelete(krlstkadr,krlstksz);
        if(acs == FALSE) {
            return NULL;
        }
        return NULL;
    }
	
	ret_td->td_privilege = prilg;
	ret_td->td_priority = prity;
	ret_td->td_krlstktop = krlstkadr + (adr_t)(krlstksz - 1);
    ret_td->td_krlstkstart = krlstkadr;
    ret_td->td_usrstktop = usrstkadr + (adr_t)(usrstksz - 1);
    ret_td->td_usrstkstart = usrstkadr;
	krlthreadkrlsatck_init(ret_td, filerun, DAFT_CPSR, DAFT_SPSR);
	krlschdclass_add_thread(ret_td);
	return ret_td;
}

thread_t *krlnew_thread(void *filerun, unsigned int flg, unsigned int prilg,
					unsigned int prity, size_t usrstksz, size_t krlstksz)
{
    size_t tustksz=0,tkstksz = 0;
    if((flg&0x1)!=0||filerun==NULL||usrstksz>DAFT_TDUSRSTKSZ||krlstksz>DAFT_TDKRLSTKSZ) {
        return NULL;
    }
    
    if((prilg != PRILG_USR && prilg != PRILG_SYS) || (prity>PRITY_MAX)) {
        return NULL;
    }
    if(usrstksz < DAFT_TDUSRSTKSZ) {
        tustksz = DAFT_TDUSRSTKSZ;    
    }
    if(krlstksz < DAFT_TDKRLSTKSZ) {
        tkstksz = DAFT_TDKRLSTKSZ;
    }
    
    return krlnew_thread_core(filerun,flg,prilg,prity,tustksz,tkstksz);
}

void krlthd_inc_tick(thread_t *thdp)
{
	cpuflg_t cpuflg;
	hal_spinlock_saveflg_cli(&thdp->td_lock, &cpuflg);
	thdp->td_tick++;
	if (thdp->td_tick > TDRUN_TICK) {
		thdp->td_tick = 0;
		krlsched_set_schedflgs();
	}
	hal_spinunlock_restflg_sti(&thdp->td_lock, &cpuflg);
}
