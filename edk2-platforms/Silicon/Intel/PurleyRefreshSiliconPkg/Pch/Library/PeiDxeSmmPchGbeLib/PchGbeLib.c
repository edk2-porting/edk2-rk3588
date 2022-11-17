/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/MmPciBaseLib.h>
#include <Library/PchPmcLib.h>
#include <PchAccess.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchCycleDecodingLib.h>

#include <PiPei.h>
#include <Ppi/Spi.h>
#include <Library/PeiServicesLib.h>
extern EFI_GUID  gPeiSpiPpiGuid;
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

  PchPcrRead32 (PID_FIAWM26, R_PCH_PCR_FIA_STRPFUSECFG1_REG_BASE, &PcieStrapFuse);
  if ((PcieStrapFuse & B_PCH_PCR_FIA_STRPFUSECFG1_GBE_PCIE_PEN) == 0) {
    return 0; // GbE disabled
  }
  GbePortSel = (PcieStrapFuse & B_PCH_PCR_FIA_STRPFUSECFG1_GBE_PCIEPORTSEL) >> N_PCH_PCR_FIA_STRPFUSECFG1_GBE_PCIEPORTSEL;

    switch (GbePortSel) {
      case 0: return  3 + 1;
      case 1: return  4 + 1;
      case 2: return  5 + 1;
      case 3: return  8 + 1;
      case 4: return 11 + 1;
    }

  DEBUG((DEBUG_ERROR, "Invalid GbE port\n"));
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

  UINT32                  SoftstrapVal;
  EFI_SPI_PROTOCOL        *SpiProtocol = NULL;
  EFI_STATUS              Status;
  UINTN                   GbePciBase;

  if (PchIsDwrFlow() == TRUE) {
    return FALSE;
  }

  GbePciBase = MmPciBase (
                 DEFAULT_PCI_BUS_NUMBER_PCH,
                 PCI_DEVICE_NUMBER_PCH_LAN,
                 PCI_FUNCTION_NUMBER_PCH_LAN
                 );

  //
  // Check GBE disable strap
  //
  Status = PeiServicesLocatePpi (
               &gPeiSpiPpiGuid,
               0,
               NULL,
               (VOID **) &SpiProtocol
               );
  ASSERT_EFI_ERROR (Status);

  Status = SpiProtocol->ReadPchSoftStrap(SpiProtocol, 0x1DC, 4, &SoftstrapVal);
  if (!EFI_ERROR(Status)) {
    if ((SoftstrapVal & BIT14) == BIT14) {
      return FALSE;
    }
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
  if (MmioRead32 (GbePciBase) == 0xFFFFFFFF) {
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


