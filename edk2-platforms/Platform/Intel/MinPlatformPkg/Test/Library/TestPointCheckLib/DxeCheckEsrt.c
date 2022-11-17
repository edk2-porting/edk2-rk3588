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
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/SystemResourceTable.h>

CHAR8 *mFwTypeString[] = {
  "  Unknown ",
  "  System  ",
  "  Device  ",
  "  Driver  ",
};

CHAR8 *mLastAttemptStatusString[] = {
  "Success",
  "Error: Unsuccessful",
  "Error: Insufficient Resources",
  "Error: Incorrect Version",
  "Error: Invalid Format",
  "Error: Auth Error",
  "Error: Power Event AC",
  "Error: Power Event Battery",
};

STATIC CHAR8 mUnknownStr[11];

CHAR8 *
FwTypeToString(
  IN UINT32 FwType
  )
{
  if (FwType < sizeof(mFwTypeString) / sizeof(mFwTypeString[0])) {
    return mFwTypeString[FwType];
  } else {
    AsciiSPrint(mUnknownStr, sizeof(mUnknownStr), "[%08x]", FwType);
    return mUnknownStr;
  }
}

CHAR8 *
LastAttemptStatusToString(
  IN UINT32 LastAttemptStatus
  )
{
  if (LastAttemptStatus < sizeof(mLastAttemptStatusString) / sizeof(mLastAttemptStatusString[0])) {
    return mLastAttemptStatusString[LastAttemptStatus];
  } else {
    return "Error: Unknown";
  }
}

VOID
DumpEsrt (
  IN EFI_SYSTEM_RESOURCE_TABLE  *Esrt
  )
{
  UINTN                      Index;
  EFI_SYSTEM_RESOURCE_ENTRY  *EsrtEntry;

  DEBUG ((DEBUG_INFO, "ESRT Table:"));
  DEBUG ((DEBUG_INFO, " Count=0x%x", Esrt->FwResourceCount));
  DEBUG ((DEBUG_INFO, " CountMax=0x%x", Esrt->FwResourceCountMax));
  DEBUG ((DEBUG_INFO, " Version=0x%lx\n", Esrt->FwResourceVersion));

  DEBUG ((DEBUG_INFO, "               FwClass                   FwType    Version   LowestVer    Flags    AttemptVer Attempt\n"));
//DEBUG ((DEBUG_INFO, "  00000000-0000-0000-0000-000000000000 [00000000] 0x00000000 0x00000000 0x00000000 0x00000000 Success\n"));
  EsrtEntry = (VOID *)(Esrt + 1);
  for (Index = 0; Index < Esrt->FwResourceCount; Index++) {
    DEBUG ((DEBUG_INFO, "  %g ", &EsrtEntry->FwClass));
    DEBUG ((DEBUG_INFO, FwTypeToString(EsrtEntry->FwType)));
    DEBUG ((DEBUG_INFO, " 0x%08x", EsrtEntry->FwVersion));
    DEBUG ((DEBUG_INFO, " 0x%08x", EsrtEntry->LowestSupportedFwVersion));
    DEBUG ((DEBUG_INFO, " 0x%08x", EsrtEntry->CapsuleFlags));
    DEBUG ((DEBUG_INFO, " 0x%08x ", EsrtEntry->LastAttemptVersion));
    DEBUG ((DEBUG_INFO, LastAttemptStatusToString(EsrtEntry->LastAttemptStatus)));
    DEBUG ((DEBUG_INFO, "\n"));
    EsrtEntry++;
  }
}

EFI_STATUS
TestPointCheckEsrt (
  VOID
  )
{
  EFI_STATUS                 Status;
  EFI_SYSTEM_RESOURCE_TABLE  *Esrt;
  
  DEBUG ((DEBUG_INFO, "==== TestPointCheckEsrt - Enter\n"));
  Status = EfiGetSystemConfigurationTable (&gEfiSystemResourceTableGuid, (VOID **)&Esrt);
  if (!EFI_ERROR(Status)) {
    DumpEsrt(Esrt);
  } else {
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE8_READY_TO_BOOT_ESRT_TABLE_FUNCTIONAL_ERROR_CODE \
        TEST_POINT_READY_TO_BOOT \
        TEST_POINT_BYTE8_READY_TO_BOOT_ESRT_TABLE_FUNCTIONAL_ERROR_STRING
      );
  }
  DEBUG ((DEBUG_INFO, "==== TestPointCheckEsrt - Exit\n"));

  return Status;
}