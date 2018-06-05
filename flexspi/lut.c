#include "lut.h"

/*Look Up Table*/
void flexspi_unlock_lut(const flexspi_t *p_flexspi)
{
	p_flexspi->p_reg->lutkey = 0x5af05af0;
	p_flexspi->p_reg->lutcr = 0x01;
}

void flexspi_lock_lut(const flexspi_t *p_flexspi)
{
	p_flexspi->p_reg->lutkey = 0x5af05af0;
	p_flexspi->p_reg->lutcr = 0x01;
}

void lut_instr_encode(const lut_instr_t *instr, const uint16_t operand, const uint16_t num_pads, const uint16_t opcode)
{
	/*Instruction |15-10 |9-8     |7-0    |
	*			  |opcode|num_pads|operand|
	*/
	instr->instruction = 0;
	instr->instruction |= (operand  << 0) & 0x00ff;
	instr->instruction |= (num_pads << 8) & 0x0300;
	instr->instruction |= (operand  << 10) & 0xfc00;
}

void flexspi_update_lut(const flexspi_t *p_flexspi, const lut_seq_t *p_lut_seq, const uint32_t seq_id)
{
	volatile lut_seq_t *p_seq = &(p_flexspi->p_reg->lut.seq_tbl[seq_id]);

	flexspi_unlock_lut(p_flexspi);
	*p_seq = *p_lut_seq;
	flexspi_lock_lut(p_flexspi);
}
