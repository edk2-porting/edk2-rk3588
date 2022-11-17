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
#include <Protocol/IoMmu.h>
#include <Protocol/PciIo.h>
#include <Protocol/DxeSmmReadyToLock.h>
#include "Usb3DebugPortLibInternal.h"

extern EFI_SMRAM_DESCRIPTOR mSmramCheckRanges[MAX_SMRAM_RANGE];
extern UINTN                mSmramCheckRangeCount;
extern BOOLEAN              mUsb3InSmm;
extern UINT64               mUsb3MmioSize;
extern BOOLEAN              mUsb3GetCapSuccess;

GUID                        gUsb3DbgGuid =  USB3_DBG_GUID;

USB3_DEBUG_PORT_CONTROLLER  mUsb3DebugPort;
USB3_DEBUG_PORT_INSTANCE    *mUsb3Instance = NULL;
EFI_PCI_IO_PROTOCOL         *mUsb3PciIo = NULL;

/**
  Creates a named event that can be signaled.

  This function creates an event using NotifyTpl, NoifyFunction.
  If Name is NULL, then ASSERT().
  If NotifyTpl is not a legal TPL value, then ASSERT().
  If NotifyFunction is NULL, then ASSERT().

  @param  Name                  Supplies the GUID name of the event.
  @param  NotifyTpl             Supplies the task priority level of the event notifications.
  @param  NotifyFunction        Supplies the function to notify when the event is signaled.
  @param  Event                 A pointer to the event created.

  @retval EFI_SUCCESS           A named event was created.
  @retval EFI_OUT_OF_RESOURCES  There are not enough resource to create the named event.

**/
EFI_STATUS
EFIAPI
Usb3NamedEventListen (
  IN CONST EFI_GUID    *Name,
  IN EFI_TPL           NotifyTpl,
  IN EFI_EVENT_NOTIFY  NotifyFunction,
  IN EFI_EVENT         *Event
  )
{
  EFI_STATUS  Status;
  VOID        *RegistrationLocal;

  ASSERT (Name != NULL);
  ASSERT (NotifyFunction != NULL);
  ASSERT (NotifyTpl <= TPL_HIGH_LEVEL);

  //
  // Create event
  //
  Status = gBS->CreateEvent (
                  EVT_NOTIFY_SIGNAL,
                  NotifyTpl,
                  NotifyFunction,
                  NULL,
                  Event
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Register for an installation of protocol interface
  //
  Status = gBS->RegisterProtocolNotify (
                  (EFI_GUID *) Name,
                  *Event,
                  &RegistrationLocal
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  USB3 map one DMA buffer.

  @param PciIo          Pointer to PciIo for USB3 debug port.
  @param Address        DMA buffer address to be mapped.
  @param NumberOfBytes  Number of bytes to be mapped.

**/
VOID
Usb3MapOneDmaBuffer (
  IN EFI_PCI_IO_PROTOCOL        *PciIo,
  IN EFI_PHYSICAL_ADDRESS       Address,
  IN UINTN                      NumberOfBytes
  )
{
  EFI_STATUS                    Status;
  VOID                          *HostAddress;
  EFI_PHYSICAL_ADDRESS          DeviceAddress;
  VOID                          *Mapping;

  HostAddress = (VOID *) (UINTN) Address;
  Status = PciIo->Map (
                    PciIo,
                    EfiPciIoOperationBusMasterCommonBuffer,
                    HostAddress,
                    &NumberOfBytes,
                    &DeviceAddress,
                    &Mapping
                    );
  ASSERT_EFI_ERROR (Status);
  ASSERT (DeviceAddress == ((EFI_PHYSICAL_ADDRESS) (UINTN) HostAddress));
}

/**
  USB3 map DMA buffers.

  @param Instance       Pointer to USB3 debug port instance.
  @param PciIo          Pointer to PciIo for USB3 debug port.

**/
VOID
Usb3MapDmaBuffers (
  IN USB3_DEBUG_PORT_INSTANCE   *Instance,
  IN EFI_PCI_IO_PROTOCOL        *PciIo
  )
{
  Usb3MapOneDmaBuffer (
    PciIo,
    Instance->Urb.Data,
    XHC_DEBUG_PORT_DATA_LENGTH
    );

  Usb3MapOneDmaBuffer (
    PciIo,
    Instance->TransferRingIn.RingSeg0,
    sizeof (TRB_TEMPLATE) * TR_RING_TRB_NUMBER
    );

  Usb3MapOneDmaBuffer (
    PciIo,
    Instance->TransferRingOut.RingSeg0,
    sizeof (TRB_TEMPLATE) * TR_RING_TRB_NUMBER
    );

  Usb3MapOneDmaBuffer (
    PciIo,
    Instance->EventRing.EventRingSeg0,
    sizeof (TRB_TEMPLATE) * EVENT_RING_TRB_NUMBER
    );

  Usb3MapOneDmaBuffer (
    PciIo,
    Instance->EventRing.ERSTBase,
    sizeof (EVENT_RING_SEG_TABLE_ENTRY) * ERST_NUMBER
    );

  Usb3MapOneDmaBuffer (
    PciIo,
    Instance->DebugCapabilityContext,
    sizeof (XHC_DC_CONTEXT)
    );

  Usb3MapOneDmaBuffer (
    PciIo,
    ((XHC_DC_CONTEXT *) (UINTN) Instance->DebugCapabilityContext)->DbcInfoContext.String0DescAddress,
    STRING0_DESC_LEN + MANU_DESC_LEN + PRODUCT_DESC_LEN + SERIAL_DESC_LEN
    );
}

/**
  Invoke a notification event

  @param[in]  Event                 Event whose notification function is being invoked.
  @param[in]  Context               The pointer to the notification function's context,
                                    which is implementation-dependent.

**/
VOID
EFIAPI
Usb3DxeSmmReadyToLockNotify (
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
  )
{
  ASSERT (mUsb3Instance != NULL);

  //
  // For the case that the USB3 debug port instance and DMA buffers are
  // from PEI HOB with IOMMU enabled.
  // Reinitialize USB3 debug port with granted DXE DMA buffer accessible
  // by SMM environment.
  //
  InitializeUsb3DebugPort (mUsb3Instance);

  SaveUsb3InstanceAddress (mUsb3Instance);

  gBS->CloseEvent (Event);
}

/**
  USB3 get IOMMU protocol.

  @return Pointer to IOMMU protocol.

**/
EDKII_IOMMU_PROTOCOL *
Usb3GetIoMmu (
  VOID
  )
{
  EFI_STATUS                Status;
  EDKII_IOMMU_PROTOCOL      *IoMmu;

  IoMmu = NULL;
  Status = gBS->LocateProtocol (
             &gEdkiiIoMmuProtocolGuid,
             NULL,
             (VOID **) &IoMmu
             );
  if (!EFI_ERROR (Status) && (IoMmu != NULL)) {
    return IoMmu;
  }

  return NULL;
}

/**
  Invoke a notification event

  @param[in]  Event                 Event whose notification function is being invoked.
  @param[in]  Context               The pointer to the notification function's context,
                                    which is implementation-dependent.

**/
VOID
EFIAPI
Usb3PciIoNotify (
  IN  EFI_EVENT                Event,
  IN  VOID                     *Context
  )
{
  EFI_STATUS                    Status;
  UINTN                         PciIoHandleCount;
  EFI_HANDLE                    *PciIoHandleBuffer;
  UINTN                         Index;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINTN                         PciSegment;
  UINTN                         PciBusNumber;
  UINTN                         PciDeviceNumber;
  UINTN                         PciFunctionNumber;
  EFI_EVENT                     SmmReadyToLockEvent;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiPciIoProtocolGuid,
                  NULL,
                  &PciIoHandleCount,
                  &PciIoHandleBuffer
                  );
  if (!EFI_ERROR (Status) &&
      (PciIoHandleBuffer != NULL) &&
      (PciIoHandleCount != 0)) {
    for (Index = 0; Index < PciIoHandleCount; Index++) {
      Status = gBS->HandleProtocol (
                      PciIoHandleBuffer[Index],
                      &gEfiPciIoProtocolGuid,
                      (VOID **) &PciIo
                      );
      ASSERT_EFI_ERROR (Status);
      Status = PciIo->GetLocation (PciIo, &PciSegment, &PciBusNumber, &PciDeviceNumber, &PciFunctionNumber);
      ASSERT_EFI_ERROR (Status);
      if ((PciBusNumber == mUsb3DebugPort.PciAddress.Bus) &&
          (PciDeviceNumber == mUsb3DebugPort.PciAddress.Device) &&
          (PciFunctionNumber == mUsb3DebugPort.PciAddress.Function)) {
        //
        // Found the PciIo for USB3 debug port.
        //
        ASSERT (mUsb3Instance != NULL);
        if (Usb3GetIoMmu () != NULL) {
          Usb3MapDmaBuffers (mUsb3Instance, PciIo);

          if (mUsb3Instance->FromHob) {
            mUsb3PciIo = PciIo;
            Usb3NamedEventListen (
              &gEfiDxeSmmReadyToLockProtocolGuid,
              TPL_NOTIFY,
              Usb3DxeSmmReadyToLockNotify,
              &SmmReadyToLockEvent
              );
          }
        }
        gBS->CloseEvent (Event);
        break;
      }
    }

    gBS->FreePool (PciIoHandleBuffer);
  }
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
  EFI_EVENT                   Event;

  if ((gST == NULL) || (gBS == NULL)) {
    //
    // gST and gBS have not been initialized yet
    //
    return EFI_DEVICE_ERROR;
  }

  Status = EfiGetSystemConfigurationTable (&gUsb3DbgGuid, (VOID **) &mUsb3Instance);
  if (!EFI_ERROR (Status)) {
    goto Done;
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

Done:
  if ((mUsb3Instance != NULL) && mUsb3Instance->Ready && (mUsb3Instance->PciIoEvent == 0)) {
    Status = Usb3NamedEventListen (
               &gEfiPciIoProtocolGuid,
               TPL_NOTIFY,
               Usb3PciIoNotify,
               &Event
               );
    if (!EFI_ERROR (Status)) {
      mUsb3Instance->PciIoEvent = (EFI_PHYSICAL_ADDRESS) (UINTN) Event;
    }
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
  The destructor function.

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS   The destructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
Usb3DebugPortLibDxeDestructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  if ((mUsb3Instance != NULL) && (mUsb3Instance->PciIoEvent != 0)) {
    //
    // Close the event created.
    //
    gBS->CloseEvent ((EFI_EVENT) (UINTN) mUsb3Instance->PciIoEvent);
    mUsb3Instance->PciIoEvent = 0;
  }
  return EFI_SUCCESS;
}

/**
  Allocates pages that are suitable for an OperationBusMasterCommonBuffer or
  OperationBusMasterCommonBuffer64 mapping.

  @param PciIo                  Pointer to PciIo for USB3 debug port.
  @param Pages                  The number of pages to allocate.
  @param Address                A pointer to store the base system memory address of the
                                allocated range.

  @retval EFI_SUCCESS           The requested memory pages were allocated.
  @retval EFI_UNSUPPORTED       Attributes is unsupported. The only legal attribute bits are
                                MEMORY_WRITE_COMBINE and MEMORY_CACHED.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The memory pages could not be allocated.

**/
EFI_STATUS
Usb3AllocateDmaBuffer (
  IN EFI_PCI_IO_PROTOCOL    *PciIo,
  IN UINTN                  Pages,
  OUT VOID                  **Address
  )
{
  EFI_STATUS            Status;

  *Address = NULL;
  Status = PciIo->AllocateBuffer (
                    PciIo,
                    AllocateAnyPages,
                    EfiRuntimeServicesData,
                    Pages,
                    Address,
                    0
                    );
  if (!EFI_ERROR (Status)) {
    Usb3MapOneDmaBuffer (
      PciIo,
      (EFI_PHYSICAL_ADDRESS) (UINTN) *Address,
      EFI_PAGES_TO_SIZE (Pages)
      );
  }
  return Status;
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
    if (mUsb3PciIo != NULL) {
      Usb3AllocateDmaBuffer (
        mUsb3PciIo,
        EFI_SIZE_TO_PAGES (BufferSize),
        &Buf
        );
    } else {
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

