/** @file
  Pch SATA library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
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
  return 1;
}

/**
  Get Pch Maximum Sata Port Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval Pch Maximum Sata Port Number
**/
UINT8
GetPchMaxSataPortNum (
  IN UINT32      SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < GetPchMaxSataControllerNum ());

  if (IsPchLp ()) {
    return 3;
  } else {
    return 8;
  }
}

/**
  Get SATA controller PCIe Device Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Device Number
**/
UINT8
GetSataPcieDeviceNum (
  IN UINT32  SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < GetPchMaxSataControllerNum ());

  return PCI_DEVICE_NUMBER_PCH_SATA;
}

/**
  Get SATA controller PCIe Function Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Function Number
**/
UINT8
GetSataPcieFunctionNum (
  IN UINT32  SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < GetPchMaxSataControllerNum ());

  return PCI_FUNCTION_NUMBER_PCH_SATA;
}

