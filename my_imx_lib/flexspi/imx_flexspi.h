#ifndef IMX_FLEXSPI_H
#define IMX_FLEXSPI_H

#include <stdint.h>

#define FLEXSPI_BASE        0x30BB0000U
#define FLEXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1) (FLEXSPI_LUT_OPERAND0(op0) | FLEXSPI_LUT_NUM_PADS0(pad0) | FLEXSPI_LUT_OPCODE0(cmd0) | FLEXSPI_LUT_OPERAND1(op1) | FLEXSPI_LUT_NUM_PADS1(pad1) | FLEXSPI_LUT_OPCODE1(cmd1))

typedef struct {
    volatile uint32_t mcr0;                        /**< Module Control Register 0, offset: 0x0 */
    volatile uint32_t mcr1;                        /**< Module Control Register 1, offset: 0x4 */
    volatile uint32_t mcr2;                        /**< Module Control Register 2, offset: 0x8 */
    volatile uint32_t ahbcr;                       /**< AHB Bus Control Register, offset: 0xC */
    volatile uint32_t inten;                       /**< Interrupt Enable Register, offset: 0x10 */
    volatile uint32_t intr;                        /**< Interrupt Register, offset: 0x14 */
    volatile uint32_t lutkey;                      /**< LUT Key Register, offset: 0x18 */
    volatile uint32_t lutcr;                       /**< LUT Control Register, offset: 0x1C */
    volatile uint32_t ahbrxbufcr0[8];              /**< AHB RX Buffer 0 Control Register 0..
                                                     AHB RX Buffer 3 Control Register 0, array offset: 0x20, array step: 0x4 */
    uint8_t reserved0[32];
    volatile uint32_t flashcr0[4];                  /**< Flash A1 Control Register 0..
                                                     Flash B2 Control Register 0, array offset: 0x60, array step: 0x4 */
    volatile uint32_t flashcr1[4];                  /**< Flash A1 Control Register 1..
                                                     Flash B2 Control Register 1, array offset: 0x70, array step: 0x4 */
    volatile uint32_t flashcr2[4];                  /**< Flash A1 Control Register 2..
                                                     Flash B2 Control Register 2, array offset: 0x80, array step: 0x4 */
    uint8_t reserved1[8];
    volatile uint32_t flashcr5;                     /**< Flash Control Register 5, offset: 0x98 */
    uint8_t reserved2[4];
    volatile uint32_t ipcr0;                       /**< IP Control Register 0, offset: 0xA0 */
    volatile uint32_t ipcr1;                       /**< IP Control Register 1, offset: 0xA4 */
    uint8_t reserved3[8];
    volatile uint32_t ipcmd;                       /**< IP Command Register, offset: 0xB0 */
    volatile uint32_t dlpr;                        /**< Data Learn Pattern Register, offset: 0xB4 */
    volatile uint32_t iprxcr;                     /**< IP RX FIFO Control Register, offset: 0xB8 */
    volatile uint32_t iptxcr;                     /**< IP TX FIFO Control Register, offset: 0xBC */
    volatile uint32_t dllcr[2];                    /**< DLL Control Register 0, array offset: 0xC0, array step: 0x4 */
    uint8_t reserved4[24];
    volatile uint32_t sts0;                         /**< Status Register 0, offset: 0xE0 */
    volatile uint32_t sts1;                         /**< Status Register 1, offset: 0xE4 */
    volatile uint32_t sts2;                         /**< Status Register 2, offset: 0xE8 */
    volatile uint32_t ahbspndsts;                   /**< AHB Suspend Status Register, offset: 0xEC */
    volatile uint32_t iprxsts;                     /**< IP RX FIFO Status Register, offset: 0xF0 */
    volatile uint32_t iptxsts;                     /**< IP TX FIFO Status Register, offset: 0xF4 */
    uint8_t reserved5[8];
    volatile uint32_t rfdr[32];                     /**< IP RX FIFO Data Register 0..
                                                      IP RX FIFO Data Register 31, array offset: 0x100, array step: 0x4 */
    volatile uint32_t tfdr[32];                     /**< IP TX FIFO Data Register 0..
                                                      IP TX FIFO Data Register 31, array offset: 0x180, array step: 0x4 */
    volatile uint32_t lut[64];                     /**< LUT 0..LUT 63, array offset: 0x200, array step: 0x4 */
} flexspi_t;


/*! @name MCR0 - Module Control Register 0 */
#define FLEXSPI_MCR0_SWRESET_MASK                (0x1U)
#define FLEXSPI_MCR0_SWRESET_SHIFT               (0U)
#define FLEXSPI_MCR0_SWRESET(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_SWRESET_SHIFT)) & FLEXSPI_MCR0_SWRESET_MASK)
#define FLEXSPI_MCR0_MDIS_MASK                   (0x2U)
#define FLEXSPI_MCR0_MDIS_SHIFT                  (1U)
#define FLEXSPI_MCR0_MDIS(x)                     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_MDIS_SHIFT)) & FLEXSPI_MCR0_MDIS_MASK)
#define FLEXSPI_MCR0_RXCLKSRC_MASK               (0x30U)
#define FLEXSPI_MCR0_RXCLKSRC_SHIFT              (4U)
#define FLEXSPI_MCR0_RXCLKSRC(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_RXCLKSRC_SHIFT)) & FLEXSPI_MCR0_RXCLKSRC_MASK)
#define FLEXSPI_MCR0_ARDFEN_MASK                 (0x40U)
#define FLEXSPI_MCR0_ARDFEN_SHIFT                (6U)
#define FLEXSPI_MCR0_ARDFEN(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_ARDFEN_SHIFT)) & FLEXSPI_MCR0_ARDFEN_MASK)
#define FLEXSPI_MCR0_ATDFEN_MASK                 (0x80U)
#define FLEXSPI_MCR0_ATDFEN_SHIFT                (7U)
#define FLEXSPI_MCR0_ATDFEN(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_ATDFEN_SHIFT)) & FLEXSPI_MCR0_ATDFEN_MASK)
#define FLEXSPI_MCR0_HSEN_MASK                   (0x800U)
#define FLEXSPI_MCR0_HSEN_SHIFT                  (11U)
#define FLEXSPI_MCR0_HSEN(x)                     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_HSEN_SHIFT)) & FLEXSPI_MCR0_HSEN_MASK)
#define FLEXSPI_MCR0_DOZEEN_MASK                 (0x1000U)
#define FLEXSPI_MCR0_DOZEEN_SHIFT                (12U)
#define FLEXSPI_MCR0_DOZEEN(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_DOZEEN_SHIFT)) & FLEXSPI_MCR0_DOZEEN_MASK)
#define FLEXSPI_MCR0_COMBINATIONEN_MASK          (0x2000U)
#define FLEXSPI_MCR0_COMBINATIONEN_SHIFT         (13U)
#define FLEXSPI_MCR0_COMBINATIONEN(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_COMBINATIONEN_SHIFT)) & FLEXSPI_MCR0_COMBINATIONEN_MASK)
#define FLEXSPI_MCR0_SCKFREERUNEN_MASK           (0x4000U)
#define FLEXSPI_MCR0_SCKFREERUNEN_SHIFT          (14U)
#define FLEXSPI_MCR0_SCKFREERUNEN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_SCKFREERUNEN_SHIFT)) & FLEXSPI_MCR0_SCKFREERUNEN_MASK)
#define FLEXSPI_MCR0_LEARNEN_MASK                (0x8000U)
#define FLEXSPI_MCR0_LEARNEN_SHIFT               (15U)
#define FLEXSPI_MCR0_LEARNEN(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_LEARNEN_SHIFT)) & FLEXSPI_MCR0_LEARNEN_MASK)
#define FLEXSPI_MCR0_IPGRANTWAIT_MASK            (0xFF0000U)
#define FLEXSPI_MCR0_IPGRANTWAIT_SHIFT           (16U)
#define FLEXSPI_MCR0_IPGRANTWAIT(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_IPGRANTWAIT_SHIFT)) & FLEXSPI_MCR0_IPGRANTWAIT_MASK)
#define FLEXSPI_MCR0_AHBGRANTWAIT_MASK           (0xFF000000U)
#define FLEXSPI_MCR0_AHBGRANTWAIT_SHIFT          (24U)
#define FLEXSPI_MCR0_AHBGRANTWAIT(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR0_AHBGRANTWAIT_SHIFT)) & FLEXSPI_MCR0_AHBGRANTWAIT_MASK)

/*! @name MCR1 - Module Control Register 1 */
#define FLEXSPI_MCR1_AHBBUSWAIT_MASK             (0xFFFFU)
#define FLEXSPI_MCR1_AHBBUSWAIT_SHIFT            (0U)
#define FLEXSPI_MCR1_AHBBUSWAIT(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR1_AHBBUSWAIT_SHIFT)) & FLEXSPI_MCR1_AHBBUSWAIT_MASK)
#define FLEXSPI_MCR1_SEQWAIT_MASK                (0xFFFF0000U)
#define FLEXSPI_MCR1_SEQWAIT_SHIFT               (16U)
#define FLEXSPI_MCR1_SEQWAIT(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR1_SEQWAIT_SHIFT)) & FLEXSPI_MCR1_SEQWAIT_MASK)

/*! @name MCR2 - Module Control Register 2 */
#define FLEXSPI_MCR2_CLRAHBBUFOPT_MASK           (0x800U)
#define FLEXSPI_MCR2_CLRAHBBUFOPT_SHIFT          (11U)
#define FLEXSPI_MCR2_CLRAHBBUFOPT(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR2_CLRAHBBUFOPT_SHIFT)) & FLEXSPI_MCR2_CLRAHBBUFOPT_MASK)
#define FLEXSPI_MCR2_CLRLEARNPHASE_MASK          (0x4000U)
#define FLEXSPI_MCR2_CLRLEARNPHASE_SHIFT         (14U)
#define FLEXSPI_MCR2_CLRLEARNPHASE(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR2_CLRLEARNPHASE_SHIFT)) & FLEXSPI_MCR2_CLRLEARNPHASE_MASK)
#define FLEXSPI_MCR2_SAMEDEVICEEN_MASK           (0x8000U)
#define FLEXSPI_MCR2_SAMEDEVICEEN_SHIFT          (15U)
#define FLEXSPI_MCR2_SAMEDEVICEEN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR2_SAMEDEVICEEN_SHIFT)) & FLEXSPI_MCR2_SAMEDEVICEEN_MASK)
#define FLEXSPI_MCR2_SCKBDIFFOPT_MASK            (0x80000U)
#define FLEXSPI_MCR2_SCKBDIFFOPT_SHIFT           (19U)
#define FLEXSPI_MCR2_SCKBDIFFOPT(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR2_SCKBDIFFOPT_SHIFT)) & FLEXSPI_MCR2_SCKBDIFFOPT_MASK)
#define FLEXSPI_MCR2_RESUMEWAIT_MASK             (0xFF000000U)
#define FLEXSPI_MCR2_RESUMEWAIT_SHIFT            (24U)
#define FLEXSPI_MCR2_RESUMEWAIT(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_MCR2_RESUMEWAIT_SHIFT)) & FLEXSPI_MCR2_RESUMEWAIT_MASK)

/*! @name AHBCR - AHB Bus Control Register */
#define FLEXSPI_AHBCR_APAREN_MASK                (0x1U)
#define FLEXSPI_AHBCR_APAREN_SHIFT               (0U)
#define FLEXSPI_AHBCR_APAREN(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBCR_APAREN_SHIFT)) & FLEXSPI_AHBCR_APAREN_MASK)
#define FLEXSPI_AHBCR_CACHABLEEN_MASK            (0x8U)
#define FLEXSPI_AHBCR_CACHABLEEN_SHIFT           (3U)
#define FLEXSPI_AHBCR_CACHABLEEN(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBCR_CACHABLEEN_SHIFT)) & FLEXSPI_AHBCR_CACHABLEEN_MASK)
#define FLEXSPI_AHBCR_BUFFERABLEEN_MASK          (0x10U)
#define FLEXSPI_AHBCR_BUFFERABLEEN_SHIFT         (4U)
#define FLEXSPI_AHBCR_BUFFERABLEEN(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBCR_BUFFERABLEEN_SHIFT)) & FLEXSPI_AHBCR_BUFFERABLEEN_MASK)
#define FLEXSPI_AHBCR_PREFETCHEN_MASK            (0x20U)
#define FLEXSPI_AHBCR_PREFETCHEN_SHIFT           (5U)
#define FLEXSPI_AHBCR_PREFETCHEN(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBCR_PREFETCHEN_SHIFT)) & FLEXSPI_AHBCR_PREFETCHEN_MASK)
#define FLEXSPI_AHBCR_READADDROPT_MASK           (0x40U)
#define FLEXSPI_AHBCR_READADDROPT_SHIFT          (6U)
#define FLEXSPI_AHBCR_READADDROPT(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBCR_READADDROPT_SHIFT)) & FLEXSPI_AHBCR_READADDROPT_MASK)

/*! @name INTEN - Interrupt Enable Register */
#define FLEXSPI_INTEN_IPCMDDONEEN_MASK           (0x1U)
#define FLEXSPI_INTEN_IPCMDDONEEN_SHIFT          (0U)
#define FLEXSPI_INTEN_IPCMDDONEEN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_IPCMDDONEEN_SHIFT)) & FLEXSPI_INTEN_IPCMDDONEEN_MASK)
#define FLEXSPI_INTEN_IPCMDGEEN_MASK             (0x2U)
#define FLEXSPI_INTEN_IPCMDGEEN_SHIFT            (1U)
#define FLEXSPI_INTEN_IPCMDGEEN(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_IPCMDGEEN_SHIFT)) & FLEXSPI_INTEN_IPCMDGEEN_MASK)
#define FLEXSPI_INTEN_AHBCMDGEEN_MASK            (0x4U)
#define FLEXSPI_INTEN_AHBCMDGEEN_SHIFT           (2U)
#define FLEXSPI_INTEN_AHBCMDGEEN(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_AHBCMDGEEN_SHIFT)) & FLEXSPI_INTEN_AHBCMDGEEN_MASK)
#define FLEXSPI_INTEN_IPCMDERREN_MASK            (0x8U)
#define FLEXSPI_INTEN_IPCMDERREN_SHIFT           (3U)
#define FLEXSPI_INTEN_IPCMDERREN(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_IPCMDERREN_SHIFT)) & FLEXSPI_INTEN_IPCMDERREN_MASK)
#define FLEXSPI_INTEN_AHBCMDERREN_MASK           (0x10U)
#define FLEXSPI_INTEN_AHBCMDERREN_SHIFT          (4U)
#define FLEXSPI_INTEN_AHBCMDERREN(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_AHBCMDERREN_SHIFT)) & FLEXSPI_INTEN_AHBCMDERREN_MASK)
#define FLEXSPI_INTEN_IPRXWAEN_MASK              (0x20U)
#define FLEXSPI_INTEN_IPRXWAEN_SHIFT             (5U)
#define FLEXSPI_INTEN_IPRXWAEN(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_IPRXWAEN_SHIFT)) & FLEXSPI_INTEN_IPRXWAEN_MASK)
#define FLEXSPI_INTEN_IPTXWEEN_MASK              (0x40U)
#define FLEXSPI_INTEN_IPTXWEEN_SHIFT             (6U)
#define FLEXSPI_INTEN_IPTXWEEN(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_IPTXWEEN_SHIFT)) & FLEXSPI_INTEN_IPTXWEEN_MASK)
#define FLEXSPI_INTEN_DATALEARNFAILEN_MASK       (0x80U)
#define FLEXSPI_INTEN_DATALEARNFAILEN_SHIFT      (7U)
#define FLEXSPI_INTEN_DATALEARNFAILEN(x)         (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_DATALEARNFAILEN_SHIFT)) & FLEXSPI_INTEN_DATALEARNFAILEN_MASK)
#define FLEXSPI_INTEN_SCKSTOPBYRDEN_MASK         (0x100U)
#define FLEXSPI_INTEN_SCKSTOPBYRDEN_SHIFT        (8U)
#define FLEXSPI_INTEN_SCKSTOPBYRDEN(x)           (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_SCKSTOPBYRDEN_SHIFT)) & FLEXSPI_INTEN_SCKSTOPBYRDEN_MASK)
#define FLEXSPI_INTEN_SCKSTOPBYWREN_MASK         (0x200U)
#define FLEXSPI_INTEN_SCKSTOPBYWREN_SHIFT        (9U)
#define FLEXSPI_INTEN_SCKSTOPBYWREN(x)           (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_SCKSTOPBYWREN_SHIFT)) & FLEXSPI_INTEN_SCKSTOPBYWREN_MASK)
#define FLEXSPI_INTEN_AHBBUSTIMEOUTEN_MASK       (0x400U)
#define FLEXSPI_INTEN_AHBBUSTIMEOUTEN_SHIFT      (10U)
#define FLEXSPI_INTEN_AHBBUSTIMEOUTEN(x)         (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_AHBBUSTIMEOUTEN_SHIFT)) & FLEXSPI_INTEN_AHBBUSTIMEOUTEN_MASK)
#define FLEXSPI_INTEN_SEQTIMEOUTEN_MASK          (0x800U)
#define FLEXSPI_INTEN_SEQTIMEOUTEN_SHIFT         (11U)
#define FLEXSPI_INTEN_SEQTIMEOUTEN(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTEN_SEQTIMEOUTEN_SHIFT)) & FLEXSPI_INTEN_SEQTIMEOUTEN_MASK)

/*! @name INTR - Interrupt Register */
#define FLEXSPI_INTR_IPCMDDONE_MASK              (0x1U)
#define FLEXSPI_INTR_IPCMDDONE_SHIFT             (0U)
#define FLEXSPI_INTR_IPCMDDONE(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_IPCMDDONE_SHIFT)) & FLEXSPI_INTR_IPCMDDONE_MASK)
#define FLEXSPI_INTR_IPCMDGE_MASK                (0x2U)
#define FLEXSPI_INTR_IPCMDGE_SHIFT               (1U)
#define FLEXSPI_INTR_IPCMDGE(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_IPCMDGE_SHIFT)) & FLEXSPI_INTR_IPCMDGE_MASK)
#define FLEXSPI_INTR_AHBCMDGE_MASK               (0x4U)
#define FLEXSPI_INTR_AHBCMDGE_SHIFT              (2U)
#define FLEXSPI_INTR_AHBCMDGE(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_AHBCMDGE_SHIFT)) & FLEXSPI_INTR_AHBCMDGE_MASK)
#define FLEXSPI_INTR_IPCMDERR_MASK               (0x8U)
#define FLEXSPI_INTR_IPCMDERR_SHIFT              (3U)
#define FLEXSPI_INTR_IPCMDERR(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_IPCMDERR_SHIFT)) & FLEXSPI_INTR_IPCMDERR_MASK)
#define FLEXSPI_INTR_AHBCMDERR_MASK              (0x10U)
#define FLEXSPI_INTR_AHBCMDERR_SHIFT             (4U)
#define FLEXSPI_INTR_AHBCMDERR(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_AHBCMDERR_SHIFT)) & FLEXSPI_INTR_AHBCMDERR_MASK)
#define FLEXSPI_INTR_IPRXWA_MASK                 (0x20U)
#define FLEXSPI_INTR_IPRXWA_SHIFT                (5U)
#define FLEXSPI_INTR_IPRXWA(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_IPRXWA_SHIFT)) & FLEXSPI_INTR_IPRXWA_MASK)
#define FLEXSPI_INTR_IPTXWE_MASK                 (0x40U)
#define FLEXSPI_INTR_IPTXWE_SHIFT                (6U)
#define FLEXSPI_INTR_IPTXWE(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_IPTXWE_SHIFT)) & FLEXSPI_INTR_IPTXWE_MASK)
#define FLEXSPI_INTR_DATALEARNFAIL_MASK          (0x80U)
#define FLEXSPI_INTR_DATALEARNFAIL_SHIFT         (7U)
#define FLEXSPI_INTR_DATALEARNFAIL(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_DATALEARNFAIL_SHIFT)) & FLEXSPI_INTR_DATALEARNFAIL_MASK)
#define FLEXSPI_INTR_SCKSTOPBYRD_MASK            (0x100U)
#define FLEXSPI_INTR_SCKSTOPBYRD_SHIFT           (8U)
#define FLEXSPI_INTR_SCKSTOPBYRD(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_SCKSTOPBYRD_SHIFT)) & FLEXSPI_INTR_SCKSTOPBYRD_MASK)
#define FLEXSPI_INTR_SCKSTOPBYWR_MASK            (0x200U)
#define FLEXSPI_INTR_SCKSTOPBYWR_SHIFT           (9U)
#define FLEXSPI_INTR_SCKSTOPBYWR(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_SCKSTOPBYWR_SHIFT)) & FLEXSPI_INTR_SCKSTOPBYWR_MASK)
#define FLEXSPI_INTR_AHBBUSTIMEOUT_MASK          (0x400U)
#define FLEXSPI_INTR_AHBBUSTIMEOUT_SHIFT         (10U)
#define FLEXSPI_INTR_AHBBUSTIMEOUT(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_AHBBUSTIMEOUT_SHIFT)) & FLEXSPI_INTR_AHBBUSTIMEOUT_MASK)
#define FLEXSPI_INTR_SEQTIMEOUT_MASK             (0x800U)
#define FLEXSPI_INTR_SEQTIMEOUT_SHIFT            (11U)
#define FLEXSPI_INTR_SEQTIMEOUT(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_INTR_SEQTIMEOUT_SHIFT)) & FLEXSPI_INTR_SEQTIMEOUT_MASK)

/*! @name LUTKEY - LUT Key Register */
#define FLEXSPI_LUTKEY_KEY_MASK                  (0xFFFFFFFFU)
#define FLEXSPI_LUTKEY_KEY_SHIFT                 (0U)
#define FLEXSPI_LUTKEY_KEY(x)                    (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUTKEY_KEY_SHIFT)) & FLEXSPI_LUTKEY_KEY_MASK)

/*! @name LUTCR - LUT Control Register */
#define FLEXSPI_LUTCR_LOCK_MASK                  (0x1U)
#define FLEXSPI_LUTCR_LOCK_SHIFT                 (0U)
#define FLEXSPI_LUTCR_LOCK(x)                    (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUTCR_LOCK_SHIFT)) & FLEXSPI_LUTCR_LOCK_MASK)
#define FLEXSPI_LUTCR_UNLOCK_MASK                (0x2U)
#define FLEXSPI_LUTCR_UNLOCK_SHIFT               (1U)
#define FLEXSPI_LUTCR_UNLOCK(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUTCR_UNLOCK_SHIFT)) & FLEXSPI_LUTCR_UNLOCK_MASK)

/*! @name AHBRXBUFCR0 - AHB RX Buffer 0 Control Register 0..AHB RX Buffer 3 Control Register 0 */
#define FLEXSPI_AHBRXBUFCR0_BUFSZ_MASK           (0xFFU)
#define FLEXSPI_AHBRXBUFCR0_BUFSZ_SHIFT          (0U)
#define FLEXSPI_AHBRXBUFCR0_BUFSZ(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBRXBUFCR0_BUFSZ_SHIFT)) & FLEXSPI_AHBRXBUFCR0_BUFSZ_MASK)
#define FLEXSPI_AHBRXBUFCR0_MSTRID_MASK          (0xF0000U)
#define FLEXSPI_AHBRXBUFCR0_MSTRID_SHIFT         (16U)
#define FLEXSPI_AHBRXBUFCR0_MSTRID(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBRXBUFCR0_MSTRID_SHIFT)) & FLEXSPI_AHBRXBUFCR0_MSTRID_MASK)
#define FLEXSPI_AHBRXBUFCR0_PRIORITY_MASK        (0x3000000U)
#define FLEXSPI_AHBRXBUFCR0_PRIORITY_SHIFT       (24U)
#define FLEXSPI_AHBRXBUFCR0_PRIORITY(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBRXBUFCR0_PRIORITY_SHIFT)) & FLEXSPI_AHBRXBUFCR0_PRIORITY_MASK)

/* The count of FLEXSPI_AHBRXBUFCR0 */
#define FLEXSPI_AHBRXBUFCR0_COUNT                (8U)

/*! @name FLSHCR0 - Flash A1 Control Register 0..Flash B2 Control Register 0 */
#define FLEXSPI_FLSHCR0_FLSHSZ_MASK              (0x7FFFFFU)
#define FLEXSPI_FLSHCR0_FLSHSZ_SHIFT             (0U)
#define FLEXSPI_FLSHCR0_FLSHSZ(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR0_FLSHSZ_SHIFT)) & FLEXSPI_FLSHCR0_FLSHSZ_MASK)

/* The count of FLEXSPI_FLSHCR0 */
#define FLEXSPI_FLSHCR0_COUNT                    (4U)

/*! @name FLSHCR1 - Flash A1 Control Register 1..Flash B2 Control Register 1 */
#define FLEXSPI_FLSHCR1_TCSS_MASK                (0x1FU)
#define FLEXSPI_FLSHCR1_TCSS_SHIFT               (0U)
#define FLEXSPI_FLSHCR1_TCSS(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR1_TCSS_SHIFT)) & FLEXSPI_FLSHCR1_TCSS_MASK)
#define FLEXSPI_FLSHCR1_TCSH_MASK                (0x3E0U)
#define FLEXSPI_FLSHCR1_TCSH_SHIFT               (5U)
#define FLEXSPI_FLSHCR1_TCSH(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR1_TCSH_SHIFT)) & FLEXSPI_FLSHCR1_TCSH_MASK)
#define FLEXSPI_FLSHCR1_WA_MASK                  (0x400U)
#define FLEXSPI_FLSHCR1_WA_SHIFT                 (10U)
#define FLEXSPI_FLSHCR1_WA(x)                    (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR1_WA_SHIFT)) & FLEXSPI_FLSHCR1_WA_MASK)
#define FLEXSPI_FLSHCR1_CAS_MASK                 (0x7800U)
#define FLEXSPI_FLSHCR1_CAS_SHIFT                (11U)
#define FLEXSPI_FLSHCR1_CAS(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR1_CAS_SHIFT)) & FLEXSPI_FLSHCR1_CAS_MASK)
#define FLEXSPI_FLSHCR1_CSINTERVALUNIT_MASK      (0x8000U)
#define FLEXSPI_FLSHCR1_CSINTERVALUNIT_SHIFT     (15U)
#define FLEXSPI_FLSHCR1_CSINTERVALUNIT(x)        (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR1_CSINTERVALUNIT_SHIFT)) & FLEXSPI_FLSHCR1_CSINTERVALUNIT_MASK)
#define FLEXSPI_FLSHCR1_CSINTERVAL_MASK          (0xFFFF0000U)
#define FLEXSPI_FLSHCR1_CSINTERVAL_SHIFT         (16U)
#define FLEXSPI_FLSHCR1_CSINTERVAL(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR1_CSINTERVAL_SHIFT)) & FLEXSPI_FLSHCR1_CSINTERVAL_MASK)

/* The count of FLEXSPI_FLSHCR1 */
#define FLEXSPI_FLSHCR1_COUNT                    (4U)

/*! @name FLSHCR2 - Flash A1 Control Register 2..Flash B2 Control Register 2 */
#define FLEXSPI_FLSHCR2_ARDSEQID_MASK            (0xFU)
#define FLEXSPI_FLSHCR2_ARDSEQID_SHIFT           (0U)
#define FLEXSPI_FLSHCR2_ARDSEQID(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_ARDSEQID_SHIFT)) & FLEXSPI_FLSHCR2_ARDSEQID_MASK)
#define FLEXSPI_FLSHCR2_ARDSEQNUM_MASK           (0xE0U)
#define FLEXSPI_FLSHCR2_ARDSEQNUM_SHIFT          (5U)
#define FLEXSPI_FLSHCR2_ARDSEQNUM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_ARDSEQNUM_SHIFT)) & FLEXSPI_FLSHCR2_ARDSEQNUM_MASK)
#define FLEXSPI_FLSHCR2_AWRSEQID_MASK            (0xF00U)
#define FLEXSPI_FLSHCR2_AWRSEQID_SHIFT           (8U)
#define FLEXSPI_FLSHCR2_AWRSEQID(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_AWRSEQID_SHIFT)) & FLEXSPI_FLSHCR2_AWRSEQID_MASK)
#define FLEXSPI_FLSHCR2_AWRSEQNUM_MASK           (0xE000U)
#define FLEXSPI_FLSHCR2_AWRSEQNUM_SHIFT          (13U)
#define FLEXSPI_FLSHCR2_AWRSEQNUM(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_AWRSEQNUM_SHIFT)) & FLEXSPI_FLSHCR2_AWRSEQNUM_MASK)
#define FLEXSPI_FLSHCR2_AWRWAIT_MASK             (0xFFF0000U)
#define FLEXSPI_FLSHCR2_AWRWAIT_SHIFT            (16U)
#define FLEXSPI_FLSHCR2_AWRWAIT(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_AWRWAIT_SHIFT)) & FLEXSPI_FLSHCR2_AWRWAIT_MASK)
#define FLEXSPI_FLSHCR2_AWRWAITUNIT_MASK         (0x70000000U)
#define FLEXSPI_FLSHCR2_AWRWAITUNIT_SHIFT        (28U)
#define FLEXSPI_FLSHCR2_AWRWAITUNIT(x)           (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_AWRWAITUNIT_SHIFT)) & FLEXSPI_FLSHCR2_AWRWAITUNIT_MASK)
#define FLEXSPI_FLSHCR2_CLRINSTRPTR_MASK         (0x80000000U)
#define FLEXSPI_FLSHCR2_CLRINSTRPTR_SHIFT        (31U)
#define FLEXSPI_FLSHCR2_CLRINSTRPTR(x)           (((uint32_t)(((uint32_t)(x)) << FLEXSPI_FLSHCR2_CLRINSTRPTR_SHIFT)) & FLEXSPI_FLSHCR2_CLRINSTRPTR_MASK)

/* The count of FLEXSPI_FLSHCR2 */
#define FLEXSPI_FLSHCR2_COUNT                    (4U)

/*! @name IPCR0 - IP Control Register 0 */
#define FLEXSPI_IPCR0_SFAR_MASK                  (0xFFFFFFFFU)
#define FLEXSPI_IPCR0_SFAR_SHIFT                 (0U)
#define FLEXSPI_IPCR0_SFAR(x)                    (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCR0_SFAR_SHIFT)) & FLEXSPI_IPCR0_SFAR_MASK)

/*! @name IPCR1 - IP Control Register 1 */
#define FLEXSPI_IPCR1_IDATSZ_MASK                (0xFFFFU)
#define FLEXSPI_IPCR1_IDATSZ_SHIFT               (0U)
#define FLEXSPI_IPCR1_IDATSZ(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCR1_IDATSZ_SHIFT)) & FLEXSPI_IPCR1_IDATSZ_MASK)
#define FLEXSPI_IPCR1_ISEQID_MASK                (0xF0000U)
#define FLEXSPI_IPCR1_ISEQID_SHIFT               (16U)
#define FLEXSPI_IPCR1_ISEQID(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCR1_ISEQID_SHIFT)) & FLEXSPI_IPCR1_ISEQID_MASK)
#define FLEXSPI_IPCR1_ISEQNUM_MASK               (0x7000000U)
#define FLEXSPI_IPCR1_ISEQNUM_SHIFT              (24U)
#define FLEXSPI_IPCR1_ISEQNUM(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCR1_ISEQNUM_SHIFT)) & FLEXSPI_IPCR1_ISEQNUM_MASK)
#define FLEXSPI_IPCR1_IPAREN_MASK                (0x80000000U)
#define FLEXSPI_IPCR1_IPAREN_SHIFT               (31U)
#define FLEXSPI_IPCR1_IPAREN(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCR1_IPAREN_SHIFT)) & FLEXSPI_IPCR1_IPAREN_MASK)

/*! @name IPCMD - IP Command Register */
#define FLEXSPI_IPCMD_TRG_MASK                   (0x1U)
#define FLEXSPI_IPCMD_TRG_SHIFT                  (0U)
#define FLEXSPI_IPCMD_TRG(x)                     (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPCMD_TRG_SHIFT)) & FLEXSPI_IPCMD_TRG_MASK)

/*! @name DLPR - Data Learn Pattern Register */
#define FLEXSPI_DLPR_DLP_MASK                    (0xFFFFFFFFU)
#define FLEXSPI_DLPR_DLP_SHIFT                   (0U)
#define FLEXSPI_DLPR_DLP(x)                      (((uint32_t)(((uint32_t)(x)) << FLEXSPI_DLPR_DLP_SHIFT)) & FLEXSPI_DLPR_DLP_MASK)

/*! @name IPRXFCR - IP RX FIFO Control Register */
#define FLEXSPI_IPRXFCR_CLRIPRXF_MASK            (0x1U)
#define FLEXSPI_IPRXFCR_CLRIPRXF_SHIFT           (0U)
#define FLEXSPI_IPRXFCR_CLRIPRXF(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPRXFCR_CLRIPRXF_SHIFT)) & FLEXSPI_IPRXFCR_CLRIPRXF_MASK)
#define FLEXSPI_IPRXFCR_RXDMAEN_MASK             (0x2U)
#define FLEXSPI_IPRXFCR_RXDMAEN_SHIFT            (1U)
#define FLEXSPI_IPRXFCR_RXDMAEN(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPRXFCR_RXDMAEN_SHIFT)) & FLEXSPI_IPRXFCR_RXDMAEN_MASK)
#define FLEXSPI_IPRXFCR_RXWMRK_MASK              (0x3CU)
#define FLEXSPI_IPRXFCR_RXWMRK_SHIFT             (2U)
#define FLEXSPI_IPRXFCR_RXWMRK(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPRXFCR_RXWMRK_SHIFT)) & FLEXSPI_IPRXFCR_RXWMRK_MASK)

/*! @name IPTXFCR - IP TX FIFO Control Register */
#define FLEXSPI_IPTXFCR_CLRIPTXF_MASK            (0x1U)
#define FLEXSPI_IPTXFCR_CLRIPTXF_SHIFT           (0U)
#define FLEXSPI_IPTXFCR_CLRIPTXF(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPTXFCR_CLRIPTXF_SHIFT)) & FLEXSPI_IPTXFCR_CLRIPTXF_MASK)
#define FLEXSPI_IPTXFCR_TXDMAEN_MASK             (0x2U)
#define FLEXSPI_IPTXFCR_TXDMAEN_SHIFT            (1U)
#define FLEXSPI_IPTXFCR_TXDMAEN(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPTXFCR_TXDMAEN_SHIFT)) & FLEXSPI_IPTXFCR_TXDMAEN_MASK)
#define FLEXSPI_IPTXFCR_TXWMRK_MASK              (0x3CU)
#define FLEXSPI_IPTXFCR_TXWMRK_SHIFT             (2U)
#define FLEXSPI_IPTXFCR_TXWMRK(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPTXFCR_TXWMRK_SHIFT)) & FLEXSPI_IPTXFCR_TXWMRK_MASK)

/*! @name DLLCR - DLL Control Register 0 */
#define FLEXSPI_DLLCR_DLLEN_MASK                 (0x1U)
#define FLEXSPI_DLLCR_DLLEN_SHIFT                (0U)
#define FLEXSPI_DLLCR_DLLEN(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_DLLCR_DLLEN_SHIFT)) & FLEXSPI_DLLCR_DLLEN_MASK)
#define FLEXSPI_DLLCR_DLLRESET_MASK              (0x2U)
#define FLEXSPI_DLLCR_DLLRESET_SHIFT             (1U)
#define FLEXSPI_DLLCR_DLLRESET(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_DLLCR_DLLRESET_SHIFT)) & FLEXSPI_DLLCR_DLLRESET_MASK)
#define FLEXSPI_DLLCR_SLVDLYTARGET_MASK          (0x78U)
#define FLEXSPI_DLLCR_SLVDLYTARGET_SHIFT         (3U)
#define FLEXSPI_DLLCR_SLVDLYTARGET(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_DLLCR_SLVDLYTARGET_SHIFT)) & FLEXSPI_DLLCR_SLVDLYTARGET_MASK)
#define FLEXSPI_DLLCR_OVRDEN_MASK                (0x100U)
#define FLEXSPI_DLLCR_OVRDEN_SHIFT               (8U)
#define FLEXSPI_DLLCR_OVRDEN(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_DLLCR_OVRDEN_SHIFT)) & FLEXSPI_DLLCR_OVRDEN_MASK)
#define FLEXSPI_DLLCR_OVRDVAL_MASK               (0x7E00U)
#define FLEXSPI_DLLCR_OVRDVAL_SHIFT              (9U)
#define FLEXSPI_DLLCR_OVRDVAL(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_DLLCR_OVRDVAL_SHIFT)) & FLEXSPI_DLLCR_OVRDVAL_MASK)

/* The count of FLEXSPI_DLLCR */
#define FLEXSPI_DLLCR_COUNT                      (2U)

/*! @name STS0 - Status Register 0 */
#define FLEXSPI_STS0_SEQIDLE_MASK                (0x1U)
#define FLEXSPI_STS0_SEQIDLE_SHIFT               (0U)
#define FLEXSPI_STS0_SEQIDLE(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS0_SEQIDLE_SHIFT)) & FLEXSPI_STS0_SEQIDLE_MASK)
#define FLEXSPI_STS0_ARBIDLE_MASK                (0x2U)
#define FLEXSPI_STS0_ARBIDLE_SHIFT               (1U)
#define FLEXSPI_STS0_ARBIDLE(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS0_ARBIDLE_SHIFT)) & FLEXSPI_STS0_ARBIDLE_MASK)
#define FLEXSPI_STS0_ARBCMDSRC_MASK              (0xCU)
#define FLEXSPI_STS0_ARBCMDSRC_SHIFT             (2U)
#define FLEXSPI_STS0_ARBCMDSRC(x)                (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS0_ARBCMDSRC_SHIFT)) & FLEXSPI_STS0_ARBCMDSRC_MASK)
#define FLEXSPI_STS0_DATALEARNPHASEA_MASK        (0xF0U)
#define FLEXSPI_STS0_DATALEARNPHASEA_SHIFT       (4U)
#define FLEXSPI_STS0_DATALEARNPHASEA(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS0_DATALEARNPHASEA_SHIFT)) & FLEXSPI_STS0_DATALEARNPHASEA_MASK)
#define FLEXSPI_STS0_DATALEARNPHASEB_MASK        (0xF00U)
#define FLEXSPI_STS0_DATALEARNPHASEB_SHIFT       (8U)
#define FLEXSPI_STS0_DATALEARNPHASEB(x)          (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS0_DATALEARNPHASEB_SHIFT)) & FLEXSPI_STS0_DATALEARNPHASEB_MASK)

/*! @name STS1 - Status Register 1 */
#define FLEXSPI_STS1_AHBCMDERRID_MASK            (0xFU)
#define FLEXSPI_STS1_AHBCMDERRID_SHIFT           (0U)
#define FLEXSPI_STS1_AHBCMDERRID(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS1_AHBCMDERRID_SHIFT)) & FLEXSPI_STS1_AHBCMDERRID_MASK)
#define FLEXSPI_STS1_AHBCMDERRCODE_MASK          (0xF00U)
#define FLEXSPI_STS1_AHBCMDERRCODE_SHIFT         (8U)
#define FLEXSPI_STS1_AHBCMDERRCODE(x)            (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS1_AHBCMDERRCODE_SHIFT)) & FLEXSPI_STS1_AHBCMDERRCODE_MASK)
#define FLEXSPI_STS1_IPCMDERRID_MASK             (0xF0000U)
#define FLEXSPI_STS1_IPCMDERRID_SHIFT            (16U)
#define FLEXSPI_STS1_IPCMDERRID(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS1_IPCMDERRID_SHIFT)) & FLEXSPI_STS1_IPCMDERRID_MASK)
#define FLEXSPI_STS1_IPCMDERRCODE_MASK           (0xF000000U)
#define FLEXSPI_STS1_IPCMDERRCODE_SHIFT          (24U)
#define FLEXSPI_STS1_IPCMDERRCODE(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS1_IPCMDERRCODE_SHIFT)) & FLEXSPI_STS1_IPCMDERRCODE_MASK)

/*! @name STS2 - Status Register 2 */
#define FLEXSPI_STS2_ASLVLOCK_MASK               (0x1U)
#define FLEXSPI_STS2_ASLVLOCK_SHIFT              (0U)
#define FLEXSPI_STS2_ASLVLOCK(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_ASLVLOCK_SHIFT)) & FLEXSPI_STS2_ASLVLOCK_MASK)
#define FLEXSPI_STS2_AREFLOCK_MASK               (0x2U)
#define FLEXSPI_STS2_AREFLOCK_SHIFT              (1U)
#define FLEXSPI_STS2_AREFLOCK(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_AREFLOCK_SHIFT)) & FLEXSPI_STS2_AREFLOCK_MASK)
#define FLEXSPI_STS2_ASLVSEL_MASK                (0xFCU)
#define FLEXSPI_STS2_ASLVSEL_SHIFT               (2U)
#define FLEXSPI_STS2_ASLVSEL(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_ASLVSEL_SHIFT)) & FLEXSPI_STS2_ASLVSEL_MASK)
#define FLEXSPI_STS2_AREFSEL_MASK                (0x3F00U)
#define FLEXSPI_STS2_AREFSEL_SHIFT               (8U)
#define FLEXSPI_STS2_AREFSEL(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_AREFSEL_SHIFT)) & FLEXSPI_STS2_AREFSEL_MASK)
#define FLEXSPI_STS2_BSLVLOCK_MASK               (0x10000U)
#define FLEXSPI_STS2_BSLVLOCK_SHIFT              (16U)
#define FLEXSPI_STS2_BSLVLOCK(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_BSLVLOCK_SHIFT)) & FLEXSPI_STS2_BSLVLOCK_MASK)
#define FLEXSPI_STS2_BREFLOCK_MASK               (0x20000U)
#define FLEXSPI_STS2_BREFLOCK_SHIFT              (17U)
#define FLEXSPI_STS2_BREFLOCK(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_BREFLOCK_SHIFT)) & FLEXSPI_STS2_BREFLOCK_MASK)
#define FLEXSPI_STS2_BSLVSEL_MASK                (0xFC0000U)
#define FLEXSPI_STS2_BSLVSEL_SHIFT               (18U)
#define FLEXSPI_STS2_BSLVSEL(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_BSLVSEL_SHIFT)) & FLEXSPI_STS2_BSLVSEL_MASK)
#define FLEXSPI_STS2_BREFSEL_MASK                (0x3F000000U)
#define FLEXSPI_STS2_BREFSEL_SHIFT               (24U)
#define FLEXSPI_STS2_BREFSEL(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_STS2_BREFSEL_SHIFT)) & FLEXSPI_STS2_BREFSEL_MASK)

/*! @name AHBSPNDSTS - AHB Suspend Status Register */
#define FLEXSPI_AHBSPNDSTS_ACTIVE_MASK           (0x1U)
#define FLEXSPI_AHBSPNDSTS_ACTIVE_SHIFT          (0U)
#define FLEXSPI_AHBSPNDSTS_ACTIVE(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBSPNDSTS_ACTIVE_SHIFT)) & FLEXSPI_AHBSPNDSTS_ACTIVE_MASK)
#define FLEXSPI_AHBSPNDSTS_BUFID_MASK            (0xEU)
#define FLEXSPI_AHBSPNDSTS_BUFID_SHIFT           (1U)
#define FLEXSPI_AHBSPNDSTS_BUFID(x)              (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBSPNDSTS_BUFID_SHIFT)) & FLEXSPI_AHBSPNDSTS_BUFID_MASK)
#define FLEXSPI_AHBSPNDSTS_DATLFT_MASK           (0xFFFF0000U)
#define FLEXSPI_AHBSPNDSTS_DATLFT_SHIFT          (16U)
#define FLEXSPI_AHBSPNDSTS_DATLFT(x)             (((uint32_t)(((uint32_t)(x)) << FLEXSPI_AHBSPNDSTS_DATLFT_SHIFT)) & FLEXSPI_AHBSPNDSTS_DATLFT_MASK)

/*! @name IPRXFSTS - IP RX FIFO Status Register */
#define FLEXSPI_IPRXFSTS_FILL_MASK               (0xFFU)
#define FLEXSPI_IPRXFSTS_FILL_SHIFT              (0U)
#define FLEXSPI_IPRXFSTS_FILL(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPRXFSTS_FILL_SHIFT)) & FLEXSPI_IPRXFSTS_FILL_MASK)
#define FLEXSPI_IPRXFSTS_RDCNTR_MASK             (0xFFFF0000U)
#define FLEXSPI_IPRXFSTS_RDCNTR_SHIFT            (16U)
#define FLEXSPI_IPRXFSTS_RDCNTR(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPRXFSTS_RDCNTR_SHIFT)) & FLEXSPI_IPRXFSTS_RDCNTR_MASK)

/*! @name IPTXFSTS - IP TX FIFO Status Register */
#define FLEXSPI_IPTXFSTS_FILL_MASK               (0xFFU)
#define FLEXSPI_IPTXFSTS_FILL_SHIFT              (0U)
#define FLEXSPI_IPTXFSTS_FILL(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPTXFSTS_FILL_SHIFT)) & FLEXSPI_IPTXFSTS_FILL_MASK)
#define FLEXSPI_IPTXFSTS_WRCNTR_MASK             (0xFFFF0000U)
#define FLEXSPI_IPTXFSTS_WRCNTR_SHIFT            (16U)
#define FLEXSPI_IPTXFSTS_WRCNTR(x)               (((uint32_t)(((uint32_t)(x)) << FLEXSPI_IPTXFSTS_WRCNTR_SHIFT)) & FLEXSPI_IPTXFSTS_WRCNTR_MASK)

/*! @name RFDR - IP RX FIFO Data Register 0..IP RX FIFO Data Register 31 */
#define FLEXSPI_RFDR_RXDATA_MASK                 (0xFFFFFFFFU)
#define FLEXSPI_RFDR_RXDATA_SHIFT                (0U)
#define FLEXSPI_RFDR_RXDATA(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_RFDR_RXDATA_SHIFT)) & FLEXSPI_RFDR_RXDATA_MASK)

/* The count of FLEXSPI_RFDR */
#define FLEXSPI_RFDR_COUNT                       (32U)

/*! @name TFDR - IP TX FIFO Data Register 0..IP TX FIFO Data Register 31 */
#define FLEXSPI_TFDR_TXDATA_MASK                 (0xFFFFFFFFU)
#define FLEXSPI_TFDR_TXDATA_SHIFT                (0U)
#define FLEXSPI_TFDR_TXDATA(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_TFDR_TXDATA_SHIFT)) & FLEXSPI_TFDR_TXDATA_MASK)

/* The count of FLEXSPI_TFDR */
#define FLEXSPI_TFDR_COUNT                       (32U)

/*! @name LUT - LUT 0..LUT 63 */
#define FLEXSPI_LUT_OPERAND0_MASK                (0xFFU)
#define FLEXSPI_LUT_OPERAND0_SHIFT               (0U)
#define FLEXSPI_LUT_OPERAND0(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUT_OPERAND0_SHIFT)) & FLEXSPI_LUT_OPERAND0_MASK)
#define FLEXSPI_LUT_NUM_PADS0_MASK               (0x300U)
#define FLEXSPI_LUT_NUM_PADS0_SHIFT              (8U)
#define FLEXSPI_LUT_NUM_PADS0(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUT_NUM_PADS0_SHIFT)) & FLEXSPI_LUT_NUM_PADS0_MASK)
#define FLEXSPI_LUT_OPCODE0_MASK                 (0xFC00U)
#define FLEXSPI_LUT_OPCODE0_SHIFT                (10U)
#define FLEXSPI_LUT_OPCODE0(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUT_OPCODE0_SHIFT)) & FLEXSPI_LUT_OPCODE0_MASK)
#define FLEXSPI_LUT_OPERAND1_MASK                (0xFF0000U)
#define FLEXSPI_LUT_OPERAND1_SHIFT               (16U)
#define FLEXSPI_LUT_OPERAND1(x)                  (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUT_OPERAND1_SHIFT)) & FLEXSPI_LUT_OPERAND1_MASK)
#define FLEXSPI_LUT_NUM_PADS1_MASK               (0x3000000U)
#define FLEXSPI_LUT_NUM_PADS1_SHIFT              (24U)
#define FLEXSPI_LUT_NUM_PADS1(x)                 (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUT_NUM_PADS1_SHIFT)) & FLEXSPI_LUT_NUM_PADS1_MASK)
#define FLEXSPI_LUT_OPCODE1_MASK                 (0xFC000000U)
#define FLEXSPI_LUT_OPCODE1_SHIFT                (26U)
#define FLEXSPI_LUT_OPCODE1(x)                   (((uint32_t)(((uint32_t)(x)) << FLEXSPI_LUT_OPCODE1_SHIFT)) & FLEXSPI_LUT_OPCODE1_MASK)

/* The count of FLEXSPI_LUT */
#define FLEXSPI_LUT_COUNT                        (64U)

#define FLEXSPI_LUT_KEY_VAL     (0x5AF05AF0ul)

#define CMD_INDEX_READ              0
#define CMD_INDEX_READSTATUS        1
#define CMD_INDEX_WRITEENABLE       2
#define CMD_INDEX_WRITE             4

#define CMD_LUT_SEQ_IDX_READ            0
#define CMD_LUT_SEQ_IDX_READSTATUS      1
#define CMD_LUT_SEQ_IDX_WRITEENABLE     3
#define CMD_LUT_SEQ_IDX_WRITE           9

#define NOR_CMD_LUT_SEQ_IDX_READ            CMD_LUT_SEQ_IDX_READ
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS      CMD_LUT_SEQ_IDX_READSTATUS
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS_XPI  2
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE     CMD_LUT_SEQ_IDX_WRITEENABLE
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE_XPI 4
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR     5
#define NOR_CMD_LUT_SEQ_IDX_ERASEBLOCK      8
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM     CMD_LUT_SEQ_IDX_WRITE
#define NOR_CMD_LUT_SEQ_IDX_CHIPERASE       11
#define NOR_CMD_LUT_SEQ_IDX_READ_ID         12
#define NOR_CMD_LUT_SEQ_IDX_READ_SFDP       13
#define NOR_CMD_LUT_SEQ_IDX_RESTORE_NOCMD   14
#define NOR_CMD_LUT_SEQ_IDX_EXIT_NOCMD      15

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

#define FLEXSPI_1PAD    0
#define FLEXSPI_2PAD    1
#define FLEXSPI_4PAD    2
#define FLEXSPI_8PAD    3

typedef enum {
    FLEXSPI_CMD_WRITE,
    FLEXSPI_CMD_READ,
    FLEXSPI_CMD_OPER,
} flexspi_ip_cmd_op_e;

typedef struct flexspi_ip_cmd_tag{

    uint8_t *dst;
    uint8_t *src;
    uint32_t size;
    flexspi_ip_cmd_op_e op;
    uint8_t seq_id;
    uint8_t seq_size;
    uint8_t is_parallel;
} flexspi_ip_cmd_t;

typedef struct flexspi_instance_tag {
    flexspi_t *flexspi_reg;
    uint32_t instance;
} flexspi_instance_t;

extern uint32_t imx_flexspi_ip_command(flexspi_t *flexspi_reg, flexspi_ip_cmd_t *flexspi_ip_cmd_p);
extern uint32_t imx_flexspi_init(flexspi_instance_t *flexspi_instance);
extern void imx_flexspi_update_lut(flexspi_t *flexspi_reg, uint32_t seq_idx, uint32_t seq[]);
extern void imx_flexspi_swreset(flexspi_t *flexspi_reg);
extern flexspi_instance_t g_flexspi_instance_arr[2];

/* System level port interface */
extern void imx_flexspi_clock_gate_disable(uint32_t instance);
extern void imx_flexspi_clock_config(uint32_t instance);
extern void imx_flexspi_clock_gate_enable(uint32_t instance);
extern void imx_flexspi_iomux_config(uint32_t instance);


#endif /* IMX_FLEXSPI_H */
