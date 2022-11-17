/** @file
  Pch SATA library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SataLib.h>
#include <Register/PchRegs.h>
/**
  Get SATA controller address that can be passed to the PCI Segment Library functions.

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller address in PCI Segment Library representation
**/
UINT64
GetSataRegBase (
  IN UINT32  SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < GetPchMaxSataControllerNum ());

  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           GetSataPcieDeviceNum (SataCtrlIndex),
           GetSataPcieFunctionNum (SataCtrlIndex),
           0
           );
}
