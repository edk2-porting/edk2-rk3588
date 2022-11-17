/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>

#include <Guid/MemoryTypeInformation.h>

CHAR8 *
ShortNameOfMemoryType(
  IN UINT32 Type
  );

VOID
DumpMemoryTypeInfoSummary (
  IN EFI_MEMORY_TYPE_INFORMATION *CurrentMemoryTypeInformation,
  IN EFI_MEMORY_TYPE_INFORMATION *PreviousMemoryTypeInformation
  )
{
  UINTN                        Index;
  UINTN                        Index1;
  EFI_BOOT_MODE                BootMode;
  UINT32                       Previous;
  UINT32                       Current;
  UINT32                       Next;
  BOOLEAN                      MemoryTypeInformationModified;

  MemoryTypeInformationModified = FALSE;
  BootMode = GetBootModeHob();

  //
  // Use a heuristic to adjust the Memory Type Information for the next boot
  //
  DEBUG ((DEBUG_INFO, "\n"));
  DEBUG ((DEBUG_INFO, "             (HOB)   (ConfTabl)  (Var)  \n"));
  DEBUG ((DEBUG_INFO, "  Memory    Previous  Current    Next   \n"));
  DEBUG ((DEBUG_INFO, "   Type      Pages     Pages     Pages  \n"));
  DEBUG ((DEBUG_INFO, "==========  ========  ========  ========\n"));

  for (Index = 0; PreviousMemoryTypeInformation[Index].Type != EfiMaxMemoryType; Index++) {

    for (Index1 = 0; CurrentMemoryTypeInformation[Index1].Type != EfiMaxMemoryType; Index1++) {
      if (PreviousMemoryTypeInformation[Index].Type == CurrentMemoryTypeInformation[Index1].Type) {
        break;
      }
    }
    if (CurrentMemoryTypeInformation[Index1].Type == EfiMaxMemoryType) {
      continue;
    }

    //
    // Previous is the number of pages pre-allocated
    // Current is the number of pages actually needed
    //
    Previous = PreviousMemoryTypeInformation[Index].NumberOfPages;
    Current = CurrentMemoryTypeInformation[Index1].NumberOfPages;
    Next = Previous;

    //
    // Inconsistent Memory Reserved across bootings may lead to S4 fail
    // Write next varible to 125% * current when the pre-allocated memory is:
    //  1. More than 150% of needed memory and boot mode is BOOT_WITH_DEFAULT_SETTING
    //  2. Less than the needed memory
    //
    if ((Current + (Current >> 1)) < Previous) {
      if (BootMode == BOOT_WITH_DEFAULT_SETTINGS) {
        Next = Current + (Current >> 2);
      }
    } else if (Current > Previous) {
      Next = Current + (Current >> 2);
    }
    if (Next > 0 && Next < 4) {
      Next = 4;
    }

    if (Next != Previous) {
      PreviousMemoryTypeInformation[Index].NumberOfPages = Next;
      MemoryTypeInformationModified = TRUE;
    }

    DEBUG ((DEBUG_INFO, ShortNameOfMemoryType(PreviousMemoryTypeInformation[Index].Type)));
    DEBUG ((DEBUG_INFO, "  %08x  %08x  %08x\n", Previous, Current, Next));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  if (MemoryTypeInformationModified) {
    DEBUG ((DEBUG_INFO, "MemoryTypeInformation - Modified. RESET Needed!\n"));
  } else {
    DEBUG ((DEBUG_INFO, "MemoryTypeInformation - Unmodified.\n"));
  }
  DEBUG ((DEBUG_INFO, "\n"));
}

EFI_STATUS
TestPointCheckMemoryTypeInformation (
  VOID
  )
{
  EFI_STATUS              Status;
  EFI_HOB_GUID_TYPE       *GuidHob;
  VOID                    *CurrentMemoryTypeInformation;
  VOID                    *PreviousMemoryTypeInformation;
  VOID                    *VariableMemoryTypeInformation;
  
  DEBUG ((DEBUG_INFO, "==== TestPointCheckMemoryTypeInformation - Enter\n"));
  CurrentMemoryTypeInformation = NULL;
  PreviousMemoryTypeInformation = NULL;

  Status = EfiGetSystemConfigurationTable (&gEfiMemoryTypeInformationGuid, &CurrentMemoryTypeInformation);
  if (EFI_ERROR(Status)) {
    goto Done;
  }

  GuidHob = GetFirstGuidHob (&gEfiMemoryTypeInformationGuid);
  if (GuidHob != NULL) {
    PreviousMemoryTypeInformation = GET_GUID_HOB_DATA(GuidHob);
  } else {
    Status = EFI_NOT_FOUND;
    goto Done;
  }

  GetVariable2 (
             EFI_MEMORY_TYPE_INFORMATION_VARIABLE_NAME,
             &gEfiMemoryTypeInformationGuid,
             &VariableMemoryTypeInformation,
             NULL
             );

  if ((CurrentMemoryTypeInformation != NULL) && (PreviousMemoryTypeInformation != NULL)) {
    DumpMemoryTypeInfoSummary(CurrentMemoryTypeInformation, PreviousMemoryTypeInformation);
  }
  DEBUG ((DEBUG_INFO, "==== TestPointCheckMemoryTypeInformation - Exit\n"));

Done:
  if (EFI_ERROR(Status)) {
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE4_READY_TO_BOOT_MEMORY_TYPE_INFORMATION_FUNCTIONAL_ERROR_CODE \
        TEST_POINT_READY_TO_BOOT \
        TEST_POINT_BYTE4_READY_TO_BOOT_MEMORY_TYPE_INFORMATION_FUNCTIONAL_ERROR_STRING
      );
  }
  return Status;
}
