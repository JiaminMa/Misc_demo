/*
 *Created by Jiamin Ma
 *1.SMC instruction can be called only in secure world PL1.
 *  So in this demo, svc mode of sercur world is firstly set.
 *
 *2.Monitor mode has it own r13(sp), in this demo we firstly
 *  setup the stack base of monitor mode. The monitor mode in
 *  cpsr m[4:0] is "10110"
 *
 *3.Monitor mode has its own exception vectors and they are 
 *  stored in MVBAR register.
 *	The MVBAR register:
 *	|31 ---------............-----------5|4-------0|
 *  |Monitor mode exception vectors base |Resevered|
 *
 *  In this demo, I use the low 5 reserved bits like system call
 *  to achieve that 16 different smc handler when call smc.
 *  
 *4.The pseudocode how cpu work the smc instruction:
 *  Take SMCException()
 *		ITAdvanced();
 *		lr = cpsr.T == 1 ? pc : pc - 4; //Thumb related
 *		spsr = cpsr;
 *		cpsr.M = "10110";					  //Change to Monitor Mode
 *		cpsr.j = '0';
 *		cpsr.T = sctlr.TE
 *		cpsr.E = sctlr.EE
 *		cpsr.A = '1';
 *		cpsr.F = '1';
 *		cpsr.I = '1';
 *		cpsr.IT = '0000_0000'
 *		pc = monitor_vetors + 8;			
 *	In this demo, I just store the registers according to the ATPCS, LR
 *  and spsr in the smc instruction handle code.
 *  See it in monitor_mode function in this file. 	
 *
 *5.Runtime env:
 *  I just use qemu-system-arm with vexpress-a9 board to run this demo like this.
 *		qemu-system-arm -M vexpress-a9 -m 256M -nographic -kernel smc_test.bin
 *  And this command can debug the demo
 *		gdb server:		
 *			qemu-system-arm -M vexpress-a9 -m 256M -nographic -kernel smc_test.elf -s -S
 *		gdb client:
 *			arm-linux-gdb smc_test.elf -> target localhost:1234
 *
 *
 */

.align 4
.global reset
.global smc_handler
.global board_puthex
.global board_puts
.global board_putbin
.global test_in_smc_in_fun
.global call_smc
.global test_call_smc

reset:
	/*set the cpu to monitor mode and stack*/
	mrs r0, cpsr
	bic r0, r0, #0x1f
	orr r0, r0, #0xd6
	msr cpsr, r0
	ldr sp, =smc_stack_top

    /*Set the cpu to SVC32 mode*/
	/*Only secure world pl1 can change mvbar reg*/
    mrs r0, cpsr
    bic r0, r0, #0x1f
    orr r0, r0, #0xd3
    msr cpsr, r0
	ldr sp, =svc_stack_top
	
	/*Set up secure world using SCR reg*/	
	mrc p15, 0, r0, c1, c0, 0   @ Read CP15 SCR Register
    bic r0, #0
	mcr p15, 0, r0, c1, c0, 0   @ Write CP15 SCR Register
	
	/*Set up monitor mode vectors by MVBAR*/
	ldr r0, =__monitor_vector
    mcr p15, 0, r0, c12, c0, 1  @Set MVBAR	
	
//	mrs r0, cpsr
//    bl board_puthex
	/*Now let's test!*/
	/*1. call smc directly*/
	smc #0

	mrs r0, cpsr
    bl board_puthex

	
    bl c_entry

system_halt:
	b .

test_func_call_smc:

	smc #0		
	nop
	nop
	nop
	push {lr}	
	bl test_in_smc_in_fun	
	pop {lr}
	nop
	smc #0
	nop
	nop
	nop	
	movs pc, lr


    .align 4
__monitor_vector:
    @ Monitor-mode exception vector
    .word 0        /* reset */
    .word 0        /* undef */
    ldr pc, =monitor_mode
	.word 0 /* pabt */
    .word 0 /* dabt */
	.word 0
	.word 0 /* irq */
    .word 0 /* fiq */
    .align

monitor_mode:

	/*Save register according to ATPCS*/
	stmfd sp!, {r0-r3, r12, lr}	
	/*Save spsr register caused by smc #0 
	 *changes the spsr
	 */
	mrs r0, spsr
	;push {r0}
    bl board_puthex
	/*Real SMC Handler code*/
	ldr r0, [lr, #-4]
	bic r0, r0, #0xfffffff0
	bl smc_handler

	/*restore the spsr and regs of ATCPS*/
	;pop {r0}
	;msr spsr, r0
	ldmfd sp!, {r0-r3, r12, pc}^

call_smc:
	smc #0
	mov pc, lr
