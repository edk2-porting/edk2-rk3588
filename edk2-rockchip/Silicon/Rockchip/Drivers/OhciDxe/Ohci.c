/** @file
This file contains the implementation of Usb Hc Protocol.

Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>
Copyright (c) 2013-2016 Intel Corporation.

SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Ohci.h"

/**
  Retrieves the Host Controller capabilities.

  @param  This           A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  MaxSpeed       Host controller data transfer speed.
  @param  PortNumber     Number of the root hub ports.
  @param  Is64BitCapable TRUE if controller supports 64-bit memory addressing,
                         FALSE otherwise.

  @retval EFI_SUCCESS           The host controller capabilities were retrieved successfully.
  @retval EFI_INVALID_PARAMETER One of the input args was NULL.
  @retval EFI_DEVICE_ERROR      An error was encountered while attempting to
                                retrieve the capabilities.

**/
EFI_STATUS
EFIAPI
OhciGetCapability (
  IN  EFI_USB2_HC_PROTOCOL  *This,
  OUT UINT8                 *MaxSpeed,
  OUT UINT8                 *PortNumber,
  OUT UINT8                 *Is64BitCapable
  )
{
  USB_OHCI_HC_DEV  *Ohc;

  Ohc = USB_OHCI_HC_DEV_FROM_THIS (This);

  if ((MaxSpeed == NULL) && (PortNumber == NULL) && (Is64BitCapable == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (MaxSpeed != NULL) {
    *MaxSpeed = EFI_USB_SPEED_FULL;
  }

  if (PortNumber != NULL) {
    *PortNumber = (UINT8)OhciGetRootHubDescriptor (Ohc, RH_NUM_DS_PORTS);
  }

  if (Is64BitCapable != NULL) {
    *Is64BitCapable = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  Provides software reset for the USB host controller.

  @param  This       A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  Attributes A bit mask of the reset operation to perform.

  @retval EFI_SUCCESS           The reset operation succeeded.
  @retval EFI_INVALID_PARAMETER Attributes is not valid.
  @retval EFI_UNSUPPORTED       The type of reset specified by Attributes is not currently
                                supported by the host controller hardware.
  @retval EFI_ACCESS_DENIED     Reset operation is rejected due to the debug port being configured
                                and active; only EFI_USB_HC_RESET_GLOBAL_WITH_DEBUG or
                                EFI_USB_HC_RESET_HOST_WITH_DEBUG reset Attributes can be used to
                                perform reset operation for this host controller.
  @retval EFI_DEVICE_ERROR      An error was encountered while attempting to
                                retrieve the capabilities.

**/
EFI_STATUS
EFIAPI
OhciReset (
  IN EFI_USB2_HC_PROTOCOL  *This,
  IN UINT16                Attributes
  )
{
  EFI_STATUS       Status;
  USB_OHCI_HC_DEV  *Ohc;
  UINT8            Index;
  UINT8            NumOfPorts;
  UINT32           PowerOnGoodTime;
  UINT32           Data32;
  BOOLEAN          Flag = FALSE;

  if ((Attributes & ~(EFI_USB_HC_RESET_GLOBAL | EFI_USB_HC_RESET_HOST_CONTROLLER)) != 0) {
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_SUCCESS;
  Ohc    = USB_OHCI_HC_DEV_FROM_THIS (This);

  if ((Attributes & EFI_USB_HC_RESET_HOST_CONTROLLER) != 0) {
    Status = OhciSetHcCommandStatus (Ohc, HC_RESET, HC_RESET);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    //
    // Wait for host controller reset.
    //
    PowerOnGoodTime = 50;
    do {
      gBS->Stall (1 * 1000);
      Data32 = OhciGetOperationalReg (Ohc, HC_COMMAND_STATUS);
      if (EFI_ERROR (Status)) {
        return EFI_DEVICE_ERROR;
      }

      if ((Data32 & HC_RESET) == 0) {
        Flag = TRUE;
        break;
      }
    } while (PowerOnGoodTime--);

    if (!Flag) {
      return EFI_DEVICE_ERROR;
    }
  }

  OhciFreeIntTransferMemory (Ohc);
  Status = OhciInitializeInterruptList (Ohc);
  OhciSetFrameInterval (Ohc, FRAME_INTERVAL, 0x2edf);
  if ((Attributes &  EFI_USB_HC_RESET_GLOBAL) != 0) {
    Status = OhciSetHcControl (Ohc, HC_FUNCTIONAL_STATE, HC_STATE_RESET);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    gBS->Stall (50 * 1000);
  }

  //
  // Initialize host controller operational registers
  //
  OhciSetFrameInterval (Ohc, FS_LARGEST_DATA_PACKET, 0x2778);
  OhciSetFrameInterval (Ohc, FRAME_INTERVAL, 0x2edf);
  OhciSetPeriodicStart (Ohc, 0x2a2f);
  OhciSetHcControl (Ohc, CONTROL_BULK_RATIO, 0x3);
  OhciSetHcCommandStatus (Ohc, CONTROL_LIST_FILLED | BULK_LIST_FILLED, 0);
  OhciSetRootHubDescriptor (Ohc, RH_PSWITCH_MODE, 0);
  OhciSetRootHubDescriptor (Ohc, RH_NO_PSWITCH | RH_NOC_PROT, 1);
  // OhciSetRootHubDescriptor (Hc, RH_PSWITCH_MODE | RH_NO_PSWITCH, 0);
  // OhciSetRootHubDescriptor (Hc, RH_PSWITCH_MODE | RH_NOC_PROT, 1);

  OhciSetRootHubDescriptor (Ohc, RH_DEV_REMOVABLE, 0);
  OhciSetRootHubDescriptor (Ohc, RH_PORT_PWR_CTRL_MASK, 0xffff);
  OhciSetRootHubStatus (Ohc, RH_LOCAL_PSTAT_CHANGE);
  OhciSetRootHubPortStatus (Ohc, 0, RH_SET_PORT_POWER);
  OhciGetCapability (This, NULL, &NumOfPorts, NULL);
  for (Index = 0; Index < NumOfPorts; Index++) {
    if (!EFI_ERROR (OhciSetRootHubPortFeature (This, Index, EfiUsbPortReset))) {
      OhciClearRootHubPortFeature (This, Index, EfiUsbPortReset);
      OhciSetRootHubPortFeature (This, Index, EfiUsbPortEnable);
    }
  }

  OhciSetMemoryPointer (Ohc, HC_HCCA, Ohc->HccaMemoryBlock);
  OhciSetMemoryPointer (Ohc, HC_CONTROL_HEAD, NULL);
  OhciSetMemoryPointer (Ohc, HC_BULK_HEAD, NULL);
  OhciSetHcControl (Ohc, PERIODIC_ENABLE | CONTROL_ENABLE | BULK_ENABLE, 1); /*ISOCHRONOUS_ENABLE*/
  OhciSetHcControl (Ohc, HC_FUNCTIONAL_STATE, HC_STATE_OPERATIONAL);

  //
  // Wait till first SOF occurs, and then clear it
  //
  while (OhciGetHcInterruptStatus (Ohc, START_OF_FRAME) == 0) {
  }

  OhciClearInterruptStatus (Ohc, START_OF_FRAME);
  gBS->Stall (1000);

  return Status;
}

/**
  Retrieves current state of the USB host controller.

  @param  This  A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  State A pointer to the EFI_USB_HC_STATE data structure that
                indicates current state of the USB host controller.

  @retval EFI_SUCCESS           The state information of the host controller was returned in State.
  @retval EFI_INVALID_PARAMETER State is NULL.
  @retval EFI_DEVICE_ERROR      An error was encountered while attempting to retrieve the
                                host controller's current state.

**/
EFI_STATUS
EFIAPI
OhciGetState (
  IN  EFI_USB2_HC_PROTOCOL  *This,
  OUT EFI_USB_HC_STATE      *State
  )
{
  USB_OHCI_HC_DEV  *Ohc;
  UINT32           FuncState;

  if (State == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Ohc = USB_OHCI_HC_DEV_FROM_THIS (This);

  FuncState = OhciGetHcControl (Ohc, HC_FUNCTIONAL_STATE);

  switch (FuncState) {
    case HC_STATE_RESET:
    case HC_STATE_RESUME:
      *State = EfiUsbHcStateHalt;
      break;

    case HC_STATE_OPERATIONAL:
      *State = EfiUsbHcStateOperational;
      break;

    case HC_STATE_SUSPEND:
      *State = EfiUsbHcStateSuspend;
      break;

    default:
      ASSERT (FALSE);
  }

  return EFI_SUCCESS;
}

/**
  Sets the USB host controller to a specific state.

  @param  This  A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  State Indicates the state of the host controller that will be set.

  @retval EFI_SUCCESS           The USB host controller was successfully placed in the state
                                specified by State.
  @retval EFI_INVALID_PARAMETER State is not valid.
  @retval EFI_DEVICE_ERROR      Failed to set the state specified by State due to device error.

**/
EFI_STATUS
EFIAPI
OhciSetState (
  IN EFI_USB2_HC_PROTOCOL  *This,
  IN EFI_USB_HC_STATE      State
  )
{
  EFI_STATUS       Status;
  USB_OHCI_HC_DEV  *Ohc;

  Ohc = USB_OHCI_HC_DEV_FROM_THIS (This);

  switch (State) {
    case EfiUsbHcStateHalt:
      Status = OhciSetHcControl (Ohc, HC_FUNCTIONAL_STATE, HC_STATE_RESET);
      break;

    case EfiUsbHcStateOperational:
      Status = OhciSetHcControl (Ohc, HC_FUNCTIONAL_STATE, HC_STATE_OPERATIONAL);
      break;

    case EfiUsbHcStateSuspend:
      Status = OhciSetHcControl (Ohc, HC_FUNCTIONAL_STATE, HC_STATE_SUSPEND);
      break;

    default:
      Status = EFI_INVALID_PARAMETER;
  }

  gBS->Stall (1000);

  return Status;
}

/**
  Submits control transfer to a target USB device.

  @param  This                A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  DeviceAddress       Represents the address of the target device on the USB.
  @param  DeviceSpeed         Indicates device speed.
  @param  MaximumPacketLength Indicates the maximum packet size that the default control transfer
                              endpoint is capable of sending or receiving.
  @param  Request             A pointer to the USB device request that will be sent to the USB device.
  @param  TransferDirection   Specifies the data direction for the transfer. There are three values
                              available, EfiUsbDataIn, EfiUsbDataOut and EfiUsbNoData.
  @param  Data                A pointer to the buffer of data that will be transmitted to USB device or
                              received from USB device.
  @param  DataLength          On input, indicates the size, in bytes, of the data buffer specified by Data.
                              On output, indicates the amount of data actually transferred.
  @param  TimeOut             Indicates the maximum time, in milliseconds, which the transfer is
                              allowed to complete.
  @param  Translator          A pointer to the transaction translator data.
  @param  TransferResult      A pointer to the detailed result information generated by this control
                              transfer.

  @retval EFI_SUCCESS           The control transfer was completed successfully.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The control transfer could not be completed due to a lack of resources.
  @retval EFI_TIMEOUT           The control transfer failed due to timeout.
  @retval EFI_DEVICE_ERROR      The control transfer failed due to host controller or device error.
                                Caller should check TransferResult for detailed error information.

**/
EFI_STATUS
EFIAPI
OhciControlTransfer (
  IN     EFI_USB2_HC_PROTOCOL                *This,
  IN     UINT8                               DeviceAddress,
  IN     UINT8                               DeviceSpeed,
  IN     UINTN                               MaximumPacketLength,
  IN     EFI_USB_DEVICE_REQUEST              *Request,
  IN     EFI_USB_DATA_DIRECTION              TransferDirection,
  IN OUT VOID                                *Data       OPTIONAL,
  IN OUT UINTN                               *DataLength OPTIONAL,
  IN     UINTN                               TimeOut,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR  *Translator,
  OUT    UINT32                              *TransferResult
  )
{
  USB_OHCI_HC_DEV  *Ohc;
  ED_DESCRIPTOR    *HeadEd;
  ED_DESCRIPTOR    *Ed;
  TD_DESCRIPTOR    *HeadTd;
  TD_DESCRIPTOR    *SetupTd;
  TD_DESCRIPTOR    *DataTd;
  TD_DESCRIPTOR    *StatusTd;
  TD_DESCRIPTOR    *EmptyTd;
  EFI_STATUS       Status;
  UINT32           DataPidDir;
  UINT32           StatusPidDir;
  UINTN            TimeCount;
  OHCI_ED_RESULT   EdResult;
  BOOLEAN          IsSlowDevice;

  DMA_MAP_OPERATION  MapOp;

  UINTN  ActualSendLength;
  UINTN  LeftLength;
  UINT8  DataToggle;

  VOID                  *ReqMapping   = NULL;
  UINTN                 ReqMapLength  = 0;
  EFI_PHYSICAL_ADDRESS  ReqMapPhyAddr = 0;

  VOID                  *DataMapping   = NULL;
  UINTN                 DataMapLength  = 0;
  EFI_PHYSICAL_ADDRESS  DataMapPhyAddr = 0;

  HeadTd = NULL;
  DataTd = NULL;

  IsSlowDevice = (DeviceSpeed == EFI_USB_SPEED_LOW);

  if (((TransferDirection != EfiUsbDataOut) && (TransferDirection != EfiUsbDataIn) &&
       (TransferDirection != EfiUsbNoData)) ||
      (Request == NULL) || (DataLength == NULL) || (TransferResult == NULL) ||
      ((TransferDirection == EfiUsbNoData) && ((*DataLength != 0) || (Data != NULL))) ||
      ((TransferDirection != EfiUsbNoData) && ((*DataLength == 0) || (Data == NULL))) ||
      (IsSlowDevice && (MaximumPacketLength != 8)) ||
      ((MaximumPacketLength != 8) && (MaximumPacketLength != 16) &&
       (MaximumPacketLength != 32) && (MaximumPacketLength != 64)))
  {
    return EFI_INVALID_PARAMETER;
  }

  if (*DataLength > MAX_BYTES_PER_TD) {
    DEBUG ((DEBUG_ERROR, "OhciControlTransfer: Request data size is too large\r\n"));
    return EFI_INVALID_PARAMETER;
  }

  Ohc = USB_OHCI_HC_DEV_FROM_THIS (This);

  if (TransferDirection == EfiUsbDataIn) {
    DataPidDir   = TD_IN_PID;
    StatusPidDir = TD_OUT_PID;
  } else {
    DataPidDir   = TD_OUT_PID;
    StatusPidDir = TD_IN_PID;
  }

  Status = OhciSetHcControl (Ohc, CONTROL_ENABLE, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OhciControlTransfer: fail to disable CONTROL_ENABLE\r\n"));
    *TransferResult = EFI_USB_ERR_SYSTEM;
    return EFI_DEVICE_ERROR;
  }

  Status = OhciSetHcCommandStatus (Ohc, CONTROL_LIST_FILLED, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OhciControlTransfer: fail to disable CONTROL_LIST_FILLED\r\n"));
    *TransferResult = EFI_USB_ERR_SYSTEM;
    return EFI_DEVICE_ERROR;
  }

  OhciSetMemoryPointer (Ohc, HC_CONTROL_HEAD, NULL);
  Ed = OhciCreateED (Ohc);
  if (Ed == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_INFO, "OhciControlTransfer: Fail to allocate ED buffer\r\n"));
    goto CTRL_EXIT;
  }

  OhciSetEDField (Ed, ED_SKIP, 1);
  OhciSetEDField (Ed, ED_FUNC_ADD, DeviceAddress);
  OhciSetEDField (Ed, ED_ENDPT_NUM, 0);
  OhciSetEDField (Ed, ED_DIR, ED_FROM_TD_DIR);
  OhciSetEDField (Ed, ED_SPEED, IsSlowDevice);
  OhciSetEDField (Ed, ED_FORMAT | ED_HALTED | ED_DTTOGGLE, 0);
  OhciSetEDField (Ed, ED_MAX_PACKET, MaximumPacketLength);
  OhciSetEDField (Ed, ED_PDATA, 0);
  OhciSetEDField (Ed, ED_ZERO, 0);
  OhciSetEDField (Ed, ED_TDHEAD_PTR, 0);
  OhciSetEDField (Ed, ED_TDTAIL_PTR, 0);
  OhciSetEDField (Ed, ED_NEXT_EDPTR, 0);
  HeadEd = OhciAttachEDToList (Ohc, CONTROL_LIST, Ed, NULL);
  //
  // Setup Stage
  //
  if (Request != NULL) {
    ReqMapLength = sizeof (EFI_USB_DEVICE_REQUEST);
    MapOp        = MapOperationBusMasterRead;
    Status       = DmaMap (MapOp, (UINT8 *)Request, &ReqMapLength, &ReqMapPhyAddr, &ReqMapping);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OhciControlTransfer: Fail to Map Request Buffer\r\n"));
      goto FREE_ED_BUFF;
    }
  }

  SetupTd = OhciCreateTD (Ohc);
  if (SetupTd == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_INFO, "OhciControlTransfer: Fail to allocate Setup TD buffer\r\n"));
    goto UNMAP_SETUP_BUFF;
  }

  HeadTd = SetupTd;
  OhciSetTDField (SetupTd, TD_PDATA, 0);
  OhciSetTDField (SetupTd, TD_BUFFER_ROUND, 1);
  OhciSetTDField (SetupTd, TD_DIR_PID, TD_SETUP_PID);
  OhciSetTDField (SetupTd, TD_DELAY_INT, TD_NO_DELAY);
  OhciSetTDField (SetupTd, TD_DT_TOGGLE, 2);
  OhciSetTDField (SetupTd, TD_ERROR_CNT, 0);
  OhciSetTDField (SetupTd, TD_COND_CODE, TD_TOBE_PROCESSED);
  OhciSetTDField (SetupTd, TD_CURR_BUFFER_PTR, (UINT32)ReqMapPhyAddr);
  OhciSetTDField (SetupTd, TD_NEXT_PTR, 0);
  OhciSetTDField (SetupTd, TD_BUFFER_END_PTR, (UINT32)(ReqMapPhyAddr + sizeof (EFI_USB_DEVICE_REQUEST) - 1));
  SetupTd->ActualSendLength = sizeof (EFI_USB_DEVICE_REQUEST);
  SetupTd->DataBuffer       = (UINT32)ReqMapPhyAddr;
  SetupTd->NextTDPointer    = 0;

  if (TransferDirection == EfiUsbDataIn) {
    MapOp = MapOperationBusMasterWrite;
  } else {
    MapOp = MapOperationBusMasterRead;
  }

  DataMapLength = *DataLength;
  if ((Data != NULL) && (DataMapLength != 0)) {
    Status = DmaMap (MapOp, Data, &DataMapLength, &DataMapPhyAddr, &DataMapping);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OhciControlTransfer: Fail To Map Data Buffer\r\n"));
      goto FREE_TD_BUFF;
    }
  }

  //
  // Data Stage
  //
  LeftLength       = DataMapLength;
  ActualSendLength = DataMapLength;
  DataToggle       = 1;
  while (LeftLength > 0) {
    ActualSendLength = LeftLength;
    if (LeftLength > MaximumPacketLength) {
      ActualSendLength = MaximumPacketLength;
    }

    DataTd = OhciCreateTD (Ohc);
    if (DataTd == NULL) {
      DEBUG ((DEBUG_INFO, "OhciControlTransfer: Fail to allocate buffer for Data Stage TD\r\n"));
      Status = EFI_OUT_OF_RESOURCES;
      goto UNMAP_DATA_BUFF;
    }

    OhciSetTDField (DataTd, TD_PDATA, 0);
    OhciSetTDField (DataTd, TD_BUFFER_ROUND, 1);
    OhciSetTDField (DataTd, TD_DIR_PID, DataPidDir);
    OhciSetTDField (DataTd, TD_DELAY_INT, TD_NO_DELAY);
    OhciSetTDField (DataTd, TD_DT_TOGGLE, DataToggle);
    OhciSetTDField (DataTd, TD_ERROR_CNT, 0);
    OhciSetTDField (DataTd, TD_COND_CODE, TD_TOBE_PROCESSED);
    OhciSetTDField (DataTd, TD_CURR_BUFFER_PTR, (UINT32)DataMapPhyAddr);
    OhciSetTDField (DataTd, TD_BUFFER_END_PTR, (UINT32)(DataMapPhyAddr + ActualSendLength - 1));
    OhciSetTDField (DataTd, TD_NEXT_PTR, 0);
    DataTd->ActualSendLength = (UINT32)ActualSendLength;
    DataTd->DataBuffer       = (UINT32)DataMapPhyAddr;
    DataTd->NextTDPointer    = 0;
    OhciLinkTD (HeadTd, DataTd);
    DataToggle     ^= 1;
    DataMapPhyAddr += ActualSendLength;
    LeftLength     -= ActualSendLength;
  }

  //
  // Status Stage
  //
  StatusTd = OhciCreateTD (Ohc);
  if (StatusTd == NULL) {
    DEBUG ((DEBUG_INFO, "OhciControlTransfer: Fail to allocate buffer for Status Stage TD\r\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto UNMAP_DATA_BUFF;
  }

  OhciSetTDField (StatusTd, TD_PDATA, 0);
  OhciSetTDField (StatusTd, TD_BUFFER_ROUND, 1);
  OhciSetTDField (StatusTd, TD_DIR_PID, StatusPidDir);
  OhciSetTDField (StatusTd, TD_DELAY_INT, 7);
  OhciSetTDField (StatusTd, TD_DT_TOGGLE, 3);
  OhciSetTDField (StatusTd, TD_ERROR_CNT, 0);
  OhciSetTDField (StatusTd, TD_COND_CODE, TD_TOBE_PROCESSED);
  OhciSetTDField (StatusTd, TD_CURR_BUFFER_PTR, 0);
  OhciSetTDField (StatusTd, TD_NEXT_PTR, 0);
  OhciSetTDField (StatusTd, TD_BUFFER_END_PTR, 0);
  StatusTd->ActualSendLength = 0;
  StatusTd->DataBuffer       = 0;
  StatusTd->NextTDPointer    = 0;
  OhciLinkTD (HeadTd, StatusTd);
  //
  // Empty Stage
  //
  EmptyTd = OhciCreateTD (Ohc);
  if (EmptyTd == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto UNMAP_DATA_BUFF;
  }

  OhciSetTDField (EmptyTd, TD_PDATA, 0);
  OhciSetTDField (EmptyTd, TD_BUFFER_ROUND, 0);
  OhciSetTDField (EmptyTd, TD_DIR_PID, 0);
  OhciSetTDField (EmptyTd, TD_DELAY_INT, 0);
  // OhciSetTDField (EmptyTd, TD_DT_TOGGLE, CurrentToggle);
  EmptyTd->Word0.DataToggle = 0;
  OhciSetTDField (EmptyTd, TD_ERROR_CNT, 0);
  OhciSetTDField (EmptyTd, TD_COND_CODE, 0);
  OhciSetTDField (EmptyTd, TD_CURR_BUFFER_PTR, 0);
  OhciSetTDField (EmptyTd, TD_BUFFER_END_PTR, 0);
  OhciSetTDField (EmptyTd, TD_NEXT_PTR, 0);
  EmptyTd->ActualSendLength = 0;
  EmptyTd->DataBuffer       = 0;
  EmptyTd->NextTDPointer    = 0;
  OhciLinkTD (HeadTd, EmptyTd);
  Ed->TdTailPointer = (UINT32)(UINTN)EmptyTd;
  OhciAttachTDListToED (Ed, HeadTd);
  //
  // For debugging,  dump ED & TD buffer befor transferring
  //
  //
  // OhciDumpEdTdInfo (Ohc, Ed, HeadTd, TRUE);
  //
  OhciSetEDField (Ed, ED_SKIP, 0);
  Status = OhciSetHcControl (Ohc, CONTROL_ENABLE, 1);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OhciControlTransfer: fail to enable CONTROL_ENABLE\r\n"));
    *TransferResult = EFI_USB_ERR_SYSTEM;
    Status          = EFI_DEVICE_ERROR;
    goto UNMAP_DATA_BUFF;
  }

  Status = OhciSetHcCommandStatus (Ohc, CONTROL_LIST_FILLED, 1);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OhciControlTransfer: fail to enable CONTROL_LIST_FILLED\r\n"));
    *TransferResult = EFI_USB_ERR_SYSTEM;
    Status          = EFI_DEVICE_ERROR;
    goto UNMAP_DATA_BUFF;
  }

  TimeCount = 0;
  Status    = CheckIfDone (Ohc, CONTROL_LIST, Ed, HeadTd, &EdResult);

  while (Status == EFI_NOT_READY && TimeCount <= TimeOut) {
    gBS->Stall (1000);
    TimeCount++;
    Status = CheckIfDone (Ohc, CONTROL_LIST, Ed, HeadTd, &EdResult);
  }

  //
  // For debugging, dump ED & TD buffer after transferring
  //
  // OhciDumpEdTdInfo (Ohc, Ed, HeadTd, FALSE);
  //
  *TransferResult = ConvertErrorCode (EdResult.ErrorCode);

  if (EdResult.ErrorCode != TD_NO_ERROR) {
    if (EdResult.ErrorCode == TD_TOBE_PROCESSED) {
      DEBUG ((DEBUG_ERROR, "Control pipe timeout, > %d mS\r\n", TimeOut));
    } else {
      DEBUG ((DEBUG_ERROR, "Control pipe broken\r\n"));
    }

    *DataLength = 0;
  } else {
    DEBUG ((DEBUG_VERBOSE, "Control transfer successed\r\n"));
  }

UNMAP_DATA_BUFF:
  OhciSetEDField (Ed, ED_SKIP, 1);
  if (HeadEd == Ed) {
    OhciSetMemoryPointer (Ohc, HC_CONTROL_HEAD, NULL);
  } else {
    HeadEd->NextED = Ed->NextED;
  }

  if (DataMapping != NULL) {
    DmaUnmap (DataMapping);
  }

FREE_TD_BUFF:
  while (HeadTd) {
    DataTd = HeadTd;
    HeadTd = (TD_DESCRIPTOR *)(UINTN)(HeadTd->NextTDPointer);
    UsbHcFreeMem (Ohc->MemPool, DataTd, sizeof (TD_DESCRIPTOR));
  }

UNMAP_SETUP_BUFF:
  if (ReqMapping != NULL) {
    DmaUnmap (ReqMapping);
  }

FREE_ED_BUFF:
  UsbHcFreeMem (Ohc->MemPool, Ed, sizeof (ED_DESCRIPTOR));

CTRL_EXIT:
  return Status;
}

/**
  Submits bulk transfer to a bulk endpoint of a USB device.

  @param  This                A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  DeviceAddress       Represents the address of the target device on the USB.
  @param  EndPointAddress     The combination of an endpoint number and an endpoint direction of the
                              target USB device.
  @param  DeviceSpeed         Indicates device speed.
  @param  MaximumPacketLength Indicates the maximum packet size the target endpoint is capable of
                              sending or receiving.
  @param  DataBuffersNumber   Number of data buffers prepared for the transfer.
  @param  Data                Array of pointers to the buffers of data that will be transmitted to USB
                              device or received from USB device.
  @param  DataLength          When input, indicates the size, in bytes, of the data buffers specified by
                              Data. When output, indicates the actually transferred data size.
  @param  DataToggle          A pointer to the data toggle value.
  @param  TimeOut             Indicates the maximum time, in milliseconds, which the transfer is
                              allowed to complete.
  @param  Translator          A pointer to the transaction translator data.
  @param  TransferResult      A pointer to the detailed result information of the bulk transfer.

  @retval EFI_SUCCESS           The bulk transfer was completed successfully.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The bulk transfer could not be submitted due to a lack of resources.
  @retval EFI_TIMEOUT           The bulk transfer failed due to timeout.
  @retval EFI_DEVICE_ERROR      The bulk transfer failed due to host controller or device error.
                                Caller should check TransferResult for detailed error information.

**/
EFI_STATUS
EFIAPI
OhciBulkTransfer (
  IN     EFI_USB2_HC_PROTOCOL                *This,
  IN     UINT8                               DeviceAddress,
  IN     UINT8                               EndPointAddress,
  IN     UINT8                               DeviceSpeed,
  IN     UINTN                               MaximumPacketLength,
  IN     UINT8                               DataBuffersNumber,
  IN OUT VOID                                *Data[EFI_USB_MAX_BULK_BUFFER_NUM],
  IN OUT UINTN                               *DataLength,
  IN OUT UINT8                               *DataToggle,
  IN     UINTN                               TimeOut,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR  *Translator,
  OUT    UINT32                              *TransferResult
  )
{
  USB_OHCI_HC_DEV  *Ohc;
  ED_DESCRIPTOR    *HeadEd;
  ED_DESCRIPTOR    *Ed;
  UINT32           DataPidDir;
  TD_DESCRIPTOR    *HeadTd;
  TD_DESCRIPTOR    *DataTd;
  TD_DESCRIPTOR    *EmptyTd;
  EFI_STATUS       Status;
  UINT8            EndPointNum;
  UINTN            TimeCount;
  OHCI_ED_RESULT   EdResult;
  VOID             *TransferData;

  DMA_MAP_OPERATION     MapOp;
  VOID                  *Mapping;
  UINTN                 MapLength;
  EFI_PHYSICAL_ADDRESS  MapPyhAddr;
  UINTN                 LeftLength;
  UINTN                 ActualSendLength;
  BOOLEAN               FirstTD;

  Mapping    = NULL;
  MapLength  = 0;
  MapPyhAddr = 0;
  LeftLength = 0;
  Status     = EFI_SUCCESS;

  if ((Data == NULL) || (DataLength == NULL) || (DataToggle == NULL) || (TransferResult == NULL) ||
      (*DataLength == 0) || ((*DataToggle != 0) && (*DataToggle != 1)) ||
      ((MaximumPacketLength != 8) && (MaximumPacketLength != 16) &&
       (MaximumPacketLength != 32) && (MaximumPacketLength != 64)))
  {
    return EFI_INVALID_PARAMETER;
  }

  // Full-speed transfers only use the first data buffer.
  TransferData = Data[0];

  if (TransferData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Ohc = USB_OHCI_HC_DEV_FROM_THIS (This);

  if ((EndPointAddress & 0x80) != 0) {
    DataPidDir = TD_IN_PID;
    MapOp      = MapOperationBusMasterWrite;
  } else {
    DataPidDir = TD_OUT_PID;
    MapOp      = MapOperationBusMasterRead;
  }

  EndPointNum         = (EndPointAddress & 0xF);
  EdResult.NextToggle = *DataToggle;

  Status = OhciSetHcControl (Ohc, BULK_ENABLE, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OhciControlTransfer: fail to disable BULK_ENABLE\r\n"));
    *TransferResult = EFI_USB_ERR_SYSTEM;
    return EFI_DEVICE_ERROR;
  }

  Status = OhciSetHcCommandStatus (Ohc, BULK_LIST_FILLED, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "OhciControlTransfer: fail to disable BULK_LIST_FILLED\r\n"));
    *TransferResult = EFI_USB_ERR_SYSTEM;
    return EFI_DEVICE_ERROR;
  }

  OhciSetMemoryPointer (Ohc, HC_BULK_HEAD, NULL);

  Ed = OhciCreateED (Ohc);
  if (Ed == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  OhciSetEDField (Ed, ED_SKIP, 1);
  OhciSetEDField (Ed, ED_FUNC_ADD, DeviceAddress);
  OhciSetEDField (Ed, ED_ENDPT_NUM, EndPointNum);
  OhciSetEDField (Ed, ED_DIR, ED_FROM_TD_DIR);
  OhciSetEDField (Ed, ED_SPEED, HI_SPEED);
  OhciSetEDField (Ed, ED_FORMAT | ED_HALTED | ED_DTTOGGLE, 0);
  OhciSetEDField (Ed, ED_MAX_PACKET, MaximumPacketLength);
  OhciSetEDField (Ed, ED_PDATA, 0);
  OhciSetEDField (Ed, ED_ZERO, 0);
  OhciSetEDField (Ed, ED_TDHEAD_PTR, 0);
  OhciSetEDField (Ed, ED_TDTAIL_PTR, 0);
  OhciSetEDField (Ed, ED_NEXT_EDPTR, 0);
  HeadEd = OhciAttachEDToList (Ohc, BULK_LIST, Ed, NULL);

  if (Data != NULL) {
    MapLength = *DataLength;
    Status    = DmaMap (MapOp, TransferData, &MapLength, &MapPyhAddr, &Mapping);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "OhciBulkTransfer: Fail to Map Data Buffer for Bulk\r\n"));
      goto FREE_ED_BUFF;
    }
  }

  //
  // Data Stage
  //
  LeftLength       = MapLength;
  ActualSendLength = MapLength;
  HeadTd           = NULL;
  FirstTD          = TRUE;
  while (LeftLength > 0) {
    ActualSendLength = LeftLength;
    if (LeftLength > MaximumPacketLength) {
      ActualSendLength = MaximumPacketLength;
    }

    DataTd = OhciCreateTD (Ohc);
    if (DataTd == NULL) {
      DEBUG ((DEBUG_INFO, "OhciBulkTransfer: Fail to allocate buffer for Data Stage TD\r\n"));
      Status = EFI_OUT_OF_RESOURCES;
      goto FREE_OHCI_TDBUFF;
    }

    OhciSetTDField (DataTd, TD_PDATA, 0);
    OhciSetTDField (DataTd, TD_BUFFER_ROUND, 1);
    OhciSetTDField (DataTd, TD_DIR_PID, DataPidDir);
    OhciSetTDField (DataTd, TD_DELAY_INT, TD_NO_DELAY);
    OhciSetTDField (DataTd, TD_DT_TOGGLE, *DataToggle);
    OhciSetTDField (DataTd, TD_ERROR_CNT, 0);
    OhciSetTDField (DataTd, TD_COND_CODE, TD_TOBE_PROCESSED);
    OhciSetTDField (DataTd, TD_CURR_BUFFER_PTR, (UINT32)MapPyhAddr);
    OhciSetTDField (DataTd, TD_BUFFER_END_PTR, (UINT32)(MapPyhAddr + ActualSendLength - 1));
    OhciSetTDField (DataTd, TD_NEXT_PTR, 0);
    DataTd->ActualSendLength = (UINT32)ActualSendLength;
    DataTd->DataBuffer       = (UINT32)MapPyhAddr;
    DataTd->NextTDPointer    = 0;
    if (FirstTD) {
      HeadTd  = DataTd;
      FirstTD = FALSE;
    } else {
      OhciLinkTD (HeadTd, DataTd);
    }

    *DataToggle ^= 1;
    MapPyhAddr  += ActualSendLength;
    LeftLength  -= ActualSendLength;
  }

  //
  // Empty Stage
  //
  EmptyTd = OhciCreateTD (Ohc);
  if (EmptyTd == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_INFO, "OhciBulkTransfer: Fail to allocate buffer for Empty TD\r\n"));
    goto FREE_OHCI_TDBUFF;
  }

  OhciSetTDField (EmptyTd, TD_PDATA, 0);
  OhciSetTDField (EmptyTd, TD_BUFFER_ROUND, 0);
  OhciSetTDField (EmptyTd, TD_DIR_PID, 0);
  OhciSetTDField (EmptyTd, TD_DELAY_INT, 0);
  // OhciSetTDField (EmptyTd, TD_DT_TOGGLE, CurrentToggle);
  EmptyTd->Word0.DataToggle = 0;
  OhciSetTDField (EmptyTd, TD_ERROR_CNT, 0);
  OhciSetTDField (EmptyTd, TD_COND_CODE, 0);
  OhciSetTDField (EmptyTd, TD_CURR_BUFFER_PTR, 0);
  OhciSetTDField (EmptyTd, TD_BUFFER_END_PTR, 0);
  OhciSetTDField (EmptyTd, TD_NEXT_PTR, 0);
  EmptyTd->ActualSendLength = 0;
  EmptyTd->DataBuffer       = 0;
  EmptyTd->NextTDPointer    = 0;
  OhciLinkTD (HeadTd, EmptyTd);
  Ed->TdTailPointer = (UINT32)(UINTN)EmptyTd;
  OhciAttachTDListToED (Ed, HeadTd);

  OhciSetEDField (Ed, ED_SKIP, 0);
  Status = OhciSetHcCommandStatus (Ohc, BULK_LIST_FILLED, 1);
  if (EFI_ERROR (Status)) {
    *TransferResult = EFI_USB_ERR_SYSTEM;
    Status          = EFI_DEVICE_ERROR;
    DEBUG ((DEBUG_INFO, "OhciControlTransfer: Fail to enable BULK_LIST_FILLED\r\n"));
    goto FREE_OHCI_TDBUFF;
  }

  Status = OhciSetHcControl (Ohc, BULK_ENABLE, 1);
  if (EFI_ERROR (Status)) {
    *TransferResult = EFI_USB_ERR_SYSTEM;
    Status          = EFI_DEVICE_ERROR;
    DEBUG ((DEBUG_INFO, "OhciControlTransfer: Fail to enable BULK_ENABLE\r\n"));
    goto FREE_OHCI_TDBUFF;
  }

  TimeCount = 0;
  Status    = CheckIfDone (Ohc, BULK_LIST, Ed, HeadTd, &EdResult);
  while (Status == EFI_NOT_READY && TimeCount <= TimeOut) {
    gBS->Stall (1000);
    TimeCount++;
    Status = CheckIfDone (Ohc, BULK_LIST, Ed, HeadTd, &EdResult);
  }

  *TransferResult = ConvertErrorCode (EdResult.ErrorCode);

  if (EdResult.ErrorCode != TD_NO_ERROR) {
    if (EdResult.ErrorCode == TD_TOBE_PROCESSED) {
      DEBUG ((DEBUG_ERROR, "Bulk pipe timeout, > %d mS\r\n", TimeOut));
    } else {
      DEBUG ((DEBUG_ERROR, "Bulk pipe broken\r\n"));
      *DataToggle = EdResult.NextToggle;
    }

    *DataLength = 0;
  } else {
    DEBUG ((DEBUG_VERBOSE, "Bulk transfer successed\r\n"));
  }

  // *DataToggle = (UINT8) OhciGetEDField (Ed, ED_DTTOGGLE);

FREE_OHCI_TDBUFF:
  OhciSetEDField (Ed, ED_SKIP, 1);
  if (HeadEd == Ed) {
    OhciSetMemoryPointer (Ohc, HC_BULK_HEAD, NULL);
  } else {
    HeadEd->NextED = Ed->NextED;
  }

  while (HeadTd) {
    DataTd = HeadTd;
    HeadTd = (TD_DESCRIPTOR *)(UINTN)(HeadTd->NextTDPointer);
    UsbHcFreeMem (Ohc->MemPool, DataTd, sizeof (TD_DESCRIPTOR));
  }

  if (Mapping != NULL) {
    DmaUnmap (Mapping);
  }

FREE_ED_BUFF:
  UsbHcFreeMem (Ohc->MemPool, Ed, sizeof (ED_DESCRIPTOR));

  return Status;
}

/**

  Submits an interrupt transfer to an interrupt endpoint of a USB device.

  @param  Ohc                   Device private data
  @param  DeviceAddress         Represents the address of the target device on the USB,
                                which is assigned during USB enumeration.
  @param  EndPointAddress       The combination of an endpoint number and an endpoint
                                direction of the target USB device. Each endpoint address
                                supports data transfer in one direction except the
                                control endpoint (whose default endpoint address is 0).
                                It is the caller's responsibility to make sure that
                                the EndPointAddress represents an interrupt endpoint.
  @param  IsSlowDevice          Indicates whether the target device is slow device
                                or full-speed device.
  @param  MaximumPacketLength   Indicates the maximum packet size the target endpoint
                                is capable of sending or receiving.
  @param  IsNewTransfer         If TRUE, an asynchronous interrupt pipe is built between
                                the host and the target interrupt endpoint.
                                If FALSE, the specified asynchronous interrupt pipe
                                is canceled.
  @param  DataToggle            A pointer to the data toggle value.  On input, it is valid
                                when IsNewTransfer is TRUE, and it indicates the initial
                                data toggle value the asynchronous interrupt transfer
                                should adopt.
                                On output, it is valid when IsNewTransfer is FALSE,
                                and it is updated to indicate the data toggle value of
                                the subsequent asynchronous interrupt transfer.
  @param  PollingInterval       Indicates the interval, in milliseconds, that the
                                asynchronous interrupt transfer is polled.
                                This parameter is required when IsNewTransfer is TRUE.
  @param  UCBuffer              Uncacheable buffer
  @param  DataLength            Indicates the length of data to be received at the
                                rate specified by PollingInterval from the target
                                asynchronous interrupt endpoint.  This parameter
                                is only required when IsNewTransfer is TRUE.
  @param  CallBackFunction      The Callback function.This function is called at the
                                rate specified by PollingInterval.This parameter is
                                only required when IsNewTransfer is TRUE.
  @param  Context               The context that is passed to the CallBackFunction.
                                This is an optional parameter and may be NULL.
  @param  IsPeriodic            Periodic interrupt or not
  @param  OutputED              The correspoding ED carried out
  @param  OutputTD              The correspoding TD carried out


  @retval EFI_SUCCESS           The asynchronous interrupt transfer request has been successfully
                                submitted or canceled.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.

**/
EFI_STATUS
OhciInterruptTransfer (
  IN     USB_OHCI_HC_DEV                  *Ohc,
  IN     UINT8                            DeviceAddress,
  IN     UINT8                            EndPointAddress,
  IN     BOOLEAN                          IsSlowDevice,
  IN     UINT8                            MaximumPacketLength,
  IN     BOOLEAN                          IsNewTransfer,
  IN OUT UINT8                            *DataToggle        OPTIONAL,
  IN     UINTN                            PollingInterval    OPTIONAL,
  IN     VOID                             *UCBuffer          OPTIONAL,
  IN     UINTN                            DataLength         OPTIONAL,
  IN     EFI_ASYNC_USB_TRANSFER_CALLBACK  CallBackFunction   OPTIONAL,
  IN     VOID                             *Context           OPTIONAL,
  IN     BOOLEAN                          IsPeriodic         OPTIONAL,
  OUT    ED_DESCRIPTOR                    **OutputED         OPTIONAL,
  OUT    TD_DESCRIPTOR                    **OutputTD         OPTIONAL
  )
{
  ED_DESCRIPTOR            *Ed;
  UINT8                    EdDir;
  ED_DESCRIPTOR            *HeadEd;
  TD_DESCRIPTOR            *HeadTd;
  TD_DESCRIPTOR            *DataTd;
  TD_DESCRIPTOR            *EmptTd;
  UINTN                    Depth;
  UINTN                    Index;
  EFI_STATUS               Status;
  UINT8                    EndPointNum;
  UINT32                   DataPidDir;
  INTERRUPT_CONTEXT_ENTRY  *Entry;
  EFI_TPL                  OldTpl;
  BOOLEAN                  FirstTD;

  VOID                  *Mapping;
  UINTN                 MapLength;
  EFI_PHYSICAL_ADDRESS  MapPyhAddr;
  UINTN                 LeftLength;
  UINTN                 ActualSendLength;

  if (DataLength > MAX_BYTES_PER_TD) {
    DEBUG ((DEBUG_ERROR, "OhciInterruptTransfer: Error param\r\n"));
    return EFI_INVALID_PARAMETER;
  }

  if ((EndPointAddress & 0x80) != 0) {
    EdDir      = ED_IN_DIR;
    DataPidDir = TD_IN_PID;
  } else {
    EdDir      = ED_OUT_DIR;
    DataPidDir = TD_OUT_PID;
  }

  EndPointNum = (EndPointAddress & 0xF);
  if (!IsNewTransfer) {
    OldTpl = gBS->RaiseTPL (TPL_NOTIFY);
    OhciSetHcControl (Ohc, PERIODIC_ENABLE, 0);
    OhciFreeInterruptContext (Ohc, DeviceAddress, EndPointAddress, DataToggle);
    Status = OhciFreeInterruptEdByAddr (Ohc, DeviceAddress, EndPointNum);
    OhciSetHcControl (Ohc, PERIODIC_ENABLE, 1);
    gBS->RestoreTPL (OldTpl);
    return Status;
  }

  MapLength = DataLength;
  Status    = DmaMap (
                MapOperationBusMasterWrite,
                UCBuffer,
                &MapLength,
                &MapPyhAddr,
                &Mapping
                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "OhciInterruptTransfer: Fail to Map buffer \r\n"));
    goto EXIT;
  }

  Depth = 5;
  Index = 1;
  while (PollingInterval >= Index * 2 && Depth > 0) {
    Index *= 2;
    Depth--;
  }

  //
  // ED Stage
  //
  HeadEd = OhciFindMinInterruptEDList (Ohc, (UINT32)Depth);
  if ((Ed = OhciFindWorkingEd (HeadEd, DeviceAddress, EndPointNum, EdDir)) != NULL) {
    OhciSetEDField (Ed, ED_SKIP, 1);
  } else {
    Ed = OhciCreateED (Ohc);
    if (Ed == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      DEBUG ((DEBUG_ERROR, "OhciInterruptTransfer: Fail to allocate buffer for ED\r\n"));
      goto UNMAP_OHCI_XBUFF;
    }

    OhciSetEDField (Ed, ED_SKIP, 1);
    OhciSetEDField (Ed, ED_FUNC_ADD, DeviceAddress);
    OhciSetEDField (Ed, ED_ENDPT_NUM, EndPointNum);
    OhciSetEDField (Ed, ED_DIR, ED_FROM_TD_DIR);
    OhciSetEDField (Ed, ED_SPEED, IsSlowDevice);
    OhciSetEDField (Ed, ED_FORMAT, 0);
    OhciSetEDField (Ed, ED_MAX_PACKET, MaximumPacketLength);
    OhciSetEDField (Ed, ED_PDATA | ED_ZERO | ED_HALTED | ED_DTTOGGLE, 0);
    OhciSetEDField (Ed, ED_TDHEAD_PTR, 0);
    OhciSetEDField (Ed, ED_TDTAIL_PTR, 0);
    OhciSetEDField (Ed, ED_NEXT_EDPTR, 0);
    OhciAttachEDToList (Ohc, INTERRUPT_LIST, Ed, HeadEd);
  }

  //
  // Data Stage
  //
  LeftLength       = MapLength;
  ActualSendLength = MapLength;
  HeadTd           = NULL;
  FirstTD          = TRUE;
  while (LeftLength > 0) {
    ActualSendLength = LeftLength;
    if (LeftLength > MaximumPacketLength) {
      ActualSendLength = MaximumPacketLength;
    }

    DataTd = OhciCreateTD (Ohc);
    if (DataTd == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      DEBUG ((DEBUG_ERROR, "OhciInterruptTransfer: Fail to allocate buffer for Data Stage TD\r\n"));
      goto FREE_OHCI_TDBUFF;
    }

    OhciSetTDField (DataTd, TD_PDATA, 0);
    OhciSetTDField (DataTd, TD_BUFFER_ROUND, 1);
    OhciSetTDField (DataTd, TD_DIR_PID, DataPidDir);
    OhciSetTDField (DataTd, TD_DELAY_INT, TD_NO_DELAY);
    OhciSetTDField (DataTd, TD_DT_TOGGLE, *DataToggle);
    OhciSetTDField (DataTd, TD_ERROR_CNT, 0);
    OhciSetTDField (DataTd, TD_COND_CODE, TD_TOBE_PROCESSED);
    OhciSetTDField (DataTd, TD_CURR_BUFFER_PTR, (UINT32)MapPyhAddr);
    OhciSetTDField (DataTd, TD_BUFFER_END_PTR, (UINT32)(MapPyhAddr + ActualSendLength - 1));
    OhciSetTDField (DataTd, TD_NEXT_PTR, 0);
    DataTd->ActualSendLength = (UINT32)ActualSendLength;
    DataTd->DataBuffer       = (UINT32)(UINTN)MapPyhAddr;
    DataTd->NextTDPointer    = 0;
    if (FirstTD) {
      HeadTd  = DataTd;
      FirstTD = FALSE;
    } else {
      OhciLinkTD (HeadTd, DataTd);
    }

    *DataToggle ^= 1;
    MapPyhAddr  += ActualSendLength;
    LeftLength  -= ActualSendLength;
  }

  EmptTd = OhciCreateTD (Ohc);
  if (EmptTd == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_ERROR, "OhciInterruptTransfer: Fail to allocate buffer for Empty Stage TD\r\n"));
    goto FREE_OHCI_TDBUFF;
  }

  OhciSetTDField (EmptTd, TD_PDATA, 0);
  OhciSetTDField (EmptTd, TD_BUFFER_ROUND, 0);
  OhciSetTDField (EmptTd, TD_DIR_PID, 0);
  OhciSetTDField (EmptTd, TD_DELAY_INT, 0);
  // OhciSetTDField (EmptTd, TD_DT_TOGGLE, CurrentToggle);
  EmptTd->Word0.DataToggle = 0;
  OhciSetTDField (EmptTd, TD_ERROR_CNT, 0);
  OhciSetTDField (EmptTd, TD_COND_CODE, 0);
  OhciSetTDField (EmptTd, TD_CURR_BUFFER_PTR, 0);
  OhciSetTDField (EmptTd, TD_BUFFER_END_PTR, 0);
  OhciSetTDField (EmptTd, TD_NEXT_PTR, 0);
  EmptTd->ActualSendLength = 0;
  EmptTd->DataBuffer       = 0;
  EmptTd->NextTDPointer    = 0;
  OhciLinkTD (HeadTd, EmptTd);
  Ed->TdTailPointer = (UINT32)(UINTN)EmptTd;
  OhciAttachTDListToED (Ed, HeadTd);

  if (OutputED != NULL) {
    *OutputED = Ed;
  }

  if (OutputTD != NULL) {
    *OutputTD = HeadTd;
  }

  if (CallBackFunction != NULL) {
    Entry = AllocatePool (sizeof (INTERRUPT_CONTEXT_ENTRY));
    if (Entry == NULL) {
      goto FREE_OHCI_TDBUFF;
    }

    Entry->DeviceAddress       = DeviceAddress;
    Entry->EndPointAddress     = EndPointAddress;
    Entry->Ed                  = Ed;
    Entry->DataTd              = HeadTd;
    Entry->IsSlowDevice        = IsSlowDevice;
    Entry->MaximumPacketLength = MaximumPacketLength;
    Entry->PollingInterval     = PollingInterval;
    Entry->CallBackFunction    = CallBackFunction;
    Entry->Context             = Context;
    Entry->IsPeriodic          = IsPeriodic;
    Entry->UCBuffer            = UCBuffer;
    Entry->UCBufferMapping     = Mapping;
    Entry->DataLength          = DataLength;
    Entry->Toggle              = DataToggle;
    Entry->NextEntry           = NULL;
    OhciAddInterruptContextEntry (Ohc, Entry);
  }

  OhciSetEDField (Ed, ED_SKIP, 0);

  if (OhciGetHcControl (Ohc, PERIODIC_ENABLE) == 0) {
    Status = OhciSetHcControl (Ohc, PERIODIC_ENABLE, 1);
    gBS->Stall (1000);
  }

  return EFI_SUCCESS;

FREE_OHCI_TDBUFF:
  while (HeadTd) {
    DataTd = HeadTd;
    HeadTd = (TD_DESCRIPTOR *)(UINTN)(HeadTd->NextTDPointer);
    UsbHcFreeMem (Ohc->MemPool, DataTd, sizeof (TD_DESCRIPTOR));
  }

  // FREE_OHCI_EDBUFF:
  if ((HeadEd != Ed) && HeadEd && Ed) {
    while (HeadEd->NextED != (UINT32)(UINTN)Ed) {
      HeadEd = (ED_DESCRIPTOR *)(UINTN)(HeadEd->NextED);
    }

    HeadEd->NextED = Ed->NextED;
    UsbHcFreeMem (Ohc->MemPool, Ed, sizeof (ED_DESCRIPTOR));
  }

UNMAP_OHCI_XBUFF:
  DmaUnmap (Mapping);

EXIT:
  return Status;
}

/**
  Submits an asynchronous interrupt transfer to an interrupt endpoint of a USB device.
  Translator parameter doesn't exist in UEFI2.0 spec, but it will be updated in the following specification version.

  @param  This                A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  DeviceAddress       Represents the address of the target device on the USB.
  @param  EndPointAddress     The combination of an endpoint number and an endpoint direction of the
                              target USB device.
  @param  DeviceSpeed         Indicates device speed.
  @param  MaximumPacketLength Indicates the maximum packet size the target endpoint is capable of
                              sending or receiving.
  @param  IsNewTransfer       If TRUE, an asynchronous interrupt pipe is built between the host and the
                              target interrupt endpoint. If FALSE, the specified asynchronous interrupt
                              pipe is canceled. If TRUE, and an interrupt transfer exists for the target
                              end point, then EFI_INVALID_PARAMETER is returned.
  @param  DataToggle          A pointer to the data toggle value.
  @param  PollingInterval     Indicates the interval, in milliseconds, that the asynchronous interrupt
                              transfer is polled.
  @param  DataLength          Indicates the length of data to be received at the rate specified by
                              PollingInterval from the target asynchronous interrupt endpoint.
  @param  Translator          A pointr to the transaction translator data.
  @param  CallBackFunction    The Callback function. This function is called at the rate specified by
                              PollingInterval.
  @param  Context             The context that is passed to the CallBackFunction. This is an
                              optional parameter and may be NULL.

  @retval EFI_SUCCESS           The asynchronous interrupt transfer request has been successfully
                                submitted or canceled.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The request could not be completed due to a lack of resources.

**/
EFI_STATUS
EFIAPI
OhciAsyncInterruptTransfer (
  IN     EFI_USB2_HC_PROTOCOL                *This,
  IN     UINT8                               DeviceAddress,
  IN     UINT8                               EndPointAddress,
  IN     UINT8                               DeviceSpeed,
  IN     UINTN                               MaximumPacketLength,
  IN     BOOLEAN                             IsNewTransfer,
  IN OUT UINT8                               *DataToggle,
  IN     UINTN                               PollingInterval  OPTIONAL,
  IN     UINTN                               DataLength       OPTIONAL,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR  *Translator      OPTIONAL,
  IN     EFI_ASYNC_USB_TRANSFER_CALLBACK     CallBackFunction OPTIONAL,
  IN     VOID                                *Context         OPTIONAL
  )
{
  EFI_STATUS       Status;
  USB_OHCI_HC_DEV  *Ohc;
  VOID             *UCBuffer;
  UINTN            Pages;
  BOOLEAN          IsSlowDevice;

  if ((DataToggle == NULL) || ((EndPointAddress & 0x80) == 0) ||
      (IsNewTransfer && ((DataLength == 0) ||
                         ((*DataToggle != 0) && (*DataToggle != 1)) || ((PollingInterval < 1) || (PollingInterval > 255)))))
  {
    return EFI_INVALID_PARAMETER;
  }

  Ohc = USB_OHCI_HC_DEV_FROM_THIS (This);

  IsSlowDevice = (DeviceSpeed == EFI_USB_SPEED_LOW);

  if ( IsNewTransfer ) {
    Pages  = EFI_SIZE_TO_PAGES (DataLength);
    Status = DmaAllocateBuffer (
               EfiBootServicesData,
               Pages,
               &UCBuffer
               );
    if (EFI_ERROR (Status)) {
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    UCBuffer = NULL;
  }

  Status = OhciInterruptTransfer (
             Ohc,
             DeviceAddress,
             EndPointAddress,
             IsSlowDevice,
             MaximumPacketLength,
             IsNewTransfer,
             DataToggle,
             PollingInterval,
             UCBuffer,
             DataLength,
             CallBackFunction,
             Context,
             TRUE,
             NULL,
             NULL
             );

  if ( UCBuffer ) {
    if (EFI_ERROR (Status)) {
      DmaFreeBuffer (Pages, UCBuffer);
    }
  }

  return Status;
}

/**
  Submits synchronous interrupt transfer to an interrupt endpoint of a USB device.
  Translator parameter doesn't exist in UEFI2.0 spec, but it will be updated in the following specification version.

  @param  This                  A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  DeviceAddress         Represents the address of the target device on the USB.
  @param  EndPointAddress       The combination of an endpoint number and an endpoint direction of the
                                target USB device.
  @param  DeviceSpeed           Indicates device speed.
  @param  MaximumPacketLength   Indicates the maximum packet size the target endpoint is capable of
                                sending or receiving.
  @param  Data                  A pointer to the buffer of data that will be transmitted to USB device or
                                received from USB device.
  @param  DataLength            On input, the size, in bytes, of the data buffer specified by Data. On
                                output, the number of bytes transferred.
  @param  DataToggle            A pointer to the data toggle value.
  @param  TimeOut               Indicates the maximum time, in milliseconds, which the transfer is
                                allowed to complete.
  @param  Translator            A pointr to the transaction translator data.
  @param  TransferResult        A pointer to the detailed result information from the synchronous
                                interrupt transfer.

  @retval EFI_SUCCESS           The synchronous interrupt transfer was completed successfully.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The synchronous interrupt transfer could not be submitted due to a lack of resources.
  @retval EFI_TIMEOUT           The synchronous interrupt transfer failed due to timeout.
  @retval EFI_DEVICE_ERROR      The synchronous interrupt transfer failed due to host controller or device error.
                                Caller should check TransferResult for detailed error information.

**/
EFI_STATUS
EFIAPI
OhciSyncInterruptTransfer (
  IN     EFI_USB2_HC_PROTOCOL                *This,
  IN     UINT8                               DeviceAddress,
  IN     UINT8                               EndPointAddress,
  IN     UINT8                               DeviceSpeed,
  IN     UINTN                               MaximumPacketLength,
  IN OUT VOID                                *Data,
  IN OUT UINTN                               *DataLength,
  IN OUT UINT8                               *DataToggle,
  IN     UINTN                               TimeOut,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR  *Translator,
  OUT    UINT32                              *TransferResult
  )
{
  USB_OHCI_HC_DEV  *Ohc;
  EFI_STATUS       Status;
  ED_DESCRIPTOR    *Ed;
  TD_DESCRIPTOR    *HeadTd;
  OHCI_ED_RESULT   EdResult;
  VOID             *UCBuffer;
  BOOLEAN          IsSlowDevice;

  IsSlowDevice = (DeviceSpeed == EFI_USB_SPEED_LOW);

  if (((EndPointAddress & 0x80) == 0) || (Data == NULL) || (DataLength == NULL) || (*DataLength == 0) ||
      (IsSlowDevice && (MaximumPacketLength > 8)) || (!IsSlowDevice && (MaximumPacketLength > 64)) ||
      (DataToggle == NULL) || ((*DataToggle != 0) && (*DataToggle != 1)) || (TransferResult == NULL))
  {
    return EFI_INVALID_PARAMETER;
  }

  Ohc      = USB_OHCI_HC_DEV_FROM_THIS (This);
  UCBuffer = AllocatePool (*DataLength);
  if (UCBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = OhciInterruptTransfer (
             Ohc,
             DeviceAddress,
             EndPointAddress,
             IsSlowDevice,
             MaximumPacketLength,
             TRUE,
             DataToggle,
             1,
             UCBuffer,
             *DataLength,
             NULL,
             NULL,
             FALSE,
             &Ed,
             &HeadTd
             );

  if (!EFI_ERROR (Status)) {
    Status = CheckIfDone (Ohc, INTERRUPT_LIST, Ed, HeadTd, &EdResult);
    while (Status == EFI_NOT_READY && TimeOut > 0) {
      gBS->Stall (1000);
      TimeOut--;
      Status = CheckIfDone (Ohc, INTERRUPT_LIST, Ed, HeadTd, &EdResult);
    }

    *TransferResult = ConvertErrorCode (EdResult.ErrorCode);
  }

  CopyMem (Data, UCBuffer, *DataLength);
  Status = OhciInterruptTransfer (
             Ohc,
             DeviceAddress,
             EndPointAddress,
             IsSlowDevice,
             MaximumPacketLength,
             FALSE,
             DataToggle,
             0,
             NULL,
             0,
             NULL,
             NULL,
             FALSE,
             NULL,
             NULL
             );

  return Status;
}

/**
  Submits isochronous transfer to an isochronous endpoint of a USB device.

  This function is used to submit isochronous transfer to a target endpoint of a USB device.
  The target endpoint is specified by DeviceAddressand EndpointAddress. Isochronous transfers are
  used when working with isochronous date. It provides periodic, continuous communication between
  the host and a device. Isochronous transfers can beused only by full-speed, high-speed, and
  super-speed devices.

  High-speed isochronous transfers can be performed using multiple data buffers. The number of
  buffers that are actually prepared for the transfer is specified by DataBuffersNumber. For
  full-speed isochronous transfers this value is ignored.

  Data represents a list of pointers to the data buffers. For full-speed isochronous transfers
  only the data pointed by Data[0]shall be used. For high-speed isochronous transfers and for
  the split transactions depending on DataLengththere several data buffers canbe used. For the
  high-speed isochronous transfers the total number of buffers must not exceed EFI_USB_MAX_ISO_BUFFER_NUM.

  For split transactions performed on full-speed device by high-speed host controller the total
  number of buffers is limited to EFI_USB_MAX_ISO_BUFFER_NUM1.
  If the isochronous transfer is successful, then EFI_SUCCESSis returned. The isochronous transfer
  is designed to be completed within one USB frame time, if it cannot be completed, EFI_TIMEOUT
  is returned. If an error other than timeout occurs during the USB transfer, then EFI_DEVICE_ERROR
  is returned and the detailed status code will be returned in TransferResult.

  EFI_INVALID_PARAMETERis returned if one of the following conditionsis satisfied:
    - Data is NULL.
    - DataLength is 0.
    - DeviceSpeed is not one of the supported values listed above.
    - MaximumPacketLength is invalid. MaximumPacketLength must be 1023 or less for full-speed devices,
      and 1024 or less for high-speed and super-speed devices.
    - TransferResult is NULL.

  @param  This                  A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  DeviceAddress         Represents the address of the target device on the USB.
  @param  EndPointAddress       The combination of an endpoint number and an endpoint direction of the
                                target USB device.
  @param  DeviceSpeed           Indicates device speed. The supported values are EFI_USB_SPEED_FULL,
                                EFI_USB_SPEED_HIGH, or EFI_USB_SPEED_SUPER.
  @param  MaximumPacketLength   Indicates the maximum packet size the target endpoint is capable of
                                sending or receiving.
  @param  DataBuffersNumber     Number of data buffers prepared for the transfer.
  @param  Data                  Array of pointers to the buffers of data that will be transmitted to USB
                                device or received from USB device.
  @param  DataLength            Specifies the length, in bytes, of the data to be sent to or received from
                                the USB device.
  @param  Translator            A pointer to the transaction translator data.
  @param  TransferResult        A pointer to the detailed result information of the isochronous transfer.

  @retval EFI_SUCCESS           The isochronous transfer was completed successfully.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The isochronous transfer could not be submitted due to a lack of resources.
  @retval EFI_TIMEOUT           The isochronous transfer cannot be completed within the one USB frame time.
  @retval EFI_DEVICE_ERROR      The isochronous transfer failed due to host controller or device error.
                                Caller should check TransferResult for detailed error information.

**/
EFI_STATUS
EFIAPI
OhciIsochronousTransfer (
  IN     EFI_USB2_HC_PROTOCOL                *This,
  IN     UINT8                               DeviceAddress,
  IN     UINT8                               EndPointAddress,
  IN     UINT8                               DeviceSpeed,
  IN     UINTN                               MaximumPacketLength,
  IN     UINT8                               DataBuffersNumber,
  IN OUT VOID                                *Data[EFI_USB_MAX_ISO_BUFFER_NUM],
  IN     UINTN                               DataLength,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR  *Translator,
  OUT    UINT32                              *TransferResult
  )
{
  if ((Data == NULL) || (DataLength == 0) || (TransferResult == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_UNSUPPORTED;
}

/**
  Submits nonblocking isochronous transfer to an isochronous endpoint of a USB device.

  This is an asynchronous type of USB isochronous transfer. If the caller submits a USB
  isochronous transfer request through this function, this function will return immediately.

  When the isochronous transfer completes, the IsochronousCallbackfunction will be triggered,
  the caller can know the transfer results. If the transfer is successful, the caller can get
  the data received or sent in this callback function.

  The target endpoint is specified by DeviceAddressand EndpointAddress. Isochronous transfers
  are used when working with isochronous date. It provides periodic, continuous communication
  between the host and a device. Isochronous transfers can be used only by full-speed, high-speed,
  and super-speed devices.

  High-speed isochronous transfers can be performed using multiple data buffers. The number of
  buffers that are actually prepared for the transfer is specified by DataBuffersNumber. For
  full-speed isochronous transfers this value is ignored.

  Data represents a list of pointers to the data buffers. For full-speed isochronous transfers
  only the data pointed by Data[0] shall be used. For high-speed isochronous transfers and for
  the split transactions depending on DataLength there several data buffers can be used. For
  the high-speed isochronous transfers the total number of buffers must not exceed EFI_USB_MAX_ISO_BUFFER_NUM.

  For split transactions performed on full-speed device by high-speed host controller the total
  number of buffers is limited to EFI_USB_MAX_ISO_BUFFER_NUM1.

  EFI_INVALID_PARAMETER is returned if one of the following conditionsis satisfied:
    - Data is NULL.
    - DataLength is 0.
    - DeviceSpeed is not one of the supported values listed above.
    - MaximumPacketLength is invalid. MaximumPacketLength must be 1023 or less for full-speed
      devices and 1024 or less for high-speed and super-speed devices.

  @param  This                  A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  DeviceAddress         Represents the address of the target device on the USB.
  @param  EndPointAddress       The combination of an endpoint number and an endpoint direction of the
                                target USB device.
  @param  DeviceSpeed           Indicates device speed. The supported values are EFI_USB_SPEED_FULL,
                                EFI_USB_SPEED_HIGH, or EFI_USB_SPEED_SUPER.
  @param  MaximumPacketLength   Indicates the maximum packet size the target endpoint is capable of
                                sending or receiving.
  @param  DataBuffersNumber     Number of data buffers prepared for the transfer.
  @param  Data                  Array of pointers to the buffers of data that will be transmitted to USB
                                device or received from USB device.
  @param  DataLength            Specifies the length, in bytes, of the data to be sent to or received from
                                the USB device.
  @param  Translator            A pointer to the transaction translator data.
  @param  IsochronousCallback   The Callback function. This function is called if the requested
                                isochronous transfer is completed.
  @param  Context               Data passed to the IsochronousCallback function. This is an
                                optional parameter and may be NULL.

  @retval EFI_SUCCESS           The asynchronous isochronous transfer request has been successfully
                                submitted or canceled.
  @retval EFI_INVALID_PARAMETER Some parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The asynchronous isochronous transfer could not be submitted due to
                                a lack of resources.

**/
EFI_STATUS
EFIAPI
OhciAsyncIsochronousTransfer (
  IN     EFI_USB2_HC_PROTOCOL                *This,
  IN     UINT8                               DeviceAddress,
  IN     UINT8                               EndPointAddress,
  IN     UINT8                               DeviceSpeed,
  IN     UINTN                               MaximumPacketLength,
  IN     UINT8                               DataBuffersNumber,
  IN OUT VOID                                *Data[EFI_USB_MAX_ISO_BUFFER_NUM],
  IN     UINTN                               DataLength,
  IN     EFI_USB2_HC_TRANSACTION_TRANSLATOR  *Translator,
  IN     EFI_ASYNC_USB_TRANSFER_CALLBACK     IsochronousCallBack,
  IN     VOID                                *Context OPTIONAL
  )
{
  if ((Data == NULL) || (DataLength == 0)) {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_UNSUPPORTED;
}

/**
  Retrieves the current status of a USB root hub port.

  @param  This       A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  PortNumber Specifies the root hub port from which the status is to be retrieved.
                     This value is zero based.
  @param  PortStatus A pointer to the current port status bits and port status change bits.

  @retval EFI_SUCCESS           The status of the USB root hub port specified by PortNumber
                                was returned in PortStatus.
  @retval EFI_INVALID_PARAMETER PortNumber is invalid.

**/
EFI_STATUS
EFIAPI
OhciGetRootHubPortStatus (
  IN        EFI_USB2_HC_PROTOCOL  *This,
  IN        UINT8                 PortNumber,
  OUT       EFI_USB_PORT_STATUS   *PortStatus
  )
{
  USB_OHCI_HC_DEV  *Ohc;
  UINT8            NumOfPorts;

  Ohc = USB_OHCI_HC_DEV_FROM_THIS (This);

  OhciGetCapability (This, NULL, &NumOfPorts, NULL);
  if (PortNumber >= NumOfPorts) {
    return EFI_INVALID_PARAMETER;
  }

  PortStatus->PortStatus       = 0;
  PortStatus->PortChangeStatus = 0;

  if (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_CURR_CONNECT_STAT)) {
    PortStatus->PortStatus |= USB_PORT_STAT_CONNECTION;
  }

  if (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_ENABLE_STAT)) {
    PortStatus->PortStatus |= USB_PORT_STAT_ENABLE;
  }

  if (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_SUSPEND_STAT)) {
    PortStatus->PortStatus |= USB_PORT_STAT_SUSPEND;
  }

  if (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_OC_INDICATOR)) {
    PortStatus->PortStatus |= USB_PORT_STAT_OVERCURRENT;
  }

  if (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_RESET_STAT)) {
    PortStatus->PortStatus |= USB_PORT_STAT_RESET;
  }

  if (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_POWER_STAT)) {
    PortStatus->PortStatus |= USB_PORT_STAT_POWER;
  }

  if (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_LSDEVICE_ATTACHED)) {
    PortStatus->PortStatus |= USB_PORT_STAT_LOW_SPEED;
  }

  if (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_ENABLE_STAT_CHANGE)) {
    PortStatus->PortChangeStatus |= USB_PORT_STAT_C_ENABLE;
  }

  if (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_CONNECT_STATUS_CHANGE)) {
    PortStatus->PortChangeStatus |= USB_PORT_STAT_C_CONNECTION;
  }

  if (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_SUSPEND_STAT_CHANGE)) {
    PortStatus->PortChangeStatus |= USB_PORT_STAT_C_SUSPEND;
  }

  if (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_OC_INDICATOR_CHANGE)) {
    PortStatus->PortChangeStatus |= USB_PORT_STAT_C_OVERCURRENT;
  }

  if (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_RESET_STAT_CHANGE)) {
    PortStatus->PortChangeStatus |= USB_PORT_STAT_C_RESET;
  }

  return EFI_SUCCESS;
}

/**
  Sets a feature for the specified root hub port.

  @param  This        A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  PortNumber  Specifies the root hub port whose feature is requested to be set. This
                      value is zero based.
  @param  PortFeature Indicates the feature selector associated with the feature set request.

  @retval EFI_SUCCESS           The feature specified by PortFeature was set for the USB
                                root hub port specified by PortNumber.
  @retval EFI_INVALID_PARAMETER PortNumber is invalid or PortFeature is invalid for this function.

**/
EFI_STATUS
EFIAPI
OhciSetRootHubPortFeature (
  IN EFI_USB2_HC_PROTOCOL  *This,
  IN UINT8                 PortNumber,
  IN EFI_USB_PORT_FEATURE  PortFeature
  )
{
  USB_OHCI_HC_DEV  *Ohc;
  EFI_STATUS       Status;
  UINT8            NumOfPorts;
  UINTN            RetryTimes;

  OhciGetCapability (This, NULL, &NumOfPorts, NULL);
  if (PortNumber >= NumOfPorts) {
    return EFI_INVALID_PARAMETER;
  }

  Ohc = USB_OHCI_HC_DEV_FROM_THIS (This);

  Status = EFI_SUCCESS;

  switch (PortFeature) {
    case EfiUsbPortPower:
      Status = OhciSetRootHubPortStatus (Ohc, PortNumber, RH_SET_PORT_POWER);

      //
      // Verify the state
      //
      RetryTimes = 0;
      do {
        gBS->Stall (1000);
        RetryTimes++;
      } while (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_POWER_STAT) == 0 &&
               RetryTimes < MAX_RETRY_TIMES);

      if (RetryTimes >= MAX_RETRY_TIMES) {
        return EFI_DEVICE_ERROR;
      }

      break;

    case EfiUsbPortReset:
      Status = OhciSetRootHubPortStatus (Ohc, PortNumber, RH_SET_PORT_RESET);

      //
      // Verify the state
      //
      RetryTimes = 0;
      do {
        gBS->Stall (1000);
        RetryTimes++;
      } while ((OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_RESET_STAT_CHANGE) == 0 ||
                OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_RESET_STAT) == 1) &&
               RetryTimes < MAX_RETRY_TIMES);

      if (RetryTimes >= MAX_RETRY_TIMES) {
        return EFI_DEVICE_ERROR;
      }

      OhciSetRootHubPortStatus (Ohc, PortNumber, RH_PORT_RESET_STAT_CHANGE);
      break;

    case EfiUsbPortEnable:
      Status = OhciSetRootHubPortStatus (Ohc, PortNumber, RH_SET_PORT_ENABLE);

      //
      // Verify the state
      //
      RetryTimes = 0;
      do {
        gBS->Stall (1000);
        RetryTimes++;
      } while (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_ENABLE_STAT) == 0 &&
               RetryTimes < MAX_RETRY_TIMES);

      if (RetryTimes >= MAX_RETRY_TIMES) {
        return EFI_DEVICE_ERROR;
      }

      break;

    case EfiUsbPortSuspend:
      Status = OhciSetRootHubPortStatus (Ohc, PortNumber, RH_SET_PORT_SUSPEND);

      //
      // Verify the state
      //
      RetryTimes = 0;
      do {
        gBS->Stall (1000);
        RetryTimes++;
      } while (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_SUSPEND_STAT) == 0 &&
               RetryTimes < MAX_RETRY_TIMES);

      if (RetryTimes >= MAX_RETRY_TIMES) {
        return EFI_DEVICE_ERROR;
      }

      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  return Status;
}

/**
  Clears a feature for the specified root hub port.

  @param  This        A pointer to the EFI_USB2_HC_PROTOCOL instance.
  @param  PortNumber  Specifies the root hub port whose feature is requested to be cleared. This
                      value is zero based.
  @param  PortFeature Indicates the feature selector associated with the feature clear request.

  @retval EFI_SUCCESS           The feature specified by PortFeature was cleared for the USB
                                root hub port specified by PortNumber.
  @retval EFI_INVALID_PARAMETER PortNumber is invalid or PortFeature is invalid for this function.

**/
EFI_STATUS
EFIAPI
OhciClearRootHubPortFeature (
  IN EFI_USB2_HC_PROTOCOL  *This,
  IN UINT8                 PortNumber,
  IN EFI_USB_PORT_FEATURE  PortFeature
  )
{
  USB_OHCI_HC_DEV  *Ohc;
  EFI_STATUS       Status;
  UINT8            NumOfPorts;
  UINTN            RetryTimes;

  OhciGetCapability (This, NULL, &NumOfPorts, NULL);
  if (PortNumber >= NumOfPorts) {
    return EFI_INVALID_PARAMETER;
  }

  Ohc = USB_OHCI_HC_DEV_FROM_THIS (This);

  Status = EFI_SUCCESS;

  switch (PortFeature) {
    case EfiUsbPortEnable:
      Status = OhciSetRootHubPortStatus (Ohc, PortNumber, RH_CLEAR_PORT_ENABLE);

      //
      // Verify the state
      //
      RetryTimes = 0;
      do {
        gBS->Stall (1000);
        RetryTimes++;
      } while (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_ENABLE_STAT) == 1 &&
               RetryTimes < MAX_RETRY_TIMES);

      if (RetryTimes >= MAX_RETRY_TIMES) {
        return EFI_DEVICE_ERROR;
      }

      break;

    case EfiUsbPortSuspend:
      Status = OhciSetRootHubPortStatus (Ohc, PortNumber, RH_CLEAR_SUSPEND_STATUS);

      //
      // Verify the state
      //
      RetryTimes = 0;
      do {
        gBS->Stall (1000);
        RetryTimes++;
      } while (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_SUSPEND_STAT) == 1 &&
               RetryTimes < MAX_RETRY_TIMES);

      if (RetryTimes >= MAX_RETRY_TIMES) {
        return EFI_DEVICE_ERROR;
      }

      break;

    case EfiUsbPortReset:
      break;

    case EfiUsbPortPower:
      Status = OhciSetRootHubPortStatus (Ohc, PortNumber, RH_CLEAR_PORT_POWER);

      //
      // Verify the state
      //
      RetryTimes = 0;
      do {
        gBS->Stall (1000);
        RetryTimes++;
      } while (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_POWER_STAT) == 1 &&
               RetryTimes < MAX_RETRY_TIMES);

      if (RetryTimes >= MAX_RETRY_TIMES) {
        return EFI_DEVICE_ERROR;
      }

      break;

    case EfiUsbPortConnectChange:
      Status = OhciSetRootHubPortStatus (Ohc, PortNumber, RH_CONNECT_STATUS_CHANGE);

      //
      // Verify the state
      //
      RetryTimes = 0;
      do {
        gBS->Stall (1000);
        RetryTimes++;
      } while (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_CONNECT_STATUS_CHANGE) == 1 &&
               RetryTimes < MAX_RETRY_TIMES);

      if (RetryTimes >= MAX_RETRY_TIMES) {
        return EFI_DEVICE_ERROR;
      }

      break;

    case EfiUsbPortResetChange:
      Status = OhciSetRootHubPortStatus (Ohc, PortNumber, RH_PORT_RESET_STAT_CHANGE);

      //
      // Verify the state
      //
      RetryTimes = 0;
      do {
        gBS->Stall (1000);
        RetryTimes++;
      } while (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_RESET_STAT_CHANGE) == 1 &&
               RetryTimes < MAX_RETRY_TIMES);

      if (RetryTimes >= MAX_RETRY_TIMES) {
        return EFI_DEVICE_ERROR;
      }

      break;

    case EfiUsbPortEnableChange:
      Status = OhciSetRootHubPortStatus (Ohc, PortNumber, RH_PORT_ENABLE_STAT_CHANGE);

      //
      // Verify the state
      //
      RetryTimes = 0;
      do {
        gBS->Stall (1000);
        RetryTimes++;
      } while (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_ENABLE_STAT_CHANGE) == 1 &&
               RetryTimes < MAX_RETRY_TIMES);

      if (RetryTimes >= MAX_RETRY_TIMES) {
        return EFI_DEVICE_ERROR;
      }

      break;

    case EfiUsbPortSuspendChange:
      Status = OhciSetRootHubPortStatus (Ohc, PortNumber, RH_PORT_SUSPEND_STAT_CHANGE);

      //
      // Verify the state
      //
      RetryTimes = 0;
      do {
        gBS->Stall (1000);
        RetryTimes++;
      } while (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_PORT_SUSPEND_STAT_CHANGE) == 1 &&
               RetryTimes < MAX_RETRY_TIMES);

      if (RetryTimes >= MAX_RETRY_TIMES) {
        return EFI_DEVICE_ERROR;
      }

      break;

    case EfiUsbPortOverCurrentChange:
      Status = OhciSetRootHubPortStatus (Ohc, PortNumber, RH_OC_INDICATOR_CHANGE);

      //
      // Verify the state
      //
      RetryTimes = 0;
      do {
        gBS->Stall (1000);
        RetryTimes++;
      } while (OhciReadRootHubPortStatus (Ohc, PortNumber, RH_OC_INDICATOR_CHANGE) == 1 &&
               RetryTimes < MAX_RETRY_TIMES);

      if (RetryTimes >= MAX_RETRY_TIMES) {
        return EFI_DEVICE_ERROR;
      }

      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  return Status;
}

EFI_STATUS
OhcInitHC (
  IN USB_OHCI_HC_DEV  *Ohc
  )
{
  return OhciReset (
           &Ohc->Usb2Hc,
           EFI_USB_HC_RESET_GLOBAL | EFI_USB_HC_RESET_HOST_CONTROLLER
           );
}

/**

  Free the OHCI device and release its associated resources.

  @param  Ohc                   The OHCI device to release.

**/
VOID
OhciFreeDev (
  IN USB_OHCI_HC_DEV  *Ohc
  )
{
  OhciFreeFixedIntMemory (Ohc);

  if (Ohc->HouseKeeperTimer != NULL) {
    gBS->CloseEvent (Ohc->HouseKeeperTimer);
  }

  if (Ohc->ExitBootServiceEvent != NULL) {
    gBS->CloseEvent (Ohc->ExitBootServiceEvent);
  }

  if (Ohc->MemPool != NULL) {
    UsbHcFreeMemPool (Ohc->MemPool);
  }

  if (Ohc->HccaMemoryMapping != NULL ) {
    DmaFreeBuffer (Ohc->HccaMemoryPages, Ohc->HccaMemoryBuf);
  }

  if (Ohc->ControllerNameTable != NULL) {
    FreeUnicodeStringTable (Ohc->ControllerNameTable);
  }

  FreePool (Ohc);
}

/**

  Uninstall all Ohci Interface.

  @param  Controller            Controller handle.
  @param  This                  Protocol instance pointer.

**/
VOID
OhciCleanDevUp (
  IN  EFI_HANDLE            Controller,
  IN  EFI_USB2_HC_PROTOCOL  *This
  )
{
  USB_OHCI_HC_DEV  *Ohc;

  //
  // Retrieve private context structure
  //
  Ohc = USB_OHCI_HC_DEV_FROM_THIS (This);

  //
  // Uninstall the USB_HC and USB_HC2 protocol
  //
  gBS->UninstallProtocolInterface (
         Controller,
         &gEfiUsb2HcProtocolGuid,
         &Ohc->Usb2Hc
         );

  //
  // Cancel the timer event
  //
  gBS->SetTimer (Ohc->HouseKeeperTimer, TimerCancel, 0);

  //
  // Stop the host controller
  //
  OhciSetHcControl (Ohc, PERIODIC_ENABLE | CONTROL_ENABLE | ISOCHRONOUS_ENABLE | BULK_ENABLE, 0);
  This->Reset (This, EFI_USB_HC_RESET_GLOBAL);
  This->SetState (This, EfiUsbHcStateHalt);

  //
  // Free resources
  //
  OhciFreeDynamicIntMemory (Ohc);

  //
  // Free the private context structure
  //
  OhciFreeDev (Ohc);
}

/**

  One notified function to stop the Host Controller when gBS->ExitBootServices() called.

  @param  Event                 Pointer to this event
  @param  Context               Event handler private data
**/
VOID
EFIAPI
OhcExitBootService (
  EFI_EVENT  Event,
  VOID       *Context
  )
{
  USB_OHCI_HC_DEV       *Ohc;
  EFI_USB2_HC_PROTOCOL  *Usb2Hc;

  Ohc = (USB_OHCI_HC_DEV *)Context;

  Usb2Hc = &Ohc->Usb2Hc;
  //
  // Stop the Host Controller
  //
  // OhciStopHc (Ohc, OHC_GENERIC_TIMEOUT);
  OhciSetHcControl (Ohc, PERIODIC_ENABLE | CONTROL_ENABLE | ISOCHRONOUS_ENABLE | BULK_ENABLE, 0);
  Usb2Hc->Reset (Usb2Hc, EFI_USB_HC_RESET_GLOBAL);
  Usb2Hc->SetState (Usb2Hc, EfiUsbHcStateHalt);

  return;
}

/**
  Test to see if this driver supports ControllerHandle. Any
  ControllerHandle that has UsbHcProtocol installed will be supported.

  @param  This                 Protocol instance pointer.
  @param  Controller           Handle of device to test.
  @param  RemainingDevicePath  Not used.

  @return EFI_SUCCESS          This driver supports this device.
  @return EFI_UNSUPPORTED      This driver does not support this device.

**/
EFI_STATUS
EFIAPI
OHCIDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath
  )
{
  OHCI_DEVICE_PROTOCOL  *Dev;
  EFI_STATUS            Status;

  //
  //  Connect to the non-discoverable device
  //
  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gOhciDeviceProtocolGuid,
                  (VOID **)&Dev,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Clean up.
  //
  gBS->CloseProtocol (
         ControllerHandle,
         &gOhciDeviceProtocolGuid,
         This->DriverBindingHandle,
         ControllerHandle
         );

  return EFI_SUCCESS;
}

/**

  Allocate and initialize the empty OHCI device.

  @param  OriginalPciAttributes  The original PCI attributes.

  @return Allocated OHCI device  If err, return NULL.

**/
EFI_STATUS
EFIAPI
OHCIDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath   OPTIONAL
  )
{
  USB_OHCI_HC_DEV       *Ohc;
  EFI_STATUS            Status;
  VOID                  *Buf;
  EFI_PHYSICAL_ADDRESS  PhyAddr;
  VOID                  *Map;
  UINTN                 Pages;
  UINTN                 Bytes;

  Ohc = AllocateZeroPool (sizeof (USB_OHCI_HC_DEV));
  if (Ohc == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gOhciDeviceProtocolGuid,
                  (VOID **)&Ohc->Protocol,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Couldn't open protocol: %r\n",
      __FUNCTION__,
      Status
      ));
    goto FREE_OHC;
  }

  Ohc->Signature = USB_OHCI_HC_DEV_SIGNATURE;

  Ohc->Usb2Hc.GetCapability            = OhciGetCapability;
  Ohc->Usb2Hc.Reset                    = OhciReset;
  Ohc->Usb2Hc.GetState                 = OhciGetState;
  Ohc->Usb2Hc.SetState                 = OhciSetState;
  Ohc->Usb2Hc.ControlTransfer          = OhciControlTransfer;
  Ohc->Usb2Hc.BulkTransfer             = OhciBulkTransfer;
  Ohc->Usb2Hc.AsyncInterruptTransfer   = OhciAsyncInterruptTransfer;
  Ohc->Usb2Hc.SyncInterruptTransfer    = OhciSyncInterruptTransfer;
  Ohc->Usb2Hc.IsochronousTransfer      = OhciIsochronousTransfer;
  Ohc->Usb2Hc.AsyncIsochronousTransfer = OhciAsyncIsochronousTransfer;
  Ohc->Usb2Hc.GetRootHubPortStatus     = OhciGetRootHubPortStatus;
  Ohc->Usb2Hc.SetRootHubPortFeature    = OhciSetRootHubPortFeature;
  Ohc->Usb2Hc.ClearRootHubPortFeature  = OhciClearRootHubPortFeature;
  Ohc->Usb2Hc.MajorRevision            = 0x1;
  Ohc->Usb2Hc.MinorRevision            = 0x1;

  Ohc->UsbHcBaseAddress     = Ohc->Protocol->BaseAddress;
  Ohc->HccaMemoryBlock      = NULL;
  Ohc->HccaMemoryMapping    = NULL;
  Ohc->HccaMemoryBuf        = NULL;
  Ohc->HccaMemoryPages      = 0;
  Ohc->InterruptContextList = NULL;
  Ohc->ControllerNameTable  = NULL;
  Ohc->HouseKeeperTimer     = NULL;

  Ohc->MemPool = UsbHcInitMemPool (TRUE, 0);
  if (Ohc->MemPool == NULL) {
    goto FREE_DEV_BUFFER;
  }

  Bytes = 4096;
  Pages = EFI_SIZE_TO_PAGES (Bytes);

  Status = DmaAllocateBuffer (
             EfiBootServicesData,
             Pages,
             &Buf
             );

  if (EFI_ERROR (Status)) {
    goto FREE_MEM_POOL;
  }

  Status = DmaMap (
             MapOperationBusMasterCommonBuffer,
             Buf,
             &Bytes,
             &PhyAddr,
             &Map
             );

  if (EFI_ERROR (Status) || (Bytes != 4096)) {
    goto FREE_MEM_PAGE;
  }

  Ohc->HccaMemoryBlock   = (HCCA_MEMORY_BLOCK *)(UINTN)PhyAddr;
  Ohc->HccaMemoryMapping = Map;
  Ohc->HccaMemoryBuf     = (VOID *)(UINTN)Buf;
  Ohc->HccaMemoryPages   = Pages;

  //
  // Install Host Controller Protocol
  //
  Status = gBS->InstallProtocolInterface (
                  &ControllerHandle,
                  &gEfiUsb2HcProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &Ohc->Usb2Hc
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Install protocol error"));
    goto FREE_OHC;
  }

  //
  // Create event to stop the HC on exit boot services.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  OhcExitBootService,
                  Ohc,
                  &gEfiEventExitBootServicesGuid,
                  &Ohc->ExitBootServiceEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Create exit boot event error"));
    goto UNINSTALL_USBHC;
  }

  //
  // Set 0.01 s timer
  //
  Status = gBS->CreateEvent (
                  EVT_TIMER | EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  OhciHouseKeeper,
                  Ohc,
                  &Ohc->HouseKeeperTimer
                  );
  if (EFI_ERROR (Status)) {
    goto FREE_OHC;
  }

  Status = OhcInitHC (Ohc);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "failed to init ohci host controller\n"));
    goto UNINSTALL_USBHC;
  }

  Status = gBS->SetTimer (Ohc->HouseKeeperTimer, TimerPeriodic, 10 * 1000 * 10);
  if (EFI_ERROR (Status)) {
    goto FREE_OHC;
  }

  DEBUG ((
    DEBUG_INFO,
    "OHCI started for controller @ %p, base address: 0x%p\n",
    ControllerHandle,
    Ohc->UsbHcBaseAddress
    ));
  return EFI_SUCCESS;

FREE_OHC:
  OhciFreeDev (Ohc);
UNINSTALL_USBHC:
  gBS->UninstallMultipleProtocolInterfaces (
         ControllerHandle,
         &gEfiUsb2HcProtocolGuid,
         &Ohc->Usb2Hc,
         NULL
         );
  gBS->CloseProtocol (
         ControllerHandle,
         &gOhciDeviceProtocolGuid,
         This->DriverBindingHandle,
         ControllerHandle
         );
FREE_MEM_PAGE:
  DmaFreeBuffer (Pages, Buf);
FREE_MEM_POOL:
  UsbHcFreeMemPool (Ohc->MemPool);
FREE_DEV_BUFFER:
  FreePool (Ohc);

  return Status;
}

/**
  Stop this driver on ControllerHandle. Support stopping any child handles
  created by this driver.

  @param  This                  Protocol instance pointer.
  @param  Controller            Handle of device to stop driver on.
  @param  NumberOfChildren      Number of Children in the ChildHandleBuffer.
  @param  ChildHandleBuffer     List of handles for the children we need to stop.

  @return EFI_SUCCESS
  @return others

**/
EFI_STATUS
EFIAPI
OHCIDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   Controller,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer
  )
{
  EFI_STATUS            Status;
  EFI_USB2_HC_PROTOCOL  *Usb2Hc;

  Status = gBS->OpenProtocol (
                  Controller,
                  &gEfiUsb2HcProtocolGuid,
                  (VOID **)&Usb2Hc,
                  This->DriverBindingHandle,
                  Controller,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  OhciCleanDevUp (Controller, Usb2Hc);

  gBS->CloseProtocol (
         Controller,
         &gOhciDeviceProtocolGuid,
         This->DriverBindingHandle,
         Controller
         );
  return EFI_SUCCESS;
}

EFI_DRIVER_BINDING_PROTOCOL  gOhciDriverBinding = {
  OHCIDriverBindingSupported,
  OHCIDriverBindingStart,
  OHCIDriverBindingStop,
  0x10,
  NULL,
  NULL
};

EFI_STATUS
EFIAPI
OhciInitialise (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return EfiLibInstallDriverBindingComponentName2 (
           ImageHandle,
           SystemTable,
           &gOhciDriverBinding,
           ImageHandle,
           &gOhciComponentName,
           &gOhciComponentName2
           );
}
