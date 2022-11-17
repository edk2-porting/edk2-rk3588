/** @file
  Pch SATA library for CedarFork SouthCluster.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PchInfoLib.h>
#include <PchLimits.h>
#include <Register/PchRegsSata.h>
#include <Library/SataLib.h>

/**
  Get Maximum Sata Controller Number

  @param[in] None

  @retval Maximum Sata Controller Number
**/
UINT8
GetPchMaxSataControllerNum (
  VOID
  )
{
  return 3;
}

/**
  Get Pch Maximum Sata Port Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval Pch Maximum Sata Port Number
**/
UINT8
GetPchMaxSataPortNum (
  IN UINT8      SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < GetPchMaxSataControllerNum ());

  return 8;
}

/**
  Get SATA controller PCIe Device Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Device Number
**/
UINT8
GetSataPcieDeviceNum (
  IN UINT8  SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < GetPchMaxSataControllerNum ());

  if (SataCtrlIndex == SATA_1_CONTROLLER_INDEX) {
    return PCI_DEVICE_NUMBER_CDF_PCH_SATA_1;
  } else if (SataCtrlIndex == SATA_2_CONTROLLER_INDEX) {
    return PCI_DEVICE_NUMBER_CDF_PCH_SATA_2;
  } else if (SataCtrlIndex == SATA_3_CONTROLLER_INDEX) {
    return PCI_DEVICE_NUMBER_CDF_PCH_SATA_3;
  } else {
    ASSERT (FALSE);
    return 0;
  }
}

/**
  Get SATA controller PCIe Function Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Function Number
**/
UINT8
GetSataPcieFunctionNum (
  IN UINT8  SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < GetPchMaxSataControllerNum ());

  if (SataCtrlIndex == SATA_1_CONTROLLER_INDEX) {
    return PCI_FUNCTION_NUMBER_CDF_PCH_SATA_1;
  } else if (SataCtrlIndex == SATA_2_CONTROLLER_INDEX) {
    return PCI_FUNCTION_NUMBER_CDF_PCH_SATA_2;
  } else if (SataCtrlIndex == SATA_3_CONTROLLER_INDEX) {
    return PCI_FUNCTION_NUMBER_CDF_PCH_SATA_3;
  } else {
    ASSERT (FALSE);
    return 0;
  }
}

