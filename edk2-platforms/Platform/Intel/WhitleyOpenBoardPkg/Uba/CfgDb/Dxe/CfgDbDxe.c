/** @file
  UbaConfigDatabaseDxe Driver.

  @copyright
  Copyright 2013 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "CfgDbDxe.h"

#include <PiDxe.h>  // For Hob

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

/**
  Internal function for getting the platform record node in the database.

  @param This                   uba Protocol instance.
  @param SkuNode                The pointer to pointer of Platform record node.

  @retval EFI_NOT_FOUND         Platform record not found.
  @retval EFI_SUCCESS           Platform found.
**/
EFI_STATUS
InternalGetSkuNode (
  IN  UBA_CONFIG_DATABASE_PROTOCOL  *This,
  OUT UBA_BOARD_NODE                **SkuNode
  )
{
  UBA_DXE_PRIVATE_DATA              *UbaDxePrivate;
  UbaDxePrivate = NULL;

  UbaDxePrivate = PRIVATE_DATA_FROM_PROTOCOL (This);
  *SkuNode = UbaDxePrivate->CurrentSku;

  return EFI_SUCCESS;
}

/**
  Internal function for init the platform record to database.
  Create the connections between UBA_BOARD_NODE and UBA_DXE_PRIVATE_DATA

  @param This                   UBA Protocol instance.
  @param BoardId                The platform type.
  @param BoardGuid              The platform GUID.
  @param BoardName              The platform user friendly name.
  @param SkuNode                The pointer to pointer of Platform record node.

  @retval EFI_ALREADY_STARTED   Platform record already exist.
  @retval EFI_OUT_OF_RESOURCES  No enough resource.
  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
InternalInitSkuDxe (
  IN  UBA_CONFIG_DATABASE_PROTOCOL        *This,
  IN  UINT32                              BoardId,
  IN  EFI_GUID                            *BoardGuid,     OPTIONAL
  IN  CHAR8                               *BoardName,     OPTIONAL
  OUT UBA_BOARD_NODE                      **SkuNode       OPTIONAL
  )
{
  UBA_DXE_PRIVATE_DATA            *UbaDxePrivate;
  UBA_BOARD_NODE                  *NewSkuNode;
  UbaDxePrivate = NULL;
  NewSkuNode = NULL;

  UbaDxePrivate = PRIVATE_DATA_FROM_PROTOCOL (This);

  NewSkuNode = AllocateZeroPool (sizeof (UBA_BOARD_NODE));
  if (NewSkuNode == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  NewSkuNode->Signature     = UBA_BOARD_SIGNATURE;
  NewSkuNode->Version       = UBA_BOARD_VERSION;
  NewSkuNode->BoardId       = BoardId;

  if (BoardName != NULL) {
    AsciiStrnCpyS (NewSkuNode->BoardName, AsciiStrSize (BoardName) / sizeof (CHAR8), BoardName, sizeof (NewSkuNode->BoardName) - 1);
  }

  if (BoardGuid != NULL) {
    CopyMem (&NewSkuNode->BoardGuid, BoardGuid, sizeof (EFI_GUID));
  }

  // Initialize the list head for Datalink.
  InitializeListHead (&NewSkuNode->DataLinkHead);

  if (SkuNode != NULL) {
    // Output the point of sku node.
    *SkuNode = NewSkuNode;
    // Pass the point to CurrentSku in UbaDxePrivate.
    UbaDxePrivate->CurrentSku = NewSkuNode;
  }

  return EFI_SUCCESS;
}

/**
  Internal function for adding new configuration data record to database.

  @param This                   uba Protocol instance.
  @param ResId                  The resource ID.
  @param Data                   Data pointer.
  @param DataSize               Data size.

  @retval EFI_INVALID_PARAMETER Parameter invalid.
  @retval EFI_OUT_OF_RESOURCES  No enough resource.
  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
InternalAddNewConfigData (
  IN  UBA_CONFIG_DATABASE_PROTOCOL        *This,
  IN  EFI_GUID                            *ResId,
  IN  VOID                                *Data,
  IN  UINTN                               DataSize
  )
{
  EFI_STATUS                      Status;
  EFI_HANDLE                      Handle;
  UBA_DXE_PRIVATE_DATA            *UbaDxePrivate;
  UBA_CONFIG_NODE                 *NewDataNode;
  UBA_BOARD_NODE                  *SkuNode;
  UbaDxePrivate = NULL;
  NewDataNode   = NULL;
  SkuNode       = NULL;

  if ((ResId == NULL) || (Data == NULL) || (DataSize <= 0)) {
    return EFI_INVALID_PARAMETER;
  }

  UbaDxePrivate = PRIVATE_DATA_FROM_PROTOCOL (This);

  Status = InternalGetSkuNode (This, &SkuNode);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  NewDataNode = AllocateZeroPool (sizeof (UBA_CONFIG_NODE));
  if (NewDataNode == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  NewDataNode->Signature      = UBA_BOARD_SIGNATURE;
  NewDataNode->Version        = UBA_BOARD_VERSION;
  NewDataNode->Handle         = (EFI_HANDLE) (UINTN) UbaDxePrivate->HandleCount;
  NewDataNode->Data           = AllocateCopyPool (DataSize, Data);
  NewDataNode->Size           = (UINT32) DataSize;

  CopyMem (&NewDataNode->ResId, ResId, sizeof (EFI_GUID));

  InsertTailList (&SkuNode->DataLinkHead, &NewDataNode->DataLink);
  SkuNode->DataCount ++;
  UbaDxePrivate->ConfigDataCount ++;
  UbaDxePrivate->HandleCount ++;

  //
  // This Protocol just install for Protocol notify
  // The Protocol instance UbaCfgDbProtocol should not used
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &NewDataNode->ResId,
                  EFI_NATIVE_INTERFACE,
                  &UbaDxePrivate->UbaCfgDbProtocol
                  );
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Internal function for Getting configuration data from database.

  @param This                   uba Protocol instance.
  @param SkuNode                The platform node record.
  @param ResId                  The resource ID.
  @param Data                   Data pointer.
  @param DataSize               Data size pointer.

  @retval EFI_INVALID_PARAMETER Parameter invalid.
  @retval EFI_NOT_FOUND         Resource not found.
  @retval EFI_BUFFER_TOO_SMALL  The buffer is too small to copy the data.
  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
InternalGetConfigData (
  IN  UBA_CONFIG_DATABASE_PROTOCOL        *This,
  IN  UBA_BOARD_NODE                      *SkuNode,
  IN  EFI_GUID                            *ResId,
  OUT VOID                                *Data,      OPTIONAL
  OUT UINTN                               *DataSize   OPTIONAL
  )
{
  LIST_ENTRY                            *ListHead;
  LIST_ENTRY                            *Link;
  UBA_CONFIG_NODE                       *DataNode;
  ListHead = NULL;
  Link     = NULL;
  DataNode = NULL;

  if ((SkuNode == NULL) || (ResId == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  ListHead = &SkuNode->DataLinkHead;
  if (IsListEmpty (ListHead)) {
    return EFI_NOT_FOUND;
  }

  for (Link = GetFirstNode (ListHead); !IsNull (ListHead, Link); Link = GetNextNode (ListHead, Link)) {

    DataNode = CONFIG_NODE_INSTANCE_FROM_THIS (Link);
    ASSERT (DataNode != NULL);
    if (DataNode == NULL) {
      return EFI_UNSUPPORTED;
    }
    if (CompareGuid (ResId, &DataNode->ResId)) {

      if (DataSize != NULL) {

        if (*DataSize < DataNode->Size) {
          *DataSize = DataNode->Size;
          return EFI_BUFFER_TOO_SMALL;
        }

        *DataSize = DataNode->Size;

        if (Data != NULL) {
          CopyMem (Data, DataNode->Data, DataNode->Size);
        }
      }

      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  Get platform's GUID and user friendly name by BoardId.

  This is used when you need a BoardGuid to Add/Get platform data

  Core will create a new platform for you if the BoardId is not
  recorded in database, and assgin a unique GUID for this platform.

  @param This                   uba Protocol instance.
  @param BoardId           The platform type, same define as Platform.h.
  @param BoardGuid             The GUID for this platform.
  @param BoardName           The user friendly name for this platform.

  @retval EFI_ALREADY_STARTED   Create new for an exist platform.
  @retval EFI_OUT_OF_RESOURCES  Resource not enough.
  @retval EFI_NOT_FOUND         Platform not found.
  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
EFIAPI
DxeUbaGetPlatformSku (
  IN  UBA_CONFIG_DATABASE_PROTOCOL        *This,
  OUT UINT32                              *BoardId,
  OUT EFI_GUID                            *BoardGuid,    OPTIONAL
  OUT CHAR8                               *BoardName   OPTIONAL
  )
{
  EFI_STATUS                 Status;
  UBA_BOARD_NODE             *SkuNode;
  SkuNode = NULL;

  Status = InternalGetSkuNode (This, &SkuNode);

  if (!EFI_ERROR (Status)) {
    if (BoardId != NULL) {
      *BoardId = SkuNode->BoardId;
    }
    if (BoardName != NULL) {
      AsciiStrCpyS (BoardName, AsciiStrSize (SkuNode->BoardName) / sizeof (CHAR8) , SkuNode->BoardName);
    }

    if (BoardGuid != NULL) {
      CopyMem (BoardGuid, &SkuNode->BoardGuid, sizeof (EFI_GUID));
    }

    return EFI_SUCCESS;
  }

  return Status;
}

/**
  Add configuration data to uba configuration database.

  @param This                   uba Protocol instance.
  @param ResId                  The configuration data resource id.
  @param Data                   The data buffer pointer.
  @param DataSize               Size of data want to add into database.

  @retval EFI_INVALID_PARAMETER Required parameters not correct.
  @retval EFI_OUT_OF_RESOURCES  Resource not enough.
  @retval EFI_NOT_FOUND         Platform not found.
  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
EFIAPI
DxeUbaAddData (
  IN  UBA_CONFIG_DATABASE_PROTOCOL        *This,
  IN  EFI_GUID                            *ResId,
  IN  VOID                                *Data,
  IN  UINTN                               DataSize
  )
{
  EFI_STATUS                 Status;
  UBA_BOARD_NODE             *SkuNode;
  SkuNode = NULL;

  Status = InternalGetSkuNode (This, &SkuNode);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InternalAddNewConfigData (This, ResId, Data, DataSize);

  return Status;
}

/**
  Get configuration data from uba configuration database.

  @param This                   uba Protocol instance.
  @param ResId                  The configuration data resource id.
  @param Data                   The data buffer pointer.
  @param DataSize               IN:Size of data want to get, OUT: Size of data in database.

  @retval EFI_INVALID_PARAMETER Required parameters not correct.
  @retval EFI_BUFFER_TOO_SMALL  The DataSize of Data buffer is too small to get this configuration data
  @retval EFI_OUT_OF_RESOURCES  Resource not enough.
  @retval EFI_NOT_FOUND         Platform or data not found.
  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
EFIAPI
DxeUbaGetData (
  IN  UBA_CONFIG_DATABASE_PROTOCOL        *This,
  IN  EFI_GUID                            *ResId,
  OUT VOID                                *Data,
  OUT UINTN                               *DataSize
  )
{
  EFI_STATUS                      Status;
  UBA_DXE_PRIVATE_DATA            *UbaDxePrivate;
  UBA_BOARD_NODE                  *SkuNode;
  UbaDxePrivate = NULL;
  SkuNode = NULL;

  if ((ResId == NULL) || (Data == NULL) || (DataSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  UbaDxePrivate = PRIVATE_DATA_FROM_PROTOCOL (This);


  Status = InternalGetSkuNode (This, &SkuNode);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InternalGetConfigData (This, SkuNode, ResId, Data, DataSize);
  if (!EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  return EFI_NOT_FOUND;
}

/**
  Internal function for getting current platform's configuration data from HOB, which passed by PEIM.

  @param This                   uba Protocol instance.

  @retval EFI_INVALID_PARAMETER Required parameters not correct.
  @retval EFI_OUT_OF_RESOURCES  Resource not enough.
  @retval EFI_NOT_FOUND         Platform or data not found.
  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
InternalGetConfigDataFromHob (
  IN  UBA_CONFIG_DATABASE_PROTOCOL  *This
  )
{
  EFI_STATUS                            Status;
  EFI_PEI_HOB_POINTERS                  Hob;
  UINT8                                 *HobDataStart;
  UBA_CONFIG_HOB_HEADER                 *HobHeader;
  UBA_CONFIG_HOB_FIELD                  *HobDataFieldStart;
  UINTN                                 Index;
  UBA_DXE_PRIVATE_DATA                  *UbaDxePrivate;
  UBA_BOARD_NODE                        *SkuNode;
  HobHeader = NULL;
  HobDataFieldStart = NULL;
  UbaDxePrivate = NULL;
  SkuNode = NULL;

  UbaDxePrivate = PRIVATE_DATA_FROM_PROTOCOL (This);

  Hob.Raw = GetFirstGuidHob (&gUbaCurrentConfigHobGuid);
  ASSERT (Hob.Raw != NULL);
  if (Hob.Raw == NULL) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "UbaConfigDatabasedxeEntry: get first hob!\n"));

  HobDataStart = GET_GUID_HOB_DATA (Hob);

  HobHeader          = (UBA_CONFIG_HOB_HEADER *) HobDataStart;
  HobDataFieldStart  = HobHeader->HobField;

  //Call internal function for SKU init, accroding to hob data.
  Status = InternalInitSkuDxe (This, HobHeader->BoardId, &HobHeader->BoardGuid, HobHeader->BoardName, &SkuNode);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Obtain all data stored in the hob from PEI phase.
  for (Index = 0; Index < HobHeader->DataCount; Index ++) {
    Status = This->AddData (
                     This,
                     &HobDataFieldStart[Index].ResId,
                     (VOID *) ((UINTN) HobDataFieldStart[Index].DataOffset + (UINTN) HobHeader),
                     HobDataFieldStart[Index].Size
                     );
    ASSERT_EFI_ERROR (Status);
    if (Status != EFI_SUCCESS) {
      return Status;
    }
  }

  return EFI_SUCCESS;
}

/**
  The Driver Entry Point.

  The function is the driver Entry point.

  @param ImageHandle   A handle for the image that is initializing this driver
  @param SystemTable   A pointer to the EFI system table

  @retval EFI_SUCCESS:              Driver initialized successfully
  @retval EFI_LOAD_ERROR:           Failed to Initialize or has been loaded
  @retval EFI_OUT_OF_RESOURCES      Could not allocate needed resources
**/
EFI_STATUS
EFIAPI
UbaConfigDatabaseEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  EFI_STATUS                            Status;
  UBA_DXE_PRIVATE_DATA                  *UbaDxePrivate;
  EFI_HANDLE                            Handle;
  UbaDxePrivate = NULL;

  UbaDxePrivate = AllocateZeroPool (sizeof (UBA_DXE_PRIVATE_DATA));
  ASSERT (UbaDxePrivate != NULL);
  if (UbaDxePrivate == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  DEBUG ((DEBUG_INFO, "UbaConfigDatabasedxeEntry: after allocate Memory!\n"));
  UbaDxePrivate->Signature  = UBA_BOARD_SIGNATURE;
  UbaDxePrivate->Version    = UBA_BOARD_VERSION;

  UbaDxePrivate->ConfigDataCount           = 0;
  UbaDxePrivate->HandleCount               = 0;

  UbaDxePrivate->UbaCfgDbProtocol.Signature      = UBA_CONFIG_PROTOCOL_SIGNATURE;
  UbaDxePrivate->UbaCfgDbProtocol.Version        = UBA_CONFIG_PROTOCOL_VERSION;

  UbaDxePrivate->UbaCfgDbProtocol.GetSku         = DxeUbaGetPlatformSku;
  UbaDxePrivate->UbaCfgDbProtocol.AddData        = DxeUbaAddData;
  UbaDxePrivate->UbaCfgDbProtocol.GetData        = DxeUbaGetData;

  //
  // Just produce our Protocol
  //
  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gUbaConfigDatabaseProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &UbaDxePrivate->UbaCfgDbProtocol
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "UbaConfigDatabasedxeEntry: before get data from hob!\n"));
  // Init sku dxe and get configuration data from hob passed by PEIM.
  Status = InternalGetConfigDataFromHob (&UbaDxePrivate->UbaCfgDbProtocol);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

