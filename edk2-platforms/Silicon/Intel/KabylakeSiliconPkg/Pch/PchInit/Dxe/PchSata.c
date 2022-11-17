/** @file
  Perform related functions for PCH Sata in DXE phase

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <PchInit.h>

/**
  Perform the remaining configuration on PCH SATA to perform device detection,
  then set the SATA SPD and PxE corresponding, and set the Register Lock on PCH SATA

  @retval None
**/
VOID
ConfigurePchSataOnEndOfDxe (
  VOID
  )
{
  UINTN         PciSataRegBase;
  UINT16        SataPortsEnabled;
  PCH_SERIES    PchSeries;
  UINT16        WordReg;
  UINT32        DwordReg;
  UINTN         Index;

  ///
  /// SATA PCS: Enable the port in any of below condition:
  /// i.)   Hot plug is enabled
  /// ii.)  A device is attached
  /// iii.) Test mode is enabled
  /// iv.)  Configured as eSATA port
  ///
  PciSataRegBase    = MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_SATA, PCI_FUNCTION_NUMBER_PCH_SATA);
  PchSeries         = GetPchSeries ();
  SataPortsEnabled  = 0;

  if (MmioRead16 (PciSataRegBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return;
  }
  if (PchSeries == PchLp) {
    WordReg           = MmioRead16 (PciSataRegBase + R_PCH_LP_SATA_PCS);
    for (Index = 0; Index < GetPchMaxSataPortNum (); Index++) {
      if ((mPchConfigHob->Sata.PortSettings[Index].HotPlug == TRUE) ||
          (WordReg & (B_PCH_LP_SATA_PCS_P0P << Index)) ||
          (mPchConfigHob->Sata.TestMode == TRUE) ||
          (mPchConfigHob->Sata.PortSettings[Index].External == TRUE)) {
        SataPortsEnabled |= (mPchConfigHob->Sata.PortSettings[Index].Enable << Index);
      }
    }

    ///
    /// Set MAP."Sata PortX Disable", SATA PCI offset 90h[10:8] to 1b if SATA Port 0/1/2 is disabled
    ///
    MmioOr16 (PciSataRegBase + R_PCH_LP_SATA_MAP, ((~SataPortsEnabled << N_PCH_LP_SATA_MAP_SPD) & (UINT16) B_PCH_LP_SATA_MAP_SPD));
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint16,
      (UINTN) (PciSataRegBase + R_PCH_LP_SATA_MAP),
      1,
      (VOID *) (UINTN) (PciSataRegBase + R_PCH_LP_SATA_MAP)
      );

    ///
    /// Program PCS "Port X Enabled", SATA PCI offset 92h[2:0] = Port 0~2 Enabled bit as per SataPortsEnabled value.
    ///
    MmioOr16 (PciSataRegBase + R_PCH_LP_SATA_PCS, SataPortsEnabled);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint16,
      (UINTN) (PciSataRegBase + R_PCH_LP_SATA_PCS),
      1,
      (VOID *) (UINTN) (PciSataRegBase + R_PCH_LP_SATA_PCS)
      );
  } else {
    DwordReg          = MmioRead32 (PciSataRegBase + R_PCH_H_SATA_PCS);
    for (Index = 0; Index < GetPchMaxSataPortNum (); Index++) {
      if ((mPchConfigHob->Sata.PortSettings[Index].HotPlug == TRUE) ||
          (DwordReg & (B_PCH_H_SATA_PCS_P0P << Index)) ||
          (mPchConfigHob->Sata.TestMode == TRUE) ||
          (mPchConfigHob->Sata.PortSettings[Index].External == TRUE)) {
        SataPortsEnabled |= (mPchConfigHob->Sata.PortSettings[Index].Enable << Index);
      }
    }

    ///
    /// Set MAP."Sata PortX Disable", SATA PCI offset 90h[23:16] to 1b if SATA Port 0/1/2/3/4/5/6/7 is disabled
    ///
    MmioOr32 (PciSataRegBase + R_PCH_H_SATA_MAP, ((~SataPortsEnabled << N_PCH_H_SATA_MAP_SPD) & (UINT32) B_PCH_H_SATA_MAP_SPD));
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      (UINTN) (PciSataRegBase + R_PCH_H_SATA_MAP),
      1,
      (VOID *) (UINTN) (PciSataRegBase + R_PCH_H_SATA_MAP)
      );

    ///
    /// Program PCS "Port X Enabled", SATA PCI offset 94h[7:0] = Port 0~7 Enabled bit as per SataPortsEnabled value.
    ///
    MmioOr16 (PciSataRegBase + R_PCH_H_SATA_PCS, SataPortsEnabled);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint16,
      (UINTN) (PciSataRegBase + R_PCH_H_SATA_PCS),
      1,
      (VOID *) (UINTN) (PciSataRegBase + R_PCH_H_SATA_PCS)
      );
  }

  ///
  /// Step 14
  /// Program SATA PCI offset 9Ch [31] to 1b
  ///
  MmioOr32 ((UINTN) (PciSataRegBase + R_PCH_SATA_SATAGC), BIT31);
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    (UINTN) (PciSataRegBase + R_PCH_SATA_SATAGC),
    1,
    (VOID *) (UINTN) (PciSataRegBase + R_PCH_SATA_SATAGC)
    );
}
