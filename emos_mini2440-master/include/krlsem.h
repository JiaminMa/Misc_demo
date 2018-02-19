#ifndef _KRLSEM_H
#define _KRLSEM_H
#include "sem.h"
void krlsem_t_init(sem_t* initp);
void krlsem_set_sem(sem_t* setsem,unsigned int flg,int conut);
void krlsem_down(sem_t* sem);
void krlsem_up(sem_t* sem);
#endif // KRLSEM_H
