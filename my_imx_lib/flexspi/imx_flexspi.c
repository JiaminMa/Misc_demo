#include "imx_flexspi.h"

flexspi_instance_t g_flexspi_instance_arr[2] = {
    {(flexspi_t *)0x30BB0000U, 0},
    {(flexspi_t *)0, 1},
}; 

void imx_flexspi_swreset(flexspi_t *flexspi_reg)
{
    flexspi_reg->mcr0 |= FLEXSPI_MCR0_SWRESET_MASK;
    while (flexspi_reg->mcr0 & FLEXSPI_MCR0_SWRESET_MASK);
}

static void imx_flexspi_config_ahb(flexspi_t *flexspi_reg)
{
    uint32_t temp;
    uint32_t i;

    temp = flexspi_reg->ahbcr & (~FLEXSPI_AHBCR_APAREN_MASK);
    temp |= FLEXSPI_AHBCR_READADDROPT_MASK;
    flexspi_reg->ahbcr = temp;
    flexspi_reg->ahbcr |= FLEXSPI_AHBCR_PREFETCHEN_MASK;

    for (i = 0; i < FLEXSPI_AHBRXBUFCR0_COUNT - 1; i++) {
        flexspi_reg->ahbrxbufcr0[i] &=
            ~(FLEXSPI_AHBRXBUFCR0_BUFSZ_MASK | FLEXSPI_AHBRXBUFCR0_MSTRID_MASK |
                    FLEXSPI_AHBRXBUFCR0_PRIORITY_MASK);
    }
}

static void imx_flexspi_config_flash_controller(flexspi_t *flexspi_reg)
{
    uint32_t temp = 0;
    flexspi_reg->flashcr0[0] = 64 * 1024;

    temp = FLEXSPI_FLSHCR1_TCSS(0x3) | FLEXSPI_FLSHCR1_TCSH(0x3) |
        FLEXSPI_FLSHCR1_CAS(0);
    flexspi_reg->flashcr1[0] = temp;

    temp = 0;
    temp |= FLEXSPI_FLSHCR2_AWRSEQID(CMD_LUT_SEQ_IDX_WRITE);
    flexspi_reg->flashcr2[0] = temp;
}

static void imx_flexspi_wait_until_ip_idle(flexspi_t * base)
{
    while ((base->sts0 & FLEXSPI_STS0_SEQIDLE_MASK) != FLEXSPI_STS0_SEQIDLE_MASK);

    while ((base->sts0 & FLEXSPI_STS0_ARBIDLE_MASK) != FLEXSPI_STS0_ARBIDLE_MASK);
}

static void imx_flexspi_unlock_lut(flexspi_t *base)
{
    base->lutkey = FLEXSPI_LUT_KEY_VAL;
    base->lutcr = 0x02;
}

static void imx_flexspi_lock_lut(flexspi_t *base)
{
    base->lutkey = FLEXSPI_LUT_KEY_VAL;
    base->lutcr = 0x01;
}

void imx_flexspi_update_lut(flexspi_t *flexspi_reg, uint32_t seq_idx, uint32_t seq[])
{
    uint32_t i = 0;

    imx_flexspi_wait_until_ip_idle(flexspi_reg);
    imx_flexspi_unlock_lut(flexspi_reg);

    for (i = 0; i < 4; i++) {
        flexspi_reg->lut[seq_idx * 4 + i] =  seq[i];
    }

    imx_flexspi_lock_lut(flexspi_reg);
}

static void imx_flexspi_init_lut(flexspi_t *flexspi_reg)
{
    imx_flexspi_wait_until_ip_idle(flexspi_reg);
    imx_flexspi_unlock_lut(flexspi_reg);

    flexspi_reg->lut[0] = 0x08180403;
    flexspi_reg->lut[1] = 0x2480;
    flexspi_reg->lut[2] = 0x0;
    flexspi_reg->lut[3] = 0x0;

    flexspi_reg->lut[4] = 0x24040405;
    flexspi_reg->lut[5] = 0x0;
    flexspi_reg->lut[6] = 0x0;
    flexspi_reg->lut[7] = 0x0;

    flexspi_reg->lut[8] = 0x0;
    flexspi_reg->lut[9] = 0x0;
    flexspi_reg->lut[10] = 0x0;
    flexspi_reg->lut[11] = 0x0;

    flexspi_reg->lut[12] = 0x0406;
    flexspi_reg->lut[13] = 0x0;
    flexspi_reg->lut[14] = 0x0;
    flexspi_reg->lut[15] = 0x0;

    flexspi_reg->lut[16] = 0x20020401;
    flexspi_reg->lut[17] = 0x0;
    flexspi_reg->lut[18] = 0x0;
    flexspi_reg->lut[19] = 0x0;

    flexspi_reg->lut[20] = 0x082004dc;
    flexspi_reg->lut[21] = 0x0;
    flexspi_reg->lut[22] = 0x0;
    flexspi_reg->lut[23] = 0x0;

    flexspi_reg->lut[24] = 0x04c7;
    flexspi_reg->lut[25] = 0x0;
    flexspi_reg->lut[26] = 0x0;
    flexspi_reg->lut[27] = 0x0;

    flexspi_reg->lut[28] = 0x0;
    flexspi_reg->lut[29] = 0x0;
    flexspi_reg->lut[30] = 0x0;
    flexspi_reg->lut[31] = 0x0;

    flexspi_reg->lut[32] = 0x0;
    flexspi_reg->lut[33] = 0x0;
    flexspi_reg->lut[34] = 0x0;
    flexspi_reg->lut[35] = 0x0;

    flexspi_reg->lut[36] = 0x08200412;
    flexspi_reg->lut[37] = 0x2080;
    flexspi_reg->lut[38] = 0x0;
    flexspi_reg->lut[39] = 0x0;

    flexspi_reg->lut[40] = 0x0;
    flexspi_reg->lut[41] = 0x0;
    flexspi_reg->lut[42] = 0x0;
    flexspi_reg->lut[43] = 0x0;

    flexspi_reg->lut[44] = 0x0460;
    flexspi_reg->lut[45] = 0x0;
    flexspi_reg->lut[46] = 0x0;
    flexspi_reg->lut[47] = 0x0;

    flexspi_reg->lut[48] = 0x0;
    flexspi_reg->lut[49] = 0x0;
    flexspi_reg->lut[50] = 0x0;
    flexspi_reg->lut[51] = 0x0;

    flexspi_reg->lut[52] = 0x0;
    flexspi_reg->lut[53] = 0x0;
    flexspi_reg->lut[54] = 0x0;
    flexspi_reg->lut[55] = 0x0;

    flexspi_reg->lut[56] = 0x0;
    flexspi_reg->lut[57] = 0x0;
    flexspi_reg->lut[58] = 0x0;
    flexspi_reg->lut[59] = 0x0;

    flexspi_reg->lut[60] = 0x0;
    flexspi_reg->lut[61] = 0x0;
    flexspi_reg->lut[62] = 0x0;
    flexspi_reg->lut[63] = 0x0;

    imx_flexspi_lock_lut(flexspi_reg);
}

static void imx_flexspi_clear_ip_txfifo(flexspi_t * base)
{
    base->iptxcr |= FLEXSPI_IPTXFCR_CLRIPTXF_MASK;
    while (base->iptxcr & FLEXSPI_IPTXFCR_CLRIPTXF_MASK);
}

static void imx_flexspi_clear_ip_rxfifo(flexspi_t * base)
{
    base->iprxcr |= FLEXSPI_IPRXFCR_CLRIPRXF_MASK;
    while (base->iprxcr & FLEXSPI_IPRXFCR_CLRIPRXF_MASK);
}


static void do_flexspi_ip_cmd_write(flexspi_ip_cmd_t *flexspi_ip_cmd_p)
{
    uint32_t i = 0;
    uint32_t remain_size = flexspi_ip_cmd_p->size;
    uint32_t *buf_ptr = (uint32_t *)flexspi_ip_cmd_p->src;
    uint32_t temp = 0;
    flexspi_t *flexspi_reg = (flexspi_t *)FLEXSPI_BASE;

    flexspi_reg->ipcr0 = (uint32_t)flexspi_ip_cmd_p->dst; 
    temp |= FLEXSPI_IPCR1_IDATSZ(flexspi_ip_cmd_p->size);
    temp |= FLEXSPI_IPCR1_ISEQID(flexspi_ip_cmd_p->seq_id) 
                    | FLEXSPI_IPCR1_ISEQNUM(flexspi_ip_cmd_p->seq_size - 1)
                    | FLEXSPI_IPCR1_IPAREN(flexspi_ip_cmd_p->is_parallel);
    flexspi_reg->ipcr1 = temp;
    flexspi_reg->ipcmd |= FLEXSPI_IPCMD_TRG_MASK;

    while (remain_size > 0) {
        if (flexspi_reg->intr & FLEXSPI_INTR_IPTXWE_MASK) {
            if (remain_size >= 8) {
                flexspi_reg->tfdr[0] = *buf_ptr++;
                flexspi_reg->tfdr[1] = *buf_ptr++;
                remain_size -= 8;
            } else {
            }
            flexspi_reg->intr |= FLEXSPI_INTR_IPTXWE_MASK;
        }
        if ((flexspi_reg->intr & FLEXSPI_INTR_IPCMDERR_MASK) != 0U) {
            break;
        }
    }
}

static void do_flexspi_ip_cmd_op(flexspi_ip_cmd_t *flexspi_ip_cmd_p)
{
    flexspi_t *flexspi_reg = (flexspi_t *)FLEXSPI_BASE;
    uint32_t temp = 0;

    flexspi_reg->ipcr0 = (uint32_t)flexspi_ip_cmd_p->dst; 
    temp |= FLEXSPI_IPCR1_IDATSZ(flexspi_ip_cmd_p->size);
    temp |= FLEXSPI_IPCR1_ISEQID(flexspi_ip_cmd_p->seq_id) 
                | FLEXSPI_IPCR1_ISEQNUM(flexspi_ip_cmd_p->seq_size - 1) |
        FLEXSPI_IPCR1_IPAREN(flexspi_ip_cmd_p->is_parallel);
    flexspi_reg->ipcr1 = temp;
    flexspi_reg->ipcmd |= FLEXSPI_IPCMD_TRG_MASK; 
}

static void do_flexspi_ip_cmd_read(flexspi_ip_cmd_t *flexspi_ip_cmd_p)
{
    uint32_t i = 0;
    uint32_t remain_size = flexspi_ip_cmd_p->size;
    uint32_t *buf_ptr = (uint32_t *)flexspi_ip_cmd_p->dst;
    uint32_t temp = 0;
    uint32_t buffer[2] = {0};
    uint8_t *src;
    uint8_t *dst;
    
    flexspi_t *flexspi_reg = (flexspi_t *)FLEXSPI_BASE;

    flexspi_reg->ipcr0 = (uint32_t)flexspi_ip_cmd_p->src; 
    temp |= FLEXSPI_IPCR1_IDATSZ(flexspi_ip_cmd_p->size);
    temp |= FLEXSPI_IPCR1_ISEQID(flexspi_ip_cmd_p->seq_id) 
            | FLEXSPI_IPCR1_ISEQNUM(flexspi_ip_cmd_p->seq_size - 1) 
            | FLEXSPI_IPCR1_IPAREN(flexspi_ip_cmd_p->is_parallel);
    flexspi_reg->ipcr1 = temp;
    flexspi_reg->ipcmd |= FLEXSPI_IPCMD_TRG_MASK;

    while (remain_size > 0) {
        if (remain_size >= 8) {
            // Make sure the RX FIFO contains valid data before read
            if (flexspi_reg->intr & FLEXSPI_INTR_IPRXWA_MASK) {
                *buf_ptr++ = flexspi_reg->rfdr[0];
                *buf_ptr++ = flexspi_reg->rfdr[1];
                remain_size -= 8;

                flexspi_reg->intr |= FLEXSPI_INTR_IPRXWA_MASK;
            }
        } else {
            if (flexspi_reg->iprxsts & FLEXSPI_IPRXFSTS_FILL_MASK) {
                buffer[0] = flexspi_reg->rfdr[0];
                buffer[1] = flexspi_reg->rfdr[1];

                src = (uint8_t *)buffer;
                dst = (uint8_t *)buf_ptr;
                for (i = 0; i < remain_size; i++) {
                    *dst++ = *src++;
                }
                remain_size = 0;

                flexspi_reg->intr |= FLEXSPI_INTR_IPRXWA_MASK;
            } 
        }
        
        if ((flexspi_reg->intr & FLEXSPI_INTR_IPCMDERR_MASK) != 0U) {
            break;
        }
    }
}

uint32_t imx_flexspi_ip_command(flexspi_t *flexspi_reg, flexspi_ip_cmd_t *flexspi_ip_cmd_p)
{
    
    imx_flexspi_clear_ip_rxfifo(flexspi_reg);
    imx_flexspi_clear_ip_txfifo(flexspi_reg);

    switch (flexspi_ip_cmd_p->op) {
        case FLEXSPI_CMD_READ:
            do_flexspi_ip_cmd_read(flexspi_ip_cmd_p);
            break;
        case FLEXSPI_CMD_WRITE:
            do_flexspi_ip_cmd_write(flexspi_ip_cmd_p);
            break;
        case FLEXSPI_CMD_OPER:
            do_flexspi_ip_cmd_op(flexspi_ip_cmd_p);
        default:
            break;
    }

    imx_flexspi_wait_until_ip_idle(flexspi_reg);
    
    if (flexspi_reg->intr & FLEXSPI_INTR_IPCMDERR_MASK) {
        switch ((flexspi_reg->sts1 & FLEXSPI_STS1_IPCMDERRCODE_MASK) 
                >> FLEXSPI_STS1_IPCMDERRCODE_SHIFT) {
        default:
            return 1;
        case 7 :
            return 2;
        }
    }
    return 0;
}  

uint32_t imx_flexspi_init(flexspi_instance_t *flexspi_instance)
{
    uint32_t mcr0 = 0;

    flexspi_t *flexspi_reg = flexspi_instance->flexspi_reg;
    uint32_t instance = flexspi_instance->instance;
    /* Enable controller clocks in System level.*/
    imx_flexspi_clock_gate_disable(instance);
    imx_flexspi_clock_config(instance);
    imx_flexspi_clock_gate_enable(instance);
    imx_flexspi_iomux_config(instance);

    /* Set MCR0[MDIS] to 0x1(Make sure controller is configured in module stop mode)*/
    flexspi_reg->mcr0 &= ~FLEXSPI_MCR0_MDIS_MASK;
    imx_flexspi_swreset(flexspi_reg);
    flexspi_reg->mcr0 |= FLEXSPI_MCR0_MDIS_MASK;

    /* Configure module control registers: MCR0, MCR1, MCR2
     * Don't change MCR0[MDIS]
     * */
    mcr0 = flexspi_reg->mcr0 & (uint32_t) ~(FLEXSPI_MCR0_RXCLKSRC_MASK 
            | FLEXSPI_MCR0_IPGRANTWAIT_MASK | FLEXSPI_MCR0_AHBGRANTWAIT_MASK 
            | FLEXSPI_MCR0_COMBINATIONEN_MASK | FLEXSPI_MCR0_ATDFEN_MASK | FLEXSPI_MCR0_ARDFEN_MASK);

    mcr0 |= FLEXSPI_MCR0_IPGRANTWAIT_MASK;
    mcr0 |= FLEXSPI_MCR0_AHBGRANTWAIT_MASK;
    mcr0 |= FLEXSPI_MCR0_RXCLKSRC(0);

    flexspi_reg->mcr0 = mcr0;
    flexspi_reg->mcr1 = FLEXSPI_MCR1_SEQWAIT(0xFFFF) | FLEXSPI_MCR1_AHBBUSWAIT(0xFFFF); 
    flexspi_reg->mcr2 &= ~FLEXSPI_MCR2_SAMEDEVICEEN_MASK;

    /* Configure AHB bus control register and AHB RX Buffer control registers */
    imx_flexspi_config_ahb(flexspi_reg);

    /* Configure Flash control registers(FLSHxCR0, FLASHxCR1, FLASHxCR2) */
    imx_flexspi_config_flash_controller(flexspi_reg);

    /* Configure DLL control regiseter(DLLxCR) according to sample clock source */
    flexspi_reg->dllcr[0] = 0x100UL;

    /* Set MCR0[MDIS] to 0x0(Exit module stop mode) */
    flexspi_reg->mcr0 &= ~FLEXSPI_MCR0_MDIS_MASK;

    /* Configure LUT as needed(For AHB command or IP command) */
    imx_flexspi_init_lut(flexspi_reg);
    imx_flexspi_swreset(flexspi_reg);
    return 0;
}

