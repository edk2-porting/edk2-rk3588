/** @file
*
*  Copyright 2018-2020 NXP
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/ArmLib.h>
#include <Library/ArmPlatformLib.h>
#include <Library/SocLib.h>

#include <Ppi/ArmMpCoreInfo.h>
#include <Ppi/NxpPlatformGetClock.h>

ARM_CORE_INFO mLX2160aMpCoreInfoTable[] = {
  {
    // Cluster 0, Core 0
    0x0, 0x0,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  }
};

/**
  Return the current Boot Mode

  This function returns the boot reason on the platform

**/
EFI_BOOT_MODE
ArmPlatformGetBootMode (
  VOID
  )
{
  return BOOT_WITH_FULL_CONFIGURATION;
}

/**
  Get the clocks supplied by Platform(Board) to NXP Layerscape SOC IPs

  @param[in]  ClockType  Variable of Type NXP_IP_CLOCK. Indicates which IP clock
                         is to be retrieved.
  @param[in]  ...        Variable argument list which is parsed based on
                         ClockType. e.g. if the ClockType is NXP_I2C_CLOCK, then
                         the second argument will be interpreted as controller
                         number.
                         if ClockType is NXP_CORE_CLOCK, then second argument
                         is interpreted as cluster number and third argument is
                         interpreted as core number (within the cluster)

  @return                Actual Clock Frequency. Return value 0 should be
                         interpreted as clock not being provided to IP.
**/
UINT64
EFIAPI
NxpPlatformGetClock(
  IN  UINT32  ClockType,
  ...
  )
{
  UINT64      Clock;
  VA_LIST     Args;

  Clock = 0;

  VA_START (Args, ClockType);

  switch (ClockType) {
  case NXP_SYSTEM_CLOCK:
    Clock = 100 * 1000 * 1000; // 100 MHz
    break;
  case NXP_I2C_CLOCK:
  case NXP_UART_CLOCK:
    Clock = NxpPlatformGetClock (NXP_SYSTEM_CLOCK);
    Clock = SocGetClock (Clock, ClockType, Args);
    break;
  default:
    break;
  }

  VA_END (Args);

  return Clock;
}

/**
  Initialize controllers that must setup in the normal world

  This function is called by the ArmPlatformPkg/PrePi or ArmPlatformPkg/PlatformPei
  in the PEI phase.

**/
EFI_STATUS
ArmPlatformInitialize (
  IN  UINTN                     MpId
  )
{
  SocInit ();

  return EFI_SUCCESS;
}

EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN                   *CoreCount,
  OUT ARM_CORE_INFO           **ArmCoreTable
  )
{
  if (ArmIsMpCore()) {
    *CoreCount    = sizeof(mLX2160aMpCoreInfoTable) / sizeof(ARM_CORE_INFO);
    *ArmCoreTable = mLX2160aMpCoreInfoTable;
    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }
}

ARM_MP_CORE_INFO_PPI mMpCoreInfoPpi = { PrePeiCoreGetMpCoreInfo };
NXP_PLATFORM_GET_CLOCK_PPI gPlatformGetClockPpi = { NxpPlatformGetClock };

EFI_PEI_PPI_DESCRIPTOR      gPlatformPpiTable[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gArmMpCoreInfoPpiGuid,
    &mMpCoreInfoPpi
  }
};

VOID
ArmPlatformGetPlatformPpiList (
  OUT UINTN                   *PpiListSize,
  OUT EFI_PEI_PPI_DESCRIPTOR  **PpiList
  )
{
  if (ArmIsMpCore()) {
    *PpiListSize = sizeof(gPlatformPpiTable);
    *PpiList = gPlatformPpiTable;
  } else {
    *PpiListSize = 0;
    *PpiList = NULL;
  }
}
