#include "imx_ccm.h"
#include "ccm_psp.h"

void ccm_gpr_wr(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_gprx = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_GPR0_OFFSET + 0x10 * idx);
    *ccm_gprx = val;
}

void ccm_observe_wr(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_observex = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_OBSERVE0_OFFSET + 0x10 * idx);
    *ccm_observex = val;

}

void ccm_ctrl_wr(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_ctrlx = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_CTRL0_OFFSET + 0x10 * idx);
    *ccm_ctrlx = val;
}

void ccm_ccgr_wr(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_ccgrx = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_CCGR0_OFFSET + 0x10 * idx);
    *ccm_ccgrx = val;
}

void ccm_target_root_wr(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_target_rootx = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_ROOT0_TARGET_OFFSET + 0x80 * idx);
    *ccm_target_rootx = val;
}

uint32_t ccm_gpr_rd(uint32_t idx)
{
    volatile uint32_t *ccm_gprx = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_GPR0_OFFSET + 0x10 * idx);
    return *ccm_gprx;
}

uint32_t ccm_observe_rd(uint32_t idx)
{
    volatile uint32_t *ccm_observex = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_OBSERVE0_OFFSET + 0x10 * idx);
    return *ccm_observex;
}

uint32_t ccm_ctrl_rd(uint32_t idx)
{
    volatile uint32_t *ccm_ctrlx = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                   + CCM_CTRL0_OFFSET + 0x10 * idx);
    return *ccm_ctrlx;
}

uint32_t ccm_ccgr_rd(uint32_t idx)
{
    volatile uint32_t *ccm_ccgrx = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_CCGR0_OFFSET + 0x10 * idx);
    return *ccm_ccgrx;
}

uint32_t ccm_target_root_rd(uint32_t idx)
{
    volatile uint32_t *ccm_target_rootx = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_ROOT0_TARGET_OFFSET + 0x80 * idx);
    return *ccm_target_rootx;
}

void ccm_gpr_set(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_gprx_set = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_GPR0_OFFSET + 0x10 * idx + 4);
    *ccm_gprx_set = val;
}

void ccm_observe_set(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_observex_set = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_OBSERVE0_OFFSET + 0x10 * idx + 4);
    *ccm_observex_set = val;
}

void ccm_ctrl_set(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_ctrlx_set = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                   + CCM_CTRL0_OFFSET + 0x10 * idx + 4);
    *ccm_ctrlx_set = val;

}

void ccm_ccgr_set(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_ccgrx_set = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_CCGR0_OFFSET + 0x10 * idx + 4);
    *ccm_ccgrx_set = val;
}

void ccm_target_root_set(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_target_rootx_set = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                            + CCM_ROOT0_TARGET_OFFSET + 0x80 * idx + 4);
    *ccm_target_rootx_set = val;
}

void ccm_gpr_clr(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_gprx_clr = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_GPR0_OFFSET + 0x10 * idx + 8);
    *ccm_gprx_clr = val;
}

void ccm_observe_clr(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_observex_clr = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_OBSERVE0_OFFSET + 0x10 * idx + 8);
    *ccm_observex_clr = val;
}

void ccm_ctrl_clr(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_ctrlx_clr = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                    + CCM_CTRL0_OFFSET + 0x10 * idx + 8);
    *ccm_ctrlx_clr = val;
}

void ccm_ccgr_clr(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_ccgrx_clr = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_CCGR0_OFFSET + 0x10 * idx + 8);
    *ccm_ccgrx_clr = val;
}

void ccm_target_root_clr(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_target_rootx_clr = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                            + CCM_ROOT0_TARGET_OFFSET + 0x80 * idx + 8);
    *ccm_target_rootx_clr = val;
}

void ccm_gpr_toggle(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_gprx_toggle = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_GPR0_OFFSET + 0x10 * idx + 12);
    *ccm_gprx_toggle = val;
}

void ccm_observe_toggle(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_observex_toggle = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_OBSERVE0_OFFSET + 0x10 * idx + 12);
    *ccm_observex_toggle = val;
}

void ccm_ctrl_toggle(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_ctrlx_toggle = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                    + CCM_CTRL0_OFFSET + 0x10 * idx + 12);
    *ccm_ctrlx_toggle = val;
}

void ccm_ccgr_toggle(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_ccgrx_toggle = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                                + CCM_CCGR0_OFFSET + 0x10 * idx + 12);
    *ccm_ccgrx_toggle = val;
}

void ccm_target_root_toggle(uint32_t idx, uint32_t val)
{
    volatile uint32_t *ccm_target_rootx_toggle = (volatile uint32_t *)(CCM_BASE_ADDRESS
                                            + CCM_ROOT0_TARGET_OFFSET + 0x80 * idx + 12);
    *ccm_target_rootx_toggle = val;
}
