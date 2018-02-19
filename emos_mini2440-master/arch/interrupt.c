#include "interrupt.h"
#include "mach.h"
#include "cpuctrl.h"
#include "global.h"
#include "types.h"
#include "s3c2440.h"

static void intfltdsc_t_init(intfltdsc_t* initp,unsigned int flg,
			unsigned int stus, unsigned int pbitnr, unsigned int irqnr) 
{
	hal_spinlock_init(&initp->i_lock);
	list_init(&initp->i_serlist);
	initp->i_sernr = 0;
	initp->i_flg = flg;
	initp->i_stus = stus;
	initp->i_pndbitnr = pbitnr;
	initp->i_irqnr = irqnr;
	initp->i_deep = 0;
	initp->i_indx = 0;
}

static void init_intfltdsc(mach_t *machp)
{
	intfltdsc_t *ifdp = machp->mh_intfltdsc;
	unsigned int ifdnr = machp->mh_intfltnr;
	unsigned int i;
	
	if(ifdnr>IRQ_MAX) {
        hal_sysdie("init intflt err");
    }
	
	for(i = MINTNR_START; i < MINTNR_END; i++) {
		intfltdsc_t_init(&ifdp[i], MINT_FLG, 0, i - MINT_OFFSET, i);
	}
	
	for(i = SINTNR_START; i < SINTNR_END; i++) {
		intfltdsc_t_init(&ifdp[i], SINT_FLG, 0, i - SINT_OFFSET, i);
	}
	
	for(i = EINTNR_START; i < EINTNR_END; i++) {
		intfltdsc_t_init(&ifdp[i], EINT_FLG, 0, i - EINT_OFFSET, i);
	}
}

void init_intupt()
{
	init_intfltdsc(&osmach);
}

/**
 *根据中断号获取中断描述符
 */
intfltdsc_t *retn_intfltdsc(unsigned int ifdnr)
{
	if (ifdnr >= osmach.mh_intfltnr) {
		return NULL;
	}
	return &osmach.mh_intfltdsc[ifdnr];
}

/**
 *读取中断偏移寄存器读取中断号
 */
unsigned int retn_intnr()
{
	return (unsigned int)hal_io32_read(INTOFFSET_R);
}

/**
 *中断处理完了，必须清除这个中断源在终端挂起寄存器的
 *相关位，它才能相应下一次中断
 */
drvstus_t clear_srcpnd(unsigned int ifdnr) 
{
	unsigned int inttmp;
	unsigned int flg;
	unsigned int phylinenr;
	intfltdsc_t *ifdp = retn_intfltdsc(ifdnr);
	
	if (ifdp == NULL) {
		return DFCERRSTUS;
	}
	
	flg = ifdp->i_flg & 0xff;
	phylinenr = ifdp->i_pndbitnr;
	/*处理主中断*/
	if (flg == MINT_FLG) {
		inttmp = (1 << phylinenr);
		hal_io32_write(SRCPND_R, inttmp);
		return DFCOKSTUS;
	}
	
	/*次中断*/
	if (flg == SINT_FLG) {
		inttmp = (1 << phylinenr);
		hal_io32_write(SUBSRCPND_R, inttmp);
		return DFCOKSTUS;
	}
	
	/*外部中断*/
	if (flg == EINT_FLG) {
		inttmp = (1 << phylinenr);
		hal_io32_write(EINTPEND_R, inttmp);
		return DFCOKSTUS;
	}
	return DFCERRSTUS;	
}

static drvstus_t control_intline(unsigned int ifdnr, bool_t enable) 
{
	unsigned int inttmp;
	unsigned int flg;
	unsigned int phylinenr;
	intfltdsc_t *ifdp = retn_intfltdsc(ifdnr);
	
	if (ifdp == NULL) {
		return DFCERRSTUS;
	}
	
	flg = ifdp->i_flg & 0xff;
	phylinenr = ifdp->i_pndbitnr;
	
	if (flg == MINT_FLG) {
		inttmp = hal_io32_read(INTMSK_R);
		if (enable == TRUE) {
			inttmp &= (~(1 << phylinenr));
		} else {
			inttmp |= (1 << phylinenr);
		}
		hal_io32_write(INTMSK_R, inttmp);
		return DFCOKSTUS;
	}
	
	if (flg == SINT_FLG) {
		inttmp = hal_io32_read(INTSUBMSK_R);
		if (enable == TRUE) {
			inttmp &= (~(1 << phylinenr));
		} else {
			inttmp |= (1 << phylinenr);
		}
		hal_io32_write(INTSUBMSK_R, inttmp);
		return DFCOKSTUS;
	}
	
	if (flg == EINT_FLG) {
		inttmp = hal_io32_read(EINTMASK_R);
		if (enable == TRUE) {
			inttmp &= (~(1 << phylinenr));
		} else {
			inttmp |= (1 << phylinenr);
		}
		hal_io32_write(EINTMASK_R, inttmp);
		return DFCOKSTUS;
	}
	return DFCERRSTUS;
}

drvstus_t enable_intline(unsigned int ifdnr)
{
	return control_intline(ifdnr, TRUE);
}

drvstus_t disable_intline(unsigned int ifdnr)
{
	return control_intline(ifdnr, FALSE);
}

void intserdsc_t_init(intserdsc_t* initp , unsigned int flg,intfltdsc_t* intfltp,
					void* device, intflthandle_t handle)
{
    list_init(&initp->s_list);
    list_init(&initp->s_indevlst);
    initp->s_flg = flg;
    initp->s_intfltp = intfltp;
    initp->s_indx = 0;
    initp->s_device = device;
    initp->s_handle = handle;	
}

bool_t hal_add_ihandle(intfltdsc_t *intdscp, intserdsc_t *serdscp)
{
	if (intdscp == NULL || serdscp == NULL) {
		return FALSE;
	}
	
	cpuflg_t cpuflg;
	hal_spinlock_saveflg_cli(&intdscp->i_lock, &cpuflg);
	list_add(&serdscp->s_list, &intdscp->i_serlist);
	intdscp->i_sernr++;
	hal_spinunlock_restflg_sti(&intdscp->i_lock, &cpuflg);
	return TRUE;
}

