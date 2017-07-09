.align 4
.global reset
.global svc_stack_top
reset:

    MRC p15,0,r0,c1,c0,2    // Read CP Access register
    ORR r0,r0,#0x00f00000   // Enable full access to NEON/VFP (Coprocessors 10 and 11)
    MCR p15,0,r0,c1,c0,2    // Write CP Access register
    ISB
    MOV r0,#0x40000000      // Switch on the VFP and NEON hardware
    MSR FPEXC,r0            // Set EN bit in 

    ldr sp, =svc_stack_top
    bl c_entry

system_halt:
	b .
