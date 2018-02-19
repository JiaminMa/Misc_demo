#ifndef _KERNEL_THREAD_H_
#define _KERNEL_THREAD_H_
#include "types.h"
#include "spinlock.h"
#include "krldevice.h"

#define TDSTUS_RUN 0
#define TDSTUS_SLEEP 3
#define TDSTUS_WAIT 4
#define TDSTUS_NEW 5
#define TDSTUS_ZOMB 6

#define TDRUN_TICK 20
#define TD_HAND_MAX 8
#define CPUCORE_MAX 1

#define DAFT_TDUSRSTKSZ 0x8000
#define DAFT_TDKRLSTKSZ 0x8000

#define PRITY_MAX 64
#define PRITY_MIN 0
#define PRILG_SYS 0
#define PRILG_USR 5

#define DAFT_SPSR 0x10
#define DAFT_CPSR 0xd3
#define DAFT_CIDLESPSR 0x13

typedef struct s_CONTEXT{
	reg_t ctx_usrsp;
	reg_t ctx_svcsp;
	reg_t ctx_svcspsr;
	reg_t ctx_cpsr;
	reg_t ctx_lr;
}context_t;

typedef struct s_THREAD {
	spinlock_t td_lock;
	list_h_t td_list;
	unsigned int td_flgs;
	unsigned int td_stus;
	unsigned int td_cpuid;
	unsigned int td_id;
	unsigned int td_tick;
	unsigned int td_privilege;
	unsigned int td_priority;
	unsigned int td_runmode;
	adr_t td_krlstktop;	/*进程内核栈顶指针*/
	adr_t td_krlstkstart;
	adr_t td_usrstktop;
	adr_t td_usrstkstart;
	void *td_mmdsc;
	void *td_resdsc;
	void *td_privtep;
	void *td_extdatap;
	context_t td_context;
	objnode_t *td_handtbl[TD_HAND_MAX];
	char *name;
}thread_t;

typedef struct s_THRDLIST
{
	list_h_t tdl_lsth;
	thread_t *tdl_curruntd;
	unsigned int tdl_nr;
}thrdlst_t;

typedef struct s_SCHDATA {
	spinlock_t sda_lock;
	unsigned int sda_cpuid;
	unsigned int sda_schdflgs;
	unsigned int sda_premptidx;
	unsigned int sda_threadnr;
	unsigned int sda_prityidx;
	thread_t *sda_cpuidle;
	thread_t *sda_currtd;
	thrdlst_t sda_thdlst[PRITY_MAX];
}schdata_t;

typedef struct s_SCHEDCLASS{
	spinlock_t scls_lock;
	unsigned int scls_cpunr;
	unsigned int scls_threadnr;
	unsigned int scls_threadid_inc;
	schdata_t scls_schda[CPUCORE_MAX];
}schedclass_t;

void krlthreadkrlsatck_init(thread_t *thdp, void *runadr, reg_t cpsr, reg_t spsr);
thread_t* krlnew_thread_dsc();
void thread_t_init(thread_t *nitp);
thread_t *krlnew_thread(void *filerun, unsigned int flg, unsigned int prilg,
					unsigned int prity, size_t usrstksz, size_t krlstksz);
void krlthd_inc_tick(thread_t *thdp);
thread_t *krlsched_retn_currthread();
#endif
