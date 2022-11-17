/*******************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>

#include <Protocol/PciIo.h>
#include <Protocol/SdMmcOverride.h>

#include "XenonPciHci.h"

#define SD_BAR_INDEX 0

#define SIZE_512B    0x200

/* Register Offset of SD Host Controller */
#define SDHC_SDMA_ADDR                0x0000
#define SDHC_ARG2                     0x0000
#define SDHC_BLK_SIZE                 0x0004
#define SDHC_BLK_COUNT                0x0006
#define SDHC_ARG1                     0x0008
#define SDHC_TRANS_MOD                0x000C
#define SDHC_COMMAND                  0x000E
#define SDHC_RESPONSE                 0x0010
#define SDHC_BUF_DAT_PORT             0x0020
#define SDHC_PRESENT_STATE            0x0024
#define SDHC_HOST_CTRL1               0x0028
#define SDHC_POWER_CTRL               0x0029
#define SDHC_BLK_GAP_CTRL             0x002A
#define SDHC_WAKEUP_CTRL              0x002B
#define SDHC_CLOCK_CTRL               0x002C
#define SDHC_TIMEOUT_CTRL             0x002E
#define SDHC_SW_RST                   0x002F
#define SDHC_NOR_INT_STS              0x0030
#define SDHC_ERR_INT_STS              0x0032
#define SDHC_NOR_INT_STS_EN           0x0034
#define SDHC_ERR_INT_STS_EN           0x0036
#define SDHC_NOR_INT_SIG_EN           0x0038
#define SDHC_ERR_INT_SIG_EN           0x003A
#define SDHC_AUTO_CMD_ERR_STS         0x003C
#define SDHC_HOST_CTRL2               0x003E
#define UHS_MODE_SELECT_MASK          0x7
#define SDHC_CAP                      0x0040
#define SDHC_CAP_BUS_WIDTH8           BIT18
#define SDHC_CAP_VOLTAGE_33           BIT24
#define SDHC_CAP_VOLTAGE_30           BIT25
#define SDHC_CAP_VOLTAGE_18           BIT26
#define SDHC_CAP_SLOT_TYPE_OFFSET     30
#define SDHC_CAP_SLOT_TYPE_MASK       (BIT30 | BIT31)
#define SDHC_CAP_SDR50                BIT32
#define SDHC_CAP_SDR104               BIT33
#define SDHC_CAP_DDR50                BIT34
#define SDHC_CAP_HS400                BIT63
#define SDHC_MAX_CURRENT_CAP          0x0048
#define SDHC_FORCE_EVT_AUTO_CMD       0x0050
#define SDHC_FORCE_EVT_ERR_INT        0x0052
#define SDHC_ADMA_ERR_STS             0x0054
#define SDHC_ADMA_SYS_ADDR            0x0058
#define SDHC_PRESET_VAL               0x0060
#define SDHC_SHARED_BUS_CTRL          0x00E0
#define SDHC_SLOT_INT_STS             0x00FC
#define SDHC_CTRL_VER                 0x00FE

#define SDHC_IPID                     0x0100
#define SDHC_SYS_CFG_INFO             0x0104
#define SLOT_TYPE_SDIO_SHIFT          24
#define SLOT_TYPE_EMMC_MASK           0xff
#define SLOT_TYPE_EMMC_SHIFT          16
#define SLOT_TYPE_SD_SDIO_MMC_MASK    0xff
#define SLOT_TYPE_SD_SDIO_MMC_SHIFT   8

#define SDHC_SYS_OP_CTRL              0x0108
#define AUTO_CLKGATE_DISABLE_MASK     (0x1<<20)
#define SDCLK_IDLEOFF_ENABLE_MASK     (1 << 8)
#define SLOT_ENABLE_SHIFT             0

#define SDHC_SYS_EXT_OP_CTRL          0x010c
#define MASK_CMD_CONFLICT_ERR         (1 << 8)

#define SDHC_TEST_OUT                 0x0110
#define SDHC_TESTOUT_MUXSEL           0x0114

#define SDHC_SLOT_EXT_INT_STATUS      0x0120
#define SDHC_SLOT_EXT_ERR_STATUS      0x0122
#define SDHC_SLOT_EXT_INT_STATUS_EN   0x0124
#define SDHC_SLOT_EXT_ERR_STATUS_EN   0x0126

#define SDHC_SLOT_OP_STATUS_CTRL      0x0128
#define TUNING_PROG_FIXED_DELAY_MASK  0x7ff
#define FORCE_SEL_INVERSE_CLK_SHIFT   11

#define SDHC_SLOT_FIFO_CTRL           0x012c
#define SDHC_SLOT_FIFO_DEFAULT_CONFIG 0x315

#define SDHC_SLOT_eMMC_CTRL           0x0130
#define ENABLE_DATA_STROBE_SHIFT      24
#define SET_EMMC_RSTN_SHIFT           16
#define eMMC_VCCQ_MASK                0x3
#define eMMC_VCCQ_1_8V                0x1
#define eMMC_VCCQ_1_2V                0x2
#define eMMC_VCCQ_3_3V                0x3

#define SDHC_SLOT_OUTPUT_DLY_CTRL     0x0134
#define SDHC_SLOT_DCM_CTRL            0x0137

#define SDHC_SLOT_DLL_CTRL            0x0138
#define SELECT_DEF_DLL                0x1

#define SDHC_SLOT_DLL_PHASE_SEL       0x013c
#define DLL_UPDATE_STROBE             7

#define SDHC_SLOT_STROBE_DLY_CTRL     0x0140
#define STROBE_DELAY_FIXED_MASK       0xffff

#define SDHC_SLOT_RETUNING_REQ_CTRL   0x0144
#define RETUNING_COMPATIBLE           0x1

#define SDHC_SLOT_AUTO_RETUNING_CTRL  0x0148
#define ENABLE_AUTO_RETUNING          0x1

#define SDHC_SLOT_EXT_PRESENT_STATE   0x014c
#define SDHC_SLOT_DLL_CUR_DLY_VAL     0x0150
#define SDHC_SLOT_TUNING_CUR_DLY_VAL  0x0154
#define SDHC_SLOT_STROBE_CUR_DLY_VAL  0x0158
#define SDHC_SLOT_SUB_CMD_STRL        0x015c

#define SDHC_SLOT_CQ_TASK_INFO        0x0160

#define SDHC_SLOT_TUNING_DEBUG_INFO   0x01f0
#define SDHC_SLOT_DATAIN_DEBUG_INFO   0x01f4

#define SDHC_CMD_RESET_BIT            (1 << 1)
#define SDHC_DATA_RESET_BIT           (1 << 2)

#define SDHC_CMD_INHIBIT              (1 << 0)
#define SDHC_DATA_INHIBIT             (1 << 1)

#define SDHC_REG_SIZE_1B              1
#define SDHC_REG_SIZE_2B              2
#define SDHC_REG_SIZE_4B              4

#define SDHC_DAT_BUF_PORT_ADDR        0xF06E0020

/* Command register bits description */
#define RESP_TYPE_136_BITS            (1 << 0)
#define RESP_TYPE_48_BITS             (1 << 1)
#define RESP_TYPE_48_BITS_NO_CRC      ((1 << 0) | (1 << 1))
#define CMD_CRC_CHK_EN                (1 << 3)
#define CMD_INDEX_CHK_EN              (1 << 4)
#define DATA_PRESENT                  (1 << 5)

/* Transfer mode register bits description */
#define SDHC_TRNS_BLK_CNT_EN          (1 << 1)
#define SDHC_TRNS_MULTI_BLK_SEL       (1 << 5)
#define SDHC_TRNS_TO_HOST_DIR         (1 << 4)

/* Block size register bits description */
#define BLK_SIZE_HOST_DMA_BDRY_OFFSET 12
#define BLK_SIZE_512KB                0x7

/* Int status register bits description */
#define NOR_INT_STS_CMD_COMPLETE      (1 << 0)
#define NOR_INT_STS_XFER_COMPLETE     (1 << 1)
#define NOR_INT_STS_TX_RDY            (1 << 4)
#define NOR_INT_STS_RX_RDY            (1 << 5)
#define NOR_INT_STS_CARD_INS          (1 << 6)
#define NOR_INT_STS_CARD_INT          (1 << 8)
#define NOR_INT_STS_ERR_INT           (1 << 15)
#define ERR_INT_STS_CMD_TIMEOUT_ERR   (1 << 16)
#define ERR_INT_STS_DATA_TIMEOUT_ERR  (1 << 20)

#define NOR_INT_SIG_EN_CARD_INT       (1 << 8)

#define PRESENT_STATE_BUFFER_RD_EN    (1 << 11)
#define PRESENT_STATE_BUFFER_WR_EN    (1 << 10)

#define SDHC_CLR_IRQ_STS_MASK         0xFFFF
#define SDHC_CLR_ALL_IRQ_MASK         0xFFFFFFFFUL

/* Max clock in Hz */
#define XENON_MMC_MAX_CLK             400000000
#define XENON_MMC_CLK_RATIO           2046
#define XENON_MMC_BASE_CLK            XENON_MMC_MAX_CLK / XENON_MMC_CLK_RATIO
#define XENON_MMC_MIN_CLK             100000

#define XENON_MMC_CMD_MAX_TIMEOUT     3200
#define XENON_MMC_CMD_DEFAULT_TIMEOUT 100

/* Tuning Parameter */
#define TMR_RETUN_NO_PRESENT          0xf
#define XENON_MAX_TUN_COUNT           0xb

#define XENON_SLOT_OP_STATUS_CTRL     0x0128
#define TUN_CONSECUTIVE_TIMES_SHIFT   16
#define TUN_CONSECUTIVE_TIMES_MASK    0x7
#define TUN_CONSECUTIVE_TIMES         0x4
#define TUNING_STEP_SHIFT             12
#define TUNING_STEP_MASK              0xF

#define XENON_SLOT_EMMC_CTRL          0x130
#define ENABLE_DATA_STROBE            (1 << 24)

#define XENON_SLOT_EXT_PRESENT_STATE  0x014C
#define DLL_LOCK_STATE                0x1

#define XENON_SLOT_DLL_CUR_DLY_VAL    0x0150

#define EMMC_PHY_REG_BASE                 0x170
#define EMMC_PHY_TIMING_ADJUST            EMMC_PHY_REG_BASE
#define OUTPUT_QSN_PHASE_SELECT           (1 << 17)
#define SAMPL_INV_QSP_PHASE_SELECT        (1 << 18)
#define SAMPL_INV_QSP_PHASE_SELECT_SHIFT  18
#define QSN_PHASE_SLOW_MODE_BIT           (1 << 29)
#define PHY_INITIALIZAION                 (1 << 31)
#define WAIT_CYCLE_BEFORE_USING_MASK      0xf
#define WAIT_CYCLE_BEFORE_USING_SHIFT     12
#define FC_SYNC_EN_DURATION_MASK          0xf
#define FC_SYNC_EN_DURATION_SHIFT         8
#define FC_SYNC_RST_EN_DURATION_MASK      0xf
#define FC_SYNC_RST_EN_DURATION_SHIFT     4
#define FC_SYNC_RST_DURATION_MASK         0xf
#define FC_SYNC_RST_DURATION_SHIFT        0

#define EMMC_PHY_FUNC_CONTROL     (EMMC_PHY_REG_BASE + 0x4)
#define DQ_ASYNC_MODE             (1 << 4)
#define DQ_DDR_MODE_SHIFT         8
#define DQ_DDR_MODE_MASK          0xff
#define CMD_DDR_MODE              (1 << 16)

#define EMMC_PHY_PAD_CONTROL      (EMMC_PHY_REG_BASE + 0x8)
#define REC_EN_SHIFT              24
#define REC_EN_MASK               0xf
#define FC_DQ_RECEN               (1 << 24)
#define FC_CMD_RECEN              (1 << 25)
#define FC_QSP_RECEN              (1 << 26)
#define FC_QSN_RECEN              (1 << 27)
#define OEN_QSN                   (1 << 28)
#define AUTO_RECEN_CTRL           (1 << 30)
#define FC_ALL_CMOS_RECEIVER      0xF000

#define EMMC_PHY_PAD_CONTROL1        (EMMC_PHY_REG_BASE + 0xc)
#define EMMC5_1_FC_QSP_PD            (1 << 9)
#define EMMC5_1_FC_QSP_PU            (1 << 25)
#define EMMC5_1_FC_CMD_PD            (1 << 8)
#define EMMC5_1_FC_CMD_PU            (1 << 24)
#define EMMC5_1_FC_DQ_PD             0xFF
#define EMMC5_1_FC_DQ_PU             (0xFF << 16)

#define EMMC_PHY_PAD_CONTROL2        (EMMC_PHY_REG_BASE + 0x10)
#define ZNR_MASK                     0x1F
#define ZNR_SHIFT                    8
#define ZPR_MASK                     0x1F
#define ZNR_DEF_VALUE                0xF
#define ZPR_DEF_VALUE                0xF

#define EMMC_PHY_DLL_CONTROL         (EMMC_PHY_REG_BASE + 0x14)
#define DLL_ENABLE                   (1 << 31)
#define DLL_UPDATE_STROBE_5_0        (1 << 30)
#define DLL_REFCLK_SEL               (1 << 30)
#define DLL_UPDATE                   (1 << 23)
#define DLL_PHSEL1_SHIFT             24
#define DLL_PHSEL0_SHIFT             16
#define DLL_PHASE_MASK               0x3F
#define DLL_PHASE_90_DEGREE          0x1F
#define DLL_FAST_LOCK                (1 << 5)
#define DLL_GAIN2X                   (1 << 3)
#define DLL_BYPASS_EN                (1 << 0)

#define EMMC_LOGIC_TIMING_ADJUST       (EMMC_PHY_REG_BASE + 0x18)
#define EMMC_LOGIC_TIMING_ADJUST_LOW   (EMMC_PHY_REG_BASE + 0x1c)

#define LOGIC_TIMING_VALUE             0x5a54 /* Recommend by HW team */

#define TUNING_STEP_DIVIDER_SHIFT      6

/* XENON only have one slot 0 */
#define XENON_MMC_SLOT_ID              (0)

#define MMC_TIMING_LEGACY     0
#define MMC_TIMING_MMC_HS     1
#define MMC_TIMING_SD_HS      2
#define MMC_TIMING_UHS_SDR12  3
#define MMC_TIMING_UHS_SDR25  4
#define MMC_TIMING_UHS_SDR50  5
#define MMC_TIMING_UHS_SDR104 6
#define MMC_TIMING_UHS_DDR50  7
#define MMC_TIMING_MMC_HS200  8
#define MMC_TIMING_MMC_HS400  10
#define MMC_TIMING_MMC_DDR52  11

/* Custom UHS signaling field values */
#define XENON_SD_MMC_HC_CTRL_HS200    0x5
#define XENON_SD_MMC_HC_CTRL_HS400    0x6

/* Data time out default value 0xE: TMCLK x 227 */
#define DATA_TIMEOUT_DEF_VAL          0xE

/* Max retry count for INT status ready */
#define SDHC_INT_STATUS_POLL_RETRY              1000
#define SDHC_INT_STATUS_POLL_RETRY_DATA_TRAN    100000

/* Take 2.5 seconds as generic time out value, 1 microsecond as unit */
#define SD_GENERIC_TIMEOUT            2500 * 1000

/* SDMA start address should allign with 0x8, align mask 0x7 */
#define DMA_START_ADDR_ALIGN_MASK 0x7

#define  SDHCI_RETUNE_EVT_INTSIG  0x00001000

/* MMC modes */
#define XENON_MMC_MODE_EMMC     0
#define XENON_MMC_MODE_SD_SDIO  1

/* MMC Voltage */
#define MMC_VDD_165_195   0x00000080  /* VDD voltage 1.65 - 1.95 */
#define MMC_VDD_20_21     0x00000100  /* VDD voltage 2.0 ~ 2.1 */
#define MMC_VDD_21_22     0x00000200  /* VDD voltage 2.1 ~ 2.2 */
#define MMC_VDD_22_23     0x00000400  /* VDD voltage 2.2 ~ 2.3 */
#define MMC_VDD_23_24     0x00000800  /* VDD voltage 2.3 ~ 2.4 */
#define MMC_VDD_24_25     0x00001000  /* VDD voltage 2.4 ~ 2.5 */
#define MMC_VDD_25_26     0x00002000  /* VDD voltage 2.5 ~ 2.6 */
#define MMC_VDD_26_27     0x00004000  /* VDD voltage 2.6 ~ 2.7 */
#define MMC_VDD_27_28     0x00008000  /* VDD voltage 2.7 ~ 2.8 */
#define MMC_VDD_28_29     0x00010000  /* VDD voltage 2.8 ~ 2.9 */
#define MMC_VDD_29_30     0x00020000  /* VDD voltage 2.9 ~ 3.0 */
#define MMC_VDD_30_31     0x00040000  /* VDD voltage 3.0 ~ 3.1 */
#define MMC_VDD_31_32     0x00080000  /* VDD voltage 3.1 ~ 3.2 */
#define MMC_VDD_32_33     0x00100000  /* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_33_34     0x00200000  /* VDD voltage 3.3 ~ 3.4 */
#define MMC_VDD_34_35     0x00400000  /* VDD voltage 3.4 ~ 3.5 */
#define MMC_VDD_35_36     0x00800000  /* VDD voltage 3.5 ~ 3.6 */

/* SDHCI FLAGS */
#define SDHCI_POWER_ON   0x01
#define SDHCI_POWER_180  0x0A
#define SDHCI_POWER_300  0x0C
#define SDHCI_POWER_330  0x0E

#define SDHCI_DIVIDER_SHIFT     8
#define SDHCI_DIVIDER_HI_SHIFT  6
#define SDHCI_DIV_MASK          0xFF
#define SDHCI_DIV_MASK_LEN      8
#define SDHCI_DIV_HI_MASK       0x300
#define SDHCI_CLOCK_CARD_EN     0x0004
#define SDHCI_CLOCK_INT_STABLE  0x0002
#define SDHCI_CLOCK_INT_EN      0x0001

#define SDHCI_VENDOR_VER_MASK   0xFF00
#define SDHCI_VENDOR_VER_SHIFT  8
#define SDHCI_SPEC_VER_MASK     0x00FF
#define SDHCI_SPEC_VER_SHIFT    0
#define SDHCI_SPEC_100          0
#define SDHCI_SPEC_200          1
#define SDHCI_SPEC_300          2

#define SDHCI_SIGNAL_ENABLE     0x38

#define SDHCI_MAX_DIV_SPEC_200  256
#define SDHCI_MAX_DIV_SPEC_300  2046

/* SD DEVICE STATUS FLAGS */
#define CURRENT_STATE_MASK           (0xF << 9)
#define CURRENT_STATE_N_READY_MASK   (7 << 9)
#define READY_FOR_DATA               (1 << 8)
#define CARD_STATUS_ERROR_MASK       (~0x0206BF7F)

#define RCA_BITS_OFFSET              16

UINTN
XenonSetClk (
  IN EFI_PCI_IO_PROTOCOL   *PciIo,
  IN UINT32 Clock
  );

VOID
XenonPhyInit (
  IN EFI_PCI_IO_PROTOCOL   *PciIo
  );

VOID
XenonReset (
  IN EFI_PCI_IO_PROTOCOL *PciIo,
  IN UINT8 Slot,
  IN UINT8 Mask
  );

EFI_STATUS
XenonTransferData (
  IN EFI_PCI_IO_PROTOCOL *PciIo,
  IN UINT8 Slot,
  IN OUT VOID *Buffer,
  IN UINT32 DataLen,
  IN UINT16 BlockSize,
  IN UINT16 Blocks,
  IN BOOLEAN Read
  );

EFI_STATUS
XenonInit (
  IN EFI_PCI_IO_PROTOCOL   *PciIo,
  IN BOOLEAN               Support1v8,
  IN BOOLEAN               SlowMode,
  IN UINT8                 TuningStepDivisor
  );

EFI_STATUS
XenonSetPhy (
  IN EFI_PCI_IO_PROTOCOL   *PciIo,
  IN BOOLEAN               SlowMode,
  IN UINT8                 TuningStepDivisor,
  IN SD_MMC_BUS_MODE       Timing
  );
