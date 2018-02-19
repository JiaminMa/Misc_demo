.section .lmosem_handle.intvect,"ax"
.code 32
.align 4
.extern handle_undefins_distr
.extern handle_swi_distr
.extern handle_prefabt_distr
.extern handle_dataabt_distr
.extern handle_bugdie_distr
.extern handle_irq_distr
.extern handle_frq_distr
.global handle_undefins
.global handle_swi
.global handle_prefabt
.global handle_dataabt
.global handle_bugdie
.global handle_irq
.global handle_frq

.equ ALLREGS_STKSZ, 64
.equ STKOFFSET_PC, 60
.equ NPCREGS_STKSZ,60
 
.equ CPUMODE_MASK,31
.equ CPUSVE_MODE,19
.equ MSTK_OFF,12
.equ NINT_SVEMODE,0xd3
.equ NINT_IRQMODE,0xd2
.equ NINT_FRQMODE,0xd1
.equ NINT_ABTMODE,0xd7
.equ NINT_UNDMODE,0xdb

handle_undefins:
	msr   cpsr_c,#NINT_SVEMODE
	sub   sp,sp,#ALLREGS_STKSZ
	stmia sp,{r0-lr}^
	
	msr   cpsr_c,#NINT_UNDMODE
	sub   r1,lr,#0
	mrs   r0,spsr
	msr   cpsr_c,#NINT_SVEMODE
	str   r1,[sp,#STKOFFSET_PC]
	stmfd sp!,{r0,lr}
	
	mov   r0,sp
	bl    handle_undefins_distr
	
	ldmfd sp!,{r0,lr}
	msr   spsr,r0
	ldmia sp,{r0-lr}^
	add   sp,sp,#NPCREGS_STKSZ
	ldmfd sp!,{pc}^
	
handle_swi:
	sub   sp,sp,#ALLREGS_STKSZ
	stmia sp,{r1-lr}^
	mrs   r1,spsr
	str   lr,[sp,#STKOFFSET_PC]
	stmfd sp!,{r1,lr}
	mov   r1,sp
	bl    handle_swi_distr
	ldmfd sp!,{r1,lr}
	msr   spsr,r1
	ldmia sp,{r1-lr}^
	add   sp,sp,#NPCREGS_STKSZ
	ldmfd sp!,{pc}^
	
handle_prefabt:
	msr   cpsr_c,#NINT_SVEMODE
	sub   sp,sp,#ALLREGS_STKSZ
	stmia sp,{r0-lr}^
	msr   cpsr_c,#NINT_ABTMODE
	sub   r1,lr,#4
	mrs   r0,spsr
	msr   cpsr_c,#NINT_SVEMODE
	str   r1,[sp,#STKOFFSET_PC]
	stmfd sp!,{r0,lr}
	mov   r0,sp
	bl    handle_prefabt_distr
	ldmfd sp!,{r0,lr}
	msr   spsr,r0
	ldmia sp,{r0-lr}^
	add   sp,sp,#NPCREGS_STKSZ
	ldmfd sp!,{pc}^
        
handle_dataabt:
	msr   cpsr_c,#NINT_SVEMODE
	sub   sp,sp,#ALLREGS_STKSZ
	stmia sp,{r0-lr}^
	msr   cpsr_c,#NINT_ABTMODE
	sub   r1,lr,#8
	mrs   r0,spsr
	msr   cpsr_c,#NINT_SVEMODE
	str   r1,[sp,#STKOFFSET_PC]
	stmfd sp!,{r0,lr}
	mov   r0,sp
	bl    handle_dataabt_distr
	ldmfd sp!,{r0,lr}
	msr   spsr,r0
	ldmia sp,{r0-lr}^
	add   sp,sp,#NPCREGS_STKSZ
	ldmfd sp!,{pc}^
	
handle_bugdie:
    b .
	
handle_irq:
	msr   cpsr_c,#NINT_SVEMODE
	sub   sp,sp,#ALLREGS_STKSZ
	stmia sp,{r0-lr}^
	msr   cpsr_c,#NINT_IRQMODE
	sub   r1,lr,#4
	mrs   r0,spsr
	msr   cpsr_c,#NINT_SVEMODE
	str   r1,[sp,#STKOFFSET_PC]
	stmfd sp!,{r0,lr}
	mov   r0,sp
	bl    handle_irq_distr
	ldmfd sp!,{r0,lr}
	msr   spsr,r0
	ldmia sp,{r0-lr}^
	add   sp,sp,#NPCREGS_STKSZ
	ldmfd sp!,{pc}^
handle_frq:
	msr   cpsr_c,#NINT_SVEMODE
	sub   sp,sp,#ALLREGS_STKSZ
	stmia sp,{r0-lr}^
	msr   cpsr_c,#NINT_FRQMODE
	sub   r1,lr,#4
	mrs   r0,spsr
	msr   cpsr_c,#NINT_SVEMODE
	str   r1,[sp,#STKOFFSET_PC]
	stmfd sp!,{r0,lr}
	mov   r0,sp
	bl    handle_frq_distr
	ldmfd sp!,{r0,lr}
	msr   spsr,r0
	ldmia sp,{r0-lr}^
	add   sp,sp,#NPCREGS_STKSZ
	ldmfd sp!,{pc}^


