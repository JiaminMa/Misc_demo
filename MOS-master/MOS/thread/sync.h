#ifndef _THREAD_SYNC_H_
#define _THREAD_SYNC_H_
#include "list.h"
#include "stdint.h"
#include "thread.h"

struct semaphore {
	uint8_t value;
	struct list waiters;
};

struct lock {
	struct task_struct *holder;
	struct semaphore semaphore;
	uint32_t holder_repeat_nr;
};

void sema_init(struct semaphore *, uint8_t);
void lock_init(struct lock *);
void sema_down(struct semaphore *);
void sema_up(struct semaphore *);
void lock_acquire(struct lock *);
void lock_release(struct lock *);

#endif
