/** @file
  Usb3 Debug Port initialization

  Copyright (c) 2013 - 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>

#include <PiPei.h>
#include <Library/SerialPortLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/HobLib.h>
#include <Library/Usb3DebugPortParamLib.h>
#include "Usb3DebugPortLibInternal.h"

UINT16   mString0Desc[] = {
  //  String Descriptor Type + Length
  ( USB_DESC_TYPE_STRING << 8 ) + STRING0_DESC_LEN,
  0x0409
};

UINT16   mManufacturerStrDesc[] = {
  //  String Descriptor Type + Length
  ( USB_DESC_TYPE_STRING << 8 ) + MANU_DESC_LEN,
  'I', 'n', 't', 'e', 'l'
};

//USB 3.0 Debug Cable
UINT16   mProductStrDesc[] = {
  //  String Descriptor Type + Length
  ( USB_DESC_TYPE_STRING << 8 ) +  PRODUCT_DESC_LEN,
  'U', 'S', 'B', ' ', '3', '.', '0', ' ', 'D', 'e', 'b', 'u', 'g', ' ', 'C', 'a', 'b', 'l', 'e'
};

UINT16   mSerialNumberStrDesc[] = {
  //  String Descriptor Type + Length
  ( USB_DESC_TYPE_STRING << 8 ) +  SERIAL_DESC_LEN,
  '1'
};

XHC_DC_CONTEXT   DebugCapabilityContextTemplate = {
  {
    0,
    0,
    0,
    0,
    STRING0_DESC_LEN,
    MANU_DESC_LEN,
    PRODUCT_DESC_LEN,
    SERIAL_DESC_LEN,
    0,
    0,
    0,
    0
  },
  {
    0,  // EPState
    0,  // RsvdZ1
    0,  // Mult
    0,  // MaxPStreams
    0,  // LSA
    0,  // Interval
    0,  // RsvdZ2
    0,  // RsvdZ3
    3,  // CERR
    ED_BULK_OUT,  // EPType
    0,  // RsvdZ4
    0,  // HID
    0,  // MaxBurstSize
    1024, // MaxPacketSize
    0,    // PtrLo
    0,    // PtrHi
    0x1000, // AverageTRBLength
    0,     // MaxESITPayload
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  },
  {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    3,    //CERR
    ED_BULK_IN,
    0,
    0,
    0,
    1024, //MaxPacketSize
    0,
    0,
    0x1000,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
  }
};

/**
  Return command register value in XHCI controller.

**/
UINT16
GetXhciPciCommand (
  VOID
  )
{
  UINT8                                  Bus;
  UINT8                                  Device;
  UINT8                                  Function;
  UINT16                                 Command;
  USB3_DEBUG_PORT_CONTROLLER             UsbDebugPort;

  UsbDebugPort.Controller = GetUsb3DebugPortController();

  Bus = UsbDebugPort.PciAddress.Bus;
  Device = UsbDebugPort.PciAddress.Device;
  Function = UsbDebugPort.PciAddress.Function;
  Command = PciRead16(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET));
  return Command;
}

/**
  Discover the USB3 debug device.

  @param[in] Instance           Pointer to USB3 debug port object instance.

  @retval RETURN_SUCCESS        The USB3 debug device was found.
  @retval RETURN_DEVICE_ERROR   The USB3 debug device was not found.

**/
RETURN_STATUS
DiscoverUsb3DebugPort(
  IN USB3_DEBUG_PORT_INSTANCE   *Instance
  )
{
  UINT8                           Bus;
  UINT8                           Device;
  UINT8                           Function;
  UINT16                          Command;
  EFI_PHYSICAL_ADDRESS            UsbBase;
  USB3_DEBUG_PORT_CONTROLLER      UsbDebugPort;
  EFI_PHYSICAL_ADDRESS            CapabilityPointer;
  UINT32                          Capability;
  BOOLEAN                         Flag;
  UINT8                           CapLength;

  UsbDebugPort.Controller = GetUsb3DebugPortController();
  Bus = UsbDebugPort.PciAddress.Bus;
  Device = UsbDebugPort.PciAddress.Device;
  Function = UsbDebugPort.PciAddress.Function;
  if ((PciRead8(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_CLASSCODE_OFFSET + 2)) != PCI_CLASS_SERIAL) ||
      (PciRead8(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_CLASSCODE_OFFSET + 1)) != PCI_CLASS_SERIAL_USB) ||
      (PciRead8(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_CLASSCODE_OFFSET)) != 0x30)) {
    //
    // The device is not XHCI controller
    //
    return RETURN_NOT_FOUND;
  }

  //
  // USBBASE is at 10-13h, i.e. the first BAR, clear the low bits which is not part of base address
  //
  UsbBase = GetXhciBaseAddress ();

  //
  // Set XHCI MMIO base address if necessary
  //
  if ((UsbBase == 0) || (UsbBase == XHCI_BASE_ADDRESS_64_BIT_MASK)) {
    UsbBase  = PcdGet32 (PcdXhciDefaultBaseAddress);
    PciWrite32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET), (UINT32)UsbBase);
    PciWrite32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4), 0x0);
    UsbBase = PciRead32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET)) & XHCI_BASE_ADDRESS_32_BIT_MASK;
    if (UsbBase == 0 || UsbBase == XHCI_BASE_ADDRESS_32_BIT_MASK) {
      return RETURN_DEVICE_ERROR;
    }
  }

  //
  //  Set MSE and BME bit - enable the address space
  //
  Command = PciRead16 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET));
  if ((Command & EFI_PCI_COMMAND_MEMORY_SPACE) == 0) {
    PciWrite16(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET), Command | EFI_PCI_COMMAND_MEMORY_SPACE);
    PciRead16(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET));
  }

  CapLength = MmioRead8 ((UINTN) UsbBase);

  //
  // Get capability pointer from HCCPARAMS at offset 0x10
  //
  CapabilityPointer = UsbBase + (MmioRead32 ((UINTN)(UsbBase + XHC_HCCPARAMS_OFFSET)) >> 16) * 4;

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
      // Reach the end of capability list, quit
      //
      break;
    }
    CapabilityPointer += ((Capability & XHC_NEXT_CAPABILITY_MASK) >> 8) * 4;
    Capability = MmioRead32 ((UINTN)CapabilityPointer);
  }

  Instance->Signature             = USB3_DEBUG_PORT_INSTANCE_SIGNATURE;

  if (Flag) {
    Instance->DebugSupport          = TRUE;
    Instance->DebugCapabilityBase   = CapabilityPointer;
    Instance->DebugCapabilityOffset = CapabilityPointer - UsbBase;

    Instance->XhciOpRegister        = UsbBase + CapLength;
    Instance->XhcMmioBase           = UsbBase;

    Instance->PciBusNumber          = Bus;
    Instance->PciDeviceNumber       = Device;
    Instance->PciFunctionNumber     = Function;
  }

  //
  // Restore Command Register
  //
  PciWrite16(PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), Command);

  return RETURN_SUCCESS;
}

/**
  Create XHCI event ring.

  @param  Xhc                 The XHCI Instance.
  @param  EventRing           The created event ring.

**/
EFI_STATUS
CreateEventRing (
  IN  USB3_DEBUG_PORT_INSTANCE   *Xhc,
  OUT EVENT_RING                 *EventRing
  )
{
  VOID                        *Buf;
  EVENT_RING_SEG_TABLE_ENTRY  *ERSTBase;

  ASSERT (EventRing != NULL);

  //
  // Allocate Event Ring
  //
  Buf = AllocateAlignBuffer (sizeof (TRB_TEMPLATE) * EVENT_RING_TRB_NUMBER);
  ASSERT (Buf != NULL);
  ASSERT (((UINTN) Buf & 0x3F) == 0);
  ZeroMem (Buf, sizeof (TRB_TEMPLATE) * EVENT_RING_TRB_NUMBER);

  EventRing->EventRingSeg0    = (EFI_PHYSICAL_ADDRESS)(UINTN) Buf;
  EventRing->TrbNumber        = EVENT_RING_TRB_NUMBER;
  EventRing->EventRingDequeue = (EFI_PHYSICAL_ADDRESS)(UINTN) EventRing->EventRingSeg0;
  EventRing->EventRingEnqueue = (EFI_PHYSICAL_ADDRESS)(UINTN) EventRing->EventRingSeg0;

  //
  // Software maintains an Event Ring Consumer Cycle State (CCS) bit, initializing it to '1'
  // and toggling it every time the Event Ring Dequeue Pointer wraps back to the beginning of the Event Ring.
  //
  EventRing->EventRingCCS = 1;

  //
  // Allocate Event Ring Segment Table Entry 0 in Event Ring Segment Table
  //
  Buf = AllocateAlignBuffer (sizeof (EVENT_RING_SEG_TABLE_ENTRY) * ERST_NUMBER);
  ASSERT (Buf != NULL);
  ASSERT (((UINTN) Buf & 0x3F) == 0);
  ZeroMem (Buf, sizeof (EVENT_RING_SEG_TABLE_ENTRY) * ERST_NUMBER);

  ERSTBase              = (EVENT_RING_SEG_TABLE_ENTRY *) Buf;
  EventRing->ERSTBase   = (EFI_PHYSICAL_ADDRESS)(UINTN) ERSTBase;

  //
  // Fill Event Segment address
  //
  ERSTBase->PtrLo       = XHC_LOW_32BIT (EventRing->EventRingSeg0);
  ERSTBase->PtrHi       = XHC_HIGH_32BIT (EventRing->EventRingSeg0);
  ERSTBase->RingTrbSize = EVENT_RING_TRB_NUMBER;

  //
  // Program the Interrupter Event Ring Dequeue Pointer (DCERDP) register (7.6.4.1)
  //
  XhcWriteDebugReg (
    Xhc,
    XHC_DC_DCERDP,
    XHC_LOW_32BIT((UINT64)(UINTN)EventRing->EventRingDequeue)
    );

  XhcWriteDebugReg (
    Xhc,
    XHC_DC_DCERDP + 4,
    XHC_HIGH_32BIT((UINT64)(UINTN)EventRing->EventRingDequeue)
    );

  //
  // Program the Debug Capability Event Ring Segment Table Base Address (DCERSTBA) register(7.6.4.1)
  //
  XhcWriteDebugReg (
    Xhc,
    XHC_DC_DCERSTBA,
    XHC_LOW_32BIT((UINT64)(UINTN)ERSTBase)
    );

  XhcWriteDebugReg (
    Xhc,
    XHC_DC_DCERSTBA + 4,
    XHC_HIGH_32BIT((UINT64)(UINTN)ERSTBase)
    );

  //
  // Program the Debug Capability Event Ring Segment Table Size (DCERSTSZ) register(7.6.4.1)
  //
  XhcWriteDebugReg (
    Xhc,
    XHC_DC_DCERSTSZ,
    ERST_NUMBER
    );
  return EFI_SUCCESS;
}

/**
  Create XHCI transfer ring.

  @param  Xhc               The XHCI Instance.
  @param  TrbNum            The number of TRB in the ring.
  @param  TransferRing      The created transfer ring.

**/
VOID
CreateTransferRing (
  IN  USB3_DEBUG_PORT_INSTANCE    *Xhc,
  IN  UINT32                      TrbNum,
  OUT TRANSFER_RING               *TransferRing
  )
{
  VOID                  *Buf;
  LINK_TRB              *EndTrb;

  Buf = AllocateAlignBuffer (sizeof (TRB_TEMPLATE) * TrbNum);
  ASSERT (Buf != NULL);
  ASSERT (((UINTN) Buf & 0xF) == 0);
  ZeroMem (Buf, sizeof (TRB_TEMPLATE) * TrbNum);

  TransferRing->RingSeg0     = (EFI_PHYSICAL_ADDRESS)(UINTN) Buf;
  TransferRing->TrbNumber    = TrbNum;
  TransferRing->RingEnqueue  = TransferRing->RingSeg0;
  TransferRing->RingDequeue  = TransferRing->RingSeg0;
  TransferRing->RingPCS      = 1;
  //
  // 4.9.2 Transfer Ring Management
  // To form a ring (or circular queue) a Link TRB may be inserted at the end of a ring to
  // point to the first TRB in the ring.
  //
  EndTrb        = (LINK_TRB *) ((UINTN)Buf + sizeof (TRB_TEMPLATE) * (TrbNum - 1));
  EndTrb->Type  = TRB_TYPE_LINK;
  EndTrb->PtrLo = XHC_LOW_32BIT (Buf);
  EndTrb->PtrHi = XHC_HIGH_32BIT (Buf);
  //
  // Toggle Cycle (TC). When set to '1', the xHC shall toggle its interpretation of the Cycle bit.
  //
  EndTrb->TC    = 1;
  //
  // Set Cycle bit as other TRB PCS init value
  //
  EndTrb->CycleBit = 0;
}

/**
  Create debug capability context for XHC debug device.

  @param  Xhc          The XHCI Instance.

  @retval EFI_SUCCESS  The bit successfully changed by host controller.
  @retval EFI_TIMEOUT  The time out occurred.

**/
EFI_STATUS
CreateDebugCapabilityContext (
  IN  USB3_DEBUG_PORT_INSTANCE   *Xhc
  )
{
  VOID                        *Buf;
  XHC_DC_CONTEXT              *DebugCapabilityContext;
  UINT8                       *String0Desc;
  UINT8                       *ManufacturerStrDesc;
  UINT8                       *ProductStrDesc;
  UINT8                       *SerialNumberStrDesc;

  //
  // Allocate debug device context
  //
  Buf = AllocateAlignBuffer (sizeof (XHC_DC_CONTEXT));
  ASSERT (Buf != NULL);
  ASSERT (((UINTN) Buf & 0xF) == 0);
  ZeroMem (Buf, sizeof (XHC_DC_CONTEXT));

  DebugCapabilityContext = (XHC_DC_CONTEXT *)(UINTN) Buf;
  Xhc->DebugCapabilityContext = (EFI_PHYSICAL_ADDRESS)(UINTN) DebugCapabilityContext;

  CopyMem (DebugCapabilityContext, &DebugCapabilityContextTemplate, sizeof (XHC_DC_CONTEXT));

  //
  // Update string descriptor address
  //
  String0Desc = (UINT8 *) AllocateAlignBuffer (STRING0_DESC_LEN + MANU_DESC_LEN + PRODUCT_DESC_LEN + SERIAL_DESC_LEN);
  ASSERT (String0Desc != NULL);
  ZeroMem (String0Desc, STRING0_DESC_LEN + MANU_DESC_LEN + PRODUCT_DESC_LEN + SERIAL_DESC_LEN);
  CopyMem (String0Desc, mString0Desc, STRING0_DESC_LEN);
  DebugCapabilityContext->DbcInfoContext.String0DescAddress = (UINT64)(UINTN)String0Desc;

  ManufacturerStrDesc = String0Desc + STRING0_DESC_LEN;
  CopyMem (ManufacturerStrDesc, mManufacturerStrDesc, MANU_DESC_LEN);
  DebugCapabilityContext->DbcInfoContext.ManufacturerStrDescAddress = (UINT64)(UINTN)ManufacturerStrDesc;

  ProductStrDesc = ManufacturerStrDesc + MANU_DESC_LEN;
  CopyMem (ProductStrDesc, mProductStrDesc, PRODUCT_DESC_LEN);
  DebugCapabilityContext->DbcInfoContext.ProductStrDescAddress = (UINT64)(UINTN)ProductStrDesc;

  SerialNumberStrDesc = ProductStrDesc + PRODUCT_DESC_LEN;
  CopyMem (SerialNumberStrDesc, mSerialNumberStrDesc, SERIAL_DESC_LEN);
  DebugCapabilityContext->DbcInfoContext.SerialNumberStrDescAddress = (UINT64)(UINTN)SerialNumberStrDesc;

  //
  // Allocate and initialize the Transfer Ring for the Input Endpoint Context.
  //
  ZeroMem (&Xhc->TransferRingIn, sizeof (TRANSFER_RING));
  CreateTransferRing (Xhc, TR_RING_TRB_NUMBER, &Xhc->TransferRingIn);

  //
  // Can not set BIT0, otherwise there is no transfer ring detected.
  //
  DebugCapabilityContext->EpInContext.PtrLo = XHC_LOW_32BIT (Xhc->TransferRingIn.RingSeg0) | BIT0;
  DebugCapabilityContext->EpInContext.PtrHi = XHC_HIGH_32BIT (Xhc->TransferRingIn.RingSeg0);

  //
  // Allocate and initialize the Transfer Ring for the Output Endpoint Context.
  //
  ZeroMem (&Xhc->TransferRingOut, sizeof (TRANSFER_RING));
  CreateTransferRing (Xhc, TR_RING_TRB_NUMBER, &Xhc->TransferRingOut);

  //
  // Can not set BIT0, otherwise there is no transfer ring detected.
  //
  DebugCapabilityContext->EpOutContext.PtrLo = XHC_LOW_32BIT (Xhc->TransferRingOut.RingSeg0) | BIT0;
  DebugCapabilityContext->EpOutContext.PtrHi = XHC_HIGH_32BIT (Xhc->TransferRingOut.RingSeg0);

  //
  // Program the Debug Capability Context Pointer (DCCP) register(7.6.8.7)
  //
  XhcWriteDebugReg (
    Xhc,
    XHC_DC_DCCP,
    XHC_LOW_32BIT((UINT64)(UINTN)DebugCapabilityContext)
    );
  XhcWriteDebugReg (
    Xhc,
    XHC_DC_DCCP + 4,
    XHC_HIGH_32BIT((UINT64)(UINTN)DebugCapabilityContext)
    );
  return EFI_SUCCESS;
}

/**
  Initialize the USB3 debug Device hardware.

  @param[in] Instance       Pointer to USB3 debug port object instance.

  @retval RETURN_SUCCESS    The USB3 debug device was initialized successfully.
  @retval !RETURN_SUCCESS   Error.

**/
RETURN_STATUS
InitializeUsb3DebugPort (
  IN USB3_DEBUG_PORT_INSTANCE   *Instance
  )
{
  RETURN_STATUS                   Status;
  UINT64                          XhciOpRegister;
  volatile UINT32                 Dcctrl;
  UINT8                           Bus;
  UINT8                           Device;
  UINT8                           Function;
  UINT16                          Command;
  EFI_BOOT_MODE                   BootMode;
  UINT64                          TimeOut;
  CHAR8                           *TestString;
  UINTN                           Length;
  UINT32                          TransferResult;

  Bus      = Instance->PciBusNumber;
  Device   = Instance->PciDeviceNumber;
  Function = Instance->PciFunctionNumber;

  Command = GetXhciPciCommand ();

  //
  // Save and set Command Register
  //
  if (((Command & EFI_PCI_COMMAND_MEMORY_SPACE) == 0) || ((Command & EFI_PCI_COMMAND_BUS_MASTER) == 0)) {
    PciWrite16(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET), Command | EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
    PciRead16(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET));
  }

  //
  // Clear DCE bit and LSE bit in DCCTRL
  //
  if ((XhcReadDebugReg (Instance, XHC_DC_DCCTRL) & (BIT1|BIT31)) == (BIT1|BIT31)) {
    XhcClearDebugRegBit (Instance, XHC_DC_DCCTRL, BIT1|BIT31);
  }

  XhciOpRegister = Instance->XhciOpRegister;

  //
  // Get current Boot Mode
  //
  BootMode = GetBootModeHob ();

  if (BootMode != BOOT_ON_S3_RESUME) {
    if (!XhcIsBitSet((UINTN)(XhciOpRegister + XHC_USBSTS_OFFSET), XHC_USBSTS_HALT)) {
      XhcClrR32Bit((UINTN) XhciOpRegister + XHC_USBCMD_OFFSET, XHC_USBCMD_RUN);
      while (!XhcIsBitSet((UINTN)(XhciOpRegister + XHC_USBSTS_OFFSET), XHC_USBSTS_HALT)) {
        MicroSecondDelay (10);
      }
    }

    //
    // Reset host controller
    //
    XhcSetR32Bit((UINTN)XhciOpRegister + XHC_USBCMD_OFFSET, XHC_USBCMD_RESET);

    //
    // Ensure that the host controller is reset (RESET bit must be cleared after reset)
    //
    while (XhcIsBitSet((UINTN)XhciOpRegister + XHC_USBCMD_OFFSET, XHC_USBCMD_RESET)) {
      MicroSecondDelay (10);
    }
  }

  //
  // Initialize event ring
  //
  ZeroMem (&Instance->EventRing, sizeof (EVENT_RING));
  Status = CreateEventRing (Instance, &Instance->EventRing);
  ASSERT_EFI_ERROR (Status);

  //
  // Init IN and OUT endpoint context
  //
  Status = CreateDebugCapabilityContext (Instance);
  ASSERT_EFI_ERROR (Status);

  //
  // Init data buffer used to transfer
  //
  Instance->Urb.Data = (EFI_PHYSICAL_ADDRESS) (UINTN) AllocateAlignBuffer (XHC_DEBUG_PORT_DATA_LENGTH);

  //
  // Init DCDDI1 and DCDDI2
  //
  XhcWriteDebugReg (
   Instance,
   XHC_DC_DCDDI1,
   (UINT32)((XHCI_DEBUG_DEVICE_VENDOR_ID << 16) | XHCI_DEBUG_DEVICE_PROTOCOL)
   );

  XhcWriteDebugReg (
   Instance,
   XHC_DC_DCDDI2,
   (UINT32)((XHCI_DEBUG_DEVICE_REVISION << 16) | XHCI_DEBUG_DEVICE_PRODUCT_ID)
   );

  //
  // Set DCE bit and LSE bit to "1" in DCCTRL
  //
  XhcSetDebugRegBit (Instance, XHC_DC_DCCTRL, BIT1|BIT31);

  TimeOut = DivU64x32 (PcdGet64 (PcdXhciHostWaitTimeout), XHC_POLL_DELAY) + 1;
  while (TimeOut != 0) {
    //
    // Check if debug device is in configured state
    //
    Dcctrl = XhcReadDebugReg (Instance, XHC_DC_DCCTRL);
    if ((Dcctrl & BIT0) != 0) {
      //
      // Set the flag to indicate debug device is ready
      //
      Instance->Ready = TRUE;
      break;
    }
    MicroSecondDelay (XHC_POLL_DELAY);
    TimeOut--;
  }

  if (!Instance->Ready) {
    XhcClearDebugRegBit (Instance, XHC_DC_DCCTRL, BIT1|BIT31);
  } else {
    TestString = "Usb 3.0 Debug Message Start\n";
    Length = AsciiStrLen (TestString);
    XhcDataTransfer (
      Instance,
      EfiUsbDataOut,
      TestString,
      &Length,
      0,
      &TransferResult
      );
  }

  //
  // Restore Command Register
  //
  PciWrite16 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), Command);

  return EFI_SUCCESS;
}

/**
  Update XHC hardware address when MMIO base is changed.

  @param  Instance          The XHCI Instance.
  @param  XhcMmioBase       XHCI MMIO base address.

**/
VOID
FixUsb3InstanceResource (
  IN OUT USB3_DEBUG_PORT_INSTANCE           *Instance,
  IN EFI_PHYSICAL_ADDRESS                   XhcMmioBase
  )
{
  if ((Instance == NULL) || (Instance->XhcMmioBase == XhcMmioBase)) {
    return;
  }

  //
  // Need fix Instance data according to PCI resource
  //
  Instance->XhcMmioBase         = XhcMmioBase;
  Instance->DebugCapabilityBase = XhcMmioBase + Instance->DebugCapabilityOffset;
  Instance->XhciOpRegister      = XhcMmioBase + MmioRead8 ((UINTN)XhcMmioBase);
}

/**
  Save USB3 instance address.

  @param[in] Instance   The XHCI Instance.

**/
VOID
SaveUsb3InstanceAddress (
  IN USB3_DEBUG_PORT_INSTANCE   *Instance
  )
{
  UINT16                        Command;
  USB3_DEBUG_PORT_CONTROLLER    UsbDebugPort;
  UINT8                         Bus;
  UINT8                         Device;
  UINT8                         Function;

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
  // After debug device is finished to enumerate, use DCDDI2 register to store instance address
  //
  XhcWriteDebugReg (
   Instance,
   XHC_DC_DCDDI2,
   (UINT32)(UINTN)Instance
   );

  //
  // Restore Command Register
  //
  PciWrite16 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), Command);
}

