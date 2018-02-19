#ifndef _KERNEL_GLOBAL_H_
#define _KERNEL_GLOBAL_H_

#include "krlpagempol.h"
#include "krldevice.h"
#include "krlthread.h"
#ifdef	KRLGOBAL_HEAD
#undef	KEXTERN
#define KEXTERN
#endif

#define KRL_DEFGLOB_VARIABLE(vartype,varname) \
__attribute__((section(".data"))) vartype varname

extern KRL_DEFGLOB_VARIABLE(kmempool_t, oskmempool);
extern KRL_DEFGLOB_VARIABLE(devtable_t, osdevtable);
extern KRL_DEFGLOB_VARIABLE(drventyexit_t,osdrvetytabl)[];
extern KRL_DEFGLOB_VARIABLE(schedclass_t, osschedcls);
#endif

