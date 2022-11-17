/** @file
 *
*   Copyright (c) 2020, ARM Limited. All rights reserved.
 *  Copyright (c) 2017-2018, Andrey Warkentin <andrey.warkentin@gmail.com>
 *  Copyright (c) 2015-2016, Linaro Limited. All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <IndustryStandard/Bcm2836.h>
#include "DwUsbHostDxe.h"
#include "DwcHw.h"

/*
 * Excessive (10s) timeout for reset.
 */
#define DW_HC_RESET_TIMEOUT_MS (10000)

  /*
   * TimerPeriodic to account for timeout processing
   * within DwHcTransfer.
   */
#define TimerForTransfer TimerRelative

/*
 * https://www.quicklogic.com/assets/pdf/data-sheets/QL-Hi-Speed-USB-2.0-OTG-Controller-Data-Sheet.pdf
 */

typedef enum {
  XFER_NOT_HALTED,
  XFER_ERROR,
  XFER_CSPLIT,
  XFER_NAK,
  XFER_STALL,
  XFER_FRMOVRUN,
  XFER_DONE
} CHANNEL_HALT_REASON;

typedef struct {
  BOOLEAN Splitting;
  BOOLEAN SplitStart;
  UINT32 Tries;
} SPLIT_CONTROL;

EFI_STATUS
DwHcInit (
  IN DWUSB_OTGHC_DEV *DwHc,
  IN EFI_EVENT       Timeout
  );

EFI_STATUS
DwCoreInit (
  IN DWUSB_OTGHC_DEV *DwHc,
  IN EFI_EVENT        Timeout
  );

EFI_STATUS
Wait4Bit (
  IN  EFI_EVENT Timeout,
  IN  UINT32    Reg,
  IN  UINT32    Mask,
  IN  BOOLEAN   Set
  )
{
  UINT32 Value;

  do {
    Value = MmioRead32 (Reg);
    if (!Set) {
      Value = ~Value;
    }

    if ((Value & Mask) == Mask) {
      return EFI_SUCCESS;
    }
  } while (EFI_ERROR (gBS->CheckEvent (Timeout)));

  return EFI_TIMEOUT;
}

CHANNEL_HALT_REASON
Wait4Chhltd (
  IN  DWUSB_OTGHC_DEV *DwHc,
  IN  EFI_EVENT       Timeout,
  IN  UINT32          Channel,
  IN  UINT32          *Sub,
  IN  UINT32          *Toggle,
  IN  BOOLEAN         IgnoreAck,
  IN  SPLIT_CONTROL   *Split
  )
{
  EFI_STATUS Status;
  UINT32  Hcint, Hctsiz;
  UINT32  HcintCompHltAck = DWC2_HCINT_XFERCOMP;

  MicroSecondDelay (100);
  Status = Wait4Bit (Timeout, DwHc->DwUsbBase + HCINT (Channel),
                     DWC2_HCINT_CHHLTD, 1);
  if (EFI_ERROR (Status)) {
    return XFER_NOT_HALTED;
  }

  MicroSecondDelay (100);
  Hcint = MmioRead32 (DwHc->DwUsbBase + HCINT (Channel));

  ASSERT ((Hcint & DWC2_HCINT_CHHLTD) != 0);
  Hcint &= ~DWC2_HCINT_CHHLTD;

  if (!IgnoreAck ||
      (Split->Splitting && Split->SplitStart)) {
    HcintCompHltAck |= DWC2_HCINT_ACK;
  } else {
    Hcint &= ~DWC2_HCINT_ACK;
  }

  if ((Hcint & DWC2_HCINT_XACTERR) != 0) {
    return XFER_ERROR;
  }

  if ((Hcint & DWC2_HCINT_NYET) != 0) {
    return XFER_CSPLIT;
  }

  if ((Hcint & DWC2_HCINT_NAK) != 0) {
    return XFER_NAK;
  }

  if ((Hcint & DWC2_HCINT_STALL) != 0) {
    return XFER_STALL;
  }

  if ((Hcint & DWC2_HCINT_FRMOVRUN) != 0) {
    return XFER_FRMOVRUN;
  }

  if (Split->Splitting &&
      Split->SplitStart &&
      ((Hcint & DWC2_HCINT_ACK) != 0)) {
    Split->SplitStart = FALSE;
    Split->Tries = 0;
    return XFER_CSPLIT;
  }

  if (Hcint != HcintCompHltAck) {
    DEBUG ((DEBUG_ERROR, "Wait4Chhltd: Channel %u HCINT 0x%x %a%a\n",
      Channel, Hcint,
      IgnoreAck ? "IgnoreAck " : "",
      Split->SplitStart ? "split start" :
      (Split->Splitting ? "split complete" : "")));
    return XFER_ERROR;
  }

  Hctsiz = MmioRead32 (DwHc->DwUsbBase + HCTSIZ (Channel));
  *Sub = (Hctsiz & DWC2_HCTSIZ_XFERSIZE_MASK) >> DWC2_HCTSIZ_XFERSIZE_OFFSET;
  *Toggle = (Hctsiz & DWC2_HCTSIZ_PID_MASK) >> DWC2_HCTSIZ_PID_OFFSET;

  return XFER_DONE;
}

VOID
DwOtgHcInit (
  IN  DWUSB_OTGHC_DEV    *DwHc,
  IN  UINT8              HcNum,
  IN  EFI_USB2_HC_TRANSACTION_TRANSLATOR  *Translator,
  IN  UINT8              DeviceSpeed,
  IN  UINT8              DevAddr,
  IN  UINT8              Endpoint,
  IN  UINT8              EpDir,
  IN  UINT8              EpType,
  IN  UINT16             MaxPacket,
  IN  SPLIT_CONTROL *SplitControl
  )
{
  UINT32 Split = 0;
  UINT32 Hcchar = (DevAddr << DWC2_HCCHAR_DEVADDR_OFFSET) |
    (Endpoint << DWC2_HCCHAR_EPNUM_OFFSET) |
    (EpDir << DWC2_HCCHAR_EPDIR_OFFSET) |
    (EpType << DWC2_HCCHAR_EPTYPE_OFFSET) |
    (MaxPacket << DWC2_HCCHAR_MPS_OFFSET) |
    ((DeviceSpeed == EFI_USB_SPEED_LOW) ? DWC2_HCCHAR_LSPDDEV : 0);

  MmioWrite32 (DwHc->DwUsbBase + HCINT (HcNum), 0x3FFF);

  MmioWrite32 (DwHc->DwUsbBase + HCCHAR (HcNum), Hcchar);

  if (SplitControl->Splitting) {
    Split = DWC2_HCSPLT_SPLTENA |
      ((Translator->TranslatorPortNumber) << DWC2_HCSPLT_PRTADDR_OFFSET) |
      ((Translator->TranslatorHubAddress) << DWC2_HCSPLT_HUBADDR_OFFSET);

    if (!SplitControl->SplitStart) {
      Split |= DWC2_HCSPLT_COMPSPLT;
    }
  }

  MmioWrite32 (DwHc->DwUsbBase + HCSPLT (HcNum), Split);
}

EFI_STATUS
DwCoreReset (
  IN  DWUSB_OTGHC_DEV *DwHc,
  IN  EFI_EVENT Timeout
  )
{
  EFI_STATUS Status;

  Status = Wait4Bit (Timeout, DwHc->DwUsbBase + GRSTCTL, DWC2_GRSTCTL_AHBIDLE, 1);
  if (Status) {
    DEBUG ((DEBUG_ERROR, "DwCoreReset: AHBIDLE Timeout!\n"));
    return Status;
  }

  MmioWrite32 (DwHc->DwUsbBase + GRSTCTL, DWC2_GRSTCTL_CSFTRST);

  Status = Wait4Bit (Timeout, DwHc->DwUsbBase + GRSTCTL, DWC2_GRSTCTL_CSFTRST, 0);
  if (Status) {
    DEBUG ((DEBUG_ERROR, "DwCoreReset: CSFTRST Timeout!\n"));
    return Status;
  }

  MicroSecondDelay (100000);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
DwHcTransfer (
  IN      DWUSB_OTGHC_DEV        *DwHc,
  IN      EFI_EVENT              Timeout,
  IN      UINT32                 Channel,
  IN      EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
  IN      UINT8                  DeviceSpeed,
  IN      UINT8                  DeviceAddress,
  IN      UINTN                  MaximumPacketLength,
  IN  OUT UINT32                *Pid,
  IN      UINT32                 TransferDirection,
  IN  OUT VOID                  *Data,
  IN  OUT UINTN                 *DataLength,
  IN      UINT32                 EpAddress,
  IN      UINT32                 EpType,
  OUT     UINT32                 *TransferResult,
  IN      BOOLEAN                IgnoreAck
  )
{
  UINT32                          TxferLen;
  UINT32                          Done = 0;
  UINT32                          NumPackets;
  UINT32                          Sub;
  UINT32                          Ret = 0;
  UINT32                          StopTransfer = 0;
  EFI_STATUS                      Status = EFI_SUCCESS;
  SPLIT_CONTROL                   Split = { 0 };

  EFI_TPL Tpl = gBS->RaiseTPL (TPL_NOTIFY);

  *TransferResult = EFI_USB_NOERROR;

  do {
  RestartXfer:
    if (DeviceSpeed == EFI_USB_SPEED_LOW ||
        DeviceSpeed == EFI_USB_SPEED_FULL) {
      Split.Splitting = TRUE;
      Split.SplitStart = TRUE;
      Split.Tries = 0;
    }

    TxferLen = *DataLength - Done;

    if (TxferLen > DWC2_MAX_TRANSFER_SIZE) {
      TxferLen = DWC2_MAX_TRANSFER_SIZE - MaximumPacketLength + 1;
    }

    if (TxferLen > DWC2_DATA_BUF_SIZE) {
      TxferLen = DWC2_DATA_BUF_SIZE - MaximumPacketLength + 1;
    }

    if (Split.Splitting || TxferLen == 0) {
      NumPackets = 1;
    } else {
      NumPackets = (TxferLen + MaximumPacketLength - 1) / MaximumPacketLength;
      if (NumPackets > DWC2_MAX_PACKET_COUNT) {
        NumPackets = DWC2_MAX_PACKET_COUNT;
        TxferLen = NumPackets * MaximumPacketLength;
      }
    }

    if (TransferDirection) { // in
      TxferLen = NumPackets * MaximumPacketLength;
    } else {
      CopyMem (DwHc->AlignedBuffer, Data + Done, TxferLen);
      ArmDataSynchronizationBarrier ();
    }

  RestartChannel:
    MmioWrite32 (DwHc->DwUsbBase + HCDMA (Channel),
      (UINTN)DwHc->AlignedBufferBusAddress);

    DwOtgHcInit (DwHc, Channel, Translator, DeviceSpeed,
      DeviceAddress, EpAddress,
      TransferDirection, EpType,
      MaximumPacketLength, &Split);

    MmioWrite32 (DwHc->DwUsbBase + HCTSIZ (Channel),
      (TxferLen << DWC2_HCTSIZ_XFERSIZE_OFFSET) |
      (NumPackets << DWC2_HCTSIZ_PKTCNT_OFFSET) |
      (*Pid << DWC2_HCTSIZ_PID_OFFSET));

    MmioAndThenOr32 (DwHc->DwUsbBase + HCCHAR (Channel),
      ~(DWC2_HCCHAR_MULTICNT_MASK |
        DWC2_HCCHAR_CHEN |
        DWC2_HCCHAR_CHDIS),
        ((1 << DWC2_HCCHAR_MULTICNT_OFFSET) |
          DWC2_HCCHAR_CHEN));

    Ret = Wait4Chhltd (DwHc, Timeout, Channel, &Sub, Pid, IgnoreAck, &Split);

    if (Ret == XFER_NOT_HALTED) {
      *TransferResult = EFI_USB_ERR_TIMEOUT;
      MmioOr32 (DwHc->DwUsbBase + HCCHAR (Channel), DWC2_HCCHAR_CHDIS);
      Status = gBS->SetTimer (Timeout, TimerRelative,
                              EFI_TIMER_PERIOD_MILLISECONDS (1));
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR (Status)) {
        break;
      }
      Status = Wait4Bit (Timeout, DwHc->DwUsbBase + HCINT (Channel),
                         DWC2_HCINT_CHHLTD, 1);
      if (Status == EFI_SUCCESS) {
        Status = EFI_TIMEOUT;
      } else {
        DEBUG ((DEBUG_ERROR, "Channel %u did not halt\n", Channel));
        Status = EFI_DEVICE_ERROR;
      }
      break;
    } else if (Ret == XFER_STALL) {
      *TransferResult = EFI_USB_ERR_STALL;
      Status = EFI_DEVICE_ERROR;
      break;
    } else if (Ret == XFER_CSPLIT) {
      ASSERT (Split.Splitting);

      if (Split.Tries++ < 3) {
        goto RestartChannel;
      }

      goto RestartXfer;
    } else if (Ret == XFER_ERROR) {
      *TransferResult =
        EFI_USB_ERR_CRC |
        EFI_USB_ERR_TIMEOUT |
        EFI_USB_ERR_BITSTUFF |
        EFI_USB_ERR_SYSTEM;
      Status = EFI_DEVICE_ERROR;
      break;
    } else if (Ret == XFER_FRMOVRUN) {
      goto RestartChannel;
    } else if (Ret == XFER_NAK) {
      if (Split.Splitting &&
          (EpType == DWC2_HCCHAR_EPTYPE_CONTROL)) {
        goto RestartXfer;
      }

      *TransferResult = EFI_USB_ERR_NAK;
      Status = EFI_DEVICE_ERROR;
      break;
    }

    if (TransferDirection) { // in
      ArmDataSynchronizationBarrier ();
      TxferLen -= Sub;
      CopyMem (Data + Done, DwHc->AlignedBuffer, TxferLen);
      if (Sub) {
        StopTransfer = 1;
      }
    }

    Done += TxferLen;
  } while (Done < *DataLength && !StopTransfer);

  MmioWrite32 (DwHc->DwUsbBase + HCINTMSK (Channel), 0);
  MmioWrite32 (DwHc->DwUsbBase + HCINT (Channel), 0xFFFFFFFF);

  *DataLength = Done;

  gBS->RestoreTPL (Tpl);

  ASSERT (!EFI_ERROR (Status) || *TransferResult != EFI_USB_NOERROR);

  return Status;
}

STATIC
DWUSB_DEFERRED_REQ *
DwHcFindDeferredTransfer (
  IN  DWUSB_OTGHC_DEV *DwHc,
  IN  UINT8 DeviceAddress,
  IN  UINT8 EndPointAddress
  )
{
  LIST_ENTRY *Entry;

  EFI_LIST_FOR_EACH (Entry, &DwHc->DeferredList) {
    DWUSB_DEFERRED_REQ *Req = EFI_LIST_CONTAINER (Entry, DWUSB_DEFERRED_REQ, List);

    if (Req->DeviceAddress == DeviceAddress &&
        Req->EpAddress == (EndPointAddress & 0xF) &&
        Req->TransferDirection == ((EndPointAddress >> 7) & 0x01)) {
      return Req;
    }
  }

  return NULL;
}

STATIC
VOID
DwHcDeferredTransfer (
  IN  DWUSB_DEFERRED_REQ *Req
  )
{
  EFI_STATUS Status;
  EFI_EVENT TimeoutEvt = NULL;

  Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimeoutEvt);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Status = gBS->SetTimer (TimeoutEvt, TimerForTransfer,
                  EFI_TIMER_PERIOD_MILLISECONDS (Req->TimeOut));
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Req->TransferResult = EFI_USB_NOERROR;
  Status = DwHcTransfer (Req->DwHc, TimeoutEvt,
             Req->Channel, Req->Translator,
             Req->DeviceSpeed, Req->DeviceAddress,
             Req->MaximumPacketLength, &Req->Pid,
             Req->TransferDirection, Req->Data, &Req->DataLength,
             Req->EpAddress, Req->EpType, &Req->TransferResult,
             Req->IgnoreAck);

  if (Req->EpType == DWC2_HCCHAR_EPTYPE_INTR &&
      Status == EFI_DEVICE_ERROR &&
      Req->TransferResult == EFI_USB_ERR_NAK) {
    /*
     * Swallow the NAK, the upper layer expects us to resubmit automatically.
     */
    goto Exit;
  }

  Req->CallbackFunction (Req->Data, Req->DataLength,
         Req->CallbackContext,
         Req->TransferResult);
Exit:
  if (TimeoutEvt != NULL) {
    gBS->CloseEvent (TimeoutEvt);
  }
}

/**
   EFI_USB2_HC_PROTOCOL APIs
**/

EFI_STATUS
EFIAPI
DwHcGetCapability (
  IN  EFI_USB2_HC_PROTOCOL *This,
  OUT UINT8                *MaxSpeed,
  OUT UINT8                *PortNumber,
  OUT UINT8                *Is64BitCapable
  )
{
  if ((MaxSpeed == NULL) || (PortNumber == NULL) || (Is64BitCapable == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *MaxSpeed = EFI_USB_SPEED_HIGH;
  *PortNumber = 1;
  *Is64BitCapable = 1;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DwHcReset (
  IN EFI_USB2_HC_PROTOCOL *This,
  IN UINT16               Attributes
  )
{
  EFI_STATUS Status;
  EFI_EVENT TimeoutEvt = NULL;

  DWUSB_OTGHC_DEV *DwHc;
  DwHc = DWHC_FROM_THIS (This);

  switch (Attributes) {
  case EFI_USB_HC_RESET_GLOBAL:
  case EFI_USB_HC_RESET_HOST_CONTROLLER:
    break;

  case EFI_USB_HC_RESET_GLOBAL_WITH_DEBUG:
  case EFI_USB_HC_RESET_HOST_WITH_DEBUG:
    Status = EFI_UNSUPPORTED;
    goto Exit;

  default:
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimeoutEvt);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Status = gBS->SetTimer (TimeoutEvt, TimerRelative,
                  EFI_TIMER_PERIOD_MILLISECONDS (DW_HC_RESET_TIMEOUT_MS));
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Status = DwCoreInit (DwHc, TimeoutEvt);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "DwCoreInit failed\n"));
    goto Exit;
  }

  Status = DwHcInit (DwHc, TimeoutEvt);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "DwHcInit failed\n"));
    goto Exit;
  }

  MmioAndThenOr32 (DwHc->DwUsbBase + HPRT0,
    ~(DWC2_HPRT0_PRTENA | DWC2_HPRT0_PRTCONNDET |
      DWC2_HPRT0_PRTENCHNG | DWC2_HPRT0_PRTOVRCURRCHNG),
    DWC2_HPRT0_PRTRST);

  MicroSecondDelay (50000);

  MmioAnd32 (DwHc->DwUsbBase + HPRT0, ~(DWC2_HPRT0_PRTENA | DWC2_HPRT0_PRTCONNDET |
    DWC2_HPRT0_PRTENCHNG | DWC2_HPRT0_PRTOVRCURRCHNG |
    DWC2_HPRT0_PRTRST));

Exit:
  if (TimeoutEvt != NULL) {
    gBS->CloseEvent (TimeoutEvt);
  }

  return Status;
}

EFI_STATUS
EFIAPI
DwHcGetState (
  IN  EFI_USB2_HC_PROTOCOL *This,
  OUT EFI_USB_HC_STATE     *State
  )
{
  DWUSB_OTGHC_DEV *DwHc;

  DwHc = DWHC_FROM_THIS (This);

  if (State == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *State = DwHc->DwHcState;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DwHcSetState (
  IN  EFI_USB2_HC_PROTOCOL *This,
  IN  EFI_USB_HC_STATE     State
  )
{
  DWUSB_OTGHC_DEV   *DwHc;
  EFI_STATUS        Status;

  DwHc = DWHC_FROM_THIS (This);

  switch (State) {
  case EfiUsbHcStateHalt:
  case EfiUsbHcStateOperational:
  case EfiUsbHcStateSuspend:
    DwHc->DwHcState = State;
    Status = EFI_SUCCESS;
    break;

  default:
    Status = EFI_INVALID_PARAMETER;
    break;
  }

  return Status;
}

EFI_STATUS
EFIAPI
DwHcGetRootHubPortStatus (
  IN  EFI_USB2_HC_PROTOCOL *This,
  IN  UINT8                PortNumber,
  OUT EFI_USB_PORT_STATUS  *PortStatus
  )
{
  DWUSB_OTGHC_DEV *DwHc;
  UINT32          Hprt0;

  if (PortNumber > DWC2_HC_PORT) {
    return EFI_INVALID_PARAMETER;
  }

  if (PortStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DwHc = DWHC_FROM_THIS (This);

  PortStatus->PortStatus = 0;
  PortStatus->PortChangeStatus = 0;
  Hprt0 = MmioRead32 (DwHc->DwUsbBase + HPRT0);

  if (Hprt0 & DWC2_HPRT0_PRTCONNSTS) {
    PortStatus->PortStatus |= USB_PORT_STAT_CONNECTION;
  }

  if (Hprt0 & DWC2_HPRT0_PRTENA) {
    PortStatus->PortStatus |= USB_PORT_STAT_ENABLE;
  }

  if (Hprt0 & DWC2_HPRT0_PRTSUSP) {
    PortStatus->PortStatus |= USB_PORT_STAT_SUSPEND;
  }

  if (Hprt0 & DWC2_HPRT0_PRTOVRCURRACT) {
    PortStatus->PortStatus |= USB_PORT_STAT_OVERCURRENT;
  }

  if (Hprt0 & DWC2_HPRT0_PRTRST) {
    PortStatus->PortStatus |= USB_PORT_STAT_RESET;
  }

  if (Hprt0 & DWC2_HPRT0_PRTPWR) {
    PortStatus->PortStatus |= USB_PORT_STAT_POWER;
  }

  PortStatus->PortStatus |= USB_PORT_STAT_HIGH_SPEED;

  if (Hprt0 & DWC2_HPRT0_PRTENCHNG) {
    //              PortStatus->PortChangeStatus |= USB_PORT_STAT_C_ENABLE;
  }

  if (Hprt0 & DWC2_HPRT0_PRTCONNDET) {
    PortStatus->PortChangeStatus |= USB_PORT_STAT_C_CONNECTION;
  }

  if (Hprt0 & DWC2_HPRT0_PRTOVRCURRCHNG) {
    PortStatus->PortChangeStatus |= USB_PORT_STAT_C_OVERCURRENT;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DwHcSetRootHubPortFeature (
  IN  EFI_USB2_HC_PROTOCOL *This,
  IN  UINT8                PortNumber,
  IN  EFI_USB_PORT_FEATURE PortFeature
  )
{
  DWUSB_OTGHC_DEV         *DwHc;
  UINT32                  Hprt0;
  EFI_STATUS              Status = EFI_SUCCESS;

  if (PortNumber > DWC2_HC_PORT) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DwHc = DWHC_FROM_THIS (This);

  switch (PortFeature) {
  case EfiUsbPortEnable:
    break;
  case EfiUsbPortSuspend:
    Hprt0 = MmioRead32 (DwHc->DwUsbBase + HPRT0);
    Hprt0 &= ~(DWC2_HPRT0_PRTENA | DWC2_HPRT0_PRTCONNDET |
               DWC2_HPRT0_PRTENCHNG | DWC2_HPRT0_PRTOVRCURRCHNG);
    Hprt0 |= DWC2_HPRT0_PRTSUSP;
    MmioWrite32 (DwHc->DwUsbBase + HPRT0, Hprt0);
    break;
  case EfiUsbPortReset:
    MmioAndThenOr32 (DwHc->DwUsbBase + HPRT0,
      ~(DWC2_HPRT0_PRTENA | DWC2_HPRT0_PRTCONNDET |
        DWC2_HPRT0_PRTENCHNG | DWC2_HPRT0_PRTOVRCURRCHNG),
      DWC2_HPRT0_PRTRST);
    MicroSecondDelay (50000);
    MmioAnd32 (DwHc->DwUsbBase + HPRT0, ~DWC2_HPRT0_PRTRST);
    break;
  case EfiUsbPortPower:
    Hprt0 = MmioRead32 (DwHc->DwUsbBase + HPRT0);
    Hprt0 &= ~(DWC2_HPRT0_PRTENA | DWC2_HPRT0_PRTCONNDET |
               DWC2_HPRT0_PRTENCHNG | DWC2_HPRT0_PRTOVRCURRCHNG);
    Hprt0 |= DWC2_HPRT0_PRTPWR;
    MmioWrite32 (DwHc->DwUsbBase + HPRT0, Hprt0);
    break;
  case EfiUsbPortOwner:
    break;
  default:
    Status = EFI_INVALID_PARAMETER;
    break;
  }

Exit:
  return Status;
}

EFI_STATUS
EFIAPI
DwHcClearRootHubPortFeature (
  IN  EFI_USB2_HC_PROTOCOL *This,
  IN  UINT8                PortNumber,
  IN  EFI_USB_PORT_FEATURE PortFeature
  )
{
  DWUSB_OTGHC_DEV         *DwHc;
  UINT32                  Hprt0;
  EFI_STATUS              Status = EFI_SUCCESS;

  if (PortNumber > DWC2_HC_PORT) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  DwHc = DWHC_FROM_THIS (This);

  switch (PortFeature) {
  case EfiUsbPortEnable:
    Hprt0 = MmioRead32 (DwHc->DwUsbBase + HPRT0);
    Hprt0 &= ~(DWC2_HPRT0_PRTENA | DWC2_HPRT0_PRTCONNDET |
               DWC2_HPRT0_PRTENCHNG | DWC2_HPRT0_PRTOVRCURRCHNG);
    Hprt0 |= DWC2_HPRT0_PRTENA;
    MmioWrite32 (DwHc->DwUsbBase + HPRT0, Hprt0);
    break;
  case EfiUsbPortReset:
    MmioAndThenOr32 (DwHc->DwUsbBase + HPRT0,
      ~(DWC2_HPRT0_PRTENA | DWC2_HPRT0_PRTCONNDET |
        DWC2_HPRT0_PRTENCHNG | DWC2_HPRT0_PRTOVRCURRCHNG),
      DWC2_HPRT0_PRTRST);
    MicroSecondDelay (50000);
    MmioAnd32 (DwHc->DwUsbBase + HPRT0, ~DWC2_HPRT0_PRTRST);
    break;
  case EfiUsbPortSuspend:
    MmioWrite32 (DwHc->DwUsbBase + PCGCCTL, 0);
    MicroSecondDelay (40000);
    Hprt0 = MmioRead32 (DwHc->DwUsbBase + HPRT0);
    Hprt0 &= ~(DWC2_HPRT0_PRTENA | DWC2_HPRT0_PRTCONNDET |
               DWC2_HPRT0_PRTENCHNG | DWC2_HPRT0_PRTOVRCURRCHNG);
    Hprt0 |= DWC2_HPRT0_PRTRES;
    MmioWrite32 (DwHc->DwUsbBase + HPRT0, Hprt0);
    Hprt0 &= ~DWC2_HPRT0_PRTSUSP;
    MicroSecondDelay (150000);
    Hprt0 &= ~DWC2_HPRT0_PRTRES;
    MmioWrite32 (DwHc->DwUsbBase + HPRT0, Hprt0);
    break;
  case EfiUsbPortPower:
    Hprt0 = MmioRead32 (DwHc->DwUsbBase + HPRT0);
    Hprt0 &= ~(DWC2_HPRT0_PRTENA | DWC2_HPRT0_PRTCONNDET |
               DWC2_HPRT0_PRTENCHNG | DWC2_HPRT0_PRTOVRCURRCHNG);
    Hprt0 &= ~DWC2_HPRT0_PRTPWR;
    MmioWrite32 (DwHc->DwUsbBase + HPRT0, Hprt0);
    break;
  case EfiUsbPortOwner:
    break;
  case EfiUsbPortConnectChange:
    Hprt0 = MmioRead32 (DwHc->DwUsbBase + HPRT0);
    Hprt0 &= ~DWC2_HPRT0_PRTCONNDET;
    MmioWrite32 (DwHc->DwUsbBase + HPRT0, Hprt0);
    break;
  case EfiUsbPortResetChange:
    break;
  case EfiUsbPortEnableChange:
    Hprt0 = MmioRead32 (DwHc->DwUsbBase + HPRT0);
    Hprt0 &= ~DWC2_HPRT0_PRTENCHNG;
    MmioWrite32 (DwHc->DwUsbBase + HPRT0, Hprt0);
    break;
  case EfiUsbPortSuspendChange:
    break;
  case EfiUsbPortOverCurrentChange:
    Hprt0 = MmioRead32 (DwHc->DwUsbBase + HPRT0);
    Hprt0 &= ~DWC2_HPRT0_PRTOVRCURRCHNG;
    MmioWrite32 (DwHc->DwUsbBase + HPRT0, Hprt0);
    break;
  default:
    Status = EFI_INVALID_PARAMETER;
    break;
  }

Exit:
  return Status;
}

EFI_STATUS
EFIAPI
DwHcControlTransfer (
  IN  EFI_USB2_HC_PROTOCOL               *This,
  IN  UINT8                              DeviceAddress,
  IN  UINT8                              DeviceSpeed,
  IN  UINTN                              MaximumPacketLength,
  IN  EFI_USB_DEVICE_REQUEST             *Request,
  IN  EFI_USB_DATA_DIRECTION             TransferDirection,
  IN  OUT VOID                           *Data,
  IN  OUT UINTN                          *DataLength,
  IN  UINTN                              TimeOut,
  IN  EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
  OUT UINT32                             *TransferResult
  )
{
  DWUSB_OTGHC_DEV         *DwHc;
  EFI_STATUS              Status;
  UINT32                  Pid;
  UINTN                   Length;
  EFI_USB_DATA_DIRECTION  StatusDirection;
  UINT32                  Direction;
  EFI_EVENT TimeoutEvt = NULL;

  if ((Request == NULL) || (TransferResult == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((TransferDirection != EfiUsbDataIn) &&
      (TransferDirection != EfiUsbDataOut) &&
      (TransferDirection != EfiUsbNoData)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((TransferDirection == EfiUsbNoData) &&
      ((Data != NULL) || (*DataLength != 0))) {
    return EFI_INVALID_PARAMETER;
  }

  if ((TransferDirection != EfiUsbNoData) &&
      ((Data == NULL) || (*DataLength == 0))) {
    return EFI_INVALID_PARAMETER;
  }

  if ((MaximumPacketLength != 8) && (MaximumPacketLength != 16) &&
      (MaximumPacketLength != 32) && (MaximumPacketLength != 64)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((DeviceSpeed == EFI_USB_SPEED_LOW) && (MaximumPacketLength != 8)) {
    return EFI_INVALID_PARAMETER;
  }

  DwHc = DWHC_FROM_THIS (This);

  Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimeoutEvt);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Status = gBS->SetTimer (TimeoutEvt, TimerForTransfer,
                  EFI_TIMER_PERIOD_MILLISECONDS (TimeOut));
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Pid = DWC2_HC_PID_SETUP;
  Length = 8;
  Status = DwHcTransfer (DwHc, TimeoutEvt,
             DWC2_HC_CHANNEL, Translator, DeviceSpeed,
             DeviceAddress, MaximumPacketLength, &Pid, 0,
             Request, &Length, 0, DWC2_HCCHAR_EPTYPE_CONTROL,
             TransferResult, 1);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Setup Stage Error for device 0x%x: 0x%x\n",
      DeviceAddress, *TransferResult));
    goto Exit;
  }

  if (Data) {
    Pid = DWC2_HC_PID_DATA1;

    if (TransferDirection == EfiUsbDataIn) {
      Direction = 1;
    } else {
      Direction = 0;
    }

    Status = DwHcTransfer (DwHc, TimeoutEvt,
               DWC2_HC_CHANNEL, Translator, DeviceSpeed,
               DeviceAddress, MaximumPacketLength, &Pid,
               Direction, Data, DataLength, 0,
               DWC2_HCCHAR_EPTYPE_CONTROL,
               TransferResult, 0);

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Data Stage Error for device 0x%x: 0x%x\n",
        DeviceAddress, *TransferResult));
      goto Exit;
    }
  }

  if ((TransferDirection == EfiUsbDataOut) ||
      (TransferDirection == EfiUsbNoData)) {
    StatusDirection = 1;
  } else {
    StatusDirection = 0;
  }

  Pid = DWC2_HC_PID_DATA1;
  Length = 0;
  Status = DwHcTransfer (DwHc, TimeoutEvt,
             DWC2_HC_CHANNEL, Translator, DeviceSpeed,
             DeviceAddress, MaximumPacketLength, &Pid,
             StatusDirection, DwHc->StatusBuffer, &Length, 0,
             DWC2_HCCHAR_EPTYPE_CONTROL, TransferResult, 1);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Status Stage Error for 0x%x: 0x%x\n",
      DeviceAddress, *TransferResult));
    goto Exit;
  }

Exit:
  if (TimeoutEvt != NULL) {
    gBS->CloseEvent (TimeoutEvt);
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RequestType 0x%x\n", Request->RequestType));
    DEBUG ((DEBUG_ERROR, "Request 0x%x\n", Request->Request));
    DEBUG ((DEBUG_ERROR, "Value 0x%x\n", Request->Value));
    DEBUG ((DEBUG_ERROR, "Index 0x%x\n", Request->Index));
    DEBUG ((DEBUG_ERROR, "Length 0x%x\n", Request->Length));
  }
  return Status;
}

EFI_STATUS
EFIAPI
DwHcBulkTransfer (
  IN  EFI_USB2_HC_PROTOCOL               *This,
  IN  UINT8                              DeviceAddress,
  IN  UINT8                              EndPointAddress,
  IN  UINT8                              DeviceSpeed,
  IN  UINTN                              MaximumPacketLength,
  IN  UINT8                              DataBuffersNumber,
  IN  OUT VOID                           *Data[EFI_USB_MAX_BULK_BUFFER_NUM],
  IN  OUT UINTN                          *DataLength,
  IN  OUT UINT8                          *DataToggle,
  IN  UINTN                              TimeOut,
  IN  EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
  OUT UINT32                             *TransferResult
  )
{
  DWUSB_OTGHC_DEV         *DwHc;
  EFI_STATUS              Status;
  UINT8                   TransferDirection;
  UINT8                   EpAddress;
  UINT32                  Pid;
  EFI_EVENT TimeoutEvt = NULL;

  if ((Data == NULL) || (Data[0] == NULL) ||
      (DataLength == NULL) || (*DataLength == 0) ||
      (TransferResult == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((*DataToggle != 0) && (*DataToggle != 1)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((DeviceSpeed == EFI_USB_SPEED_LOW) ||
      (DeviceSpeed == EFI_USB_SPEED_SUPER)) {
    return EFI_INVALID_PARAMETER;
  }

  if (((DeviceSpeed == EFI_USB_SPEED_FULL) && (MaximumPacketLength > 64)) ||
      ((DeviceSpeed == EFI_USB_SPEED_HIGH) && (MaximumPacketLength > 512)))
    return EFI_INVALID_PARAMETER;

  DwHc = DWHC_FROM_THIS (This);

  Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimeoutEvt);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Status = gBS->SetTimer (TimeoutEvt, TimerForTransfer,
                  EFI_TIMER_PERIOD_MILLISECONDS (TimeOut));
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Status = EFI_DEVICE_ERROR;
  TransferDirection = (EndPointAddress >> 7) & 0x01;
  EpAddress = EndPointAddress & 0x0F;
  Pid = (*DataToggle << 1);

  Status = DwHcTransfer (DwHc, TimeoutEvt,
             DWC2_HC_CHANNEL_BULK, Translator, DeviceSpeed,
             DeviceAddress, MaximumPacketLength, &Pid,
             TransferDirection, Data[0], DataLength, EpAddress,
             DWC2_HCCHAR_EPTYPE_BULK, TransferResult, 1);

  *DataToggle = (Pid >> 1);

Exit:
  if (TimeoutEvt != NULL) {
    gBS->CloseEvent (TimeoutEvt);
  }

  return Status;
}

EFI_STATUS
EFIAPI
DwHcAsyncInterruptTransfer (
  IN  EFI_USB2_HC_PROTOCOL               *This,
  IN  UINT8                              DeviceAddress,
  IN  UINT8                              EndPointAddress,
  IN  UINT8                              DeviceSpeed,
  IN  UINTN                              MaximumPacketLength,
  IN  BOOLEAN                            IsNewTransfer,
  IN  OUT UINT8                          *DataToggle,
  IN  UINTN                              PollingInterval,
  IN  UINTN                              DataLength,
  IN  EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
  IN  EFI_ASYNC_USB_TRANSFER_CALLBACK    CallbackFunction,
  IN  VOID                               *Context OPTIONAL
  )
{
  DWUSB_OTGHC_DEV                 *DwHc;
  EFI_STATUS                      Status;
  EFI_TPL                         PreviousTpl;
  VOID                            *Data = NULL;
  DWUSB_DEFERRED_REQ              *FoundReq = NULL;
  DWUSB_DEFERRED_REQ              *NewReq = NULL;

  if (!(EndPointAddress & USB_ENDPOINT_DIR_IN)) {
    return EFI_INVALID_PARAMETER;
  }

  DwHc = DWHC_FROM_THIS (This);

  PreviousTpl = gBS->RaiseTPL (TPL_NOTIFY);
  FoundReq = DwHcFindDeferredTransfer (DwHc, DeviceAddress, EndPointAddress);

  if (IsNewTransfer) {
    if (FoundReq != NULL) {
      Status = EFI_INVALID_PARAMETER;
      goto Done;
    }

    if (DataLength == 0) {
      Status = EFI_INVALID_PARAMETER;
      goto Done;
    }

    if ((*DataToggle != 1) && (*DataToggle != 0)) {
      Status = EFI_INVALID_PARAMETER;
      goto Done;
    }

    if ((PollingInterval > 255) || (PollingInterval < 1)) {
      Status = EFI_INVALID_PARAMETER;
      goto Done;
    }

    if (CallbackFunction == NULL) {
      Status = EFI_INVALID_PARAMETER;
      goto Done;
    }
  }

  if (!IsNewTransfer) {
    if (FoundReq == NULL) {
      DEBUG ((DEBUG_ERROR, "%u:%u> transfer not found\n", DeviceAddress, EndPointAddress & 0xF));
      Status = EFI_INVALID_PARAMETER;
      goto Done;
    }

    *DataToggle = FoundReq->Pid >> 1;
    FreePool (FoundReq->Data);

    RemoveEntryList (&FoundReq->List);
    FreePool (FoundReq);

    Status = EFI_SUCCESS;
    goto Done;
  }

  NewReq = AllocateZeroPool (sizeof *NewReq);
  if (NewReq == NULL) {
    DEBUG ((DEBUG_ERROR, "DwHcAsyncInterruptTransfer: failed to allocate req"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

  Data = AllocateZeroPool (DataLength);
  if (Data == NULL) {
    DEBUG ((DEBUG_ERROR, "DwHcAsyncInterruptTransfer: failed to allocate buffer\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

  InitializeListHead (&NewReq->List);

  NewReq->FrameInterval = PollingInterval;
  NewReq->TargetFrame = DwHc->CurrentFrame +
    NewReq->FrameInterval;

  NewReq->DwHc = DwHc;
  NewReq->Channel = DWC2_HC_CHANNEL_ASYNC;
  NewReq->Translator = Translator;
  NewReq->DeviceSpeed = DeviceSpeed;
  NewReq->DeviceAddress = DeviceAddress;
  NewReq->MaximumPacketLength = MaximumPacketLength;
  NewReq->TransferDirection = (EndPointAddress >> 7) & 0x01;
  NewReq->Data = Data;
  NewReq->DataLength = DataLength;
  NewReq->Pid = *DataToggle << 1;
  NewReq->EpAddress = EndPointAddress & 0x0F;
  NewReq->EpType = DWC2_HCCHAR_EPTYPE_INTR;
  NewReq->IgnoreAck = FALSE;
  NewReq->CallbackFunction = CallbackFunction;
  NewReq->CallbackContext = Context;
  NewReq->TimeOut = 1000; /* 1000 ms */

  InsertTailList (&DwHc->DeferredList, &NewReq->List);
  Status = EFI_SUCCESS;

Done:
  gBS->RestoreTPL (PreviousTpl);

  if (Status != EFI_SUCCESS) {
    if (Data != NULL) {
      FreePool (Data);
    }

    if (NewReq != NULL) {
      FreePool (NewReq);
    }
  }

  return Status;
}

EFI_STATUS
EFIAPI
DwHcSyncInterruptTransfer (
  IN  EFI_USB2_HC_PROTOCOL               *This,
  IN  UINT8                              DeviceAddress,
  IN  UINT8                              EndPointAddress,
  IN  UINT8                              DeviceSpeed,
  IN  UINTN                              MaximumPacketLength,
  IN  OUT VOID                           *Data,
  IN  OUT UINTN                          *DataLength,
  IN  OUT UINT8                          *DataToggle,
  IN  UINTN                              TimeOut,
  IN  EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
  OUT UINT32                             *TransferResult
  )
{
  DWUSB_OTGHC_DEV *DwHc;
  EFI_STATUS Status;
  EFI_EVENT TimeoutEvt;
  UINT8 TransferDirection;
  UINT8 EpAddress;
  UINT32 Pid;

  DwHc = DWHC_FROM_THIS (This);

  if (Data == NULL ||
      DataLength == NULL ||
      DataToggle == NULL ||
      TransferResult == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (*DataLength == 0) {
    return EFI_INVALID_PARAMETER;
  }

  if ((*DataToggle != 0) && (*DataToggle != 1)) {
    return EFI_INVALID_PARAMETER;
  }

  if (((DeviceSpeed == EFI_USB_SPEED_LOW) && (MaximumPacketLength != 8))  ||
      ((DeviceSpeed == EFI_USB_SPEED_FULL) && (MaximumPacketLength > 64)) ||
      ((DeviceSpeed == EFI_USB_SPEED_HIGH) && (MaximumPacketLength > 3072))) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->CreateEvent (EVT_TIMER, 0, NULL, NULL, &TimeoutEvt);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->SetTimer (TimeoutEvt, TimerForTransfer,
                  EFI_TIMER_PERIOD_MILLISECONDS (TimeOut));
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  TransferDirection = (EndPointAddress >> 7) & 0x01;
  EpAddress = EndPointAddress & 0x0F;
  Pid = (*DataToggle << 1);
  Status = DwHcTransfer (DwHc, TimeoutEvt,
             DWC2_HC_CHANNEL_SYNC, Translator,
             DeviceSpeed, DeviceAddress,
             MaximumPacketLength,
             &Pid, TransferDirection, Data,
             DataLength, EpAddress,
             DWC2_HCCHAR_EPTYPE_INTR,
             TransferResult, 0);
  *DataToggle = (Pid >> 1);

Exit:
  if (TimeoutEvt != NULL) {
    gBS->CloseEvent (TimeoutEvt);
  }
  return Status;
}

EFI_STATUS
EFIAPI
DwHcIsochronousTransfer (
  IN  EFI_USB2_HC_PROTOCOL               *This,
  IN  UINT8                              DeviceAddress,
  IN  UINT8                              EndPointAddress,
  IN  UINT8                              DeviceSpeed,
  IN  UINTN                              MaximumPacketLength,
  IN  UINT8                              DataBuffersNumber,
  IN  OUT VOID                           *Data[EFI_USB_MAX_ISO_BUFFER_NUM],
  IN  UINTN                              DataLength,
  IN  EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
  OUT UINT32                             *TransferResult
  )
{
  DEBUG ((DEBUG_ERROR, "Iso\n"));
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
DwHcAsyncIsochronousTransfer (
  IN  EFI_USB2_HC_PROTOCOL               *This,
  IN  UINT8                              DeviceAddress,
  IN  UINT8                              EndPointAddress,
  IN  UINT8                              DeviceSpeed,
  IN  UINTN                              MaximumPacketLength,
  IN  UINT8                              DataBuffersNumber,
  IN  OUT VOID                           *Data[EFI_USB_MAX_ISO_BUFFER_NUM],
  IN  UINTN                              DataLength,
  IN  EFI_USB2_HC_TRANSACTION_TRANSLATOR *Translator,
  IN  EFI_ASYNC_USB_TRANSFER_CALLBACK    IsochronousCallBack,
  IN  VOID                               *Context
  )
{
  DEBUG ((DEBUG_ERROR, "AsyncIso\n"));
  return EFI_UNSUPPORTED;
}

/**
   Supported Functions
**/

VOID
InitFslspClkSel (
  IN  DWUSB_OTGHC_DEV *DwHc
  )
{
  UINT32  PhyClk;

  PhyClk = DWC2_HCFG_FSLSPCLKSEL_30_60_MHZ;

  MmioAndThenOr32 (DwHc->DwUsbBase + HCFG,
    ~DWC2_HCFG_FSLSPCLKSEL_MASK,
    PhyClk << DWC2_HCFG_FSLSPCLKSEL_OFFSET);
}

VOID
DwFlushTxFifo (
  IN  DWUSB_OTGHC_DEV *DwHc,
  IN EFI_EVENT Timeout,
  IN INT32 Num
  )
{
  EFI_STATUS Status;

  MmioWrite32 (DwHc->DwUsbBase + GRSTCTL, DWC2_GRSTCTL_TXFFLSH |
    (Num << DWC2_GRSTCTL_TXFNUM_OFFSET));

  Status = Wait4Bit (Timeout, DwHc->DwUsbBase + GRSTCTL, DWC2_GRSTCTL_TXFFLSH, 0);
  if (Status)
    DEBUG ((DEBUG_ERROR, "DwFlushTxFifo: Timeout!\n"));

  MicroSecondDelay (1);
}

VOID
DwFlushRxFifo (
  IN  DWUSB_OTGHC_DEV *DwHc,
  IN  EFI_EVENT Timeout
  )
{
  EFI_STATUS Status;

  MmioWrite32 (DwHc->DwUsbBase + GRSTCTL, DWC2_GRSTCTL_RXFFLSH);

  Status = Wait4Bit (Timeout, DwHc->DwUsbBase + GRSTCTL, DWC2_GRSTCTL_RXFFLSH, 0);
  if (Status)
    DEBUG ((DEBUG_ERROR, "DwFlushRxFifo: Timeout!\n"));

  MicroSecondDelay (1);
}

EFI_STATUS
DwHcInit (
  IN  DWUSB_OTGHC_DEV *DwHc,
  IN  EFI_EVENT Timeout
  )
{
  UINT32 NpTxFifoSz = 0;
  UINT32 pTxFifoSz = 0;
  UINT32 Hprt0 = 0;
  INT32  i, Status, NumChannels;

  MmioWrite32 (DwHc->DwUsbBase + PCGCCTL, 0);

  InitFslspClkSel (DwHc);

  MmioWrite32 (DwHc->DwUsbBase + GRXFSIZ, DWC2_HOST_RX_FIFO_SIZE);

  NpTxFifoSz |= DWC2_HOST_NPERIO_TX_FIFO_SIZE << DWC2_FIFOSIZE_DEPTH_OFFSET;
  NpTxFifoSz |= DWC2_HOST_RX_FIFO_SIZE << DWC2_FIFOSIZE_STARTADDR_OFFSET;
  MmioWrite32 (DwHc->DwUsbBase + GNPTXFSIZ, NpTxFifoSz);

  pTxFifoSz |= DWC2_HOST_PERIO_TX_FIFO_SIZE << DWC2_FIFOSIZE_DEPTH_OFFSET;
  pTxFifoSz |= (DWC2_HOST_RX_FIFO_SIZE + DWC2_HOST_NPERIO_TX_FIFO_SIZE) <<
    DWC2_FIFOSIZE_STARTADDR_OFFSET;
  MmioWrite32 (DwHc->DwUsbBase + HPTXFSIZ, pTxFifoSz);

  MmioAnd32 (DwHc->DwUsbBase + GOTGCTL, ~(DWC2_GOTGCTL_HSTSETHNPEN));

  DwFlushTxFifo (DwHc, Timeout, 0x10);
  DwFlushRxFifo (DwHc, Timeout);

  NumChannels = MmioRead32 (DwHc->DwUsbBase + GHWCFG2);
  NumChannels &= DWC2_HWCFG2_NUM_HOST_CHAN_MASK;
  NumChannels >>= DWC2_HWCFG2_NUM_HOST_CHAN_OFFSET;
  NumChannels += 1;
  DEBUG ((DEBUG_INFO, "Host has %u channels\n", NumChannels));

  for (i = 0; i < NumChannels; i++)
    MmioAndThenOr32 (DwHc->DwUsbBase + HCCHAR (i),
      ~(DWC2_HCCHAR_CHEN | DWC2_HCCHAR_EPDIR),
      DWC2_HCCHAR_CHDIS);

  for (i = 0; i < NumChannels; i++) {
    MmioAndThenOr32 (DwHc->DwUsbBase + HCCHAR (i),
      ~DWC2_HCCHAR_EPDIR,
      (DWC2_HCCHAR_CHEN | DWC2_HCCHAR_CHDIS));
    Status = Wait4Bit (Timeout, DwHc->DwUsbBase + HCCHAR (i), DWC2_HCCHAR_CHEN, 0);
    if (Status) {
      DEBUG ((DEBUG_ERROR, "DwHcInit: Timeout!\n"));
      return Status;
    }
  }

  if (MmioRead32 (DwHc->DwUsbBase + GINTSTS) & DWC2_GINTSTS_CURMODE_HOST) {
    Hprt0 = MmioRead32 (DwHc->DwUsbBase + HPRT0);
    Hprt0 &= ~(DWC2_HPRT0_PRTENA | DWC2_HPRT0_PRTCONNDET);
    Hprt0 &= ~(DWC2_HPRT0_PRTENCHNG | DWC2_HPRT0_PRTOVRCURRCHNG);

    if (!(Hprt0 & DWC2_HPRT0_PRTPWR)) {
      Hprt0 |= DWC2_HPRT0_PRTPWR;
      MmioWrite32 (DwHc->DwUsbBase + HPRT0, Hprt0);
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
DwCoreInit (
  IN  DWUSB_OTGHC_DEV *DwHc,
  IN  EFI_EVENT Timeout
  )
{
  UINT32 AhbCfg = 0;
  UINT32 UsbCfg = 0;
  EFI_STATUS Status;

  UsbCfg = MmioRead32 (DwHc->DwUsbBase + GUSBCFG);

  UsbCfg |= DWC2_GUSBCFG_ULPI_EXT_VBUS_DRV;
  UsbCfg &= ~DWC2_GUSBCFG_TERM_SEL_DL_PULSE;

  MmioWrite32 (DwHc->DwUsbBase + GUSBCFG, UsbCfg);

  Status = DwCoreReset (DwHc, Timeout);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "DwCoreReset failed\n"));
    return Status;
  }

  UsbCfg &= ~(DWC2_GUSBCFG_ULPI_UTMI_SEL | DWC2_GUSBCFG_PHYIF);
  UsbCfg |= CONFIG_DWC2_PHY_TYPE << DWC2_GUSBCFG_ULPI_UTMI_SEL_OFFSET;
  UsbCfg &= ~DWC2_GUSBCFG_DDRSEL;

  MmioWrite32 (DwHc->DwUsbBase + GUSBCFG, UsbCfg);

  Status = DwCoreReset (DwHc, Timeout);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "DwCoreReset 2 failed\n"));
    return Status;
  }

  UsbCfg = MmioRead32 (DwHc->DwUsbBase + GUSBCFG);

  UsbCfg &= ~(DWC2_GUSBCFG_ULPI_FSLS | DWC2_GUSBCFG_ULPI_CLK_SUS_M);
  MmioWrite32 (DwHc->DwUsbBase + GUSBCFG, UsbCfg);

  AhbCfg &= ~DWC2_GAHBCFG_AXI_BURST4_MASK;
  AhbCfg |= DWC2_GAHBCFG_DMAENABLE | DWC2_GAHBCFG_WAIT_AXI_WRITES;

  MmioWrite32 (DwHc->DwUsbBase + GAHBCFG, AhbCfg);
  MmioAnd32 (DwHc->DwUsbBase + GUSBCFG, ~(DWC2_GUSBCFG_HNPCAP | DWC2_GUSBCFG_SRPCAP));

  return EFI_SUCCESS;
}

VOID
DestroyDwUsbHc (
  IN  DWUSB_OTGHC_DEV *DwHc
  )
{
  UINT32 Pages;
  EFI_TPL PreviousTpl;

  if (DwHc == NULL) {
    return;
  }

  if (DwHc->PeriodicEvent != NULL) {
    PreviousTpl = gBS->RaiseTPL (TPL_NOTIFY);
    gBS->CloseEvent (DwHc->PeriodicEvent);
    gBS->RestoreTPL (PreviousTpl);
  }

  if (DwHc->ExitBootServiceEvent != NULL) {
    gBS->CloseEvent (DwHc->ExitBootServiceEvent);
  }

  Pages = EFI_SIZE_TO_PAGES (DWC2_DATA_BUF_SIZE);
  DmaUnmap (DwHc->AlignedBufferMapping);
  DmaFreeBuffer (Pages, DwHc->AlignedBuffer);

  Pages = EFI_SIZE_TO_PAGES (DWC2_STATUS_BUF_SIZE);
  FreePages (DwHc->StatusBuffer, Pages);

  gBS->FreePool (DwHc);
}

STATIC
VOID
EFIAPI
DwUsbHcExitBootService (
  IN  EFI_EVENT Event,
  IN  VOID *Context
  )
{
  DWUSB_OTGHC_DEV *DwHc;

  DwHc = (DWUSB_OTGHC_DEV*)Context;
  DwHcQuiesce (DwHc);
}

STATIC
UINT32
FramesPassed (
  IN  DWUSB_OTGHC_DEV *DwHc
  )
{
  UINT32 MicroFrameStart = DwHc->LastMicroFrame;
  UINT32 MicroFrameEnd =
    MmioRead32 (DwHc->DwUsbBase + HFNUM) &
    DWC2_HFNUM_FRNUM_MASK;
  UINT32 MicroFramesPassed;

  DwHc->LastMicroFrame = (UINT16)MicroFrameEnd;

  if (MicroFrameEnd < MicroFrameStart) {
    /*
     * Being delayed by 0x8000 microframes is 262 seconds.
     * Unlikely. Also, we can't really do better unless we
     * start polling time (which is tedious in EFI...).
     */
    MicroFrameEnd += DWC2_HFNUM_FRNUM_MASK + 1;
  }

  MicroFramesPassed = MicroFrameEnd - MicroFrameStart;

  /*
   * Round up. We're supposedly getting called every
   * 8 microframes anyway. This means we'll end up
   * going a bit faster, which is okay.
   */
  return ALIGN_VALUE (MicroFramesPassed, 8) / 8;
}

STATIC
VOID
DwHcPeriodicHandler (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  UINT32 Frame;
  LIST_ENTRY *Entry;
  LIST_ENTRY *NextEntry;
  DWUSB_OTGHC_DEV *DwHc = Context;

  DwHc->CurrentFrame += FramesPassed (DwHc);
  Frame = DwHc->CurrentFrame;

  EFI_LIST_FOR_EACH_SAFE (Entry, NextEntry,
    &DwHc->DeferredList) {
    DWUSB_DEFERRED_REQ *Req = EFI_LIST_CONTAINER (Entry, DWUSB_DEFERRED_REQ, List);

    if (Frame >= Req->TargetFrame) {
      Req->TargetFrame = Frame + Req->FrameInterval;
      DwHcDeferredTransfer (Req);
    }
  }
}

EFI_STATUS
CreateDwUsbHc (
  OUT DWUSB_OTGHC_DEV **OutDwHc
  )
{
  DWUSB_OTGHC_DEV *DwHc;
  UINT32          Pages;
  UINTN           BufferSize;
  EFI_STATUS      Status;

  DwHc = AllocateZeroPool (sizeof (DWUSB_OTGHC_DEV));
  if (DwHc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  DwHc->Signature                                 = DWUSB_OTGHC_DEV_SIGNATURE;
  DwHc->DwUsbOtgHc.GetCapability                  = DwHcGetCapability;
  DwHc->DwUsbOtgHc.Reset                          = DwHcReset;
  DwHc->DwUsbOtgHc.GetState                       = DwHcGetState;
  DwHc->DwUsbOtgHc.SetState                       = DwHcSetState;
  DwHc->DwUsbOtgHc.ControlTransfer                = DwHcControlTransfer;
  DwHc->DwUsbOtgHc.BulkTransfer                   = DwHcBulkTransfer;
  DwHc->DwUsbOtgHc.AsyncInterruptTransfer         = DwHcAsyncInterruptTransfer;
  DwHc->DwUsbOtgHc.SyncInterruptTransfer          = DwHcSyncInterruptTransfer;
  DwHc->DwUsbOtgHc.IsochronousTransfer            = DwHcIsochronousTransfer;
  DwHc->DwUsbOtgHc.AsyncIsochronousTransfer       = DwHcAsyncIsochronousTransfer;
  DwHc->DwUsbOtgHc.GetRootHubPortStatus           = DwHcGetRootHubPortStatus;
  DwHc->DwUsbOtgHc.SetRootHubPortFeature          = DwHcSetRootHubPortFeature;
  DwHc->DwUsbOtgHc.ClearRootHubPortFeature        = DwHcClearRootHubPortFeature;
  DwHc->DwUsbOtgHc.MajorRevision                  = 0x02;
  DwHc->DwUsbOtgHc.MinorRevision                  = 0x00;
  DwHc->DwUsbBase                                 = BCM2836_USB_BASE_ADDRESS;

  Pages = EFI_SIZE_TO_PAGES (DWC2_STATUS_BUF_SIZE);
  DwHc->StatusBuffer = AllocatePages (Pages);
  if (DwHc->StatusBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "CreateDwUsbHc: No pages available for StatusBuffer\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Pages = EFI_SIZE_TO_PAGES (DWC2_DATA_BUF_SIZE);
  Status = DmaAllocateBuffer (EfiBootServicesData, Pages, (VOID**)&DwHc->AlignedBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreateDwUsbHc: DmaAllocateBuffer: %r\n", Status));
    return Status;
  }

  BufferSize = EFI_PAGES_TO_SIZE (Pages);
  Status = DmaMap (MapOperationBusMasterCommonBuffer, DwHc->AlignedBuffer, &BufferSize,
             &DwHc->AlignedBufferBusAddress, &DwHc->AlignedBufferMapping);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreateDwUsbHc: DmaMap: %r\n", Status));
    return Status;
  }

  InitializeListHead (&DwHc->DeferredList);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  DwUsbHcExitBootService,
                  DwHc,
                  &gEfiEventExitBootServicesGuid,
                  &DwHc->ExitBootServiceEvent
                );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreateDwUsbHc: DwUsbHcExitBootService: %r\n", Status));
    return Status;
  }

  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  DwHcPeriodicHandler,
                  DwHc, &DwHc->PeriodicEvent
                );
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "CreateDwUsbHc: DwHcPeriodicHandler: %r\n", Status));
    return Status;
  }

  Status = gBS->SetTimer (DwHc->PeriodicEvent, TimerPeriodic,
                  EFI_TIMER_PERIOD_MILLISECONDS (1));
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "CreateDwUsbHc: PeriodicEvent: %r\n", Status));
    return Status;
  }

  *OutDwHc = DwHc;
  return EFI_SUCCESS;
}

VOID
DwHcQuiesce (
  IN  DWUSB_OTGHC_DEV *DwHc
  )
{
  if (DwHc->PeriodicEvent != NULL) {
    EFI_TPL PreviousTpl;
    PreviousTpl = gBS->RaiseTPL (TPL_NOTIFY);
    gBS->CloseEvent (DwHc->PeriodicEvent);
    DwHc->PeriodicEvent = NULL;
    gBS->RestoreTPL (PreviousTpl);
  }

  MmioAndThenOr32 (DwHc->DwUsbBase + HPRT0,
    ~(DWC2_HPRT0_PRTENA | DWC2_HPRT0_PRTCONNDET |
      DWC2_HPRT0_PRTENCHNG | DWC2_HPRT0_PRTOVRCURRCHNG),
    DWC2_HPRT0_PRTRST);

  MicroSecondDelay (50000);

  MmioWrite32 (DwHc->DwUsbBase + GRSTCTL, DWC2_GRSTCTL_CSFTRST);
  MicroSecondDelay (100000);
}
