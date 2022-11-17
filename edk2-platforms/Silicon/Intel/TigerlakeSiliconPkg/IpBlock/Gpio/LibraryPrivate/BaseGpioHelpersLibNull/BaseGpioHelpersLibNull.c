/** @file
  This file contains NULL implementation for GPIO Helpers Lib

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Uefi/UefiBaseType.h>
#include <Library/GpioConfig.h>

/**
  This procedure stores GPIO pad unlock information

  @param[in] GpioPad         GPIO pad
  @param[in] GpioLockConfig  GPIO Lock Configuration

  @retval Status
**/
EFI_STATUS
GpioStoreUnlockData (
  IN GPIO_PAD             GpioPad,
  IN GPIO_LOCK_CONFIG     GpioLockConfig
  )
{
  return EFI_SUCCESS;
}

/**
  This procedure stores GPIO group data about pads which PadConfig needs to be unlocked.

  @param[in]  GroupIndex          GPIO group index
  @param[in]  DwNum               DWORD index for a group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  UnlockedPads        DWORD bitmask for pads which are going to be left unlocked
                                  Bit position - PadNumber
                                  Bit value - 0: Skip, 1: Leave unlocked

  @retval Status
**/
EFI_STATUS
GpioStoreGroupDwUnlockPadConfigData (
  IN UINT32                       GroupIndex,
  IN UINT32                       DwNum,
  IN UINT32                       UnlockedPads
  )
{
  return EFI_SUCCESS;
}

/**
  This procedure stores GPIO group data about pads which Output state needs to be unlocked.

  @param[in]  GroupIndex          GPIO group index
  @param[in]  DwNum               DWORD index for a group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @param[in]  UnlockedPads        DWORD bitmask for pads which are going to be left unlocked
                                  Bit position - PadNumber
                                  Bit value - 0: Skip, 1: Leave unlocked
  @retval Status
**/
EFI_STATUS
GpioStoreGroupDwUnlockOutputData (
  IN UINT32                       GroupIndex,
  IN UINT32                       DwNum,
  IN UINT32                       UnlockedPads
  )
{
  return EFI_SUCCESS;
}

/**
  This procedure will get GPIO group data with pads, which PadConfig is supposed to be left unlock

  @param[in]  GroupIndex          GPIO group index
  @param[in]  DwNum               DWORD index for a group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @retval     UnlockedPads        DWORD bitmask for pads which are going to be left unlocked
                                  Bit position - PadNumber
                                  Bit value - 0: to be locked, 1: Leave unlocked
**/
UINT32
GpioGetGroupDwUnlockPadConfigMask (
  IN UINT32                       GroupIndex,
  IN UINT32                       DwNum
  )
{
  return 0;
}

/**
  This procedure will get GPIO group data with pads, which Output is supposed to be left unlock

  @param[in]  GroupIndex          GPIO group index
  @param[in]  DwNum               DWORD index for a group.
                                  For group which has less then 32 pads per group DwNum must be 0.
  @retval     UnlockedPads        DWORD bitmask for pads which are going to be left unlocked
                                  Bit position - PadNumber
                                  Bit value - 0: to be locked, 1: Leave unlocked
**/
UINT32
GpioGetGroupDwUnlockOutputMask (
  IN UINT32                       GroupIndex,
  IN UINT32                       DwNum
  )
{
  return 0;
}

/**
  Returns Gpio Override Level1 Information

  @retval  TRUE/FALSE GPIO Override Level 1 Enabled/Disabled
**/
BOOLEAN
GpioOverrideLevel1Enabled (
  VOID
  )
{
  return FALSE;
}
