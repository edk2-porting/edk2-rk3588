/** @file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCI_HOST_BRIDGE_H_
#define _PCI_HOST_BRIDGE_H_


#include <PiDxe.h>
#include <IndustryStandard/Acpi.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciHostBridgeLib.h>
#include <Protocol/PciHostBridgeResourceAllocation.h>
#include <Protocol/IoMmu.h>

#include "PciRootBridge.h"

#define PCI_HOST_BRIDGE_SIGNATURE SIGNATURE_32 ('p', 'h', 'b', 'g')
typedef struct {
  UINTN                                             Signature;
  EFI_HANDLE                                        Handle;
  UINTN                                             RootBridgeCount;
  LIST_ENTRY                                        RootBridges;
  BOOLEAN                                           ResourceSubmited;
  BOOLEAN                                           CanRestarted;
  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL  ResAlloc;
} PCI_HOST_BRIDGE_INSTANCE;

#define PCI_HOST_BRIDGE_FROM_THIS(a) CR (a, PCI_HOST_BRIDGE_INSTANCE, ResAlloc, PCI_HOST_BRIDGE_SIGNATURE)

//
// Driver Entry Point
//
/**

  Entry point of this driver.

  @param ImageHandle  -  Image handle of this driver.
  @param SystemTable  -  Pointer to standard EFI system table.

  @retval EFI_SUCCESS       -  Succeed.
  @retval EFI_DEVICE_ERROR  -  Fail to install PCI_ROOT_BRIDGE_IO protocol.

**/
EFI_STATUS
EFIAPI
InitializePciHostBridge (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  );

//
//  HostBridge Resource Allocation interface
//
/**

  Enter a certain phase of the PCI enumeration process.

  @param This   The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL instance.
  @param Phase  The phase during enumeration.

  @retval EFI_SUCCESS            Succeed.
  @retval EFI_INVALID_PARAMETER  Wrong phase parameter passed in.
  @retval EFI_NOT_READY          Resources have not been submitted yet.

**/
EFI_STATUS
EFIAPI
NotifyPhase (
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL   *This,
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PHASE      Phase
  );

/**

  Return the device handle of the next PCI root bridge that is associated with
  this Host Bridge.

  @param This              The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_ PROTOCOL instance.
  @param RootBridgeHandle  Returns the device handle of the next PCI Root Bridge.
                           On input, it holds the RootBridgeHandle returned by the most
                           recent call to GetNextRootBridge().The handle for the first
                           PCI Root Bridge is returned if RootBridgeHandle is NULL on input.

  @retval EFI_SUCCESS            Succeed.
  @retval EFI_NOT_FOUND          Next PCI root bridge not found.
  @retval EFI_INVALID_PARAMETER  Wrong parameter passed in.

**/
EFI_STATUS
EFIAPI
GetNextRootBridge (
  IN     EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This,
  IN OUT EFI_HANDLE                                       *RootBridgeHandle
  );

/**

  Returns the attributes of a PCI Root Bridge.

  @param This              -  The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_ PROTOCOL instance
  @param RootBridgeHandle  -  The device handle of the PCI Root Bridge
                              that the caller is interested in
  @param Attributes        -  The pointer to attributes of the PCI Root Bridge

  @retval EFI_SUCCESS            -  Succeed.
  @retval EFI_INVALID_PARAMETER  -  Attributes parameter passed in is NULL or
                            @retval RootBridgeHandle is not an EFI_HANDLE
                            @retval that was returned on a previous call to
                            @retval GetNextRootBridge().

**/
EFI_STATUS
EFIAPI
GetAttributes (
  IN  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This,
  IN  EFI_HANDLE                                       RootBridgeHandle,
  OUT UINT64                                           *Attributes
  );

/**

  This is the request from the PCI enumerator to set up
  the specified PCI Root Bridge for bus enumeration process.

  @param This              -  The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_ PROTOCOL instance.
  @param RootBridgeHandle  -  The PCI Root Bridge to be set up.
  @param Configuration     -  Pointer to the pointer to the PCI bus resource descriptor.

  @retval EFI_SUCCESS            -  Succeed.
  @retval EFI_OUT_OF_RESOURCES   -  Not enough pool to be allocated.
  @retval EFI_INVALID_PARAMETER  -  RootBridgeHandle is not a valid handle.

**/
EFI_STATUS
EFIAPI
StartBusEnumeration (
  IN  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This,
  IN  EFI_HANDLE                                       RootBridgeHandle,
  OUT VOID                                             **Configuration
  );

/**

  This function programs the PCI Root Bridge hardware so that
  it decodes the specified PCI bus range.

  @param This              -  The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_ PROTOCOL instance.
  @param RootBridgeHandle  -  The PCI Root Bridge whose bus range is to be programmed.
  @param Configuration     -  The pointer to the PCI bus resource descriptor.

  @retval EFI_SUCCESS            -  Succeed.
  @retval EFI_INVALID_PARAMETER  -  Wrong parameters passed in.

**/
EFI_STATUS
EFIAPI
SetBusNumbers (
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This,
  IN EFI_HANDLE                                       RootBridgeHandle,
  IN VOID                                             *Configuration
  );

/**

  Submits the I/O and memory resource requirements for the specified PCI Root Bridge.

  @param This              -  The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_ PROTOCOL instance
  @param RootBridgeHandle  -  The PCI Root Bridge whose I/O and memory resource requirements
                              are being submitted
  @param Configuration     -  The pointer to the PCI I/O and PCI memory resource descriptor

  @retval EFI_SUCCESS            -  Succeed.
  @retval EFI_INVALID_PARAMETER  -  Wrong parameters passed in.

**/
EFI_STATUS
EFIAPI
SubmitResources (
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This,
  IN EFI_HANDLE                                       RootBridgeHandle,
  IN VOID                                             *Configuration
  );

/**

  This function returns the proposed resource settings for the specified
  PCI Root Bridge.

  @param This              -  The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_ PROTOCOL instance.
  @param RootBridgeHandle  -  The PCI Root Bridge handle.
  @param Configuration     -  The pointer to the pointer to the PCI I/O
                              and memory resource descriptor.

  @retval EFI_SUCCESS            -  Succeed.
  @retval EFI_OUT_OF_RESOURCES   -  Not enough pool to be allocated.
  @retval EFI_INVALID_PARAMETER  -  RootBridgeHandle is not a valid handle.

**/
EFI_STATUS
EFIAPI
GetProposedResources (
  IN  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This,
  IN  EFI_HANDLE                                       RootBridgeHandle,
  OUT VOID                                             **Configuration
  );

/**

  This function is called for all the PCI controllers that the PCI
  bus driver finds. Can be used to Preprogram the controller.

  @param This              -  The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_ PROTOCOL instance.
  @param RootBridgeHandle  -  The PCI Root Bridge handle.
  @param PciAddress        -  Address of the controller on the PCI bus.
  @param Phase             -  The Phase during resource allocation.

  @retval EFI_SUCCESS            -  Succeed.
  @retval EFI_INVALID_PARAMETER  -  RootBridgeHandle is not a valid handle.

**/
EFI_STATUS
EFIAPI
PreprocessController (
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL          *This,
  IN EFI_HANDLE                                                RootBridgeHandle,
  IN EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS               PciAddress,
  IN EFI_PCI_CONTROLLER_RESOURCE_ALLOCATION_PHASE              Phase
  );

//
// Host Bridge Silicon specific hooks
//
/**

  Returns the Allocation attributes for the BNB Root Bridge.

  @param RootBridgeIndex  -  The root bridge number. 0 based.

  @retval EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM | EFI_PCI_HOST_BRIDGE_MEM64_DECODE

**/
UINT64
GetAllocAttributes (
  IN  UINTN        RootBridgeIndex
  )
;

/**

  Returns memory apertures for the BNB Root Bridge.

  @param PciRootBridgeIo  -  Pointer to Efi Pci root bridge Io protocol interface instance.
  @param Mem32Base        -  Pointer to 32 bit memory base. This is the lowest 32 bit memory address
                             that is decoded by the Host Bridge.
  @param Mem32Limit       -  Pointer to 32 bit memory limit.This is the highest 32 bit memory address
                             that is decoded by the Host Bridge. The size of the 32 bit window is
                             (Mem32Limit - Mem32base + 1).
  @param Mem64Base        -  Pointer to 64 bit memory base. This is the lowest 64 bit memory address
                             that is decoded by the Host Bridge.
  @param Mem64Limit       -  Pointer to 64 bit memory limit.This is the highest 64 bit memory address
                             that is decoded by the Host Bridge. The size of the 64 bit window is
                             (Mem64Limit - Mem64base + 1). Set Mem64Limit < Mem64Base if the Host bridge
                             does not support 64 bit memory addresses.

  @retval EFI_SUCCESS  -  Success.

**/
EFI_STATUS
GetHostBridgeMemApertures (
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL               *PciRootBridgeIo,
  OUT UINT32                                        *Mem32Base,
  OUT UINT32                                        *Mem32Limit,
  OUT UINT64                                        *Mem64Base,
  OUT UINT64                                        *Mem64Limit
  );

/**

  Calculate maximum memory length that can be fit to a mtrr.

  @param MemoryLength  -  Input memory length.

  @retval Returned Maximum length.

**/
UINT64
Power2MaxMemory (
  IN UINT64         MemoryLength
  );

extern EDKII_IOMMU_PROTOCOL        *mIoMmu;

#endif // _PCI_HOST_BRIDGE_H_

