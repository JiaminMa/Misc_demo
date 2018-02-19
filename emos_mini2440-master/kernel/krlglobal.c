#define KRLGOBAL_HEAD
#include "krlglobal.h"
#include "drvtick.h"
#include "drvfs.h"
KRL_DEFGLOB_VARIABLE(kmempool_t,oskmempool);
KRL_DEFGLOB_VARIABLE(devtable_t, osdevtable);
KRL_DEFGLOB_VARIABLE(drventyexit_t,osdrvetytabl)[]={
//systick_entry,uart_entry,rtc_entry,rfs_entry,
systick_entry, rfs_entry, NULL};
KRL_DEFGLOB_VARIABLE(schedclass_t, osschedcls);
