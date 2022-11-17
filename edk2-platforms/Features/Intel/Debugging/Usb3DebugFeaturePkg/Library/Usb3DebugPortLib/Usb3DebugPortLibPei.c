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
#include "Usb3DebugPortLibInternal.h"

GUID                      gUsb3DbgGuid =  USB3_DBG_GUID;

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
  USB3_DEBUG_PORT_INSTANCE  UsbDbg;
  VOID                      *DataPtr;
  EFI_PEI_HOB_POINTERS      Hob;

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
    return TRUE;
  }

  return FALSE;
}

