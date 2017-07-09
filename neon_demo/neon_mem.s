.align 4
.global memcpy_neon

memcpy_neon:
    mov r0, #1
//    VLDM r1!,{d0}
//    VSTM r0!,{d0}
    VADD.I8 D0, D1, D2
    mov pc, lr
