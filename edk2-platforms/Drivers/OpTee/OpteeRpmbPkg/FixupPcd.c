/** @file

  Update the patched PCDs to their correct value

  Copyright (c) 2020, Linaro Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

/**
 * Patch the relevant PCDs of the RPMB driver with the correct address of the
 * allocated memory
 *
**/
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MmServicesTableLib.h>
#include <Library/PcdLib.h>

#include <Protocol/FirmwareVolumeBlock.h>
#include <Protocol/SmmFirmwareVolumeBlock.h>

#include "OpTeeRpmbFvb.h"

/**
  Fixup the Pcd values for variable storage

  Since the upper layers of EDK2 expect a memory mapped interface and we can't
  offer that from an RPMB, the driver allocates memory on init and passes that
  on the upper layers. Since the memory is dynamically allocated and we can't set the
  PCD in StMM context, we need to patch it correctly on each access

  @retval EFI_SUCCESS            Protocol was found and PCDs patched up
  @retval EFI_INVALID_PARAMETER  Invalid parameter
  @retval EFI_NOT_FOUND          Protocol interface not found
**/
EFI_STATUS
EFIAPI
FixPcdMemory (
  VOID
  )
{
  EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *FvbProtocol;
  MEM_INSTANCE                        *Instance;
  EFI_STATUS                          Status;

  //
  // Locate SmmFirmwareVolumeBlockProtocol
  //
  Status = gMmst->MmLocateProtocol (
                    &gEfiSmmFirmwareVolumeBlockProtocolGuid,
                    NULL,
                    (VOID **) &FvbProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  Instance = INSTANCE_FROM_FVB_THIS (FvbProtocol);

  // Patch PCDs with the correct values
  PatchPcdSet64 (PcdFlashNvStorageVariableBase64, Instance->MemBaseAddress);
  PatchPcdSet64 (
    PcdFlashNvStorageFtwWorkingBase64,
    Instance->MemBaseAddress + PcdGet32 (PcdFlashNvStorageVariableSize)
    );
  PatchPcdSet64 (
    PcdFlashNvStorageFtwSpareBase64,
    Instance->MemBaseAddress +
    PcdGet32 (PcdFlashNvStorageVariableSize) +
    PcdGet32 (PcdFlashNvStorageFtwWorkingSize)
    );

  DEBUG ((DEBUG_INFO, "%a: Fixup PcdFlashNvStorageVariableBase64: 0x%lx\n",
    __FUNCTION__, PcdGet64 (PcdFlashNvStorageVariableBase64)));
  DEBUG ((DEBUG_INFO, "%a: Fixup PcdFlashNvStorageFtwWorkingBase64: 0x%lx\n",
    __FUNCTION__, PcdGet64 (PcdFlashNvStorageFtwWorkingBase64)));
  DEBUG ((DEBUG_INFO, "%a: Fixup PcdFlashNvStorageFtwSpareBase64: 0x%lx\n",
    __FUNCTION__, PcdGet64 (PcdFlashNvStorageFtwSpareBase64)));

  return Status;
}
