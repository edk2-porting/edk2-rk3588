/** @file
  Usb Debug Port library instance

  Copyright (c) 2013 - 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>

#include <PiPei.h>
#include <Library/SerialPortLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/Usb3DebugPortParamLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/IoMmu.h>
#include "Usb3DebugPortLibInternal.h"

GUID                      gUsb3DbgGuid =  USB3_DBG_GUID;

/**
  USB3 IOMMU PPI notify.

  @param[in] PeiServices    Pointer to PEI Services Table.
  @param[in] NotifyDesc     Pointer to the descriptor for the Notification event that
                            caused this function to execute.
  @param[in] Ppi            Pointer to the PPI data associated with this function.

  @retval EFI_STATUS        Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
Usb3IoMmuPpiNotify (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDesc,
  IN VOID                       *Ppi
  )
{
  USB3_DEBUG_PORT_INSTANCE      *Instance;

  Instance = GetUsb3DebugPortInstance ();
  ASSERT (Instance != NULL);
  //
  // Reinitialize USB3 debug port with granted DMA buffer from IOMMU PPI.
  //
  InitializeUsb3DebugPort (Instance);

  SaveUsb3InstanceAddress (Instance);

  return EFI_SUCCESS;
}

EFI_PEI_NOTIFY_DESCRIPTOR mUsb3IoMmuPpiNotifyDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiIoMmuPpiGuid,
  Usb3IoMmuPpiNotify
};

/**
  Allocates pages that are suitable for an OperationBusMasterCommonBuffer or
  OperationBusMasterCommonBuffer64 mapping.

  @param IoMmu                  Pointer to IOMMU PPI.
  @param Pages                  The number of pages to allocate.
  @param HostAddress            A pointer to store the base system memory address of the
                                allocated range.
  @param DeviceAddress          The resulting map address for the bus master PCI controller to use to
                                access the hosts HostAddress.
  @param Mapping                A resulting value to pass to Unmap().

  @retval EFI_SUCCESS           The requested memory pages were allocated.
  @retval EFI_UNSUPPORTED       Attributes is unsupported. The only legal attribute bits are
                                MEMORY_WRITE_COMBINE and MEMORY_CACHED.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The memory pages could not be allocated.

**/
EFI_STATUS
IoMmuAllocateBuffer (
  IN EDKII_IOMMU_PPI        *IoMmu,
  IN UINTN                  Pages,
  OUT VOID                  **HostAddress,
  OUT EFI_PHYSICAL_ADDRESS  *DeviceAddress,
  OUT VOID                  **Mapping
  )
{
  EFI_STATUS            Status;
  UINTN                 NumberOfBytes;

  *HostAddress = NULL;
  *DeviceAddress = 0;
  *Mapping = NULL;

  Status = IoMmu->AllocateBuffer (
                    IoMmu,
                    EfiRuntimeServicesData,
                    Pages,
                    HostAddress,
                    0
                    );
  if (EFI_ERROR (Status)) {
    return EFI_OUT_OF_RESOURCES;
  }

  NumberOfBytes = EFI_PAGES_TO_SIZE (Pages);
  Status = IoMmu->Map (
                    IoMmu,
                    EdkiiIoMmuOperationBusMasterCommonBuffer,
                    *HostAddress,
                    &NumberOfBytes,
                    DeviceAddress,
                    Mapping
                    );
  if (EFI_ERROR (Status)) {
    IoMmu->FreeBuffer (IoMmu, Pages, *HostAddress);
    *HostAddress = NULL;
    return EFI_OUT_OF_RESOURCES;
  }
  Status = IoMmu->SetAttribute (
                    IoMmu,
                    *Mapping,
                    EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE
                    );
  if (EFI_ERROR (Status)) {
    IoMmu->Unmap (IoMmu, *Mapping);
    IoMmu->FreeBuffer (IoMmu, Pages, *HostAddress);
    *Mapping = NULL;
    *HostAddress = NULL;
    return Status;
  }

  return Status;
}

/**
  USB3 get IOMMU PPI.

  @return Pointer to IOMMU PPI.

**/
EDKII_IOMMU_PPI *
Usb3GetIoMmu (
  VOID
  )
{
  EFI_STATUS                Status;
  EDKII_IOMMU_PPI           *IoMmu;
  CONST EFI_PEI_SERVICES    **PeiServices;

  PeiServices = GetPeiServicesTablePointer ();

  IoMmu = NULL;
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gEdkiiIoMmuPpiGuid,
                             0,
                             NULL,
                             (VOID **) &IoMmu
                             );
  if (!EFI_ERROR (Status) && (IoMmu != NULL)) {
    return IoMmu;
  }

  return NULL;
}

/**
  Return XHCI MMIO base address.

**/
EFI_PHYSICAL_ADDRESS
GetXhciBaseAddress (
  VOID
  )
{
  UINT8                       Bus;
  UINT8                       Device;
  UINT8                       Function;
  USB3_DEBUG_PORT_CONTROLLER   UsbDebugPort;
  EFI_PHYSICAL_ADDRESS        Address;
  UINT32                      Low;
  UINT32                      High;

  UsbDebugPort.Controller = GetUsb3DebugPortController();
  Bus = UsbDebugPort.PciAddress.Bus;
  Device = UsbDebugPort.PciAddress.Device;
  Function = UsbDebugPort.PciAddress.Function;
  Low = PciRead32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET));
  High = PciRead32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4));
  Address = (EFI_PHYSICAL_ADDRESS) (LShiftU64 ((UINT64) High, 32) | Low);

  //
  // Mask other parts which are not part of base address
  //
  Address &= XHCI_BASE_ADDRESS_64_BIT_MASK;
  return Address;
}

/**
  Return XHCI debug instance address.

**/
USB3_DEBUG_PORT_INSTANCE *
GetUsb3DebugPortInstance (
  VOID
  )
{
  USB3_DEBUG_PORT_INSTANCE               *Instance;
  EFI_PEI_HOB_POINTERS                   Hob;
  EFI_PHYSICAL_ADDRESS                   XhcMmioBase;

  Hob.Raw = GetFirstGuidHob (&gUsb3DbgGuid);
  if (Hob.Raw == NULL) {
    return NULL;
  }
  Instance = GET_GUID_HOB_DATA (Hob.Guid);

  //
  // Update XHCI MMIO base address
  //
  XhcMmioBase = GetXhciBaseAddress ();

  FixUsb3InstanceResource (Instance, XhcMmioBase);

  return Instance;
}

/**
  Initialize USB3 debug port.

  This method invokes various internal functions to facilitate
  detection and initialization of USB3 debug port.

  @retval RETURN_SUCCESS        The serial device was initialized.
**/
RETURN_STATUS
EFIAPI
USB3Initialize (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  Initialize USB3 debug port.

  This method invokes various internal functions to facilitate
  detection and initialization of USB3 debug port.

  @retval RETURN_SUCCESS        The serial device was initialized.
**/
RETURN_STATUS
EFIAPI
USB3InitializeReal (
  VOID
  )
{
  EFI_STATUS                Status;
  USB3_DEBUG_PORT_INSTANCE  UsbDbg;
  VOID                      *DataPtr;
  EFI_PEI_HOB_POINTERS      Hob;
  CONST EFI_PEI_SERVICES    **PeiServices;

  PeiServices = GetPeiServicesTablePointer ();

  //
  // USB Initilization has to be done only once. So this function should just return when
  // it is called more than once.
  //
  Hob.Raw = GetFirstGuidHob (&gUsb3DbgGuid);
  if (Hob.Raw != NULL) {
    return RETURN_SUCCESS;
  }

  //
  // Initialize USB debug for PEI at the first time
  //
  SetMem (&UsbDbg, sizeof(UsbDbg), 0);
  UsbDbg.FromHob = TRUE;
  DiscoverUsb3DebugPort (&UsbDbg);
  if (UsbDbg.DebugSupport) {
    InitializeUsb3DebugPort (&UsbDbg);
  }

  if (UsbDbg.Ready && (Usb3GetIoMmu () == NULL)) {
    Status = (*PeiServices)->NotifyPpi (PeiServices, &mUsb3IoMmuPpiNotifyDesc);
    ASSERT_EFI_ERROR (Status);
  }

  //
  // Save Instance into HOB
  //
  DataPtr = BuildGuidDataHob (
              &gUsb3DbgGuid,
              (VOID*) &UsbDbg,
              sizeof (UsbDbg)
              );

  if (UsbDbg.DebugSupport) {
    SaveUsb3InstanceAddress ((USB3_DEBUG_PORT_INSTANCE *) DataPtr);
  }

  return RETURN_SUCCESS;
}

/**
  Allocate aligned memory for XHC's usage.

  @param BufferSize     The size, in bytes, of the Buffer.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID*
AllocateAlignBuffer (
  IN UINTN                    BufferSize
  )
{
  VOID                     *Buf;
  EFI_PHYSICAL_ADDRESS     Address;
  CONST EFI_PEI_SERVICES   **PeiServices;
  EFI_STATUS               Status;
  VOID                     *MemoryDiscoveredPpi;
  EDKII_IOMMU_PPI          *IoMmu;
  VOID                     *HostAddress;
  VOID                     *Mapping;

  Buf = NULL;
  PeiServices = GetPeiServicesTablePointer ();

  //
  // Make sure the allocated memory is physical memory.
  //
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gEfiPeiMemoryDiscoveredPpiGuid,
                             0,
                             NULL,
                             (VOID **) &MemoryDiscoveredPpi
                             );
  if (!EFI_ERROR (Status)) {
    IoMmu = Usb3GetIoMmu ();
    if (IoMmu != NULL) {
      Status = IoMmuAllocateBuffer (
                 IoMmu,
                 EFI_SIZE_TO_PAGES (BufferSize),
                 &HostAddress,
                 &Address,
                 &Mapping
                 );
      if (!EFI_ERROR (Status)) {
        ASSERT (Address == ((EFI_PHYSICAL_ADDRESS) (UINTN) HostAddress));
        Buf = (VOID *)(UINTN) Address;
      }
    } else {
      Status = (*PeiServices)->AllocatePages (
                                 PeiServices,
                                 EfiACPIMemoryNVS,
                                 EFI_SIZE_TO_PAGES (BufferSize),
                                 &Address
                                 );
      if (!EFI_ERROR (Status)) {
        Buf = (VOID *)(UINTN) Address;
      }
    }
  }
  return Buf;
}

/**
  Check whether AllocatePages in permanent memory is ready.

  @retval TRUE  AllocatePages in permanent memory is ready.
  @retval FALSE AllocatePages in permanent memory is not ready.

**/
BOOLEAN
IsAllocatePagesReady (
  VOID
  )
{
  CONST EFI_PEI_SERVICES   **PeiServices;
  EFI_STATUS               Status;
  VOID                     *MemoryDiscoveredPpi;
  EDKII_IOMMU_PPI          *IoMmu;
  VOID                     *HostAddress;

  PeiServices = GetPeiServicesTablePointer ();

  //
  // Make sure the allocated memory is physical memory.
  //
  Status = (*PeiServices)->LocatePpi (
                             PeiServices,
                             &gEfiPeiMemoryDiscoveredPpiGuid,
                             0,
                             NULL,
                             (VOID **) &MemoryDiscoveredPpi
                             );
  if (!EFI_ERROR (Status)) {
    Status = (*PeiServices)->LocatePpi (
                               PeiServices,
                               &gEdkiiIoMmuPpiGuid,
                               0,
                               NULL,
                               (VOID **) &IoMmu
                               );
    if (!EFI_ERROR (Status)) {
      Status = IoMmu->AllocateBuffer (
                        IoMmu,
                        EfiRuntimeServicesData,
                        1,
                        &HostAddress,
                        0
                        );
      if (EFI_ERROR (Status)) {
        //
        // DMA protection has been enabled,
        // but DMA buffer could not be allocated yet.
        //
        return FALSE;
      }
      IoMmu->FreeBuffer (
               IoMmu,
               1,
               HostAddress
               );
    }
    return TRUE;
  }

  return FALSE;
}

