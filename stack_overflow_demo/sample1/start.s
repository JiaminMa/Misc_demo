    CODE32
    IMPORT c_entry
    EXPORT _Reset
    PRESERVE8
    AREA start, CODE, READONLY
_Reset
        LDR sp, =0x60020000
        BL c_entry
        B .

    END
