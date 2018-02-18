#ifndef _SPINLOCK_H
#define _SPINLOCK_H

typedef struct
{
	 volatile unsigned int lock;
} spinlock_t;
#endif
