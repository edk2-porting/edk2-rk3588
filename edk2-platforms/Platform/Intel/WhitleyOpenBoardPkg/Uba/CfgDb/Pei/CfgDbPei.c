/** @file
  UbaConfigDatabase Peim.

  @copyright
  Copyright 2013 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "CfgDbPei.h"

#include <Ppi/EndOfPeiPhase.h>

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>

/**
  Internal function for getting the platform record node in the database.

  @param This                   uba Ppi instance.
  @param SkuNode                The pointer to pointer of Platform record node.

  @retval EFI_NOT_FOUND         Platform record not found.
  @retval EFI_SUCCESS           Platform found.
**/
EFI_STATUS
InternalGetSkuNode (
  IN  UBA_CONFIG_DATABASE_PPI       *This,
  OUT UBA_BOARD_NODE                **SkuNode
  )
{
  UBA_PEIM_PRIVATE_DATA           *UbaPeimPrivate;
  UbaPeimPrivate = NULL;

  UbaPeimPrivate = PRIVATE_DATA_FROM_PPI (This);
  *SkuNode = UbaPeimPrivate->CurrentSku;

  return EFI_SUCCESS;
}

/**
  Internal function for init the platform record to database.
  Create the connections between UBA_BOARD_NODE and UBA_PEIM_PRIVATE_DATA

  @param This                   uba Ppi instance.
  @param BoardId                The platform type.
  @param BoardGuid              The platform GUID.
  @param BoardName              The platform user friendly name.
  @param SkuNode                The pointer to pointer of Platform record node.

  @retval EFI_ALREADY_STARTED   Platform record already exist.
  @retval EFI_OUT_OF_RESOURCES  No enough resource.
  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
InternalInitSku (
  IN  UBA_CONFIG_DATABASE_PPI             *This,
  IN  UINT32                              BoardId,
  IN  EFI_GUID                            *BoardGuid,     OPTIONAL
  IN  CHAR8                               *BoardName,     OPTIONAL
  OUT UBA_BOARD_NODE                      **SkuNode       OPTIONAL
  )
{
  UBA_PEIM_PRIVATE_DATA           *UbaPeimPrivate;
  UBA_BOARD_NODE                  *NewSkuNode;
  UbaPeimPrivate = NULL;
  NewSkuNode = NULL;

  UbaPeimPrivate = PRIVATE_DATA_FROM_PPI (This);

  NewSkuNode = AllocateZeroPool (sizeof (UBA_BOARD_NODE));
  if (NewSkuNode == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  NewSkuNode->Signature     = UBA_BOARD_SIGNATURE;
  NewSkuNode->Version       = UBA_BOARD_VERSION;
  NewSkuNode->BoardId       = BoardId;

  if (BoardName != NULL) {
    AsciiStrnCpyS (NewSkuNode->BoardName, sizeof (NewSkuNode->BoardName), BoardName, sizeof (NewSkuNode->BoardName) - 1);
  }

  if (BoardGuid != NULL) {
    CopyMem (&NewSkuNode->BoardGuid, BoardGuid, sizeof (EFI_GUID));
  }

  //Initialize the list head for Datalink.
  InitializeListHead (&NewSkuNode->DataLinkHead);


  if (SkuNode != NULL) {
    *SkuNode = NewSkuNode;
    UbaPeimPrivate->CurrentSku = NewSkuNode;
  }

  return EFI_SUCCESS;
}

/**
  Internal function for adding new configuration data record to database.

  @param This                   uba Ppi instance.
  @param ResId                  The resource ID.
  @param Data                   Data pointer.
  @param DataSize               Data size.

  @retval EFI_INVALID_PARAMETER Parameter invalid.
  @retval EFI_OUT_OF_RESOURCES  No enough resource.
  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
InternalAddNewConfigData (
  IN  UBA_CONFIG_DATABASE_PPI             *This,
  IN  EFI_GUID                            *ResId,
  IN  VOID                                *Data,
  IN  UINTN                               DataSize
  )
{
  EFI_STATUS                      Status;
  UBA_PEIM_PRIVATE_DATA           *UbaPeimPrivate;
  UBA_CONFIG_NODE                 *NewDataNode;
  UBA_BOARD_NODE                  *SkuNode;
  EFI_PEI_PPI_DESCRIPTOR          *ConfigDataPpi;
  UbaPeimPrivate = NULL;
  NewDataNode = NULL;
  SkuNode = NULL;
  ConfigDataPpi = NULL;

  if ((ResId == NULL) || (Data == NULL) || (DataSize <= 0)) {
    return EFI_INVALID_PARAMETER;
  }

  UbaPeimPrivate = PRIVATE_DATA_FROM_PPI (This);

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
  NewDataNode->Handle         = (EFI_HANDLE) (UINTN) UbaPeimPrivate->HandleCount;
  NewDataNode->Data           = AllocateCopyPool (DataSize, Data);
  NewDataNode->Size           = (UINT32) DataSize;

  CopyMem (&NewDataNode->ResId, ResId, sizeof (EFI_GUID));

  InsertTailList (&SkuNode->DataLinkHead, &NewDataNode->DataLink);
  SkuNode->DataCount ++;
  UbaPeimPrivate->ConfigDataCount ++;
  UbaPeimPrivate->HandleCount ++;

  //
  // This PPI just install for NotifyPpi
  // The PPI instance UbaCfgDbPpi should not used
  //
  ConfigDataPpi = AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (ConfigDataPpi == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ConfigDataPpi->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  ConfigDataPpi->Guid  = &NewDataNode->ResId;
  ConfigDataPpi->Ppi   = &UbaPeimPrivate->UbaCfgDbPpi;

  Status = PeiServicesInstallPpi (ConfigDataPpi);
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Internal function for Getting configuration data from database.

  @param This                   uba Ppi instance.
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
  IN  UBA_CONFIG_DATABASE_PPI             *This,
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
  Link = NULL;
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
  Covert the LIST_ENTRY pointer from CAR to RAM after memory init.

  @param Link                   The LIST_ENTRY pointer want to convert.
  @param PtrPositive            The pointer need positive fix.
  @param PtrDelta               The pointer delta value after memory init.

  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
PeiConvertListPointer (
  IN  LIST_ENTRY                          *Link,
  IN  BOOLEAN                             PtrPositive,
  IN  UINTN                               PtrDelta
  )
{
  if (PtrPositive) {
    Link->ForwardLink = (LIST_ENTRY *) ((UINTN) Link->ForwardLink + PtrDelta);
    Link->BackLink    = (LIST_ENTRY *) ((UINTN) Link->BackLink + PtrDelta);
  } else {
    Link->ForwardLink = (LIST_ENTRY *) ((UINTN) Link->ForwardLink - PtrDelta);
    Link->BackLink    = (LIST_ENTRY *) ((UINTN) Link->BackLink - PtrDelta);
  }

  return EFI_SUCCESS;
}

/**
  Covert general pointer from CAR to RAM after memory init.

  @param Ptr                    The VOID pointer want to convert.
  @param PtrPositive            The pointer need positive fix.
  @param PtrDelta               The pointer delta value after memory init.

  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
PeiConvertVoidPointer (
  IN  VOID                                **Ptr,
  IN  BOOLEAN                             PtrPositive,
  IN  UINTN                               PtrDelta
  )
{
  if (PtrPositive) {
    *Ptr = (VOID *) ((UINTN)*Ptr + PtrDelta);
  } else {
    *Ptr = (VOID *) ((UINTN)*Ptr - PtrDelta);
  }

  return EFI_SUCCESS;
}

/**
  Covert all pointers from CAR to RAM after memory init.

  @param This                   The uba Ppi pointer.
  @param PtrPositive            The pointer need positive fix.
  @param PtrDelta               The pointer delta value after memory init.

  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
PeiConvertDataPointer (
  IN  UBA_CONFIG_DATABASE_PPI             *This,
  IN  BOOLEAN                             PtrPositive,
  IN  UINTN                               PtrDelta
  )
{
  UBA_PEIM_PRIVATE_DATA                 *UbaPeimPrivate;
  UBA_BOARD_NODE                        *SkuNode;
  UBA_CONFIG_NODE                       *DataNode;
  LIST_ENTRY                            *DataListHead;
  LIST_ENTRY                            *DataLink;
  UbaPeimPrivate = NULL;
  SkuNode = NULL;
  DataNode = NULL;
  DataListHead = NULL;
  DataLink = NULL;

  UbaPeimPrivate = PRIVATE_DATA_FROM_PPI (This);
  if (UbaPeimPrivate->ThisAddress == (UINTN) This) {
    return EFI_SUCCESS;
  }

  PeiConvertVoidPointer ((VOID**) &UbaPeimPrivate->CurrentSku, PtrPositive, PtrDelta);

  SkuNode       = UbaPeimPrivate->CurrentSku;
  DataListHead  = &SkuNode->DataLinkHead;

  PeiConvertListPointer (DataListHead, PtrPositive, PtrDelta);

  DataLink = DataListHead->ForwardLink;
  while (DataListHead != DataLink) {

    PeiConvertListPointer (DataLink, PtrPositive, PtrDelta);
    DataNode = CONFIG_NODE_INSTANCE_FROM_THIS (DataLink);
    PeiConvertVoidPointer (&DataNode->Data, PtrPositive, PtrDelta);

    DataLink = DataLink->ForwardLink;
  }

  UbaPeimPrivate->ThisAddress = (UINTN) This;

  return EFI_SUCCESS;
}

/**
  Check whether the This pointer have been relocated after memory init,
  if true, we need also relocate our own pointers in the heap, because core won't
  do it for us.

  @param This                   The uba Ppi pointer.

  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
PeiCheckPointerRelocated (
  IN  UBA_CONFIG_DATABASE_PPI       *This
  )
{
  EFI_STATUS                        Status;
  UBA_PEIM_PRIVATE_DATA             *UbaPeimPrivate;
  BOOLEAN                           PtrPositive;
  UINTN                             PtrDelta;
  Status = EFI_SUCCESS;
  UbaPeimPrivate = NULL;
  PtrPositive = FALSE;
  PtrDelta = 0;

  UbaPeimPrivate = PRIVATE_DATA_FROM_PPI (This);
  if (UbaPeimPrivate->ThisAddress != (UINTN) This) {
    if ((UINTN) This > UbaPeimPrivate->ThisAddress) {
      PtrPositive = TRUE;
      PtrDelta    = (UINTN) This - UbaPeimPrivate->ThisAddress;
    } else {
      PtrPositive = FALSE;
      PtrDelta    = UbaPeimPrivate->ThisAddress - (UINTN) This;
    }
    Status = PeiConvertDataPointer (This, PtrPositive, PtrDelta);
  }

  return Status;
}

/**
  Set platform's GUID and user friendly name by BoardId.

  If the BoardId is not exist in database, it will create a new platform.

  @param This                   UBA Ppi instance.
  @param BoardId                The platform type, same define as Platform.h.
  @param BoardGuid              The GUID for this platform.
  @param BoardName              The user friendly name for this platform.

  @retval EFI_ALREADY_STARTED   Create new for an exist platform.
  @retval EFI_OUT_OF_RESOURCES  Resource not enough.
  @retval EFI_NOT_FOUND         Platform not found.
  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
EFIAPI
PeiUbaInit (
  IN  UBA_CONFIG_DATABASE_PPI             *This,
  IN  UINT32                              BoardId,
  IN  EFI_GUID                            *BoardGuid,    OPTIONAL
  IN  CHAR8                               *BoardName     OPTIONAL
  )
{
  EFI_STATUS                 Status;
  UBA_BOARD_NODE             *SkuNode;
  Status  = EFI_SUCCESS;
  SkuNode = NULL;

  PeiCheckPointerRelocated (This);
  Status = InternalInitSku (This, BoardId, BoardGuid, BoardName, &SkuNode);
  return Status;
}

/**
  Get platform's GUID and user friendly name by BoardId.

  This is used when you need a BoardGuid to Add/Get platform data

  Core will create a new platform for you if the BoardId is not
  recorded in database, and assgin a unique GUID for this platform.

  @param This                   uba Ppi instance.
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
PeiUbaGetSku (
  IN  UBA_CONFIG_DATABASE_PPI             *This,
  OUT UINT32                              *BoardId,
  OUT EFI_GUID                            *BoardGuid,    OPTIONAL
  OUT CHAR8                               *BoardName     OPTIONAL
  )
{
  EFI_STATUS                 Status;
  UBA_BOARD_NODE             *SkuNode;
  SkuNode = NULL;

  PeiCheckPointerRelocated (This);

  Status = InternalGetSkuNode (This, &SkuNode);

  if (!EFI_ERROR (Status)) {
    if (BoardId != NULL) {
      *BoardId = SkuNode->BoardId;
    }
    if (BoardName != NULL) {
      AsciiStrCpyS (BoardName, AsciiStrSize (SkuNode->BoardName) / sizeof (CHAR8), SkuNode->BoardName);
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

  @param This                   uba Ppi instance.
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
PeiUbaAddData (
  IN  UBA_CONFIG_DATABASE_PPI             *This,
  IN  EFI_GUID                            *ResId,
  IN  VOID                                *Data,
  IN  UINTN                               DataSize
  )
{
  EFI_STATUS                 Status;
  UBA_BOARD_NODE             *SkuNode;
  SkuNode = NULL;

  PeiCheckPointerRelocated (This);

  Status = InternalGetSkuNode (This, &SkuNode);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = InternalAddNewConfigData (This, ResId, Data, DataSize);
  return Status;
}

/**
  Get configuration data from uba configuration database.

  @param This                   uba Ppi instance.
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
PeiUbaGetData (
  IN  UBA_CONFIG_DATABASE_PPI             *This,
  IN  EFI_GUID                            *ResId,
  OUT VOID                                *Data,
  OUT UINTN                               *DataSize
  )
{
  EFI_STATUS                            Status;
  UBA_PEIM_PRIVATE_DATA                 *UbaPeimPrivate;
  UBA_BOARD_NODE                        *SkuNode;
  UbaPeimPrivate = NULL;
  SkuNode = NULL;

  if ((ResId == NULL) || (Data == NULL) || (DataSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  PeiCheckPointerRelocated (This);

  UbaPeimPrivate = PRIVATE_DATA_FROM_PPI (This);

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
  Internal function for creating a HOB for platform data,
  and copy all the platform information and configuration data to the HOB,
  it will be passed to DXE driver.

  @param SkuNode                The platform node in database.
  @param HobGuid                The Hob GUID want to build.

  @retval EFI_OUT_OF_RESOURCES  No enough resource.
  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
InternalCreateHobForSkuNode (
  IN UBA_BOARD_NODE                     *SkuNode,
  IN EFI_GUID                           *HobGuid
  )
{
  UINT8                                 *HobData;
  UBA_CONFIG_HOB_HEADER                 *HobHeader;
  UBA_CONFIG_HOB_FIELD                  *HobDataFieldStart;
  UINT8                                 *ConfigDataHobPtr;
  UINTN                                 DataLength;
  UINTN                                 Index;
  LIST_ENTRY                            *ListHead;
  LIST_ENTRY                            *Link;
  UBA_CONFIG_NODE                       *DataNode;
  HobData = NULL;
  HobHeader = NULL;
  HobDataFieldStart = NULL;
  ConfigDataHobPtr = NULL;
  DataLength = 0;
  ListHead = NULL;
  Link = NULL;
  DataNode = NULL;

  if (SkuNode->DataCount != 0) {
    DataLength = sizeof (UBA_CONFIG_HOB_HEADER) +
      sizeof (UBA_CONFIG_HOB_FIELD) * (SkuNode->DataCount - 1);

    //
    // Get config data size
    //
    ListHead = &SkuNode->DataLinkHead;
    for (Link = GetFirstNode (ListHead); !IsNull (ListHead, Link); Link = GetNextNode (ListHead, Link)) {
      DataNode = CONFIG_NODE_INSTANCE_FROM_THIS (Link);
      DataLength += DataNode->Size;
    }

    HobData = BuildGuidHob (HobGuid, DataLength);
    ASSERT (HobData != NULL);
    if (HobData == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    HobHeader         = (UBA_CONFIG_HOB_HEADER *) HobData;
    HobDataFieldStart = HobHeader->HobField;
    ConfigDataHobPtr  = (UINT8 *) HobDataFieldStart + (sizeof (UBA_CONFIG_HOB_FIELD) * SkuNode->DataCount);

    //
    // Copy our HOB header
    //
    HobHeader->Signature  = UBA_CONFIG_HOB_SIGNATURE;
    HobHeader->Version    = UBA_CONFIG_HOB_VERSION;
    HobHeader->HobLength  = DataLength;
    CopyGuid (&HobHeader->DataGuid, HobGuid);

    HobHeader->BoardId = SkuNode->BoardId;
    HobHeader->DataCount = SkuNode->DataCount;
    CopyGuid (&HobHeader->BoardGuid, &SkuNode->BoardGuid);
    CopyMem (HobHeader->BoardName, SkuNode->BoardName, AsciiStrSize (SkuNode->BoardName));

    //
    // Copy N * (UBA_CONFIG_HOB_NODE & UBA_CONFIG_HOB_NODE->Data)
    //
    Index     = 0;
    ListHead  = &SkuNode->DataLinkHead;
    for (Link = GetFirstNode (ListHead); !IsNull (ListHead, Link); Link = GetNextNode (ListHead, Link)) {
      DataNode = CONFIG_NODE_INSTANCE_FROM_THIS (Link);
      ASSERT (DataNode != NULL);
      if (DataNode == NULL) {
        return EFI_UNSUPPORTED;
      }

      HobDataFieldStart[Index].Size       = DataNode->Size;
      HobDataFieldStart[Index].Signature  = DataNode->Signature;
      HobDataFieldStart[Index].Version    = DataNode->Version;
      CopyGuid (&HobDataFieldStart[Index].ResId, &DataNode->ResId);

      CopyMem (ConfigDataHobPtr, DataNode->Data, DataNode->Size);

      //
      // Fix the config data pointer after memory copy
      // Set the pointer address into memory in HOB
      //
      HobDataFieldStart[Index].DataOffset = ((UINTN) ConfigDataHobPtr - (UINTN) HobHeader);

      Index ++;
      ConfigDataHobPtr += DataNode->Size;
    }
  }

  return EFI_SUCCESS;
}

/**
  End of PEI phase callback, we need build configuration data HOB in this callback,
  it will pass to DXE driver.

  @param PeiServices            The PEI service pointer.
  @param NotifyDescriptor       The notify descriptor.
  @param Ppi                    The PPI was notified.

  @retval EFI_OUT_OF_RESOURCES  No enough resource.
  @retval EFI_NOT_FOUND         Platform/data not found.
  @retval EFI_SUCCESS           Operation success.
**/
EFI_STATUS
EndOfPeiPpiNotifyCallback (
  IN EFI_PEI_SERVICES                   **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR          *NotifyDescriptor,
  IN VOID                               *Ppi
  )
{
  EFI_STATUS                      Status;
  UBA_CONFIG_DATABASE_PPI         *UbaConfigPpi;
  UBA_PEIM_PRIVATE_DATA           *UbaPeimPrivate;
  UBA_BOARD_NODE                  *SkuNode;
  UbaPeimPrivate = NULL;
  SkuNode = NULL;

  Status = PeiServicesLocatePpi (
             &gUbaConfigDatabasePpiGuid,
             0,
             NULL,
             (VOID**) &UbaConfigPpi
             );
  ASSERT_EFI_ERROR (Status);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  PeiCheckPointerRelocated (UbaConfigPpi);

  //
  // Build GUID data HOB for current platform configuration data
  //
  UbaPeimPrivate = PRIVATE_DATA_FROM_PPI (UbaConfigPpi);
  if (UbaPeimPrivate->CurrentSku != NULL) {
    SkuNode = UbaPeimPrivate->CurrentSku;
    Status = InternalCreateHobForSkuNode (SkuNode, &gUbaCurrentConfigHobGuid);
    ASSERT_EFI_ERROR (Status);
    if (Status != EFI_SUCCESS) {
      return Status;
    }
  }

  //
  // Other will not pass to DXE because it's useless
  //
  return EFI_SUCCESS;
}

EFI_PEI_NOTIFY_DESCRIPTOR    mNotifyList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiEndOfPeiSignalPpiGuid,
    EndOfPeiPpiNotifyCallback
  }
};

/**
  Entry point function for the PEIM

  @param FileHandle      Handle of the file being invoked.
  @param PeiServices     Describes the list of possible PEI Services.

  @return EFI_SUCCESS    If we installed our PPI
**/
EFI_STATUS
EFIAPI
UbaConfigDatabasePeimEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                      Status;
  UBA_PEIM_PRIVATE_DATA           *UbaPeimPrivate;
  UbaPeimPrivate = NULL;

  UbaPeimPrivate = AllocateZeroPool (sizeof (UBA_PEIM_PRIVATE_DATA));
  if (UbaPeimPrivate == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DEBUG ((DEBUG_INFO, "UbaConfigDatabasePeimEntry!\n"));
  UbaPeimPrivate->Signature  = UBA_BOARD_SIGNATURE;
  UbaPeimPrivate->Version    = UBA_BOARD_VERSION;

  UbaPeimPrivate->ConfigDataCount      = 0;
  UbaPeimPrivate->HandleCount          = 0;
  UbaPeimPrivate->ThisAddress          = (UINTN) &UbaPeimPrivate->UbaCfgDbPpi;

  UbaPeimPrivate->UbaPeimPpiList.Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  UbaPeimPrivate->UbaPeimPpiList.Guid  = &gUbaConfigDatabasePpiGuid;
  UbaPeimPrivate->UbaPeimPpiList.Ppi   = &UbaPeimPrivate->UbaCfgDbPpi;

  UbaPeimPrivate->UbaCfgDbPpi.Signature = UBA_CONFIG_PPI_SIGNATURE;
  UbaPeimPrivate->UbaCfgDbPpi.Version   = UBA_CONFIG_PPI_VERSION;

  UbaPeimPrivate->UbaCfgDbPpi.InitSku   = PeiUbaInit;
  UbaPeimPrivate->UbaCfgDbPpi.GetSku    = PeiUbaGetSku;

  UbaPeimPrivate->UbaCfgDbPpi.AddData   = PeiUbaAddData;
  UbaPeimPrivate->UbaCfgDbPpi.GetData   = PeiUbaGetData;

  //
  // Just produce our PPI
  //
  Status = PeiServicesInstallPpi (&UbaPeimPrivate->UbaPeimPpiList);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = PeiServicesNotifyPpi (&mNotifyList[0]);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

