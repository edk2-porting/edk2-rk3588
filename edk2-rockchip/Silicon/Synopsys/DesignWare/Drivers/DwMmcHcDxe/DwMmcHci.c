/** @file
  This driver is used to manage Designware SD/MMC PCI host controllers.

  It would expose EFI_SD_MMC_PASS_THRU_PROTOCOL for upper layer use.

  Copyright (c) 2015 - 2021, Intel Corporation. All rights reserved.<BR>
  Copyright (c) 2018, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <IndustryStandard/Emmc.h>
#include <IndustryStandard/Sd.h>

#include <Library/ArmLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DmaLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "DwMmcHcDxe.h"

/**
  Dump the content of SD/MMC host controller's Capability Register.

  @param[in]  Slot            The slot number of the SD card to send the
                              command to.
  @param[in]  Capability      The buffer to store the capability data.

**/
VOID
DumpCapabilityReg (
  IN UINT8               Slot,
  IN DW_MMC_HC_SLOT_CAP  *Capability
  )
{
  //
  // Dump Capability Data
  //
  DEBUG ((
    DEBUG_INFO,
    " == Slot [%d] Capability is 0x%x ==\n",
    Slot,
    Capability
    ));
  DEBUG ((
    DEBUG_INFO,
    "   Base Clk Freq     %dKHz\n",
    Capability->BaseClkFreq
    ));
  DEBUG ((
    DEBUG_INFO,
    "   BusWidth          %d\n",
    Capability->BusWidth
    ));
  DEBUG ((
    DEBUG_INFO,
    "   HighSpeed Support %a\n",
    Capability->HighSpeed ? "TRUE" : "FALSE"
    ));
  DEBUG ((
    DEBUG_INFO,
    "   Voltage 1.8       %a\n",
    Capability->Voltage18 ? "TRUE" : "FALSE"
    ));
  DEBUG ((
    DEBUG_INFO,
    "   64-bit Sys Bus    %a\n",
    Capability->SysBus64 ? "TRUE" : "FALSE"
    ));
  DEBUG ((DEBUG_INFO, "   SlotType          "));
  if (Capability->SlotType == 0x00) {
    DEBUG ((DEBUG_INFO, "%a\n", "Removable Slot"));
  } else if (Capability->SlotType == 0x01) {
    DEBUG ((DEBUG_INFO, "%a\n", "Embedded Slot"));
  } else if (Capability->SlotType == 0x02) {
    DEBUG ((DEBUG_INFO, "%a\n", "Shared Bus Slot"));
  } else {
    DEBUG ((DEBUG_INFO, "%a\n", "Reserved"));
  }

  DEBUG ((
    DEBUG_INFO,
    "   SDR50  Support    %a\n",
    Capability->Sdr50 ? "TRUE" : "FALSE"
    ));
  DEBUG ((
    DEBUG_INFO,
    "   SDR104 Support    %a\n",
    Capability->Sdr104 ? "TRUE" : "FALSE"
    ));
  DEBUG ((
    DEBUG_INFO,
    "   DDR50  Support    %a\n",
    Capability->Ddr50 ? "TRUE" : "FALSE"
    ));
  return;
}

/**
  Set all interrupt status bits in Normal and Error Interrupt Status Enable
  register.

  @param[in] DevIo          The DEVICE IO protocol instance.

  @retval EFI_SUCCESS       The operation executes successfully.
  @retval Others            The operation fails.

**/
EFI_STATUS
DwMmcHcEnableInterrupt (
  UINTN  DevBase
  )
{
  UINT32  IntStatus;
  UINT32  IdIntEn;
  UINT32  IdSts;

  //
  // Enable all bits in Interrupt Mask Register
  //
  IntStatus = 0;
  MmioWrite32 (DevBase + DW_MMC_INTMASK, IntStatus);

  //
  // Clear status in Interrupt Status Register
  //
  IntStatus = ~0;
  MmioWrite32 (DevBase + DW_MMC_RINTSTS, IntStatus);

  IdIntEn = ~0;
  MmioWrite32 (DevBase + DW_MMC_IDINTEN, IdIntEn);

  IdSts = ~0;
  MmioWrite32 (DevBase + DW_MMC_IDSTS, IdSts);

  return EFI_SUCCESS;
}

EFI_STATUS
DwMmcHcGetCapability (
  IN     UINTN            DevBase,
  IN     EFI_HANDLE       Controller,
  IN     UINT8            Slot,
  OUT DW_MMC_HC_SLOT_CAP  *Capacity
  )
{
  PLATFORM_DW_MMC_PROTOCOL  *PlatformDwMmc;
  EFI_STATUS                Status;

  if (Capacity == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gPlatformDwMmcProtocolGuid,
                  NULL,
                  (VOID **)&PlatformDwMmc
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = PlatformDwMmc->GetCapability (Controller, Slot, Capacity);
  return Status;
}

/**
  Detect whether there is a SD/MMC card attached at the specified SD/MMC host
  controller slot.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.1 for details.

  @param[in]  DevIo         The DEVICE IO protocol instance.
  @param[in]  Slot          The slot number of the SD card to send the command
                            to.
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
  )
{
  PLATFORM_DW_MMC_PROTOCOL  *PlatformDwMmc;
  EFI_STATUS                Status;

  if (MediaPresent == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (
                  &gPlatformDwMmcProtocolGuid,
                  NULL,
                  (VOID **)&PlatformDwMmc
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *MediaPresent = PlatformDwMmc->CardDetect (Controller, Slot);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
DwMmcHcUpdateClock (
  IN UINTN  DevBase
  )
{
  UINT32  Cmd;
  UINT32  IntStatus;

  Cmd = BIT_CMD_WAIT_PRVDATA_COMPLETE | BIT_CMD_UPDATE_CLOCK_ONLY |
        BIT_CMD_START;
  MmioWrite32 (DevBase + DW_MMC_CMD, Cmd);

  while (1) {
    Cmd = MmioRead32 (DevBase + DW_MMC_CMD);

    if (!(Cmd & CMD_START_BIT)) {
      break;
    }

    IntStatus = MmioRead32 (DevBase + DW_MMC_RINTSTS);

    if (IntStatus & DW_MMC_INT_HLE) {
      DEBUG ((
        DEBUG_ERROR,
        "DwMmcHcUpdateClock: failed to update mmc clock frequency\n"
        ));
      return EFI_DEVICE_ERROR;
    }
  }

  return EFI_SUCCESS;
}

/**
  Stop SD/MMC card clock.

  @param[in] DevIo          The DEVICE IO protocol instance.

  @retval EFI_SUCCESS       Succeed to stop SD/MMC clock.
  @retval Others            Fail to stop SD/MMC clock.

**/
EFI_STATUS
DwMmcHcStopClock (
  IN UINTN  DevBase
  )
{
  EFI_STATUS  Status;
  UINT32      ClkEna;

  //
  // Disable MMC clock first
  //
  ClkEna = 0;
  MmioWrite32 (DevBase + DW_MMC_CLKENA, ClkEna);

  Status = DwMmcHcUpdateClock (DevBase);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return Status;
}

/**
  SD/MMC card clock supply.

  @param[in] DevIo          The DEVICE IO protocol instance.
  @param[in] ClockFreq      The max clock frequency to be set. The unit is KHz.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The clock is supplied successfully.
  @retval Others            The clock isn't supplied successfully.

**/
EFI_STATUS
DwMmcHcClockSupply (
  IN UINTN               DevBase,
  IN UINT64              ClockFreq,
  IN DW_MMC_HC_SLOT_CAP  Capability
  )
{
  EFI_STATUS  Status;
  UINT32      BaseClkFreq;
  UINT32      SettingFreq;
  UINT32      Divisor;
  UINT32      Remainder;
  UINT32      MmcStatus;
  UINT32      ClkEna;
  UINT32      ClkSrc;

  //
  // Calculate a divisor for SD clock frequency
  //
  ASSERT (Capability.BaseClkFreq != 0);

  BaseClkFreq = Capability.BaseClkFreq;
  if (ClockFreq == 0) {
    return EFI_INVALID_PARAMETER;
  }

  if (ClockFreq > BaseClkFreq) {
    ClockFreq = BaseClkFreq;
  }

  //
  // Calculate the divisor of base frequency.
  //
  Divisor     = 0;
  SettingFreq = BaseClkFreq;
  while (ClockFreq < SettingFreq) {
    Divisor++;

    SettingFreq = BaseClkFreq / (2 * Divisor);
    Remainder   = BaseClkFreq % (2 * Divisor);
    if ((ClockFreq == SettingFreq) && (Remainder == 0)) {
      break;
    }

    if ((ClockFreq == SettingFreq) && (Remainder != 0)) {
      SettingFreq++;
    }
  }

  DEBUG ((
    DEBUG_INFO,
    "BaseClkFreq %dKHz Divisor %d ClockFreq %dKhz\n",
    BaseClkFreq,
    Divisor,
    ClockFreq
    ));

  //
  // Wait until MMC is idle
  //
  do {
    MmcStatus = MmioRead32 (DevBase + DW_MMC_STATUS);
  } while (MmcStatus & DW_MMC_STS_DATA_BUSY);

  do {
    Status = DwMmcHcStopClock (DevBase);
  } while (EFI_ERROR (Status));

  do {
    ClkSrc = 0;
    MmioWrite32 (DevBase + DW_MMC_CLKSRC, ClkSrc);
    //
    // Set clock divisor
    //
    MmioWrite32 (DevBase + DW_MMC_CLKDIV, Divisor);
    //
    // Enable MMC clock
    //
    ClkEna = 1;
    MmioWrite32 (DevBase + DW_MMC_CLKENA, ClkEna);

    Status = DwMmcHcUpdateClock (DevBase);
  } while (EFI_ERROR (Status));

  return EFI_SUCCESS;
}

/**
  Set the SD/MMC bus width.

  Refer to SD Host Controller Simplified spec 3.0 Section 3.4 for details.

  @param[in] DevIo          The DEVICE IO protocol instance.
  @param[in] IsDdr          A boolean to indicate it's dual data rate or not.
  @param[in] BusWidth       The bus width used by the SD/MMC device, it must be
                            1, 4 or 8.

  @retval EFI_SUCCESS       The bus width is set successfully.
  @retval Others            The bus width isn't set successfully.

**/
EFI_STATUS
DwMmcHcSetBusWidth (
  IN UINTN    DevBase,
  IN BOOLEAN  IsDdr,
  IN UINT16   BusWidth
  )
{
  UINT32  Ctype;
  UINT32  Uhs;

  switch (BusWidth) {
    case 1:
      Ctype = MMC_1BIT_MODE;
      break;
    case 4:
      Ctype = MMC_4BIT_MODE;
      break;
    case 8:
      Ctype = MMC_8BIT_MODE;
      break;
    default:
      return EFI_INVALID_PARAMETER;
  }

  MmioWrite32 (DevBase + DW_MMC_CTYPE, Ctype);

  Uhs = MmioRead32 (DevBase + DW_MMC_UHSREG);

  if (IsDdr) {
    Uhs |= UHS_DDR_MODE;
  } else {
    Uhs &= ~(UHS_DDR_MODE);
  }

  MmioWrite32 (DevBase + DW_MMC_UHSREG, Uhs);

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS  Status;
  UINT32      InitFreq;

  //
  // Calculate a divisor for SD clock frequency
  //
  if (Capability.BaseClkFreq == 0) {
    //
    // Don't support get Base Clock Frequency information via another method
    //
    return EFI_UNSUPPORTED;
  }

  //
  // Supply 400KHz clock frequency at initialization phase.
  //
  InitFreq = DWMMC_INIT_CLOCK_FREQ;
  Status   = DwMmcHcClockSupply (DevBase, InitFreq, Capability);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MicroSecondDelay (100);
  return Status;
}

STATIC
EFI_STATUS
DwMmcHcWaitReset (
  IN UINTN   DevBase,
  IN UINT32  ResetValue
  )
{
  UINT32  Timeout;
  UINT32  Data;

  MmioWrite32 (DevBase + DW_MMC_CTRL, ResetValue);

  Timeout = DW_MMC_HC_GENERIC_TIMEOUT;
  while (Timeout--) {
    Data = MmioRead32 (DevBase + DW_MMC_CTRL);
    if ((Data & DW_MMC_CTRL_RESET_ALL) == 0) {
      return EFI_SUCCESS;
    }

    gBS->Stall (1);
  }

  return EFI_TIMEOUT;
}

/**
  Supply SD/MMC card with maximum voltage at initialization.

  @param[in] DevIo          The DEVICE IO protocol instance.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The voltage is supplied successfully.
  @retval Others            The voltage isn't supplied successfully.

**/
EFI_STATUS
DwMmcHcInitPowerVoltage (
  IN UINTN               DevBase,
  IN DW_MMC_HC_SLOT_CAP  Capability
  )
{
  EFI_STATUS  Status;

  MmioWrite32 (DevBase + DW_MMC_PWREN, 0x1);

  Status = DwMmcHcWaitReset (DevBase, DW_MMC_CTRL_RESET_ALL);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "DwMmcHcInitPowerVoltage: reset failed due to timeout"
      ));
    return Status;
  }

  MmioWrite32 (DevBase + DW_MMC_CTRL, DW_MMC_CTRL_INT_EN);

  return EFI_SUCCESS;
}

/**
  Initialize the Timeout Control register with most conservative value at
  initialization.

  @param[in] DevIo          The DEVICE IO protocol instance.

  @retval EFI_SUCCESS       The timeout control register is configured
                            successfully.
  @retval Others            The timeout control register isn't configured
                            successfully.

**/
EFI_STATUS
DwMmcHcInitTimeoutCtrl (
  IN UINTN  DevBase
  )
{
  UINT32  Data;

  Data = ~0;
  MmioWrite32 (DevBase + DW_MMC_TMOUT, Data);

  Data = 0x00FFFFFF;
  MmioWrite32 (DevBase + DW_MMC_DEBNCE, Data);

  return EFI_SUCCESS;
}

/**
  Initial SD/MMC host controller with lowest clock frequency, max power and
  max timeout value at initialization.

  @param[in] DevIo          The DEVICE IO protocol instance.
  @param[in] Slot           The slot number of the SD card to send the command
                            to.
  @param[in] Capability     The capability of the slot.

  @retval EFI_SUCCESS       The host controller is initialized successfully.
  @retval Others            The host controller isn't initialized successfully.

**/
EFI_STATUS
DwMmcHcInitHost (
  IN UINTN               DevBase,
  IN DW_MMC_HC_SLOT_CAP  Capability
  )
{
  EFI_STATUS  Status;

  Status = DwMmcHcInitPowerVoltage (DevBase, Capability);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return Status;
}

EFI_STATUS
DwMmcHcStartDma (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  )
{
  EFI_STATUS  Status;
  UINTN       DevBase;
  UINT32      Ctrl;
  UINT32      Bmod;

  //  DevIo  = Trb->Private->DevIo;
  DevBase = Trb->Private->DevBase;

  //
  // Reset DMA
  //
  Status = DwMmcHcWaitReset (DevBase, DW_MMC_CTRL_DMA_RESET);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Timed out waiting for CTRL_DMA_RESET"));
    return Status;
  }

  Bmod = DW_MMC_IDMAC_SWRESET | MmioRead32 (DevBase + DW_MMC_BMOD);

  MmioWrite32 (DevBase + DW_MMC_BMOD, Bmod);

  //
  // Select IDMAC
  //
  Ctrl  = DW_MMC_CTRL_IDMAC_EN;
  Ctrl |= MmioRead32 (DevBase + DW_MMC_CTRL);
  MmioWrite32 (DevBase + DW_MMC_CTRL, Ctrl);

  //
  // Enable IDMAC
  //
  Bmod  = DW_MMC_IDMAC_ENABLE | DW_MMC_IDMAC_FB;
  Bmod |= MmioRead32 (DevBase + DW_MMC_BMOD);

  MmioWrite32 (DevBase + DW_MMC_BMOD, Bmod);

  return EFI_SUCCESS;
}

EFI_STATUS
DwMmcHcStopDma (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  )
{
  UINTN   DevBase;
  UINT32  Ctrl;
  UINT32  Bmod;

  DevBase = Trb->Private->DevBase;

  //
  // Disable and reset IDMAC
  //
  Ctrl  = MmioRead32 (DevBase + DW_MMC_CTRL);
  Ctrl &= ~DW_MMC_CTRL_IDMAC_EN;
  Ctrl |= DW_MMC_CTRL_DMA_RESET;
  MmioWrite32 (DevBase + DW_MMC_CTRL, Ctrl);

  //
  // Stop IDMAC
  //
  Bmod  = MmioRead32 (DevBase + DW_MMC_BMOD);
  Bmod &= ~(DW_MMC_BMOD_FB | DW_MMC_BMOD_DE);
  Bmod |= DW_MMC_BMOD_SWR;
  MmioWrite32 (DevBase + DW_MMC_BMOD, Bmod);

  return EFI_SUCCESS;
}

/**
  Build DMA descriptor table for transfer.

  @param[in] Trb            The pointer to the DW_MMC_HC_TRB instance.

  @retval EFI_SUCCESS       The DMA descriptor table is created successfully.
  @retval Others            The DMA descriptor table isn't created successfully.

**/
EFI_STATUS
BuildDmaDescTable (
  IN DW_MMC_HC_TRB  *Trb
  )
{
  EFI_PHYSICAL_ADDRESS     Data;
  UINT64                   DataLen;
  UINT64                   Entries;
  UINT32                   Index;
  UINT64                   Remaining;
  UINTN                    TableSize;
  UINTN                    DevBase;
  EFI_STATUS               Status;
  UINTN                    Bytes;
  UINTN                    Blocks;
  DW_MMC_HC_DMA_DESC_LINE  *DmaDesc;
  UINT32                   DmaDescPhy;
  UINT32                   Idsts;
  UINT32                   BytCnt;
  UINT32                   BlkSize;

  Data    = Trb->DataPhy;
  DataLen = Trb->DataLen;
  DevBase = Trb->Private->DevBase;
  //
  // Only support 32bit DMA Descriptor Table
  //
  if ((Data >= 0x100000000ul) || ((Data + DataLen) > 0x100000000ul)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Address field shall be set on 32-bit boundary (Lower 2-bit is always set
  // to 0) for 32-bit address descriptor table.
  //
  if ((Data & (BIT0 | BIT1)) != 0) {
    DEBUG ((
      DEBUG_INFO,
      "The buffer [0x%x] to construct DMA desc is not aligned to 4 bytes!\n",
      Data
      ));
  }

  Entries   = (DataLen + DWMMC_DMA_BUF_SIZE - 1) / DWMMC_DMA_BUF_SIZE;
  TableSize = Entries * sizeof (DW_MMC_HC_DMA_DESC_LINE);
  Blocks    = (DataLen + DW_MMC_BLOCK_SIZE - 1) / DW_MMC_BLOCK_SIZE;

  Trb->DmaDescPages = (UINT32)EFI_SIZE_TO_PAGES (Entries * DWMMC_DMA_BUF_SIZE);

  /*  Status = DevIo->AllocateBuffer (
                      DevIo,
                      AllocateAnyPages,
                      EfiBootServicesData,
                      EFI_SIZE_TO_PAGES (TableSize),
                      (EFI_PHYSICAL_ADDRESS *)&Trb->DmaDesc
                      );*/
  Status = DmaAllocateBuffer (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES (TableSize),
             (VOID *)&Trb->DmaDesc
             );
  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (Trb->DmaDesc, TableSize);
  Bytes = TableSize;

  Status = DmaMap (
             MapOperationBusMasterCommonBuffer,
             (EFI_PHYSICAL_ADDRESS *)Trb->DmaDesc,
             &Bytes,
             &Trb->DmaDescPhy,
             &Trb->DmaMap
             );

  /*  Status = DevIo->Map (
                      DevIo,
                      EfiBusMasterCommonBuffer,
                      (EFI_PHYSICAL_ADDRESS *)Trb->DmaDesc,
                      &Bytes,
                      &Trb->DmaDescPhy,
                      &Trb->DmaMap
                      );*/

  if (EFI_ERROR (Status) || (Bytes != TableSize)) {
    //
    // Map error or unable to map the whole RFis buffer into a contiguous
    // region.
    //

    /*    DevIo->FreeBuffer (
                 DevIo,
                 EFI_SIZE_TO_PAGES (TableSize),
                 (EFI_PHYSICAL_ADDRESS)Trb->DmaDesc
                 );*/
    return EFI_OUT_OF_RESOURCES;
  }

  if ((UINT64)(UINTN)Trb->DmaDescPhy > 0x100000000ul) {
    //
    // The DMA doesn't support 64bit addressing.
    //
    DmaUnmap (Trb->DmaMap);

    /*    DevIo->Unmap (
          DevIo,
          Trb->DmaMap
        );*/
    return EFI_DEVICE_ERROR;
  }

  if (DataLen < DW_MMC_BLOCK_SIZE) {
    BlkSize   = DataLen;
    BytCnt    = DataLen;
    Remaining = DataLen;
  } else {
    BlkSize   = DW_MMC_BLOCK_SIZE;
    BytCnt    = DW_MMC_BLOCK_SIZE * Blocks;
    Remaining = DW_MMC_BLOCK_SIZE * Blocks;
  }

  MmioWrite32 (DevBase + DW_MMC_BLKSIZ, BlkSize);
  MmioWrite32 (DevBase + DW_MMC_BYTCNT, BytCnt);
  DmaDesc = Trb->DmaDesc;
  for (Index = 0; Index < Entries; Index++, DmaDesc++) {
    DmaDesc->Des0 = DW_MMC_IDMAC_DES0_OWN | DW_MMC_IDMAC_DES0_CH |
                    DW_MMC_IDMAC_DES0_DIC;
    DmaDesc->Des1 = DW_MMC_IDMAC_DES1_BS1 (DWMMC_DMA_BUF_SIZE);
    //
    // Buffer Address
    //
    DmaDesc->Des2 = (UINT32)((UINTN)Trb->DataPhy +
                             (DWMMC_DMA_BUF_SIZE * Index));
    //
    // Next Descriptor Address
    //
    DmaDesc->Des3 = (UINT32)((UINTN)Trb->DmaDescPhy +
                             sizeof (DW_MMC_HC_DMA_DESC_LINE) * (Index + 1));
    Remaining = Remaining - DWMMC_DMA_BUF_SIZE;
  }

  //
  // First Descriptor
  //
  Trb->DmaDesc[0].Des0 |= DW_MMC_IDMAC_DES0_FS;
  //
  // Last Descriptor
  //
  Trb->DmaDesc[Entries - 1].Des0 &= ~(DW_MMC_IDMAC_DES0_CH |
                                      DW_MMC_IDMAC_DES0_DIC);
  Trb->DmaDesc[Entries - 1].Des0 |= DW_MMC_IDMAC_DES0_OWN |
                                    DW_MMC_IDMAC_DES0_LD;
  Trb->DmaDesc[Entries - 1].Des1 = DW_MMC_IDMAC_DES1_BS1 (
                                     Remaining +
                                     DWMMC_DMA_BUF_SIZE
                                     );
  //
  // Set the next field of the Last Descriptor
  //
  Trb->DmaDesc[Entries - 1].Des3 = 0;
  DmaDescPhy                     = (UINT32)Trb->DmaDescPhy;

  MmioWrite32 (DevBase + DW_MMC_DBADDR, DmaDescPhy);

  ArmDataSynchronizationBarrier ();
  ArmInstructionSynchronizationBarrier ();
  //
  // Clear interrupts
  //
  Idsts = ~0;
  MmioWrite32 (DevBase + DW_MMC_IDSTS, Idsts);

  return Status;
}

EFI_STATUS
TransferFifo (
  IN DW_MMC_HC_TRB  *Trb
  )
{
  UINTN   DevBase;
  UINT32  Data;
  UINT32  Received;
  UINT32  Count;
  UINT32  Intsts;
  UINT32  Sts;
  UINT32  FifoCount;
  UINT32  Index;                       /* count with bytes */
  UINT32  Ascending;
  UINT32  Descending;
  UINT32  Timeout;

  DevBase    = Trb->Private->DevBase;
  Received   = 0;
  Count      = 0;
  Index      = 0;
  Ascending  = 0;
  Descending = ((Trb->DataLen + 3) & ~3) - 4;
  Timeout    = DW_MMC_HC_GENERIC_TIMEOUT;

  do {
    Intsts = MmioRead32 (DevBase + DW_MMC_RINTSTS);

    if (!Trb->Read && (Intsts & DW_MMC_INT_TXDR)) {
      Sts = MmioRead32 (DevBase + DW_MMC_STATUS);

      while (  !(DW_MMC_STS_FIFO_FULL (Sts))
            && (Received < Trb->DataLen)
            && (Intsts & DW_MMC_INT_TXDR))
      {
        if (Trb->UseBE) {
          Data       = SwapBytes32 (*(UINT32 *)((UINTN)Trb->Data + Descending));
          Descending = Descending - 4;
        } else {
          Data       = *(UINT32 *)((UINTN)Trb->Data + Ascending);
          Ascending += 4;
        }

        Index    += 4;
        Received += 4;

        MmioWrite32 (DevBase + DW_MMC_FIFO_START, Data);

        Intsts = DW_MMC_INT_TXDR;
        MmioWrite32 (DevBase + DW_MMC_RINTSTS, Intsts);

        Intsts = MmioRead32 (DevBase + DW_MMC_RINTSTS);
        Sts    = MmioRead32 (DevBase + DW_MMC_STATUS);
      }

      Timeout = DW_MMC_HC_GENERIC_TIMEOUT;
      goto Verify;
    }

    if (Trb->Read && ((Intsts & DW_MMC_INT_RXDR) ||
                      (Intsts & DW_MMC_INT_DTO)))
    {
      Sts = MmioRead32 (DevBase + DW_MMC_STATUS);
      //
      // Convert to bytes
      //
      FifoCount = GET_STS_FIFO_COUNT (Sts) << 2;
      if ((FifoCount == 0) && (Received < Trb->DataLen)) {
        goto Verify;
      }

      Index = 0;
      Count = (MIN (FifoCount, Trb->DataLen) + 3) & ~3;
      while (Index < Count) {
        Data = MmioRead32 (DevBase + DW_MMC_FIFO_START);

        if (Trb->UseBE) {
          *(UINT32 *)((UINTN)Trb->Data + Descending) = SwapBytes32 (Data);
          Descending                                 = Descending - 4;
        } else {
          *(UINT32 *)((UINTN)Trb->Data + Ascending) = Data;
          Ascending                                += 4;
        }

        Index    += 4;
        Received += 4;
      } /* while */

      Timeout = DW_MMC_HC_GENERIC_TIMEOUT;
    } /* if */

Verify:
    if (Intsts & DW_MMC_INT_DATA_ERR) {
      DEBUG ((
        DEBUG_ERROR,
        "%a: Data error. CmdIndex=%d, IntStatus=%p\n",
        __func__,
        Trb->Packet->SdMmcCmdBlk->CommandIndex,
        Intsts
        ));
      return EFI_DEVICE_ERROR;
    }

    if (Timeout < DW_MMC_HC_GENERIC_TIMEOUT) {
      MicroSecondDelay (1);
    }

    if (--Timeout == 0) {
      DEBUG ((
        DEBUG_ERROR,
        "%a: Timed out. CmdIndex=%d, IntStatus=%p\n",
        __func__,
        Trb->Packet->SdMmcCmdBlk->CommandIndex,
        Intsts
        ));
      return EFI_TIMEOUT;
    }
  } while ((Received < Trb->DataLen));

  return EFI_SUCCESS;
}

/**
  Create a new TRB for the SD/MMC cmd request.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.
  @param[in] Slot           The slot number of the SD card to send the command
                            to.
  @param[in] Packet         A pointer to the SD command data structure.
  @param[in] Event          If Event is NULL, blocking I/O is performed. If
                            Event is not NULL, then nonblocking I/O is
                            performed, and Event will be signaled when the
                            Packet completes.

  @return Created Trb or NULL.

**/
DW_MMC_HC_TRB *
DwMmcCreateTrb (
  IN DW_MMC_HC_PRIVATE_DATA               *Private,
  IN UINT8                                Slot,
  IN EFI_SD_MMC_PASS_THRU_COMMAND_PACKET  *Packet,
  IN EFI_EVENT                            Event
  )
{
  DW_MMC_HC_TRB      *Trb;
  EFI_STATUS         Status;
  EFI_TPL            OldTpl;
  DMA_MAP_OPERATION  MapOperation;
  UINTN              MapLength;

  Trb = AllocateZeroPool (sizeof (DW_MMC_HC_TRB));
  if (Trb == NULL) {
    return NULL;
  }

  Trb->Signature = DW_MMC_HC_TRB_SIG;
  Trb->Slot      = Slot;
  Trb->BlockSize = 0x200;
  Trb->Packet    = Packet;
  Trb->Event     = Event;
  Trb->Started   = FALSE;
  Trb->Timeout   = Packet->Timeout;
  Trb->Private   = Private;

  if ((Packet->InTransferLength != 0) && (Packet->InDataBuffer != NULL)) {
    Trb->Data    = Packet->InDataBuffer;
    Trb->DataLen = Packet->InTransferLength;
    Trb->Read    = TRUE;
    ZeroMem (Trb->Data, Trb->DataLen);
  } else if (Packet->OutTransferLength && (Packet->OutDataBuffer != NULL)) {
    Trb->Data    = Packet->OutDataBuffer;
    Trb->DataLen = Packet->OutTransferLength;
    Trb->Read    = FALSE;
  } else if (!Packet->InTransferLength && !Packet->OutTransferLength) {
    Trb->Data    = NULL;
    Trb->DataLen = 0;
  } else {
    goto Error;
  }

  if (((Private->Slot[Trb->Slot].CardType == EmmcCardType) &&
       (Packet->SdMmcCmdBlk->CommandIndex == EMMC_SEND_TUNING_BLOCK)) ||
      ((Private->Slot[Trb->Slot].CardType == SdCardType) &&
       (Packet->SdMmcCmdBlk->CommandIndex == SD_SEND_TUNING_BLOCK)))
  {
    Trb->Mode = SdMmcPioMode;
  } else {
    if (Trb->Read) {
      MapOperation = MapOperationBusMasterWrite;
    } else {
      MapOperation = MapOperationBusMasterRead;
    }

    if (Private->Slot[Trb->Slot].CardType == SdCardType) {
      Trb->UseFifo = TRUE;
    } else {
      Trb->UseFifo = FALSE;
      if (Trb->DataLen) {
        MapLength = Trb->DataLen;
        Status    = DmaMap (MapOperation, Trb->Data, &MapLength, &Trb->DataPhy, &Trb->DataMap);

        /*        Status = DevIo->Map (
                                  DevIo,
                                  Flag,
                                  Trb->Data,
                                  &MapLength,
                                  &Trb->DataPhy,
                                  &Trb->DataMap
                                  );*/
        if (EFI_ERROR (Status) || (Trb->DataLen != MapLength)) {
          Status = EFI_BAD_BUFFER_SIZE;
          goto Error;
        }

        Status = BuildDmaDescTable (Trb);
        if (EFI_ERROR (Status)) {
          DmaUnmap (Trb->DataMap);
          goto Error;
        }

        Status = DwMmcHcStartDma (Private, Trb);
        if (EFI_ERROR (Status)) {
          DmaUnmap (Trb->DataMap);
          goto Error;
        }
      }
    }
  } /* TuningBlock */

  if (Event != NULL) {
    OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
    InsertTailList (&Private->Queue, &Trb->TrbList);
    gBS->RestoreTPL (OldTpl);
  }

  return Trb;

Error:
  return NULL;
}

/**
  Free the resource used by the TRB.

  @param[in] Trb            The pointer to the DW_MMC_HC_TRB instance.

**/
VOID
DwMmcFreeTrb (
  IN DW_MMC_HC_TRB  *Trb
  )
{
  if (Trb->DmaMap != NULL) {
    DmaUnmap (Trb->DmaMap);
  }

  if (Trb->DataMap != NULL) {
    DmaUnmap (Trb->DataMap);
  }

  FreePool (Trb);
}

/**
  Check if the env is ready for execute specified TRB.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.
  @param[in] Trb            The pointer to the DW_MMC_HC_TRB instance.

  @retval EFI_SUCCESS       The env is ready for TRB execution.
  @retval EFI_NOT_READY     The env is not ready for TRB execution.
  @retval Others            Some erros happen.

**/
EFI_STATUS
DwMmcCheckTrbEnv (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  )
{
  return EFI_SUCCESS;
}

/**
  Wait for the env to be ready for execute specified TRB.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.
  @param[in] Trb            The pointer to the DW_MMC_HC_TRB instance.

  @retval EFI_SUCCESS       The env is ready for TRB execution.
  @retval EFI_TIMEOUT       The env is not ready for TRB execution in time.
  @retval Others            Some erros happen.

**/
EFI_STATUS
DwMmcWaitTrbEnv (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  )
{
  EFI_STATUS                           Status;
  EFI_SD_MMC_PASS_THRU_COMMAND_PACKET  *Packet;
  UINT64                               Timeout;
  BOOLEAN                              InfiniteWait;

  //
  // Wait Command Complete Interrupt Status bit in Normal Interrupt Status
  // Register
  //
  Packet  = Trb->Packet;
  Timeout = Packet->Timeout;
  if (Timeout == 0) {
    InfiniteWait = TRUE;
  } else {
    InfiniteWait = FALSE;
  }

  while (InfiniteWait || (Timeout > 0)) {
    //
    // Check Trb execution result by reading Normal Interrupt Status register.
    //
    Status = DwMmcCheckTrbEnv (Private, Trb);
    if (Status != EFI_NOT_READY) {
      return Status;
    }

    //
    // Stall for 1 microsecond.
    //
    gBS->Stall (1);

    Timeout--;
  }

  return EFI_TIMEOUT;
}

EFI_STATUS
DwEmmcExecTrb (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  )
{
  EFI_SD_MMC_PASS_THRU_COMMAND_PACKET  *Packet;
  UINTN                                DevBase;
  UINT32                               Cmd;
  UINT32                               MmcStatus;
  UINT32                               IntStatus;
  UINT32                               Argument;
  UINT32                               ErrMask;
  UINT32                               Timeout;

  Packet  = Trb->Packet;
  DevBase = Trb->Private->DevBase;

  ArmDataSynchronizationBarrier ();
  ArmInstructionSynchronizationBarrier ();
  //
  // Wait until MMC is idle
  //
  do {
    MmcStatus = MmioRead32 (DevBase + DW_MMC_STATUS);
  } while (MmcStatus & DW_MMC_STS_DATA_BUSY);

  IntStatus = ~0;
  MmioWrite32 (DevBase + DW_MMC_RINTSTS, IntStatus);
  Cmd = CMD_INDEX (Packet->SdMmcCmdBlk->CommandIndex);
  if ((Packet->SdMmcCmdBlk->CommandType == SdMmcCommandTypeAc) ||
      (Packet->SdMmcCmdBlk->CommandType == SdMmcCommandTypeAdtc))
  {
    switch (Packet->SdMmcCmdBlk->CommandIndex) {
      case EMMC_SET_RELATIVE_ADDR:
        Cmd |= BIT_CMD_SEND_INIT;
        break;
      case EMMC_SEND_STATUS:
        Cmd |= BIT_CMD_WAIT_PRVDATA_COMPLETE;
        break;
      case EMMC_STOP_TRANSMISSION:
        Cmd |= BIT_CMD_STOP_ABORT_CMD;
        break;
    }

    if (Packet->InTransferLength) {
      Cmd |= BIT_CMD_WAIT_PRVDATA_COMPLETE | BIT_CMD_DATA_EXPECTED |
             BIT_CMD_READ;
    } else if (Packet->OutTransferLength) {
      Cmd |= BIT_CMD_WAIT_PRVDATA_COMPLETE | BIT_CMD_DATA_EXPECTED |
             BIT_CMD_WRITE;
    }

    Cmd |= BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC;
  } else {
    switch (Packet->SdMmcCmdBlk->CommandIndex) {
      case EMMC_GO_IDLE_STATE:
        Cmd |= BIT_CMD_SEND_INIT;
        break;
      case EMMC_SEND_OP_COND:
        Cmd |= BIT_CMD_RESPONSE_EXPECT;
        break;
      case EMMC_ALL_SEND_CID:
        Cmd |= BIT_CMD_RESPONSE_EXPECT | BIT_CMD_LONG_RESPONSE |
               BIT_CMD_CHECK_RESPONSE_CRC | BIT_CMD_SEND_INIT;
        break;
    }
  }

  switch (Packet->SdMmcCmdBlk->ResponseType) {
    case SdMmcResponseTypeR2:
      Cmd |= BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC |
             BIT_CMD_LONG_RESPONSE;
      break;
    case SdMmcResponseTypeR3:
      Cmd |= BIT_CMD_RESPONSE_EXPECT;
      break;
  }

  Cmd |= BIT_CMD_USE_HOLD_REG | BIT_CMD_START;

  Argument = Packet->SdMmcCmdBlk->CommandArgument;
  MmioWrite32 (DevBase + DW_MMC_CMDARG, Argument);

  ArmDataSynchronizationBarrier ();
  ArmInstructionSynchronizationBarrier ();

  MmioWrite32 (DevBase + DW_MMC_CMD, Cmd);
  ArmDataSynchronizationBarrier ();
  ArmInstructionSynchronizationBarrier ();

  ErrMask = DW_MMC_INT_EBE | DW_MMC_INT_HLE | DW_MMC_INT_RTO |
            DW_MMC_INT_RCRC | DW_MMC_INT_RE;
  ErrMask |= DW_MMC_INT_DCRC | DW_MMC_INT_DRT | DW_MMC_INT_SBE;
  do {
    Timeout = 10000;
    if (--Timeout == 0) {
      break;
    }

    IntStatus = MmioRead32 (DevBase + DW_MMC_RINTSTS);
    if (IntStatus & ErrMask) {
      return EFI_DEVICE_ERROR;
    }

    if (Trb->DataLen && ((IntStatus & DW_MMC_INT_DTO) == 0)) {
      //
      // Transfer Not Done
      //
      MicroSecondDelay (10);
      continue;
    }

    MicroSecondDelay (10);
  } while (!(IntStatus & DW_MMC_INT_CMD_DONE));

  switch (Packet->SdMmcCmdBlk->ResponseType) {
    case SdMmcResponseTypeR1:
    case SdMmcResponseTypeR1b:
    case SdMmcResponseTypeR3:
    case SdMmcResponseTypeR4:
    case SdMmcResponseTypeR5:
      Packet->SdMmcStatusBlk->Resp0 = MmioRead32 (DevBase + DW_MMC_RESP0);
      break;
    case SdMmcResponseTypeR2:
      Packet->SdMmcStatusBlk->Resp0 = MmioRead32 (DevBase + DW_MMC_RESP0);
      Packet->SdMmcStatusBlk->Resp1 = MmioRead32 (DevBase + DW_MMC_RESP1);
      Packet->SdMmcStatusBlk->Resp2 = MmioRead32 (DevBase + DW_MMC_RESP2);
      Packet->SdMmcStatusBlk->Resp3 = MmioRead32 (DevBase + DW_MMC_RESP3);
      break;
  }

  //
  // The workaround on EMMC_SEND_CSD is used to be compatible with SDHC.
  //
  if (Packet->SdMmcCmdBlk->CommandIndex == EMMC_SEND_CSD) {
    {
      UINT32  Buf[4];
      ZeroMem (Buf, sizeof (Buf));
      CopyMem (
        (UINT8 *)Buf,
        (UINT8 *)&Packet->SdMmcStatusBlk->Resp0 + 1,
        sizeof (Buf) - 1
        );
      CopyMem (
        (UINT8 *)&Packet->SdMmcStatusBlk->Resp0,
        (UINT8 *)Buf,
        sizeof (Buf) - 1
        );
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
DwSdExecTrb (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  )
{
  EFI_SD_MMC_PASS_THRU_COMMAND_PACKET  *Packet;
  UINTN                                DevBase;
  UINT32                               Cmd;
  UINT32                               MmcStatus;
  UINT32                               IntStatus;
  UINT32                               Argument;
  UINT32                               ErrMask;
  UINT32                               Timeout;
  UINT32                               Idsts;
  UINT32                               BytCnt;
  UINT32                               BlkSize;
  EFI_STATUS                           Status;

  Packet  = Trb->Packet;
  DevBase = Trb->Private->DevBase;

  ArmDataSynchronizationBarrier ();
  ArmInstructionSynchronizationBarrier ();
  //
  // Wait until MMC is idle
  //
  do {
    MmcStatus = MmioRead32 (DevBase + DW_MMC_STATUS);
  } while (MmcStatus & DW_MMC_STS_DATA_BUSY);

  IntStatus = ~0;
  MmioWrite32 (DevBase + DW_MMC_RINTSTS, IntStatus);
  Cmd = CMD_INDEX (Packet->SdMmcCmdBlk->CommandIndex);
  if ((Packet->SdMmcCmdBlk->CommandType == SdMmcCommandTypeAc) ||
      (Packet->SdMmcCmdBlk->CommandType == SdMmcCommandTypeAdtc))
  {
    switch (Packet->SdMmcCmdBlk->CommandIndex) {
      case SD_SET_RELATIVE_ADDR:
        Cmd |= BIT_CMD_SEND_INIT;
        break;
      case SD_STOP_TRANSMISSION:
        Cmd |= BIT_CMD_STOP_ABORT_CMD;
        break;
      case SD_SEND_SCR:
        Trb->UseBE = TRUE;
        break;
    }

    if (Packet->InTransferLength) {
      Cmd |= BIT_CMD_WAIT_PRVDATA_COMPLETE | BIT_CMD_DATA_EXPECTED |
             BIT_CMD_READ;
    } else if (Packet->OutTransferLength) {
      Cmd |= BIT_CMD_WAIT_PRVDATA_COMPLETE | BIT_CMD_DATA_EXPECTED |
             BIT_CMD_WRITE;
    }

    Cmd |= BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC |
           BIT_CMD_SEND_AUTO_STOP;
  } else {
    switch (Packet->SdMmcCmdBlk->CommandIndex) {
      case SD_GO_IDLE_STATE:
        Cmd |= BIT_CMD_SEND_INIT;
        break;
    }
  }

  switch (Packet->SdMmcCmdBlk->ResponseType) {
    case SdMmcResponseTypeR2:
      Cmd |= BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC |
             BIT_CMD_LONG_RESPONSE;
      break;
    case SdMmcResponseTypeR3:
      Cmd |= BIT_CMD_RESPONSE_EXPECT;
      break;
    case SdMmcResponseTypeR1b:
    case SdMmcResponseTypeR4:
    case SdMmcResponseTypeR6:
    case SdMmcResponseTypeR7:
      Cmd |= BIT_CMD_RESPONSE_EXPECT | BIT_CMD_CHECK_RESPONSE_CRC;
      break;
  }

  Cmd |= BIT_CMD_USE_HOLD_REG | BIT_CMD_START;

  if (Trb->UseFifo == TRUE) {
    BytCnt = Trb->Read ? Packet->InTransferLength : Packet->OutTransferLength;
    MmioWrite32 (DevBase + DW_MMC_BYTCNT, BytCnt);
    if (Trb->Read) {
      if (Packet->InTransferLength > DW_MMC_BLOCK_SIZE) {
        BlkSize = DW_MMC_BLOCK_SIZE;
      } else {
        BlkSize = Packet->InTransferLength;
      }
    } else {
      if (Packet->OutTransferLength > DW_MMC_BLOCK_SIZE) {
        BlkSize = DW_MMC_BLOCK_SIZE;
      } else {
        BlkSize = Packet->OutTransferLength;
      }
    }

    MmioWrite32 (DevBase + DW_MMC_BLKSIZ, BlkSize);

    if (Trb->DataLen) {
      Status = DwMmcHcWaitReset (DevBase, DW_MMC_CTRL_FIFO_RESET);
      if (EFI_ERROR (Status)) {
        DEBUG ((
          DEBUG_ERROR,
          "%a: FIFO reset timed out. CmdIndex=%d\n",
          __func__,
          Packet->SdMmcCmdBlk->CommandIndex
          ));
      }
    }
  }

  Argument = Packet->SdMmcCmdBlk->CommandArgument;
  MmioWrite32 (DevBase + DW_MMC_CMDARG, Argument);
  ArmDataSynchronizationBarrier ();
  ArmInstructionSynchronizationBarrier ();
  MmioWrite32 (DevBase + DW_MMC_CMD, Cmd);
  ArmDataSynchronizationBarrier ();
  ArmInstructionSynchronizationBarrier ();

  ErrMask = DW_MMC_INT_EBE | DW_MMC_INT_HLE | DW_MMC_INT_RTO |
            DW_MMC_INT_RCRC | DW_MMC_INT_RE;
  ErrMask |= DW_MMC_INT_DRT | DW_MMC_INT_SBE;
  if (Packet->InTransferLength || Packet->OutTransferLength) {
    ErrMask |= DW_MMC_INT_DCRC;
  }

  Timeout = 10000;
  do {
    if (--Timeout == 0) {
      break;
    }

    IntStatus = MmioRead32 (DevBase + DW_MMC_RINTSTS);
    MicroSecondDelay (1);
  } while (!(IntStatus & DW_MMC_INT_CMD_DONE));

  if (IntStatus & ErrMask) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Command error. CmdIndex=%d, IntStatus=%p\n",
      __func__,
      Packet->SdMmcCmdBlk->CommandIndex,
      IntStatus
      ));
    return EFI_DEVICE_ERROR;
  }

  if (Trb->DataLen) {
    if (Trb->UseFifo == TRUE) {
      Status = TransferFifo (Trb);
      if (EFI_ERROR (Status)) {
        return Status;
      }
    } else {
      if (Packet->InTransferLength) {
        do {
          Idsts = MmioRead32 (DevBase + DW_MMC_IDSTS);
        } while ((Idsts & DW_MMC_IDSTS_RI) == 0);

        Status = DwMmcHcStopDma (Private, Trb);
        if (EFI_ERROR (Status)) {
          return Status;
        }
      } else if (Packet->OutTransferLength) {
        do {
          Idsts = MmioRead32 (DevBase + DW_MMC_IDSTS);
        } while ((Idsts & DW_MMC_IDSTS_TI) == 0);

        Status = DwMmcHcStopDma (Private, Trb);
        if (EFI_ERROR (Status)) {
          return Status;
        }
      } /* Packet->InTransferLength */
    } /* UseFifo */
  } /* Trb->DataLen */

  switch (Packet->SdMmcCmdBlk->ResponseType) {
    case SdMmcResponseTypeR1:
    case SdMmcResponseTypeR1b:
    case SdMmcResponseTypeR3:
    case SdMmcResponseTypeR4:
    case SdMmcResponseTypeR5:
    case SdMmcResponseTypeR6:
    case SdMmcResponseTypeR7:
      Packet->SdMmcStatusBlk->Resp0 = MmioRead32 (DevBase + DW_MMC_RESP0);
      break;
    case SdMmcResponseTypeR2:
      Packet->SdMmcStatusBlk->Resp0 = MmioRead32 (DevBase + DW_MMC_RESP0);
      Packet->SdMmcStatusBlk->Resp1 = MmioRead32 (DevBase + DW_MMC_RESP1);
      Packet->SdMmcStatusBlk->Resp2 = MmioRead32 (DevBase + DW_MMC_RESP2);
      Packet->SdMmcStatusBlk->Resp3 = MmioRead32 (DevBase + DW_MMC_RESP3);
      break;
  }

  //
  // The workaround on SD_SEND_CSD/CID is used to be compatible with SDHC.
  //
  if (  (Packet->SdMmcCmdBlk->CommandIndex == SD_SEND_CSD)
     || (Packet->SdMmcCmdBlk->CommandIndex == SD_SEND_CID))
  {
    {
      UINT32  Buf[4];
      ZeroMem (Buf, sizeof (Buf));
      CopyMem (
        (UINT8 *)Buf,
        (UINT8 *)&Packet->SdMmcStatusBlk->Resp0 + 1,
        sizeof (Buf) - 1
        );
      CopyMem (
        (UINT8 *)&Packet->SdMmcStatusBlk->Resp0,
        (UINT8 *)Buf,
        sizeof (Buf) - 1
        );
    }
  }

  return EFI_SUCCESS;
}

/**
  Execute the specified TRB.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.
  @param[in] Trb            The pointer to the DW_MMC_HC_TRB instance.

  @retval EFI_SUCCESS       The TRB is sent to host controller successfully.
  @retval Others            Some erros happen when sending this request to the
                            host controller.

**/
EFI_STATUS
DwMmcExecTrb (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  )
{
  EFI_STATUS  Status = EFI_SUCCESS;
  UINT32      Slot;

  Slot = Trb->Slot;
  if (Private->Slot[Slot].CardType == EmmcCardType) {
    Status = DwEmmcExecTrb (Private, Trb);
  } else if (Private->Slot[Slot].CardType == SdCardType) {
    Status = DwSdExecTrb (Private, Trb);
  } else {
    ASSERT (0);
  }

  return Status;
}

/**
  Check the TRB execution result.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.
  @param[in] Trb            The pointer to the DW_MMC_HC_TRB instance.

  @retval EFI_SUCCESS       The TRB is executed successfully.
  @retval EFI_NOT_READY     The TRB is not completed for execution.
  @retval Others            Some erros happen when executing this request.

**/
EFI_STATUS
DwMmcCheckTrbResult (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  )
{
  EFI_SD_MMC_PASS_THRU_COMMAND_PACKET  *Packet;
  UINT32                               Idsts;
  UINTN                                DevBase;
  UINT32                               IntStatus;

  DevBase = Private->DevBase;
  Packet  = Trb->Packet;
  if (Trb->UseFifo == TRUE) {
    if (Trb->DataLen) {
      IntStatus = MmioRead32 (DevBase + DW_MMC_RINTSTS);
      //
      // Check Auto CMD12 completion
      //
      if (!(IntStatus & DW_MMC_INT_ACD)) {
        return EFI_NOT_READY;
      }

      //
      // Check data trans over
      //
      if (!(IntStatus & DW_MMC_INT_DTO)) {
        return EFI_NOT_READY;
      }

      //
      // There will be some errors reported (SBE, HTO, DRT, DCRC, RCRC)
      // depending on the command sent (read/write, single/multi block).
      //
      // Not sure why this happens (do we need to manually stop the command?),
      // but it does not seem to affect operation in any way.
      // All data is correctly transferred and the FIFO is empty by the time
      // we reach this code path.
      //
      // Each transfer is validated in TransferFifo(), which would fail on any
      // of the error states above and more.
      // Interrupt status is cleared before a new command is issued, no need
      // to do it here.
      //
    }

    return EFI_SUCCESS;
  }

  if (Packet->InTransferLength) {
    do {
      Idsts = MmioRead32 (DevBase + DW_MMC_IDSTS);
    } while ((Idsts & BIT1) == 0);
  } else if (Packet->OutTransferLength) {
    do {
      Idsts = MmioRead32 (DevBase + DW_MMC_IDSTS);
    } while ((Idsts & BIT0) == 0);
  } else {
    return EFI_SUCCESS;
  }

  Idsts = ~0;
  MmioWrite32 (DevBase + DW_MMC_IDSTS, Idsts);

  return EFI_SUCCESS;
}

/**
  Wait for the TRB execution result.

  @param[in] Private        A pointer to the DW_MMC_HC_PRIVATE_DATA instance.
  @param[in] Trb            The pointer to the DW_MMC_HC_TRB instance.

  @retval EFI_SUCCESS       The TRB is executed successfully.
  @retval Others            Some erros happen when executing this request.

**/
EFI_STATUS
DwMmcWaitTrbResult (
  IN DW_MMC_HC_PRIVATE_DATA  *Private,
  IN DW_MMC_HC_TRB           *Trb
  )
{
  EFI_STATUS                           Status;
  EFI_SD_MMC_PASS_THRU_COMMAND_PACKET  *Packet;
  UINT64                               Timeout;
  BOOLEAN                              InfiniteWait;

  Packet = Trb->Packet;
  //
  // Wait Command Complete Interrupt Status bit in Normal Interrupt Status
  // Register
  //
  Timeout = Packet->Timeout;
  if (Timeout == 0) {
    InfiniteWait = TRUE;
  } else {
    InfiniteWait = FALSE;
  }

  while (InfiniteWait || (Timeout > 0)) {
    //
    // Check Trb execution result by reading Normal Interrupt Status register.
    //
    Status = DwMmcCheckTrbResult (Private, Trb);
    if (Status != EFI_NOT_READY) {
      return Status;
    }

    //
    // Stall for 1 microsecond.
    //
    gBS->Stall (1);

    Timeout--;
  }

  return EFI_TIMEOUT;
}
