/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/TestPointLib.h>
#include <Protocol/AdapterInformation.h>

VOID
DumpTestPoint (
  IN VOID                     *TestPointData
  )
{
  ADAPTER_INFO_PLATFORM_TEST_POINT *TestPoint;
  UINT8                            *Features;
  CHAR16                           *ErrorString;
  UINTN                            Index;
  CHAR16                           ErrorChar;

  TestPoint = TestPointData;
  Print (L"TestPoint\n");
  Print (L"  Version                     - 0x%08x\n", TestPoint->Version);
  Print (L"  Role                        - 0x%08x\n", TestPoint->Role);
  Print (L"  ImplementationID            - %S\n", TestPoint->ImplementationID);
  Print (L"  FeaturesSize                - 0x%08x\n", TestPoint->FeaturesSize);

  Features = (UINT8 *)(TestPoint + 1);
  Print (L"  FeaturesImplemented - ");
  for (Index = 0; Index < TestPoint->FeaturesSize; Index++) {
    Print (L"%02x ", Features[Index]);
  }
  Print (L"\n");

  Features = (UINT8 *)(Features + TestPoint->FeaturesSize);
  Print (L"  FeaturesVerified    - ");
  for (Index = 0; Index < TestPoint->FeaturesSize; Index++) {
    Print (L"%02x ", Features[Index]);
  }
  Print (L"\n");

  ErrorString = (CHAR16 *)(Features + TestPoint->FeaturesSize);
  Print (L"  ErrorString                 - \"");
  CopyMem (&ErrorChar, ErrorString, sizeof(ErrorChar));
  for (; ErrorChar != 0;) {
    if (ErrorChar == L'\r') {
      Print (L"\\r");
    } else if (ErrorChar == L'\n') {
      Print (L"\\n");
    } else {
      Print (L"%c", ErrorChar);
    }
    ErrorString++;
    CopyMem (&ErrorChar, ErrorString, sizeof(ErrorChar));
  }
  Print (L"\"\n");
}

VOID
DumpTestPointDataDxe (
  IN UINT32                   Role OPTIONAL,
  IN CHAR16                   *ImplementationID OPTIONAL
  )
{
  EFI_STATUS                        Status;
  EFI_ADAPTER_INFORMATION_PROTOCOL  *Aip;
  UINTN                             NoHandles;
  EFI_HANDLE                        *Handles;
  UINTN                             Index;
  EFI_GUID                          *InfoTypesBuffer;
  UINTN                             InfoTypesBufferCount;
  UINTN                             InfoTypesIndex;
  EFI_ADAPTER_INFORMATION_PROTOCOL  *AipCandidate;
  VOID                              *InformationBlock;
  UINTN                             InformationBlockSize;
  ADAPTER_INFO_PLATFORM_TEST_POINT  *TestPoint;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiAdapterInformationProtocolGuid,
                  NULL,
                  &NoHandles,
                  &Handles
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }

  TestPoint = NULL;
  Aip = NULL;
  InformationBlock = NULL;
  InformationBlockSize = 0;
  for (Index = 0; Index < NoHandles; Index++) {
    Status = gBS->HandleProtocol (
                    Handles[Index],
                    &gEfiAdapterInformationProtocolGuid,
                    (VOID **)&Aip
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    //
    // Check AIP
    //
    Status = Aip->GetSupportedTypes (
                    Aip,
                    &InfoTypesBuffer,
                    &InfoTypesBufferCount
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    AipCandidate = NULL;
    for (InfoTypesIndex = 0; InfoTypesIndex < InfoTypesBufferCount; InfoTypesIndex++) {
      if (CompareGuid (&InfoTypesBuffer[InfoTypesIndex], &gAdapterInfoPlatformTestPointGuid)) {
        AipCandidate = Aip;
        break;
      }
    }
    FreePool (InfoTypesBuffer);

    if (AipCandidate == NULL) {
      continue;
    }

    //
    // Check Role
    //
    Aip = AipCandidate;
    Status = Aip->GetInformation (
                    Aip,
                    &gAdapterInfoPlatformTestPointGuid,
                    &InformationBlock,
                    &InformationBlockSize
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    TestPoint = InformationBlock;

    if ((Role == 0) ||
        ((TestPoint->Role == Role) && 
         ((ImplementationID == NULL) || (StrCmp (ImplementationID, TestPoint->ImplementationID) == 0)))) {
      DumpTestPoint (TestPoint);
    }
    FreePool (InformationBlock);
  }
  FreePool (Handles);
}

EFI_STATUS
EFIAPI
TestPointDumpAppEntrypoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
  DumpTestPointDataDxe (0, NULL);

  return EFI_SUCCESS;
}
