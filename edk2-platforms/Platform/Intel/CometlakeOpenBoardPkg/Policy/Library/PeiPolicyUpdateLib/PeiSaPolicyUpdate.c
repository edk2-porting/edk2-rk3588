/** @file
Do Platform Stage System Agent initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiSaPolicyUpdate.h"
#include <Protocol/GraphicsOutput.h>
#include <IndustryStandard/Bmp.h>
#include <Guid/MemoryTypeInformation.h>
#include <Library/HobLib.h>
#include <Platform.h>
#include <Ppi/FirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Pi/PiPeiCis.h>
#include <Core/Pei/PeiMain.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiSaPolicyLib.h>
#include <GpioPinsCnlLp.h>
#include <GpioPinsCnlH.h>
#include <Library/ConfigBlockLib.h>

/**
  UpdatePeiSaPolicy performs SA PEI Policy initialization

  @param[in out] SiPolicyPpi     - SI_POLICY PPI

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
UpdatePeiSaPolicy (
  IN OUT   SI_POLICY_PPI      *SiPolicyPpi
  )
{
  EFI_STATUS                      Status;
  EFI_GUID                        FileGuid;
  VOID                            *Buffer;
  UINT8                           SaDisplayConfigTable[9] = {0};
  VOID                            *MemBuffer;
  BMP_IMAGE_HEADER                *BmpHeader;
  UINT64                          BltBufferSize;
  UINT32                          Size;
  GRAPHICS_PEI_CONFIG             *GtConfig;
  GNA_CONFIG                      *GnaConfig;
  WDT_PPI                         *gWdtPei;
  PCIE_PEI_CONFIG                 *PciePeiConfig;
  SA_MISC_PEI_CONFIG              *MiscPeiConfig;
  EFI_BOOT_MODE                   BootMode;

  DEBUG((DEBUG_INFO, "\nUpdating SA Policy in Post Mem\n"));

  Size = 0;
  MemBuffer = NULL;
  BmpHeader = NULL;
  BltBufferSize = 0;
  GtConfig = NULL;
  GnaConfig = NULL;

  Status = GetConfigBlock((VOID *) SiPolicyPpi, &gGraphicsPeiConfigGuid, (VOID *)&GtConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *) SiPolicyPpi, &gGnaConfigGuid, (VOID *)&GnaConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSaPciePeiConfigGuid, (VOID *)&PciePeiConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gSaMiscPeiConfigGuid, (VOID *)&MiscPeiConfig);
  ASSERT_EFI_ERROR (Status);


  //
  // Locate WDT_PPI (ICC WDT PPI)
  //
  gWdtPei = NULL;
  Status = PeiServicesLocatePpi(
             &gWdtPpiGuid,
             0,
             NULL,
             (VOID **) &gWdtPei
             );

  Status = PeiServicesGetBootMode(&BootMode);
  ASSERT_EFI_ERROR(Status);

  if (!EFI_ERROR (Status)) {
    Buffer = NULL;

    CopyMem(&FileGuid, PcdGetPtr(PcdIntelGraphicsVbtFileGuid), sizeof(FileGuid));
    PeiGetSectionFromFv(FileGuid, &Buffer, &Size);
    if (Buffer == NULL) {
      DEBUG((DEBUG_ERROR, "Could not locate VBT\n"));
    }

    MemBuffer = (VOID *)AllocatePages (EFI_SIZE_TO_PAGES ((UINTN)Size));
    if ((MemBuffer != NULL) && (Buffer != NULL)) {
      CopyMem (MemBuffer, (VOID *)Buffer, (UINTN)Size);
      GtConfig->GraphicsConfigPtr = MemBuffer;
    } else {
      DEBUG((DEBUG_WARN, "Error in locating / copying VBT.\n"));
      GtConfig->GraphicsConfigPtr = NULL;
    }

    GtConfig->PeiGraphicsPeimInit = 1;

    DEBUG((DEBUG_INFO, "Vbt Pointer from PeiGetSectionFromFv is 0x%x\n", GtConfig->GraphicsConfigPtr));
    DEBUG((DEBUG_INFO, "Vbt Size from PeiGetSectionFromFv is 0x%x\n", Size));

    PeiGetSectionFromFv (gTianoLogoGuid, &Buffer, &Size);
    if (Buffer == NULL) {
      DEBUG((DEBUG_WARN, "Could not locate Logo\n"));
    }

    MemBuffer = (VOID *)AllocatePages (EFI_SIZE_TO_PAGES ((UINTN)Size));
    if ((MemBuffer != NULL) && (Buffer != NULL)) {
      CopyMem (MemBuffer, (VOID *)Buffer, (UINTN)Size);
      GtConfig->LogoPtr = MemBuffer;
      GtConfig->LogoSize = Size;

      //
      // Calculate the BltBuffer needed size.
      //
      BmpHeader = (BMP_IMAGE_HEADER *) GtConfig->LogoPtr;

      if (BmpHeader->CharB == 'B' && BmpHeader->CharM == 'M') {
        BltBufferSize = MultU64x32 ((UINT64) BmpHeader->PixelWidth, BmpHeader->PixelHeight);
        if (BltBufferSize < DivU64x32 ((UINTN) ~0, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) {
          BltBufferSize = MultU64x32 (BltBufferSize, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
          GtConfig->BltBufferSize    = (UINT32) BltBufferSize;
          GtConfig->BltBufferAddress = (VOID *) AllocatePages (EFI_SIZE_TO_PAGES ((UINTN)GtConfig->BltBufferSize));
        } else {
          DEBUG ((DEBUG_ERROR, "Blt Buffer Size overflow.\n"));
          ASSERT (FALSE);
        }
      } else {
        DEBUG ((DEBUG_ERROR, "Wrong Bmp Image Header.\n"));
        ASSERT (FALSE);
      }

    } else {
      DEBUG((DEBUG_WARN, "Error in locating / copying LogoPtr.\n"));
      GtConfig->LogoPtr = NULL;
      GtConfig->LogoSize = 0;
    }

    DEBUG((DEBUG_INFO, "LogoPtr from PeiGetSectionFromFv is 0x%x\n", GtConfig->LogoPtr));
    DEBUG((DEBUG_INFO, "LogoSize from PeiGetSectionFromFv is 0x%x\n", GtConfig->LogoSize));

    //
    // Display DDI Initialization ( default Native GPIO as per board during AUTO case)
    //
    if (PcdGet32 (PcdSaDisplayConfigTable) != 0) {
      CopyMem (SaDisplayConfigTable, (VOID *) (UINTN) PcdGet32 (PcdSaDisplayConfigTable), (UINTN)PcdGet16 (PcdSaDisplayConfigTableSize));
      GtConfig->DdiConfiguration.DdiPortEdp     = SaDisplayConfigTable[0];
      GtConfig->DdiConfiguration.DdiPortBHpd    = SaDisplayConfigTable[1];
      GtConfig->DdiConfiguration.DdiPortCHpd    = SaDisplayConfigTable[2];
      GtConfig->DdiConfiguration.DdiPortDHpd    = SaDisplayConfigTable[3];
      GtConfig->DdiConfiguration.DdiPortFHpd    = SaDisplayConfigTable[4];
      GtConfig->DdiConfiguration.DdiPortBDdc    = SaDisplayConfigTable[5];
      GtConfig->DdiConfiguration.DdiPortCDdc    = SaDisplayConfigTable[6];
      GtConfig->DdiConfiguration.DdiPortDDdc    = SaDisplayConfigTable[7];
      GtConfig->DdiConfiguration.DdiPortFDdc    = SaDisplayConfigTable[8];
    }
  }

  PciePeiConfig->DmiAspm = 0x3;

  return EFI_SUCCESS;
}

/**
  PeiGetSectionFromFv finds the file in FV and gets file Address and Size

  @param[in] NameGuid              - File GUID
  @param[out] Address              - Pointer to the File Address
  @param[out] Size                 - Pointer to File Size

  @retval EFI_SUCCESS                Successfull in reading the section from FV
**/
EFI_STATUS
EFIAPI
PeiGetSectionFromFv (
  IN CONST  EFI_GUID        NameGuid,
  OUT VOID                  **Address,
  OUT UINT32                *Size
  )
{
  EFI_STATUS                           Status;
  EFI_PEI_FIRMWARE_VOLUME_PPI          *FvPpi;
  EFI_FV_FILE_INFO                     FvFileInfo;
  PEI_CORE_INSTANCE                    *PrivateData;
  UINTN                                CurrentFv;
  PEI_CORE_FV_HANDLE                   *CoreFvHandle;
  EFI_PEI_FILE_HANDLE                  VbtFileHandle;
  EFI_GUID                             *VbtGuid;
  EFI_COMMON_SECTION_HEADER            *Section;
  CONST EFI_PEI_SERVICES               **PeiServices;

  PeiServices = GetPeiServicesTablePointer();

  PrivateData = PEI_CORE_INSTANCE_FROM_PS_THIS(PeiServices);

  Status = PeiServicesLocatePpi(
             &gEfiFirmwareFileSystem2Guid,
             0,
             NULL,
             (VOID **)&FvPpi
             );
  ASSERT_EFI_ERROR(Status);

  CurrentFv = PrivateData->CurrentPeimFvCount;
  CoreFvHandle = &(PrivateData->Fv[CurrentFv]);

  Status = FvPpi->FindFileByName(FvPpi, &NameGuid, &CoreFvHandle->FvHandle, &VbtFileHandle);
  if (!EFI_ERROR(Status) && VbtFileHandle != NULL) {

    DEBUG((DEBUG_INFO, "Find SectionByType \n"));

    Status = FvPpi->FindSectionByType(FvPpi, EFI_SECTION_RAW, VbtFileHandle, (VOID **)&VbtGuid);
    if (!EFI_ERROR(Status)) {

      DEBUG((DEBUG_INFO, "GetFileInfo \n"));

      Status = FvPpi->GetFileInfo(FvPpi, VbtFileHandle, &FvFileInfo);
      Section = (EFI_COMMON_SECTION_HEADER *)FvFileInfo.Buffer;

      if (IS_SECTION2(Section)) {
        ASSERT(SECTION2_SIZE(Section) > 0x00FFFFFF);
        *Size = SECTION2_SIZE(Section) - sizeof (EFI_COMMON_SECTION_HEADER2);
        *Address = ((UINT8 *)Section + sizeof (EFI_COMMON_SECTION_HEADER2));
      } else {
        *Size = SECTION_SIZE(Section) - sizeof (EFI_COMMON_SECTION_HEADER);
        *Address = ((UINT8 *)Section + sizeof (EFI_COMMON_SECTION_HEADER));
      }
    }
  }

  return EFI_SUCCESS;
}

