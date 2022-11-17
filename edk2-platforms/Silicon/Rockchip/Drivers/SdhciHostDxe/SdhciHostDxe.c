/** @file
 *
 *  Copyright (c) 2017, Andrei Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include "SdhciHostDxe.h"
#include <Library/PcdLib.h>
#include <Library/RockchipPlatformLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/TimerLib.h>
#include <string.h>

#define CONFIG_MMC_SDHCI_SDMA
#define word32   *(volatile unsigned int *)(long)

//#define SDHCI_MAX_RETRY_COUNT (1000 * 20)

// MMC/SD/SDIO1 register definitions.
#define MMCHS1_OFFSET     0x00300000
#define MMCHS2_OFFSET     0x00340000
#define MMCHS1_BASE       (BCM2836_SOC_REGISTERS + MMCHS1_OFFSET)
#define MMCHS2_BASE       (BCM2836_SOC_REGISTERS + MMCHS2_OFFSET)
#define MMCHS1_LENGTH     0x00000100
#define MMCHS2_LENGTH     0x00000100
#define MMCHS_DMA_ADDRESS  (mMmcHsBase + 0x0)

#define MMCHS_BLK_SIZE     (mMmcHsBase + 0x4)
#define MMCHS_BLK_COUNT    (mMmcHsBase + 0x6)

#define SDHCI_MAKE_BLKSZ(dma, blksz) (((dma & 0x7) << 12) | (blksz & 0xFFF))
/*
 * Host SDMA buffer boundary. Valid values from 4K to 512K in powers of 2.
 */
#define SDHCI_DEFAULT_BOUNDARY_SIZE	(512 * 1024)
#define SDHCI_DEFAULT_BOUNDARY_ARG	(7)

#define MMCHS_TRANS_MODE   (mMmcHsBase + 0xC)
#define  SDHCI_TRNS_DMA         BIT0
#define  SDHCI_TRNS_BLK_CNT_EN	BIT1
#define  SDHCI_TRNS_ACMD12	    BIT2
#define  SDHCI_TRNS_READ	    BIT4
#define  SDHCI_TRNS_MULTI	    BIT5

#define BLEN_512BYTES     (0x200UL << 0)

#define MMCHS_ARG         (mMmcHsBase + 0x8)

#define MMCHS_CMD         (mMmcHsBase + 0xC)
#define MMCHS_CMD16       (mMmcHsBase + 0xE)

#define BCE_ENABLE        BIT1
#define DDIR_READ         BIT4
#define DDIR_WRITE        (0x0UL << 4)
#define MSBS_SGLEBLK      (0x0UL << 5)
#define MSBS_MULTBLK      BIT5
#define RSP_TYPE_MASK     (0x3UL << 16)
#define RSP_TYPE_136BITS  BIT16
#define RSP_TYPE_48BITS   (0x2UL << 16)
#define RSP_TYPE_48BUSY   (0x3UL << 16)
#define CCCE_ENABLE       BIT19
#define CICE_ENABLE       BIT20
#define DP_ENABLE         BIT21

#define CMD_TYPE_NORMAL      0
#define CMD_TYPE_ABORT       3
#define TYPE(CMD_TYPE)       (((CMD_TYPE) & 0x3) << 22)
#define _INDX(CMD_INDX)      ((CMD_INDX & 0x3F) << 24)
#define MMC_CMD_NUM(CMD)     (((CMD) >> 24) & 0x3F)
#define INDX(CMD_INDX)       (TYPE(CMD_TYPE_NORMAL) | _INDX(CMD_INDX))
#define INDX_ABORT(CMD_INDX) (TYPE(CMD_TYPE_ABORT) | _INDX(CMD_INDX))

#define MMCHS_RSP10       (mMmcHsBase + 0x10)
#define MMCHS_RSP32       (mMmcHsBase + 0x14)
#define MMCHS_RSP54       (mMmcHsBase + 0x18)
#define MMCHS_RSP76       (mMmcHsBase + 0x1C)
#define MMCHS_DATA        (mMmcHsBase + 0x20)

#define MMCHS_PRES_STATE  (mMmcHsBase + 0x24)
#define CMDI_MASK         BIT0
#define CMDI_ALLOWED      (0x0UL << 0)
#define CMDI_NOT_ALLOWED  BIT0
#define DATI_MASK         BIT1
#define DATI_ALLOWED      (0x0UL << 1)
#define DATI_NOT_ALLOWED  BIT1
#define WRITE_PROTECT_OFF BIT19

#define SDHCI_CMD_INHIBIT           BIT0
#define SDHCI_DATA_INHIBIT          BIT1
#define SDHCI_DOING_WRITE           BIT8
#define SDHCI_DOING_READ            BIT9)
#define SDHCI_SPACE_AVAILABLE       BIT10
#define SDHCI_DATA_AVAILABLE        BIT11
#define SDHCI_CARD_PRESENT          BIT16
#define SDHCI_CARD_STATE_STABLE     BIT17
#define SDHCI_CARD_DETECT_PIN_LEVEL  BIT18
#define SDHCI_WRITE_PROTECT         BIT19
#define SDHCI_DATA_0_LVL            BIT20

#define MMCHS_HCTL        (mMmcHsBase + 0x28)
#define DTW_1_BIT         (0x0UL << 1)
#define DTW_4_BIT         BIT1
#define HIGH_SPEED_EN     BIT2
#define DTW_8_BIT         BIT5

#define SDBP_MASK         BIT8
#define SDBP_OFF          (0x0UL << 8)
#define SDBP_ON           BIT8
#define SDVS_MASK         (0x7UL << 9)
#define SDVS_1_8_V        (0x5UL << 9)
#define SDVS_3_0_V        (0x6UL << 9)
#define SDVS_3_3_V        (0x7UL << 9)
#define IWE               BIT24

#define SDHCI_CTRL_LED          BIT0
#define SDHCI_CTRL_4BITBUS      BIT1
#define SDHCI_CTRL_HISPD        BIT2
#define SDHCI_CTRL_DMA_MASK     0x18
#define SDHCI_CTRL_SDMA         0x00

#define MMCHS_SYSCTL      (mMmcHsBase + 0x2C)
#define ICE               BIT0
#define ICS_MASK          BIT1
#define ICS               BIT1
#define CEN               BIT2
#define CLKD_MASK         (0x3FFUL << 6)
#define CLKD_80KHZ        (0x258UL) //(96*1000/80)/2
#define CLKD_400KHZ       (0xF0UL)
#define CLKD_12500KHZ     (0x200UL)
#define DTO_MASK          (0xFUL << 16)
#define DTO_VAL           (0xEUL << 16)
#define SRA               BIT24
#define SRC_MASK          BIT25
#define SRC               BIT25
#define SRD               BIT26

#define MMCHS_RESET       (mMmcHsBase + 0x2F)
#define RST_ALL           0x01
#define RST_CMD           0x02
#define RST_DATA          0x04

#define MMCHS_INT_STAT    (mMmcHsBase + 0x30)
#define CC                BIT0
#define TC                BIT1
#define BWR               BIT4
#define BRR               BIT5
#define CARD_INS          BIT6
#define ERRI              BIT15
#define CTO               BIT16
#define DTO               BIT20
#define DCRC              BIT21
#define DEB               BIT22

#define SDHCI_INT_RESPONSE      BIT0
#define SDHCI_INT_DATA_END      BIT1
#define SDHCI_INT_DMA_END       BIT3
#define SDHCI_INT_SPACE_AVAIL   BIT4
#define SDHCI_INT_DATA_AVAIL    BIT5
#define SDHCI_INT_CARD_INSERT   BIT6
#define SDHCI_INT_CARD_REMOVE   BIT7
#define SDHCI_INT_CARD_INT      BIT8
#define SDHCI_INT_ERROR	        BIT15
#define SDHCI_INT_TIMEOUT       BIT16
#define SDHCI_INT_CRC           BIT17
#define SDHCI_INT_END_BIT       BIT18
#define SDHCI_INT_INDEX         BIT19
#define SDHCI_INT_DATA_TIMEOUT  BIT20
#define SDHCI_INT_DATA_CRC      BIT21
#define SDHCI_INT_DATA_END_BIT  BIT22
#define SDHCI_INT_BUS_POWER     BIT23
#define SDHCI_INT_ACMD12ERR     BIT24
#define SDHCI_INT_ADMA_ERROR    BIT25

#define MMCHS_IE          (mMmcHsBase + 0x34)
#define CC_EN             BIT0
#define TC_EN             BIT1
#define BWR_EN            BIT4
#define BRR_EN            BIT5
#define CTO_EN            BIT16
#define CCRC_EN           BIT17
#define CEB_EN            BIT18
#define CIE_EN            BIT19
#define DTO_EN            BIT20
#define DCRC_EN           BIT21
#define DEB_EN            BIT22
#define CERR_EN           BIT28
#define BADA_EN           BIT29
#define ALL_EN            0x27f003b //0xFFFFFFFF
#define ALL_MASK          0xFFFFFFFF

#define MMCHS_ISE         (mMmcHsBase + 0x38)
#define CC_SIGEN          BIT0
#define TC_SIGEN          BIT1
#define BWR_SIGEN         BIT4
#define BRR_SIGEN         BIT5
#define CTO_SIGEN         BIT16
#define CCRC_SIGEN        BIT17
#define CEB_SIGEN         BIT18
#define CIE_SIGEN         BIT19
#define DTO_SIGEN         BIT20
#define DCRC_SIGEN        BIT21
#define DEB_SIGEN         BIT22
#define CERR_SIGEN        BIT28
#define BADA_SIGEN        BIT29

#define MMCHS_AC12        (mMmcHsBase + 0x3C)
#define MMCHS_HCTL2       (mMmcHsBase + 0x3E)

#define SDHCI_CTRL_UHS_MASK             0x0007
#define SDHCI_CTRL_UHS_SDR12            0x0000
#define SDHCI_CTRL_UHS_SDR25            0x0001
#define SDHCI_CTRL_UHS_SDR50            0x0002
#define SDHCI_CTRL_UHS_SDR104           0x0003
#define SDHCI_CTRL_UHS_DDR50            0x0004

#define SDHCI_CTRL_EMMC_SDR50           0x0001
#define SDHCI_CTRL_EMMC_HS200           0x0003
#define SDHCI_CTRL_EMMC_HS400           0x0007

#define SDHCI_CTRL_VDD_180              0x0008
#define SDHCI_CTRL_DRV_TYPE_MASK        0x0030
#define SDHCI_CTRL_DRV_TYPE_B           0x0000
#define SDHCI_CTRL_DRV_TYPE_A           0x0010
#define SDHCI_CTRL_DRV_TYPE_C           0x0020
#define SDHCI_CTRL_DRV_TYPE_D           0x0030
#define SDHCI_CTRL_EXEC_TUNING          0x0040
#define SDHCI_CTRL_TUNED_CLK            0x0080
#define SDHCI_CTRL_PRESET_VAL_ENABLE    0x8000

#define MMCHS_CAPA        (mMmcHsBase + 0x40)
#define VS30              BIT25
#define VS18              BIT26

#define MMCHS_CUR_CAPA    (mMmcHsBase + 0x48)
#define MMCHS_REV         (mMmcHsBase + 0xFC)

#define BLOCK_COUNT_SHIFT 16
#define RCA_SHIFT         16

#define CMD_R1            (RSP_TYPE_48BITS | CCCE_ENABLE | CICE_ENABLE)
#define CMD_R1B           (RSP_TYPE_48BUSY | CCCE_ENABLE | CICE_ENABLE)
#define CMD_R2            (RSP_TYPE_136BITS | CCCE_ENABLE)
#define CMD_R3            (RSP_TYPE_48BITS)
#define CMD_R6            (RSP_TYPE_48BITS | CCCE_ENABLE | CICE_ENABLE)
#define CMD_R7            (RSP_TYPE_48BITS | CCCE_ENABLE | CICE_ENABLE)

#define CMD_R1_ADTC       (CMD_R1 | DP_ENABLE)
#define CMD_R1_ADTC_READ  (CMD_R1_ADTC | DDIR_READ)
#define CMD_R1_ADTC_WRITE (CMD_R1_ADTC | DDIR_WRITE)

#define CMD0              (INDX(0)) // Go idle
#define CMD1              (INDX(1) | CMD_R3) // MMC: Send Op Cond
#define CMD2              (INDX(2) | CMD_R2) // Send CID
#define CMD3              (INDX(3) | CMD_R6) // Set Relative Addr
#define CMD4              (INDX(4)) // Set DSR
#define CMD5              (INDX(5) | CMD_R1B) // SDIO: Sleep/Awake
//#define CMD6              (INDX(6) | CMD_R1_ADTC_READ) // Switch
#define CMD6              (INDX(6) | CMD_R1B) // Switch
#define CMD7              (INDX(7) | CMD_R1) // Select/Deselect
#define CMD8_SD           (INDX(8) | CMD_R7) // Send If Cond
#define CMD8_SD_ARG       (0x0UL << 12 | BIT8 | 0xCEUL << 0)
#define CMD8_MMC          (INDX(8) | CMD_R1_ADTC_READ) // Send Ext Csd
#define CMD8_MMC_ARG      (0)
#define CMD9              (INDX(9) | CMD_R2) // Send CSD
#define CMD10             (INDX(10) | CMD_R2) // Send CID
#define CMD11             (INDX(11) | CMD_R1) // Voltage Switch
#define CMD12             (INDX_ABORT(12) | CMD_R1B) // Stop Transmission
#define CMD13             (INDX(13) | CMD_R1) // Send Status
#define CMD15             (INDX(15)) // Go inactive state
#define CMD16             (INDX(16) | CMD_R1) // Set Blocklen
#define CMD17             (INDX(17) | CMD_R1_ADTC_READ) // Read Single Block
#define CMD18             (INDX(18) | CMD_R1_ADTC_READ | MSBS_MULTBLK) // Read Multiple Blocks
#define CMD19             (INDX(19) | CMD_R1_ADTC_READ) // SD: Send Tuning Block (64 bytes)
#define CMD20             (INDX(20) | CMD_R1B) // SD: Speed Class Control
#define CMD23             (INDX(23) | CMD_R1) // Set Block Count for CMD18 and CMD25
#define CMD24             (INDX(24) | CMD_R1_ADTC_WRITE) // Write Block
#define CMD25             (INDX(25) | CMD_R1_ADTC_WRITE | MSBS_MULTBLK) // Write Multiple Blocks
#define CMD55             (INDX(55) | CMD_R1) // App Cmd

#define ACMD6             (INDX(6) | CMD_R1) // Set Bus Width
#define ACMD22            (INDX(22) | CMD_R1_ADTC_READ) // SEND_NUM_WR_BLOCKS
#define ACMD41            (INDX(41) | CMD_R3) // Send Op Cond
#define ACMD51            (INDX(51) | CMD_R1_ADTC_READ) // Send SCR
#define MMC_ACMD22         (MMC_INDX(22) | MMC_CMD_WAIT_RESPONSE)

// User-friendly command names
#define CMD_IO_SEND_OP_COND      CMD5
#define CMD_SEND_CSD             CMD9  // CSD: Card-Specific Data
#define CMD_STOP_TRANSMISSION    CMD12
#define CMD_SEND_STATUS          CMD13
#define CMD_READ_SINGLE_BLOCK    CMD17
#define CMD_READ_MULTIPLE_BLOCK  CMD18
#define CMD_SET_BLOCK_COUNT      CMD23
#define CMD_WRITE_SINGLE_BLOCK   CMD24
#define CMD_WRITE_MULTIPLE_BLOCK CMD25

#define DEBUG_MMCHOST_SD DEBUG_INFO

#define MMC_DATA_READ           1
#define MMC_DATA_WRITE          2

typedef struct {
  UINT8 *buffer;
  UINT32 flags;
  UINT32 blocks;
  UINT32 blocksize;
} SDHCI_DATA;

//STATIC BOOLEAN mCardIsPresent = FALSE;
//STATIC CARD_DETECT_STATE mCardDetectState = CardDetectRequired;
UINT32 LastExecutedCommand = (UINT32) -1;

STATIC UINTN mMmcHsBase;
STATIC UINT32 mMmcDataCommand;
STATIC UINT32 mMmcDataArgument;
STATIC SDHCI_DATA gSdhciData;

STATIC
UINT32
EFIAPI
SdMmioWrite32 (
  IN      UINTN                     Address,
  IN      UINT32                    Value
  )
{
  UINT32 ret;
  ret = (UINT32)MmioWrite32 (Address, Value);
  // There is a bug about clock domain crossing on writes, delay to avoid it
  //gBS->Stall (STALL_AFTER_REG_WRITE_US);
  return ret;
}

STATIC
UINT32
EFIAPI
SdMmioOr32 (
  IN      UINTN                     Address,
  IN      UINT32                    OrData
  )
{
  return SdMmioWrite32 (Address, MmioRead32 (Address) | OrData);
}

STATIC
UINT32
EFIAPI
SdMmioAnd32 (
  IN      UINTN                     Address,
  IN      UINT32                    AndData
  )
{
  return SdMmioWrite32 (Address, MmioRead32 (Address) & AndData);
}

STATIC
UINT32
EFIAPI
SdMmioAndThenOr32 (
  IN      UINTN                     Address,
  IN      UINT32                    AndData,
  IN      UINT32                    OrData
  )
{
  return SdMmioWrite32 (Address, (MmioRead32 (Address) & AndData) | OrData);
}

void IomemShow(const char *label, unsigned long base, unsigned int start, unsigned int end)
{
  unsigned int val, offset = start, nr = 0;

  if (label)
    DEBUG ((DEBUG_ERROR, "%a:\n", label));

  DEBUG ((DEBUG_ERROR, "%08lx:  ", base + offset));
  for (offset = start; offset <= end; offset += 0x04) {
    if (nr >= 4) {
      DEBUG ((DEBUG_ERROR, "\n%08lx:  ", base + offset));
      nr = 0;
    }
    val = MmioRead32(base + offset);
    DEBUG ((DEBUG_ERROR, "%08lx ", val));
    nr++;
  }
  DEBUG ((DEBUG_ERROR, "\n"));
}

/**
   These SD commands are optional, according to the SD Spec
**/
BOOLEAN
IgnoreCommand (
  UINT32 Command
  )
{
  switch (Command) {
  case MMC_CMD20:
    return TRUE;
  default:
    return FALSE;
  }
}

/**
   Translates a generic SD command into the format used by the Arasan SD Host Controller
**/
UINT32
TranslateCommand (
  UINT32 Command,
  UINT32 Argument
  )
{
  UINT32 Translation = 0xffffffff;

  if (LastExecutedCommand == CMD55) {
    switch (Command) {
    case MMC_CMD6:
      Translation = ACMD6;
      DEBUG ((DEBUG_MMCHOST_SD, "ACMD6\n"));
      break;
    case MMC_ACMD22:
      Translation = ACMD22;
      DEBUG ((DEBUG_MMCHOST_SD, "ACMD22\n"));
      break;
    case MMC_ACMD41:
      Translation = ACMD41;
      DEBUG ((DEBUG_MMCHOST_SD, "ACMD41\n"));
      break;
    case MMC_ACMD51:
      Translation = ACMD51;
      DEBUG ((DEBUG_MMCHOST_SD, "ACMD51\n"));
      break;
    default:
      DEBUG ((DEBUG_ERROR, "MMCHost: TranslateCommand(): Unrecognized App command: %d\n", Command));
    }
  } else {
    switch (Command) {
    case MMC_CMD0:
      Translation = CMD0;
      break;
    case MMC_CMD1:
      Translation = CMD1;
      break;
    case MMC_CMD2:
      Translation = CMD2;
      break;
    case MMC_CMD3:
      Translation = CMD3;
      break;
    case MMC_CMD5:
      Translation = CMD5;
      break;
    case MMC_CMD6:
      Translation = CMD6;
      break;
    case MMC_CMD7:
      Translation = CMD7;
      break;
    case MMC_CMD8: {
      if (Argument == CMD8_SD_ARG) {
        Translation = CMD8_SD;
        DEBUG ((DEBUG_MMCHOST_SD, "Sending SD CMD8 variant\n"));
      } else {
        ASSERT (Argument == CMD8_MMC_ARG);
        Translation = CMD8_MMC;
        DEBUG ((DEBUG_MMCHOST_SD, "Sending MMC CMD8 variant\n"));
      }
      break;
    }
    case MMC_CMD9:
      Translation = CMD9;
      break;
    case MMC_CMD11:
      Translation = CMD11;
      break;
    case MMC_CMD12:
      Translation = CMD12;
      break;
    case MMC_CMD13:
      Translation = CMD13;
      break;
    case MMC_CMD16:
      Translation = CMD16;
      break;
    case MMC_CMD17:
      Translation = CMD17;
      break;
    case MMC_CMD18:
      Translation = CMD18;
      break;
    case MMC_CMD23:
      Translation = CMD23;
      break;
    case MMC_CMD24:
      Translation = CMD24;
      break;
    case MMC_CMD25:
      Translation = CMD25;
      break;
    case MMC_CMD55:
      Translation = CMD55;
      break;
    default:
      DEBUG ((DEBUG_ERROR, "MMCHost: TranslateCommand(): Unrecognized Command: %d\n", Command));
    }
  }

  return Translation;
}

/**
   Repeatedly polls a register until its value becomes correct, or until MAX_RETRY_COUNT polls is reached
**/
EFI_STATUS
PollRegisterWithMask (
  IN UINTN Register,
  IN UINTN Mask,
  IN UINTN ExpectedValue
  )
{
  UINTN RetryCount = 0;

  while (RetryCount < MAX_RETRY_COUNT) {
    if ((MmioRead32 (Register) & Mask) != ExpectedValue) {
      RetryCount++;
      MicroSecondDelay(2);
    } else {
      break;
    }
  }

  if (RetryCount == MAX_RETRY_COUNT) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

/**
   Calculate the clock divisor
**/
EFI_STATUS
CalculateClockFrequencyDivisor (
  IN UINTN TargetFrequency,
  OUT UINT32 *DivisorValue,
  OUT UINTN *ActualFrequency
  )
{
  EFI_STATUS Status;
  UINT32 Divisor;
  UINTN BaseFrequency = 0;

  Status = SdhciGetClockRate(TargetFrequency, &BaseFrequency);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Couldn't get RPI_MBOX_CLOCK_RATE_EMMC\n"));
    return Status;
  }

  ASSERT (BaseFrequency != 0);
  Divisor = BaseFrequency / TargetFrequency;

  // Arasan controller is based on 3.0 spec so the div is multiple of 2
  // Actual Frequency = BaseFequency/(Div*2)
  Divisor /= 2;

  if ((TargetFrequency < BaseFrequency) &&
      (TargetFrequency * 2 * Divisor != BaseFrequency)) {
    Divisor += 1;
  }

  if (Divisor > MAX_DIVISOR_VALUE) {
    Divisor = MAX_DIVISOR_VALUE;
  }

  DEBUG ((DEBUG_MMCHOST_SD, "MMCHost: BaseFrequency 0x%x Divisor 0x%x\n", BaseFrequency, Divisor));

  *DivisorValue = (Divisor & 0xFF) << 8;
  Divisor >>= 8;
  *DivisorValue |= (Divisor & 0x03) << 6;

  if (ActualFrequency) {
    if (Divisor == 0) {
      *ActualFrequency = BaseFrequency;
    } else {
      *ActualFrequency = BaseFrequency / Divisor;
      *ActualFrequency >>= 1;
    }
    DEBUG ((DEBUG_MMCHOST_SD, "MMCHost: *ActualFrequency 0x%x\n", *ActualFrequency));
  }

  DEBUG ((DEBUG_MMCHOST_SD, "MMCHost: *DivisorValue 0x%x\n", *DivisorValue));

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
SdhciSoftReset (
  IN UINT32 Mask
  )
{
  DEBUG ((DEBUG_INFO, "SoftReset with mask 0x%x\n", Mask));

  SdMmioOr32 (MMCHS_SYSCTL, Mask);
  if (PollRegisterWithMask (MMCHS_SYSCTL, Mask, 0) == EFI_TIMEOUT) {
    DEBUG ((DEBUG_ERROR, "Failed to SoftReset with mask 0x%x\n", Mask));
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

STATIC void 
SdhciTransferPio(
 IN EFI_MMC_HOST_PROTOCOL  *This,
 IN SDHCI_DATA  *data
 )
{
  UINT32 *buf = (UINT32 *)data->buffer;
  UINT32 len = data->blocksize>>2;

  if (data->flags == MMC_DATA_READ) {
    while(len--)
    *buf++ = MmioRead32 (MMCHS_DATA);
  }
  else {
    while(len--)
    MmioWrite32 (MMCHS_DATA, *buf++);
  }
}

STATIC EFI_STATUS
SdhciTransferData(
  IN EFI_MMC_HOST_PROTOCOL  *This,
  IN SDHCI_DATA             *data,
  IN UINTN StartAddr
  )
{
  unsigned int stat, rdy, mask, timeout, block = 0;
  BOOLEAN transfer_done = FALSE;
#ifdef CONFIG_MMC_SDHCI_SDMA
  unsigned char ctrl;
  ctrl = MmioRead8(MMCHS_HCTL);
  ctrl &= ~SDHCI_CTRL_DMA_MASK;
  MmioWrite8(MMCHS_HCTL, ctrl);
#endif

  timeout = 1000000;
  rdy = SDHCI_INT_SPACE_AVAIL | SDHCI_INT_DATA_AVAIL;
  mask = SDHCI_DATA_AVAILABLE | SDHCI_SPACE_AVAILABLE;
  do {
    stat = MmioRead32(MMCHS_INT_STAT);
    if (stat & SDHCI_INT_ERROR) {
      //UINT32 i, *Buffer;
      DEBUG ((DEBUG_ERROR, "+++++%a Error detected in status(0x%X)!\n", __FUNCTION__, stat));
      #if 0
      IomemShow("emmc regs", (unsigned long)mMmcHsBase, 0, 0x3E);
      IomemShow("PHY regs", (unsigned long)(mMmcHsBase+0x800), 0, 0x13);
      Buffer = (UINT32*)data->buffer;
      for (i = 0; i < 128*data->blocks; i++) {
        if (0xCCCCCCCC == Buffer[i]) {
          break;
        }
      }
      
      DEBUG ((DEBUG_ERROR, "Buffer[%u]:0x%x, 0x%x\n", i-2, Buffer[i-2], Buffer[i-1]));
      DEBUG ((DEBUG_ERROR, "blocks:%u Trans size:%u, blks:%u\n", data->blocks, i*4, i/128));
      IomemShow("last buf", (unsigned long)&Buffer[i], 0, 0x1FF);
      #endif
      return EFI_DEVICE_ERROR;
    }
    if (!transfer_done && (stat & rdy)) {
      if (!(MmioRead32(MMCHS_PRES_STATE) & mask))
        continue;
      MmioWrite32(MMCHS_INT_STAT, rdy);
      SdhciTransferPio(This, data);
      data->buffer += data->blocksize;
      if (++block >= data->blocks) {
        /* Keep looping until the SDHCI_INT_DATA_END is
         * cleared, even if we finished sending all the
         * blocks.
         */
        transfer_done = TRUE;
        continue;
      }
    }
#ifdef CONFIG_MMC_SDHCI_SDMA
  if (!transfer_done && (stat & SDHCI_INT_DMA_END)) {
    MmioWrite32(MMCHS_INT_STAT, SDHCI_INT_DMA_END);
    StartAddr &= ~(SDHCI_DEFAULT_BOUNDARY_SIZE - 1);
    StartAddr += SDHCI_DEFAULT_BOUNDARY_SIZE;
    MmioWrite32 (MMCHS_DMA_ADDRESS, (UINT32)StartAddr);
  }
#endif

    if (timeout-- > 0)
      MicroSecondDelay(5);
    else {
      DEBUG ((DEBUG_ERROR, "%a Transfer data timeout\n", __FUNCTION__));
      return EFI_TIMEOUT;
    }
  } while (!(stat & SDHCI_INT_DATA_END));

  return EFI_SUCCESS;
}

EFI_STATUS
SdhciSendCommand (
  IN EFI_MMC_HOST_PROTOCOL    *This,
  IN MMC_CMD                  MmcCmd,
  IN UINT32                   Argument,
  IN UINT32                   CmdSendOKMask
  )
{
  UINTN MmcStatus;
  UINTN RetryCount = 0;
  EFI_STATUS Status = EFI_SUCCESS;

  DEBUG ((DEBUG_MMCHOST_SD, "DATA_CMD REG:0x%x 0x%x  0x%x\n", MmcCmd, Argument, LastExecutedCommand));
  DEBUG ((DEBUG_MMCHOST_SD, "BLK SIZE:0x%x 0x%x\n", MmioRead16(MMCHS_BLK_SIZE), MmioRead16(MMCHS_BLK_COUNT)));
  DEBUG ((DEBUG_MMCHOST_SD, "PSTATE:0x%x 0x%x\n", MmioRead32(MMCHS_PRES_STATE), MmioRead32(MMCHS_INT_STAT)));
  //IomemShow("SDHCI", mMmcHsBase, 0, 0xFF);

  MmioWrite16 (MMCHS_TRANS_MODE, (MmcCmd&0xffff));
  // Set command argument register
  MmioWrite32 (MMCHS_ARG, Argument);

  //DEBUG ((DEBUG_ERROR, "MmcCmd:0x%x\n",  (MmcCmd>>16)));
  // Send the command
  MmioWrite16 (MMCHS_CMD16, (MmcCmd>>16));

  // Check for the command status.
  while (RetryCount < MAX_RETRY_COUNT) {
    MmcStatus = MmioRead32(MMCHS_INT_STAT);

    // Read status of command response
    if ((MmcStatus & ERRI) != 0) {
      // CMD5 (CMD_IO_SEND_OP_COND) is only valid for SDIO
      // cards and thus expected to fail.

      DEBUG ((DEBUG_ERROR, "%a(%u): MMC_CMD%u completion TIMEOUT PresState 0x%x MmcStatus 0x%x,0x%x\n",
             __FUNCTION__, __LINE__, MMC_CMD_NUM (MmcCmd),
      MmioRead32 (MMCHS_PRES_STATE), MmcStatus, CmdSendOKMask));

      SdhciSoftReset(SRC);

      Status = EFI_DEVICE_ERROR;
      goto Exit;
    }

    // Check if command is completed.
    if ((MmcStatus & CmdSendOKMask) == CmdSendOKMask) {
      SdMmioWrite32 (MMCHS_INT_STAT, CmdSendOKMask);
      break;
    }

    RetryCount++;
    MicroSecondDelay(1);
  }

  if (RetryCount == MAX_RETRY_COUNT) {
    DEBUG ((DEBUG_ERROR, "%a(%u): MMC_CMD%u completion TIMEOUT PresState 0x%x MmcStatus 0x%x,0x%x\n",
           __FUNCTION__, __LINE__, MMC_CMD_NUM (MmcCmd),
    MmioRead32 (MMCHS_PRES_STATE), MmcStatus, CmdSendOKMask));
    Status = EFI_TIMEOUT;
    goto Exit;
  }

Exit:
  if (EFI_ERROR (Status)) {
    LastExecutedCommand = (UINT32) -1;
  } else {
    LastExecutedCommand = MmcCmd;
  }
  return Status;
}

EFI_STATUS
MMCSetClock (
  IN UINTN ClockFrequency
  )
{
  EFI_STATUS Status;
  UINT32 Divisor;

  // First turn off the clock
  SdMmioAnd32 (MMCHS_SYSCTL, ~CEN);

  Status = CalculateClockFrequencyDivisor (ClockFrequency, &Divisor, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MMCHost: MmcStandByState(): Fail to initialize SD clock to %u Hz\n",
      ClockFrequency));
    return Status;
  }

  // Setup new divisor
  SdMmioAndThenOr32 (MMCHS_SYSCTL, (UINT32) ~CLKD_MASK, Divisor);

  // Wait for the clock to stabilise
  while ((MmioRead32 (MMCHS_SYSCTL) & ICS_MASK) != ICS);

  // Set Data Timeout Counter value, set clock frequency, enable internal clock
  SdMmioOr32 (MMCHS_SYSCTL, CEN);

  return EFI_SUCCESS;
}

BOOLEAN
MMCIsReadOnly (
  IN EFI_MMC_HOST_PROTOCOL *This
  )
{
  BOOLEAN IsReadOnly = !((MmioRead32 (MMCHS_PRES_STATE) & WRITE_PROTECT_OFF) == WRITE_PROTECT_OFF);
  DEBUG ((DEBUG_MMCHOST_SD, "MMCHost: MMCIsReadOnly(): %d\n", IsReadOnly));
  return IsReadOnly;
}

EFI_STATUS
MMCBuildDevicePath (
  IN EFI_MMC_HOST_PROTOCOL       *This,
  IN EFI_DEVICE_PATH_PROTOCOL    **DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL *NewDevicePathNode;
  EFI_GUID DevicePathGuid = EFI_CALLER_ID_GUID;

  DEBUG ((DEBUG_MMCHOST_SD, "MMCHost: MMCBuildDevicePath()\n"));

  NewDevicePathNode = CreateDeviceNode (HARDWARE_DEVICE_PATH, HW_VENDOR_DP, sizeof (VENDOR_DEVICE_PATH));
  CopyGuid (&((VENDOR_DEVICE_PATH*) NewDevicePathNode)->Guid, &DevicePathGuid);
  *DevicePath = NewDevicePathNode;

  return EFI_SUCCESS;
}

EFI_STATUS
MMCSendCommand (
  IN EFI_MMC_HOST_PROTOCOL    *This,
  IN MMC_CMD                  MmcCmd,
  IN UINT32                   Argument
  )
{
  UINTN MmcStatus;
  UINTN RetryCount = 0;
  UINTN CmdSendOKMask;
  EFI_STATUS Status = EFI_SUCCESS;
  BOOLEAN IsAppCmd = (LastExecutedCommand == CMD55);
  BOOLEAN IsDATCmd = FALSE;
  //BOOLEAN IsADTCCmd = FALSE;
  SDHCI_DATA *data = &gSdhciData;

  DEBUG ((DEBUG_MMCHOST_SD, "MMCHost: MMCSendCommand(MmcCmd: %08x, Argument: %08x)\n", MmcCmd, Argument));

  if (IgnoreCommand (MmcCmd)) {
    return EFI_SUCCESS;
  }

  MmcCmd = TranslateCommand (MmcCmd, Argument);
  if (MmcCmd == 0xffffffff) {
    return EFI_UNSUPPORTED;
  }

  if ((MmcCmd & CMD_R1_ADTC) == CMD_R1_ADTC) {
    IsDATCmd = TRUE;
  }
  #if 0
  if (((MmcCmd & CMD_R1B) == CMD_R1B &&
    /*
     * Abort commands don't get inhibited by DAT.
     */
    (MmcCmd & TYPE (CMD_TYPE_ABORT)) != TYPE (CMD_TYPE_ABORT)) ||
    IsADTCCmd ||
    /*
     * We want to detect BRR/BWR change.
     */
    MmcCmd == CMD_SEND_STATUS) {
    IsDATCmd = TRUE;
  }
  #endif

  CmdSendOKMask = CMDI_MASK;
  if (IsDATCmd) {
    CmdSendOKMask |= DATI_MASK;
  }

  if (PollRegisterWithMask (MMCHS_PRES_STATE,
    CmdSendOKMask, 0) == EFI_TIMEOUT) {
    DEBUG ((DEBUG_ERROR, "%a(%u): not ready for MMC_CMD%u PresState 0x%x MmcStatus 0x%x\n",
      __FUNCTION__, __LINE__, MMC_CMD_NUM (MmcCmd),
      MmioRead32 (MMCHS_PRES_STATE), MmioRead32 (MMCHS_INT_STAT)));
    Status = EFI_TIMEOUT;
    goto Exit;
  }

  if (IsAppCmd && MmcCmd == ACMD22) {
    SdMmioWrite32 (MMCHS_BLK_SIZE, 4);
    data->blocksize = 4;
  } else if (IsAppCmd && MmcCmd == ACMD51) {
    SdMmioWrite32 (MMCHS_BLK_SIZE, 8);
    data->blocksize = 8;
  } else if (!IsAppCmd && MmcCmd == CMD6) {
    SdMmioWrite32 (MMCHS_BLK_SIZE, 64);
    data->blocksize = 64;
  } else if (IsDATCmd) {
    SdMmioWrite32 (MMCHS_BLK_SIZE, BLEN_512BYTES);
    data->blocksize = 512;
  }

  // Set Data timeout counter value to max value.
  SdMmioAndThenOr32 (MMCHS_SYSCTL, (UINT32) ~DTO_MASK, DTO_VAL);

  // Clear Interrupt Status Register, but not the Card Inserted bit
  // to avoid messing with card detection logic.
  SdMmioWrite32 (MMCHS_INT_STAT, ALL_MASK);

  //DEBUG ((DEBUG_ERROR, "MMC_CMD%u REG:0x%x 0x%x\n", MMC_CMD_NUM(MmcCmd), MmcCmd, Argument));
  if (IsDATCmd) {
    //DEBUG ((DEBUG_ERROR, "MMC_CMD%u Data Cmd\n", MMC_CMD_NUM(MmcCmd)));
    mMmcDataCommand = MmcCmd;
    mMmcDataArgument = Argument;
    LastExecutedCommand = MmcCmd;
    return EFI_SUCCESS;
  }
  // Set command argument register
  SdMmioWrite32 (MMCHS_ARG, Argument);

  // Send the command
  SdMmioWrite32 (MMCHS_CMD, MmcCmd);
  //DEBUG ((DEBUG_ERROR, "MmcCmd:0x%x, 0x%x\n",  (MmcCmd>>16), Argument));

  // Check for the command status.
  while (RetryCount < MAX_RETRY_COUNT) {
    MmcStatus = MmioRead32 (MMCHS_INT_STAT);

    // Read status of command response
    if ((MmcStatus & ERRI) != 0) {
      //
      // CMD5 (CMD_IO_SEND_OP_COND) is only valid for SDIO
      // cards and thus expected to fail.
      //
      if (MmcCmd != CMD_IO_SEND_OP_COND) {
        DEBUG ((DEBUG_ERROR, "%a(%u): MMC_CMD%u ERRI MmcStatus 0x%x,0x%x\n",
          __FUNCTION__, __LINE__, MMC_CMD_NUM (MmcCmd), MmcStatus, CmdSendOKMask));
        DEBUG ((DEBUG_ERROR, "MMC_CMD REG:0x%x  0x%x\n", MmioRead32(MMCHS_CMD), MmioRead32(MMCHS_ARG)));
      }

      SdhciSoftReset(SRC);

      Status = EFI_DEVICE_ERROR;
      goto Exit;
    }

    // Check if command is completed.
    if ((MmcStatus & CmdSendOKMask) == CmdSendOKMask) {
      SdMmioWrite32 (MMCHS_INT_STAT, CmdSendOKMask);
      break;
    }

    RetryCount++;
    MicroSecondDelay(1);
  }

  MicroSecondDelay(1);

  if (RetryCount == MAX_RETRY_COUNT) {
    DEBUG ((DEBUG_ERROR, "%a(%u): MMC_CMD%u completion TIMEOUT PresState 0x%x MmcStatus 0x%x,0x%x\n",
      __FUNCTION__, __LINE__, MMC_CMD_NUM (MmcCmd),
      MmioRead32 (MMCHS_PRES_STATE), MmcStatus, CmdSendOKMask));
    Status = EFI_TIMEOUT;
    goto Exit;
  }

Exit:
  SdhciSoftReset(SRC | SRD);
  if (EFI_ERROR (Status)) {
    LastExecutedCommand = (UINT32) -1;
  } else {
    LastExecutedCommand = MmcCmd;
  }
  return Status;
}

EFI_STATUS
MMCNotifyState (
  IN EFI_MMC_HOST_PROTOCOL    *This,
  IN MMC_STATE                State
  )
{
  EFI_STATUS Status;
  UINTN ClockFrequency;
  UINT32 Divisor;

  DEBUG ((DEBUG_MMCHOST_SD, "MMCHost: MMCNotifyState(State: %d)\n", State));

  switch (State) {
  case MmcHwInitializationState:
    {
      DEBUG ((DEBUG_MMCHOST_SD, "MMCHost: current divisor %x\n", MmioRead32(MMCHS_SYSCTL)));
      DEBUG ((DEBUG_MMCHOST_SD, "MMCHost Release: %a, %a\n",  __DATE__, __TIME__));
      Status = SdhciSoftReset(SRA);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      DEBUG ((DEBUG_MMCHOST_SD, "MMCHost: CAP %X CAPH %X\n", MmioRead32(MMCHS_CAPA),MmioRead32(MMCHS_CUR_CAPA)));

      // Lets switch to card detect test mode.
      //SdMmioOr32 (MMCHS_HCTL, BIT7|BIT6);

      // set card voltage
      SdMmioAnd32 (MMCHS_HCTL, ~SDBP_ON);
      SdMmioAndThenOr32 (MMCHS_HCTL, (UINT32) ~SDBP_MASK, SDVS_1_8_V);
      SdMmioOr32 (MMCHS_HCTL, SDBP_ON);

      DEBUG ((DEBUG_MMCHOST_SD, "MMCHost: AC12 %X HCTL %X\n", MmioRead32(MMCHS_AC12),MmioRead32(MMCHS_HCTL)));

      // First turn off the clock
      SdMmioAnd32 (MMCHS_SYSCTL, ~CEN);

      // Attempt to set the clock to 400Khz which is the expected initialization speed
      Status = CalculateClockFrequencyDivisor (400000, &Divisor, NULL);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "MMCHost: MMCNotifyState(): Fail to initialize SD clock\n"));
        return Status;
      }

      // Set Data Timeout Counter value, set clock frequency, enable internal clock
      SdMmioOr32 (MMCHS_SYSCTL, DTO_VAL | Divisor | CEN | ICS | ICE);
      SdMmioOr32 (MMCHS_HCTL, SDBP_ON);
      // wait for ICS
      while ((MmioRead32 (MMCHS_SYSCTL) & ICS_MASK) != ICS);

      DEBUG ((DEBUG_MMCHOST_SD, "MMCHost: AC12 %X HCTL %X\n", MmioRead32(MMCHS_AC12),MmioRead32(MMCHS_HCTL)));

      // Enable interrupts
      SdMmioWrite32 (MMCHS_IE, ALL_EN);
    }
    break;
  case MmcIdleState:
    break;
  case MmcReadyState:
    break;
  case MmcIdentificationState:
    break;
  case MmcStandByState:
    ClockFrequency = 25000000;
    Status = MMCSetClock(ClockFrequency);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "MMCHost: MmcStandByState(): Fail to initialize SD clock to %u Hz\n",
        ClockFrequency));
      return Status;
    }
    break;
  case MmcTransferState:
    break;
  case MmcSendingDataState:
    break;
  case MmcReceiveDataState:
    break;
  case MmcProgrammingState:
    break;
  case MmcDisconnectState:
  case MmcInvalidState:
  default:
    DEBUG ((DEBUG_ERROR, "MMCHost: MMCNotifyState(): Invalid State: %d\n", State));
    ASSERT (0);
  }

  return EFI_SUCCESS;
}

BOOLEAN
MMCIsCardPresent (
  IN EFI_MMC_HOST_PROTOCOL *This
)
{
  return TRUE;
}

EFI_STATUS
MMCReceiveResponse (
  IN EFI_MMC_HOST_PROTOCOL    *This,
  IN MMC_RESPONSE_TYPE        Type,
  IN UINT32*                  Buffer
  )
{
  ASSERT (Buffer != NULL);

  if (Type == MMC_RESPONSE_TYPE_R2) {

    // 16-byte response
    Buffer[0] = MmioRead32 (MMCHS_RSP10);
    Buffer[1] = MmioRead32 (MMCHS_RSP32);
    Buffer[2] = MmioRead32 (MMCHS_RSP54);
    Buffer[3] = MmioRead32 (MMCHS_RSP76);

    Buffer[3] <<= 8;
    Buffer[3] |= (Buffer[2] >> 24) & 0xFF;
    Buffer[2] <<= 8;
    Buffer[2] |= (Buffer[1] >> 24) & 0xFF;
    Buffer[1] <<= 8;
    Buffer[1] |= (Buffer[0] >> 24) & 0xFF;
    Buffer[0] <<= 8;

    DEBUG ((DEBUG_MMCHOST_SD,
      "MMCHost: MMCReceiveResponse(Type: %x), Buffer[0-3]: %08x, %08x, %08x, %08x\n",
      Type, Buffer[0], Buffer[1], Buffer[2], Buffer[3]));
  } else {
    // 4-byte response
    Buffer[0] = MmioRead32 (MMCHS_RSP10);
    DEBUG ((DEBUG_MMCHOST_SD, "MMCHost: MMCReceiveResponse(Type: %08x), Buffer[0]: %08x\n", Type, Buffer[0]));
  }

  gBS->Stall (STALL_AFTER_REC_RESP_US);
  if (LastExecutedCommand == CMD_STOP_TRANSMISSION) {
    DEBUG ((DEBUG_MMCHOST_SD, "MMCHost: soft-resetting after CMD12\n"));
    return SdhciSoftReset(SRC | SRD);
  }
  return EFI_SUCCESS;
}

EFI_STATUS
MMCReadBlockData (
  IN EFI_MMC_HOST_PROTOCOL    *This,
  IN EFI_LBA                  Lba,
  IN UINTN                    Length,
  IN UINT32*                  Buffer
  )
{

  UINT16 Mode;
  EFI_STATUS Status = EFI_SUCCESS;
  SDHCI_DATA *data = &gSdhciData;
  UINT32 cmd = 0;
  UINTN StartAddr = (UINTN)Buffer;

  DEBUG ((DEBUG_MMCHOST_SD, "%a(%u): LBA: 0x%x, Length: 0x%x, Buffer: 0x%x)\n",
    __FUNCTION__, __LINE__, Lba, Length, Buffer));

  if (Buffer == NULL) {
    DEBUG ((DEBUG_ERROR, "%a(%u): NULL Buffer\n", __FUNCTION__, __LINE__));
    return EFI_INVALID_PARAMETER;
  }

  if ((StartAddr & 0x7) != 0x0) {
    DEBUG ((DEBUG_ERROR, "%a(%u): Buffer need 8 Byte align\n", __FUNCTION__, __LINE__));
    return EFI_INVALID_PARAMETER;
  }

  if (Length % sizeof (UINT32) != 0) {
    DEBUG ((DEBUG_ERROR, "%a(%u): bad Length %u\n", __FUNCTION__, __LINE__, Length));
    return EFI_INVALID_PARAMETER;
  }

  if (data->blocksize == 0 || data->blocksize > 512)
  {
    DEBUG ((DEBUG_ERROR, "%a(%u): blocksize %u\n", __FUNCTION__, __LINE__, data->blocksize));
    return EFI_INVALID_PARAMETER;
  }

  data->flags = MMC_DATA_READ;
  data->blocks = (UINT32)((Length) / data->blocksize);
  data->buffer = (UINT8 *)Buffer;
  //memset(Buffer, 0xCC, Length);

  MmioWrite16 (MMCHS_BLK_SIZE, SDHCI_MAKE_BLKSZ(SDHCI_DEFAULT_BOUNDARY_ARG, data->blocksize));
  MmioWrite16 (MMCHS_BLK_COUNT, (UINT16)data->blocks);

  Mode = SDHCI_TRNS_BLK_CNT_EN | SDHCI_TRNS_READ;
  if (data->blocks > 1)
    Mode |= (SDHCI_TRNS_MULTI /*| SDHCI_TRNS_ACMD12*/);

#ifdef CONFIG_MMC_SDHCI_SDMA
  MmioWrite32 (MMCHS_DMA_ADDRESS, (UINT32)StartAddr);
  Mode |= SDHCI_TRNS_DMA;
  InvalidateDataCacheRange (Buffer, Length);
#endif
  //MmioWrite16 (MMCHS_TRANS_MODE, Mode);
  cmd = (mMmcDataCommand & 0xFFFF0000) | Mode;
  Status = SdhciSendCommand(This, cmd, mMmcDataArgument, CMDI_MASK /*| DATI_MASK*/);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(%u): bad cmd %x\n", __FUNCTION__, __LINE__, mMmcDataCommand));
    goto Exit;
  }

  Status = SdhciTransferData(This, data, StartAddr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(%u): SdhciTransferData fail!\n", __FUNCTION__, __LINE__));
    goto Exit;
  }

  MicroSecondDelay(1);
Exit:
  //stat = MmioRead32(MMCHS_INT_STAT);
  MmioWrite32 (MMCHS_INT_STAT, ALL_MASK);
  SdhciSoftReset(SRC | SRD);
  return Status;
}

EFI_STATUS
MMCWriteBlockData (
  IN EFI_MMC_HOST_PROTOCOL    *This,
  IN EFI_LBA                  Lba,
  IN UINTN                    Length,
  IN UINT32*                  Buffer
  )
{
  UINT16 Mode;
  EFI_STATUS Status = EFI_SUCCESS;
  SDHCI_DATA *data = &gSdhciData;
  UINT32 cmd = 0;
  UINTN StartAddr = (UINTN)Buffer;

  DEBUG ((DEBUG_MMCHOST_SD, "%a(%u): LBA: 0x%x, Length: 0x%x, Buffer: 0x%x)\n",
    __FUNCTION__, __LINE__, Lba, Length, Buffer));

  if (Buffer == NULL) {
    DEBUG ((DEBUG_ERROR, "%a(%u): NULL Buffer\n", __FUNCTION__, __LINE__));
    return EFI_INVALID_PARAMETER;
  }

  if ((StartAddr & 0x7) != 0x0) {
    DEBUG ((DEBUG_ERROR, "%a(%u): Buffer need 8 Byte align\n", __FUNCTION__, __LINE__));
    return EFI_INVALID_PARAMETER;
  }

  if (Length % sizeof (UINT32) != 0) {
    DEBUG ((DEBUG_ERROR, "%a(%u): bad Length %u\n", __FUNCTION__, __LINE__, Length));
    return EFI_INVALID_PARAMETER;
  }

  data->flags = MMC_DATA_WRITE;
  data->blocks = (UINT32)((Length + data->blocksize - 1) / data->blocksize);
  data->buffer = (UINT8 *)Buffer;
  MmioWrite16 (MMCHS_BLK_SIZE, SDHCI_MAKE_BLKSZ(SDHCI_DEFAULT_BOUNDARY_ARG, data->blocksize));
  MmioWrite16 (MMCHS_BLK_COUNT, (UINT16)data->blocks);

  Mode = SDHCI_TRNS_BLK_CNT_EN;
  if (data->blocks > 1)
    Mode |= SDHCI_TRNS_MULTI;

#ifdef CONFIG_MMC_SDHCI_SDMA
  MmioWrite32 (MMCHS_DMA_ADDRESS, (UINT32)StartAddr);
  Mode |= SDHCI_TRNS_DMA;
  WriteBackDataCacheRange (Buffer, Length);
#endif

  cmd = (mMmcDataCommand & 0xFFFF0000) | Mode;
  Status = SdhciSendCommand(This, cmd, mMmcDataArgument, CMDI_MASK /*| DATI_MASK*/);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(%u): bad cmd %x\n", __FUNCTION__, __LINE__, mMmcDataCommand));
    goto Exit;
  }

  Status = SdhciTransferData(This, data, StartAddr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a(%u): SdhciTransferData fail!\n", __FUNCTION__, __LINE__));
    goto Exit;
  }

  MicroSecondDelay(1);
Exit:
  //stat = MmioRead32(MMCHS_INT_STAT);
  MmioWrite32 (MMCHS_INT_STAT, ALL_MASK);
  SdhciSoftReset(SRC | SRD);
  return Status;
}

EFI_STATUS
MMCSetIos (
  IN EFI_MMC_HOST_PROTOCOL      *This,
  IN  UINT32                    BusClockFreq,
  IN  UINT32                    BusWidth,
  IN  UINT32                    TimingMode
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32    Ctrl, Ctrl2;

  if (BusClockFreq > 200*1000*1000) {
    return EFI_UNSUPPORTED;
  }

  Ctrl = MmioRead8(MMCHS_HCTL);
  if (TimingMode != EMMCBACKWARD) {
    Ctrl2 = MmioRead16(MMCHS_HCTL2);
    /* Select Bus Speed Mode for host */
    Ctrl2 &= ~SDHCI_CTRL_UHS_MASK;

    switch (TimingMode) {
    case EMMCHS52DDR1V2:
    case EMMCHS52DDR1V8:
    case EMMCHS52:
      //Ctrl |= HIGH_SPEED_EN;
      Ctrl &= ~HIGH_SPEED_EN;
      Ctrl2 |= (SDHCI_CTRL_VDD_180 | SDHCI_CTRL_DRV_TYPE_A | SDHCI_CTRL_EMMC_SDR50);
      break;
    case EMMCBACKWARD:
    case EMMCHS26:
      Ctrl &= ~HIGH_SPEED_EN;
      break;
    case EMMCHS400DDR1V8:
    case EMMCHS400DDR1V2:
      Ctrl |= HIGH_SPEED_EN;
      Ctrl2 |= (SDHCI_CTRL_VDD_180 | SDHCI_CTRL_DRV_TYPE_A | SDHCI_CTRL_EMMC_HS400);
      break;

    default:
      return EFI_UNSUPPORTED;
    }
    MmioWrite16(MMCHS_HCTL2, Ctrl2);
  }

  switch (BusWidth) {
  case 1:
    Ctrl &= ~DTW_4_BIT;
    break;
  case 4:
    Ctrl |= DTW_4_BIT;;
    break;
  case 8:
    Ctrl &= ~DTW_4_BIT;
    Ctrl |= DTW_8_BIT;
    break;
  default:
    return EFI_UNSUPPORTED;
  }

  MmioWrite8(MMCHS_HCTL, Ctrl);
  if (BusClockFreq) {
    Status = MMCSetClock (BusClockFreq);
  }
  return Status;
}

BOOLEAN
MMCIsMultiBlock (
  IN EFI_MMC_HOST_PROTOCOL *This
  )
{
  return TRUE;
}

EFI_MMC_HOST_PROTOCOL gSdhciHost =
{
  MMC_HOST_PROTOCOL_REVISION,
  MMCIsCardPresent,
  MMCIsReadOnly,
  MMCBuildDevicePath,
  MMCNotifyState,
  MMCSendCommand,
  MMCReceiveResponse,
  MMCReadBlockData,
  MMCWriteBlockData,
  MMCSetIos,
  MMCIsMultiBlock
};

EFI_STATUS
MMCInitialize (
  IN EFI_HANDLE          ImageHandle,
  IN EFI_SYSTEM_TABLE    *SystemTable
  )
{
  EFI_STATUS Status;
  EFI_HANDLE Handle = NULL;

  DEBUG ((DEBUG_MMCHOST_SD, "RkSdhciHost: MMCInitialize()\n"));

  mMmcHsBase = PcdGet32 (PcdSdhciDxeBaseAddress);
  SdhciEmmcDxeIoMux();

  #if 0
  word32(mMmcHsBase + 0x800) = (0x1 << 1);
  MicroSecondDelay(10);
  word32(mMmcHsBase + 0x800) = 0;

  word32(0xFD7C0000 + 0x0A7C) = ((0x1FUL << 4) << 16) | (0x1F << 4);
  MicroSecondDelay(10);
  word32(0xFD7C0000 + 0x0A7C) = ((0x1FUL << 4) << 16) | (0 << 4);

  word32(0xFD5FD000 + 0x40) = 0xFFFF6666;
  word32(0xFD5FD000 + 0x58) = 0xFFFF6666;
  word32(0xFD5FD000 + 0x5C) = 0xFFFF6666;
  #endif

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gEmbeddedMmcHostProtocolGuid,
                  &gSdhciHost,
                  NULL
                );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
