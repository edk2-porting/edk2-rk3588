/** @file
 *
 *  Copyright (c) 2020, ARM Limited. All rights reserved.
 *  Copyright (c) 2017-2018, Andrey Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2015-2016, Linaro Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#ifndef __DWUSBHOSTDXE_H__
#define __DWUSBHOSTDXE_H__

#include <Uefi.h>

#include <IndustryStandard/Bcm2836.h>
#include <IndustryStandard/RpiMbox.h>
#include <Protocol/Usb2HostController.h>
#include <Protocol/RpiFirmware.h>

#include <Guid/EventGroup.h>

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/DmaLib.h>
#include <Library/ArmLib.h>

#define MAX_DEVICE                      16
#define MAX_ENDPOINT                    16

#define DWUSB_OTGHC_DEV_SIGNATURE       SIGNATURE_32 ('d', 'w', 'h', 'c')
#define DWHC_FROM_THIS(a)               CR(a, DWUSB_OTGHC_DEV, DwUsbOtgHc, DWUSB_OTGHC_DEV_SIGNATURE)

//
// Iterate through the double linked list. NOT delete safe
//
#define EFI_LIST_FOR_EACH(Entry, ListHead)    \
  for(Entry = (ListHead)->ForwardLink; Entry != (ListHead); Entry = Entry->ForwardLink)

//
// Iterate through the double linked list. This is delete-safe.
// Do not touch NextEntry
//
#define EFI_LIST_FOR_EACH_SAFE(Entry, NextEntry, ListHead)            \
  for(Entry = (ListHead)->ForwardLink, NextEntry = Entry->ForwardLink;\
      Entry != (ListHead); Entry = NextEntry, NextEntry = Entry->ForwardLink)

#define EFI_LIST_CONTAINER(Entry, Type, Field) BASE_CR(Entry, Type, Field)

//
// The RequestType in EFI_USB_DEVICE_REQUEST is composed of
// three fields: One bit direction, 2 bit type, and 5 bit
// target.
//
#define USB_REQUEST_TYPE(Dir, Type, Target)                             \
  ((UINT8)((((Dir) == EfiUsbDataIn ? 0x01 : 0) << 7) | (Type) | (Target)))

typedef struct {
  VENDOR_DEVICE_PATH            Custom;
  EFI_DEVICE_PATH_PROTOCOL      EndDevicePath;
} EFI_DW_DEVICE_PATH;

typedef struct _DWUSB_DEFERRED_REQ {
  IN OUT LIST_ENTRY                         List;
  IN     struct _DWUSB_OTGHC_DEV            *DwHc;
  IN     UINT32                             Channel;
  IN     UINT32                             FrameInterval;
  IN     UINT32                             TargetFrame;
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator;
  IN     UINT8                              DeviceSpeed;
  IN     UINT8                              DeviceAddress;
  IN     UINTN                              MaximumPacketLength;
  IN     UINT32                             TransferDirection;
  IN OUT VOID                               *Data;
  IN OUT UINTN                              DataLength;
  IN OUT UINT32                             Pid;
  IN     UINT32                             EpAddress;
  IN     UINT32                             EpType;
  OUT    UINT32                             TransferResult;
  IN     BOOLEAN                            IgnoreAck;
  IN     EFI_ASYNC_USB_TRANSFER_CALLBACK    CallbackFunction;
  IN     VOID                               *CallbackContext;
  IN     UINTN                              TimeOut;
} DWUSB_DEFERRED_REQ;

typedef struct _DWUSB_OTGHC_DEV {
  UINTN                           Signature;

  EFI_USB2_HC_PROTOCOL            DwUsbOtgHc;

  EFI_USB_HC_STATE                DwHcState;

  EFI_EVENT                       ExitBootServiceEvent;

  EFI_EVENT                       PeriodicEvent;

  EFI_PHYSICAL_ADDRESS            DwUsbBase;
  UINT8                           *StatusBuffer;

  UINT8                           *AlignedBuffer;
  VOID *                          AlignedBufferMapping;
  UINTN                           AlignedBufferBusAddress;
  LIST_ENTRY                      DeferredList;
  /*
   * 1ms frames.
   */
  UINTN                           CurrentFrame;
  /*
   * 125us frames;
   */
  UINT16                          LastMicroFrame;
} DWUSB_OTGHC_DEV;

extern EFI_COMPONENT_NAME_PROTOCOL  gComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL gComponentName2;
extern EFI_DRIVER_BINDING_PROTOCOL  mDriverBinding;

EFI_STATUS
CreateDwUsbHc (
  OUT DWUSB_OTGHC_DEV **OutDwHc
  );

VOID
DestroyDwUsbHc (
  IN  DWUSB_OTGHC_DEV *Dev
  );

EFI_STATUS
EFIAPI
DwHcReset (
  IN  EFI_USB2_HC_PROTOCOL *This,
  IN  UINT16               Attributes
  );

EFI_STATUS
EFIAPI
DwHcSetState (
  IN  EFI_USB2_HC_PROTOCOL *This,
  IN  EFI_USB_HC_STATE     State
  );

VOID
DwHcQuiesce (
  IN  DWUSB_OTGHC_DEV *DwHc
  );

#endif /* __DWUSBHOSTDXE_H__ */
