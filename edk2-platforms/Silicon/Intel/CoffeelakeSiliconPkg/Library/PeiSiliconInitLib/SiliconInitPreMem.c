/** @file
  Silicon Init APIs for MinPlatform BoardInitLib implementations.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Guid/TcoWdtHob.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PmcLib.h>
#include <Register/PchRegsLpc.h>
#include <Register/PchRegsPmc.h>

/**
  Early Silicon initialization
**/
VOID
EarlySiliconInit (
  VOID
  )
{
  UINT16       Data16;
  UINT8        Data8;
  UINT8        TcoRebootHappened;
  TCO_WDT_HOB  *TcoWdtHobPtr;
  EFI_STATUS   Status;

  ///
  /// LPC I/O Configuration
  ///
  PchLpcIoDecodeRangesSet (
    (V_LPC_CFG_IOD_LPT_378 << N_LPC_CFG_IOD_LPT)    |
    (V_LPC_CFG_IOD_COMB_3E8 << N_LPC_CFG_IOD_COMB)  |
    (V_LPC_CFG_IOD_COMA_3F8 << N_LPC_CFG_IOD_COMA)
    );

  PchLpcIoEnableDecodingSet (
    B_LPC_CFG_IOE_ME2 |
    B_LPC_CFG_IOE_SE  |
    B_LPC_CFG_IOE_ME1 |
    B_LPC_CFG_IOE_KE  |
    B_LPC_CFG_IOE_HGE |
    B_LPC_CFG_IOE_LGE |
    B_LPC_CFG_IOE_FDE |
    B_LPC_CFG_IOE_PPE |
    B_LPC_CFG_IOE_CBE |
    B_LPC_CFG_IOE_CAE
    );

  ///
  /// Halt the TCO timer
  ///
  Data16 = IoRead16 (PcdGet16 (PcdTcoBaseAddress) + R_TCO_IO_TCO1_CNT);
  Data16 |= B_TCO_IO_TCO1_CNT_TMR_HLT;
  IoWrite16 (PcdGet16 (PcdTcoBaseAddress) + R_TCO_IO_TCO1_CNT, Data16);

  ///
  /// Read the Second TO status bit
  ///
  Data8 = IoRead8 (PcdGet16 (PcdTcoBaseAddress) + R_TCO_IO_TCO2_STS);
  if ((Data8 & B_TCO_IO_TCO2_STS_SECOND_TO) == B_TCO_IO_TCO2_STS_SECOND_TO) {
    TcoRebootHappened = 1;
    DEBUG ((DEBUG_INFO, "PlatformInitPreMem - TCO Second TO status bit is set. This might be a TCO reboot\n"));
  }
  else {
    TcoRebootHappened = 0;
  }

  ///
  /// Create HOB
  ///
  Status = PeiServicesCreateHob (EFI_HOB_TYPE_GUID_EXTENSION, sizeof(TCO_WDT_HOB), (VOID **)&TcoWdtHobPtr);
  if (!EFI_ERROR (Status)) {
    TcoWdtHobPtr->Header.Name = gTcoWdtHobGuid;
    TcoWdtHobPtr->TcoRebootHappened = TcoRebootHappened;
  }

  ///
  /// Clear the Second TO status bit
  ///
  IoWrite8 (PcdGet16 (PcdTcoBaseAddress) + R_TCO_IO_TCO2_STS, B_TCO_IO_TCO2_STS_SECOND_TO);
}

/**
  Initialize the GPIO IO selection, GPIO USE selection, and GPIO signal inversion registers

**/
VOID
SiliconInit (
  VOID
  )
{
  UINT16       ABase;

  ABase = PmcGetAcpiBase ();

  ///
  /// Clear all pending SMI. On S3 clear power button enable so it will not generate an SMI.
  ///
  IoWrite16 (ABase + R_ACPI_IO_PM1_EN, 0);
  IoWrite32 (ABase + R_ACPI_IO_GPE0_EN_127_96, 0);
}
