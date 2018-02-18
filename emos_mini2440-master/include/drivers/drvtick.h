#ifndef _DRV_TICK_H_
#define _DRV_TICK_H_
#include "types.h"
#include "krldevice.h"
#include "interrupt.h"
#include "s3c2440.h"

#define SYSTICK_PHYINTLINE 14

#define SYSTICK_PHYINTLINE 14

drvstus_t systick_entry(driver_t *drvp, unsigned int val, void *p);
drvstus_t systick_exit(driver_t *drvp, unsigned int val, void *p);
drvstus_t systick_handle(unsigned int ift_nr, void * devp, void *sframe);
drvstus_t systick_open(device_t *devp, void *iopack);
drvstus_t systick_close(device_t *devp, void *iopack);
drvstus_t systick_read(device_t *devp, void *iopack);
drvstus_t systick_write(device_t *devp, void *iopack);
drvstus_t systick_lseek(device_t *devp, void *iopack);
drvstus_t systick_ioctrl(device_t *devp, void *iopack);
drvstus_t systick_start(device_t *devp, void *iopack);
drvstus_t systick_stop(device_t *devp, void *iopack);
drvstus_t systick_powerstus(device_t *devp, void *iopack);
drvstus_t systick_enum_dev(device_t *devp, void *iopack);
drvstus_t systick_flush(device_t *devp, void *iopack);
drvstus_t systick_shutdown(device_t *devp, void *iopack);

#endif
