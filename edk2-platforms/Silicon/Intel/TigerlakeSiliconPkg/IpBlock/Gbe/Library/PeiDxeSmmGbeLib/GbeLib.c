/** @file
  Gbe Library.
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
#include <Library/PchPcrLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/GbeMdiLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/GbeRegs.h>
#include <Library/PchPciBdfLib.h>

/**
  Check whether GbE region is valid
  Check SPI region directly since GbE might be disabled in SW.

  @retval TRUE                    Gbe Region is valid
  @retval FALSE                   Gbe Region is invalid
**/
BOOLEAN
IsGbeRegionValid (
  VOID
  )
{
  return SpiIsGbeRegionValid ();
}

/**
  Check whether GBE controller is enabled in the platform.

  @retval TRUE                    GbE is enabled
  @retval FALSE                   GbE is disabled
**/
BOOLEAN
IsGbePresent (
  VOID
  )
{
  //
  // Check PCH Support
  //
  if (!PchIsGbeSupported ()) {
    return FALSE;
  }
  //
  // Check PMC strap/fuse
  //
  if (!PmcIsGbeSupported ()) {
    return FALSE;
  }
  //
  // Check GbE NVM
  //
  if (IsGbeRegionValid () == FALSE) {
    return FALSE;
  }
  return TRUE;
}

/**
  Verifies Gigabit Ethernet PCI Class Code

  @param [in]  GbePciCfgBase      GbE PCI Config Space Address

  @retval TRUE                    GbE Class Code match
  @retval FALSE                   GbE Class Code does not match
**/
BOOLEAN
STATIC
GbeCheckClassCode (
  UINT64 GbePciCfgBase
  )
{
  UINT8 BaseCode;
  UINT8 SubClassCode;

  SubClassCode  = PciSegmentRead8 (GbePciCfgBase + PCI_CLASSCODE_OFFSET + 1);
  BaseCode      = PciSegmentRead8 (GbePciCfgBase + PCI_CLASSCODE_OFFSET + 2);

  if ((BaseCode != PCI_CLASS_NETWORK) || (SubClassCode != PCI_CLASS_NETWORK_ETHERNET)) {
    DEBUG ((DEBUG_ERROR, "GbeCheckClassCode : BaseCode(0x%x) or ClassCode(0x%x) is not supported\n", BaseCode, SubClassCode));
    ASSERT (FALSE);
    return FALSE;
  }
  return TRUE;
}

/**
  Checks if Gbe is Enabled or Disabled

  @retval  BOOLEAN    TRUE if device is enabled, FALSE otherwise.
**/
BOOLEAN
IsGbeEnabled (
  VOID
  )
{
  UINT64  GbePciBase;

  GbePciBase = GbePciCfgBase ();

  if (PciSegmentRead32 (GbePciBase) != 0xFFFFFFFF) {
    return GbeCheckClassCode (GbePciBase);
  }

  return FALSE;
}
