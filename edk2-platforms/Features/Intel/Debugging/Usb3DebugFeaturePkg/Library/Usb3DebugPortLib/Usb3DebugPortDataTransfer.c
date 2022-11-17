/** @file
  Usb3 Debug Port library instance

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
#include <Library/Usb3DebugPortParamLib.h>
#include "Usb3DebugPortLibInternal.h"

EFI_SMRAM_DESCRIPTOR        mSmramCheckRanges[MAX_SMRAM_RANGE];
UINTN                       mSmramCheckRangeCount = 0;
BOOLEAN                     mUsb3InSmm            = FALSE;
UINT64                      mUsb3MmioSize         = 0;

/**
  Synchronize the specified transfer ring to update the enqueue and dequeue pointer.

  @param  Xhc         The XHCI Instance.
  @param  TrsRing     The transfer ring to sync.

  @retval EFI_SUCCESS The transfer ring is synchronized successfully.

**/
EFI_STATUS
EFIAPI
XhcSyncTrsRing (
  IN USB3_DEBUG_PORT_INSTANCE  *Xhc,
  IN TRANSFER_RING             *TrsRing
  )
{
  UINTN               Index;
  TRB_TEMPLATE        *TrsTrb;

  ASSERT (TrsRing != NULL);

  //
  // Calculate the latest RingEnqueue and RingPCS
  //
  TrsTrb = (TRB_TEMPLATE *)(UINTN) TrsRing->RingEnqueue;

  ASSERT (TrsTrb != NULL);

  for (Index = 0; Index < TrsRing->TrbNumber; Index++) {
    if (TrsTrb->CycleBit != (TrsRing->RingPCS & BIT0)) {
      break;
    }
    TrsTrb++;
    if ((UINT8) TrsTrb->Type == TRB_TYPE_LINK) {
      ASSERT (((LINK_TRB*)TrsTrb)->TC != 0);
      //
      // set cycle bit in Link TRB as normal
      //
      ((LINK_TRB*)TrsTrb)->CycleBit = TrsRing->RingPCS & BIT0;
      //
      // Toggle PCS maintained by software
      //
      TrsRing->RingPCS = (TrsRing->RingPCS & BIT0) ? 0 : 1;
      TrsTrb           = (TRB_TEMPLATE *)(UINTN)((TrsTrb->Parameter1 | LShiftU64 ((UINT64)TrsTrb->Parameter2, 32)) & ~0x0F);
    }
  }
  ASSERT (Index != TrsRing->TrbNumber);

  if ((EFI_PHYSICAL_ADDRESS)(UINTN) TrsTrb != TrsRing->RingEnqueue) {
    TrsRing->RingEnqueue = (EFI_PHYSICAL_ADDRESS)(UINTN) TrsTrb;
  }

  //
  // Clear the Trb context for enqueue, but reserve the PCS bit
  //
  TrsTrb->Parameter1 = 0;
  TrsTrb->Parameter2 = 0;
  TrsTrb->Status     = 0;
  TrsTrb->RsvdZ1     = 0;
  TrsTrb->Type       = 0;
  TrsTrb->Control    = 0;

  return EFI_SUCCESS;
}

/**
  Synchronize the specified event ring to update the enqueue and dequeue pointer.

  @param  Xhc         The XHCI Instance.
  @param  EvtRing     The event ring to sync.

  @retval EFI_SUCCESS The event ring is synchronized successfully.

**/
EFI_STATUS
EFIAPI
XhcSyncEventRing (
  IN USB3_DEBUG_PORT_INSTANCE  *Xhc,
  IN EVENT_RING                *EvtRing
  )
{
  UINTN               Index;
  TRB_TEMPLATE        *EvtTrb1;

  ASSERT (EvtRing != NULL);

  //
  // Calculate the EventRingEnqueue and EventRingCCS.
  // Note: only support single Segment
  //
  EvtTrb1 = (TRB_TEMPLATE *)(UINTN) EvtRing->EventRingDequeue;

  for (Index = 0; Index < EvtRing->TrbNumber; Index++) {
    if (EvtTrb1->CycleBit != EvtRing->EventRingCCS) {
      break;
    }

    EvtTrb1++;

    if ((UINTN)EvtTrb1 >= ((UINTN) EvtRing->EventRingSeg0 + sizeof (TRB_TEMPLATE) * EvtRing->TrbNumber)) {
      EvtTrb1 = (TRB_TEMPLATE *)(UINTN) EvtRing->EventRingSeg0;
      EvtRing->EventRingCCS = (EvtRing->EventRingCCS) ? 0 : 1;
    }
  }

  if (Index < EvtRing->TrbNumber) {
    EvtRing->EventRingEnqueue = (EFI_PHYSICAL_ADDRESS)(UINTN)EvtTrb1;
  } else {
    ASSERT (FALSE);
  }

  return EFI_SUCCESS;
}

/**
  Check if there is a new generated event.

  @param  Xhc           The XHCI Instance.
  @param  EvtRing       The event ring to check.
  @param  NewEvtTrb     The new event TRB found.

  @retval EFI_SUCCESS   Found a new event TRB at the event ring.
  @retval EFI_NOT_READY The event ring has no new event.

**/
EFI_STATUS
EFIAPI
XhcCheckNewEvent (
  IN  USB3_DEBUG_PORT_INSTANCE *Xhc,
  IN  EVENT_RING               *EvtRing,
  OUT TRB_TEMPLATE             **NewEvtTrb
  )
{
  EFI_STATUS          Status;

  ASSERT (EvtRing != NULL);

  *NewEvtTrb = (TRB_TEMPLATE *)(UINTN) EvtRing->EventRingDequeue;

  if (EvtRing->EventRingDequeue == EvtRing->EventRingEnqueue) {
    return EFI_NOT_READY;
  }

  Status = EFI_SUCCESS;

  EvtRing->EventRingDequeue += sizeof (TRB_TEMPLATE);
  //
  // If the dequeue pointer is beyond the ring, then roll-back it to the begining of the ring.
  //
  if ((UINTN)EvtRing->EventRingDequeue >= ((UINTN) EvtRing->EventRingSeg0 + sizeof (TRB_TEMPLATE) * EvtRing->TrbNumber)) {
    EvtRing->EventRingDequeue = EvtRing->EventRingSeg0;
  }

  return Status;
}

/**
  Check if the Trb is a transaction of the URB.

  @param Trb    The TRB to be checked
  @param Urb    The transfer ring to be checked.

  @retval TRUE  It is a transaction of the URB.
  @retval FALSE It is not any transaction of the URB.

**/
BOOLEAN
IsTransferRingTrb (
  IN  TRB_TEMPLATE        *Trb,
  IN  URB                 *Urb
  )
{
  TRB_TEMPLATE  *CheckedTrb;
  TRANSFER_RING *Ring;
  UINTN         Index;

  Ring = (TRANSFER_RING *)(UINTN) Urb->Ring;
  CheckedTrb = (TRB_TEMPLATE *)(UINTN) Ring->RingSeg0;

  ASSERT (Ring->TrbNumber == CMD_RING_TRB_NUMBER || Ring->TrbNumber == TR_RING_TRB_NUMBER);

  for (Index = 0; Index < Ring->TrbNumber; Index++) {
    if (Trb == CheckedTrb) {
      return TRUE;
    }
    CheckedTrb++;
  }

  return FALSE;
}

/**
  Check the URB's execution result and update the URB's
  result accordingly.

  @param  Xhc             The XHCI Instance.
  @param  Urb             The URB to check result.

  @return Whether the result of URB transfer is finialized.

**/
EFI_STATUS
XhcCheckUrbResult (
  IN  USB3_DEBUG_PORT_INSTANCE *Xhc,
  IN  URB                      *Urb
  )
{
  EVT_TRB_TRANSFER        *EvtTrb;
  TRB_TEMPLATE            *TRBPtr;
  UINTN                   Index;
  UINT8                   TRBType;
  EFI_STATUS              Status;
  URB                     *CheckedUrb;
  UINT64                  XhcDequeue;
  UINT32                  High;
  UINT32                  Low;

  ASSERT ((Xhc != NULL) && (Urb != NULL));

  Status   = EFI_SUCCESS;

  if (Urb->Finished) {
    goto EXIT;
  }

  EvtTrb = NULL;

  //
  // Traverse the event ring to find out all new events from the previous check.
  //
  XhcSyncEventRing (Xhc, &Xhc->EventRing);

  for (Index = 0; Index < Xhc->EventRing.TrbNumber; Index++) {
    Status = XhcCheckNewEvent (Xhc, &Xhc->EventRing, ((TRB_TEMPLATE **)&EvtTrb));
    if (Status == EFI_NOT_READY) {
      //
      // All new events are handled, return directly.
      //
      goto EXIT;
    }
    //
    // Only handle COMMAND_COMPLETETION_EVENT and TRANSFER_EVENT.
    //
    if ((EvtTrb->Type != TRB_TYPE_COMMAND_COMPLT_EVENT) && (EvtTrb->Type != TRB_TYPE_TRANS_EVENT)) {
      continue;
    }

    TRBPtr = (TRB_TEMPLATE *)(UINTN)(EvtTrb->TRBPtrLo | LShiftU64 ((UINT64) EvtTrb->TRBPtrHi, 32));

    //
    // Update the status of Urb according to the finished event regardless of whether
    // the urb is current checked one or in the XHCI's async transfer list.
    // This way is used to avoid that those completed async transfer events don't get
    // handled in time and are flushed by newer coming events.
    //
    if (IsTransferRingTrb (TRBPtr, Urb)) {
      CheckedUrb = Urb;
    } else {
      continue;
    }
    switch (EvtTrb->Completecode) {
      case TRB_COMPLETION_STALL_ERROR:
        CheckedUrb->Result  |= EFI_USB_ERR_STALL;
        CheckedUrb->Finished = TRUE;
        break;

      case TRB_COMPLETION_BABBLE_ERROR:
        CheckedUrb->Result  |= EFI_USB_ERR_BABBLE;
        CheckedUrb->Finished = TRUE;
        break;

      case TRB_COMPLETION_DATA_BUFFER_ERROR:
        CheckedUrb->Result  |= EFI_USB_ERR_BUFFER;
        CheckedUrb->Finished = TRUE;
        break;

      case TRB_COMPLETION_USB_TRANSACTION_ERROR:
        CheckedUrb->Result  |= EFI_USB_ERR_TIMEOUT;
        CheckedUrb->Finished = TRUE;
        break;

      case TRB_COMPLETION_SHORT_PACKET:
      case TRB_COMPLETION_SUCCESS:
        if (EvtTrb->Completecode == TRB_COMPLETION_SHORT_PACKET) {
        }

        TRBType = (UINT8) (TRBPtr->Type);
        if ((TRBType == TRB_TYPE_DATA_STAGE) ||
            (TRBType == TRB_TYPE_NORMAL) ||
            (TRBType == TRB_TYPE_ISOCH)) {
          CheckedUrb->Completed += (CheckedUrb->DataLen - EvtTrb->Length);
        }

        break;

      default:
        CheckedUrb->Result  |= EFI_USB_ERR_TIMEOUT;
        CheckedUrb->Finished = TRUE;
        break;
    }

    //
    // Only check first and end Trb event address
    //

    if ((EFI_PHYSICAL_ADDRESS)(UINTN) TRBPtr == CheckedUrb->TrbStart) {
      CheckedUrb->StartDone = TRUE;
    }

    if ((EFI_PHYSICAL_ADDRESS)(UINTN) TRBPtr == CheckedUrb->TrbEnd) {
      CheckedUrb->EndDone = TRUE;
    }

    if (CheckedUrb->StartDone && CheckedUrb->EndDone) {
      CheckedUrb->Finished = TRUE;
    }

  }

EXIT:
  //
  // Advance event ring to last available entry
  //
  // Some 3rd party XHCI external cards don't support single 64-bytes width register access,
  // So divide it to two 32-bytes width register access.
  //
  Low  = XhcReadDebugReg (Xhc, XHC_DC_DCERDP);
  High = XhcReadDebugReg (Xhc, XHC_DC_DCERDP + 4);
  XhcDequeue = (UINT64)(LShiftU64((UINT64)High, 32) | Low);

  if ((XhcDequeue & (~0x0F)) != ((UINT64)(UINTN)Xhc->EventRing.EventRingDequeue & (~0x0F))) {
    //
    // Some 3rd party XHCI external cards don't support single 64-bytes width register access,
    // So divide it to two 32-bytes width register access.
    //
    XhcWriteDebugReg (Xhc, XHC_DC_DCERDP, XHC_LOW_32BIT (Xhc->EventRing.EventRingDequeue));
    XhcWriteDebugReg (Xhc, XHC_DC_DCERDP + 4, XHC_HIGH_32BIT (Xhc->EventRing.EventRingDequeue));
  }

  return Status;
}

/**
  Ring the door bell to notify XHCI there is a transaction to be executed.

  @param  Xhc           The XHCI Instance.
  @param  Urb           The pointer to URB.

  @retval EFI_SUCCESS   Successfully ring the door bell.

**/
EFI_STATUS
EFIAPI
XhcRingDoorBell (
  IN USB3_DEBUG_PORT_INSTANCE  *Xhc,
  IN URB                       *Urb
  )
{
  UINT32      Dcdb;

  //
  // 7.6.8.2 DCDB Register
  //
  if (Urb->Direction == EfiUsbDataIn) {
    Dcdb = 0x100;
  } else {
    Dcdb = 0x0;
  }

  XhcWriteDebugReg (
    Xhc,
    XHC_DC_DCDB,
    Dcdb
    );

  return EFI_SUCCESS;
}

/**
  Execute the transfer by polling the URB. This is a synchronous operation.

  @param  Xhc               The XHCI Instance.
  @param  Urb               The URB to execute.
  @param  Timeout           The time to wait before abort, in millisecond.

  @return EFI_DEVICE_ERROR  The transfer failed due to transfer error.
  @return EFI_TIMEOUT       The transfer failed due to time out.
  @return EFI_SUCCESS       The transfer finished OK.

**/
EFI_STATUS
XhcExecTransfer (
  IN  USB3_DEBUG_PORT_INSTANCE *Xhc,
  IN  URB                      *Urb,
  IN  UINTN                    Timeout
  )
{
  EFI_STATUS              Status;
  UINTN                   Index;
  UINTN                   Loop;
  TRB_TEMPLATE            *Trb;
  TRANSFER_RING           *Ring;
  TRB_TEMPLATE            *TrbStart;
  TRB_TEMPLATE            *TrbEnd;

  Status = EFI_SUCCESS;

  Loop   = (Timeout * XHC_1_MILLISECOND / XHC_POLL_DELAY) + 1;
  if (Timeout == 0) {
    Loop = 0xFFFFFFFF;
  }
  XhcRingDoorBell (Xhc, Urb);
  //
  // DSCT BIT0: Event Ring Not Empty bit can only be set to 1 by XHC after ringing door bell with some delay.
  //

  for (Index = 0; Index < Loop; Index++) {
    Status = XhcCheckUrbResult (Xhc, Urb);
    if (Urb->Finished) {
      break;
    }
    MicroSecondDelay (XHC_POLL_DELAY);
  }
  if (Index == Loop) {
    Urb->Result = EFI_USB_ERR_TIMEOUT;
  }

  //
  // If URB transfer is error, restore transfer ring to original value before URB transfer
  // This will make the current transfer TRB is always at the latest unused one in transfer ring.
  // Without this code, when there is read TRB from target, but host does not write anything, this TRB (A)
  // will be still here, next read TRB (B) will be put next to TRB (A), when host write then, the TRB (A)
  // will be used to contain data, not TRB(B), this will cause Finished flag will not be set and return error in this function.
  //
  Ring = (TRANSFER_RING *)(UINTN) Urb->Ring;
  if (Urb->Result != EFI_USB_NOERROR) {
    Ring->RingEnqueue = Urb->TrbStart;
    //
    // Clear CCS flag for next use
    //
    TrbStart = (TRB_TEMPLATE *)(UINTN) Urb->TrbStart;
    TrbEnd   = (TRB_TEMPLATE *)(UINTN) Urb->TrbEnd;
    for (Trb = TrbStart; Trb <= TrbEnd; Trb++) {
      Trb->CycleBit = ((~Ring->RingPCS) & BIT0);
    }
  }
  return Status;
}

/**
  Create a transfer TRB.

  @param  Xhc     The XHCI Instance
  @param  Urb     The urb used to construct the transfer TRB.

  @return Created TRB or NULL

**/
EFI_STATUS
XhcCreateTransferTrb (
  IN USB3_DEBUG_PORT_INSTANCE   *Xhc,
  IN URB                        *Urb
  )
{
  TRANSFER_RING                 *EPRing;
  TRB                           *TrbStart;
  UINT32                        TotalLen;
  UINT32                        Len;
  UINT32                        TrbNum;

  Urb->Finished  = FALSE;
  Urb->StartDone = FALSE;
  Urb->EndDone   = FALSE;
  Urb->Completed = 0;
  Urb->Result    = EFI_USB_NOERROR;

  if (Urb->Direction == EfiUsbDataIn) {
    EPRing    = &Xhc->TransferRingIn;
  } else {
    EPRing    = &Xhc->TransferRingOut;
  }

  Urb->Ring = (EFI_PHYSICAL_ADDRESS)(UINTN) EPRing;
  //
  // Construct the TRB for ED_BULK_OUT and ED_BULK_IN
  //
  XhcSyncTrsRing (Xhc, EPRing);

  Urb->TrbStart = EPRing->RingEnqueue;

  TotalLen = 0;
  Len      = 0;
  TrbNum   = 0;
  TrbStart = (TRB *)(UINTN)EPRing->RingEnqueue;

  while (TotalLen < Urb->DataLen) {
    if ((TotalLen + 0x10000) >= Urb->DataLen) {
      Len = Urb->DataLen - TotalLen;
    } else {
      Len = 0x10000;
    }
    TrbStart = (TRB *)(UINTN)EPRing->RingEnqueue;
    TrbStart->TrbNormal.TRBPtrLo  = XHC_LOW_32BIT(Urb->Data + TotalLen);
    TrbStart->TrbNormal.TRBPtrHi  = XHC_HIGH_32BIT(Urb->Data + TotalLen);
    TrbStart->TrbNormal.Length    = Len;
    TrbStart->TrbNormal.TDSize    = 0;
    TrbStart->TrbNormal.IntTarget = 0;
    TrbStart->TrbNormal.ISP       = 1;
    TrbStart->TrbNormal.IOC       = 1;
    TrbStart->TrbNormal.Type      = TRB_TYPE_NORMAL;

    //
    // Update the cycle bit
    //
    TrbStart->TrbNormal.CycleBit = EPRing->RingPCS & BIT0;

    XhcSyncTrsRing (Xhc, EPRing);
    TrbNum++;
    TotalLen += Len;
  }

  Urb->TrbNum = TrbNum;

  //
  // Update to the last TRB
  //
  Urb->TrbEnd = (EFI_PHYSICAL_ADDRESS)(UINTN) TrbStart;
  return EFI_SUCCESS;
}

/**
  Create a new URB for a new transaction.

  @param  Xhc        The XHCI Instance
  @param  Direction  The direction of data flow.
  @param  Data       The user data to transfer
  @param  DataLen    The length of data buffer

  @return Created URB or NULL

**/
URB*
XhcCreateUrb (
  IN USB3_DEBUG_PORT_INSTANCE           *Xhc,
  IN EFI_USB_DATA_DIRECTION             Direction,
  IN VOID                               *Data,
  IN UINTN                              DataLen
  )
{
  EFI_STATUS                    Status;
  URB                           *Urb;
  EFI_PHYSICAL_ADDRESS          DataAddress;

  Urb = &Xhc->Urb;
  ASSERT (Urb->Data != 0);
  DataAddress = Urb->Data;
  ZeroMem (Urb, sizeof (URB));

  Urb->Signature = USB3_DEBUG_PORT_INSTANCE_SIGNATURE;
  Urb->Direction = Direction;
  Urb->Data = DataAddress;

  ZeroMem ((VOID*)(UINTN) Urb->Data, DataLen);
  CopyMem ((VOID*)(UINTN) Urb->Data, Data, DataLen);

  Urb->DataLen  = (UINT32) DataLen;
  Status = XhcCreateTransferTrb (Xhc, Urb);
  ASSERT_EFI_ERROR (Status);

  return Urb;
}

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
  )
{
  URB                     *Urb;
  EFI_STATUS              Status;

  //
  // Validate the parameters
  //
  if ((DataLength == NULL) || (*DataLength == 0) ||
      (Data == NULL) || (TransferResult == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *TransferResult = EFI_USB_ERR_SYSTEM;
  Status          = EFI_DEVICE_ERROR;

  //
  // Create a new URB, insert it into the asynchronous
  // schedule list, then poll the execution status.
  //
  Urb = XhcCreateUrb (Xhc, Direction, Data, *DataLength);

  if (Urb == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ON_EXIT;
  }

  Status = XhcExecTransfer (Xhc, Urb, Timeout);

  *TransferResult = Urb->Result;
  *DataLength     = Urb->Completed;

  if (*TransferResult == EFI_USB_NOERROR) {
    Status = EFI_SUCCESS;
  }

ON_EXIT:

  return Status;
}

/**
  Check whether the MMIO Bar is within any of the SMRAM ranges.

  @param[in]  XhcMmioBase  The address of the MMIO to be checked.

  @retval     TURE         The XHCI MMIO is in SMRAM ranges.
  @retval     FALSE        The XHCI MMIO is out of SMRAM ranges.
**/
BOOLEAN
EFIAPI
Usb3DebugIsAddressInSmram (
  IN EFI_PHYSICAL_ADDRESS   XhcMmioBase
  )
{
  UINTN                       Index;

  if (mSmramCheckRangeCount == 0) {
    //
    // When we invoke this function, we are already in SMM mode,
    // but SmmAccess->GetCapabilities failed which kept mSmramCheckRanges as 0.
    //
    return TRUE;
  }

  for (Index = 0; Index < mSmramCheckRangeCount; Index ++) {
    if (((XhcMmioBase >= mSmramCheckRanges[Index].CpuStart) && (XhcMmioBase < mSmramCheckRanges[Index].CpuStart + mSmramCheckRanges[Index].PhysicalSize)) ||
        ((mSmramCheckRanges[Index].CpuStart >= XhcMmioBase) && (mSmramCheckRanges[Index].CpuStart < XhcMmioBase + mUsb3MmioSize))) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
  Transfer data via XHC controller.

  @param  Data         Data buffer.
  @param  Length       Data length.
  @param  Direction    Transfer direction.

**/
VOID
Usb3DebugPortDataTransfer (
  UINT8                   *Data,
  UINTN                   *Length,
  EFI_USB_DATA_DIRECTION  Direction
  )
{
  USB3_DEBUG_PORT_INSTANCE        *Instance;
  EFI_PHYSICAL_ADDRESS            XhcMmioBase;
  UINT16                          Command;
  UINT8                           Bus;
  UINT8                           Device;
  UINT8                           Function;
  UINT32                          TransferResult;
  UINT32                          Dcctrl;
  EFI_PHYSICAL_ADDRESS            UsbBase;
  UINTN                           BytesToSend;
  USB3_DEBUG_PORT_CONTROLLER      UsbDebugPort;
  EFI_STATUS                      Status;
  USB3_DEBUG_PORT_INSTANCE        UsbDbgInstance;

  UsbDebugPort.Controller = GetUsb3DebugPortController();
  Bus      = UsbDebugPort.PciAddress.Bus;
  Device   = UsbDebugPort.PciAddress.Device;
  Function = UsbDebugPort.PciAddress.Function;

  //
  // MMIO base address is possible to clear, set it if it is cleared. (XhciMemorySpaceClose in PchUsbCommon.c)
  //
  XhcMmioBase = GetXhciBaseAddress ();
  Command     = GetXhciPciCommand ();

  if ((XhcMmioBase == 0) || (XhcMmioBase == XHCI_BASE_ADDRESS_64_BIT_MASK)) {
    //
    // XHCI device MMIO base is cleared by someone, set it again
    //
    UsbBase  = PcdGet32 (PcdXhciDefaultBaseAddress);
    PciWrite32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET), (UINT32)UsbBase);
    PciWrite32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET + 4), 0x0);
    UsbBase = PciRead32 (PCI_LIB_ADDRESS(Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET)) & XHCI_BASE_ADDRESS_32_BIT_MASK;
    if (UsbBase == 0 || UsbBase == XHCI_BASE_ADDRESS_32_BIT_MASK) {
      return;
    }
  }

  //
  // Check if XHC debug MMIO range is in SMRAM
  //
  if ((mUsb3InSmm) && (Usb3DebugIsAddressInSmram (XhcMmioBase))) {
    return;
  }

  //
  // Save and set Command Register
  //
  if (((Command & EFI_PCI_COMMAND_MEMORY_SPACE) == 0) || ((Command & EFI_PCI_COMMAND_BUS_MASTER) == 0)) {
    PciWrite16(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET), Command | EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
    PciRead16(PCI_LIB_ADDRESS(Bus, Device, Function, PCI_COMMAND_OFFSET));
  }

  Instance = GetUsb3DebugPortInstance ();

  if (Instance != NULL) {
    if (!Instance->DebugSupport) {
      //
      // Debug device is not supported by XHCI, return
      //
      goto Done;
    }
  }

  if ((Instance != NULL) && (Instance->Ready)) {
    //
    // Debug device is broken suddently (e.g. Windows OS), return
    //
    Dcctrl = XhcReadDebugReg (Instance, XHC_DC_DCCTRL);
    if ((Dcctrl & BIT0) == 0) {
      goto Done;
    }
  }

  if ((Instance != NULL) && (!Instance->Ready)) {
    //
    // Debug host does not connect with target
    //
    goto Done;
  }

  if (Instance == NULL) {
    ZeroMem (&UsbDbgInstance, sizeof (USB3_DEBUG_PORT_INSTANCE));
    DiscoverUsb3DebugPort (&UsbDbgInstance);
    if (UsbDbgInstance.DebugSupport) {
      if (!IsAllocatePagesReady ()) {
        //
        // AllocatePages can not work, return
        //
        goto Done;
      }
    }
    Status = USB3InitializeReal ();
    if (EFI_ERROR (Status)) {
      //
      // Debug device is failed to initialize
      //
      goto Done;
    }

    //
    // Update instance
    //
    Instance = GetUsb3DebugPortInstance ();

    if (Instance == NULL) {
      //
      // Debug device instance is failed to create
      //
      goto Done;
    }

    if ((!Instance->Ready) || (!Instance->DebugSupport)) {
      //
      // Debug host does not connect at first or is not supported
      //
      goto Done;
    }
  }

  BytesToSend = 0;
  while (*Length > 0) {
    BytesToSend = ((*Length) > XHC_DEBUG_PORT_DATA_LENGTH) ? XHC_DEBUG_PORT_DATA_LENGTH : *Length;
    XhcDataTransfer (
      Instance,
      Direction,
      Data,
      &BytesToSend,
      DATA_TRANSFER_TIME_OUT,
      &TransferResult
      );
    if (TransferResult != EFI_USB_NOERROR) {
      break;
    }
    *Length -= BytesToSend;
    Data += BytesToSend;
  }

Done:
  //
  // Restore Command Register
  //
  PciWrite16(PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), Command);

}

/**
  Receive data over the USB3 debug cable.

  @param[out]     Data      Pointer to data
  @param[in, out] Length    Pointer to data length

**/
RETURN_STATUS
Usb3DbgIn (
      OUT UINT8                           *Data,
  IN  OUT UINTN                           *Length
  )
{
  Usb3DebugPortDataTransfer (Data, Length, EfiUsbDataIn);
  return EFI_SUCCESS;
}

/**
  Send data over the USB3 debug cable.

  @param[out]      Data    Pointer to data
  @param[in, out]  Length  Pointer to data length

**/
VOID
Usb3DbgOut (
  OUT      UINT8                           *Data,
  IN OUT   UINTN                           *Length
  )
{
  Usb3DebugPortDataTransfer (Data, Length, EfiUsbDataOut);
}
