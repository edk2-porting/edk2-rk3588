/** @file
  System reset library services.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/PmcLib.h>
#include <Protocol/PchReset.h>
#include <Register/PchRegsLpc.h>
#include <Register/PchRegsPmc.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT32           mDxeRuntimeResetSystemPwrmBase;
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
  @param[in] ResetTypeGuid        Pointer to an EFI_GUID, which is the Reset Type Guid.
**/
VOID
EFIAPI
PchResetCallback (
  IN  EFI_RESET_TYPE          ResetType,
  IN  EFI_GUID                *ResetTypeGuid
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
      return;
    }

    for (Index = 0; Index < NumHandles; Index++) {
      Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gPchResetCallbackProtocolGuid,
                      (VOID **) &PchResetCallback
                      );
      ASSERT_EFI_ERROR (Status);

      if (!EFI_ERROR (Status)) {
        PchResetCallback->ResetCallback (ResetType, ResetTypeGuid);
      }
    }
  }
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
  PchResetCallback (EfiResetCold, NULL);

  DumpResetMessage ();

  IoWrite8 (R_PCH_IO_RST_CNT, V_PCH_IO_RST_CNT_FULLRESET);
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
  PchResetCallback (EfiResetWarm, NULL);

  DumpResetMessage ();
  //
  // In case there are pending capsules to process, need to flush the cache.
  //
  AsmWbinvd ();

  IoWrite8 (R_PCH_IO_RST_CNT, V_PCH_IO_RST_CNT_HARDRESET);
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
  PchResetCallback (EfiResetShutdown, NULL);

  ///
  /// Firstly, GPE0_EN should be disabled to avoid any GPI waking up the system from S5
  ///
  IoWrite32 (mDxeRuntimeResetSystemABase + R_ACPI_IO_GPE0_EN_127_96, 0);

  ///
  /// Secondly, PwrSts register must be cleared
  ///
  /// Write a "1" to bit[8] of power button status register at
  /// (PM_BASE + PM1_STS_OFFSET) to clear this bit
  ///
  IoWrite16 (mDxeRuntimeResetSystemABase + R_ACPI_IO_PM1_STS, B_ACPI_IO_PM1_STS_PWRBTN);

  ///
  /// Finally, transform system into S5 sleep state
  ///
  Data32 = IoRead32 (mDxeRuntimeResetSystemABase + R_ACPI_IO_PM1_CNT);

  Data32 = (UINT32) ((Data32 &~(B_ACPI_IO_PM1_CNT_SLP_TYP + B_ACPI_IO_PM1_CNT_SLP_EN)) | V_ACPI_IO_PM1_CNT_S5);

  IoWrite32 (mDxeRuntimeResetSystemABase + R_ACPI_IO_PM1_CNT, Data32);

  Data32 = Data32 | B_ACPI_IO_PM1_CNT_SLP_EN;

  DumpResetMessage ();

  IoWrite32 (mDxeRuntimeResetSystemABase + R_ACPI_IO_PM1_CNT, Data32);

  return;
}

/**
  Internal function to execute the required HECI command for GlobalReset,
  if failed will use PCH Reest.

**/
STATIC
VOID
PchGlobalReset (
  VOID
  )
{
  //
  // Loop through callback functions of PchResetCallback Protocol
  //
  PchResetCallback (EfiResetPlatformSpecific, &gPchGlobalResetGuid);

  DumpResetMessage ();

  //
  // Let ME do global reset if Me Fw is available
  //
  if (!EfiAtRuntime ()) {
    //
    // PCH BIOS Spec Section 4.6 GPIO Reset Requirement
    //
    MmioOr32 (
      mDxeRuntimeResetSystemPwrmBase + R_PMC_PWRM_ETR3,
      (UINT32) B_PMC_PWRM_ETR3_CF9GR
      );
  }

  IoWrite8 (R_PCH_IO_RST_CNT, V_PCH_IO_RST_CNT_FULLRESET);
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
    PchGlobalReset ();
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
  The library constructuor.

  The function does the necessary initialization work for this library instance.

  The PMC PCI configuration and PWRM space memory ranges are converted into EFI_RUNTIME_XXX
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
  mDxeRuntimeResetSystemABase    = PmcGetAcpiBase ();
  mDxeRuntimeResetSystemPwrmBase = PmcGetPwrmBase ();

  return EFI_SUCCESS;
}

