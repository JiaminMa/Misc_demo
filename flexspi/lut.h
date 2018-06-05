#ifndef FLEXSPI_LUT_H
#define FLEXSPI_LUT_H

#define CMD_SDR         0x01
#define CMD_DDR         0x21
#define RADDR_SDR       0x02
#define RADDR_DDR       0x22
#define CADDR_SDR       0x03
#define CADDR_DDR       0x23
#define MODE1_SDR       0x04
#define MODE1_DDR       0x24
#define MODE2_SDR       0x05
#define MODE2_DDR       0x25
#define MODE4_SDR       0x06
#define MODE4_DDR       0x26
#define MODE8_SDR       0x07
#define MODE8_DDR       0x27
#define WRITE_SDR       0x08
#define WRITE_DDR       0x28
#define READ_SDR        0x09
#define READ_DDR        0x29
#define LEARN_SDR       0x0A
#define LEARN_DDR       0x2A
#define DATSZ_SDR       0x0B
#define DATSZ_DDR       0x2B
#define DUMMY_SDR       0x0C
#define DUMMY_DDR       0x2C
#define DUMMY_RWDS_SDR  0x0D
#define DUMMY_RWDS_DDR  0x2D
#define JMP_ON_CS       0x1F
#define STOP            0

typedef struct lut_instr_tag {
	uint16_t instruction;
}lut_instr_t;

typedef struct lut_seq_tag {
	volatile lut_instr_t instr_tbl[8];
}lut_seq_t;

typedef struct lut_tag {
	volatile lut_seq_t seq_tbl[16];
}lut_t;

extern void lut_instr_encode(const lut_instr_t *instr, const uint16_t operand, const uint16_t num_pads, const uint16_t opcode);
extern void flexspi_unlock_lut(const flexspi_t *p_flexspi);
extern void flexspi_lock_lut(const flexspi_t *p_flexspi);
extern void flexspi_update_lut(const flexspi_t *p_flexspi, const lut_seq_t *p_lut_seq, const uint32_t seq_id);

#endif /*FLEXSPI_LUT_H*/
