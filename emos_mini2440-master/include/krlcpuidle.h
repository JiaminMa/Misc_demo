#ifndef _KERNEL_CPU_IDLE_H_
#define _KERNEL_CPU_IDLE_H_

#include "krlthread.h"
#include "types.h"

void new_cpuidle();
void krlcpuidle_main();
void krlcpuidle_start();
void init_krlcpuidle();
#endif
