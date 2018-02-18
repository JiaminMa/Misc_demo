#include "krlcpuidle.h"
#include "cpuctrl.h"
#include "printk.h"
#include "krlmm.h"
#include "krlglobal.h"
#include "krlsched.h"

static thread_t *new_cpuidle_thread()
{
	thread_t *ret_td = NULL;
	bool_t acs = FALSE;
	size_t usrstksz = DAFT_TDUSRSTKSZ;
	size_t krlstksz = DAFT_TDKRLSTKSZ;
	adr_t usrstkadr = NULL;
	adr_t krlstkadr = NULL;
	
	usrstkadr = krlnew(usrstksz);
	if (usrstkadr == NULL) {
		return NULL;
	}

	krlstkadr = krlnew(krlstksz);
	if (krlstkadr == NULL) {
		if (krldelete(usrstkadr, usrstksz) == FALSE) {
			return NULL;
		}
		return NULL;
	}

	ret_td = krlnew_thread_dsc();
	if (ret_td == NULL) {
		acs = krldelete(usrstkadr, usrstksz);
		acs = krldelete(krlstkadr, krlstksz);
		if (acs == FALSE) {
			return NULL;
		} 
		return NULL;
	}

	ret_td->td_privilege = PRILG_USR;
	ret_td->td_priority = PRITY_MAX;
	ret_td->td_krlstktop = krlstkadr + (adr_t)(krlstksz - 1);
	ret_td->td_krlstkstart = krlstkadr;
	ret_td->td_usrstktop = usrstkadr + (adr_t)(usrstksz - 1);
	ret_td->td_usrstkstart = usrstkadr;
    krlthreadkrlsatck_init(ret_td, (void*)krlcpuidle_main, DAFT_CPSR, DAFT_CIDLESPSR);
	unsigned int cpuid = hal_retn_cpuid();
	schdata_t *schdap = &osschedcls.scls_schda[cpuid];
	schdap->sda_cpuidle = ret_td;
	schdap->sda_currtd = ret_td;
	return ret_td;
}

void new_cpuidle()
{
	thread_t *thp = new_cpuidle_thread();
	thp->name = "cpuidle";
	if (thp == NULL) {
		hal_sysdie("newcpuidle err");
	}
	
	printk("CPU IDLE TASK:%x\n", (unsigned int)thp);
}

void krlcpuidle_main()
{
	for(;;) {
		printk("cpuidle is run !!\n");
		krlschedul();
	}
}

void krlcpuidle_start()
{
	unsigned int cpuid = hal_retn_cpuid();
	schdata_t *schdap = &osschedcls.scls_schda[cpuid];
	thread_t *tdp = schdap->sda_cpuidle;
	tdp->td_stus = TDSTUS_RUN;
	retnfrom_first_sched(tdp);
}

void init_krlcpuidle()
{
	new_cpuidle();
	krlcpuidle_start();
}
