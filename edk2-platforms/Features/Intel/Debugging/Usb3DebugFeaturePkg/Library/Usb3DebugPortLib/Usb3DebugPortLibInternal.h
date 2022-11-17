/** @file
  This library class provides common Usb debug port functions.

  Copyright (c) 2013 - 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __USB3_DEBUG_PORT_LIB_INTERNAL__
#define __USB3_DEBUG_PORT_LIB_INTERNAL__

#include <Uefi.h>
#include <IndustryStandard/Usb.h>
#include <Library/IoLib.h>
#include <IndustryStandard/Pci.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/TimerLib.h>

//
// USB Debug GUID value
//
#define USB3_DBG_GUID \
    { \
      0xb2a56f4d, 0x9177, 0x4fc8, { 0xa6, 0x77, 0xdd, 0x96, 0x3e, 0xb4, 0xcb, 0x1b } \
    }

//
// Define the maximum of SMRAM ranges
//
#define MAX_SMRAM_RANGE     8

#define XHCI_DEBUG_DEVICE_VENDOR_ID   0x8086
#define XHCI_DEBUG_DEVICE_PRODUCT_ID  0x1234
#define XHCI_DEBUG_DEVICE_PROTOCOL    0xFF
#define XHCI_DEBUG_DEVICE_REVISION    0x00

#define XHCI_BASE_ADDRESS_64_BIT_MASK 0xFFFFFFFFFFFF0000ULL
#define XHCI_BASE_ADDRESS_32_BIT_MASK 0xFFFF0000

#define PCI_CAPABILITY_ID_DEBUG_PORT  0x0A
#define XHC_HCCPARAMS_OFFSET          0x10
#define XHC_CAPABILITY_ID_MASK        0xFF
#define XHC_NEXT_CAPABILITY_MASK      0xFF00

#define XHC_USBCMD_OFFSET             0x0 // USB Command Register Offset
#define XHC_USBSTS_OFFSET             0x4 // USB Status Register Offset

#define XHC_USBCMD_RUN                BIT0  // Run/Stop
#define XHC_USBCMD_RESET              BIT1  // Host Controller Reset

#define XHC_USBSTS_HALT               BIT0

//
// Transfer the data of 8 bytes each time
//
#define XHC_DEBUG_PORT_DATA_LENGTH   8

//
// Indicate the timeout when data is transferred. 0 means infinite timeout.
//
#define DATA_TRANSFER_TIME_OUT       0

//
// USB debug device string descritpor (header size + unicode string length)
//
#define STRING0_DESC_LEN      4
#define MANU_DESC_LEN         12
#define PRODUCT_DESC_LEN      40
#define SERIAL_DESC_LEN       4

//
// Debug Capability Register Offset
//
#define XHC_DC_DCID                  0x0
#define XHC_DC_DCDB                  0x4
#define XHC_DC_DCERSTSZ              0x8
#define XHC_DC_DCERSTBA              0x10
#define XHC_DC_DCERDP                0x18
#define XHC_DC_DCCTRL                0x20
#define XHC_DC_DCST                  0x24
#define XHC_DC_DCPORTSC              0x28
#define XHC_DC_DCCP                  0x30
#define XHC_DC_DCDDI1                0x38
#define XHC_DC_DCDDI2                0x3C

#define TRB_TYPE_LINK                6

#define ERST_NUMBER                  0x01
#define TR_RING_TRB_NUMBER           0x100
#define EVENT_RING_TRB_NUMBER        0x200
#define CMD_RING_TRB_NUMBER          0x100

#define ED_BULK_OUT                  2
#define ED_BULK_IN                   6

#define XHC_LOW_32BIT(Addr64)          ((UINT32)(((UINTN)(Addr64)) & 0xFFFFFFFF))
#define XHC_HIGH_32BIT(Addr64)         ((UINT32)(RShiftU64((UINT64)(UINTN)(Addr64), 32) & 0xFFFFFFFF))
#define XHC_BIT_IS_SET(Data, Bit)      ((BOOLEAN)(((Data) & (Bit)) == (Bit)))

#define XHC_REG_BIT_IS_SET(Xhc, Offset, Bit) \
          (XHC_BIT_IS_SET(XhcReadMmioReg ((Xhc), (Offset)), (Bit)))

//
// Endpoint Type (EP Type).
//
#define ED_NOT_VALID                          0
#define ED_ISOCH_OUT                          1
#define ED_BULK_OUT                           2
#define ED_INTERRUPT_OUT                      3
#define ED_CONTROL_BIDIR                      4
#define ED_ISOCH_IN                           5
#define ED_BULK_IN                            6
#define ED_INTERRUPT_IN                       7

//
// 6.4.5 TRB Completion Codes
//
#define TRB_COMPLETION_INVALID                0
#define TRB_COMPLETION_SUCCESS                1
#define TRB_COMPLETION_DATA_BUFFER_ERROR      2
#define TRB_COMPLETION_BABBLE_ERROR           3
#define TRB_COMPLETION_USB_TRANSACTION_ERROR  4
#define TRB_COMPLETION_TRB_ERROR              5
#define TRB_COMPLETION_STALL_ERROR            6
#define TRB_COMPLETION_SHORT_PACKET           13

//
// 6.4.6 TRB Types
//
#define TRB_TYPE_NORMAL                       1
#define TRB_TYPE_SETUP_STAGE                  2
#define TRB_TYPE_DATA_STAGE                   3
#define TRB_TYPE_STATUS_STAGE                 4
#define TRB_TYPE_ISOCH                        5
#define TRB_TYPE_LINK                         6
#define TRB_TYPE_EVENT_DATA                   7
#define TRB_TYPE_NO_OP                        8
#define TRB_TYPE_EN_SLOT                      9
#define TRB_TYPE_DIS_SLOT                     10
#define TRB_TYPE_ADDRESS_DEV                  11
#define TRB_TYPE_CON_ENDPOINT                 12
#define TRB_TYPE_EVALU_CONTXT                 13
#define TRB_TYPE_RESET_ENDPOINT               14
#define TRB_TYPE_STOP_ENDPOINT                15
#define TRB_TYPE_SET_TR_DEQUE                 16
#define TRB_TYPE_RESET_DEV                    17
#define TRB_TYPE_GET_PORT_BANW                21
#define TRB_TYPE_FORCE_HEADER                 22
#define TRB_TYPE_NO_OP_COMMAND                23
#define TRB_TYPE_TRANS_EVENT                  32
#define TRB_TYPE_COMMAND_COMPLT_EVENT         33
#define TRB_TYPE_PORT_STATUS_CHANGE_EVENT     34
#define TRB_TYPE_HOST_CONTROLLER_EVENT        37
#define TRB_TYPE_DEVICE_NOTIFI_EVENT          38
#define TRB_TYPE_MFINDEX_WRAP_EVENT           39

//
// Convert millisecond to microsecond.
//
#define XHC_1_MILLISECOND                     (1000)
#define XHC_POLL_DELAY                        (1000)
#define XHC_GENERIC_TIMEOUT                   (10 * 1000)

#define EFI_USB_SPEED_FULL                    0x0000  ///< 12 Mb/s, USB 1.1 OHCI and UHCI HC.
#define EFI_USB_SPEED_LOW                     0x0001  ///< 1 Mb/s, USB 1.1 OHCI and UHCI HC.
#define EFI_USB_SPEED_HIGH                    0x0002  ///< 480 Mb/s, USB 2.0 EHCI HC.
#define EFI_USB_SPEED_SUPER                   0x0003  ///< 4.8 Gb/s, USB 3.0 XHCI HC.

//
// Transfer types, used in URB to identify the transfer type
//
#define XHC_CTRL_TRANSFER                     0x01
#define XHC_BULK_TRANSFER                     0x02
#define XHC_INT_TRANSFER_SYNC                 0x04
#define XHC_INT_TRANSFER_ASYNC                0x08
#define XHC_INT_ONLY_TRANSFER_ASYNC           0x10

//
// USB Transfer Results
//
#define EFI_USB_NOERROR             0x00
#define EFI_USB_ERR_NOTEXECUTE      0x01
#define EFI_USB_ERR_STALL           0x02
#define EFI_USB_ERR_BUFFER          0x04
#define EFI_USB_ERR_BABBLE          0x08
#define EFI_USB_ERR_NAK             0x10
#define EFI_USB_ERR_CRC             0x20
#define EFI_USB_ERR_TIMEOUT         0x40
#define EFI_USB_ERR_BITSTUFF        0x80
#define EFI_USB_ERR_SYSTEM          0x100

#define USB3_DEBUG_PORT_INSTANCE_SIGNATURE   SIGNATURE_32('D', 'B', 'G', 'P')

#pragma pack(1)

//
// 7.6.9 OUT/IN EP Context: 64 bytes
// 7.6.9.2 When used by the DbC it is always a 64 byte data structure
//
typedef struct _ENDPOINT_CONTEXT_64 {
  UINT32                  EPState:3;
  UINT32                  RsvdZ1:5;
  UINT32                  Mult:2;         // set to 0
  UINT32                  MaxPStreams:5;  // set to 0
  UINT32                  LSA:1;          // set to 0
  UINT32                  Interval:8;     // set to 0
  UINT32                  RsvdZ2:8;

  UINT32                  RsvdZ3:1;
  UINT32                  CErr:2;
  UINT32                  EPType:3;
  UINT32                  RsvdZ4:1;
  UINT32                  HID:1;          // set to 0
  UINT32                  MaxBurstSize:8;
  UINT32                  MaxPacketSize:16;

  UINT32                  PtrLo;

  UINT32                  PtrHi;

  UINT32                  AverageTRBLength:16;
  UINT32                  MaxESITPayload:16;  // set to 0

  UINT32                  RsvdZ5;             // Reserved
  UINT32                  RsvdZ6;
  UINT32                  RsvdZ7;

  UINT32                  RsvdZ8;
  UINT32                  RsvdZ9;
  UINT32                  RsvdZ10;
  UINT32                  RsvdZ11;

  UINT32                  RsvdZ12;
  UINT32                  RsvdZ13;
  UINT32                  RsvdZ14;
  UINT32                  RsvdZ15;
} ENDPOINT_CONTEXT_64;

//
// 6.4.1.1 Normal TRB: 16 bytes
// A Normal TRB is used in several ways; exclusively on Bulk and Interrupt Transfer Rings for normal and
// Scatter/Gather operations, to define additional data buffers for Scatter/Gather operations on Isoch Transfer
// Rings, and to define the Data stage information for Control Transfer Rings.
//
typedef struct _TRANSFER_TRB_NORMAL {
  UINT32                  TRBPtrLo;

  UINT32                  TRBPtrHi;

  UINT32                  Length:17;
  UINT32                  TDSize:5;
  UINT32                  IntTarget:10;

  UINT32                  CycleBit:1;
  UINT32                  ENT:1;
  UINT32                  ISP:1;
  UINT32                  NS:1;
  UINT32                  CH:1;
  UINT32                  IOC:1;
  UINT32                  IDT:1;
  UINT32                  RsvdZ1:2;
  UINT32                  BEI:1;
  UINT32                  Type:6;
  UINT32                  RsvdZ2:16;
} TRANSFER_TRB_NORMAL;

//
// 6.4.2.1 Transfer Event TRB: 16 bytes
// A Transfer Event provides the completion status associated with a Transfer TRB. Refer to section 4.11.3.1
// for more information on the use and operation of Transfer Events.
//
typedef struct _EVT_TRB_TRANSFER {
  UINT32                  TRBPtrLo;

  UINT32                  TRBPtrHi;

  UINT32                  Length:24;
  UINT32                  Completecode:8;

  UINT32                  CycleBit:1;
  UINT32                  RsvdZ1:1;
  UINT32                  ED:1;
  UINT32                  RsvdZ2:7;
  UINT32                  Type:6;
  UINT32                  EndpointId:5;
  UINT32                  RsvdZ3:3;
  UINT32                  SlotId:8;
} EVT_TRB_TRANSFER;

//
// 6.4.4.1 Link TRB: 16 bytes
// A Link TRB provides support for non-contiguous TRB Rings.
//
typedef struct _LINK_TRB {
  UINT32                  PtrLo;

  UINT32                  PtrHi;

  UINT32                  RsvdZ1:22;
  UINT32                  InterTarget:10;

  UINT32                  CycleBit:1;
  UINT32                  TC:1;
  UINT32                  RsvdZ2:2;
  UINT32                  CH:1;
  UINT32                  IOC:1;
  UINT32                  RsvdZ3:4;
  UINT32                  Type:6;
  UINT32                  RsvdZ4:16;
} LINK_TRB;

//
// TRB Template: 16 bytes
//
typedef struct _TRB_TEMPLATE {
  UINT32                    Parameter1;

  UINT32                    Parameter2;

  UINT32                    Status;

  UINT32                    CycleBit:1;
  UINT32                    RsvdZ1:9;
  UINT32                    Type:6;
  UINT32                    Control:16;
} TRB_TEMPLATE;

//
// Refer to XHCI 6.5 Event Ring Segment Table: 16 bytes
//
typedef struct _EVENT_RING_SEG_TABLE_ENTRY {
  UINT32                  PtrLo;
  UINT32                  PtrHi;
  UINT32                  RingTrbSize:16;
  UINT32                  RsvdZ1:16;
  UINT32                  RsvdZ2;
} EVENT_RING_SEG_TABLE_ENTRY;

//
// Size: 40 bytes
//
typedef struct _EVENT_RING {
  EFI_PHYSICAL_ADDRESS      ERSTBase;
  EFI_PHYSICAL_ADDRESS      EventRingSeg0;
  UINT32                    TrbNumber;
  EFI_PHYSICAL_ADDRESS      EventRingEnqueue;
  EFI_PHYSICAL_ADDRESS      EventRingDequeue;
  UINT32                    EventRingCCS;
} EVENT_RING;

// Size: 32 bytes
typedef struct _TRANSFER_RING {
  EFI_PHYSICAL_ADDRESS      RingSeg0;
  UINT32                    TrbNumber;
  EFI_PHYSICAL_ADDRESS      RingEnqueue;
  EFI_PHYSICAL_ADDRESS      RingDequeue;
  UINT32                    RingPCS;
} TRANSFER_RING;

//
// Size: 64 bytes
//
typedef struct _DBC_INFO_CONTEXT {
  UINT64        String0DescAddress;
  UINT64        ManufacturerStrDescAddress;
  UINT64        ProductStrDescAddress;
  UINT64        SerialNumberStrDescAddress;
  UINT64        String0Length:8;
  UINT64        ManufacturerStrLength:8;
  UINT64        ProductStrLength:8;
  UINT64        SerialNumberStrLength:8;
  UINT64        RsvdZ1:32;
  UINT64        RsvdZ2;
  UINT64        RsvdZ3;
  UINT64        RsvdZ4;
} DBC_INFO_CONTEXT;

//
// Debug Capability Context Data Structure: 192 bytes
//
typedef struct _XHC_DC_CONTEXT {
  DBC_INFO_CONTEXT      DbcInfoContext;
  ENDPOINT_CONTEXT_64   EpOutContext;
  ENDPOINT_CONTEXT_64   EpInContext;
} XHC_DC_CONTEXT;

//
// Size: 16 bytes
//
typedef union _TRB {
  TRB_TEMPLATE                TrbTemplate;
  TRANSFER_TRB_NORMAL         TrbNormal;
} TRB;

///
/// USB data transfer direction
///
typedef enum {
  EfiUsbDataIn,
  EfiUsbDataOut,
  EfiUsbNoData
} EFI_USB_DATA_DIRECTION;

//
// URB (Usb Request Block) contains information for all kinds of
// usb requests.
//
typedef struct _URB {
  UINT32                          Signature;
  //
  // Transfer data
  //
  EFI_PHYSICAL_ADDRESS            Data;
  UINT32                          DataLen;
  //
  // Execute result
  //
  UINT32                          Result;
  //
  // Completed data length
  //
  UINT32                          Completed;
  //
  // Tranfer Ring info
  //
  EFI_PHYSICAL_ADDRESS            Ring;
  EFI_PHYSICAL_ADDRESS            TrbStart;
  EFI_PHYSICAL_ADDRESS            TrbEnd;
  UINT32                          TrbNum;
  BOOLEAN                         StartDone;
  BOOLEAN                         EndDone;
  BOOLEAN                         Finished;
  EFI_USB_DATA_DIRECTION          Direction;
} URB;

//
// Size: 90 bytes
//
typedef struct _USB3_DEBUG_PORT_INSTANCE {
  UINT32                                  Signature;

  UINT8                                   PciBusNumber;
  UINT8                                   PciDeviceNumber;
  UINT8                                   PciFunctionNumber;

  //
  // The flag indicates debug capability is supported
  //
  BOOLEAN                                 DebugSupport;

  //
  // The flag indicates debug device is ready
  //
  BOOLEAN                                 Ready;

  //
  // The flag indicates the instance is from HOB
  //
  BOOLEAN                                 FromHob;

  //
  // PciIo protocol event
  //
  EFI_PHYSICAL_ADDRESS                    PciIoEvent;

  //
  // XHCI MMIO Base address
  //
  EFI_PHYSICAL_ADDRESS                    XhcMmioBase;

  //
  // XHCI OP RegisterBase address
  //
  EFI_PHYSICAL_ADDRESS                    XhciOpRegister;

  //
  // XHCI Debug Register Base Address
  //
  EFI_PHYSICAL_ADDRESS                    DebugCapabilityBase;

  //
  // XHCI Debug Capability offset
  //
  UINT64                                  DebugCapabilityOffset;

  //
  // XHCI Debug Context Address
  //
  EFI_PHYSICAL_ADDRESS                    DebugCapabilityContext;

  //
  // Transfer Ring
  //
  TRANSFER_RING                           TransferRingOut;
  TRANSFER_RING                           TransferRingIn;

  //
  // EventRing
  //
  EVENT_RING                              EventRing;

  //
  // URB
  //
  URB                                     Urb;
} USB3_DEBUG_PORT_INSTANCE;

#pragma pack()

/**
  Clear one bit of the debug register while keeping other bits.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the debug register.
  @param  Bit          The bit mask of the register to set.

**/
VOID
XhcClearDebugRegBit (
  IN USB3_DEBUG_PORT_INSTANCE *Xhc,
  IN UINT32                   Offset,
  IN UINT32                   Bit
  );

/**
  Clear one bit of the MMIO register while keeping other bits.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the MMIO register.
  @param  Bit          The bit mask of the register to set.

**/
VOID
XhcClearMmioRegBit (
  IN USB3_DEBUG_PORT_INSTANCE  *Xhc,
  IN UINT32                    Offset,
  IN UINT32                    Bit
  );

/**
  Read XHCI debug register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the debug register.

  @return The register content read

**/
UINT32
XhcReadDebugReg (
  IN  USB3_DEBUG_PORT_INSTANCE    *Xhc,
  IN  UINT32                      Offset
  );

/**
  Read XHCI MMIO register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the MMIO register.

  @return The register content read

**/
UINT32
XhcReadMmioReg (
  IN  USB3_DEBUG_PORT_INSTANCE    *Xhc,
  IN  UINT32                      Offset
  );

/**
  Set one bit of the debug register while keeping other bits.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the debug register.
  @param  Bit          The bit mask of the register to set.

**/
VOID
XhcSetDebugRegBit (
  IN USB3_DEBUG_PORT_INSTANCE *Xhc,
  IN UINT32                   Offset,
  IN UINT32                   Bit
  );

/**
  Set one bit of the MMIO register while keeping other bits.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the MMIO register.
  @param  Bit          The bit mask of the register to set.

**/
VOID
XhcSetMmioRegBit (
  IN USB3_DEBUG_PORT_INSTANCE   *Xhc,
  IN UINT32                     Offset,
  IN UINT32                     Bit
  );

/**
  Wait the MMIO register's bit as specified by Bit
  to be set (or clear).

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the MMIO register.
  @param  Bit          The bit of the register to wait for.
  @param  WaitToSet    Wait the bit to set or clear.
  @param  Timeout      The time to wait before abort (in millisecond, ms).

  @retval EFI_SUCCESS  The bit successfully changed by host controller.
  @retval EFI_TIMEOUT  The time out occurred.

**/
EFI_STATUS
XhcWaitMmioRegBit (
  IN USB3_DEBUG_PORT_INSTANCE  *Xhc,
  IN UINT32                    Offset,
  IN UINT32                    Bit,
  IN BOOLEAN                   WaitToSet,
  IN UINT32                    Timeout
  );

/**
  Write the data to the debug register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the debug register.
  @param  Data         The data to write.

**/
VOID
XhcWriteDebugReg (
  IN USB3_DEBUG_PORT_INSTANCE   *Xhc,
  IN UINT32                     Offset,
  IN UINT32                     Data
  );

/**
  Write the data to the MMIO register.

  @param  Xhc          The XHCI Instance.
  @param  Offset       The offset of the MMIO register.
  @param  Data         The data to write.

**/
VOID
XhcWriteMmioReg (
  IN USB3_DEBUG_PORT_INSTANCE   *Xhc,
  IN UINT32                     Offset,
  IN UINT32                     Data
  );

/**
  Discover the USB3 debug device.

  @param[in] Instance           Pointer to USB3 debug port object instance.

  @retval RETURN_SUCCESS        The USB3 debug device was found.
  @retval RETURN_DEVICE_ERROR   The USB3 debug device was not found.

**/
RETURN_STATUS
DiscoverUsb3DebugPort(
  IN USB3_DEBUG_PORT_INSTANCE   *Instance
  );

/**
  Initialize the USB3 debug Device hardware.

  @param[in] Instance       Pointer to USB3 debug port object instance.

  @retval RETURN_SUCCESS    The USB3 debug device was initialized successfully.
  @retval !RETURN_SUCCESS   Error.

**/
RETURN_STATUS
InitializeUsb3DebugPort (
  IN USB3_DEBUG_PORT_INSTANCE   *Instance
  );

/**
  Return XHCI MMIO base address.

**/
EFI_PHYSICAL_ADDRESS
GetXhciBaseAddress (
  VOID
  );

/**
  Return XHCI debug instance address.

**/
USB3_DEBUG_PORT_INSTANCE *
GetUsb3DebugPortInstance (
  VOID
  );

/**
  Send data over the USB3 debug cable.

  @param[out]      Data    Pointer to data
  @param[in, out]  Length  Pointer to data length

**/
VOID
Usb3DbgOut (
  OUT      UINT8                           *Data,
  IN OUT   UINTN                           *Length
  );

/**
  Receive data over the USB3 debug cable.

  @param[out]     Data      Pointer to data
  @param[in, out] Length    Pointer to data length

**/
RETURN_STATUS
Usb3DbgIn (
      OUT UINT8                           *Data,
  IN  OUT UINTN                           *Length
  );

/**
  Verifies if the bit positions specified by a mask are set in a register.

  @param[in, out] Register    UNITN register
  @param[in]      BitMask     32-bit mask

  @return  BOOLEAN  - TRUE  if all bits specified by the mask are enabled.
                    - FALSE even if one of the bits specified by the mask
                            is not enabled.
**/
BOOLEAN
XhcIsBitSet(
  UINTN   Register,
  UINT32  BitMask
  );

/**
  Sets bits as per the enabled bit positions in the mask.

  @param[in, out] Register    UINTN register
  @param[in]      BitMask     32-bit mask
**/
VOID
XhcSetR32Bit(
  UINTN   Register,
  UINT32  BitMask
  );

/**
  Clears bits as per the enabled bit positions in the mask.

  @param[in, out] Register    UINTN register
  @param[in]      BitMask     32-bit mask
**/
VOID
XhcClrR32Bit(
  UINTN   Register,
  UINT32  BitMask
  );

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
  );

/**
  Return command register value in XHCI controller.

**/
UINT16
GetXhciPciCommand (
  VOID
  );

/**
  Allocate aligned memory for XHC's usage.

  @param BufferSize     The size, in bytes, of the Buffer.

  @return A pointer to the allocated buffer or NULL if allocation fails.

**/
VOID*
AllocateAlignBuffer (
  IN UINTN                    BufferSize
  );

/**
  Check whether AllocatePages in permanent memory is ready.

  @retval TRUE  AllocatePages in permanent memory is ready.
  @retval FALSE AllocatePages in permanent memory is not ready.

**/
BOOLEAN
IsAllocatePagesReady (
  VOID
  );

/**
  The real function to initialize USB3 debug port.

  This method invokes various internal functions to facilitate
  detection and initialization of USB3 debug port.

  @retval RETURN_SUCCESS        The serial device was initialized.
**/
RETURN_STATUS
EFIAPI
USB3InitializeReal (
  VOID
  );

/**
  Update XHC hardware address when MMIO base is changed.

  @param  Instance          The XHCI Instance.
  @param  XhcMmioBase       XHCI MMIO base address.

**/
VOID
FixUsb3InstanceResource (
  IN OUT USB3_DEBUG_PORT_INSTANCE           *Instance,
  IN EFI_PHYSICAL_ADDRESS                   XhcMmioBase
  );

/**
  Save USB3 instance address.

  @param[in] Instance   The XHCI Instance.

**/
VOID
SaveUsb3InstanceAddress (
  IN USB3_DEBUG_PORT_INSTANCE   *Instance
  );

/**
  Submits bulk transfer to a bulk endpoint of a USB device.

  @param  Xhc                   The instance of debug device.
  @param  Direction             The direction of data transfer.
  @param  Data                  Array of pointers to the buffers of data to transmit
                                from or receive into.
  @param  DataLength            The lenght of the data buffer.
  @param  Timeout               Indicates the maximum time, in millisecond, which
                                the transfer is allowed to complete.
  @param  TransferResult        Transfer result.

  @retval EFI_SUCCESS           The transfer was completed successfully.
  @retval EFI_OUT_OF_RESOURCES  The transfer failed due to lack of resource.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_TIMEOUT           The transfer failed due to timeout.
  @retval EFI_DEVICE_ERROR      The transfer failed due to host controller error.

**/
EFI_STATUS
EFIAPI
XhcDataTransfer (
  IN     USB3_DEBUG_PORT_INSTANCE            *Xhc,
  IN     EFI_USB_DATA_DIRECTION              Direction,
  IN OUT VOID                                *Data,
  IN OUT UINTN                               *DataLength,
  IN     UINTN                               Timeout,
  OUT    UINT32                              *TransferResult
  );

#endif //__SERIAL_PORT_LIB_USB__
