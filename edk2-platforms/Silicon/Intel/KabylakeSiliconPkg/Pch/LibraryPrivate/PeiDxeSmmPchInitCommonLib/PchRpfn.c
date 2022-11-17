/** @file
  This file contains functions that configures PCI Express Root Ports function swapping.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MmPciLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPciExpressHelpersLib.h>
#include <Library/PchInitCommonLib.h>
#include <Library/PchPsfPrivateLib.h>
#include <Library/S3BootScriptLib.h>

/**
  Configure root port function number mapping

  @retval EFI_SUCCESS                   The function completed successfully
**/
EFI_STATUS
PchConfigureRpfnMapping (
  VOID
  )
{
  UINT8                                 PortIndex;
  UINT8                                 OriginalFuncZeroRp;
  UINT8                                 MaxPciePortNum;
  UINT32                                Data32;
  UINTN                                 DevNum;
  UINTN                                 FuncNum;
  UINTN                                 RpBase;
  UINT32                                ControllerPcd[PCH_MAX_PCIE_CONTROLLERS];
  PCH_SERIES                            PchSeries;
  PCH_GENERATION                        PchGen;
  UINT32                                PcieControllers;
  UINT32                                ControllerIndex;
  UINT32                                FirstController;
  PCH_SBI_PID                           ControllerPid;

  DEBUG ((DEBUG_INFO,"PchConfigureRpfnMapping () Start\n"));

  PchSeries      = GetPchSeries ();
  PchGen         = GetPchGeneration ();
  MaxPciePortNum = GetPchMaxPciePortNum ();

  PcieControllers = GetPchMaxPcieControllerNum ();

  for (ControllerIndex = 0; ControllerIndex < PcieControllers; ++ControllerIndex) {
    PchPcrRead32 (PchGetPcieControllerSbiPid (ControllerIndex), R_PCH_PCR_SPX_PCD, &ControllerPcd[ControllerIndex]);
  }

  ///
  /// Configure root port function number mapping
  ///
  for (PortIndex = 0; PortIndex < MaxPciePortNum; ) {
    GetPchPcieRpDevFun (PortIndex, &DevNum, &FuncNum);
    RpBase = MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, (UINT32) DevNum, (UINT32) FuncNum);
    //
    // Search for first enabled function
    //
    if (MmioRead16 (RpBase) != 0xFFFF) {
      if (FuncNum != 0) {
        //
        // First enabled root port that is not function zero will be swapped with function zero on the same device
        // RP PCD register must sync with PSF RP function config register
        //
        ControllerIndex    = PortIndex / 4;
        OriginalFuncZeroRp = (PortIndex / 8) * 8;
        FirstController    = OriginalFuncZeroRp / 4;

        //
        // The enabled root port becomes function zero
        //
        ControllerPcd[ControllerIndex] &= (UINT32) ~(B_PCH_PCR_SPX_PCD_RP1FN << ((PortIndex % 4) * S_PCH_PCR_SPX_PCD_RP_FIELD));
        ControllerPcd[ControllerIndex] |= 0u;
        //
        // Origianl function zero on the same device takes the numer of the current port
        //
        ControllerPcd[FirstController] &= (UINT32) ~B_PCH_PCR_SPX_PCD_RP1FN;
        ControllerPcd[FirstController] |= (UINT32) FuncNum;

        //
        // Program PSF1 RP function config register.
        //
        PsfSetPcieFunctionWithS3BootScript (OriginalFuncZeroRp, (UINT32) FuncNum);
        PsfSetPcieFunctionWithS3BootScript (PortIndex, 0);
      }
      //
      // Once enabled root port was found move to next PCI device
      //
      PortIndex = ((PortIndex / 8) + 1) * 8;
      continue;
    }
    //
    // Continue search for first enabled root port
    //
    PortIndex++;
  }

  //
  // Write to PCD and lock the register
  //
  for (ControllerIndex = 0; ControllerIndex < PcieControllers; ++ControllerIndex) {
    ControllerPid = PchGetPcieControllerSbiPid (ControllerIndex);
    Data32 = ControllerPcd[ControllerIndex] | B_PCH_PCR_SPX_PCD_SRL;
    PchPcrWrite32 (ControllerPid, R_PCH_PCR_SPX_PCD, Data32);
    PCH_PCR_BOOT_SCRIPT_WRITE (
      S3BootScriptWidthUint32,
      ControllerPid, R_PCH_PCR_SPX_PCD,
      1,
      &Data32
      );
  }
  return EFI_SUCCESS;
}

