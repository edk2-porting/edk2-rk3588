/**
*
*  Copyright (C) 2021, Semihalf.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>

#include <Library/ArmadaBoardDescLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MvGpioLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "BoardDescriptionLib.h"

STATIC
VOID
ConfigureIoWindow (
  UINT64 WinBaseAddress,
  UINT64 WinSize,
  UINTN  WinId,
  UINT32 WinTargetId
  )
{
  UINT32 AddressHigh;
  UINT32 AddressLow;
  UINT64 MaxAddress;

  /* Disable IO window. */
  MmioWrite32 (IO_WIN_ALR_OFFSET(WinId), 0);

  /* Calculate the end address. */
  MaxAddress = (WinBaseAddress + WinSize - 1);

  AddressLow = (UINT32)((WinBaseAddress >> IO_WIN_ADDRESS_SHIFT) & IO_WIN_ADDRESS_MASK);
  AddressLow |= IO_WIN_ENABLE_BIT;
  AddressHigh = (UINT32)((MaxAddress >> IO_WIN_ADDRESS_SHIFT) & IO_WIN_ADDRESS_MASK);

  /* Write start address and end address for IO window. */
  MmioWrite32 (IO_WIN_ALR_OFFSET(WinId), AddressLow);
  MmioWrite32 (IO_WIN_AHR_OFFSET(WinId), AddressHigh);

  /* Write window target. */
  MmioWrite32 (IO_WIN_CR_OFFSET(WinId), WinTargetId);
}

//
// General purpose routine for per-board initalization
//
EFI_STATUS
ArmadaBoardInit (
  VOID
  )
{
  /*
   * Due to lack of sufficient number of IO windows registers,
   * the CP1/CP2 PCIE configuration must be performed after the
   * early firmware stages. Replace the MCI 0/1 indirect
   * windows, which are no longer needed.
   */
  ConfigureIoWindow (
    CP1_PCIE_WIN64_BASE,
    CP1_PCIE_WIN64_SIZE,
    CP1_PCIE_WIN64_ID,
    MCI0_TARGET_ID
    );

  ConfigureIoWindow (
    CP2_PCIE_WIN64_BASE,
    CP2_PCIE_WIN64_SIZE,
    CP2_PCIE_WIN64_ID,
    MCI1_TARGET_ID
    );

  /* Enable FAN */
  MmioAnd32 (CP0_GPIO1_DATA_OUT_REG, ~CP0_GPIO1_PIN_MASK);
  MmioAnd32 (CP0_GPIO1_OUT_EN_REG, ~CP0_GPIO1_PIN_MASK);

  return EFI_SUCCESS;
}

//
// GPIO Expander
//
EFI_STATUS
EFIAPI
ArmadaBoardGpioExpanderGet (
  IN OUT MV_GPIO_EXPANDER **GpioExpanders,
  IN OUT UINTN             *GpioExpanderCount
  )
{
  /* No GPIO expanders on board */
  *GpioExpanders = NULL;
  *GpioExpanderCount = 0;

  return EFI_SUCCESS;
}

//
// PCIE
//
STATIC
MV_PCIE_CONTROLLER mPcieController[] = {
  { /* CP0 PCIE0 @0xF2600000 */
    .PcieDbiAddress        = 0xF2600000,
    .ConfigSpaceAddress    = 0x800000000,
    .HaveResetGpio         = FALSE,
    .PcieResetGpio         = { 0 },
    .PcieBusMin            = 0,
    .PcieBusMax            = 0xFE,
    .PcieIoTranslation     = 0x80FF00000,
    .PcieIoWinBase         = 0x0,
    .PcieIoWinSize         = 0x10000,
    .PcieMmio32Translation = 0,
    .PcieMmio32WinBase     = 0xC0000000,
    .PcieMmio32WinSize     = 0x20000000,
    .PcieMmio64Translation = 0,
    .PcieMmio64WinBase     = 0x810000000,
    .PcieMmio64WinSize     = 0x80000000,
  },
  { /* CP1 PCIE0 @0xF4600000 */
    .PcieDbiAddress        = 0xF4600000,
    .ConfigSpaceAddress    = 0xE2000000,
    .HaveResetGpio         = FALSE,
    .PcieResetGpio         = { 0 },
    .PcieBusMin            = 0,
    .PcieBusMax            = 0xE,
    .PcieIoTranslation     = 0xE2F00000,
    .PcieIoWinBase         = 0x0,
    .PcieIoWinSize         = 0x10000,
    .PcieMmio32Translation = 0,
    .PcieMmio32WinBase     = 0xE3000000,
    .PcieMmio32WinSize     = 0x1000000,
    .PcieMmio64Translation = 0,
    .PcieMmio64WinBase     = 0x890000000,
    .PcieMmio64WinSize     = 0x10000000,
  },
  { /* CP1 PCIE1 @0xF4620000 */
    .PcieDbiAddress        = 0xF4620000,
    .ConfigSpaceAddress    = 0xE4000000,
    .HaveResetGpio         = FALSE,
    .PcieResetGpio         = { 0 },
    .PcieBusMin            = 0,
    .PcieBusMax            = 0xE,
    .PcieIoTranslation     = 0xE4F00000,
    .PcieIoWinBase         = 0x0,
    .PcieIoWinSize         = 0x10000,
    .PcieMmio32Translation = 0,
    .PcieMmio32WinBase     = 0xE5000000,
    .PcieMmio32WinSize     = 0x1000000,
    .PcieMmio64Translation = 0,
    .PcieMmio64WinBase     = 0x8A0000000,
    .PcieMmio64WinSize     = 0x10000000,
  },
  { /* CP1 PCIE2 @0xF4640000 */
    .PcieDbiAddress        = 0xF4640000,
    .ConfigSpaceAddress    = 0xE6000000,
    .HaveResetGpio         = FALSE,
    .PcieResetGpio         = { 0 },
    .PcieBusMin            = 0,
    .PcieBusMax            = 0xE,
    .PcieIoTranslation     = 0xE6F00000,
    .PcieIoWinBase         = 0x0,
    .PcieIoWinSize         = 0x10000,
    .PcieMmio32Translation = 0,
    .PcieMmio32WinBase     = 0xE7000000,
    .PcieMmio32WinSize     = 0x1000000,
    .PcieMmio64Translation = 0,
    .PcieMmio64WinBase     = 0x8B0000000,
    .PcieMmio64WinSize     = 0x10000000,
  },
  { /* CP2 PCIE0 @0xF6600000 */
    .PcieDbiAddress        = 0xF6600000,
    .ConfigSpaceAddress    = 0xE9000000,
    .HaveResetGpio         = FALSE,
    .PcieResetGpio         = { 0 },
    .PcieBusMin            = 0,
    .PcieBusMax            = 0xE,
    .PcieIoTranslation     = 0xE9F00000,
    .PcieIoWinBase         = 0x0,
    .PcieIoWinSize         = 0x10000,
    .PcieMmio32Translation = 0,
    .PcieMmio32WinBase     = 0xEA000000,
    .PcieMmio32WinSize     = 0x1000000,
    .PcieMmio64Translation = 0,
    .PcieMmio64WinBase     = 0x8C0000000,
    .PcieMmio64WinSize     = 0x10000000,
  },
  { /* CP2 PCIE1 @0xF6620000 */
    .PcieDbiAddress        = 0xF6620000,
    .ConfigSpaceAddress    = 0xEB000000,
    .HaveResetGpio         = FALSE,
    .PcieResetGpio         = { 0 },
    .PcieBusMin            = 0,
    .PcieBusMax            = 0xE,
    .PcieIoTranslation     = 0xEBF00000,
    .PcieIoWinBase         = 0x0,
    .PcieIoWinSize         = 0x10000,
    .PcieMmio32Translation = 0,
    .PcieMmio32WinBase     = 0xEC000000,
    .PcieMmio32WinSize     = 0x1000000,
    .PcieMmio64Translation = 0,
    .PcieMmio64WinBase     = 0x8D0000000,
    .PcieMmio64WinSize     = 0x10000000,
  },
  { /* CP2 PCIE2 @0xF6640000 */
    .PcieDbiAddress        = 0xF6640000,
    .ConfigSpaceAddress    = 0xED000000,
    .HaveResetGpio         = FALSE,
    .PcieResetGpio         = { 0 },
    .PcieBusMin            = 0,
    .PcieBusMax            = 0xE,
    .PcieIoTranslation     = 0xEDF00000,
    .PcieIoWinBase         = 0x0,
    .PcieIoWinSize         = 0x10000,
    .PcieMmio32Translation = 0,
    .PcieMmio32WinBase     = 0xEE000000,
    .PcieMmio32WinSize     = 0x1000000,
    .PcieMmio64Translation = 0,
    .PcieMmio64WinBase     = 0x8E0000000,
    .PcieMmio64WinSize     = 0x10000000,
  },
};

/**
  Return the number and description of PCIE controllers used on the platform.

  @param[in out] **PcieControllers      Array containing PCIE controllers'
                                        description.
  @param[in out]  *PcieControllerCount  Amount of used PCIE controllers.

  @retval EFI_SUCCESS                   The data were obtained successfully.
  @retval other                         Return error status.

**/
EFI_STATUS
EFIAPI
ArmadaBoardPcieControllerGet (
  IN OUT MV_PCIE_CONTROLLER CONST **PcieControllers,
  IN OUT UINTN                     *PcieControllerCount
  )
{
  *PcieControllers = mPcieController;
  *PcieControllerCount = ARRAY_SIZE (mPcieController);

  return EFI_SUCCESS;
}

//
// Order of devices in SdMmcDescTemplate has to be in par with ArmadaSoCDescLib
//
STATIC
MV_BOARD_SDMMC_DESC mSdMmcDescTemplate[] = {
  { /* eMMC 0xF06E0000 */
    0,     /* SOC will be filled by MvBoardDescDxe */
    0,     /* SdMmcDevCount will be filled by MvBoardDescDxe */
    TRUE,  /* Xenon1v8Enabled */
    /*
     * Force 4-bit bus width - work-around for non
     * functional HS400 mode.
     */
    FALSE, /* Xenon8BitBusEnabled */
    FALSE, /* XenonSlowModeEnabled */
    0x40,  /* XenonTuningStepDivisor */
    EmbeddedSlot /* SlotType */
  },
  { /* SD/MMC 0xF2780000 */
    0,     /* SOC will be filled by MvBoardDescDxe */
    0,     /* SdMmcDevCount will be filled by MvBoardDescDxe */
    FALSE, /* Xenon1v8Enabled */
    FALSE, /* Xenon8BitBusEnabled */
    FALSE, /* XenonSlowModeEnabled */
    0x19,  /* XenonTuningStepDivisor */
    EmbeddedSlot /* SlotType */
  },
};

EFI_STATUS
EFIAPI
ArmadaBoardDescSdMmcGet (
  OUT UINTN               *SdMmcDevCount,
  OUT MV_BOARD_SDMMC_DESC **SdMmcDesc
  )
{
  *SdMmcDesc = mSdMmcDescTemplate;
  *SdMmcDevCount = ARRAY_SIZE (mSdMmcDescTemplate);

  return EFI_SUCCESS;
}
