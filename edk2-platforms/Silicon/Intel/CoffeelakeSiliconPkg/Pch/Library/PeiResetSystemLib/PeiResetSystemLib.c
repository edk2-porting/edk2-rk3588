/** @file
  System reset library services.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchResetLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/PmcLib.h>
#include <Private/Library/PmcPrivateLib.h>
#include <Ppi/PchReset.h>
#include <Register/PchRegsLpc.h>
#include <Register/PchRegsPmc.h>

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
  for (Index = 0; Index < 30; Index++) {
    DEBUG ((DEBUG_INFO, "*"));
  }
  DEBUG ((DEBUG_INFO, "\n**    SYSTEM REBOOT !!!     **\n"));
  for (Index = 0; Index < 30; Index++) {
    DEBUG ((DEBUG_INFO, "*"));
  }
  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG_CODE_END ();
}
/**
  Execute call back function for Pch Reset.

  @param[in] ResetType            Reset Types which includes GlobalReset.
  @param[in] ResetTypeGuid        Pointer to an EFI_GUID, which is the Reset Type Guid.
**/
VOID
PchResetCallback (
  IN  EFI_RESET_TYPE      ResetType,
  IN  EFI_GUID            *ResetTypeGuid
  )
{
  EFI_STATUS              Status;
  UINTN                   Instance;
  PCH_RESET_CALLBACK_PPI  *PchResetCallbackPpi;

  Instance = 0;
  do {
    Status = PeiServicesLocatePpi (
               &gPchResetCallbackPpiGuid,
               Instance,
               NULL,
               (VOID **) &PchResetCallbackPpi
               );

    switch (Status) {
      case EFI_SUCCESS:
        PchResetCallbackPpi->ResetCallback (ResetType, ResetTypeGuid);
        break;
      case EFI_NOT_FOUND:
        break;
      default:
        ASSERT_EFI_ERROR (Status);
        break;
    }
    ++Instance;
  } while (Status == EFI_SUCCESS);
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
  // Loop through callback functions of PchResetCallback PPI
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
  // Loop through callback functions of PchResetCallback PPI
  //
  PchResetCallback (EfiResetWarm, NULL);
  DumpResetMessage ();

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
  UINT16         ABase;
  UINT32         Data32;

  //
  // Loop through callback functions of PchResetCallback PPI
  //
  PchResetCallback (EfiResetShutdown, NULL);

  ABase = PmcGetAcpiBase ();
  ///
  /// Firstly, GPE0_EN should be disabled to avoid any GPI waking up the system from S5
  ///
  IoWrite32 ((UINTN) (ABase + R_ACPI_IO_GPE0_EN_127_96), 0);

  ///
  /// Secondly, PwrSts register must be cleared
  ///
  /// Write a "1" to bit[8] of power button status register at
  /// (PM_BASE + PM1_STS_OFFSET) to clear this bit
  ///
  IoWrite16 ((UINTN) (ABase + R_ACPI_IO_PM1_STS), B_ACPI_IO_PM1_STS_PWRBTN);

  ///
  /// Finally, transform system into S5 sleep state
  ///
  Data32 = IoRead32 ((UINTN) (ABase + R_ACPI_IO_PM1_CNT));

  Data32 = (UINT32) ((Data32 &~(B_ACPI_IO_PM1_CNT_SLP_TYP + B_ACPI_IO_PM1_CNT_SLP_EN)) | V_ACPI_IO_PM1_CNT_S5);

  IoWrite32 ((UINTN) (ABase + R_ACPI_IO_PM1_CNT), Data32);

  Data32 = Data32 | B_ACPI_IO_PM1_CNT_SLP_EN;

  DumpResetMessage ();

  IoWrite32 ((UINTN) (ABase + R_ACPI_IO_PM1_CNT), Data32);

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
  // Loop through callback functions of PchResetCallback PPI
  //
  PchResetCallback (EfiResetPlatformSpecific, &gPchGlobalResetGuid);

  //
  // PCH BIOS Spec Section 4.6 GPIO Reset Requirement
  //
  PmcEnableCf9GlobalReset ();

  DumpResetMessage ();

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
    DEBUG ((DEBUG_ERROR, "[PeiResetSystemLib] ResetData is not available.\n"));
    return;
  }
  GuidPtr = (EFI_GUID *) ((UINT8 *) ResetData + DataSize - sizeof (EFI_GUID));
  if (CompareGuid (GuidPtr, &gPchGlobalResetGuid)) {
    PchGlobalReset();
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

