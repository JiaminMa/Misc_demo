#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "spinlock.h"
#include "list.h"

#define IRQ_MAX 71
#define MINT_OFFSET 0
#define SINT_OFFSET 32
#define EINT_OFFSET 47

#define MINTNR_START (0+MINT_OFFSET)
#define MINTNR_END (31+MINT_OFFSET)

#define SINTNR_START (0+SINT_OFFSET)
#define SINTNR_END (14+SINT_OFFSET)

#define EINTNR_START (0+EINT_OFFSET)
#define EINTNR_END (23+EINT_OFFSET)

#define MINT_FLG 1
#define SINT_FLG 2
#define EINT_FLG 3

#define EINT4_7 4
#define EINT8_23 5
#define INT_CAM 6
#define INT_WDT_AC97 9
#define INT_UART2 15
#define INT_LCD 16
#define INT_UART1 23
#define INT_UART0 28
#define INT_ADC 31
#define EINTPEND_BITS_MASK 0xffffff

#define EI4_7_PNDBTS 4
#define EI4_7_PNDBTE 8

#define EI8_23_PNDBTS 8
#define EI8_23_PNDBTE 24

#define IUART0_PNDBTS 0
#define IUART0_PNDBTE 3

#define IUART1_PNDBTS 3
#define IUART1_PNDBTE 6

#define IUART2_PNDBTS 6
#define IUART2_PNDBTE 9

#define IADC_PNDBTS 9
#define IADC_PNDBTE 11

#define ICAM_PNDBTS 11
#define ICAM_PNDBTE 13

#define IACWDT_PNDBTS 13
#define IACWDT_PNDBTE 15

#define ILCD_PNDBTS 0
#define ILCD_PNDBTE 2

#define MINT_OFFSET 0
#define MINT_IFDNR(x) (x+MINT_OFFSET)
#define SINT_IFDNR(x) (x+SINT_OFFSET)
#define EINT_IFDNR(x) (x+EINT_OFFSET)
#define SUBSRCPND_BITS_MASK 0x7fff

/**
 *ift_nr:中断号，device：设备指针，sframe：中断栈指针
 */
typedef drvstus_t (*intflthandle_t)(unsigned int ift_nr, 
	void * device, void *sframe);

typedef struct s_INTFLTDSC {
	spinlock_t i_lock;
	list_h_t i_serlist; 	/*设备中断服务程序表*/
	unsigned int i_sernr;	/*设备终端服务程序个数*/
	unsigned int i_flg;	
	unsigned int i_stus;
	unsigned int i_pndbitnr;	/*Pending bit number*/
	unsigned int i_irqnr;
	unsigned int i_deep;
	unsigned int i_indx;
}intfltdsc_t; /*中断源描述符*/

/*描述一个中断服务*/
typedef struct s_INTSERDSC {
	list_h_t s_list; 		/*该链表挂载intfltdsc_t的i_serlist中*/
	list_h_t s_indevlst;	/*该链表挂载设备描述中的相关链表上*/
	unsigned int s_flg;
	intfltdsc_t *s_intfltp;
	void *s_device;
	unsigned int s_indx;
	intflthandle_t s_handle;
}intserdsc_t;

void init_intupt(void);
intfltdsc_t *retn_intfltdsc(unsigned int ifdnr);
unsigned int retn_intnr();
drvstus_t clear_srcpnd(unsigned int ifdnr);
drvstus_t enable_intline(unsigned int ifdnr);
drvstus_t disable_intline(unsigned int ifdnr);
void intserdsc_t_init(intserdsc_t* initp , unsigned int flg,intfltdsc_t* intfltp,
					void* device, intflthandle_t handle);
bool_t hal_add_ihandle(intfltdsc_t *intdscp, intserdsc_t *serdscp);					
#endif
