/** @file
  Usb3 Debug Port library instance

  Copyright (c) 2013 - 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>

#include <PiDxe.h>
#include <Library/SerialPortLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/PciLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/Usb3DebugPortParamLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SmmAccess2.h>
#include "Usb3DebugPortLibInternal.h"

extern EFI_SMRAM_DESCRIPTOR mSmramCheckRanges[MAX_SMRAM_RANGE];
extern UINTN                mSmramCheckRangeCount;
extern BOOLEAN              mUsb3InSmm;
extern UINT64               mUsb3MmioSize;
extern BOOLEAN              mUsb3GetCapSuccess;

GUID                        gUsb3DbgGuid =  USB3_DBG_GUID;

USB3_DEBUG_PORT_CONTROLLER  mUsb3DebugPort;
USB3_DEBUG_PORT_INSTANCE    *mUsb3Instance = NULL;

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
  EFI_PHYSICAL_ADDRESS        Address;
  UINT32                      Low;
  UINT32                      High;

  if (mUsb3DebugPort.Controller == 0) {
    mUsb3DebugPort.Controller = GetUsb3DebugPortController();
  }

  Bus = mUsb3DebugPort.PciAddress.Bus;
  Device = mUsb3DebugPort.PciAddress.Device;
  Function = mUsb3DebugPort.PciAddress.Function;

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
  EFI_PHYSICAL_ADDRESS                   XhcMmioBase;
  UINT64                                 CapabilityPointer;
  UINT32                                 Capability;
  BOOLEAN                                Flag;
  UINT8                                  Bus;
  UINT8                                  Device;
  UINT8                                  Function;
  UINT16                                 Command;
  USB3_DEBUG_PORT_CONTROLLER             UsbDebugPort;

  Instance = NULL;

  XhcMmioBase = GetXhciBaseAddress ();

  if ((XhcMmioBase == 0) || (XhcMmioBase == XHCI_BASE_ADDRESS_64_BIT_MASK)) {
    return NULL;
  }

  if (mUsb3Instance != NULL) {
    FixUsb3InstanceResource (mUsb3Instance, XhcMmioBase);
    return mUsb3Instance;
  }

  Command = GetXhciPciCommand ();

  UsbDebugPort.Controller = GetUsb3DebugPortController();
  Bus      = UsbDebugPort.PciAddress.Bus;
  Device   = UsbDebugPort.PciAddress.Device;
  Function = UsbDebugPort.PciAddress.Function;

  //
  // Set Command Register
  //
  if ((Command & EFI_PCI_COMMAND_MEMORY_SPACE) == 0) {
    PciWrite16(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET), Command | EFI_PCI_COMMAND_MEMORY_SPACE);
    PciRead16(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET));
  }

  //
  // Calculate capability offset from HCCPARAMS [16:31], in 32-bit words
  //
  CapabilityPointer = XhcMmioBase + (MmioRead32 ((UINTN)(XhcMmioBase + XHC_HCCPARAMS_OFFSET)) >> 16) * 4;

  //
  // Search XHCI debug capability
  //
  Flag = FALSE;
  Capability = MmioRead32 ((UINTN)CapabilityPointer);
  while (TRUE) {
    if ((Capability & XHC_CAPABILITY_ID_MASK) == PCI_CAPABILITY_ID_DEBUG_PORT) {
      Flag = TRUE;
      break;
    }
    if ((((Capability & XHC_NEXT_CAPABILITY_MASK) >> 8) & XHC_CAPABILITY_ID_MASK) == 0) {
      //
      // Reach the end of list, quit
      //
      break;
    }
    CapabilityPointer += ((Capability & XHC_NEXT_CAPABILITY_MASK) >> 8) * 4;
    Capability = MmioRead32 ((UINTN)CapabilityPointer);
  }

  if (Flag) {
    Instance = (USB3_DEBUG_PORT_INSTANCE *)(UINTN) MmioRead32 ((UINTN) (CapabilityPointer + XHC_DC_DCDDI2));

    if (Instance != NULL) {
      FixUsb3InstanceResource (Instance, XhcMmioBase);
    }
  }

  //
  // Restore Command Register
  //
  PciWrite16(PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), Command);

  return Instance;
}


/**
  Initialize USB3 debug port.

  This method invokes various internal functions to facilitate
  detection and initialization of USB3 debug port.

  @retval RETURN_SUCCESS        The USB3 debug port was initialized.
**/
RETURN_STATUS
EFIAPI
USB3Initialize (
  VOID
  )
{
  //
  // Leave it empty, we assume PEI phase already do initialization
  //
  return RETURN_SUCCESS;
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
  USB3_DEBUG_PORT_INSTANCE    UsbDbg;
  USB3_DEBUG_PORT_INSTANCE    *Instance;
  EFI_PHYSICAL_ADDRESS        Address;
  EFI_STATUS                  Status;

  if ((gST == NULL) || (gBS == NULL)) {
    //
    // gST and gBS have not been initialized yet
    //
    return EFI_DEVICE_ERROR;
  }

  Status = EfiGetSystemConfigurationTable (&gUsb3DbgGuid, (VOID **) &mUsb3Instance);
  if (!EFI_ERROR (Status)) {
    return RETURN_SUCCESS;
  }

  //
  // It is first time to run DXE instance, copy Instance from Hob to ACPINvs
  // NOTE: Hob is not ready at this time, so copy it from XHCI register.
  //
  Instance = GetUsb3DebugPortInstance ();
  if (Instance == NULL) {
    //
    // Initialize USB debug
    //
    SetMem (&UsbDbg, sizeof(UsbDbg), 0);
    DiscoverUsb3DebugPort (&UsbDbg);
    if (UsbDbg.DebugSupport) {
      InitializeUsb3DebugPort (&UsbDbg);
    }
    Instance = &UsbDbg;
  }
  Address = SIZE_4GB;
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIMemoryNVS,
                  EFI_SIZE_TO_PAGES (sizeof (USB3_DEBUG_PORT_INSTANCE)),
                  &Address
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  CopyMem (
    (VOID *)(UINTN)Address,
    Instance,
    sizeof (USB3_DEBUG_PORT_INSTANCE)
    );
  mUsb3Instance = (USB3_DEBUG_PORT_INSTANCE *)(UINTN)Address;

  Status = gBS->InstallConfigurationTable (&gUsb3DbgGuid, mUsb3Instance);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (mUsb3Instance->DebugSupport) {
    SaveUsb3InstanceAddress (mUsb3Instance);
  }

  return RETURN_SUCCESS;
}

/**
  Calculate the size of XHCI MMIO space.

  @retval     TURE         The XHCI MMIO is in SMRAM ranges.
  @retval     FALSE        The XHCI MMIO is out of SMRAM ranges.
**/
UINT64
CalculateMmioSize (
  VOID
  )
{
  UINT8                       Bus;
  UINT8                       Device;
  UINT8                       Function;
  UINT32                      Value;
  UINT32                      Mask;
  UINT64                      MmioSize;
  UINT16                      Command;
  USB3_DEBUG_PORT_CONTROLLER  UsbDebugPort;
  EFI_PHYSICAL_ADDRESS        XhcMmioBase;

  UsbDebugPort.Controller = GetUsb3DebugPortController();
  Bus      = UsbDebugPort.PciAddress.Bus;
  Device   = UsbDebugPort.PciAddress.Device;
  Function = UsbDebugPort.PciAddress.Function;

  Mask     = 0xFFFFFFF0;
  MmioSize = 0;

  XhcMmioBase = GetXhciBaseAddress ();

  //
  // Disable MSE
  //
  Command = PciRead16 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET));
  PciWrite16 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), Command & ~(EFI_PCI_COMMAND_MEMORY_SPACE));

  //
  // Get Mmio Size
  //
  PciWrite32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET), 0xFFFFFFFF);
  Value    = PciRead32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET));

  switch (Value & 0x07) {
    case 0x0:
      //
      // Memory space: anywhere in 32 bit address space
      //
      MmioSize = (~(Value & Mask)) + 1;
      break;
    case 0x4:
      //
      // Memory space: anywhere in 64 bit address space
      //
      MmioSize = Value & Mask;
      PciWrite32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4), 0xFFFFFFFF);
      Value    = PciRead32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4));
      //
      // Fix the length to support some spefic 64 bit BAR
      //
      Value |= ((UINT32)(-1) << HighBitSet32 (Value));
      //
      // Calculate the size of 64bit bar
      //
      MmioSize  |= LShiftU64 ((UINT64) Value, 32);
      MmioSize  = (~(MmioSize)) + 1;
      break;
    default:
      //
      // Unknown BAR type
      //
      MmioSize = (~(Value & Mask)) + 1;
      break;
  };


  //
  // Restore MMIO address
  //
  PciWrite32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET), (UINT32)XhcMmioBase);
  PciWrite32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4), (UINT32) (XhcMmioBase >> 32));

  PciWrite16 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), Command | EFI_PCI_COMMAND_MEMORY_SPACE);

  return MmioSize;
}

/**
  The constructor function initialize USB3 debug port.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
Usb3DebugPortLibDxeConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_SMM_BASE2_PROTOCOL        *SmmBase;
  EFI_SMM_ACCESS2_PROTOCOL      *SmmAccess;
  UINTN                         Size;
  EFI_STATUS                    Status;

  //
  // Do real initialization here, because we need copy data from Hob to ACPINvs.
  // We must do it in constructor because it depends on UefiBootServicesTableLib.
  //
  if (FeaturePcdGet (PcdUsb3DebugFeatureEnable)) {
    USB3InitializeReal ();
  }

  mUsb3MmioSize = CalculateMmioSize ();

  if (gBS != NULL) {
    SmmBase = NULL;
    Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID **)&SmmBase);
    if (!EFI_ERROR (Status)) {
      SmmBase->InSmm(SmmBase, &mUsb3InSmm);
    }

    if (mUsb3InSmm) {
      //
      // Get SMRAM information
      //
      SmmAccess = NULL;
      Status = gBS->LocateProtocol (&gEfiSmmAccess2ProtocolGuid, NULL, (VOID **)&SmmAccess);
      if (!EFI_ERROR (Status)) {
        Size = sizeof (mSmramCheckRanges);

        Status = SmmAccess->GetCapabilities (SmmAccess, &Size, mSmramCheckRanges);
        if (!EFI_ERROR (Status)) {
          mSmramCheckRangeCount = Size / sizeof (EFI_SMRAM_DESCRIPTOR);
        }
      }
    }
  }

  return EFI_SUCCESS;
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
  VOID                    *Buf;
  EFI_PHYSICAL_ADDRESS    Address;
  EFI_STATUS              Status;

  Buf = NULL;

  if (gBS != NULL) {
    Address = 0xFFFFFFFF;
    Status = gBS->AllocatePages (
                    AllocateMaxAddress,
                    EfiACPIMemoryNVS,
                    EFI_SIZE_TO_PAGES (BufferSize),
                    &Address
                    );
    if (!EFI_ERROR (Status)) {
      Buf = (VOID *)(UINTN)Address;
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
  if (gBS != NULL) {
    return TRUE;
  }

  return FALSE;
}

