.equ WTCON,0x53000000
.equ INTMASK,0x4a000008
.equ INTSUBMASK,0x4a00001c
.equ INIT_HEAD_STACK_ADDR, 0x34000000

.section .head.text
.code 32
.extern lmosemhal_start
.global _start
.global lmde
.align 4

_start:
    /*关闭irq和fiq*/
    msr cpsr, #0xc0|0x13
    
    /*关闭看门狗*/
    ldr r0, =WTCON
    mov r1, #0
    str r1, [r0]

    /*屏蔽中断*/
    ldr r0, =INTMASK
    ldr r1, =0xffffffff
    str r1, [r0]

    /*屏蔽所有子中断*/
    ldr r0, =INTSUBMASK
    ldr r1, = 0x7fff
    str r1, [r0]
    ldr r0, tstw
    ldr sp, =INIT_HEAD_STACK_ADDR
    bl kernel_start

halt:
    b halt

.global tstw

tstw: .word __end_lmosem_hal_vector 
    
