/** @file
  PCH Gbe Library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/MmPciLib.h>
#include <PchAccess.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchCycleDecodingLib.h>

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
  SpiBar = MmioRead32 (MmPciBase (
                         DEFAULT_PCI_BUS_NUMBER_PCH,
                         PCI_DEVICE_NUMBER_PCH_SPI,
                         PCI_FUNCTION_NUMBER_PCH_SPI)
                         + R_PCH_SPI_BAR0) & ~B_PCH_SPI_BAR0_MASK;
  ASSERT (SpiBar != 0);
  if (MmioRead32 (SpiBar + R_PCH_SPI_FREG3_GBE) != B_PCH_SPI_FREGX_BASE_MASK) {
    return TRUE;
  }
  return FALSE;
}

/**
  Returns GbE over PCIe port number based on a soft strap.

  @return                         Root port number (1-based)
  @retval 0                       GbE over PCIe disabled
**/
UINT32
PchGetGbePortNumber (
  VOID
  )
{
  UINT32   GbePortSel;
  UINT32   PcieStrapFuse;

  PchPcrRead32 (PID_FIA, R_PCH_PCR_FIA_STRPFUSECFG1_REG_BASE, &PcieStrapFuse);
  if ((PcieStrapFuse & B_PCH_PCR_FIA_STRPFUSECFG1_GBE_PCIE_PEN) == 0) {
    return 0; // GbE disabled
  }
  GbePortSel = (PcieStrapFuse & B_PCH_PCR_FIA_STRPFUSECFG1_GBE_PCIEPORTSEL) >> N_PCH_PCR_FIA_STRPFUSECFG1_GBE_PCIEPORTSEL;
  if (GetPchSeries () == PchLp) {
    switch (GbePortSel) {
      case 0: return 3;
      case 1: return 4;
      case 2: return 5;
      case 3: return 9;
      case 4: return 10;
    }
  } else {
    switch (GbePortSel) {
      case 0: return 4;
      case 1: return 5;
      case 2: return 9;
      case 3: return 12;
      case 4: return 13;
    }
  }
  DEBUG ((DEBUG_ERROR, "Invalid GbE port\n"));
  ASSERT (FALSE);
  return 0;
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
  UINT32  PwrmBase;
  UINT32  FuseDis2State;
  //
  // Check PMC strap/fuse
  //
  PchPwrmBaseGet (&PwrmBase);
  FuseDis2State = MmioRead32 (PwrmBase + R_PCH_PWRM_FUSE_DIS_RD_2);
  if (FuseDis2State & B_PCH_PWRM_FUSE_DIS_RD_2_GBE_FUSE_SS_DIS) {
    return FALSE;
  }
  //
  // Check FIA strap/fuse
  //
  if (PchGetGbePortNumber () == 0) {
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

/**
  Check whether LAN controller is enabled in the platform.

  @deprecated Use PchIsGbePresent instead.

  @retval TRUE                    GbE is enabled
  @retval FALSE                   GbE is disabled
**/
BOOLEAN
PchIsGbeAvailable (
  VOID
  )
{
  return PchIsGbePresent ();
}


