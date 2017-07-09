.align 4
.global reset
.global svc_stack_top
reset:

    ldr sp, =svc_stack_top
    bl boot_entry

system_halt:
	b .
