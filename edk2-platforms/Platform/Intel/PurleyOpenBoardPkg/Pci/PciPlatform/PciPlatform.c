/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include "PciPlatform.h"
#include <Library/PcdLib.h>
#ifdef EFI_PCI_IOV_SUPPORT
#include "PciIovPlatformPolicy.h"
#endif

PCI_PLATFORM_PRIVATE_DATA mPciPrivateData;

BOOLEAN                   FirstCall = TRUE;
UINT8                     sSataRaidLoadEfiDriverOption;
UINT8                     SataRaidLoadEfiDriverOption;
UINT8                     BootNetworkOption;

/**

  Set the PciPolicy as EFI_RESERVE_ISA_IO_NO_ALIAS | EFI_RESERVE_VGA_IO_NO_ALIAS.

  @param This       -  The pointer to the Protocol itself.
  PciPolicy  -  the returned Policy.

  @retval EFI_UNSUPPORTED        -  Function not supported.
  @retval EFI_INVALID_PARAMETER  -  Invalid PciPolicy value.

**/
EFI_STATUS
EFIAPI
GetPlatformPolicy (
  IN  CONST EFI_PCI_PLATFORM_PROTOCOL         *This,
  OUT EFI_PCI_PLATFORM_POLICY           *PciPolicy
  )
{
  if (PciPolicy == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_UNSUPPORTED;
}

/**

  Return a PCI ROM image for the onboard device represented by PciHandle.

  @param This       -  Protocol instance pointer.
  PciHandle  -  PCI device to return the ROM image for.
  RomImage   -  PCI Rom Image for onboard device.
  RomSize    -  Size of RomImage in bytes.

  @retval EFI_SUCCESS    -  RomImage is valid.
  @retval EFI_NOT_FOUND  -  No RomImage.

**/
EFI_STATUS
EFIAPI
GetPciRom (
  IN  CONST EFI_PCI_PLATFORM_PROTOCOL   *This,
  IN  EFI_HANDLE                        PciHandle,
  OUT VOID                              **RomImage,
  OUT UINTN                             *RomSize
  )
{
  return EFI_NOT_FOUND;
}

/**

  GC_TODO: Add function description

  @param This      - GC_TODO: add argument description
  @param Function  - GC_TODO: add argument description
  @param Phase     - GC_TODO: add argument description

  @retval EFI_INVALID_PARAMETER - GC_TODO: Add description for return value
  @retval EFI_INVALID_PARAMETER - GC_TODO: Add description for return value
  @retval EFI_UNSUPPORTED - GC_TODO: Add description for return value
  @retval EFI_SUCCESS - GC_TODO: Add description for return value

**/
EFI_STATUS
EFIAPI
RegisterPciCallback (
  IN  EFI_PCI_CALLBACK_PROTOCOL               *This,
  IN  EFI_PCI_CALLBACK_FUNC                   Function,
  IN  EFI_PCI_ENUMERATION_PHASE               Phase
  )
{
  LIST_ENTRY     *NodeEntry;
  PCI_CALLBACK_DATA *PciCallbackData;

  if (Function == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if ( (Phase &  (EfiPciEnumerationDeviceScanning | EfiPciEnumerationBusNumberAssigned \
              | EfiPciEnumerationResourceAssigned)) == 0) {
    return EFI_INVALID_PARAMETER; 
  }
  //
  // Check if the node has been added
  //
  NodeEntry = GetFirstNode (&mPciPrivateData.PciCallbackList);
  while (!IsNull (&mPciPrivateData.PciCallbackList, NodeEntry)) {
    PciCallbackData = PCI_CALLBACK_DATA_FROM_LINK (NodeEntry);
    if (PciCallbackData->Function == Function) {
      return EFI_UNSUPPORTED;
    }

    NodeEntry = GetNextNode (&mPciPrivateData.PciCallbackList, NodeEntry);
  }

  PciCallbackData = NULL;
  PciCallbackData = AllocateZeroPool (sizeof (PCI_CALLBACK_DATA));
  ASSERT (PciCallbackData != NULL);

  if(PciCallbackData != NULL){
    PciCallbackData->Signature  = PCI_CALLBACK_DATA_SIGNATURE;
    PciCallbackData->Function   = Function;
    PciCallbackData->Phase      = Phase;
    InsertTailList (&mPciPrivateData.PciCallbackList, &PciCallbackData->Link);
    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }
}


/**

  Main Entry point of the Pci Platform Driver.
  
  @param ImageHandle  -  Handle to the image.
  @param SystemTable  -  Handle to System Table.
  
  @retval EFI_STATUS  -  Status of the function calling.

**/
EFI_STATUS
EFIAPI
PciPlatformDriverEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS  Status;

  ZeroMem (&mPciPrivateData, sizeof (mPciPrivateData));
  InitializeListHead (&mPciPrivateData.PciCallbackList);

  mPciPrivateData.PciPlatform.PlatformNotify          = PhaseNotify; 
  mPciPrivateData.PciPlatform.PlatformPrepController  = PlatformPrepController;
  mPciPrivateData.PciPlatform.GetPlatformPolicy       = GetPlatformPolicy;
  mPciPrivateData.PciPlatform.GetPciRom               = GetPciRom;
  mPciPrivateData.PciCallback.RegisterPciCallback     = RegisterPciCallback;
#ifdef EFI_PCI_IOV_SUPPORT
  mPciPrivateData.PciIovPlatform.GetSystemLowestPageSize = GetSystemLowestPageSize;
  mPciPrivateData.PciIovPlatform.GetPlatformPolicy       = GetIovPlatformPolicy;
#endif

  //
  // Install on a new handle					
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mPciPrivateData.PciPlatformHandle,
                  &gEfiPciPlatformProtocolGuid,
                  &mPciPrivateData.PciPlatform,
                  &gEfiPciCallbackProtocolGuid,
                  &mPciPrivateData.PciCallback,
#ifdef EFI_PCI_IOV_SUPPORT
                  &gEfiPciIovPlatformProtocolGuid,
                  &mPciPrivateData.PciIovPlatform,
#endif
                  NULL
                  );

  return Status;
}
