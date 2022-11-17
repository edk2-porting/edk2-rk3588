/** @file
Do Platform Stage System Agent initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiSaPolicyUpdate.h"
#include <Guid/MemoryTypeInformation.h>
#include <Library/HobLib.h>
#include <PchAccess.h>
#include <SaAccess.h>
#include <Pi/PiFirmwareFile.h>
#include <Pi/PiPeiCis.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiSaPolicyLib.h>
#include <Library/PeiLib.h>

CONST UINT8 mPxRcConfig[8] = { 11, 10, 11, 11, 11, 11, 11, 11 };

/**
  Performs FSP SA PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSaPolicyUpdate (
  IN OUT FSPS_UPD    *FspsUpd
  )
{
  VOID                            *Buffer;
  VOID                            *MemBuffer;
  UINT32                          Size;

  DEBUG((DEBUG_INFO, "\nUpdating SA Policy in Post Mem\n"));

  FspsUpd->FspsConfig.PeiGraphicsPeimInit = 1;

  Size   = 0;
  Buffer = NULL;
  PeiGetSectionFromAnyFv (PcdGetPtr (PcdGraphicsVbtGuid), EFI_SECTION_RAW, 0, &Buffer, &Size);
  if (Buffer == NULL) {
    DEBUG((DEBUG_WARN, "Could not locate VBT\n"));
  } else {
    MemBuffer = (VOID *)AllocatePages (EFI_SIZE_TO_PAGES ((UINTN)Size));
    if ((MemBuffer != NULL) && (Buffer != NULL)) {
      CopyMem (MemBuffer, (VOID *)Buffer, (UINTN)Size);
      FspsUpd->FspsConfig.GraphicsConfigPtr = (UINT32)(UINTN)MemBuffer;
    } else {
      DEBUG((DEBUG_WARN, "Error in locating / copying VBT.\n"));
      FspsUpd->FspsConfig.GraphicsConfigPtr = 0;
    }
  }
  DEBUG((DEBUG_INFO, "Vbt Pointer from PeiGetSectionFromFv is 0x%x\n", FspsUpd->FspsConfig.GraphicsConfigPtr));
  DEBUG((DEBUG_INFO, "Vbt Size from PeiGetSectionFromFv is 0x%x\n", Size));

  Size   = 0;
  Buffer = NULL;
  PeiGetSectionFromAnyFv (&gTianoLogoGuid, EFI_SECTION_RAW, 0, &Buffer, &Size);
  if (Buffer == NULL) {
    DEBUG((DEBUG_WARN, "Could not locate Logo\n"));
  } else {
    MemBuffer = (VOID *)AllocatePages (EFI_SIZE_TO_PAGES ((UINTN)Size));
    if ((MemBuffer != NULL) && (Buffer != NULL)) {
      CopyMem (MemBuffer, (VOID *)Buffer, (UINTN)Size);
      FspsUpd->FspsConfig.LogoPtr = (UINT32)(UINTN)MemBuffer;
      FspsUpd->FspsConfig.LogoSize = Size;
    } else {
      DEBUG((DEBUG_WARN, "Error in locating / copying LogoPtr.\n"));
      FspsUpd->FspsConfig.LogoPtr = 0;
      FspsUpd->FspsConfig.LogoSize = 0;
    }
  }
  DEBUG((DEBUG_INFO, "LogoPtr from PeiGetSectionFromFv is 0x%x\n", FspsUpd->FspsConfig.LogoPtr));
  DEBUG((DEBUG_INFO, "LogoSize from PeiGetSectionFromFv is 0x%x\n", FspsUpd->FspsConfig.LogoSize));

  CopyMem (&FspsUpd->FspsConfig.PxRcConfig, mPxRcConfig, sizeof(mPxRcConfig));

  //
  // CPU power management config
  //
  FspsUpd->FspsConfig.TdcEnable[0]                        = 0x1;
  FspsUpd->FspsConfig.TdcEnable[1]                        = 0x1;
  FspsUpd->FspsConfig.TdcEnable[3]                        = 0x1;
  FspsUpd->FspsConfig.TdcPowerLimit[1]                    = 0x2b0;
  FspsUpd->FspsConfig.TdcPowerLimit[3]                    = 0xc8;
  FspsUpd->FspsConfig.AcLoadline[0]                       = 0x406;
  FspsUpd->FspsConfig.AcLoadline[1]                       = 0xb4;
  FspsUpd->FspsConfig.AcLoadline[3]                       = 0x10e;
  FspsUpd->FspsConfig.DcLoadline[0]                       = 0x406;
  FspsUpd->FspsConfig.DcLoadline[1]                       = 0xb4;
  FspsUpd->FspsConfig.DcLoadline[3]                       = 0x10e;
  FspsUpd->FspsConfig.Psi1Threshold[0]                    = 0x50;
  FspsUpd->FspsConfig.Psi1Threshold[1]                    = 0x50;
  FspsUpd->FspsConfig.Psi1Threshold[2]                    = 0x50;
  FspsUpd->FspsConfig.Psi1Threshold[3]                    = 0x50;
  FspsUpd->FspsConfig.Psi1Threshold[4]                    = 0x50;
  FspsUpd->FspsConfig.Psi2Threshold[0]                    = 0x14;
  FspsUpd->FspsConfig.Psi2Threshold[1]                    = 0x14;
  FspsUpd->FspsConfig.Psi2Threshold[2]                    = 0x14;
  FspsUpd->FspsConfig.Psi2Threshold[3]                    = 0x14;
  FspsUpd->FspsConfig.Psi2Threshold[4]                    = 0x14;
  FspsUpd->FspsConfig.Psi3Threshold[0]                    = 0x4;
  FspsUpd->FspsConfig.Psi3Threshold[1]                    = 0x4;
  FspsUpd->FspsConfig.Psi3Threshold[2]                    = 0x4;
  FspsUpd->FspsConfig.Psi3Threshold[3]                    = 0x4;
  FspsUpd->FspsConfig.Psi3Threshold[4]                    = 0x4;
  FspsUpd->FspsConfig.IccMax[0]                           = 0x2c;
  FspsUpd->FspsConfig.IccMax[1]                           = 0x230;
  FspsUpd->FspsConfig.IccMax[3]                           = 0x80;
  FspsUpd->FspsConfig.McivrSpreadSpectrum                 = 0x3;

  FspsUpd->FspsTestConfig.OneCoreRatioLimit               = 0x29;
  FspsUpd->FspsTestConfig.TwoCoreRatioLimit               = 0x28;
  FspsUpd->FspsTestConfig.ThreeCoreRatioLimit             = 0x27;
  FspsUpd->FspsTestConfig.FourCoreRatioLimit              = 0x26;
  FspsUpd->FspsTestConfig.FiveCoreRatioLimit              = 0x25;
  FspsUpd->FspsTestConfig.SixCoreRatioLimit               = 0x24;
  FspsUpd->FspsTestConfig.TccActivationOffset             = 0x0;
  FspsUpd->FspsTestConfig.TccOffsetClamp                  = 0x0;

  FspsUpd->FspsTestConfig.PowerLimit1                     = 0x0;
  FspsUpd->FspsTestConfig.PowerLimit2Power                = 0x0;
  FspsUpd->FspsTestConfig.PowerLimit3                     = 0x0;
  FspsUpd->FspsTestConfig.PowerLimit4                     = 0x0;
  FspsUpd->FspsTestConfig.Custom1PowerLimit1              = 0x0;
  FspsUpd->FspsTestConfig.Custom1PowerLimit2              = 0x0;
  FspsUpd->FspsTestConfig.Custom2PowerLimit1              = 0x0;
  FspsUpd->FspsTestConfig.Custom2PowerLimit2              = 0x0;
  FspsUpd->FspsTestConfig.Custom3PowerLimit1              = 0x0;
  FspsUpd->FspsTestConfig.Custom3PowerLimit2              = 0x0;
  FspsUpd->FspsTestConfig.Custom1PowerLimit1Time          = 0x0;
  FspsUpd->FspsTestConfig.Custom1TurboActivationRatio     = 0x0;
  FspsUpd->FspsTestConfig.Custom2PowerLimit1Time          = 0x0;
  FspsUpd->FspsTestConfig.Custom2TurboActivationRatio     = 0x0;
  FspsUpd->FspsTestConfig.Custom3PowerLimit1Time          = 0x0;
  FspsUpd->FspsTestConfig.Custom3TurboActivationRatio     = 0x0;

  FspsUpd->FspsTestConfig.VoltageOptimization             = 0x0;
  FspsUpd->FspsTestConfig.TStates                         = 0x0;
  FspsUpd->FspsTestConfig.ProcHotResponse                 = 0x0;
  FspsUpd->FspsTestConfig.Cx                              = 0x1;
  FspsUpd->FspsTestConfig.PkgCStateLimit                  = 0xff;
  FspsUpd->FspsTestConfig.MaxRatio                        = 0x8;
  FspsUpd->FspsTestConfig.PsysPmax                        = 0x0;
  FspsUpd->FspsTestConfig.CstateLatencyControl0Irtl       = 0x4e;

  return EFI_SUCCESS;
}


