/** @file
  Implementation of Fsp SA Policy Initialization.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PeiFspPolicyInitLib.h>

#include <Ppi/SiPolicy.h>
#include <MemoryConfig.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiLib.h>
#include <IndustryStandard/Pci.h>
#include <IndustryStandard/Bmp.h>
#include <Ppi/FirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Pi/PiPeiCis.h>
#include <Core/Pei/PeiMain.h>
#include <Library/ConfigBlockLib.h>
#include <PolicyUpdateMacro.h>
#include <Pi/PiFirmwareFile.h>
#include <Protocol/GraphicsOutput.h>
#include <Library/BmpSupportLib.h>

/**
  Performs FSP SA PEI Policy initialization in pre-memory.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSaPolicyInitPreMem (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  SA_MISC_PEI_PREMEM_CONFIG   *MiscPeiPreMemConfig;
  HOST_BRIDGE_PREMEM_CONFIG   *HostBridgePreMemConfig;
  MEMORY_CONFIG_NO_CRC        *MemConfigNoCrc;
#if FixedPcdGet8(PcdFspModeSelection) == 0
  EFI_STATUS                  Status;
  SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi;
#endif

  DEBUG((DEBUG_INFO, "\nUpdating SA Policy in Pre Mem\n"));
  MiscPeiPreMemConfig = NULL;
  HostBridgePreMemConfig = NULL;
  MemConfigNoCrc      = NULL;
#if FixedPcdGet8(PcdFspModeSelection) == 0
  //
  // Locate SiPreMemPolicyPpi
  //
  SiPreMemPolicyPpi   = NULL;
  Status = PeiServicesLocatePpi(
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  if ((Status == EFI_SUCCESS) && (SiPreMemPolicyPpi != NULL)) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gSaMiscPeiPreMemConfigGuid, (VOID *) &MiscPeiPreMemConfig);
    ASSERT_EFI_ERROR (Status);
    Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
    ASSERT_EFI_ERROR(Status);
    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
    ASSERT_EFI_ERROR (Status);
    ZeroMem ((VOID *) MemConfigNoCrc->SpdData->SpdData, sizeof (SPD_DATA_BUFFER));
  }
#endif

  UPDATE_POLICY (FspmUpd->FspmConfig.SpdAddressTable[0], MiscPeiPreMemConfig->SpdAddressTable[0], PcdGet8 (PcdMrcSpdAddressTable0));
  UPDATE_POLICY (FspmUpd->FspmConfig.SpdAddressTable[1], MiscPeiPreMemConfig->SpdAddressTable[1], PcdGet8 (PcdMrcSpdAddressTable1));
  UPDATE_POLICY (FspmUpd->FspmConfig.SpdAddressTable[2], MiscPeiPreMemConfig->SpdAddressTable[2], PcdGet8 (PcdMrcSpdAddressTable2));
  UPDATE_POLICY (FspmUpd->FspmConfig.SpdAddressTable[3], MiscPeiPreMemConfig->SpdAddressTable[3], PcdGet8 (PcdMrcSpdAddressTable3));

  if (PcdGet32 (PcdMrcSpdData)) {
    DEBUG((DEBUG_INFO, "PcdMrcSpdData != NULL, MemConfigNoCrc->SpdData\n"));
    COPY_POLICY ((VOID *)((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr000, (VOID *)MemConfigNoCrc->SpdData->SpdData[0][0][0], (VOID *)(UINTN)PcdGet32 (PcdMrcSpdData), PcdGet16(PcdMrcSpdDataSize));
    COPY_POLICY ((VOID *)((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr010, (VOID *)MemConfigNoCrc->SpdData->SpdData[0][1][0], (VOID *)(UINTN)PcdGet32 (PcdMrcSpdData), PcdGet16(PcdMrcSpdDataSize));
    COPY_POLICY ((VOID *)((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr020, (VOID *)MemConfigNoCrc->SpdData->SpdData[0][2][0], (VOID *)(UINTN)PcdGet32 (PcdMrcSpdData), PcdGet16(PcdMrcSpdDataSize));
    COPY_POLICY ((VOID *)((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr030, (VOID *)MemConfigNoCrc->SpdData->SpdData[0][3][0], (VOID *)(UINTN)PcdGet32 (PcdMrcSpdData), PcdGet16(PcdMrcSpdDataSize));
    COPY_POLICY ((VOID *)((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr100, (VOID *)MemConfigNoCrc->SpdData->SpdData[1][0][0], (VOID *)(UINTN)PcdGet32 (PcdMrcSpdData), PcdGet16(PcdMrcSpdDataSize));
    COPY_POLICY ((VOID *)((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr110, (VOID *)MemConfigNoCrc->SpdData->SpdData[1][1][0], (VOID *)(UINTN)PcdGet32 (PcdMrcSpdData), PcdGet16(PcdMrcSpdDataSize));
    COPY_POLICY ((VOID *)((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr120, (VOID *)MemConfigNoCrc->SpdData->SpdData[1][2][0], (VOID *)(UINTN)PcdGet32 (PcdMrcSpdData), PcdGet16(PcdMrcSpdDataSize));
    COPY_POLICY ((VOID *)((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr130, (VOID *)MemConfigNoCrc->SpdData->SpdData[1][3][0], (VOID *)(UINTN)PcdGet32 (PcdMrcSpdData), PcdGet16(PcdMrcSpdDataSize));
  }
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TsegSize,           MiscPeiPreMemConfig->TsegSize,           PcdGet32 (PcdTsegSize));
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.UserBd,             MiscPeiPreMemConfig->UserBd,             PcdGet8 (PcdSaMiscUserBd));
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MmioSizeAdjustment, HostBridgePreMemConfig->MmioSizeAdjustment, PcdGet16 (PcdSaMiscMmioSizeAdjustment));

  return EFI_SUCCESS;
}


/**
  Performs FSP SA PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
UpdateGraphics(
    IN OUT FSPS_UPD *FspsUpd,
    GRAPHICS_PEI_CONFIG *GtConfig
  )
{
  EFI_STATUS                      Status;
  VOID                            *Buffer;
  UINT32                          Size;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *Blt;
  UINTN                           BltSize;
  UINTN                           Height;
  UINTN                           Width;

  DEBUG((DEBUG_INFO, "\nUpdating SA Policy in Post Mem\n"));
  UPDATE_POLICY (FspsUpd->FspsConfig.PeiGraphicsPeimInit, GtConfig->PeiGraphicsPeimInit, 1);

  Size = 0;
  Buffer = NULL;
  PeiGetSectionFromAnyFv(PcdGetPtr(PcdIntelGraphicsVbtFileGuid), EFI_SECTION_RAW, 0, &Buffer, &Size);
  DEBUG((DEBUG_INFO, "Vbt Pointer from PeiGetSectionFromAnyFv is 0x%x\n", Buffer));
  DEBUG((DEBUG_INFO, "Vbt Size from PeiGetSectionFromAnyFv is 0x%x\n", Size));
#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspsUpd->FspsConfig.GraphicsConfigPtr = (UINT32)(UINTN)Buffer;
#else
  GtConfig->GraphicsConfigPtr = Buffer;
#endif

  Size = 0;
  Buffer = NULL;
  PeiGetSectionFromAnyFv(&gTianoLogoGuid, EFI_SECTION_RAW, 0, &Buffer, &Size);
  DEBUG((DEBUG_INFO, "LogoPtr from PeiGetSectionFromAnyFv is 0x%x\n", Buffer));
  DEBUG((DEBUG_INFO, "LogoSize from PeiGetSectionFromAnyFv is 0x%x\n", Size));
#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspsUpd->FspsConfig.LogoPtr = (UINT32)Buffer;
  FspsUpd->FspsConfig.LogoSize = Size;
#else
  GtConfig->LogoPtr = Buffer;
  GtConfig->LogoSize = Size;
#endif

  if (Buffer != NULL) {
    Blt = NULL;
    Status = TranslateBmpToGopBlt (
              Buffer,
              Size,
              &Blt,
              &BltSize,
              &Height,
              &Width
              );
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "TranslateBmpToGopBlt, Status = %r\n", Status));
        ASSERT_EFI_ERROR (Status);
        return Status;
    }

    UPDATE_POLICY(FspsUpd->FspsConfig.BltBufferSize,  GtConfig->BltBufferSize, BltSize);
    UPDATE_POLICY(FspsUpd->FspsConfig.LogoPixelWidth,  GtConfig->LogoPixelWidth, Width);
    UPDATE_POLICY(FspsUpd->FspsConfig.LogoPixelHeight,  GtConfig->LogoPixelHeight, Height);
#if FixedPcdGet8(PcdFspModeSelection) == 1
    FspsUpd->FspsConfig.BltBufferAddress  = (UINT32) Blt;
#else
    GtConfig->BltBufferAddress = (VOID *) Blt;
#endif
  }

  return EFI_SUCCESS;
}

/**
  Performs FSP SA PEI Policy initialization.

  @param[in][out]  FspsUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspSaPolicyInit (
  IN OUT FSPS_UPD    *FspsUpd
  )
{
#if FixedPcdGet8(PcdFspModeSelection) == 0
  EFI_STATUS                Status;
  SI_POLICY_PPI             *SiPolicyPpi;
#endif
  SA_MISC_PEI_CONFIG        *MiscPeiConfig;
  GRAPHICS_PEI_CONFIG       *GtConfig;

  MiscPeiConfig = NULL;
  GtConfig      = NULL;

#if FixedPcdGet8(PcdFspModeSelection) == 0
  //
  // Locate SiPolicyPpi
  //
  SiPolicyPpi = NULL;
  Status = PeiServicesLocatePpi(
             &gSiPolicyPpiGuid,
             0,
             NULL,
             (VOID **)&SiPolicyPpi
             );
  if ((Status == EFI_SUCCESS) && (SiPolicyPpi != NULL)) {
    MiscPeiConfig = NULL;
    Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSaMiscPeiConfigGuid, (VOID *) &MiscPeiConfig);
    ASSERT_EFI_ERROR (Status);

    GtConfig = NULL;
    Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gGraphicsPeiConfigGuid, (VOID *) &GtConfig);
    ASSERT_EFI_ERROR (Status);

  }
#endif

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP Wrapper UpdatePeiSaPolicy\n"));

  //
  // Update UPD: VBT & LogoPtr
  //
  UpdateGraphics(FspsUpd, GtConfig);

  return EFI_SUCCESS;
}

