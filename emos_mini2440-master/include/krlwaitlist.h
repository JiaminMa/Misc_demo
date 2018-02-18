#ifndef _KRLWAITLIST_H
#define _KRLWAITLIST_H
#include "sem.h"
#include "krlthread.h"
void kwlst_t_init(kwlst_t* initp);
void krlwlst_wait(kwlst_t* wlst);
void krlwlst_up(kwlst_t* wlst);
void krlwlst_allup(kwlst_t* wlst);
void krlwlst_add_thread(kwlst_t* wlst,thread_t* tdp);
thread_t* krlwlst_del_thread(kwlst_t *wlst);
#endif // KRLWAITLIST_H
