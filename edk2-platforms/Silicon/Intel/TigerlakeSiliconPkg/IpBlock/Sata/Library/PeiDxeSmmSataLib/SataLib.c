/** @file
  Pch SATA library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
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
#include <Register/SataRegs.h>
#include <Library/PchPciBdfLib.h>

/**
  Get SATA controller address that can be passed to the PCI Segment Library functions.

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller address in PCI Segment Library representation
**/
UINT64
SataRegBase (
  IN UINT32      SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < MaxSataControllerNum ());

  return SataPciCfgBase (SataCtrlIndex);
}

/**
  Get SATA controller's Port Present Status

  @param[in]  SataCtrlIndex       SATA controller index

  @retval     Port Present Status
**/
UINT8
GetSataPortPresentStatus (
  IN UINT32  SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < MaxSataControllerNum ());

  return PciSegmentRead8 (SataPciCfgBase (SataCtrlIndex) + R_SATA_CFG_PCS + 2);
}

/**
  Get SATA controller Function Disable Status

  @param[in]  SataCtrlIndex       SATA controller index

  @retval 0 SATA Controller is not Function Disabled
  @retval 1 SATA Controller is Function Disabled
**/
BOOLEAN
SataControllerFunctionDisableStatus (
  IN UINT32  SataCtrlIndex
  )
{
  UINT32 SataGc;
  ASSERT (SataCtrlIndex < MaxSataControllerNum ());
  SataGc = PciSegmentRead32 (SataPciCfgBase (SataCtrlIndex) + R_SATA_CFG_SATAGC);
  return !!(SataGc & BIT10);
}

/**
  Get SATA controller ABAR size

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller ABAR size
**/
UINT32
GetSataAbarSize (
  IN UINT32  SataCtrlIndex
  )
{
  UINT32 SataGc;
  ASSERT (SataCtrlIndex < MaxSataControllerNum ());
  SataGc = PciSegmentRead32 (SataPciCfgBase (SataCtrlIndex) + R_SATA_CFG_SATAGC);

  switch (SataGc & B_SATA_CFG_SATAGC_ASSEL) {
    case V_SATA_CFG_SATAGC_ASSEL_2K:
      return SIZE_2KB;
      break;

    case V_SATA_CFG_SATAGC_ASSEL_16K:
      return SIZE_16KB;
      break;

    case V_SATA_CFG_SATAGC_ASSEL_32K:
      return SIZE_32KB;
      break;

    case V_SATA_CFG_SATAGC_ASSEL_64K:
      return SIZE_64KB;
      break;

    case V_SATA_CFG_SATAGC_ASSEL_128K:
      return SIZE_128KB;
      break;

    case V_SATA_CFG_SATAGC_ASSEL_512K:
      return SIZE_256KB;
      break;

    default:
      return SIZE_2KB;
      break;
  }
}

/**
  Get SATA controller AHCI base address

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller AHCI base address
**/
UINT32
GetSataAhciBase (
  IN UINT32  SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < MaxSataControllerNum ());

  return PciSegmentRead32 (SataPciCfgBase (SataCtrlIndex) + R_SATA_CFG_AHCI_BAR) & 0xFFFFF800;
}

