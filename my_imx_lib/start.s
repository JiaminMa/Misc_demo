.global reset
.global stack_top
.global loader_start

.align 11
el3_exception_vector:
                            //Synchronous El1t
    b reset
    .align 7
    b .                     //IRQ El1t
    .align 7
    b .                     //FIQ El1t
    .align 7
    b .                     //Error El1t
    .align 7

    b .
    .align 7
    b .                     //IRQ El1h
    .align 7
    b .                     //FIQ El1h
    .align 7
    b .                     //Error El1h
    .align 7

    b .                     //Synchronous 64-bit El0
    .align 7
    b .                     //IRQ 64-bit EL0
    .align 7
    b .                     //FIQ 64-bit EL0
    .align 7
    b .                     //Error 64-bit EL0
    .align 7

    b .                     //Synchronous 32-bit El0
    .align 7
    b .                     //IRQ 32-bit EL0
    .align 7
    b .                     //FIQ 32-bit EL0
    .align 7
    b .                     //Error 32-bit EL0
    .align 7

.align 4

reset:
    mov x0, #0
    mov x1, x0
    mov x2, x0
    mov x3, x0
    mov x4, x0
    mov x5, x0
    mov x6, x0
    mov x7, x0
    mov x8, x0
    mov x9, x0
    mov x10, x0
    mov x11, x0
    mov x12, x0
    mov x13, x0
    mov x14, x0
    mov x15, x0
    mov x16, x0
    mov x17, x0
    mov x18, x0
    mov x19, x0
    mov x20, x0
    mov x21, x0
    mov x22, x0
    mov x23, x0
    mov x24, x0
    mov x25, x0
    mov x26, x0
    mov x27, x0
    mov x28, x0
    mov x29, x0
    mov x30, x0

    ldr x0, =el3_exception_vector
    msr vbar_el3, x0

    mov x0, #0x01
    msr spsel, x0
    ldr w0, =stack_top
    mov sp, x0

    b loader_start

    b .
