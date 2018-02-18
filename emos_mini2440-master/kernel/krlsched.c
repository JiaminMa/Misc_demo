#include "krlthread.h"
#include "cpuctrl.h"
#include "krlsched.h"
#include "krlglobal.h"
#include "printk.h"
#include "krlwaitlist.h"

void thrdlst_t_init(thrdlst_t *initp)
{
	list_init(&initp->tdl_lsth);
	initp->tdl_curruntd = NULL;
	initp->tdl_nr = 0;
}

void schdata_t_init(schdata_t *initp)
{
	unsigned int ti;
	hal_spinlock_init(&initp->sda_lock);
	initp->sda_cpuid = hal_retn_cpuid();
	initp->sda_schdflgs = NOTS_SCHED_FLGS;
	initp->sda_premptidx = 0;
	initp->sda_threadnr = 0;
	initp->sda_prityidx = 0;
	initp->sda_cpuidle = NULL;
	initp->sda_currtd = NULL;
	for (ti = 0; ti < PRITY_MAX; ti++) {
		thrdlst_t_init(&initp->sda_thdlst[ti]);
	}
}

void schedclass_t_init(schedclass_t *initp)
{
	unsigned int si;
	hal_spinlock_init(&initp->scls_lock);
	initp->scls_cpunr = CPUCORE_MAX;
	initp->scls_threadnr = 0;
	initp->scls_threadid_inc = 0;
	for (si = 0; si < CPUCORE_MAX; si++) {
		schdata_t_init(&initp->scls_schda[si]);
	}
}

void init_krlsched()
{
	schedclass_t_init(&osschedcls);
}

thread_t *krlsched_retn_currthread()
{
	unsigned int cpuid = hal_retn_cpuid();
	schdata_t *schdap = &osschedcls.scls_schda[cpuid];
	if (schdap->sda_currtd == NULL) {
		hal_sysdie("schdap->sda_currtd NULL");
	}
	return schdap->sda_currtd;
}

void krlsched_wait(kwlst_t *wlst)
{
	cpuflg_t cufg, tcufg;
	unsigned int cpuid = hal_retn_cpuid();
	schdata_t *schdap = &osschedcls.scls_schda[cpuid];
	thread_t *tdp = krlsched_retn_currthread();
	unsigned int pity = tdp->td_priority;
	
	if (pity >= PRITY_MAX || wlst == NULL) {
		hal_sysdie("krlsched_wait err");
	}
	
	if (schdap->sda_thdlst[pity].tdl_nr < 1) {
		hal_sysdie("krlsched_wait err");
	}
	
	hal_spinlock_saveflg_cli(&schdap->sda_lock, &cufg);
	hal_spinlock_saveflg_cli(&tdp->td_lock, &tcufg);
	
	tdp->td_stus = TDSTUS_WAIT;
	list_del(&tdp->td_list);
	hal_spinunlock_restflg_sti(&tdp->td_lock, &tcufg);
	
	if (schdap->sda_thdlst[pity].tdl_curruntd == tdp) {
		schdap->sda_thdlst[pity].tdl_curruntd = NULL;
	}
	
	schdap->sda_thdlst[pity].tdl_nr--;
	hal_spinunlock_restflg_sti(&schdap->sda_lock, &cufg);
	krlwlst_add_thread(wlst, tdp);
}

void krlsched_up(kwlst_t *wlst)
{
	cpuflg_t cufg, tcufg;
	unsigned int cpuid = hal_retn_cpuid();
	schdata_t *schdap = &osschedcls.scls_schda[cpuid];
	
	thread_t *tdp;
	unsigned int pity;
	if (wlst == NULL) {
		goto err_step;
	}
	
	tdp = krlwlst_del_thread(wlst);
	if (tdp == NULL) {
		goto err_step;
	}
	
	pity = tdp->td_priority;
	if (pity >= PRITY_MAX) {
		goto err_step;
	}
	
	hal_spinlock_saveflg_cli(&schdap->sda_lock, &cufg);
	
	hal_spinlock_saveflg_cli(&tdp->td_lock, &tcufg);
	tdp->td_stus = TDSTUS_RUN;
	hal_spinunlock_restflg_sti(&tdp->td_lock, &tcufg);
	
	list_add_tail(&tdp->td_list, &(schdap->sda_thdlst[pity].tdl_lsth));
	schdap->sda_thdlst[pity].tdl_nr++;
	hal_spinunlock_restflg_sti(&schdap->sda_lock, &cufg);

err_step:
	hal_sysdie("krlsched_up err");
} 

thread_t *krlsched_retn_idlethread()
{
	unsigned int cpuid = hal_retn_cpuid();
	schdata_t *schdap = &osschedcls.scls_schda[cpuid];
	if (schdap->sda_cpuidle == NULL) {
		hal_sysdie("schdap->sda_cpuidle NULL");
	}
	return schdap->sda_cpuidle;
}

void krlsched_set_schedflgs()
{
	cpuflg_t cpuflg;
	unsigned int cpuid = hal_retn_cpuid();
	schdata_t *schdap = &osschedcls.scls_schda[cpuid];
	hal_spinlock_saveflg_cli(&schdap->sda_lock, &cpuflg);
	schdap->sda_schdflgs = NEED_SCHED_FLGS;
	hal_spinunlock_restflg_sti(&schdap->sda_lock, &cpuflg);
}

void krlsched_chkneed_pmptsched()
{
	cpuflg_t cpuflg;
	unsigned int schd = 0, cpuid = hal_retn_cpuid();
	schdata_t *schdap = &osschedcls.scls_schda[cpuid];
	
	hal_spinlock_saveflg_cli(&schdap->sda_lock, &cpuflg);
	if (schdap->sda_schdflgs == NEED_SCHED_FLGS 
			&& schdap->sda_premptidx == PMPT_FLGS) {
		schdap->sda_schdflgs = NOTS_SCHED_FLGS;
		schd = 1;
	}
	hal_spinunlock_restflg_sti(&schdap->sda_lock, &cpuflg);
	if (schd == 1) {
		krlschedul();
	}
}


/**********************************
 *进程选择
 *********************************/
thread_t *krlsched_select_thread()
{
	unsigned int pity;
	thread_t *retthd, *tdtmp;
	cpuflg_t cpufg;
	unsigned int cpuid = hal_retn_cpuid();
	schdata_t *schdap = &osschedcls.scls_schda[cpuid];
	
	hal_spinlock_saveflg_cli(&schdap->sda_lock, &cpufg);
	for (pity = 0; pity < PRITY_MAX; pity++) {
		if (schdap->sda_thdlst[pity].tdl_nr > 0) {
			/*如果当前优先级队列不为空，那么取出队列头结点插入到队列尾*/
			if(list_is_empty_careful(&(schdap->sda_thdlst[pity].tdl_lsth))==FALSE) {
				tdtmp = list_entry(schdap->sda_thdlst[pity].tdl_lsth.next, thread_t, td_list);
				list_del(&tdtmp->td_list);
				if(schdap->sda_thdlst[pity].tdl_curruntd != NULL) {
					list_add_tail(&(schdap->sda_thdlst[pity].tdl_curruntd->td_list), &schdap->sda_thdlst[pity].tdl_lsth);
				}
			}
			schdap->sda_thdlst[pity].tdl_curruntd = tdtmp;
			retthd = tdtmp;
			goto return_step;
		}
		
		if (schdap->sda_thdlst[pity].tdl_curruntd != NULL) {
			retthd = schdap->sda_thdlst[pity].tdl_curruntd;
			goto return_step;
		}
	}
	schdap->sda_prityidx = PRITY_MIN;
	retthd = krlsched_retn_idlethread();
	
return_step:
	hal_spinunlock_restflg_sti(&schdap->sda_lock, &cpufg);
	return retthd;
}

void krlschedul()
{
	thread_t *prev = krlsched_retn_currthread();
	thread_t *next = krlsched_select_thread();
	save_to_new_context(next, prev);
}

void krlschdclass_add_thread(thread_t* thdp)
{
	unsigned int cpuid = hal_retn_cpuid();
	schdata_t *schdap = &osschedcls.scls_schda[cpuid];  
	cpuflg_t cufg;
	
    hal_spinlock_saveflg_cli(&schdap->sda_lock,&cufg);
	list_add(&thdp->td_list, &schdap->sda_thdlst[thdp->td_priority].tdl_lsth);
	schdap->sda_thdlst[thdp->td_priority].tdl_nr++;
	schdap->sda_threadnr++;
	hal_spinunlock_restflg_sti(&schdap->sda_lock, &cufg);
	
	hal_spinlock_saveflg_cli(&osschedcls.scls_lock, &cufg);
    osschedcls.scls_threadnr++;
	hal_spinunlock_restflg_sti(&osschedcls.scls_lock, &cufg);
}

/**********************************
 *进程切换
 *********************************/
static void __to_new_context(thread_t *next, thread_t *prev)
{
	unsigned int cpuid = hal_retn_cpuid();
	schdata_t *schdap = &osschedcls.scls_schda[cpuid];
	schdap->sda_currtd = next;
	if (next->td_stus == TDSTUS_NEW) {
		next->td_stus = TDSTUS_RUN;
		retnfrom_first_sched(next);
	}
}

void save_to_new_context(thread_t* next,thread_t* prev)
{
    cpuflg_t cpuflg;
    hal_disableirq_savecpuflg(&cpuflg);
	/*保存SVC模式下的寄存器*/
    __asm__ __volatile__("stmfd sp!,{r0-r12,lr} \n\t"
         :::"memory"
    );
    __asm__ __volatile__(
		/*prev->td_context.ctx_svcspsr = spsrr*/
         "mrs lr,spsr\n\t"
         "str lr,[%[PREV_SPSR]] \n\t"
		 /*prev->td_context.ctx_cpsr = cpsr*/
         "mrs lr,cpsr\n\t"
         "str lr,[%[PREV_CPSR]] \n\t"
		 /*spsr = next->td_context.ctx_svcspsr*/
         "msr spsr,%[NEXT_SPSR] \n\t"
		 /*cpsr = next->td_context.ctx_cpsr*/
         "msr cpsr,%[NEXT_CPSR] \n\t"
		 /*prev->td_context.ctx_svcsp =  sp*/
         "str sp,[%[PREV_SVCSP]] \n\t"
		 /*sp = next->td_context.svcsp*/
         "ldr sp,[%[NEXT_SVCSP]] \n\t"
		 /*__to_new_context(next, prev)*/
         "mov r0,%[NEXT_TD] \n\t"
         "mov r1,%[PREV_TD] \n\t"
         "bl __to_new_context \n\t"
         :
         :[PREV_TD] "r" (prev), [NEXT_TD] "r" (next), 
         [PREV_SVCSP] "r" (&prev->td_context.ctx_svcsp),[PREV_SPSR] "r" (&prev->td_context.ctx_svcspsr),
         [PREV_CPSR] "r" (&prev->td_context.ctx_cpsr),
         [NEXT_SVCSP] "r" (&next->td_context.ctx_svcsp),[NEXT_SPSR] "r" (next->td_context.ctx_svcspsr),
         [NEXT_CPSR] "r" (next->td_context.ctx_cpsr)
         :"lr","cc","memory"
    );
	/*将next线程中内和栈中保存的寄存器值恢复到cpu中*/
    __asm__ __volatile__(
        "ldmfd sp!,{r0-r12,lr} \n\t"
         :::"memory"
    );
    hal_enableirq_restcpuflg(&cpuflg);
} 

void retnfrom_first_sched(thread_t* thrdp)
{
    __asm__ __volatile__(
         "msr spsr,%[svcspsr]\n\t"
         "mov sp,%[svcsp] \n\t"
         "mov lr,%[usrlr] \n\t"
         
         "ldmia sp,{r0-lr}^ \n\t"
         "add sp,sp,#60 \n\t"
         "movs pc,lr \n\t"
         :
         :[svcsp] "r" (thrdp->td_context.ctx_svcsp),[svcspsr] "r" (thrdp->td_context.ctx_svcspsr),
          [usrlr] "r" (thrdp->td_context.ctx_lr)
         : "cc","memory"
    );
}







 


