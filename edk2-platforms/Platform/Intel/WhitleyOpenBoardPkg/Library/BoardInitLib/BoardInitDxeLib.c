/** @file
Library for Board Init.

@copyright
Copyright (c) 2017 - 2021, Intel Corporation. All rights reserved.<BR>

SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Guid/SetupVariable.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Protocol/MpService.h>
#include <Protocol/ReportStatusCodeHandler.h>
#include <IioUniversalData.h>
#include <PchAccess.h>
#include <Protocol/DynamicSiLibraryProtocol.h>
#include <Cpu/CpuIds.h>

IIO_UDS             *mIioUds;

/**
  Connects Root Bridge
**/
VOID
ConnectRootBridge (
  BOOLEAN Recursive
  );


VOID
ProgramDPRregs (
  VOID
);

/**
  A hook for board-specific initialization after PCI enumeration.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitAfterPciEnumeration (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  A hook for board-specific functionality for the ReadyToBoot event.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitReadyToBoot (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  A hook for board-specific functionality for the ExitBootServices event.

  @retval EFI_SUCCESS   The board initialization was successful.
  @retval EFI_NOT_READY The board has not been detected yet.
**/
EFI_STATUS
EFIAPI
BoardInitEndOfFirmware (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This function will retrieve the DPR data from HOBs produced by MRC
  and will use it to program the DPR registers in IIO and in PCH

  @param  VOID
  @retval VOID

**/
VOID
ProgramDprRegs (
  VOID
  )
{
  return;
}

/**
  Function to set the WPE bit of the BIOS Info Flags MSR to enable Anti-Flash wearout
  protection within BIOS Guard before booting to the OS

  @param[in] EFI_EVENT        Event
  @param[in] VOID             *Context

  @retval    None

**/
VOID
EFIAPI
EnableAntiFlashWearout (
  EFI_EVENT       Event,
  VOID            *Context
  )
{
  EFI_STATUS                    Status;
  EFI_MP_SERVICES_PROTOCOL      *MpServices = NULL;
  SYSTEM_CONFIGURATION          SetupData;
  DYNAMIC_SI_LIBARY_PROTOCOL    *DynamicSiLibraryProtocol = NULL;

  Status = gBS->LocateProtocol (&gDynamicSiLibraryProtocolGuid, NULL, &DynamicSiLibraryProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  CopyMem (&SetupData, PcdGetPtr(PcdSetup), sizeof(SYSTEM_CONFIGURATION));

  //
  // First check if Anti-flash wearout feature is supported by platform and Setup variable is enabled
  //
  if (SetupData.AntiFlashWearoutSupported == TRUE && SetupData.EnableAntiFlashWearout) {
    Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **) &MpServices
                  );
    ASSERT_EFI_ERROR(Status);

    //
    // Set WPE on BSP, then all other APs
    //
    DynamicSiLibraryProtocol->SetBiosInfoFlagWpe();

    MpServices->StartupAllAPs (
                    MpServices,
                    (EFI_AP_PROCEDURE) DynamicSiLibraryProtocol->SetBiosInfoFlagWpe,
                    FALSE,
                    NULL,
                    0,
                    NULL,
                    NULL
                    );
  }
}

/**
  Before console after trusted console event callback

  @param[in] Event      The Event this notify function registered to.
  @param[in] Context    Pointer to the context data registered to the Event.
**/
VOID
BdsBoardBeforeConsoleAfterTrustedConsoleCallback (
  IN EFI_EVENT          Event,
  IN VOID               *Context
  )
{
  EFI_STATUS          Status;
  VOID                *Interface;

  DEBUG ((DEBUG_INFO, "Board gBdsEventBeforeConsoleBeforeEndOfDxeGuid callback starts\n"));
  //
  // make sure root bridge is already connected before EndOfDxe.
  // Try to locate gEfiPciEnumerationCompleteProtocolGuid to see if PciBus scan already executed.
  //
  Status = gBS->LocateProtocol (
                  &gEfiPciEnumerationCompleteProtocolGuid,
                  NULL,
                  &Interface
                  );
  if (EFI_ERROR (Status)) {
    ConnectRootBridge (FALSE);
  }
}

EFI_STATUS
EFIAPI
BoardNotificationInit (
  VOID
  )
{
  EFI_STATUS                   Status;
  EFI_EVENT                    EndOfDxeEvent;
  UINT32                       BspCpuidSignature;
  UINT32                       RegEax, RegEbx, RegEcx, RegEdx;
  EFI_HOB_GUID_TYPE            *GuidHob;
  IIO_UDS                      *UdsHobPtr;
  EFI_BOOT_MODE                BootMode;
  EFI_GUID                     UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  EFI_EVENT                    BeforeConsoleAfterTrustedConsoleEvent;
  DYNAMIC_SI_LIBARY_PROTOCOL   *DynamicSiLibraryProtocol = NULL;

  Status = gBS->LocateProtocol (&gDynamicSiLibraryProtocolGuid, NULL, &DynamicSiLibraryProtocol);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  DEBUG((DEBUG_INFO, "PlatformEarlyDxeEntry \n"));

  //
  // Get the IIO_UDS data HOB
  //
  GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
  ASSERT(GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  UdsHobPtr = GET_GUID_HOB_DATA(GuidHob);
  //
  // Allocate Memory Pool buffer for IIO_UDS data
  //
  Status = gBS->AllocatePool ( EfiBootServicesData, sizeof (IIO_UDS), (VOID **) &mIioUds );
  ASSERT_EFI_ERROR (Status);
  //
  // Initialize the Memory Pool buffer with the data from the Hand-Off-Block
  //
  CopyMem(mIioUds, UdsHobPtr, sizeof(IIO_UDS));

  //
  // Get the boot mode that we are currently in
  //
  BootMode = GetBootModeHob();

  //
  // Program DPR registers with the range from Memory Init
  //
  ProgramDprRegs ();

  //
  // Program the GenProtRange registers for BIOS Guard
  //
  DynamicSiLibraryProtocol->ProgramGenProtRangeRegs (mIioUds);

  //
  // Program the IMR registers for ME IMR region
  //
  DynamicSiLibraryProtocol->ProgramImrRegs (mIioUds);

  //
  // Program the IMR2 registers for CPM & nCPM IMR region
  //
  DynamicSiLibraryProtocol->ProgramImr2Regs (mIioUds);

  //
  // Get BSP CPU ID
  // Shift out the stepping
  //
  AsmCpuid (0x01, &RegEax, &RegEbx, &RegEcx, &RegEdx);
  BspCpuidSignature = (RegEax >> 4) & 0x0000FFFF;
  if ( (BspCpuidSignature == CPU_FAMILY_SKX) && (BootMode != BOOT_ON_FLASH_UPDATE)) {
    //
    // Register event to set WPE bit in Bios Info Flags MSR to enable Anti Flash wearout
    //
    Status = gBS->CreateEventEx (
                EVT_NOTIFY_SIGNAL,
                TPL_CALLBACK,
                EnableAntiFlashWearout,
                NULL,
                &gEfiEndOfDxeEventGroupGuid,
                &EndOfDxeEvent
                );
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Create BeforeConsoleAfterTrustedConsole event callback
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  BdsBoardBeforeConsoleAfterTrustedConsoleCallback,
                  NULL,
                  &gBdsEventBeforeConsoleAfterTrustedConsoleGuid,
                  &BeforeConsoleAfterTrustedConsoleEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
