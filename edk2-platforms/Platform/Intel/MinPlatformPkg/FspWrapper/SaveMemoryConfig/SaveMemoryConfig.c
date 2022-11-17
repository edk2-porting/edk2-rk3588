/** @file
  This is the driver that locates the MemoryConfigurationData HOB, if it
  exists, and saves the data to nvRAM.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Guid/GlobalVariable.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/VariableLock.h>

/**
  This is the standard EFI driver point that detects whether there is a
  MemoryConfigurationData HOB and, if so, saves its data to nvRAM.

  @param[in] ImageHandle    Handle for the image of this driver
  @param[in] SystemTable    Pointer to the EFI System Table

  @retval    EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
SaveMemoryConfigEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS        Status;
  EFI_HOB_GUID_TYPE *GuidHob;
  VOID              *HobData;
  VOID              *VariableData;
  UINTN             DataSize;
  UINTN             BufferSize;
  EDKII_VARIABLE_LOCK_PROTOCOL        *VariableLock;

  DataSize     = 0;
  VariableData = NULL;
  GuidHob      = NULL;
  HobData      = NULL;

  //
  // Search for the Memory Configuration GUID HOB.  If it is not present, then
  // there's nothing we can do. It may not exist on the update path.
  //
  GuidHob = GetFirstGuidHob (&gFspNonVolatileStorageHobGuid);
  if (GuidHob != NULL) {
    HobData  = GET_GUID_HOB_DATA (GuidHob);
    DataSize = GET_GUID_HOB_DATA_SIZE(GuidHob);
    if (DataSize > 0) {
      //
      // Use the HOB to save Memory Configuration Data
      //
      BufferSize = DataSize;
      VariableData = AllocatePool (BufferSize);
      if (VariableData == NULL) {
        return EFI_UNSUPPORTED;
      }
      Status = gRT->GetVariable (
                      L"MemoryConfig",
                      &gFspNonVolatileStorageHobGuid,
                      NULL,
                      &BufferSize,
                      VariableData
                      );

      if (Status == EFI_BUFFER_TOO_SMALL) {
        FreePool (VariableData);
        VariableData = AllocatePool (BufferSize);
        if (VariableData == NULL) {
          return EFI_UNSUPPORTED;
        }

        Status = gRT->GetVariable (
                        L"MemoryConfig",
                        &gFspNonVolatileStorageHobGuid,
                        NULL,
                        &BufferSize,
                        VariableData
                        );
      }

      if ( (EFI_ERROR(Status)) || BufferSize != DataSize || 0 != CompareMem (HobData, VariableData, DataSize)) {
        Status = gRT->SetVariable (
                        L"MemoryConfig",
                        &gFspNonVolatileStorageHobGuid,
                        (EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS),
                        DataSize,
                        HobData
                        );
        ASSERT_EFI_ERROR (Status);

        DEBUG((DEBUG_INFO, "Restored Size is 0x%x\n", DataSize));
      }

      //
      // Mark MemoryConfig to read-only if the Variable Lock protocol exists
      //
      Status = gBS->LocateProtocol(&gEdkiiVariableLockProtocolGuid, NULL, (VOID **)&VariableLock);
      if (!EFI_ERROR(Status)) {
        Status = VariableLock->RequestToLock(VariableLock, L"MemoryConfig", &gFspNonVolatileStorageHobGuid);
        ASSERT_EFI_ERROR(Status);
      }

      FreePool (VariableData);
    } else {
      DEBUG((DEBUG_INFO, "Memory save size is %d\n", DataSize));
    }
  } else {
    DEBUG((DEBUG_ERROR, "Memory S3 Data HOB was not found\n"));
  }

  //
  // This driver does not produce any protocol services, so always unload it.
  //
  return EFI_REQUEST_UNLOAD_IMAGE;
}
