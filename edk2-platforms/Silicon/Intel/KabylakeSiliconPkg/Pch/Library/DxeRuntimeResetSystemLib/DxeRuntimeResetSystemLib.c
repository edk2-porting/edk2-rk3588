/** @file
  System reset library services.

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <Base.h>
#include <PchAccess.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MmPciLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Protocol/PchReset.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINTN            mDxeRuntimeResetSystemPmcBase;
GLOBAL_REMOVE_IF_UNREFERENCED UINT16           mDxeRuntimeResetSystemABase;

/**
  Dump reset message for debug build readability
**/
VOID
DumpResetMessage (
  VOID
  )
{
  DEBUG_CODE_BEGIN ();
  UINTN       Index;
  //
  // ******************************
  // **    SYSTEM REBOOT !!!     **
  // ******************************
  //
  if (!EfiAtRuntime ()) {
    for (Index = 0; Index < 30; Index++) {
      DEBUG ((DEBUG_INFO, "*"));
    }
    DEBUG ((DEBUG_INFO, "\n**    SYSTEM REBOOT !!!     **\n"));
    for (Index = 0; Index < 30; Index++) {
      DEBUG ((DEBUG_INFO, "*"));
    }
    DEBUG ((DEBUG_INFO, "\n"));
  }
  DEBUG_CODE_END ();
}

/**
  Execute call back function for Pch Reset.

  @param[in] ResetType            Reset Types which includes GlobalReset.

  @retval EFI_SUCCESS             The callback function has been done successfully
  @retval EFI_NOT_FOUND           Failed to find Pch Reset Callback protocol. Or, none of
                                  callback protocol is installed.
**/
EFI_STATUS
EFIAPI
PchResetCallback (
  IN  PCH_RESET_TYPE          ResetType
  )
{
  EFI_STATUS                  Status;
  UINTN                       NumHandles;
  EFI_HANDLE                  *HandleBuffer;
  UINTN                       Index;
  PCH_RESET_CALLBACK_PROTOCOL *PchResetCallback;

  if (!EfiAtRuntime ()) {
    ///
    /// Retrieve all instances of Pch Reset Callback protocol
    ///
    Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gPchResetCallbackProtocolGuid,
                    NULL,
                    &NumHandles,
                    &HandleBuffer
                    );
    if (EFI_ERROR (Status)) {
      ///
      /// Those drivers that need to install Pch Reset Callback protocol have the responsibility
      /// to make sure themselves execute before Pch Reset Runtime driver.
      ///
      if (Status == EFI_NOT_FOUND) {
        DEBUG ((DEBUG_ERROR | DEBUG_INFO, "None of Pch Reset Callback protocol is installed.\n"));
      }
      return Status;
    }

    for (Index = 0; Index < NumHandles; Index++) {
      Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gPchResetCallbackProtocolGuid,
                      (VOID **) &PchResetCallback
                      );
      ASSERT_EFI_ERROR (Status);

      if (!EFI_ERROR (Status)) {
        PchResetCallback->ResetCallback (ResetType);
      } else {
        DEBUG ((DEBUG_ERROR | DEBUG_INFO, "Failed to locate Pch Reset Callback protocol.\n"));
        return Status;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Calling this function causes a system-wide reset. This sets
  all circuitry within the system to its initial state. This type of reset
  is asynchronous to system operation and operates without regard to
  cycle boundaries.

  System reset should not return, if it returns, it means the system does
  not support cold reset.
**/
VOID
EFIAPI
ResetCold (
  VOID
  )
{
  //
  // Loop through callback functions of PchResetCallback Protocol
  //
  PchResetCallback ((PCH_RESET_TYPE) EfiResetCold);

  DumpResetMessage ();

  IoWrite8 (R_PCH_RST_CNT, V_PCH_RST_CNT_FULLRESET);
}

/**
  Calling this function causes a system-wide initialization. The processors
  are set to their initial state, and pending cycles are not corrupted.

  System reset should not return, if it returns, it means the system does
  not support warm reset.
**/
VOID
EFIAPI
ResetWarm (
  VOID
  )
{
  //
  // Loop through callback functions of PchResetCallback Protocol
  //
  PchResetCallback ((PCH_RESET_TYPE) EfiResetWarm);

  DumpResetMessage ();
  //
  // In case there are pending capsules to process, need to flush the cache.
  //
  AsmWbinvd ();

  IoWrite8 (R_PCH_RST_CNT, V_PCH_RST_CNT_HARDRESET);
}

/**
  Calling this function causes the system to enter a power state equivalent
  to the ACPI G2/S5 or G3 states.

  System shutdown should not return, if it returns, it means the system does
  not support shut down reset.
**/
VOID
EFIAPI
ResetShutdown (
  VOID
  )
{
  UINT32         Data32;

  //
  // Loop through callback functions of PchResetCallback Protocol
  //
  PchResetCallback ((PCH_RESET_TYPE) EfiResetShutdown);

  ///
  /// Firstly, GPE0_EN should be disabled to avoid any GPI waking up the system from S5
  ///
  IoWrite32 ((UINTN) (mDxeRuntimeResetSystemABase + R_PCH_ACPI_GPE0_EN_127_96), 0);

  ///
  /// Secondly, PwrSts register must be cleared
  ///
  /// Write a "1" to bit[8] of power button status register at
  /// (PM_BASE + PM1_STS_OFFSET) to clear this bit
  ///
  IoWrite16 ((UINTN) (mDxeRuntimeResetSystemABase + R_PCH_ACPI_PM1_STS), B_PCH_ACPI_PM1_STS_PWRBTN);

  ///
  /// Finally, transform system into S5 sleep state
  ///
  Data32 = IoRead32 ((UINTN) (mDxeRuntimeResetSystemABase + R_PCH_ACPI_PM1_CNT));

  Data32 = (UINT32) ((Data32 &~(B_PCH_ACPI_PM1_CNT_SLP_TYP + B_PCH_ACPI_PM1_CNT_SLP_EN)) | V_PCH_ACPI_PM1_CNT_S5);

  IoWrite32 ((UINTN) (mDxeRuntimeResetSystemABase + R_PCH_ACPI_PM1_CNT), Data32);

  Data32 = Data32 | B_PCH_ACPI_PM1_CNT_SLP_EN;

  DumpResetMessage ();

  IoWrite32 ((UINTN) (mDxeRuntimeResetSystemABase + R_PCH_ACPI_PM1_CNT), Data32);

  return;
}

/**
  Internal function to execute the required HECI command for GlobalReset,
  if failed will use PCH Reest.

**/
STATIC
VOID
DxeRuntimePchGlobalReset (
  VOID
  )
{
  //
  // Loop through callback functions of PchResetCallback Protocol
  //
  PchResetCallback (PchGlobalReset);

  //
  // PCH BIOS Spec Section 4.6 GPIO Reset Requirement
  //
  MmioOr32 (mDxeRuntimeResetSystemPmcBase + R_PCH_PMC_ETR3, (UINT32) B_PCH_PMC_ETR3_CF9GR);

  DumpResetMessage ();

  IoWrite8 (R_PCH_RST_CNT, V_PCH_RST_CNT_FULLRESET);
}

/**
  Calling this function causes the system to enter a power state for platform specific.

  @param[in] DataSize             The size of ResetData in bytes.
  @param[in] ResetData            Optional element used to introduce a platform specific reset.
                                  The exact type of the reset is defined by the EFI_GUID that follows
                                  the Null-terminated Unicode string.

**/
VOID
EFIAPI
ResetPlatformSpecific (
  IN UINTN            DataSize,
  IN VOID             *ResetData OPTIONAL
  )
{
  EFI_GUID            *GuidPtr;

  if (ResetData == NULL) {
    if (!EfiAtRuntime ()) {
      DEBUG ((DEBUG_ERROR, "[DxeRuntimeResetSystemLib] ResetData is not available.\n"));
    }
    return;
  }
  GuidPtr = (EFI_GUID *) ((UINT8 *) ResetData + DataSize - sizeof (EFI_GUID));
  if (CompareGuid (GuidPtr, &gPchGlobalResetGuid)) {
    DxeRuntimePchGlobalReset();
  } else {
    return;
  }
}

/**
  Calling this function causes the system to enter a power state for capsule update.

  Reset update should not return, if it returns, it means the system does
  not support capsule update.

**/
VOID
EFIAPI
EnterS3WithImmediateWake (
  VOID
  )
{
  ASSERT (FALSE);
}

/**
  The ResetSystem function resets the entire platform.

  @param[in] ResetType      The type of reset to perform.
  @param[in] ResetStatus    The status code for the reset.
  @param[in] DataSize       The size, in bytes, of ResetData.
  @param[in] ResetData      For a ResetType of EfiResetCold, EfiResetWarm, or EfiResetShutdown
                            the data buffer starts with a Null-terminated string, optionally
                            followed by additional binary data. The string is a description
                            that the caller may use to further indicate the reason for the
                            system reset.
**/
VOID
EFIAPI
ResetSystem (
  IN EFI_RESET_TYPE               ResetType,
  IN EFI_STATUS                   ResetStatus,
  IN UINTN                        DataSize,
  IN VOID                         *ResetData OPTIONAL
  )
{
  switch (ResetType) {
  case EfiResetWarm:
    ResetWarm ();
    break;

  case EfiResetCold:
    ResetCold ();
    break;

  case EfiResetShutdown:
    ResetShutdown ();
    return;

  case EfiResetPlatformSpecific:
    ResetPlatformSpecific (DataSize, ResetData);
    return;

  default:
    return;
  }
}

/**
  Convert the physical PMC base addresses to virtual addresses.

  @param[in]    Event   The event that is being processed.
  @param[in]    Context The Event Context.
**/
VOID
EFIAPI
ResetSystemLibVirtualAddressChangeEvent (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EfiConvertPointer (0x0, (VOID **) &mDxeRuntimeResetSystemPmcBase);
}

/**
  The library constructuor.

  The function does the necessary initialization work for this library instance.

  The PMC PCI configuration space memory range is converted into EFI_RUNTIME_XXX
  in PciHostBridgeEntryPoint

  @param[in]  ImageHandle       The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable       A pointer to the EFI system table.

  @retval     EFI_SUCCESS       The function always return EFI_SUCCESS for now.
                                It will ASSERT on error for debug version.
  @retval     EFI_ERROR         Please reference LocateProtocol for error code details.
**/
EFI_STATUS
EFIAPI
DxeRuntimeResetSystemLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_EVENT                       AddressChangeEvent;

  //
  // Get ACPI Base Address and PMC Base Address
  //
  PchAcpiBaseGet (&mDxeRuntimeResetSystemABase);

  mDxeRuntimeResetSystemPmcBase = MmPciBase (
                                    DEFAULT_PCI_BUS_NUMBER_PCH,
                                    PCI_DEVICE_NUMBER_PCH_PMC,
                                    PCI_FUNCTION_NUMBER_PCH_PMC
                                    );
  //
  // Create Address Change event
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  ResetSystemLibVirtualAddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &AddressChangeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

