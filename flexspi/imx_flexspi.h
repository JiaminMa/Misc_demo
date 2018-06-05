#ifndef IMX_FLEXSPI_H
#define IMX_FLEXSPI_H

#include <stdint.h>

#define FLEXSPI_SUCCESS		0x3c
#define FLEXSPI_FAIL		0x5a

typedef struct flexspi_reg_tag {
	volatile uint32_t mcr0;                        /**< Module Control Register 0, offset: 0x0 */
	volatile uint32_t mcr1;                        /**< Module Control Register 1, offset: 0x4 */
	volatile uint32_t mcr2;                        /**< Module Control Register 2, offset: 0x8 */
	volatile uint32_t ahbcr;                       /**< AHB Bus Control Register, offset: 0xC */
	volatile uint32_t inten;                       /**< Interrupt Enable Register, offset: 0x10 */
	volatile uint32_t intr;                        /**< Interrupt Register, offset: 0x14 */
	volatile uint32_t lutkey;                      /**< LUT Key Register, offset: 0x18 */
	volatile uint32_t lutcr;                       /**< LUT Control Register, offset: 0x1C */
	volatile uint32_t ahb_rx_buf_cr0[4];           /**< AHB RX Buffer 0 Control Register 0..AHB RX Buffer 3 Control Register 0, array offset: 0x20, array step: 0x4 */
	uint8_t reserved0[48];
	volatile uint32_t flash_cr0[4];                /**< Flash A1 Control Register 0..Flash B2 Control Register 0, array offset: 0x60, array step: 0x4 */
	volatile uint32_t flash_cr1[4];                /**< Flash A1 Control Register 1..Flash B2 Control Register 1, array offset: 0x70, array step: 0x4 */
	volatile uint32_t flash_cr2[4];                /**< Flash A1 Control Register 2..Flash B2 Control Register 2, array offset: 0x80, array step: 0x4 */
	volatile uint32_t flash_cr3;				   /**< Flash Control Register3 */
	uint8_t reserved1[4];
	volatile uint32_t flash_cr5;                    /**< Flash Control Register 5, offset: 0x98 */
	uint8_t reserved2[4];
	volatile uint32_t ipcr0;                       /**< IP Control Register 0, offset: 0xA0 */
	volatile uint32_t ipcr1;                       /**< IP Control Register 1, offset: 0xA4 */
	uint8_t reserved3[8];
	volatile uint32_t ipcmd;                       /**< IP Command Register, offset: 0xB0 */
	volatile uint32_t dlpr;                        /**< Data Learn Pattern Register, offset: 0xB4 */
	volatile uint32_t ip_rxf_cr;                   /**< IP RX FIFO Control Register, offset: 0xB8 */
	volatile uint32_t ip_txf_cr;                   /**< IP TX FIFO Control Register, offset: 0xBC */
	volatile uint32_t dll_cr[2];                    /**< DLL Control Register 0, array offset: 0xC0, array step: 0x4 */
	uint8_t reserved4[24];
	volatile uint32_t sts0;                         /**< Status Register 0, offset: 0xE0 */
	volatile uint32_t sts1;                         /**< Status Register 1, offset: 0xE4 */
	volatile uint32_t sts2;                         /**< Status Register 2, offset: 0xE8 */
	volatile uint32_t ahb_spnd_sts;                 /**< AHB Suspend Status Register, offset: 0xEC */
	volatile uint32_t ip_rxf_sts;                   /**< IP RX FIFO Status Register, offset: 0xF0 */
	volatile uint32_t ip_txf_sts;                   /**< IP TX FIFO Status Register, offset: 0xF4 */
	uint8_t reserved5[8];
	volatile uint32_t ip_rxf_dr[32];                /**< IP RX FIFO Data Register 0..IP RX FIFO Data Register 31, array offset: 0x100, array step: 0x4 */
	volatile uint32_t ip_txf_dr[32];                /**< IP TX FIFO Data Register 0..IP TX FIFO Data Register 31, array offset: 0x180, array step: 0x4 */
	volatile lut_t lut;
}flexspi_reg_t;

typedef struct flexspi_tag {
	flexspi_reg_t *p_reg;
}flexspi_t;

extern void flexspi_init(flexspi_t *p_flexspi, uint32_t base_addr);

#endif /*IMX_FLEXSPI_H*/
