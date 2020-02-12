#include "imx_ccm.h"
#include "ccm_psp.h"
#include "clock.h"
#include "hapi_clock.h"
#include "anatop.h"

/* 
 * Function related to clock gate on/off CCM_CCGR
 */
static inline void clock_gate_on(clock_ccgr_map_e clk)
{
    ccm_ccgr_set((uint32_t)clk, 0x3UL);
}

static inline void clock_gate_off(clock_ccgr_map_e clk)
{
    ccm_ccgr_clr((uint32_t)clk, 0x3UL);
}

void hapi_clock_enable(clock_e clk)
{
    switch (clk) {

        case ENFC_CLK:
            clock_gate_on(CCGR_RAWNAND);
            break;
        case ESDHC0_CLK:
            clock_gate_on(CCGR_USDHC1);
            break;
        case ESDHC1_CLK:
            clock_gate_on(CCGR_USDHC2);
            break;
        case ESDHC2_CLK:
            clock_gate_on(CCGR_USDHC3);
            break;
        case ECSPI0_CLK:
            clock_gate_on(CCGR_ECSPI1);
            break;
        case ECSPI1_CLK:
            clock_gate_on(CCGR_ECSPI2);
            break;
        case ECSPI2_CLK:
            clock_gate_on(CCGR_ECSPI3);
            break;
        case USB_CLK:
            clock_gate_on(CCGR_SIM_HSIO);
            clock_gate_on(CCGR_USB);
            break;
        case QSPI0_CLK:
            clock_gate_on(CCGR_QSPI);
            break;
        case RDC_CLK:
            clock_gate_on(CCGR_RDC);
            break;
        default:
            break;
    }
}

void hapi_clock_disable(clock_e clk)
{
    switch (clk) {

        case ENFC_CLK:
            clock_gate_off(CCGR_RAWNAND);
            break;
        case ESDHC0_CLK:
            clock_gate_off(CCGR_USDHC1);
            break;
        case ESDHC1_CLK:
            clock_gate_off(CCGR_USDHC2);
            break;
        case ESDHC2_CLK:
            clock_gate_off(CCGR_USDHC3);
            break;
        case ECSPI0_CLK:
            clock_gate_off(CCGR_ECSPI1);
            break;
        case ECSPI1_CLK:
            clock_gate_off(CCGR_ECSPI2);
            break;
        case ECSPI2_CLK:
            clock_gate_off(CCGR_ECSPI3);
            break;
        case USB_CLK:
            clock_gate_off(CCGR_SIM_HSIO);
            clock_gate_off(CCGR_USB);
            break;
        case QSPI0_CLK:
            clock_gate_off(CCGR_QSPI);
            break;
        case RDC_CLK:
            clock_gate_off(CCGR_RDC);
            break;
        default:
            break;
    }
}

void hapi_flexspi_clock_enable(void)
{
    hapi_clock_enable(QSPI0_CLK);
}

void hapi_flexspi_clock_disable(void)
{
    hapi_clock_disable(QSPI0_CLK);
}

void hapi_usdhc_clock_enable(uint8_t port)
{
    if (ESDHC0 == port) {
        hapi_clock_enable(ESDHC0_CLK);
    } else if (ESDHC1 == port) {
        hapi_clock_enable(ESDHC1_CLK);
    } else if (ESDHC2 == port) {
        hapi_clock_enable(ESDHC2_CLK);
    }
}

void hapi_usdhc_clock_disable(uint8_t port)
{
    if (ESDHC0 == port) {
        hapi_clock_disable(ESDHC0_CLK);
    } else if (ESDHC1 == port) {
        hapi_clock_disable(ESDHC1_CLK);
    } else if (ESDHC2 == port) {
        hapi_clock_disable(ESDHC2_CLK);
    }
}

/*
 * CCM Target Interface
 * */
void hapi_configure_flexspi_clock(qspi_sclk_e type, uint32_t is_ddr)
{
    if (is_ddr == 0) {
        switch(type) {
            case CLK_INIT:      /* 20MHz = 400MHz/5/4 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV5 |CLK_ROOT_POST_DIV4);
                break;

            case CLK_LEVEL1:    /* 33MHz = 400MHz/1/12 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV1 | CLK_ROOT_POST_DIV12);
                break;

            case CLK_LEVEL2:     /* 50MHz = 400MHz/1/8 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV8);
                break;

            case CLK_LEVEL3:     /* 66MHz = 400MHz/1/6 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV6);
                break;

            case CLK_LEVEL4:     /* 80MHz = 400MHz/1/5 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV5);
                break;

            case CLK_LEVEL5:    /* 100MHz = 400MHz/1/4 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV4);
                break;

            case CLK_LEVEL6:    /* 133MHz = 400MHz/1/3 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV3);
                break;

            case CLK_LEVEL7:    /* 166MHz = 333MHz/1/2 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL2_333M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV2);
                break;

            case CLK_LEVEL8:    /* 200MHz = 400MHz/1/2 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV2);
                break;
        }
    } else {

        switch  (type)
        {
            case CLK_INIT:      /* 20MHz = 400MHz/5/2/2 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON |QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV5 |CLK_ROOT_POST_DIV2);
                break;

            case CLK_LEVEL1:     /* 33MHz = 400MHz/1/6/2 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV1 | CLK_ROOT_POST_DIV6);
                break;

            case CLK_LEVEL2:     /* 50MHz = 400MHz/1/4/2 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV4);
                break;

            case CLK_LEVEL3:     /* 66MHz = 400MHz/1/3/2 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV3);
                break;

            case CLK_LEVEL4:     /* 83MHz = 333MHz/2/2 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL2_333M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV2);
                break;

            case CLK_LEVEL5:    /* 100MHz = 400MHz/2/2 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV2);
                break;

            case CLK_LEVEL6:    /* 133MHz = 266MHz/1/2 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_266M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV1);
                break;

            case CLK_LEVEL7:    /* 166MHz = 333MHz/1/2 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL2_333M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV1);
                break;

            case CLK_LEVEL8:    /* 200MHz = 400MHz/1/2 */
                ccm_target_root_wr(QSPI_CLK_ROOT, 
                        CLK_ROOT_ON | QSPI_CLK_ROOT_FROM_SYSTEM_PLL1_400M_CLK |
                        CLK_ROOT_PRE_DIV1 |CLK_ROOT_POST_DIV1);
                break;
        }
    }
}

void hapi_gpmi_bch_clock_init(GPMIBCH_TGT_Freq TGT_Freq)
{

}

static void pll_enable_m845s(uint32_t pll)
{
    uint32_t pll_lock = 0;
    volatile uint32_t *pll_control_reg = NULL;
    switch (pll) {
        case ANATOP_PLL_ARM:
            pll_control_reg = (void* )ARM_PLL_GNRL_CTL_ADDR;
            break;
        case ANATOP_PLL_SYSTEM_PLL1:
            pll_control_reg = (void* )SYS_PLL1_GNRL_CTL_ADDR;
            break;
        case ANATOP_PLL_SYSTEM_PLL2:
            pll_control_reg = (void* )SYS_PLL2_GNRL_CTL_ADDR;
            break;
        case ANATOP_PLL_SYSTEM_PLL3:
            pll_control_reg = (void* )SYS_PLL3_GNRL_CTL_ADDR;
            break;
        case ANATOP_PLL_DRAM:
            pll_control_reg = (void* )DRAM_PLL_GNRL_CTL_ADDR;
            break;
        default:
            break;
    }
    *pll_control_reg |= (UINT32)(GNRL_CTL_PLL_RST_MASK);
    *pll_control_reg &= (UINT32)~(GNRL_CTL_PLL_BYPASS_MASK);

    while(!((*pll_control_reg) & GNRL_CTL_PLL_LOCK_MASK));
}

void hapi_clock_init(void)
{
#define CORE_CLK_ROOT_NUM       3
#define PERIPHERAL_CLK_ROOT_NUM 23

    uint32_t i = 0;
    uint32_t core_clk_root[CORE_CLK_ROOT_NUM][2] = {
        {AHB_CLK_ROOT, AHB_CLK_ROOT_TYPICAL},
        {ARM_A53_CLK_ROOT, MAIN_AXI_CLK_ROOT_TYPICAL},
        {ARM_A53_CLK_ROOT, ARM_A53_CLK_ROOT_TYPICAL}
    };

    uint32_t peripheral_clk_root[PERIPHERAL_CLK_ROOT_NUM][2] = {

        {ARM_M4_CLK_ROOT, ARM_M4_CLK_ROOT_TYPICAL},
        {VPU_A53_CLK_ROOT, VPU_A53_CLK_ROOT_TYPICAL},
        {NAND_USDHC_BUS_CLK_ROOT, NAND_USDHC_BUS_CLK_ROOT_TYPICAL},
        {USB_BUS_CLK_ROOT, USB_BUS_CLK_ROOT_TYPICAL},
        {NOC_CLK_ROOT, NOC_CLK_ROOT_TYPICAL},
        {NAND_CLK_ROOT, NAND_CLK_ROOT_TYPICAL},
        {QSPI_CLK_ROOT, QSPI_CLK_ROOT_TYPICAL},
        {USDHC1_CLK_ROOT, USDHC1_CLK_ROOT_TYPICAL},
        {USDHC2_CLK_ROOT, USDHC2_CLK_ROOT_TYPICAL},
        {USB_PHY_REF_CLK_ROOT, USB_PHY_REF_CLK_ROOT_TYPICAL},
        {ECSPI1_CLK_ROOT, ECSPI1_CLK_ROOT_TYPICAL},
        {ECSPI2_CLK_ROOT, ECSPI2_CLK_ROOT_TYPICAL},
        {ECSPI3_CLK_ROOT, ECSPI3_CLK_ROOT_TYPICAL},
        {WRCLK_CLK_ROOT, WRCLK_CLK_ROOT_TYPICAL},
        {USDHC3_CLK_ROOT, USDHC3_CLK_ROOT_TYPICAL},
        {Reserved_CLK0, 0},
        {Reserved_CLK1, 0},
        {Reserved_CLK2, 0},
        {Reserved_CLK3, 0},
        {Reserved_CLK4, 0},
        {Reserved_CLK5, 0},
        {Reserved_CLK6, 0},
        {Reserved_CLK7, 0},
    };

    /* Enable PLL */
    pll_enable_m845s(ANATOP_PLL_ARM);
    pll_enable_m845s(ANATOP_PLL_SYSTEM_PLL1);
    pll_enable_m845s(ANATOP_PLL_SYSTEM_PLL2);

    /* Set clock root */
    /* Configure AHB/AXI bus firstly, to avoid CPU being starved.*/
    for (i = 0; i < CORE_CLK_ROOT_NUM; i++) {
        ccm_target_root_wr(core_clk_root[i][0], core_clk_root[i][1]);
    } 

    /*
     * By default, all the clock root are "on" and sourced from 24MHz,
     * except DRAM/DRAM_PHYM which are sourced from DDR_PLL/2.
     */
    for (i = 0; i < PERIPHERAL_CLK_ROOT_NUM; i++) {
        ccm_target_root_wr(peripheral_clk_root[i][0], peripheral_clk_root[i][1]);
    } 
}

