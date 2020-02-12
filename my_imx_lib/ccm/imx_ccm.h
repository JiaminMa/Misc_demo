#ifndef IMX_CCM_H
#define IMX_CCM_H

#include <stdint.h>

extern void ccm_gpr_wr(uint32_t idx, uint32_t val);
extern void ccm_observe_wr(uint32_t idx, uint32_t val);
extern void ccm_ctrl_wr(uint32_t idx, uint32_t val);
extern void ccm_ccgr_wr(uint32_t idx, uint32_t val);
extern void ccm_target_root_wr(uint32_t idx, uint32_t val);

extern uint32_t ccm_gpr_rd(uint32_t idx);
extern uint32_t ccm_observe_rd(uint32_t idx);
extern uint32_t ccm_ctrl_rd(uint32_t idx);
extern uint32_t ccm_ccgr_rd(uint32_t idx);
extern uint32_t ccm_target_root_rd(uint32_t idx);

extern void ccm_gpr_set(uint32_t idx, uint32_t val);
extern void ccm_observe_set(uint32_t idx, uint32_t val);
extern void ccm_ctrl_set(uint32_t idx, uint32_t val);
extern void ccm_ccgr_set(uint32_t idx, uint32_t val);
extern void ccm_target_root_set(uint32_t idx, uint32_t val);

extern void ccm_gpr_clr(uint32_t idx, uint32_t val);
extern void ccm_observe_clr(uint32_t idx, uint32_t val);
extern void ccm_ctrl_clr(uint32_t idx, uint32_t val);
extern void ccm_ccgr_clr(uint32_t idx, uint32_t val);
extern void ccm_target_root_clr(uint32_t idx, uint32_t val);

extern void ccm_gpr_toggle(uint32_t idx, uint32_t val);
extern void ccm_observe_toggle(uint32_t idx, uint32_t val);
extern void ccm_ctrl_toggle(uint32_t idx, uint32_t val);
extern void ccm_ccgr_toggle(uint32_t idx, uint32_t val);
extern void ccm_target_root_toggle(uint32_t idx, uint32_t val);

#endif
