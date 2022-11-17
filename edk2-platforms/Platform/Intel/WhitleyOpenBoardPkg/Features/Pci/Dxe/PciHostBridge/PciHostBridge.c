/** @file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Guid/SocketPciResourceData.h>
#include <Guid/SocketIioVariable.h>
#include "PciHostBridge.h"
#include "PciRootBridge.h"
#include "PciHostResource.h"

#include <CpuAndRevisionDefines.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/IioUds.h>
#include <Upi/KtiHost.h>

#include <Library/SetupLib.h>

#include "PciRebalance.h"


/******************************************************************************
 * Local definitions.
 ******************************************************************************/
typedef struct {
  PCI_ROOT_BRIDGE_INSTANCE        RootBridge;
  EFI_PCI_ROOT_BRIDGE_DEVICE_PATH RootDevPath;
} PCI_ROOT_BRIDGE_WITH_PATH;


/******************************************************************************
 * Variables.
 ******************************************************************************/
GLOBAL_REMOVE_IF_UNREFERENCED CHAR16 *mAcpiAddressSpaceTypeStr[] = {
  L"Mem", L"I/O", L"Bus"
};
GLOBAL_REMOVE_IF_UNREFERENCED CHAR16 *mPciResourceTypeStr[] = {
  L"I/O", L"Mem32", L"PMem32", L"Mem64", L"PMem64", L"Bus"
};

//
// We can hardcode the following for a Simple IIO -
// Root Bridge Count within the Host bridge
// Root Bridge's device path
// Root Bridge's resource appeture
//

STATIC EFI_PCI_ROOT_BRIDGE_DEVICE_PATH    mEfiPciRootBridgeDevicePathTemplate = {
      {{ACPI_DEVICE_PATH,
      ACPI_DP,
      {(UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
      (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)}},
      EISA_PNP_ID(0x0A03),
      0},
      {END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      {END_DEVICE_PATH_LENGTH,
      0}}
    };

STATIC EFI_HANDLE                         mDriverImageHandle;

EDKII_IOMMU_PROTOCOL                     *mIoMmu;
EFI_EVENT                                 mIoMmuEvent;
VOID                                     *mIoMmuRegistration;

EFI_IIO_UDS_PROTOCOL                     *mIioUds;
PCI_HOST_BRIDGE_INSTANCE                 *mHostBridge;

/**
  Event notification that is fired when IOMMU protocol is installed.

  @param  Event                 The Event that is being processed.
  @param  Context               Event Context.

**/
VOID
EFIAPI
IoMmuProtocolCallback (
  IN  EFI_EVENT       Event,
  IN  VOID            *Context
  )
{
  EFI_STATUS   Status;

  Status = gBS->LocateProtocol (&gEdkiiIoMmuProtocolGuid, NULL, (VOID **)&mIoMmu);
  if (!EFI_ERROR(Status)) {
    gBS->CloseEvent (mIoMmuEvent);
  }
}


/**

  Entry point of this driver.

  @param ImageHandle  Image handle of this driver.
  @param SystemTable  Pointer to standard EFI system table.

  @retval EFI_SUCCESS       Succeed.
  @retval EFI_DEVICE_ERROR  Fail to install PCI_ROOT_BRIDGE_IO protocol.

**/
EFI_STATUS
EFIAPI
InitializePciHostBridge (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                  Status;
  UINTN                       IioIndex;
  EFI_PHYSICAL_ADDRESS        BaseAddress;
  UINTN                       IioStack;
  UINT64                      ResourceLength;
  PCI_ROOT_BRIDGE_RESOURCE_APERTURE  RbAperture;

  PCI_HOST_BRIDGE_INSTANCE          *HostBridge;
  PCI_ROOT_BRIDGE_INSTANCE          *RootBridge;
  EFI_PCI_ROOT_BRIDGE_DEVICE_PATH   *TempDevicePath;
  STACK_RES                         *CurStackRes;

  mDriverImageHandle = ImageHandle;
  //
  // Located the IIO Protocol Interface
  //
  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, &mIioUds);
  ASSERT_EFI_ERROR (Status);

  //
  // Most systems in the world including complex servers have only one Host Bridge.
  //
  HostBridge = AllocateZeroPool (sizeof (PCI_HOST_BRIDGE_INSTANCE));
  if (HostBridge == NULL) {
    ASSERT(FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  HostBridge->Signature         = PCI_HOST_BRIDGE_SIGNATURE;
  HostBridge->RootBridgeCount   = 0;
  HostBridge->ResourceSubmited  = FALSE;
  HostBridge->CanRestarted      = TRUE;
  InitializeListHead (&HostBridge->RootBridges);

  HostBridge->ResAlloc.NotifyPhase          = NotifyPhase;
  HostBridge->ResAlloc.GetNextRootBridge    = GetNextRootBridge;
  HostBridge->ResAlloc.GetAllocAttributes   = GetAttributes;
  HostBridge->ResAlloc.StartBusEnumeration  = StartBusEnumeration;
  HostBridge->ResAlloc.SetBusNumbers        = SetBusNumbers;
  HostBridge->ResAlloc.SubmitResources      = SubmitResources;
  HostBridge->ResAlloc.GetProposedResources = GetProposedResources;
  HostBridge->ResAlloc.PreprocessController = PreprocessController;

  Status = gBS->InstallProtocolInterface (
                    &HostBridge->Handle,
                    &gEfiPciHostBridgeResourceAllocationProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &HostBridge->ResAlloc
                    );
  if (EFI_ERROR (Status)) {
    FreePool (HostBridge);
    return EFI_DEVICE_ERROR;
  }

  //
  // Create Root Bridges in this Host Bridge.
  //
  for (IioIndex = 0; IioIndex < mIioUds->IioUdsPtr->PlatformData.numofIIO; IioIndex++) {

    if (!mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].Valid) {
      continue;
    } // No guarantee that the valid IIOs are sequential starting at 0!

    for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {

      if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[IioIndex].stackPresentBitmap & (1 << IioStack))) {
        continue;
      }
      // Skip stack which is disabled by resource limitation or DEBUG purpose!
      if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].Personality >= TYPE_DISABLED) {
        continue;
      }
      //
      // Create Root Bridge Instance
      //
      RootBridge = AllocateZeroPool (sizeof(PCI_ROOT_BRIDGE_WITH_PATH));
      if (RootBridge == NULL) {
        ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
        return EFI_OUT_OF_RESOURCES;
      }
      TempDevicePath = &((PCI_ROOT_BRIDGE_WITH_PATH*)RootBridge)->RootDevPath;

      RootBridge->Signature  = PCI_ROOT_BRIDGE_SIGNATURE;
      RootBridge->DevicePath = (EFI_DEVICE_PATH_PROTOCOL*)TempDevicePath;
      CopyMem (TempDevicePath, &mEfiPciRootBridgeDevicePathTemplate, sizeof(*TempDevicePath));
      TempDevicePath->AcpiDevicePath.UID = (UINT32)(IioIndex * MAX_IIO_STACK + IioStack);

      ZeroMem(&RbAperture, sizeof(RbAperture));
      {
        //
        // Update Root Bridge with UDS resource information
        //
        RbAperture.BusBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].BusBase;
        RbAperture.BusLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].BusLimit;
        RbAperture.Mem32Base = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceMem32Base;
        RbAperture.Mem32Limit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceMem32Limit;
        RbAperture.BusReserve = 0;
        if ((mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceMem64Limit >
             mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceMem64Base) &&
            (mIioUds->IioUdsPtr->PlatformData.Pci64BitResourceAllocation)) {
          RbAperture.Mem64Base = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceMem64Base;
          RbAperture.Mem64Limit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceMem64Limit;
        }
        RbAperture.IoBase = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceIoBase;
        RbAperture.IoLimit = mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceIoLimit;

        SimpleIioRootBridgeConstructor (&RootBridge->RootBridgeIo, HostBridge->Handle, &RbAperture,
                                        mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[IioIndex].PcieSegment,
                                        GetAllocAttributes (HostBridge->RootBridgeCount));
        DEBUG ((DEBUG_INFO, "[PCI] Create Root Bridge for [%d.%d] in PCI segment %d with below resource:\n", IioIndex, IioStack, RootBridge->RootBridgeIo.SegmentNumber));
        DEBUG ((DEBUG_INFO, "      Bus:   Base = 0x%08x, Limit = 0x%08x, BusReserve = 0x%08x\n", RootBridge->Aperture.BusBase, RootBridge->Aperture.BusLimit, RootBridge->Aperture.BusReserve));
        DEBUG ((DEBUG_INFO, "      IO:    Base = 0x%08x, Limit = 0x%08x\n", RootBridge->Aperture.IoBase, RootBridge->Aperture.IoLimit));
        DEBUG ((DEBUG_INFO, "      Mem32: Base = 0x%08x, Limit = 0x%08x\n", RootBridge->Aperture.Mem32Base, RootBridge->Aperture.Mem32Limit));
        DEBUG ((DEBUG_INFO, "      Mem64: Base = 0x%lx, Limit = 0x%lx\n", RootBridge->Aperture.Mem64Base, RootBridge->Aperture.Mem64Limit));
        //
        // Insert Root Bridge Instance
        //
        Status = gBS->InstallMultipleProtocolInterfaces (
                        &RootBridge->Handle,
                        &gEfiDevicePathProtocolGuid,
                        RootBridge->DevicePath,
                        &gEfiPciRootBridgeIoProtocolGuid,
                        &RootBridge->RootBridgeIo,
                        NULL
                        );
        ASSERT_EFI_ERROR (Status);
        InsertTailList (&HostBridge->RootBridges, &RootBridge->Link);
        mPciRootBridgeTable[IioIndex][IioStack] = RootBridge;
        HostBridge->RootBridgeCount++;
      }
    } // for (IioStack...)
  } // for (IioIndex...)
  //
  // Add PCIE base into Runtime memory so that it can be reported in E820 table
  //
  Status = gDS->AddMemorySpace (
                  EfiGcdMemoryTypeMemoryMappedIo,
                  mIioUds->IioUdsPtr->PlatformData.PciExpressBase,
                  mIioUds->IioUdsPtr->PlatformData.PciExpressSize,
                  EFI_MEMORY_RUNTIME | EFI_MEMORY_UC
                  );
  ASSERT_EFI_ERROR(Status);

  BaseAddress = mIioUds->IioUdsPtr->PlatformData.PciExpressBase;

  Status = gDS->AllocateMemorySpace (
                  EfiGcdAllocateAddress,
                  EfiGcdMemoryTypeMemoryMappedIo,
                  0,
                  mIioUds->IioUdsPtr->PlatformData.PciExpressSize,
                  &BaseAddress,
                  ImageHandle,
                  NULL
                  );
  ASSERT_EFI_ERROR(Status);

  Status = gDS->SetMemorySpaceAttributes (
                  mIioUds->IioUdsPtr->PlatformData.PciExpressBase,
                  mIioUds->IioUdsPtr->PlatformData.PciExpressSize,
                  EFI_MEMORY_RUNTIME
                  );
  ASSERT_EFI_ERROR (Status);

  //s4030180
  for (IioIndex = 0; IioIndex < mIioUds->IioUdsPtr->PlatformData.numofIIO; IioIndex++) {

    if (!mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].Valid) {
      continue;
    } // No guarantee that the valid IIOs are sequential starting at 0!
    for (IioStack = 0; IioStack < MAX_IIO_STACK; IioStack++) {
      if (!(mIioUds->IioUdsPtr->PlatformData.CpuQpiInfo[IioIndex].stackPresentBitmap & (1 << IioStack))) {
        continue;
      }

      if (mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceIoLimit >
           mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceIoBase) {
        //
        // At present, we use up the first 4k for fixed ranges like
        // ICH GPIO, ACPI and ISA devices. The first 4k is not
        // tracked through GCD. It should be.
        //
        Status = gDS->AddIoSpace (
                        EfiGcdIoTypeIo,
                        mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceIoBase ,
                        (mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceIoLimit -
                         mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceIoBase + 1)
                        );
        ASSERT_EFI_ERROR (Status);
      }

      CurStackRes = &mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack];
      if (CurStackRes->PciResourceMem32Limit > CurStackRes->PciResourceMem32Base) {
        //
        // Shouldn't the capabilities be UC?
        //
        ResourceLength = CurStackRes->PciResourceMem32Limit - CurStackRes->PciResourceMem32Base + 1;
        Status = gDS->AddMemorySpace (
          EfiGcdMemoryTypeMemoryMappedIo,
          CurStackRes->PciResourceMem32Base,
          ResourceLength,
          0
          );
        ASSERT_EFI_ERROR (Status);
        //
        // Track VT-d bar in GCD.
        //
        // We now have a region of memory x-z associated with this stack
        // and there is also subregion y-z that is the VT-d space (x <= y <= z)
        BaseAddress = CurStackRes->VtdBarAddress;
        ResourceLength = CurStackRes->PciResourceMem32Limit - BaseAddress + 1;
        Status = gDS->AllocateMemorySpace (
          EfiGcdAllocateAddress,
          EfiGcdMemoryTypeMemoryMappedIo,
          0,
          ResourceLength,
          &BaseAddress,
          ImageHandle,
          NULL
          );
        ASSERT_EFI_ERROR(Status);
      }

      if ((mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceMem64Limit >
             mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceMem64Base) &&
             (mIioUds->IioUdsPtr->PlatformData.Pci64BitResourceAllocation)) {

        Status = gDS->AddMemorySpace (
                           EfiGcdMemoryTypeMemoryMappedIo,
                           mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceMem64Base ,
                           (mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceMem64Limit -
                            mIioUds->IioUdsPtr->PlatformData.IIO_resource[IioIndex].StackRes[IioStack].PciResourceMem64Base + 1),
                           0
                           );
        ASSERT_EFI_ERROR (Status);
      }
    }
  }

  if (!EFI_ERROR (Status)) {
    mIoMmuEvent = EfiCreateProtocolNotifyEvent (
                    &gEdkiiIoMmuProtocolGuid,
                    TPL_CALLBACK,
                    IoMmuProtocolCallback,
                    NULL,
                    &mIoMmuRegistration
                    );
  }
  mHostBridge = HostBridge;
  return EFI_SUCCESS;
}


/**
  Enter the EfiPciHostBridgeBeginEnumeration phase of the PCI(e)
  numeration process.

  @param This     The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL
                  instance.

  @retval EFI_SUCCESS
  @retval EFI_NOT_READY
*/
STATIC EFI_STATUS
BeginBridgeEnumeration (
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This
  )
{
  PCI_HOST_BRIDGE_INSTANCE *HostBridge;
  LIST_ENTRY               *List;
  PCI_ROOT_BRIDGE_INSTANCE *RootBridge;
  PCI_RESOURCE_TYPE        Index;

  Index = TypeMax;
  HostBridge = PCI_HOST_BRIDGE_FROM_THIS (This);

  if (HostBridge->CanRestarted) {
    //
    // Reset Root Bridge
    //
    List = HostBridge->RootBridges.ForwardLink;

    while (List != &HostBridge->RootBridges) {
      RootBridge = ROOT_BRIDGE_FROM_LINK (List);
      for (Index = TypeIo; Index < TypeMax; Index++) {
        RootBridge->ResAllocNode[Index].Type    = Index;
        RootBridge->ResAllocNode[Index].Base    = 0;
        RootBridge->ResAllocNode[Index].Length  = 0;
        RootBridge->ResAllocNode[Index].Status  = ResNone;
      } // for

      List = List->ForwardLink;
    } // while

    HostBridge->ResourceSubmited  = FALSE;
    HostBridge->CanRestarted      = TRUE;
  } else {
    //
    // Can not restart
    //
    return EFI_NOT_READY;
  } // if

  return EFI_SUCCESS;
}


/**
  Enter the EfiPciHostBridgeAllocateResources phase of the
  PCI(e) numeration process.

  @param[in] This The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL instance.

  @retval EFI_SUCCESS
  @retval EFI_NOT_READY
  @retval EFI_OUT_OF_RESOURCES
*/
STATIC EFI_STATUS
BridgeAllocateResources (
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This
  )
{
  PCI_HOST_BRIDGE_INSTANCE              *HostBridge;
  PCI_ROOT_BRIDGE_INSTANCE              *RootBridge;
  PCI_RESOURCE_TYPE                     Index;
  LIST_ENTRY                            *List;
  EFI_PHYSICAL_ADDRESS                  BaseAddress;
  UINT64                                AddrLen;
  UINTN                                 BitsOfAlignment;
  UINT64                                Alignment;
  EFI_STATUS                            Status;
  EFI_STATUS                            ReturnStatus;
  PCI_RESOURCE_TYPE                     Index1;
  PCI_RESOURCE_TYPE                     Index2;
  BOOLEAN                               ResNodeHandled[TypeMax];
  UINT64                                MaxAlignment;
  SOCKET_RESOURCE_ADJUSTMENT_RESULT     RatioAdjustResult;
  UINT8                                 Socket;
  UINT8                                 Stack;

  Index = TypeMax;
  HostBridge = PCI_HOST_BRIDGE_FROM_THIS (This);

  ReturnStatus = EFI_SUCCESS;
  if (HostBridge->ResourceSubmited) {

    List = HostBridge->RootBridges.ForwardLink;
    while (List != &HostBridge->RootBridges) {

      for (Index1 = TypeIo; Index1 < TypeBus; Index1++) {
        ResNodeHandled[Index1] = FALSE;
      }
      RootBridge = ROOT_BRIDGE_FROM_LINK (List);
      Socket = Stack = (UINT8)-1;
      PciRootBridge2SocketStack (RootBridge, &Socket, &Stack);
      for (Index1 = TypeIo; Index1 < TypeBus; Index1++) {

        if (RootBridge->ResAllocNode[Index1].Status == ResNone) {

          ResNodeHandled[Index1] = TRUE;

        } else {
          //
          // Allocate the resource node with max alignment at first
          //
          MaxAlignment = 0;
          Index        = TypeMax;
          for (Index2 = TypeIo; Index2 < TypeBus; Index2++) {

            if (ResNodeHandled[Index2]) {
              continue;
            }
            if (MaxAlignment <= RootBridge->ResAllocNode[Index2].Alignment) {
              MaxAlignment = RootBridge->ResAllocNode[Index2].Alignment;
              Index = Index2;
            }
          } // for

          if (Index < TypeMax) {

            ResNodeHandled[Index] = TRUE;
            Alignment = RootBridge->ResAllocNode[Index].Alignment;
            //
            // Get the number of '1' in Alignment.
            //
            for (BitsOfAlignment = 0; Alignment != 0; BitsOfAlignment++) {
              Alignment = RShiftU64 (Alignment, 1);
            }

            AddrLen   = RootBridge->ResAllocNode[Index].Length;
            Alignment = RootBridge->ResAllocNode[Index].Alignment;

            DEBUG ((DEBUG_INFO, "\n[%d.%d] Resource Type to assign :     %s\n", Socket, Stack, mPciResourceTypeStr[Index]));
            DEBUG ((DEBUG_INFO, "      Length to allocate:           %lx\n", RootBridge->ResAllocNode[Index].Length));
            DEBUG ((DEBUG_INFO, "      Alignment:                    %lx\n", Alignment));
            DEBUG ((DEBUG_INFO, "      Base Address:                 %lx\n", RootBridge->ResAllocNode[Index].Base));

            switch (Index) {
            case TypeIo:
              if (RootBridge->Aperture.IoBase < RootBridge->Aperture.IoLimit) {
                //
                // It is impossible for 0xFFFF Alignment for IO16
                //
                if (BitsOfAlignment >= 16) {
                  Alignment = 0;
                }
                BaseAddress = RootBridge->Aperture.IoBase;
                //
                // Have to make sure Alignment is handled seeing we are doing direct address allocation
                //
                if ((BaseAddress & ~(Alignment)) != BaseAddress) {
                  BaseAddress = ((BaseAddress + Alignment) & ~(Alignment));
                }

                while((BaseAddress + AddrLen) <= RootBridge->Aperture.IoLimit + 1) {
                  Status = gDS->AllocateIoSpace (EfiGcdAllocateAddress, EfiGcdIoTypeIo, BitsOfAlignment,
                    AddrLen, &BaseAddress, mDriverImageHandle, NULL );

                  if (!EFI_ERROR (Status)) {
                    RootBridge->ResAllocNode[Index].Base    = (UINT64) BaseAddress;
                    RootBridge->ResAllocNode[Index].Status  = ResAllocated;
                    goto TypeIoFound;
                  }

                  BaseAddress += (Alignment + 1);
                } // while
              } // if

              TypeIoFound:
              if (RootBridge->ResAllocNode[Index].Status  != ResAllocated) {
                //
                // No Room at the Inn for this resources request
                //
                ReturnStatus = EFI_OUT_OF_RESOURCES;
              } // if
              break;

              case TypeMem32:
                if (RootBridge->Aperture.Mem32Base < RootBridge->Aperture.Mem32Limit) {

                  BaseAddress = RootBridge->Aperture.Mem32Base;
                  //
                  // Have to make sure Alignment is handled seeing we are doing direct address allocation
                  //
                  if ((BaseAddress & ~(Alignment)) != BaseAddress) {
                    BaseAddress = ((BaseAddress + Alignment) & ~(Alignment));
                  }

                  while ((BaseAddress + AddrLen) <= RootBridge->Aperture.Mem32Limit + 1) {
                    DEBUG ((DEBUG_INFO, "      Attempting %s allocation at 0x%llX.....",
                            mPciResourceTypeStr[Index], BaseAddress));
                    Status = gDS->AllocateMemorySpace (EfiGcdAllocateAddress, EfiGcdMemoryTypeMemoryMappedIo,
                                                       BitsOfAlignment, AddrLen, &BaseAddress, mDriverImageHandle, NULL);
                    if (!EFI_ERROR (Status)) {
                      RootBridge->ResAllocNode[Index].Base    = (UINT64) BaseAddress;
                      RootBridge->ResAllocNode[Index].Status  = ResAllocated;
                      DEBUG ((DEBUG_INFO, "Passed\n"));
                      goto TypeMem32Found;
                    } // if
                    DEBUG ((DEBUG_INFO, "Failed\n"));
                    BaseAddress += (Alignment + 1);
                  } // while
                } // if

                TypeMem32Found:
                if (RootBridge->ResAllocNode[Index].Status  != ResAllocated) {
                  //
                  // No Room at the Inn for this resources request
                  //
                  ReturnStatus = EFI_OUT_OF_RESOURCES;
                }
                break;

              case TypePMem32:
                StartTypePMem32:
                if (RootBridge->Aperture.Mem32Base < RootBridge->Aperture.Mem32Limit) {
                  BaseAddress = RootBridge->Aperture.Mem32Limit + 1;
                  BaseAddress -= AddrLen;

                  //
                  // Have to make sure Alignment is handled seeing we are doing direct address allocation
                  //
                  if ((BaseAddress & ~(Alignment)) != BaseAddress) {
                    BaseAddress = ((BaseAddress) & ~(Alignment));
                  }

                  while (RootBridge->Aperture.Mem32Base <= BaseAddress) {

                    DEBUG ((DEBUG_INFO, "      Attempting %s allocation at 0x%llX.....",
                            mPciResourceTypeStr[Index], BaseAddress));
                    Status = gDS->AllocateMemorySpace (EfiGcdAllocateAddress, EfiGcdMemoryTypeMemoryMappedIo,
                                                       BitsOfAlignment, AddrLen, &BaseAddress, mDriverImageHandle, NULL);
                    if (!EFI_ERROR (Status)) {
                      RootBridge->ResAllocNode[Index].Base    = (UINT64) BaseAddress;
                      RootBridge->ResAllocNode[Index].Status  = ResAllocated;
                      DEBUG ((DEBUG_INFO, "Passed\n"));
                      goto TypePMem32Found;
                    }
                    DEBUG ((DEBUG_INFO, "Failed\n"));
                    BaseAddress -= (Alignment + 1);
                  } // while
                } // if

                TypePMem32Found:
                if (RootBridge->ResAllocNode[Index].Status  != ResAllocated) {
                  //
                  // No Room at the Inn for this resources request
                  //
                  ReturnStatus = EFI_OUT_OF_RESOURCES;
                }
                break;

              case TypeMem64:
              case TypePMem64:
                if ((RootBridge->Aperture.Mem64Limit > RootBridge->Aperture.Mem64Base) &&
                    (mIioUds->IioUdsPtr->PlatformData.Pci64BitResourceAllocation)) {

                  if (RootBridge->Aperture.Mem64Limit < AddrLen) {
                    RootBridge->ResAllocNode[Index].Status = ResNone;
                    goto TypeMem64Found;
                  }
                  BaseAddress = RootBridge->Aperture.Mem64Limit + 1;
                  BaseAddress -= AddrLen;

                  //
                  // Have to make sure Alignment is handled seeing we are doing direct address allocation
                  //
                  if ((BaseAddress & ~(Alignment)) != BaseAddress) {
                    BaseAddress = ((BaseAddress) & ~(Alignment));
                  }

                  while (RootBridge->Aperture.Mem64Base <= BaseAddress) {

                    DEBUG ((DEBUG_INFO, "      Attempting %s allocation at 0x%llX.....",
                            mPciResourceTypeStr[Index], BaseAddress));
                    Status = gDS->AllocateMemorySpace (EfiGcdAllocateAddress, EfiGcdMemoryTypeMemoryMappedIo,
                                                       BitsOfAlignment, AddrLen, &BaseAddress, mDriverImageHandle, NULL);
                    if (!EFI_ERROR (Status)) {
                      RootBridge->ResAllocNode[Index].Base    = (UINT64) BaseAddress;
                      RootBridge->ResAllocNode[Index].Status  = ResAllocated;
                      DEBUG ((DEBUG_INFO, "Passed\n"));
                      goto TypeMem64Found;
                    }
                    DEBUG ((DEBUG_INFO, "Failed\n"));
                    BaseAddress -= (Alignment + 1);
                  } // while

                  TypeMem64Found:
                  if (RootBridge->ResAllocNode[Index].Status != ResAllocated) {
                    //
                    // No Room at the Inn for this resources request
                    //
                    ReturnStatus = EFI_OUT_OF_RESOURCES;
                  }
                } else {
                  //
                  // If 64-bit resourcing is not available, and the requested size is not greater than the Mem32Limit, then try as PMem32
                  //
                  if (AddrLen >= RootBridge->Aperture.Mem32Limit + 1) {

                    DEBUG((DEBUG_ERROR, "      64-bit resource length 0x%llX > Mem32Limit (0x%llX). Failed!!\n", AddrLen, RootBridge->Aperture.Mem32Limit + 1));
                    goto TypeMem64Found; // Let it handle out-of-resources case, try MMIO rebalance

                  } else {

                    DEBUG((DEBUG_ERROR, "      64-bit resourcing not available. Try as PMem32\n"));
                    goto StartTypePMem32;
                  }
                }
                break;

              default:
                DEBUG ((EFI_D_ERROR, "[PCI] ERROR: Unhandled resource type (%d)\n", Index));
                break;
            } // End switch (Index)

            DEBUG ((DEBUG_INFO, "Resource Type Assigned:   %s\n", mPciResourceTypeStr[Index]));
            if (RootBridge->ResAllocNode[Index].Status == ResAllocated) {
              DEBUG ((DEBUG_INFO, "  Base Address Assigned: %lx\n", RootBridge->ResAllocNode[Index].Base));
              DEBUG ((DEBUG_INFO, "  Length Assigned:       %lx\n", RootBridge->ResAllocNode[Index].Length));
            } else {
              DEBUG ((DEBUG_ERROR, "  Resource Allocation failed!  There was no room at the inn\n"));
            }
          } else {
            //
            // Index >= TypeMax
            //
            ASSERT (FALSE);
          }
        }
      }
      List = List->ForwardLink;
    }

    if (ReturnStatus == EFI_OUT_OF_RESOURCES) {

      DEBUG ((DEBUG_ERROR, "[PCI] Resource allocation failed, rebalance resource allocation and reboot\n"));
      AdjustResourceAmongRootBridges (HostBridge, &RatioAdjustResult);
      if (RatioAdjustResult == SocketResourceRatioChanged) {
        DEBUG ((DEBUG_WARN, "[PCI] WARNING: Resource allocation failed. Adjusted the resource requests and resetting the system.\n"));
        gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);

      } else {

        DEBUG ((DEBUG_WARN, "[PCI] WARNING: Resource allocation failed, rebalance not possible, continue boot\n"));
      }
    }

    //
    // Set resource to zero for nodes where allocation fails
    //
    List = HostBridge->RootBridges.ForwardLink;
    while (List != &HostBridge->RootBridges) {
      RootBridge = ROOT_BRIDGE_FROM_LINK (List);
      for (Index = TypeIo; Index < TypeBus; Index++) {
        if (RootBridge->ResAllocNode[Index].Status != ResAllocated) {
          RootBridge->ResAllocNode[Index].Length = 0;
        }
      }
      List = List->ForwardLink;
    }
    return ReturnStatus;
  } else {
    return EFI_NOT_READY;
  }
  //
  // HostBridge->CanRestarted = FALSE;
  //
}


/**
  Enter the EfiPciHostBridgeFreeResources phase of the
  PCI(e) numeration process.

  @param This     The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL
                  instance.
  @retval EFI_SUCCESS   Things worked out
  @retval other         Failures as reported by functions leveraged
*/
STATIC EFI_STATUS
BridgeFreeResources (
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This
  )
{
  PCI_HOST_BRIDGE_INSTANCE              *HostBridge;
  PCI_ROOT_BRIDGE_INSTANCE              *RootBridge;
  PCI_RESOURCE_TYPE                     Index;
  LIST_ENTRY                            *List;
  EFI_PHYSICAL_ADDRESS                  BaseAddress;
  UINT64                                AddrLen;
  EFI_STATUS                            Status;
  EFI_STATUS                            ReturnStatus;

  HostBridge = PCI_HOST_BRIDGE_FROM_THIS (This);
  ReturnStatus = EFI_SUCCESS;
  List = HostBridge->RootBridges.ForwardLink;

  while (List != &HostBridge->RootBridges) {
    RootBridge = ROOT_BRIDGE_FROM_LINK (List);
    for (Index = TypeIo; Index < TypeBus; Index++) {
      if (RootBridge->ResAllocNode[Index].Status == ResAllocated) {
        AddrLen     = RootBridge->ResAllocNode[Index].Length;
        BaseAddress = (EFI_PHYSICAL_ADDRESS) RootBridge->ResAllocNode[Index].Base;

        switch (Index) {
        case TypeIo:
          Status = gDS->FreeIoSpace (BaseAddress, AddrLen);
          if (EFI_ERROR (Status)) {
            ReturnStatus = Status;
          }
          break;

        case TypeMem32:
          Status = gDS->FreeMemorySpace (BaseAddress, AddrLen);
          if (EFI_ERROR (Status)) {
            ReturnStatus = Status;
          }
          break;

        case TypePMem32:
          break;

        case TypeMem64:
          break;

        case TypePMem64:
          Status = gDS->FreeMemorySpace (BaseAddress, AddrLen);
          if (EFI_ERROR (Status)) {
            ReturnStatus = Status;
          }
          break;

        default:
          DEBUG ((DEBUG_ERROR, "[PCI] ERROR: Invalid resource type %d\n", Index));
          break;
        }

        RootBridge->ResAllocNode[Index].Type    = Index;
        RootBridge->ResAllocNode[Index].Base    = 0;
        RootBridge->ResAllocNode[Index].Length  = 0;
        RootBridge->ResAllocNode[Index].Status  = ResNone;
      }
    }

    List = List->ForwardLink;
  }

  HostBridge->ResourceSubmited  = FALSE;
  HostBridge->CanRestarted      = TRUE;
  return ReturnStatus;
}


/**
  Enter the EfiPciHostBridgeEndResourceAllocation phase of the
  PCI(e) numeration process.

  @param This     The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL
                  instance.
  @retval EFI_SUCCESS   Things worked out
*/
STATIC EFI_STATUS
PciHostBridgeEndResourceAllocation (
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This
  )
{
  return EFI_SUCCESS;
}


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
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This,
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PHASE    Phase
  )
{
  PCI_HOST_BRIDGE_INSTANCE              *HostBridge;

  HostBridge = PCI_HOST_BRIDGE_FROM_THIS (This);

  switch (Phase) {
  case EfiPciHostBridgeBeginEnumeration:
    return BeginBridgeEnumeration (This);

  case EfiPciHostBridgeEndEnumeration:
    return EFI_SUCCESS;

  case EfiPciHostBridgeBeginBusAllocation:
    //
    // No specific action is required here, can perform any chipset specific programing
    //
    HostBridge->CanRestarted = FALSE;
    break;

  case EfiPciHostBridgeEndBusAllocation:
    //
    // No specific action is required here, can perform any chipset specific programing
    //
    break;

  case EfiPciHostBridgeBeginResourceAllocation:
    //
    // No specific action is required here, can perform any chipset specific programing
    //
    break;

  case EfiPciHostBridgeAllocateResources:
    return BridgeAllocateResources (This);

  case EfiPciHostBridgeSetResources:
    //
    // HostBridgeInstance->CanRestarted = FALSE;
    //
    break;

  case EfiPciHostBridgeFreeResources:
    return BridgeFreeResources (This);

  case EfiPciHostBridgeEndResourceAllocation:
    return PciHostBridgeEndResourceAllocation (This);

  default:
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

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
  )
{
  BOOLEAN                   NoRootBridge;
  LIST_ENTRY                *List;
  PCI_HOST_BRIDGE_INSTANCE  *HostBridge;
  PCI_ROOT_BRIDGE_INSTANCE  *RootBridge;

  NoRootBridge = TRUE;
  HostBridge = PCI_HOST_BRIDGE_FROM_THIS (This);
  List = HostBridge->RootBridges.ForwardLink;

  while (List != &HostBridge->RootBridges) {
    NoRootBridge = FALSE;
    RootBridge = ROOT_BRIDGE_FROM_LINK (List);
    if (*RootBridgeHandle == NULL) {
      //
      // Return the first Root Bridge Handle of the Host Bridge
      //
      *RootBridgeHandle = RootBridge->Handle;
      return EFI_SUCCESS;
    } else {
      if (*RootBridgeHandle == RootBridge->Handle) {
        //
        // Get next if have
        //
        List = List->ForwardLink;
        if (List != &HostBridge->RootBridges) {
          RootBridge = ROOT_BRIDGE_FROM_LINK (List);
          *RootBridgeHandle   = RootBridge->Handle;
          return EFI_SUCCESS;
        } else {
          return EFI_NOT_FOUND;
        }
      }
    }

    List = List->ForwardLink;
    //
    // end while
    //
  }

  if (NoRootBridge) {
    return EFI_NOT_FOUND;
  } else {
    return EFI_INVALID_PARAMETER;
  }
}

/**

  Returns the attributes of a PCI Root Bridge.

  @param This              The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_ PROTOCOL instance.
  @param RootBridgeHandle  The device handle of the PCI Root Bridge
                           that the caller is interested in.
  @param Attributes        The pointer to attributes of the PCI Root Bridge.

  @retval EFI_SUCCESS            Succeed.
  @retval EFI_INVALID_PARAMETER  Attributes parameter passed in is NULL or
                                 RootBridgeHandle is not an EFI_HANDLE
                                 that was returned on a previous call to
                                 GetNextRootBridge().

**/
EFI_STATUS
EFIAPI
GetAttributes (
  IN  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This,
  IN  EFI_HANDLE                                       RootBridgeHandle,
  OUT UINT64                                           *Attributes
  )
{
  LIST_ENTRY                *Link;
  PCI_HOST_BRIDGE_INSTANCE  *HostBridge;
  PCI_ROOT_BRIDGE_INSTANCE  *RootBridge;

  if (Attributes == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HostBridge = PCI_HOST_BRIDGE_FROM_THIS (This);
  for (Link = GetFirstNode (&HostBridge->RootBridges)
      ; !IsNull (&HostBridge->RootBridges, Link)
      ; Link = GetNextNode (&HostBridge->RootBridges, Link)
      ) {
    RootBridge = ROOT_BRIDGE_FROM_LINK (Link);
    if (RootBridgeHandle == RootBridge->Handle) {
      *Attributes = RootBridge->AllocationAttributes;
      return EFI_SUCCESS;
    }
  }

  return EFI_INVALID_PARAMETER;
}

/**

  This is the request from the PCI enumerator to set up
  the specified PCI Root Bridge for bus enumeration process.

  @param This              The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_ PROTOCOL instance.
  @param RootBridgeHandle  The PCI Root Bridge to be set up.
  @param Configuration     Pointer to the pointer to the PCI bus resource descriptor.

  @retval EFI_SUCCESS            Succeed.
  @retval EFI_OUT_OF_RESOURCES   Not enough pool to be allocated.
  @retval EFI_INVALID_PARAMETER  RootBridgeHandle is not a valid handle.

**/
EFI_STATUS
EFIAPI
StartBusEnumeration (
  IN  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This,
  IN  EFI_HANDLE                                       RootBridgeHandle,
  OUT VOID                                             **Configuration
  )
{
  LIST_ENTRY                *List;
  PCI_HOST_BRIDGE_INSTANCE  *HostBridge;
  PCI_ROOT_BRIDGE_INSTANCE  *RootBridge;
  VOID                      *Buffer;
  UINT8                     *Temp;
  EFI_STATUS                Status;
  UINTN                     BusStart;
  UINTN                     BusEnd;
  UINT64                    BusReserve;

  HostBridge = PCI_HOST_BRIDGE_FROM_THIS (This);
  List = HostBridge->RootBridges.ForwardLink;

  while (List != &HostBridge->RootBridges) {

    RootBridge = ROOT_BRIDGE_FROM_LINK (List);
    if (RootBridgeHandle == RootBridge->Handle) {
      //
      // Set up the Root Bridge for Bus Enumeration
      //
      BusStart  = RootBridge->Aperture.BusBase;
      BusEnd    = RootBridge->Aperture.BusLimit;
      BusReserve = RootBridge->Aperture.BusReserve;
      //
      // Program the Hardware(if needed) if error return EFI_DEVICE_ERROR
      //
      Status = gBS->AllocatePool (
                      EfiBootServicesData,
                      sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR),
                      &Buffer
                      );
      if (EFI_ERROR (Status)) {
        return EFI_OUT_OF_RESOURCES;
      }

      Temp  = (UINT8 *) Buffer;

      ((EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Temp)->Desc                  = ACPI_ADDRESS_SPACE_DESCRIPTOR;
      ((EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Temp)->Len                   = 0x2B;
      ((EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Temp)->ResType               = ACPI_ADDRESS_SPACE_TYPE_BUS;
      ((EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Temp)->GenFlag               = 0;
      ((EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Temp)->SpecificFlag          = 0;
      ((EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Temp)->AddrSpaceGranularity  = 0;
      ((EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Temp)->AddrRangeMin          = BusStart;
      ((EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Temp)->AddrRangeMax          = BusReserve;
      ((EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Temp)->AddrTranslationOffset = 0;
      ((EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Temp)->AddrLen               = BusEnd - BusStart + 1;

      Temp = Temp + sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR);
      ((EFI_ACPI_END_TAG_DESCRIPTOR *) Temp)->Desc = ACPI_END_TAG_DESCRIPTOR;
      ((EFI_ACPI_END_TAG_DESCRIPTOR *) Temp)->Checksum = 0x0;

      *Configuration = Buffer;
      return EFI_SUCCESS;
    }

    List = List->ForwardLink;
  }

  return EFI_INVALID_PARAMETER;
}

/**

  This function programs the PCI Root Bridge hardware so that
  it decodes the specified PCI bus range.

  @param This              The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_ PROTOCOL instance.
  @param RootBridgeHandle  The PCI Root Bridge whose bus range is to be programmed.
  @param Configuration     The pointer to the PCI bus resource descriptor.

  @retval EFI_SUCCESS            Succeed.
  @retval EFI_INVALID_PARAMETER  Wrong parameters passed in.

**/
EFI_STATUS
EFIAPI
SetBusNumbers (
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This,
  IN EFI_HANDLE                                       RootBridgeHandle,
  IN VOID                                             *Configuration
  )
{
  LIST_ENTRY                *List;
  PCI_HOST_BRIDGE_INSTANCE  *HostBridge;
  PCI_ROOT_BRIDGE_INSTANCE  *RootBridge;
  UINT8                     *Ptr;
  UINTN                     BusStart;
  UINTN                     BusEnd;
  UINTN                     BusLen;

  if (Configuration == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Ptr = Configuration;

  //
  // Check the Configuration is valid
  //
  if (*Ptr != ACPI_ADDRESS_SPACE_DESCRIPTOR) {
    return EFI_INVALID_PARAMETER;
  }

  if (((EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Ptr)->ResType != ACPI_ADDRESS_SPACE_TYPE_BUS) {
    return EFI_INVALID_PARAMETER;
  }

  Ptr += sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR);
  if (*Ptr != ACPI_END_TAG_DESCRIPTOR) {
    return EFI_INVALID_PARAMETER;
  }

  HostBridge = PCI_HOST_BRIDGE_FROM_THIS (This);
  List = HostBridge->RootBridges.ForwardLink;
  Ptr = Configuration;

  while (List != &HostBridge->RootBridges) {

    RootBridge = ROOT_BRIDGE_FROM_LINK (List);
    if (RootBridgeHandle == RootBridge->Handle) {
      BusStart  = (UINTN) ((EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Ptr)->AddrRangeMin;
      BusLen    = (UINTN) ((EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Ptr)->AddrLen;
      BusEnd    = BusStart + BusLen - 1;

      if (BusStart > BusEnd) {
        return EFI_INVALID_PARAMETER;
      }

      if ((BusStart < RootBridge->Aperture.BusBase) || (BusEnd > RootBridge->Aperture.BusLimit)) {
        return EFI_INVALID_PARAMETER;
      }
      //
      // Update the Bus Range
      //
      RootBridge->ResAllocNode[TypeBus].Base    = BusStart;
      RootBridge->ResAllocNode[TypeBus].Length  = BusLen + RootBridge->Aperture.BusReserve;
      RootBridge->ResAllocNode[TypeBus].Status  = ResAllocated;
      RootBridge->BusScanCount++;
      if (RootBridge->BusScanCount > 0) {
        //
        // Only care about the 2nd PCI bus scanning
        //
        RootBridge->BusNumberAssigned = TRUE;
      }

      return EFI_SUCCESS;
    }

    List = List->ForwardLink;
  }

  return EFI_INVALID_PARAMETER;
}


/**

  Submits the I/O and memory resource requirements for the specified PCI Root Bridge.

  @param This              The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_ PROTOCOL instance.
  @param RootBridgeHandle  The PCI Root Bridge whose I/O and memory resource requirements.
                           are being submitted.
  @param Configuration     The pointer to the PCI I/O and PCI memory resource descriptor.

  @retval EFI_SUCCESS            Succeed.
  @retval EFI_INVALID_PARAMETER  Wrong parameters passed in.
**/
EFI_STATUS
EFIAPI
SubmitResources (
  IN EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This,
  IN EFI_HANDLE                                       RootBridgeHandle,
  IN VOID                                             *Configuration
  )
{
  LIST_ENTRY                        *List;
  PCI_HOST_BRIDGE_INSTANCE          *HostBridge;
  PCI_ROOT_BRIDGE_INSTANCE          *RootBridge;
  UINT8                             *Temp;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *ptr;
  UINT64                            AddrLen;
  UINT64                            Alignment;
  UINT64                            Value;

  //
  // Check the input parameter: Configuration
  //
  if (Configuration == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HostBridge = PCI_HOST_BRIDGE_FROM_THIS (This);
  List = HostBridge->RootBridges.ForwardLink;

  Temp = (UINT8 *) Configuration;
  while (List != &HostBridge->RootBridges) {

    RootBridge = ROOT_BRIDGE_FROM_LINK (List);
    if (RootBridgeHandle == RootBridge->Handle) {
      //
      // Check the resource descriptors.
      // If the Configuration includes one or more invalid resource descriptors, all the resource
      // descriptors are ignored and the function returns EFI_INVALID_PARAMETER.
      //
      for (; *Temp == ACPI_ADDRESS_SPACE_DESCRIPTOR; Temp += sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR)) {

        ptr = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Temp;
        //DEBUG ((DEBUG_ERROR, " ptr->ResType:%x", ptr->ResType));
        //DEBUG ((DEBUG_ERROR, " ptr->AddrLen:0x%llX AddrRangeMin:0x%llX AddrRangeMax:0x%llX\n",ptr->AddrLen,ptr->AddrRangeMin,ptr->AddrRangeMax));
        //
        // A zero-length resource request indicates the PCI root bridge doesn't require
        // any resources. Skip the check and proceed to the next descriptor.
        //
        if (ptr->AddrLen == 0) {
          continue;
        }

        switch (ptr->ResType) {
          case ACPI_ADDRESS_SPACE_TYPE_MEM:
            if (ptr->AddrSpaceGranularity != 32 && ptr->AddrSpaceGranularity != 64) {

              DEBUG ((DEBUG_ERROR, "[PCI] ERROR: Inavlid granularity %d for resource type %d\n",
                      ptr->AddrSpaceGranularity, ptr->ResType));
              return EFI_INVALID_PARAMETER;
            }
            if (ptr->AddrSpaceGranularity == 32 && ptr->AddrLen > 0xffffffff) {

              DEBUG ((DEBUG_ERROR, "[PCI] ERROR: Inavlid granularity %d for resource type %d of size 0x%llX\n",
                      ptr->AddrSpaceGranularity, ptr->ResType, ptr->AddrLen));
              return EFI_INVALID_PARAMETER;
            }
            //
            // If the PCI root bridge does not support separate windows for nonprefetchable and
            // prefetchable memory, then the PCI bus driver needs to include requests for
            // prefetchable memory in the nonprefetchable memory pool.
            //
            if ((RootBridge->AllocationAttributes & EFI_PCI_HOST_BRIDGE_COMBINE_MEM_PMEM) != 0 &&
                ((ptr->SpecificFlag & (BIT2 | BIT1)) != 0)) {
              return EFI_INVALID_PARAMETER;
            }
          case ACPI_ADDRESS_SPACE_TYPE_IO:
            //
            // Check alignment, it should be of the form 2^n-1
            //
            Value = Power2MaxMemory (ptr->AddrRangeMax + 1);
            if (Value != (ptr->AddrRangeMax + 1)) {
              CpuDeadLoop();
              return EFI_INVALID_PARAMETER;
            }
            break;
          case ACPI_ADDRESS_SPACE_TYPE_BUS:
          default:
            return EFI_INVALID_PARAMETER;
        }
      }
      if (*Temp != ACPI_END_TAG_DESCRIPTOR) {
        return EFI_INVALID_PARAMETER;
      }

      Temp = (UINT8 *) Configuration;
      for (; *Temp == ACPI_ADDRESS_SPACE_DESCRIPTOR; Temp += sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR)) {
        ptr = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Temp;

        //
        // If a PCI root bridge does not require any resources, a zero-length resource
        // request must be explicitly submitted.
        //
        if (ptr->AddrLen == 0) {
          HostBridge->ResourceSubmited = TRUE;
          continue;
        }

        switch (ptr->ResType) {
        case ACPI_ADDRESS_SPACE_TYPE_MEM:
          AddrLen   = (UINT64) ptr->AddrLen;
          Alignment = (UINT64) ptr->AddrRangeMax;
          if (ptr->AddrSpaceGranularity == 32) {
            if (ptr->SpecificFlag == 0x06) {
              //
              // Apply from GCD
              //
              RootBridge->ResAllocNode[TypePMem32].Status = ResSubmitted;
            } else {
              RootBridge->ResAllocNode[TypeMem32].Length    = AddrLen;
              RootBridge->ResAllocNode[TypeMem32].Alignment = Alignment;
              RootBridge->ResAllocNode[TypeMem32].Status    = ResRequested;
              HostBridge->ResourceSubmited                  = TRUE;
            }
          }

          if (ptr->AddrSpaceGranularity == 64) {
            if (ptr->SpecificFlag == 0x06) {
              RootBridge->ResAllocNode[TypePMem64].Status = ResSubmitted;
            } else {
              RootBridge->ResAllocNode[TypeMem64].Length    = AddrLen;
              RootBridge->ResAllocNode[TypeMem64].Alignment = Alignment;
              RootBridge->ResAllocNode[TypeMem64].Status    = ResSubmitted;
              HostBridge->ResourceSubmited                  = TRUE;
            }
          }
          break;

        case ACPI_ADDRESS_SPACE_TYPE_IO:
          AddrLen   = (UINT64) ptr->AddrLen;
          Alignment = (UINT64) ptr->AddrRangeMax;
          RootBridge->ResAllocNode[TypeIo].Length     = AddrLen;
          RootBridge->ResAllocNode[TypeIo].Alignment  = Alignment;
          RootBridge->ResAllocNode[TypeIo].Status     = ResRequested;
          HostBridge->ResourceSubmited                = TRUE;
          break;

        default:
          break;
        }
      }

      return EFI_SUCCESS;
    }
    List = List->ForwardLink;
  }

  return EFI_INVALID_PARAMETER;
}

/**

  This function returns the proposed resource settings for the specified
  PCI Root Bridge.

  @param This              The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_ PROTOCOL instance.
  @param RootBridgeHandle  The PCI Root Bridge handle.
  @param Configuration     The pointer to the pointer to the PCI I/O
                           and memory resource descriptor.

  @retval EFI_SUCCESS            Succeed.
  @retval EFI_OUT_OF_RESOURCES   Not enough pool to be allocated.
  @retval EFI_INVALID_PARAMETER  RootBridgeHandle is not a valid handle.

**/
EFI_STATUS
EFIAPI
GetProposedResources (
  IN  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL *This,
  IN  EFI_HANDLE                                       RootBridgeHandle,
  OUT VOID                                             **Configuration
  )
{
  LIST_ENTRY                        *List;
  PCI_HOST_BRIDGE_INSTANCE          *HostBridge;
  PCI_ROOT_BRIDGE_INSTANCE          *RootBridge;
  UINTN                             Index;
  UINTN                             Number;
  VOID                              *Buffer;
  UINT8                             *Temp;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *ptr;
  EFI_STATUS                        Status;
  UINT64                            ResStatus;

  Buffer  = NULL;
  Number  = 0;
  //
  // Get the Host Bridge Instance from the resource allocation protocol
  //
  HostBridge = PCI_HOST_BRIDGE_FROM_THIS (This);
  List = HostBridge->RootBridges.ForwardLink;

  //
  // Enumerate the root bridges in this Host bridge
  //
  while (List != &HostBridge->RootBridges) {

    RootBridge = ROOT_BRIDGE_FROM_LINK (List);
    if (RootBridgeHandle == RootBridge->Handle) {
      for (Index = 0; Index < TypeBus; Index++) {
        if (RootBridge->ResAllocNode[Index].Status != ResNone) {
          Number++;
        }
      }

      Status = gBS->AllocatePool (
                      EfiBootServicesData,
                      Number * sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR),
                      &Buffer
                      );

      if (EFI_ERROR (Status)) {
        return EFI_OUT_OF_RESOURCES;
      }

      ZeroMem (Buffer, sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) * Number + sizeof (EFI_ACPI_END_TAG_DESCRIPTOR));

      Temp = Buffer;
      for (Index = 0; Index < TypeBus; Index++) {
        if (RootBridge->ResAllocNode[Index].Status != ResNone) {
          ptr       = (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR *) Temp;
          ResStatus = RootBridge->ResAllocNode[Index].Status;

          switch (Index) {

          case TypeIo:
            //
            // Io
            //
            ptr->Desc                   = 0x8A;
            ptr->Len                    = 0x2B;
            ptr->ResType                = 1;
            ptr->GenFlag                = 0;
            ptr->SpecificFlag           = 0;
            ptr->AddrRangeMin           = RootBridge->ResAllocNode[Index].Base;
            ptr->AddrRangeMax           = 0;
            ptr->AddrTranslationOffset  = (ResStatus == ResAllocated) ? EFI_RESOURCE_SATISFIED : EFI_RESOURCE_LESS;
            ptr->AddrLen                = RootBridge->ResAllocNode[Index].Length;
            break;

          case TypeMem32:
            //
            // Memory 32
            //
            ptr->Desc                   = 0x8A;
            ptr->Len                    = 0x2B;
            ptr->ResType                = 0;
            ptr->GenFlag                = 0;
            ptr->SpecificFlag           = 0;
            ptr->AddrSpaceGranularity   = 32;
            ptr->AddrRangeMin           = RootBridge->ResAllocNode[Index].Base;
            ptr->AddrRangeMax           = 0;
            ptr->AddrTranslationOffset  = (ResStatus == ResAllocated) ? EFI_RESOURCE_SATISFIED : EFI_RESOURCE_LESS;
            ptr->AddrLen                = RootBridge->ResAllocNode[Index].Length;
            break;

          case TypePMem32:
            //
            // Prefetch memory 32
            //
            ptr->Desc                   = 0x8A;
            ptr->Len                    = 0x2B;
            ptr->ResType                = 0;
            ptr->GenFlag                = 0;
            ptr->SpecificFlag           = 6;
            ptr->AddrSpaceGranularity   = 32;
            ptr->AddrRangeMin           = 0;
            ptr->AddrRangeMax           = 0;
            ptr->AddrTranslationOffset  = EFI_RESOURCE_NONEXISTENT;
            ptr->AddrLen                = 0;
            break;

          case TypeMem64:
            //
            // Memory 64
            //
            ptr->Desc                   = 0x8A;
            ptr->Len                    = 0x2B;
            ptr->ResType                = 0;
            ptr->GenFlag                = 0;
            ptr->SpecificFlag           = 0;
            ptr->AddrSpaceGranularity   = 64;
            ptr->AddrRangeMin           = RootBridge->ResAllocNode[Index].Base;
            ptr->AddrRangeMax           = 0;
            ptr->AddrTranslationOffset  = (ResStatus == ResAllocated) ? EFI_RESOURCE_SATISFIED : EFI_RESOURCE_LESS;
            ptr->AddrLen                = RootBridge->ResAllocNode[Index].Length;
            break;

          case TypePMem64:
            //
            // Prefetch memory 64
            //
            ptr->Desc                   = 0x8A;
            ptr->Len                    = 0x2B;
            ptr->ResType                = 0;
            ptr->GenFlag                = 0;
            ptr->SpecificFlag           = 6;
            ptr->AddrSpaceGranularity   = 64;
            ptr->AddrRangeMin           = 0;
            ptr->AddrRangeMax           = 0;
            ptr->AddrTranslationOffset  = EFI_RESOURCE_NONEXISTENT;
            ptr->AddrLen                = 0;
            break;
            default:
              DEBUG ((EFI_D_INFO, "default case.\n"));  //Auto added. Please review.
              break;
          }

          Temp += sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR);
        }
      }

      ((EFI_ACPI_END_TAG_DESCRIPTOR *) Temp)->Desc      = 0x79;
      ((EFI_ACPI_END_TAG_DESCRIPTOR *) Temp)->Checksum  = 0x0;

      *Configuration = Buffer;

      return EFI_SUCCESS;
    }

    List = List->ForwardLink;
  }

  return EFI_INVALID_PARAMETER;
}

/**

  This function is called for all the PCI controllers that the PCI
  bus driver finds. Can be used to Preprogram the controller.

  @param This              The EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_ PROTOCOL instance.
  @param RootBridgeHandle  The PCI Root Bridge handle.
  @param PciAddress        Address of the controller on the PCI bus.
  @param Phase             The Phase during resource allocation.

  @retval EFI_SUCCESS            Succeed.
  @retval EFI_INVALID_PARAMETER  RootBridgeHandle is not a valid handle.

**/
EFI_STATUS
EFIAPI
PreprocessController (
  IN  EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL          *This,
  IN  EFI_HANDLE                                                RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS               PciAddress,
  IN  EFI_PCI_CONTROLLER_RESOURCE_ALLOCATION_PHASE              Phase
  )
{
  BOOLEAN                   RootBridgeFound;
  LIST_ENTRY                *List;
  PCI_HOST_BRIDGE_INSTANCE  *HostBridge;
  PCI_ROOT_BRIDGE_INSTANCE  *RootBridge;

  if (RootBridgeHandle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  RootBridgeFound     = FALSE;
  HostBridge = PCI_HOST_BRIDGE_FROM_THIS (This);
  List = HostBridge->RootBridges.ForwardLink;

  while (List != &HostBridge->RootBridges) {

    RootBridge = ROOT_BRIDGE_FROM_LINK (List);
    if (RootBridgeHandle == RootBridge->Handle) {
      RootBridgeFound = TRUE;
      break;
    }
    //
    // Get next if have
    //
    List = List->ForwardLink;
  }

  if (RootBridgeFound == FALSE) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/**

  Calculate maximum memory length that can be fit to a mtrr.

  @param MemoryLength  -  Input memory length.

  @retval Returned Maximum length.

**/
UINT64
Power2MaxMemory (
  IN UINT64                     MemoryLength
  )
{
  UINT64  Result;

  if (RShiftU64 (MemoryLength, 32)) {
    Result = LShiftU64 ((UINT64) GetPowerOfTwo64 ((UINT32) RShiftU64 (MemoryLength, 32)), 32);
  } else {
    Result = (UINT64) GetPowerOfTwo64 ((UINT32) MemoryLength);
  }

  return Result;
}

