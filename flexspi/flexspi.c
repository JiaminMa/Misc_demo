#include "imx_flexspi.h"
#include <stdio.h>

void flexspi_init(flexspi_t *p_flexspi, uint32_t base_addr)
{
	p_flexspi->p_reg = base_addr;
}

int main()
{
	return 0;
}
