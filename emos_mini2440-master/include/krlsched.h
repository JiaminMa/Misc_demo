#ifndef _KERNEL_SCHED_H_
#define _KERNEL_SCHED_H_

#include "krlthread.h"

#define NOTS_SCHED_FLGS 0
#define NEED_SCHED_FLGS 1
#define PMPT_FLGS 0

void thrdlst_t_init(thrdlst_t *initp);
void schdata_t_init(schdata_t *initp);
void schedclass_t_init(schedclass_t *initp);
void init_krlsched();
void retnfrom_first_sched(thread_t* thrdp);
void krlschdclass_add_thread(thread_t* thdp);
void krlsched_set_schedflgs();
void krlsched_chkneed_pmptsched();
thread_t *krlsched_select_thread();
thread_t *krlsched_retn_idlethread();
void save_to_new_context(thread_t* next,thread_t* prev);
void krlschedul();
void krlsched_wait(kwlst_t *wlst);
void krlsched_up(kwlst_t *wlst);
#endif
