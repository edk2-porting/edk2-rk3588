/** @file

  Copyright (c) 2020 Arm, Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>

#include "BcmGenetDxe.h"

STATIC
EFI_STATUS
EFIAPI
GenetAipGetInformation (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  OUT VOID                              **InformationBlock,
  OUT UINTN                             *InformationBlockSize
  )
{
  EFI_ADAPTER_INFO_MEDIA_STATE  *AdapterInfo;
  GENET_PRIVATE_DATA            *Genet;

  if (This == NULL || InformationBlock == NULL ||
      InformationBlockSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!CompareGuid (InformationType, &gEfiAdapterInfoMediaStateGuid)) {
    return EFI_UNSUPPORTED;
  }

  AdapterInfo = AllocateZeroPool (sizeof (EFI_ADAPTER_INFO_MEDIA_STATE));
  if (AdapterInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *InformationBlock = AdapterInfo;
  *InformationBlockSize = sizeof (EFI_ADAPTER_INFO_MEDIA_STATE);

  Genet = GENET_PRIVATE_DATA_FROM_AIP_THIS (This);
  AdapterInfo->MediaState = GenericPhyUpdateConfig (&Genet->Phy);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
GenetAipSetInformation (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  IN  VOID                              *InformationBlock,
  IN  UINTN                             InformationBlockSize
  )
{
  if (This == NULL || InformationBlock == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (CompareGuid (InformationType, &gEfiAdapterInfoMediaStateGuid)) {
    return EFI_WRITE_PROTECTED;
  }

  return EFI_UNSUPPORTED;
}

STATIC
EFI_STATUS
EFIAPI
GenetAipGetSupportedTypes (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  OUT EFI_GUID                          **InfoTypesBuffer,
  OUT UINTN                             *InfoTypesBufferCount
  )
{
  EFI_GUID    *Guid;

  if (This == NULL || InfoTypesBuffer == NULL ||
      InfoTypesBufferCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Guid = AllocatePool (sizeof *Guid);
  if (Guid == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyGuid (Guid, &gEfiAdapterInfoMediaStateGuid);

  *InfoTypesBuffer      = Guid;
  *InfoTypesBufferCount = 1;

  return EFI_SUCCESS;
}

CONST EFI_ADAPTER_INFORMATION_PROTOCOL gGenetAdapterInfoTemplate = {
  GenetAipGetInformation,
  GenetAipSetInformation,
  GenetAipGetSupportedTypes,
};
