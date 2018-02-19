.section .handle.vector,"ax"
.code 32
.align 0

.extern handle_undefins
.extern handle_swi
.extern handle_prefabt
.extern handle_dataabt
.extern handle_bugdie
.extern handle_irq
.extern handle_frq
.extern _start

.global vector
vector:
        ldr pc,restart_l
        ldr pc,undefins_l
        ldr pc,swi_l
        ldr pc,prefabt_l
        ldr pc,dataabt_l
        ldr pc,bugdie_l
        ldr pc,irq_l
        ldr pc,frq_l
        nop
        nop
        nop
        nop
 _err:
        b   _start

.align 4
restart_l:  .word _start
undefins_l: .word handle_undefins
swi_l:      .word handle_swi
prefabt_l:  .word handle_prefabt
dataabt_l:  .word handle_dataabt
bugdie_l:   .word handle_bugdie
irq_l:      .word handle_irq
frq_l:      .word handle_frq


.end
