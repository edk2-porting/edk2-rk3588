/** @file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiSmm.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/TestPointCheckLib.h>
#include <Library/TestPointLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/MemoryAttributesTable.h>
#include <Guid/PiSmmMemoryAttributesTable.h>
#include <Protocol/LoadedImage.h>

VOID
TestPointDumpMemoryAttributesTable (
  IN EFI_MEMORY_ATTRIBUTES_TABLE                     *MemoryAttributesTable
  );

EFI_STATUS
TestPointCheckImageMemoryAttribute (
  IN EFI_MEMORY_ATTRIBUTES_TABLE     *MemoryAttributesTable,
  IN EFI_PHYSICAL_ADDRESS            ImageBase,
  IN UINT64                          ImageSize,
  IN BOOLEAN                         IsFromSmm
  );

EFI_STATUS
TestPointCheckSmmMemoryAttributesTable (
  IN EFI_MEMORY_ATTRIBUTES_TABLE                     *MemoryAttributesTable
  )
{
  EFI_STATUS                             Status;
  EFI_LOADED_IMAGE_PROTOCOL              *LoadedImage;
  UINTN                                  Index;
  UINTN                                  HandleBufSize;
  EFI_HANDLE                             *HandleBuf;
  UINTN                                  HandleCount;
  EFI_STATUS                             ReturnStatus;
  
  ReturnStatus = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "==== TestPointDumpSmmLoadedImage - Enter\n"));
  HandleBuf = NULL;
  HandleBufSize = 0;
  Status = gSmst->SmmLocateHandle (
                    ByProtocol,
                    &gEfiLoadedImageProtocolGuid,
                    NULL,
                    &HandleBufSize,
                    HandleBuf
                    );
  if (Status != EFI_BUFFER_TOO_SMALL) {
    goto Done ;
  }
  HandleBuf = AllocateZeroPool (HandleBufSize);
  if (HandleBuf == NULL) {
    goto Done ;
  }
  Status = gSmst->SmmLocateHandle (
                    ByProtocol,
                    &gEfiLoadedImageProtocolGuid,
                    NULL,
                    &HandleBufSize,
                    HandleBuf
                    );
  if (EFI_ERROR (Status)) {
    goto Done ;
  }
  HandleCount = HandleBufSize / sizeof(EFI_HANDLE);
  
  DEBUG ((DEBUG_INFO, "SmmLoadedImage (%d):\n", HandleCount));
  for (Index = 0; Index < HandleCount; Index++) {
    Status = gSmst->SmmHandleProtocol (
                      HandleBuf[Index],
                      &gEfiLoadedImageProtocolGuid,
                      (VOID **)&LoadedImage
                      );
    if (EFI_ERROR(Status)) {
      continue;
    }
    Status = TestPointCheckImageMemoryAttribute (
               MemoryAttributesTable,
               (EFI_PHYSICAL_ADDRESS)(UINTN)LoadedImage->ImageBase,
               LoadedImage->ImageSize,
               TRUE
               );
    if (EFI_ERROR(Status)) {
      ReturnStatus = Status;
    }
  }

Done:

  if (HandleBuf != NULL) {
    FreePool (HandleBuf);
  }

  return ReturnStatus;
}

/**
  Retrieves a pointer to the system configuration table from the SMM System Table
  based on a specified GUID.

  @param[in]   TableGuid       The pointer to table's GUID type.
  @param[out]  Table           The pointer to the table associated with TableGuid in the EFI System Table.

  @retval EFI_SUCCESS     A configuration table matching TableGuid was found.
  @retval EFI_NOT_FOUND   A configuration table matching TableGuid could not be found.

**/
EFI_STATUS
EFIAPI
SmmGetSystemConfigurationTable (
  IN  EFI_GUID  *TableGuid,
  OUT VOID      **Table
  )
{
  UINTN             Index;

  ASSERT (TableGuid != NULL);
  ASSERT (Table != NULL);

  *Table = NULL;
  for (Index = 0; Index < gSmst->NumberOfTableEntries; Index++) {
    if (CompareGuid (TableGuid, &(gSmst->SmmConfigurationTable[Index].VendorGuid))) {
      *Table = gSmst->SmmConfigurationTable[Index].VendorTable;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

EFI_STATUS
TestPointCheckSmmMemAttribute (
  VOID
  )
{
  EFI_STATUS  Status;
  VOID        *MemoryAttributesTable;
  
  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmmMemAttribute - Enter\n"));
  Status = SmmGetSystemConfigurationTable (&gEdkiiPiSmmMemoryAttributesTableGuid, (VOID **)&MemoryAttributesTable);
  if (!EFI_ERROR (Status)) {
    TestPointDumpMemoryAttributesTable(MemoryAttributesTable);
    Status = TestPointCheckSmmMemoryAttributesTable(MemoryAttributesTable);
  }

  if (EFI_ERROR (Status)) {
    TestPointLibAppendErrorString (
      PLATFORM_TEST_POINT_ROLE_PLATFORM_IBV,
      NULL,
      TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SMM_MEMORY_ATTRIBUTE_TABLE_FUNCTIONAL_ERROR_CODE \
        TEST_POINT_SMM_READY_TO_LOCK \
        TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SMM_MEMORY_ATTRIBUTE_TABLE_FUNCTIONAL_ERROR_STRING
      );
  }
  DEBUG ((DEBUG_INFO, "==== TestPointCheckSmmMemAttribute - Exit\n"));

  return Status;
}
