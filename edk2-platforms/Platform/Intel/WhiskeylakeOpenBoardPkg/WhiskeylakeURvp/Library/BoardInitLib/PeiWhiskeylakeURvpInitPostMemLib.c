/** @file

  Copyright (c) 2019 - 2020 Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <SaPolicyCommon.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HdaVerbTableLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PciLib.h>
#include <Library/PeiSaPolicyLib.h>
#include <Library/BoardInitLib.h>
#include <PchAccess.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioLib.h>
#include <GpioPinsSklLp.h>
#include <GpioPinsSklH.h>
#include <Library/GpioExpanderLib.h>
#include <SioRegs.h>
#include <Library/PchPcrLib.h>
#include <IoExpander.h>
#include <AttemptUsbFirst.h>
#include <PeiPlatformHookLib.h>
#include <Library/PeiPolicyInitLib.h>
#include <Library/PchInfoLib.h>
#include <FirwmareConfigurations.h>
#include "WhiskeylakeURvpInit.h"
#include <Library/ConfigBlockLib.h>

/**
  GPIO init function for PEI post memory phase.

  @param[in]  BoardId       An unsigned integrer represent the board id.

  @retval     EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
BoardGpioInit(
  IN UINT16 BoardId
  )
{
  //
  // GPIO Table Init.
  //
  switch (BoardId) {

    case BoardIdWhiskeyLakeRvp:
      PcdSet32S (PcdBoardGpioTable, (UINTN) mGpioTableWhlUDdr4_0);
      PcdSet16S (PcdBoardGpioTableSize, mGpioTableWhlUDdr4_0Size);
      PcdSet32S (PcdBoardGpioTable2, (UINTN) mGpioTableWhlUDdr4);
      PcdSet16S (PcdBoardGpioTable2Size, mGpioTableWhlUDdr4Size);
      break;

    default:
      DEBUG ((DEBUG_INFO, "For Unknown Board ID..Use Default GPIO Table...\n"));
      PcdSet32S (PcdBoardGpioTable, (UINTN) mGpioTableDefault);
      PcdSet16S (PcdBoardGpioTableSize, mGpioTableDefaultSize);
      break;
  }

  return EFI_SUCCESS;
}

/**
  Touch panel GPIO init function for PEI post memory phase.

  @param[in]  BoardId       An unsigned integrer represent the board id.

  @retval     EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
TouchPanelGpioInit (
  IN UINT16 BoardId
  )
{
  switch (BoardId) {
    default:
      PcdSet32S (PcdBoardGpioTableTouchPanel, 0);
    break;
  }
  return EFI_SUCCESS;
}

/**
  Misc. init function for PEI post memory phase.

  @param[in]  BoardId       An unsigned integrer represent the board id.

  @retval     EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
BoardMiscInit (
  IN UINT16 BoardId
  )
{
  PcdSetBoolS (PcdDebugUsbUartEnable, FALSE);

  switch (BoardId) {

    case BoardIdWhiskeyLakeRvp:

      PcdSetBoolS (PcdMipiCamGpioEnable, TRUE);
      break;

    default:
      PcdSetBoolS (PcdMipiCamGpioEnable, FALSE);
      break;
  }

  return EFI_SUCCESS;
}

/**
  Security GPIO init function for PEI post memory phase.

  @param[in]  BoardId       An unsigned integrer represent the board id.

  @retval     EFI_SUCCESS   The function completed successfully.
**/
EFI_STATUS
BoardSecurityInit (
  IN UINT16 BoardId
  )
{
  switch (BoardId) {
    case BoardIdWhiskeyLakeRvp:
      // TPM interrupt connects to GPIO_CNL_H_GPP_A_7
      PcdSet32S (PcdTpm2CurrentIrqNum, 0x1F);
      break;
  }

  return EFI_SUCCESS;
}

/**
  Board configuration initialization in the post-memory boot phase.
**/
VOID
BoardConfigInit (
  VOID
  )
{
  EFI_STATUS  Status;
  UINT16      BoardId;

  BoardId = BoardIdWhiskeyLakeRvp;

  Status = BoardGpioInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = TouchPanelGpioInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = HdaVerbTableInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = BoardMiscInit (BoardId);
  ASSERT_EFI_ERROR (Status);

  Status = BoardSecurityInit (BoardId);
  ASSERT_EFI_ERROR (Status);
}

/**
  Create the HOB for hotkey status for 'Attempt USB First' feature

  @retval  EFI_SUCCESS  HOB Creating successful.
  @retval  Others       HOB Creating failed.
**/
EFI_STATUS
CreateAttemptUsbFirstHotkeyInfoHob (
  VOID
  )
{
  EFI_STATUS                     Status;
  ATTEMPT_USB_FIRST_HOTKEY_INFO  AttemptUsbFirstHotkeyInfo;

  Status = EFI_SUCCESS;

  ZeroMem (
    &AttemptUsbFirstHotkeyInfo,
    sizeof (AttemptUsbFirstHotkeyInfo)
    );

  AttemptUsbFirstHotkeyInfo.RevisonId = 0;
  AttemptUsbFirstHotkeyInfo.HotkeyTriggered = FALSE;

  ///
  /// Build HOB for Attempt USB First feature
  ///
  BuildGuidDataHob (
    &gAttemptUsbFirstHotkeyInfoHobGuid,
    &(AttemptUsbFirstHotkeyInfo),
    sizeof (ATTEMPT_USB_FIRST_HOTKEY_INFO)
    );

  return Status;
}

/**
  Search and identify the physical address of a
  file module inside the FW_BINARIES_FV_SIGNED FV

  @retval  EFI_SUCCESS  If address has been found
  @retval  Others       If address has not been found
**/
EFI_STATUS
FindModuleInFlash2 (
  IN EFI_FIRMWARE_VOLUME_HEADER *FvHeader,
  IN EFI_GUID                   *GuidPtr,
  IN OUT UINT32                 *ModulePtr,
  IN OUT UINT32                 *ModuleSize
  )
{
  EFI_FFS_FILE_HEADER        *FfsHeader;
  EFI_FV_FILE_INFO           FileInfo;
  EFI_PEI_FILE_HANDLE        FileHandle;
  EFI_COMMON_SECTION_HEADER  *SectionHeader;
  VOID                       *FileBuffer;
  EFI_STATUS                 Status;

  FfsHeader = NULL;
  FileHandle = NULL;
  SectionHeader = NULL;
  FileBuffer = NULL;

  while (TRUE) {
    //
    // Locate FV_IMAGE file type in the FW_BINARIES_FV_SIGNED firmware volume
    //
    Status = PeiServicesFfsFindNextFile (EFI_FV_FILETYPE_FIRMWARE_VOLUME_IMAGE, FvHeader, &FileHandle);
    if (EFI_ERROR (Status)) {
      // unable to find FV_IMAGE file in this FV
      break;
    }

    FfsHeader = (EFI_FFS_FILE_HEADER*)FileHandle;
    DEBUG ((DEBUG_INFO, "FfsHeader 0x%X:\n", FfsHeader));
    DEBUG ((DEBUG_INFO, " Name = 0x%g\n", &FfsHeader->Name));
    DEBUG ((DEBUG_INFO, " Type = 0x%X\n", FfsHeader->Type));
    if (IS_FFS_FILE2 (FfsHeader)) {
      DEBUG ((DEBUG_INFO, " Size = 0x%X\n", FFS_FILE2_SIZE(FfsHeader)));
    }
    else {
      DEBUG ((DEBUG_INFO, " Size = 0x%X\n", FFS_FILE_SIZE(FfsHeader)));
    }

    //
    // Locate FW_BINARIES_FV FV_IMAGE Section
    //
    Status = PeiServicesFfsFindSectionData (EFI_SECTION_FIRMWARE_VOLUME_IMAGE, FileHandle, &FileBuffer);
    if (EFI_ERROR (Status)) {
      // continue to search for the next FV_IMAGE file
      DEBUG ((DEBUG_INFO, "FW_BINARIES_FV section not found. Status = %r\n", Status));
      continue;
    }

    SectionHeader = (EFI_COMMON_SECTION_HEADER *)FileBuffer;
    DEBUG ((DEBUG_INFO, "GUIDED SectionHeader 0x%X:\n",
    (UINT32)(UINT8 *)SectionHeader));
    if (IS_SECTION2(SectionHeader)) {
      DEBUG ((DEBUG_INFO, " Guid      = 0x%g\n",
        &((EFI_GUID_DEFINED_SECTION2 *)SectionHeader)->SectionDefinitionGuid));
      DEBUG ((DEBUG_INFO, " DataOfset = 0x%X\n",
        ((EFI_GUID_DEFINED_SECTION2 *)SectionHeader)->DataOffset));
    }
    else {
      DEBUG ((DEBUG_INFO, " Guid      = 0x%g\n",
        &((EFI_GUID_DEFINED_SECTION *)SectionHeader)->SectionDefinitionGuid));
      DEBUG ((DEBUG_INFO, " DataOfset = 0x%X\n",
        ((EFI_GUID_DEFINED_SECTION *)SectionHeader)->DataOffset));
    }
    DEBUG ((DEBUG_INFO, " Type      = 0x%X\n", SectionHeader->Type));

    //
    // Locate Firmware File System file within Firmware Volume
    //
    Status = PeiServicesFfsFindFileByName (GuidPtr, FileBuffer, (VOID **)&FfsHeader);
    if (EFI_ERROR (Status)) {
      // continue to search for the next FV_IMAGE file
      DEBUG ((DEBUG_INFO, "Module not found. Status = %r\n", Status));
      continue;
    }

    *ModulePtr = (UINT32)((UINT8 *)FfsHeader + sizeof(EFI_FFS_FILE_HEADER));

    //
    // Get File Information
    //
    Status = PeiServicesFfsGetFileInfo (FfsHeader, &FileInfo);
    if (!EFI_ERROR (Status)) {
      *ModuleSize = (UINT32)FileInfo.BufferSize;
      DEBUG ((DEBUG_INFO, "Module {0x%g} found at = 0x%X, Size = 0x%X\n",
        &FfsHeader->Name, *ModulePtr, *ModuleSize));
      return Status;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Get the ChipsetInit Binary pointer.

  @retval EFI_SUCCESS               - ChipsetInit Binary found.
  @retval EFI_NOT_FOUND             - ChipsetInit Binary not found.
**/
EFI_STATUS
UpdateChipsetInitPtr (
  VOID
  )
{
  EFI_STATUS                    Status;
  PCH_STEPPING                  PchStep;
  EFI_FIRMWARE_VOLUME_HEADER    *FvHeader;
  EFI_GUID                      *ChipsetInitBinaryGuidPtr;
  SI_POLICY_PPI                 *SiPolicyPpi;
  PCH_HSIO_CONFIG               *HsioConfig;
  UINT32                        ModuleAddr;
  UINT32                        ModuleSize;

  ModuleAddr = 0;
  ModuleSize = 0;
  PchStep = PchStepping ();

  Status = PeiServicesLocatePpi (
    &gSiPolicyPpiGuid,
    0,
    NULL,
    (VOID **)&SiPolicyPpi
    );
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SiPolicyPpi, &gHsioConfigGuid, (VOID *)&HsioConfig);
  ASSERT_EFI_ERROR (Status);

  ChipsetInitBinaryGuidPtr = NULL;
  if (IsPchLp()) {
    switch (PchStep) {
      case PCH_D0:
      case PCH_D1:
        ChipsetInitBinaryGuidPtr = &gCnlPchLpChipsetInitTableDxGuid;
        DEBUG ((DEBUG_INFO, "Using CnlPchLpChipsetInitTable_Dx table \n"));
        break;
      default:
        return EFI_NOT_FOUND;
    }
  } else {
    return EFI_NOT_FOUND;
  }

  //
  // Locate Firmware Volume header
  //
  FvHeader = (EFI_FIRMWARE_VOLUME_HEADER *) FixedPcdGet32 (PcdFlashFvPostMemoryBase);
  Status = FindModuleInFlash2 (FvHeader, ChipsetInitBinaryGuidPtr, &ModuleAddr, &ModuleSize);
  //
  // Get ChipsetInit Binary Pointer
  //
  HsioConfig->ChipsetInitBinPtr = ModuleAddr;

  //
  // Get File Size
  //
  HsioConfig->ChipsetInitBinLen = ModuleSize;

  DEBUG ((DEBUG_INFO, "ChipsetInit Binary Location: %x\n", HsioConfig->ChipsetInitBinPtr));
  DEBUG ((DEBUG_INFO, "ChipsetInit Binary Size: %x\n", HsioConfig->ChipsetInitBinLen));

  return Status;
}

/**
  Configure GPIO and SIO

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
EFIAPI
WhiskeylakeURvpBoardInitBeforeSiliconInit (
  VOID
  )
{
  EFI_STATUS      Status;
  UINT8           FwConfig;

  BoardConfigInit ();
  //
  // Configure GPIO and SIO
  //
  Status = BoardInit ();
  ASSERT_EFI_ERROR (Status);

  FwConfig = FwConfigProduction;
  PeiPolicyInit (FwConfig);

  //
  // Create USB Boot First hotkey information HOB
  //
  CreateAttemptUsbFirstHotkeyInfoHob ();

  //
  // Initializing Platform Specific Programming
  //
  Status = PlatformSpecificInit ();
  ASSERT_EFI_ERROR(Status);

  //
  // Update ChipsetInitPtr
  //
  Status = UpdateChipsetInitPtr ();

  ///
  /// Do Late PCH init
  ///
  LateSiliconInit ();

  return EFI_SUCCESS;
}
