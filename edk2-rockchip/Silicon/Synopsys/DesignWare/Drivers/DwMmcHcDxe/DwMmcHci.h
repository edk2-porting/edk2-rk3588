/** @file

  Provides some data structure definitions used by the SD/MMC host controller
  driver.

  Copyright (c) 2015-2021, Intel Corporation. All rights reserved.
  Copyright (c) 2018, Linaro Ltd. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DW_MMC_HCI_H_
#define _DW_MMC_HCI_H_

#include <Library/CacheMaintenanceLib.h>
#include <Library/TimerLib.h>

#include <Protocol/PlatformDwMmc.h>

//
// SD Host Controller SlotInfo Register Offset
//
#define DW_MMC_HC_SLOT_OFFSET  0x40

#define DW_MMC_HC_MAX_SLOT  1

//
// SD Host Controller MMIO Register Offset
//
#define DW_MMC_CTRL          0x000
#define DW_MMC_PWREN         0x004
#define DW_MMC_CLKDIV        0x008
#define DW_MMC_CLKSRC        0x00c
#define DW_MMC_CLKENA        0x010
#define DW_MMC_TMOUT         0x014
#define DW_MMC_CTYPE         0x018
#define DW_MMC_BLKSIZ        0x01c
#define DW_MMC_BYTCNT        0x020
#define DW_MMC_INTMASK       0x024
#define DW_MMC_CMDARG        0x028
#define DW_MMC_CMD           0x02c
#define DW_MMC_RESP0         0x030
#define DW_MMC_RESP1         0x034
#define DW_MMC_RESP2         0x038
#define DW_MMC_RESP3         0x03c
#define DW_MMC_RINTSTS       0x044
#define DW_MMC_STATUS        0x048
#define DW_MMC_FIFOTH        0x04c
#define DW_MMC_GPIO          0x058
#define DW_MMC_DEBNCE        0x064
#define DW_MMC_USRID         0x068
#define DW_MMC_VERID         0x06c
#define DW_MMC_HCON          0x070
#define DW_MMC_UHSREG        0x074
#define DW_MMC_BMOD          0x080
#define DW_MMC_DBADDR        0x088
#define DW_MMC_IDSTS         0x08c
#define DW_MMC_IDINTEN       0x090
#define DW_MMC_DSCADDR       0x094
#define DW_MMC_BUFADDR       0x098
#define DW_MMC_CARDTHRCTL    0x100
#define DW_MMC_UHSREG_EXT    0x108
#define DW_MMC_ENABLE_SHIFT  0x110
#define DW_MMC_FIFO_START    0x200

#define GET_IDSTS_DMAC_FSM(x)  (((x) >> 13) & 0xf)
#define IDSTS_FSM_DMA_IDLE         0
#define IDSTS_FSM_DMA_SUSPEND      1
#define IDSTS_FSM_DESC_RD          2
#define IDSTS_FSM_DESC_CHK         3
#define IDSTS_FSM_DMA_RD_REQ_WAIT  4
#define IDSTS_FSM_DMA_WR_REQ_WAIT  5
#define IDSTS_FSM_DMA_RD           6
#define IDSTS_FSM_DMA_WR           7
#define IDSTS_FSM_DESC_CLOSE       8
#define IDSTS_FSM_MASK             0xf

#define CMD_UPDATE_CLK  0x80202000
#define CMD_START_BIT   (1 << 31)

#define MMC_8BIT_MODE  (1 << 16)
#define MMC_4BIT_MODE  (1 << 0)
#define MMC_1BIT_MODE  0

#define DW_MMC_BLOCK_SIZE  512

#define CMD_INDEX_MASK                 0x3F
#define BIT_CMD_RESPONSE_EXPECT        (1 << 6)
#define BIT_CMD_LONG_RESPONSE          (1 << 7)
#define BIT_CMD_CHECK_RESPONSE_CRC     (1 << 8)
#define BIT_CMD_DATA_EXPECTED          (1 << 9)
#define BIT_CMD_READ                   (0 << 10)
#define BIT_CMD_WRITE                  (1 << 10)
#define BIT_CMD_BLOCK_TRANSFER         (0 << 11)
#define BIT_CMD_STREAM_TRANSFER        (1 << 11)
#define BIT_CMD_SEND_AUTO_STOP         (1 << 12)
#define BIT_CMD_WAIT_PRVDATA_COMPLETE  (1 << 13)
#define BIT_CMD_STOP_ABORT_CMD         (1 << 14)
#define BIT_CMD_SEND_INIT              (1 << 15)
#define BIT_CMD_UPDATE_CLOCK_ONLY      (1 << 21)
#define BIT_CMD_READ_CEATA_DEVICE      (1 << 22)
#define BIT_CMD_CCS_EXPECTED           (1 << 23)
#define BIT_CMD_ENABLE_BOOT            (1 << 24)
#define BIT_CMD_EXPECT_BOOT_ACK        (1 << 25)
#define BIT_CMD_DISABLE_BOOT           (1 << 26)
#define BIT_CMD_MANDATORY_BOOT         (0 << 27)
#define BIT_CMD_ALTERNATE_BOOT         (1 << 27)
#define BIT_CMD_VOLT_SWITCH            (1 << 28)
#define BIT_CMD_USE_HOLD_REG           (1 << 29)
#define BIT_CMD_START                  (1 << 31)

#define CMD_INDEX(x)  ((x) & CMD_INDEX_MASK)

#define DW_MMC_INT_EBE       (1 << 15)                          /* End-bit Err */
#define DW_MMC_INT_ACD       (1 << 14)                          /* Auto command done */
#define DW_MMC_INT_SBE       (1 << 13)                          /* Start-bit  Err */
#define DW_MMC_INT_HLE       (1 << 12)                          /* Hardware-lock Err */
#define DW_MMC_INT_FRUN      (1 << 11)                          /* FIFO UN/OV RUN */
#define DW_MMC_INT_HTO       (1 << 10)                          /* Data starvation by host timeout */
#define DW_MMC_INT_DRT       (1 << 9)                           /* Data timeout */
#define DW_MMC_INT_RTO       (1 << 8)                           /* Response timeout */
#define DW_MMC_INT_DCRC      (1 << 7)                           /* Data CRC err */
#define DW_MMC_INT_RCRC      (1 << 6)                           /* Response CRC err */
#define DW_MMC_INT_RXDR      (1 << 5)                           /* Receive FIFO data request */
#define DW_MMC_INT_TXDR      (1 << 4)                           /* Transmit FIFO data request */
#define DW_MMC_INT_DTO       (1 << 3)                           /* Data trans over */
#define DW_MMC_INT_CMD_DONE  (1 << 2)                           /* Command done */
#define DW_MMC_INT_RE        (1 << 1)                           /* Response error */

#define DW_MMC_INT_DATA_ERR  (DW_MMC_INT_EBE  |                   \
                                                 DW_MMC_INT_SBE  |\
                                                 DW_MMC_INT_HLE  |\
                                                 DW_MMC_INT_FRUN |\
                                                 DW_MMC_INT_DCRC |\
                                                 DW_MMC_INT_HTO  |\
                                                 DW_MMC_INT_DRT)

#define DW_MMC_IDMAC_DES0_DIC  (1 << 1)
#define DW_MMC_IDMAC_DES0_LD   (1 << 2)
#define DW_MMC_IDMAC_DES0_FS   (1 << 3)
#define DW_MMC_IDMAC_DES0_CH   (1 << 4)
#define DW_MMC_IDMAC_DES0_ER   (1 << 5)
#define DW_MMC_IDMAC_DES0_CES  (1 << 30)
#define DW_MMC_IDMAC_DES0_OWN  (1 << 31)
#define DW_MMC_IDMAC_DES1_BS1(x)  ((x) & 0x1fff)
#define DW_MMC_IDMAC_DES2_BS2(x)  (((x) & 0x1fff) << 13)
#define DW_MMC_IDMAC_SWRESET  (1 << 0)
#define DW_MMC_IDMAC_FB       (1 << 1)
#define DW_MMC_IDMAC_ENABLE   (1 << 7)

#define DW_MMC_CTRL_RESET       (1 << 0)
#define DW_MMC_CTRL_FIFO_RESET  (1 << 1)
#define DW_MMC_CTRL_DMA_RESET   (1 << 2)
#define DW_MMC_CTRL_INT_EN      (1 << 4)
#define DW_MMC_CTRL_DMA_EN      (1 << 5)
#define DW_MMC_CTRL_IDMAC_EN    (1 << 25)
#define DW_MMC_CTRL_RESET_ALL   (DW_MMC_CTRL_RESET | DW_MMC_CTRL_FIFO_RESET | DW_MMC_CTRL_DMA_RESET)

#define DW_MMC_STS_DATA_BUSY  (1 << 9)
#define DW_MMC_STS_FIFO_COUNT(x)  (((x) & 0x1fff) << 17)                 /* Number of filled locations in FIFO */
#define GET_STS_FIFO_COUNT(x)     (((x) >> 17) & 0x1fff)
#define DW_MMC_STS_FIFO_FULL(x)   (((x) >> 3) & 1)

#define DW_MMC_BMOD_SWR  (1 << 0)                                /* Software Reset */
#define DW_MMC_BMOD_FB   (1 << 1)                                /* Fix Burst */
#define DW_MMC_BMOD_DE   (1 << 7)                                /* IDMAC Enable */

#define DW_MMC_IDSTS_TI  (1 << 0)                                /* Transmit Interrupt */
#define DW_MMC_IDSTS_RI  (1 << 1)                                /* Receive Interrupt */

#define DW_MMC_FIFO_TWMARK(x)     ((x) & 0xfff)
#define DW_MMC_FIFO_RWMARK(x)     (((x) & 0x1ff) << 16)
#define DW_MMC_DMA_BURST_SIZE(x)  (((x) & 0x7) << 28)

#define DW_MMC_CARD_RD_THR(x)  (((x) & 0xfff) << 16)
#define DW_MMC_CARD_RD_THR_EN  (1 << 0)

#define UHS_DDR_MODE  (1 << 16)

#define GENCLK_DIV  7

#define DW_MMC_GPIO_CLK_DIV(x)         (((x) & 0xf) << 8)
#define DW_MMC_GPIO_USE_SAMPLE_DLY(x)  (((x) & 1) << 13)
#define DW_MMC_GPIO_CLK_ENABLE  BIT16

#define UHSEXT_SAMPLE_PHASE(x)     (((x) & 0x1f) << 16)
#define UHSEXT_SAMPLE_DRVPHASE(x)  (((x) & 0x1f) << 21)
#define UHSEXT_SAMPLE_DLY(x)       (((x) & 0x1f) << 26)

#define DWMMC_DMA_BUF_SIZE    (512 * 8)
#define DWMMC_FIFO_THRESHOLD  16

#define DWMMC_INIT_CLOCK_FREQ  400                               /* KHz */

//
// The transfer modes supported by SD Host Controller
// Simplified Spec 3.0 Table 1-2
//
typedef enum {
  SdMmcNoData,
  SdMmcPioMode,
  SdMmcSdmaMode,
  SdMmcAdmaMode
} DW_MMC_HC_TRANSFER_MODE;

//
// The maximum data length of each descriptor line
//
#define ADMA_MAX_DATA_PER_LINE  0x10000

typedef struct {
  UINT32    Des0;
  UINT32    Des1;
  UINT32    Des2;
  UINT32    Des3;
} DW_MMC_HC_DMA_DESC_LINE;

#define SD_MMC_SDMA_BOUNDARY  512 * 1024
#define SD_MMC_SDMA_ROUND_UP(x, n)  (((x) + n) & ~(n - 1))

typedef struct {
  UINT8    FirstBar  : 3;     // bit 0:2
  UINT8    Reserved  : 1;     // bit 3
  UINT8    SlotNum   : 3;     // bit 4:6
  UINT8    Reserved1 : 1;     // bit 7
} DW_MMC_HC_SLOT_INFO;

/**
  Dump the content of SD/MMC host controller's Capability Register.

  @param[in]  Slot            The slot number of the SD card to send the command to.
  @param[in]  Capability      The buffer to store the capability data.

**/
VOID
DumpCapabilityReg (
  IN UINT8               Slot,
  IN DW_MMC_HC_SLOT_CAP  *Capability
  );

#if 0

/**
  Read SlotInfo register from SD/MMC host controller pci config space.

  @param[in]  PciIo        The PCI IO protocol instance.
  @param[out] FirstBar     The buffer to store the first BAR value.
  @param[out] SlotNum      The buffer to store the supported slot number.

  @retval EFI_SUCCESS      The operation succeeds.
  @retval Others           The operation fails.

**/
EFI_STATUS
EFIAPI
DwMmcHcGetSlotInfo (
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  OUT UINT8                   *FirstBar,
  OUT UINT8                   *SlotNum
  );

#endif

#ifdef DWMMC_PCI

/**
  Read/Write specified SD/MMC host controller mmio register.

  @param[in]      PciIo        The PCI IO protocol instance.
  @param[in]      BarIndex     The BAR index of the standard PCI Configuration
                               header to use as the base address for the memory
                               operation to perform.
  @param[in]      Offset       The offset within the selected BAR to start the
                               memory operation.
  @param[in]      Read         A boolean to indicate it's read or write operation.
  @param[in]      Count        The width of the mmio register in bytes.
                               Must be 1, 2 , 4 or 8 bytes.
  @param[in, out] Data         For read operations, the destination buffer to store
                               the results. For write operations, the source buffer
                               to write data from. The caller is responsible for
                               having ownership of the data buffer and ensuring its
                               size not less than Count bytes.

  @retval EFI_INVALID_PARAMETER The PciIo or Data is NULL or the Count is not valid.
  @retval EFI_SUCCESS           The read/write operation succeeds.
  @retval Others                The read/write operation fails.

**/
EFI_STATUS
EFIAPI
DwMmcHcRwMmio (
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  IN     UINT8                BarIndex,
  IN     UINT32               Offset,
  IN     BOOLEAN              Read,
  IN     UINT8                Count,
  IN OUT VOID                 *Data
  );

#else

/**
  Read/Write specified SD/MMC host controller mmio register.

  @param[in]      DevIo        The DEVICE IO protocol instance.
  @param[in]      Offset       The offset within the selected BAR to start the
                               memory operation.
  @param[in]      Read         A boolean to indicate it's read or write operation.
  @param[in]      Count        The width of the mmio register in bytes.
                               Must be 1, 2 , 4 or 8 bytes.
  @param[in, out] Data         For read operations, the destination buffer to store
                               the results. For write operations, the source buffer
                               to write data from. The caller is responsible for
                               having ownership of the data buffer and ensuring its
                               size not less than Count bytes.

  @retval EFI_INVALID_PARAMETER The PciIo or Data is NULL or the Count is not valid.
  @retval EFI_SUCCESS           The read/write operation succeeds.
  @retval Others                The read/write operation fails.

**/
EFI_STATUS
EFIAPI
DwMmcHcRwMmio (
  IN     UINTN    DevBase,
  IN     UINT32   Offset,
  IN     BOOLEAN  Read,
  IN     UINT8    Count,
  IN OUT VOID     *Data
  );

#endif

#ifdef DWMMC_PCI

/**
  Do OR operation with the value of the specified SD/MMC host controller mmio register.

  @param[in] PciIo             The PCI IO protocol instance.
  @param[in] BarIndex          The BAR index of the standard PCI Configuration
                               header to use as the base address for the memory
                               operation to perform.
  @param[in] Offset            The offset within the selected BAR to start the
                               memory operation.
  @param[in] Count             The width of the mmio register in bytes.
                               Must be 1, 2 , 4 or 8 bytes.
  @param[in] OrData            The pointer to the data used to do OR operation.
                               The caller is responsible for having ownership of
                               the data buffer and ensuring its size not less than
                               Count bytes.

  @retval EFI_INVALID_PARAMETER The PciIo or OrData is NULL or the Count is not valid.
  @retval EFI_SUCCESS           The OR operation succeeds.
  @retval Others                The OR operation fails.

**/
EFI_STATUS
EFIAPI
DwMmcHcOrMmio (
  IN  EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINT8                BarIndex,
  IN  UINT32               Offset,
  IN  UINT8                Count,
  IN  VOID                 *OrData
  );

#else

/**
  Do OR operation with the value of the specified SD/MMC host controller mmio register.

  @param[in] DevIo             The DEVICE IO protocol instance.
  @param[in] BarIndex          The BAR index of the standard PCI Configuration
                               header to use as the base address for the memory
                               operation to perform.
  @param[in] Offset            The offset within the selected BAR to start the
                               memory operation.
  @param[in] Count             The width of the mmio register in bytes.
                               Must be 1, 2 , 4 or 8 bytes.
  @param[in] OrData            The pointer to the data used to do OR operation.
                               The caller is responsible for having ownership of
                               the data buffer and ensuring its size not less than
                               Count bytes.

  @retval EFI_INVALID_PARAMETER The PciIo or OrData is NULL or the Count is not valid.
  @retval EFI_SUCCESS           The OR operation succeeds.
  @retval Others                The OR operation fails.

**/
EFI_STATUS
EFIAPI
DwMmcHcOrMmio (
  IN     UINTN  DevBase,
  IN  UINT32    Offset,
  IN  UINT8     Count,
  IN  VOID      *OrData
  );

#endif

#ifdef DWMMC_PCI

/**
  Do AND operation with the value of the specified SD/MMC host controller mmio register.

  @param[in] PciIo             The PCI IO protocol instance.
  @param[in] BarIndex          The BAR index of the standard PCI Configuration
                               header to use as the base address for the memory
                               operation to perform.
  @param[in] Offset            The offset within the selected BAR to start the
                               memory operation.
  @param[in] Count             The width of the mmio register in bytes.
                               Must be 1, 2 , 4 or 8 bytes.
  @param[in] AndData           The pointer to the data used to do AND operation.
                               The caller is responsible for having ownership of
                               the data buffer and ensuring its size not less than
                               Count bytes.

  @retval EFI_INVALID_PARAMETER The PciIo or AndData is NULL or the Count is not valid.
  @retval EFI_SUCCESS           The AND operation succeeds.
  @retval Others                The AND operation fails.

**/
EFI_STATUS
EFIAPI
DwMmcHcAndMmio (
  IN  EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINT8                BarIndex,
  IN  UINT32               Offset,
  IN  UINT8                Count,
  IN  VOID                 *AndData
  );

#else

/**
  Do AND operation with the value of the specified SD/MMC host controller mmio register.

  @param[in] DevIo             The DEVICE IO protocol instance.
  @param[in] Offset            The offset within the selected BAR to start the
                               memory operation.
  @param[in] Count             The width of the mmio register in bytes.
                               Must be 1, 2 , 4 or 8 bytes.
  @param[in] AndData           The pointer to the data used to do AND operation.
                               The caller is responsible for having ownership of
                               the data buffer and ensuring its size not less than
                               Count bytes.

  @retval EFI_INVALID_PARAMETER The PciIo or AndData is NULL or the Count is not valid.
  @retval EFI_SUCCESS           The AND operation succeeds.
  @retval Others                The AND operation fails.

**/
EFI_STATUS
EFIAPI
DwMmcHcAndMmio (
  IN  UINTN   DevBase,
  IN  UINT32  Offset,
  IN  UINT8   Count,
  IN  VOID    *AndData
  );

#endif

#ifdef DWMMC_PCI

/**
  Wait for the value of the specified MMIO register set to the test value.

  @param[in]  PciIo         The PCI IO protocol instance.
  @param[in]  BarIndex      The BAR index of the standard PCI Configuration
                            header to use as the base address for the memory
                            operation to perform.
  @param[in]  Offset        The offset within the selected BAR to start the
                            memory operation.
  @param[in]  Count         The width of the mmio register in bytes.
                            Must be 1, 2, 4 or 8 bytes.
  @param[in]  MaskValue     The mask value of memory.
  @param[in]  TestValue     The test value of memory.
  @param[in]  Timeout       The time out value for wait memory set, uses 1
                            microsecond as a unit.

  @retval EFI_TIMEOUT       The MMIO register hasn't expected value in timeout
                            range.
  @retval EFI_SUCCESS       The MMIO register has expected value.
  @retval Others            The MMIO operation fails.

**/
EFI_STATUS
EFIAPI
DwMmcHcWaitMmioSet (
  IN  EFI_PCI_IO_PROTOCOL  *PciIo,
  IN  UINT8                BarIndex,
  IN  UINT32               Offset,
  IN  UINT8                Count,
  IN  UINT64               MaskValue,
  IN  UINT64               TestValue,
  IN  UINT64               Timeout
  );

#else

/**
  Wait for the value of the specified MMIO register set to the test value.

  @param[in]  DevIo         The DEVICE IO protocol instance.
  @param[in]  Offset        The offset within the selected BAR to start the
                            memory operation.
  @param[in]  Count         The width of the mmio register in bytes.
                            Must be 1, 2, 4 or 8 bytes.
  @param[in]  MaskValue     The mask value of memory.
  @param[in]  TestValue     The test value of memory.
  @param[in]  Timeout       The time out value for wait memory set, uses 1
                            microsecond as a unit.

  @retval EFI_TIMEOUT       The MMIO register hasn't expected value in timeout
                            range.
  @retval EFI_SUCCESS       The MMIO register has expected value.
  @retval Others            The MMIO operation fails.

**/
EFI_STATUS
EFIAPI
DwMmcHcWaitMmioSet (
  IN  UINTN   DevBase,
  IN  UINT32  Offset,
  IN  UINT8   Count,
  IN  UINT64  MaskValue,
  IN  UINT64  TestValue,
  IN  UINT64  Timeout
  );

#endif

#ifdef DWMMC_PCI

/**
  Software reset the specified SD/MMC host controller.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.

  @retval EFI_SUCCESS       The software reset executes successfully.
  @retval Others            The software reset fails.

**/
EFI_STATUS
DwMmcHcReset (
  fark
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN UINT8                Slot,
  IN DW_MMC_HC_SLOT_CAP   Capability
  );

#else

/**
  Software reset the specified SD/MMC host controller.

  @param[in] DevIo          The DEVICE IO protocol instance.

  @retval EFI_SUCCESS       The software reset executes successfully.
  @retval Others            The software reset fails.

**/
EFI_STATUS
DwMmcHcReset (
  IN UINTN               DevBase,
  IN DW_MMC_HC_SLOT_CAP  Capability
  );

#endif

#ifdef DWMMC_PCI

/**
  Set all interrupt status bits in Normal and Error Interrupt Status Enable
  register.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.

  @retval EFI_SUCCESS       The operation executes successfully.
  @retval Others            The operation fails.

**/
EFI_STATUS
DwMmcHcEnableInterrupt (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN UINT8                Slot
  );

#else

/**
  Set all interrupt status bits in Normal and Error Interrupt Status Enable
  register.

  @param[in] DevIo          The DEVICE IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.

  @retval EFI_SUCCESS       The operation executes successfully.
  @retval Others            The operation fails.

**/
EFI_STATUS
DwMmcHcEnableInterrupt (
  IN  UINTN  DevBase
  );

#endif

#ifdef DWMMC_PCI

/**
  Get the capability data from the specified slot.

  @param[in]  PciIo           The PCI IO protocol instance.
  @param[in]  Slot            The slot number of the SD card to send the command to.
  @param[out] Capability      The buffer to store the capability data.

  @retval EFI_SUCCESS         The operation executes successfully.
  @retval Others              The operation fails.

**/
EFI_STATUS
DwMmcHcGetCapability (
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  IN     EFI_HANDLE           Controller,
  IN     UINT8                Slot,
  OUT DW_MMC_HC_SLOT_CAP      *Capability
  );

#else

/**
  Get the capability data from the specified slot.

  @param[in]  DevIo           The DEVICE IO protocol instance.
  @param[in]  Slot            The slot number of the SD card to send the command to.
  @param[out] Capability      The buffer to store the capability data.

  @retval EFI_SUCCESS         The operation executes successfully.
  @retval Others              The operation fails.

**/
EFI_STATUS
DwMmcHcGetCapability (
  IN  UINTN               DevBase,
  IN     EFI_HANDLE       Controller,
  IN     UINT8            Slot,
  OUT DW_MMC_HC_SLOT_CAP  *Capability
  );

#endif

#if 0

/**
  Get the maximum current capability data from the specified slot.

  @param[in]  PciIo           The PCI IO protocol instance.
  @param[in]  Slot            The slot number of the SD card to send the command to.
  @param[out] MaxCurrent      The buffer to store the maximum current capability data.

  @retval EFI_SUCCESS         The operation executes successfully.
  @retval Others              The operation fails.

**/
EFI_STATUS
DwMmcHcGetMaxCurrent (
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  IN     UINT8                Slot,
  OUT UINT64                  *MaxCurrent
  );

#endif

#ifdef DWMMC_PCI

/**
  Detect whether there is a SD/MMC card attached at the specified SD/MMC host controller
  slot.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.1 for details.

  @param[in]  PciIo         The PCI IO protocol instance.
  @param[in]  Slot          The slot number of the SD card to send the command to.
  @param[out] MediaPresent  The pointer to the media present boolean value.

  @retval EFI_SUCCESS       There is no media change happened.
  @retval EFI_MEDIA_CHANGED There is media change happened.
  @retval Others            The detection fails.

**/
EFI_STATUS
DwMmcHcCardDetect (
  IN     EFI_PCI_IO_PROTOCOL  *PciIo,
  IN     EFI_HANDLE           Controller,
  IN     UINT8                Slot,
  OUT BOOLEAN                 *MediaPresent
  );

#else

/**
  Detect whether there is a SD/MMC card attached at the specified SD/MMC host controller
  slot.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.1 for details.

  @param[in]  DevIo         The DEVICE IO protocol instance.
  @param[in]  Slot          The slot number of the SD card to send the command to.
  @param[out] MediaPresent  The pointer to the media present boolean value.

  @retval EFI_SUCCESS       There is no media change happened.
  @retval EFI_MEDIA_CHANGED There is media change happened.
  @retval Others            The detection fails.

**/
EFI_STATUS
DwMmcHcCardDetect (
  IN     UINTN       DevBase,
  IN     EFI_HANDLE  Controller,
  IN     UINT8       Slot,
  OUT BOOLEAN        *MediaPresent
  );

#endif

#ifdef DWMMC_PCI

/**
  Stop SD/MMC card clock.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.2.2 for details.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.

  @retval EFI_SUCCESS       Succeed to stop SD/MMC clock.
  @retval Others            Fail to stop SD/MMC clock.

**/
EFI_STATUS
DwMmcHcStopClock (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN UINT8                Slot
  );

/**
  SD/MMC card clock supply.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.2.1 for details.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.
  @param[in] ClockFreq      The max clock frequency to be set. The unit is KHz.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The clock is supplied successfully.
  @retval Others            The clock isn't supplied successfully.

**/
EFI_STATUS
DwMmcHcClockSupply (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN UINT8                Slot,
  IN UINT64               ClockFreq,
  IN DW_MMC_HC_SLOT_CAP   Capability
  );

#else

/**
  Stop SD/MMC card clock.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.2.2 for details.

  @param[in] DevIo          The DEVICE IO protocol instance.

  @retval EFI_SUCCESS       Succeed to stop SD/MMC clock.
  @retval Others            Fail to stop SD/MMC clock.

**/
EFI_STATUS
DwMmcHcStopClock (
  IN  UINTN  DevBase
  );

/**
  SD/MMC card clock supply.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.2.1 for details.

  @param[in] DevIo          The DEVICE IO protocol instance.
  @param[in] ClockFreq      The max clock frequency to be set. The unit is KHz.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The clock is supplied successfully.
  @retval Others            The clock isn't supplied successfully.

**/
EFI_STATUS
DwMmcHcClockSupply (
  IN  UINTN              DevBase,
  IN UINT64              ClockFreq,
  IN DW_MMC_HC_SLOT_CAP  Capability
  );

#endif

#if 0

/**
  SD/MMC bus power control.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.3 for details.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.
  @param[in] PowerCtrl      The value setting to the power control register.

  @retval TRUE              There is a SD/MMC card attached.
  @retval FALSE             There is no a SD/MMC card attached.

**/
EFI_STATUS
DwMmcHcPowerControl (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN UINT8                Slot,
  IN UINT8                PowerCtrl
  );

#endif

#ifdef DWMMC_PCI

/**
  Set the SD/MMC bus width.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.4 for details.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.
  @param[in] BusWidth       The bus width used by the SD/MMC device, it must be 1, 4 or 8.

  @retval EFI_SUCCESS       The bus width is set successfully.
  @retval Others            The bus width isn't set successfully.

**/
EFI_STATUS
DwMmcHcSetBusWidth (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN UINT8                Slot,
  IN BOOLEAN              IsDdr,
  IN UINT16               BusWidth
  );

#else

/**
  Set the SD/MMC bus width.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.4 for details.

  @param[in] DevIo          The DEVICE IO protocol instance.
  @param[in] BusWidth       The bus width used by the SD/MMC device, it must be 1, 4 or 8.

  @retval EFI_SUCCESS       The bus width is set successfully.
  @retval Others            The bus width isn't set successfully.

**/
EFI_STATUS
DwMmcHcSetBusWidth (
  IN  UINTN   DevBase,
  IN BOOLEAN  IsDdr,
  IN UINT16   BusWidth
  );

#endif

#ifdef DWMMC_PCI

/**
  Supply SD/MMC card with lowest clock frequency at initialization.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The clock is supplied successfully.
  @retval Others            The clock isn't supplied successfully.

**/
EFI_STATUS
DwMmcHcInitClockFreq (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN UINT8                Slot,
  IN DW_MMC_HC_SLOT_CAP   Capability
  );

#else

/**
  Supply SD/MMC card with lowest clock frequency at initialization.

  @param[in] DevIo          The DEVICE IO protocol instance.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The clock is supplied successfully.
  @retval Others            The clock isn't supplied successfully.

**/
EFI_STATUS
DwMmcHcInitClockFreq (
  IN UINTN               DevBase,
  IN DW_MMC_HC_SLOT_CAP  Capability
  );

#endif

#ifdef DWMMC_PCI

/**
  Supply SD/MMC card with maximum voltage at initialization.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.3 for details.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The voltage is supplied successfully.
  @retval Others            The voltage isn't supplied successfully.

**/
EFI_STATUS
DwMmcHcInitPowerVoltage (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN UINT8                Slot,
  IN DW_MMC_HC_SLOT_CAP   Capability
  );

#else

/**
  Supply SD/MMC card with maximum voltage at initialization.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.3 for details.

  @param[in] DevIo          The DEVICE IO protocol instance.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The voltage is supplied successfully.
  @retval Others            The voltage isn't supplied successfully.

**/
EFI_STATUS
DwMmcHcInitPowerVoltage (
  IN UINTN               DevBase,
  IN DW_MMC_HC_SLOT_CAP  Capability
  );

#endif

#ifdef DWMMC_PCI

/**
  Initialize the Timeout Control register with most conservative value at initialization.

  Refer to SD Host Controller Simplified spec 3.0 Section 2.2.15 for details.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.

  @retval EFI_SUCCESS       The timeout control register is configured successfully.
  @retval Others            The timeout control register isn't configured successfully.

**/
EFI_STATUS
DwMmcHcInitTimeoutCtrl (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN UINT8                Slot
  );

#else

/**
  Initialize the Timeout Control register with most conservative value at initialization.

  Refer to SD Host Controller Simplified spec 3.0 Section 2.2.15 for details.

  @param[in] DevIo          The DEVICE IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.

  @retval EFI_SUCCESS       The timeout control register is configured successfully.
  @retval Others            The timeout control register isn't configured successfully.

**/
EFI_STATUS
DwMmcHcInitTimeoutCtrl (
  IN  UINTN  DevBase
  );

#endif

#ifdef DWMMC_PCI

/**
  Initial SD/MMC host controller with lowest clock frequency, max power and max timeout value
  at initialization.

  @param[in] PciIo          The PCI IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command to.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The host controller is initialized successfully.
  @retval Others            The host controller isn't initialized successfully.

**/
EFI_STATUS
DwMmcHcInitHost (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN UINT8                Slot,
  IN DW_MMC_HC_SLOT_CAP   Capability
  );

#else

/**
  Initial SD/MMC host controller with lowest clock frequency, max power and
  max timeout value at initialization.

  @param[in] DevIo          The DEVICE IO protocol instance.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The host controller is initialized successfully.
  @retval Others            The host controller isn't initialized successfully.

**/
EFI_STATUS
DwMmcHcInitHost (
  IN  UINTN              DevBase,
  IN DW_MMC_HC_SLOT_CAP  Capability
  );

#endif

#endif /* _DW_MMC_HCI_H_ */
