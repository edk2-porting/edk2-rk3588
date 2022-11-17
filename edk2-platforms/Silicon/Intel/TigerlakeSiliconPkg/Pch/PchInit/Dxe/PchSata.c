/** @file
  Perform related functions for PCH Sata in DXE phase

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/S3BootScriptLib.h>

#include "PchInit.h"
#include <Library/SataLib.h>
#include <Library/PchPciBdfLib.h>
#include <Register/SataRegs.h>

/**
  Perform the remaining configuration on SATA to perform device detection,
  then set the SATA SPD and PxE corresponding, and set the Register Lock on PCH SATA

  @retval None
**/
VOID
ConfigureSataOnEndOfDxe (
  VOID
  )
{
  UINT64        PciSataRegBase;
  UINT16        SataPortsEnabled;
  UINT32        DwordReg;
  UINTN         Index;
  UINT32        SataCtrlIndex;

  for (SataCtrlIndex = 0; SataCtrlIndex < MaxSataControllerNum (); SataCtrlIndex++) {
    ///
    /// SATA PCS: Enable the port in any of below condition:
    /// i.)   Hot plug is enabled
    /// ii.)  A device is attached
    /// iii.) Test mode is enabled
    /// iv.)  Configured as eSATA port
    ///
    PciSataRegBase    = SataPciCfgBase (SataCtrlIndex);
    SataPortsEnabled  = 0;

    DwordReg = PciSegmentRead32 (PciSataRegBase + R_SATA_CFG_PCS);
    for (Index = 0; Index < MaxSataPortNum (SataCtrlIndex); Index++) {
      if ((mPchConfigHob->Sata[SataCtrlIndex].PortSettings[Index].HotPlug == TRUE) ||
          (DwordReg & (B_SATA_CFG_PCS_P0P << Index)) ||
            (mPchConfigHob->Sata[SataCtrlIndex].TestMode == TRUE) ||
            (mPchConfigHob->Sata[SataCtrlIndex].PortSettings[Index].External == TRUE)) {
          SataPortsEnabled |= (mPchConfigHob->Sata[SataCtrlIndex].PortSettings[Index].Enable << Index);
      }
    }

    ///
    /// Set MAP."Sata PortX Disable", SATA PCI offset 90h[23:16] to 1b if SATA Port 0/1/2/3/4/5/6/7 is disabled
    ///
    PciSegmentOr32 (PciSataRegBase + R_SATA_CFG_MAP, (~SataPortsEnabled << N_SATA_CFG_MAP_SPD));
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      PcdGet64 (PcdSiPciExpressBaseAddress) + PciSataRegBase + R_SATA_CFG_MAP,
      1,
      (VOID *) (UINTN) (PcdGet64 (PcdSiPciExpressBaseAddress) + PciSataRegBase + R_SATA_CFG_MAP)
      );

    ///
    /// Program PCS "Port X Enabled", SATA PCI offset 94h[7:0] = Port 0~7 Enabled bit as per SataPortsEnabled value.
    ///
    PciSegmentOr16 (PciSataRegBase + R_SATA_CFG_PCS, SataPortsEnabled);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint16,
      PcdGet64 (PcdSiPciExpressBaseAddress) + PciSataRegBase + R_SATA_CFG_PCS,
      1,
      (VOID *) (UINTN) (PcdGet64 (PcdSiPciExpressBaseAddress) + PciSataRegBase + R_SATA_CFG_PCS)
      );

    ///
    /// Step 14
    /// Program SATA PCI offset 9Ch [31] to 1b
    ///
    PciSegmentOr32 ((UINTN) (PciSataRegBase + R_SATA_CFG_SATAGC), BIT31);
    S3BootScriptSaveMemWrite (
      S3BootScriptWidthUint32,
      PcdGet64 (PcdSiPciExpressBaseAddress) + PciSataRegBase + R_SATA_CFG_SATAGC,
      1,
      (VOID *) (UINTN) (PcdGet64 (PcdSiPciExpressBaseAddress) + PciSataRegBase + R_SATA_CFG_SATAGC)
      );
  }
}
