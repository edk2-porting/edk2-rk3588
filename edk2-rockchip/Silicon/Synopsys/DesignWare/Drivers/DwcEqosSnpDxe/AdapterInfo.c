/** @file

  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Eqos.h"

/**
  Returns the current state information for the adapter.

  This function returns information of type InformationType from the adapter.
  If an adapter does not support the requested informational type, then
  EFI_UNSUPPORTED is returned. If an adapter does not contain Information for
  the requested InformationType, it fills InformationBlockSize with 0 and
  returns EFI_NOT_FOUND.

  @param[in]  This                   A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[in]  InformationType        A pointer to an EFI_GUID that defines the contents of InformationBlock.
  @param[out] InforamtionBlock       The service returns a pointer to the buffer with the InformationBlock
                                     structure which contains details about the data specific to InformationType.
  @param[out] InforamtionBlockSize   The driver returns the size of the InformationBlock in bytes.

  @retval EFI_SUCCESS                The InformationType information was retrieved.
  @retval EFI_UNSUPPORTED            The InformationType is not known.
  @retval EFI_NOT_FOUND              Information is not available for the requested information type.
  @retval EFI_DEVICE_ERROR           The device reported an error.
  @retval EFI_OUT_OF_RESOURCES       The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER      This is NULL.
  @retval EFI_INVALID_PARAMETER      InformationBlock is NULL.
  @retval EFI_INVALID_PARAMETER      InformationBlockSize is NULL.

**/
STATIC
EFI_STATUS
EFIAPI
EqosAipGetInformation (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  OUT VOID                              **InformationBlock,
  OUT UINTN                             *InformationBlockSize
  )
{
  EFI_ADAPTER_INFO_MEDIA_STATE  *AdapterInfo;
  EQOS_PRIVATE_DATA             *Eqos;

  if ((This == NULL) || (InformationBlock == NULL) ||
      (InformationBlockSize == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  if (!CompareGuid (InformationType, &gEfiAdapterInfoMediaStateGuid)) {
    return EFI_UNSUPPORTED;
  }

  AdapterInfo = AllocateZeroPool (sizeof (EFI_ADAPTER_INFO_MEDIA_STATE));
  if (AdapterInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *InformationBlock     = AdapterInfo;
  *InformationBlockSize = sizeof (EFI_ADAPTER_INFO_MEDIA_STATE);

  Eqos                    = EQOS_PRIVATE_DATA_FROM_AIP_THIS (This);
  AdapterInfo->MediaState = EqosUpdateLink (Eqos);

  return EFI_SUCCESS;
}

/**
  Sets state information for an adapter.

  This function sends information of type InformationType for an adapter.
  If an adapter does not support the requested information type, then EFI_UNSUPPORTED
  is returned.

  @param[in]  This                   A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[in]  InformationType        A pointer to an EFI_GUID that defines the contents of InformationBlock.
  @param[in]  InforamtionBlock       A pointer to the InformationBlock structure which contains details
                                     about the data specific to InformationType.
  @param[in]  InforamtionBlockSize   The size of the InformationBlock in bytes.

  @retval EFI_SUCCESS                The information was received and interpreted successfully.
  @retval EFI_UNSUPPORTED            The InformationType is not known.
  @retval EFI_DEVICE_ERROR           The device reported an error.
  @retval EFI_INVALID_PARAMETER      This is NULL.
  @retval EFI_INVALID_PARAMETER      InformationBlock is NULL.
  @retval EFI_WRITE_PROTECTED        The InformationType cannot be modified using EFI_ADAPTER_INFO_SET_INFO().

**/
STATIC
EFI_STATUS
EFIAPI
EqosAipSetInformation (
  IN EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN EFI_GUID                          *InformationType,
  IN VOID                              *InformationBlock,
  IN UINTN                             InformationBlockSize
  )
{
  if ((This == NULL) || (InformationBlock == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (CompareGuid (InformationType, &gEfiAdapterInfoMediaStateGuid)) {
    return EFI_WRITE_PROTECTED;
  }

  return EFI_UNSUPPORTED;
}

/**
  Get a list of supported information types for this instance of the protocol.

  This function returns a list of InformationType GUIDs that are supported on an
  adapter with this instance of EFI_ADAPTER_INFORMATION_PROTOCOL. The list is returned
  in InfoTypesBuffer, and the number of GUID pointers in InfoTypesBuffer is returned in
  InfoTypesBufferCount.

  @param[in]  This                  A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[out] InfoTypesBuffer       A pointer to the array of InformationType GUIDs that are supported
                                    by This.
  @param[out] InfoTypesBufferCount  A pointer to the number of GUIDs present in InfoTypesBuffer.

  @retval EFI_SUCCESS               The list of information type GUIDs that are supported on this adapter was
                                    returned in InfoTypesBuffer. The number of information type GUIDs was
                                    returned in InfoTypesBufferCount.
  @retval EFI_INVALID_PARAMETER     This is NULL.
  @retval EFI_INVALID_PARAMETER     InfoTypesBuffer is NULL.
  @retval EFI_INVALID_PARAMETER     InfoTypesBufferCount is NULL.
  @retval EFI_OUT_OF_RESOURCES      There is not enough pool memory to store the results.

**/
STATIC
EFI_STATUS
EFIAPI
EqosAipGetSupportedTypes (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  OUT EFI_GUID                          **InfoTypesBuffer,
  OUT UINTN                             *InfoTypesBufferCount
  )
{
  EFI_GUID  *Guid;

  if ((This == NULL) || (InfoTypesBuffer == NULL) ||
      (InfoTypesBufferCount == NULL))
  {
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

///
/// EFI Adapter Information Protocol
///
CONST EFI_ADAPTER_INFORMATION_PROTOCOL  gEqosAipTemplate = {
  EqosAipGetInformation,
  EqosAipSetInformation,
  EqosAipGetSupportedTypes,
};
