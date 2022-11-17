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
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HstiLib.h>
#include <IndustryStandard/Hsti.h>

/**
  Dump HSTI info

  @param[in] HstiData     - Pointer to HSTI data
**/
VOID
DumpHsti (
  IN VOID                     *HstiData
  )
{
  ADAPTER_INFO_PLATFORM_SECURITY *Hsti;
  UINT8                          *SecurityFeatures;
  CHAR16                         *ErrorString;
  UINTN                          Index;
  CHAR16                         ErrorChar;

  Hsti = HstiData;
  DEBUG ((DEBUG_INFO, "HSTI\n"));
  DEBUG ((DEBUG_INFO, "  Version                     - 0x%08x\n", Hsti->Version));
  DEBUG ((DEBUG_INFO, "  Role                        - 0x%08x\n", Hsti->Role));
  DEBUG ((DEBUG_INFO, "  ImplementationID            - %S\n", Hsti->ImplementationID));
  DEBUG ((DEBUG_INFO, "  SecurityFeaturesSize        - 0x%08x\n", Hsti->SecurityFeaturesSize));

  SecurityFeatures = (UINT8 *) (Hsti + 1);
  DEBUG ((DEBUG_INFO, "  SecurityFeaturesImplemented - "));
  for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
    DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  SecurityFeatures = (UINT8 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
  DEBUG ((DEBUG_INFO, "  SecurityFeaturesVerified    - "));
  for (Index = 0; Index < Hsti->SecurityFeaturesSize; Index++) {
    DEBUG ((DEBUG_INFO, "%02x ", SecurityFeatures[Index]));
  }
  DEBUG ((DEBUG_INFO, "\n"));

  ErrorString = (CHAR16 *) (SecurityFeatures + Hsti->SecurityFeaturesSize);
  DEBUG ((DEBUG_INFO, "  ErrorString                 - \""));
  CopyMem (&ErrorChar, ErrorString, sizeof (ErrorChar));
  for (; ErrorChar != 0;) {
    DEBUG ((DEBUG_INFO, "%c", ErrorChar));
    ErrorString++;
    CopyMem (&ErrorChar, ErrorString, sizeof (ErrorChar));
  }
  DEBUG ((DEBUG_INFO, "\"\n"));
}

UINT32                  mRole[] = {
  PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
  PLATFORM_SECURITY_ROLE_PLATFORM_IBV,
  PLATFORM_SECURITY_ROLE_IMPLEMENTOR_OEM,
  PLATFORM_SECURITY_ROLE_IMPLEMENTOR_ODM,
};

EFI_STATUS
TestPointCheckHsti (
  VOID
  )
{
  VOID        *Hsti;
  UINTN       HstiSize;
  EFI_STATUS  Status;
  UINTN       Index;
  BOOLEAN     Result;
  
  Result = TRUE;
  DEBUG ((DEBUG_INFO, "==== TestPointCheckHsti - Enter\n"));
  for (Index = 0; Index < sizeof(mRole)/sizeof(mRole[0]); Index++) {
    Status = HstiLibGetTable (mRole[Index], NULL, &Hsti, &HstiSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HSTI (Role - 0x%08x) not found!\n", mRole[Index]));
      if (mRole[Index] <= PLATFORM_SECURITY_ROLE_PLATFORM_IBV) {
        Result = FALSE;
      }
      continue;
    }
    DumpHsti (Hsti);
    FreePool (Hsti);
  }
  DEBUG ((DEBUG_INFO, "==== TestPointCheckHsti - Exit\n"));

  if (!Result) {
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE8_READY_TO_BOOT_HSTI_TABLE_FUNCTIONAL_ERROR_CODE \
        TEST_POINT_READY_TO_BOOT \
        TEST_POINT_BYTE8_READY_TO_BOOT_HSTI_TABLE_FUNCTIONAL_ERROR_STRING
      );
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}
