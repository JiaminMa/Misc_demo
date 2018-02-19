#include "types.h"
#include "cpuctrl.h"
#include "printk.h"
#include "arm920t.h"
#include "interrupt.h"
#include "s3c2440.h"
#include "list.h"
#include "printk.h"
#include "krlsched.h"

/**
 *1.查找中断描述符intfltdsc_t
 *2.在intfltdsc_t中查找intserdsc_t,调用处理函数
 *3.回到2继续处理
 */
static void hal_run_intflthandle(unsigned int ifdnr,void* sframe)
{
	intserdsc_t *isdscp;
	list_h_t *list;
	intfltdsc_t *ifdscp = retn_intfltdsc(ifdnr);
	if (ifdscp == NULL) {
		printk("hal_run_intfdsc err\n");
		hal_sysdie("hal_run_intfdsc err");
	}
	
	list_for_each(list, &ifdscp->i_serlist) {
		isdscp = list_entry(list, intserdsc_t, s_list);
		isdscp->s_handle(ifdnr, isdscp->s_device, sframe);
	}
}

drvstus_t hal_clear_intpnd(unsigned int bitnr)
{
     unsigned int inttmp = (1 << bitnr);
     hal_io32_write(INTPND_R,inttmp);   
     return DFCOKSTUS;
}

static void hal_eint_distr(void *sframe, unsigned int mintnr,
			unsigned int pndbts, unsigned int pndbte)
{

	unsigned int bi = pndbts;
	unsigned int pnd = hal_io32_read(EINTPEND_R);
	pnd &= EINTPEND_BITS_MASK;
	for(bi = pndbts; bi < pndbte; bi++) {
		if(((pnd >> bi) & 1) == 1) {
			hal_run_intflthandle(EINT_IFDNR(bi), sframe);
			clear_srcpnd(EINT_IFDNR(bi));
		}
	}

}

static void hal_sint_distr(void* sframe,unsigned int mintnr,
			unsigned int pndbts,unsigned int pndbte)
{
	unsigned int bi;
	unsigned int pnd = hal_io32_read(SUBSRCPND_R);
	pnd &= SUBSRCPND_BITS_MASK;
	for (bi = pndbts; bi < pndbte; bi++) {
		if (((pnd >> bi) &1) == 1) {
			hal_run_intflthandle(SINT_IFDNR(bi), sframe);
			clear_srcpnd(SINT_IFDNR(bi));
		}
	}
}

static void hal_int_distr(void* sframe,unsigned int mintnr)
{
	hal_run_intflthandle(MINT_IFDNR(mintnr), sframe);
	clear_srcpnd(MINT_IFDNR(mintnr));
}


void hal_dbug_print_reg(intstkregs_t* intstkp)
{
#if 0
    printk("USR_REG r0:%x\n\r",intstkp->r0);
    printk("USR_REG r1:%x\n\r",intstkp->r1);
    printk("USR_REG r2:%x\n\r",intstkp->r2);
    printk("USR_REG r3:%x\n\r",intstkp->r3);
    printk("USR_REG r4:%x\n\r",intstkp->r4);
    printk("USR_REG r5:%x\n\r",intstkp->r5);
    printk("USR_REG r6:%x\n\r",intstkp->r6);
    printk("USR_REG r7:%x\n\r",intstkp->r7);
    printk("USR_REG r8:%x\n\r",intstkp->r8);
    printk("USR_REG r9:%x\n\r",intstkp->r9);
    printk("USR_REG r10:%x\n\r",intstkp->r10);
    printk("USR_REG r11:%x\n\r",intstkp->r11);
    printk("USR_REG r12:%x\n\r",intstkp->r12);
    printk("USR_REG r13:%x\n\r",intstkp->r13);
    printk("USR_REG r14:%x\n\r",intstkp->r14);
    printk("SVE_REG lr:%x\n\r",intstkp->s_lr);
    printk("SVE_REG spsr:%x\n\r",intstkp->s_spsr);
    printk("CSP_REG sp:%x INTPND:%x\n\r", hal_read_currmodesp(),hal_io32_read(INTPND_R));
    printk("CCR_REG cpsr:%x INTOFST:%x\n\r",hal_read_cpuflg(),hal_retn_intnr());
    thread_t* prev=krlsched_retn_currthread();
    
    printk("CURR_THREAD:%x CURR_THREAD_KSTKTOP:%x\n\r",(unsigned int)prev,(unsigned int)prev->td_krlstktop);

#endif
}

void handle_undefins_distr(void *sframe)
{
	hal_sysdie("undefines runing!");
}

sysstus_t handle_swi_distr(unsigned int swinr, void *sframe)
{
//	return krlservice(swinr, sframe);
	return 0;
}

void handle_prefabt_distr (void *sframe)
{
#if 0
    printk("CP15 c5:%x c6:%x\n\r",
    hal_read_cp15regs(CP15_C5R),hal_read_cp15regs(CP15_C6R));
    hal_dbug_print_reg((intstkregs_t*)sframe);
#endif
    hal_sysdie("pref abt runing!! ");
}

void handle_dataabt_distr (void *sframe)
{
#if 0
    printk("CP15 c5:%x c6:%x\n\r",
    hal_read_cp15regs(CP15_C5R),hal_read_cp15regs(CP15_C6R));
    hal_dbug_print_reg((intstkregs_t*)sframe);
#endif
    hal_sysdie("data abt runing!! ");
}

void handle_bugdie_distr(void *sframe)
{
    hal_sysdie("bug die runing!! ");
}

void handle_irq_distr(void *sframe)
{
	unsigned int intoset = retn_intnr();
	switch(intoset) {
        case EINT4_7:
            hal_eint_distr(sframe,intoset,EI4_7_PNDBTS,EI4_7_PNDBTE);
            break;
        case EINT8_23:
            hal_eint_distr(sframe,intoset,EI8_23_PNDBTS,EI8_23_PNDBTE);
            break;
		case INT_CAM:
            hal_sint_distr(sframe,intoset,ICAM_PNDBTS,ICAM_PNDBTE);
            break;
        case INT_WDT_AC97:
            hal_sint_distr(sframe,intoset,IACWDT_PNDBTS,IACWDT_PNDBTE);
            break;
        case INT_UART2:
            hal_sint_distr(sframe,intoset,IUART2_PNDBTS,IUART2_PNDBTE);
            break;
        
        case INT_UART1:
            hal_sint_distr(sframe,intoset,IUART1_PNDBTS,IUART1_PNDBTE);
            break;
        case INT_UART0:
            hal_sint_distr(sframe,intoset,IUART0_PNDBTS,IUART0_PNDBTE);
            break;    
        case INT_ADC:
            hal_sint_distr(sframe,intoset,IADC_PNDBTS,IADC_PNDBTE);
            break;
        default:
            hal_int_distr(sframe,intoset);
            break;  
	}
	hal_clear_intpnd(intoset);
	krlsched_chkneed_pmptsched();
// 	krlsched_retn_currthread();
}

void handle_frq_distr(void *sframe)
{
    hal_sysdie("frq runing!! ");	
}






