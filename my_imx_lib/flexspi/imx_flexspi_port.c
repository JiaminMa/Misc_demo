#include "imx_flexspi.h"
#include "hapi_clock.h"

extern void qspi_iomux_ss0_configuration(uint32_t index, uint32_t port, uint32_t override);
extern void qspi_iomux_sclk_configuration(uint32_t index, uint32_t port, uint32_t override);
extern void qspi_iomux_basic_data_configuration(uint32_t index, uint32_t port, uint32_t override);
extern void qspi_iomux_quad_configuration(uint32_t index, uint32_t port, uint32_t override);

void imx_flexspi_clock_gate_disable(uint32_t instance)
{
    hapi_flexspi_clock_disable();
}

void imx_flexspi_clock_config(uint32_t instance)
{
    hapi_configure_flexspi_clock(CLK_INIT, 0);
}

void imx_flexspi_clock_gate_enable(uint32_t instance)
{
    hapi_flexspi_clock_enable();
}

void imx_flexspi_iomux_config(uint32_t instance)
{
    qspi_iomux_ss0_configuration(0, 0, 0);
    qspi_iomux_sclk_configuration(0, 0, 0);
    qspi_iomux_basic_data_configuration(0, 0, 0);
    qspi_iomux_quad_configuration(0, 0, 0);
}

bool flexspi_is_differential_clock_enable()
{
    return false;
}

BOOLEAN bl_flexspi_nor_init(void *dev_cfg)
{
    return TRUE;
}
