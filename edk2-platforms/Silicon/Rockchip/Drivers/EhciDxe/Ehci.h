/** @file

  Provides some data struct used by EHCI controller driver.

Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
Copyright (c) Microsoft Corporation.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _EFI_EHCI_H_
#define _EFI_EHCI_H_

#include <Uefi.h>

#include <Protocol/Usb2HostController.h>
#include <Library/DmaLib.h>

#include <Guid/EventGroup.h>

#include <Library/RockchipPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/IoLib.h>
#include <Protocol/NonDiscoverableDevice.h>

typedef struct _USB2_HC_DEV  USB2_HC_DEV;

#include "UsbHcMem.h"
#include "EhciReg.h"
#include "EhciUrb.h"
#include "EhciSched.h"
#include "EhciDebug.h"

//
// EHC timeout experience values
//

#define EHC_1_MICROSECOND            1
#define EHC_1_MILLISECOND            (1000 * EHC_1_MICROSECOND)
#define EHC_1_SECOND                 (1000 * EHC_1_MILLISECOND)

//
// EHCI register operation timeout, set by experience
//
#define EHC_RESET_TIMEOUT            (1 * EHC_1_SECOND)
#define EHC_GENERIC_TIMEOUT          (10 * EHC_1_MILLISECOND)

//
// Wait for roothub port power stable, refers to Spec[EHCI1.0-2.3.9]
//
#define EHC_ROOT_PORT_RECOVERY_STALL (20 * EHC_1_MILLISECOND)

//
// Sync and Async transfer polling interval, set by experience,
// and the unit of Async is 100us, means 1ms as interval.
//
#define EHC_SYNC_POLL_INTERVAL       (1 * EHC_1_MILLISECOND)
#define EHC_ASYNC_POLL_INTERVAL      EFI_TIMER_PERIOD_MILLISECONDS(1)

//
// EHCI debug port control status register bit definition
//
#define USB_DEBUG_PORT_IN_USE        BIT10
#define USB_DEBUG_PORT_ENABLE        BIT28
#define USB_DEBUG_PORT_OWNER         BIT30
#define USB_DEBUG_PORT_IN_USE_MASK   (USB_DEBUG_PORT_IN_USE | \
                                      USB_DEBUG_PORT_OWNER)

//
// EHC raises TPL to TPL_NOTIFY to serialize all its operations
// to protect shared data structures.
//
#define  EHC_TPL                     TPL_NOTIFY

#define EFI_LIST_CONTAINER(Entry, Type, Field) BASE_CR(Entry, Type, Field)


#define EHC_LOW_32BIT(Addr64)     ((UINT32)(((UINTN)(Addr64)) & 0XFFFFFFFF))
#define EHC_HIGH_32BIT(Addr64)    ((UINT32)(RShiftU64((UINTN)(Addr64), 32) & 0XFFFFFFFF))
#define EHC_BIT_IS_SET(Data, Bit) ((BOOLEAN)(((Data) & (Bit)) == (Bit)))

#define EHC_REG_BIT_IS_SET(Ehc, Offset, Bit) \
          (EHC_BIT_IS_SET(EhcReadOpReg ((Ehc), (Offset)), (Bit)))

#define USB2_HC_DEV_SIGNATURE  SIGNATURE_32 ('e', 'h', 'c', 'i')
#define EHC_FROM_THIS(a)       CR(a, USB2_HC_DEV, Usb2Hc, USB2_HC_DEV_SIGNATURE)

typedef struct {
  VENDOR_DEVICE_PATH            Guid;
  UINTN                         Instance;
  EFI_DEVICE_PATH_PROTOCOL      End;
} EHCI_DEVICE_PATH;


struct _USB2_HC_DEV {
  UINTN                     Signature;
  EFI_USB2_HC_PROTOCOL      Usb2Hc;
  EFI_HANDLE                Controller;

  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  USBHC_MEM_POOL            *MemPool;
  UINT32                    UsbHostControllerBaseAddress;
  //
  // Schedule data shared between asynchronous and periodic
  // transfers:
  // ShortReadStop, as its name indicates, is used to terminate
  // the short read except the control transfer. EHCI follows
  // the alternative next QTD point when a short read happens.
  // For control transfer, even the short read happens, try the
  // status stage.
  //
  EHC_QTD                  *ShortReadStop;
  EFI_EVENT                 PollTimer;

  //
  // ExitBootServicesEvent is used to stop the EHC DMA operation
  // after exit boot service.
  //
  EFI_EVENT                 ExitBootServiceEvent;

  //
  // Asynchronous(bulk and control) transfer schedule data:
  // ReclaimHead is used as the head of the asynchronous transfer
  // list. It acts as the reclamation header.
  //
  EHC_QH                   *ReclaimHead;

  //
  // Periodic (interrupt) transfer schedule data:
  //
  VOID                      *PeriodFrame;     // the buffer pointed by this pointer is used to store pci bus address of the QH descriptor.
  VOID                      *PeriodFrameHost; // the buffer pointed by this pointer is used to store host memory address of the QH descriptor.
  VOID                      *PeriodFrameMap;

  EHC_QH                    *PeriodOne;
  LIST_ENTRY                AsyncIntTransfers;

  //
  // EHCI configuration data
  //
  UINT32                    HcStructParams; // Cache of HC structure parameter, EHC_HCSPARAMS_OFFSET
  UINT32                    HcCapParams;    // Cache of HC capability parameter, HCCPARAMS
  UINT32                    CapLen;         // Capability length

  //
  // Misc
  //
  EFI_UNICODE_STRING_TABLE  *ControllerNameTable;

  //
  // EHCI debug port info
  //
  UINT16                    DebugPortOffset; // The offset of debug port mmio register
  UINT8                     DebugPortBarNum; // The bar number of debug port mmio register
  UINT8                     DebugPortNum;    // The port number of usb debug port

  BOOLEAN                   Support64BitDma; // Whether 64 bit DMA may be used with this device
};

#endif

