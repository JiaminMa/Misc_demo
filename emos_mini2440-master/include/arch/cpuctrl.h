#ifndef _CPUCTRL_H
#define _CPUCTRL_H
#include "spinlock.h"
#include "types.h"

void hal_disable_fiq();
void hal_enable_fiq();
void hal_disable_irq();
void hal_enable_irq();
void hal_disable_irqfiq();
void hal_enable_irqfiq();
void hal_disablefiq_savecpuflg(cpuflg_t* cpuflg);
void hal_enablefiq_restcpuflg(cpuflg_t* cpuflg);
void hal_disableirq_savecpuflg(cpuflg_t* cpuflg);
void hal_enableirq_restcpuflg(cpuflg_t* cpuflg);
void hal_disableirqfiq_savecpuflg(cpuflg_t* cpuflg);
void hal_enableirqfiq_restcpuflg(cpuflg_t* cpuflg);
unsigned int hal_read_currmodesp();
cpuflg_t hal_read_cpuflg();
void hal_write_cpuflg(cpuflg_t cpuflg);
void hal_swhmodset_sp_rscurmod(unsigned int cpumod,reg_t sp);
cpuflg_t hal_read_scpuflg();
void hal_write_scpuflg(cpuflg_t scpuflg);
void hal_cpumode_switch(unsigned int mode);
unsigned int hal_cpumodeswitch_retoldmode(unsigned int mode);
unsigned char hal_io8_read(unsigned int ioadr);
unsigned short hal_io16_read(unsigned int ioadr);
unsigned int hal_io32_read(unsigned int ioadr);
void hal_io8_write(unsigned int ioadr,unsigned char val);
void hal_io16_write(unsigned int ioadr,unsigned short val);
void hal_io32_write(unsigned int ioadr,unsigned int val);
void hal_spinlock_init(spinlock_t* lock);
void hal_spinlock_lock(spinlock_t* lock);
void hal_spinlock_unlock(spinlock_t* lock);
void hal_spinlock_saveflg_cli(spinlock_t* lock, cpuflg_t* cpuflg);
void hal_spinunlock_restflg_sti(spinlock_t* lock, cpuflg_t* cpuflg);
void hal_memset(void* setp,size_t n,unsigned char setval);
void hal_memcpy(void* src,void* dst,size_t n);
void hal_sysdie(char* errmsg);
unsigned int hal_retn_cpuid();

#define CPU_USR_MODE 0x10
#define CPU_FIQ_MODE 0x11
#define CPU_IRQ_MODE 0x12
#define CPU_SVE_MODE 0x13
#define CPU_ABT_MODE 0x17
#define CPU_UND_MODE 0x1b
#define CPU_SYS_MODE 0x1f

#define CFIQ 0x40
#define CIRQ 0x80
#define CIRQFIQ 0xc0

#endif // CPUCTRL_H
