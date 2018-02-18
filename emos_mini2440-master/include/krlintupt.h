#ifndef _KERNEL_INTERRUPT_H_
#define _KERNEL_INTERRUPT_H_
#include "interrupt.h"
intserdsc_t *krladd_irqhandle(void *device, intflthandle_t handle,
					unsigned int phyline);
drvstus_t krlenable_intline(unsigned int ifdnr);
drvstus_t krldisable_intline(unsigned int ifdnr);
#endif
