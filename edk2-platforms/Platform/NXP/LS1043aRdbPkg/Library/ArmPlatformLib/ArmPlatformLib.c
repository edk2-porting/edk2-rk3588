/** ArmPlatformLib.c
*
*  Contains board initialization functions.
*
*  Based on BeagleBoardPkg/Library/BeagleBoardLib/BeagleBoard.c
*
*  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
*  Copyright (c) 2016, Freescale Semiconductor, Inc. All rights reserved.
*  Copyright 2017, 2020 NXP
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Library/ArmPlatformLib.h>
#include <Library/SocLib.h>
#include <Ppi/ArmMpCoreInfo.h>
#include <Ppi/NxpPlatformGetClock.h>

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
 Placeholder for Platform Initialization
**/
EFI_STATUS
ArmPlatformInitialize (
  IN  UINTN   MpId
  )
{
 SocInit ();

 return EFI_SUCCESS;
}

ARM_CORE_INFO LS1043aMpCoreInfoCTA53x4[] = {
  {
    // Cluster 0, Core 0
    0x0, 0x0,

    // MP Core MailBox Set/Get/Clear Addresses and Clear Value
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (EFI_PHYSICAL_ADDRESS)0,
    (UINT64)0xFFFFFFFF
  },
};

EFI_STATUS
PrePeiCoreGetMpCoreInfo (
  OUT UINTN                   *CoreCount,
  OUT ARM_CORE_INFO           **ArmCoreTable
  )
{
  *CoreCount    = sizeof (LS1043aMpCoreInfoCTA53x4) / sizeof (ARM_CORE_INFO);
  *ArmCoreTable = LS1043aMpCoreInfoCTA53x4;

  return EFI_SUCCESS;
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
  *PpiListSize = sizeof (gPlatformPpiTable);
  *PpiList = gPlatformPpiTable;
}

