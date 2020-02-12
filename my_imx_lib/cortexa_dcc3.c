/****************************************************************
 *
 *         Copyright: 2017 NXP, INC. All Rights Reserved.
 *
 ***************************************************************/

/*
 * Copyright (C) 2011, Freescale Semiconductor, Inc. All Rights Reserved
 * THIS SOURCE CODE IS CONFIDENTIAL AND PROPRIETARY AND MAY NOT
 * BE USED OR DISTRIBUTED WITHOUT THE WRITTEN PERMISSION OF
 * Freescale Semiconductor, Inc.
*/

/*!
 * @file    cortexa_dcc3.c
 * @brief   dcc channel driver for cortex-a
 */

#define ARCH_ARMV8_A
#if defined(ARCH_ARMV7_A)
unsigned int dcc3_get_term_status(void)
{
    unsigned int status = 0;

#if defined(__ARMCC_VERSION)
    __asm { MRC p14, 0, status, c0, c1; }
#else
    __asm__ volatile ("mrc 14, 0, %0, c0, c1":"=r" (status));
#endif

    return status;
}
#endif

#if defined(ARCH_ARMV7_A) || defined(ARCH_ARMV8_A)
void dcc3_put_block(const unsigned char *data, int size)
{
    int len;
#if defined(ARCH_ARMV8_A)
    unsigned long long ch, k;
#else
    unsigned long ch, k;
#endif

    while (size > 0) {
        len = size;
        if (len > 3)
            len = 3;
        ch = data[0] | (data[1] << 8) | (data[2]) << 16 | ((len - 1) << 24);

        do {                    /* Wait for Terminal Ready */
#if defined(__ARMCC_VERSION)
        #if defined(ARCH_ARMV8_A)
            __asm ("MRS %0, mdscr_el1" : "=r" (k));
        #else
            __asm { MRC p14, 0, k, c0, c1; }
        #endif /* defined(ARCH_ARMV8_A) */
#else
        #if defined(ARCH_ARMV8_A)
            __asm__ volatile ("mrs %0, mdscr_el1" :"=r"(k));
        #else
            __asm__ volatile ("mrc 14, 0, %0, c0, c1":"=r" (k));
        #endif /* defined(ARCH_ARMV8_A) */
#endif /* defined(__ARMCC_VERSION) */
        } while (k & 0x20000000);

#if defined(__ARMCC_VERSION)
    #if defined(ARCH_ARMV8_A)
        __asm ("MSR dbgdtr_el0, %0" ::"r" (ch));
    #else
        __asm { MCR p14, 0, ch, c0, c5; }
    #endif
#else
    #if defined(ARCH_ARMV8_A)
        __asm__ volatile ("msr dbgdtr_el0, %0" ::"r"(ch));
    #else
        __asm__ volatile ("mcr 14, 0, %0, c0, c5"::"r" (ch));
    #endif
#endif

        data += len;
        size -= len;
    }
}


void dcc3_get_block(unsigned char *data, int size)
{
    static int bufsize = 0;
    static int bufindex;
    static unsigned char buf[4];
    int i, j;
#if defined(ARCH_ARMV8_A)
    unsigned long ch;
#else
    unsigned long long ch;
#endif

    while (size > 0) {
        if (!bufsize) {
            do {                /*Wait for input byte */
#if defined(__ARMCC_VERSION)
            #if defined(ARCH_ARMV8_A)
                __asm ("MRS %0, mdscr_el1" : "=r" (ch));
            #else
                __asm { MRC p14, 0, ch, c0, c1; }
            #endif
#else
            #if defined(ARCH_ARMV8_A)
                __asm__ volatile ("mrs %0, mdscr_el1 " :"=r"(ch));
            #else
                __asm__ volatile ("mrc 14, 0, %0, c0, c1":"=r" (ch));
            #endif
#endif
            } while (!(ch & 0x40000000));

#if defined(__ARMCC_VERSION)
        #if defined(ARCH_ARMV8_A)
            __asm ("MRS %0, dbgdtr_el0" : "=r" (ch));
        #else
            __asm { MRC p14, 0, ch, c0, c5; }
        #endif
#else
        #if defined(ARCH_ARMV8_A)
            __asm__ volatile ("mrs %0, dbgdtr_el0" :"=r"(ch));
        #else
            __asm__ volatile ("mrc 14, 0, %0, c0, c5":"=r" (ch));
        #endif
#endif
            buf[0] = ch & 0xff;
            buf[1] = (ch >> 8) & 0xff;
            buf[2] = (ch >> 16) & 0xff;
            bufsize = (ch >> 24) + 1;
            bufindex = 0;
        }
        i = bufsize;
        if (i > size)
            i = size;

        for (j = 0; j < i; j++)
            data[j] = buf[bufindex + j];

        bufindex += i;
        bufsize -= i;
        data += i;
        size -= i;
    }
}

#endif

