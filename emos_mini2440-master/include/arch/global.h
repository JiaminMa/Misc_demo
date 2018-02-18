#ifndef _HALGLOBAL_H
#define _HALGLOBAL_H
#include "mach.h"
#include "mm.h"
#include "interrupt.h"

#ifdef	HALGOBAL_HEAD
#undef	EXTERN
#define EXTERN
#endif

#define HAL_DEFGLOB_VARIABLE(vartype,varname) \
EXTERN  __attribute__((section(".data"))) vartype varname

HAL_DEFGLOB_VARIABLE(mach_t, osmach);
HAL_DEFGLOB_VARIABLE(phymem_t,osphymem);
HAL_DEFGLOB_VARIABLE(phyadrspce_t,machadrspce)[PLFM_ADRSPCE_NR];
HAL_DEFGLOB_VARIABLE(intfltdsc_t, machintflt)[IRQ_MAX];


#endif // HALGLOBAL_H
