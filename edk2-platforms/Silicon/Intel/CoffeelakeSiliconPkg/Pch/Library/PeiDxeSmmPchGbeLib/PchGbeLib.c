/** @file
  PCH Gbe Library.
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
#include <Library/PchPcrLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Private/Library/PmcPrivateLib.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsSpi.h>

/**
  Check whether GbE region is valid
  Check SPI region directly since GbE might be disabled in SW.

  @retval TRUE                    Gbe Region is valid
  @retval FALSE                   Gbe Region is invalid
**/
BOOLEAN
PchIsGbeRegionValid (
  VOID
  )
{
  UINT32  SpiBar;
  SpiBar = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (
                               DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                               DEFAULT_PCI_BUS_NUMBER_PCH,
                               PCI_DEVICE_NUMBER_PCH_SPI,
                               PCI_FUNCTION_NUMBER_PCH_SPI,
                               R_SPI_CFG_BAR0)) & ~B_SPI_CFG_BAR0_MASK;
  ASSERT (SpiBar != 0);
  if (MmioRead32 (SpiBar + R_SPI_MEM_FREG3_GBE) != B_SPI_MEM_FREGX_BASE_MASK) {
    return TRUE;
  }
  return FALSE;
}


/**
  Check whether LAN controller is enabled in the platform.

  @retval TRUE                    GbE is enabled
  @retval FALSE                   GbE is disabled
**/
BOOLEAN
PchIsGbePresent (
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
  if (PchIsGbeRegionValid () == FALSE) {
    return FALSE;
  }
  return TRUE;
}

