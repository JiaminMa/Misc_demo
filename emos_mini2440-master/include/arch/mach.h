#ifndef _MACH_H_
#define _MACH_H_

#include "spinlock.h"
#include "list.h"
#include "mm.h"
#include "interrupt.h"

typedef struct s_MACH {
	spinlock_t mh_lock;
	list_h_t mh_list;
	unsigned int mh_kerinramstart;
	unsigned int mh_kerinramend;
	
	mmapdsc_t *mh_mmapdscadr; 			/*内存位图在内存中的起始地址*/
	unsigned int mh_mmapdscnr;			/*内存位图的个数*/
	phyadrspce_t *mh_spedsc;
	unsigned int mh_spedscnr;			/*物理地址空间描述符*/

#if 0
	ilnedsc_t *mh_ilnedsc;
	unsigned int mh_ilnedscnr;
#endif
	intfltdsc_t *mh_intfltdsc;			/*中断描述符*/
	unsigned int mh_intfltnr;

}mach_t;

void init_mach();
void init_mmapdsc(mach_t *mahp);
void print_mach(mach_t *mach_p);
void print_mmapdsc(mach_t *mchp);
#endif

