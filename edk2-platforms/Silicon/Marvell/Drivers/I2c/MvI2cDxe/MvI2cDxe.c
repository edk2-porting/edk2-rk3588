/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include <Protocol/BoardDesc.h>
#include <Protocol/I2cMaster.h>
#include <Protocol/I2cEnumerate.h>
#include <Protocol/I2cBusConfigurationManagement.h>
#include <Protocol/DevicePath.h>
#include <Protocol/MvI2c.h>

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "MvI2cDxe.h"

STATIC MV_I2C_BAUD_RATE baud_rate;

STATIC MV_I2C_DEVICE_PATH MvI2cDevicePathProtocol = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(OFFSET_OF (MV_I2C_DEVICE_PATH, End)),
        (UINT8)(OFFSET_OF (MV_I2C_DEVICE_PATH, End) >> 8),
      },
    },
    EFI_CALLER_ID_GUID
  },
  0,  // Instance
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      sizeof(EFI_DEVICE_PATH_PROTOCOL),
      0
    }
  }
};

STATIC
UINT32
I2C_READ(
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN off)
{
  ASSERT (I2cMasterContext != NULL);
  return MmioRead32 (I2cMasterContext->BaseAddress + off);
}

STATIC
EFI_STATUS
I2C_WRITE (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN off,
  IN UINT32 Value)
{
  ASSERT (I2cMasterContext != NULL);
  return MmioWrite32 (I2cMasterContext->BaseAddress + off, Value);
}

EFI_STATUS
EFIAPI
MvI2cInitialiseController (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable,
  IN EFI_PHYSICAL_ADDRESS BaseAddress
  )
{
  EFI_STATUS Status;
  I2C_MASTER_CONTEXT *I2cMasterContext;
  STATIC INTN Bus = 0;
  MV_I2C_DEVICE_PATH *DevicePath;

  DevicePath = AllocateCopyPool (sizeof(MvI2cDevicePathProtocol),
                                 &MvI2cDevicePathProtocol);
  if (DevicePath == NULL) {
    DEBUG((DEBUG_ERROR, "MvI2cDxe: I2C device path allocation failed\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  DevicePath->Instance = Bus;

  /* if attachment succeeds, this gets freed at ExitBootServices */
  I2cMasterContext = AllocateZeroPool (sizeof (I2C_MASTER_CONTEXT));
  if (I2cMasterContext == NULL) {
    DEBUG((DEBUG_ERROR, "MvI2cDxe: I2C master context allocation failed\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  I2cMasterContext->Signature = I2C_MASTER_SIGNATURE;
  I2cMasterContext->I2cMaster.Reset = MvI2cReset;
  I2cMasterContext->I2cMaster.StartRequest = MvI2cStartRequest;
  I2cMasterContext->I2cEnumerate.Enumerate = MvI2cEnumerate;
  I2cMasterContext->I2cBusConf.EnableI2cBusConfiguration = MvI2cEnableConf;
  I2cMasterContext->TclkFrequency = PcdGet32 (PcdI2cClockFrequency);
  I2cMasterContext->BaseAddress = BaseAddress;
  I2cMasterContext->Bus = Bus;
  /* I2cMasterContext->Lock is responsible for serializing I2C operations */
  EfiInitializeLock(&I2cMasterContext->Lock, TPL_NOTIFY);

  MvI2cCalBaudRate( I2cMasterContext,
                    PcdGet32 (PcdI2cBaudRate),
                    &baud_rate,
                    I2cMasterContext->TclkFrequency
                  );

  Status = gBS->InstallMultipleProtocolInterfaces(
      &I2cMasterContext->Controller,
      &gEfiI2cMasterProtocolGuid,
      &I2cMasterContext->I2cMaster,
      &gEfiI2cEnumerateProtocolGuid,
      &I2cMasterContext->I2cEnumerate,
      &gEfiI2cBusConfigurationManagementProtocolGuid,
      &I2cMasterContext->I2cBusConf,
      &gEfiDevicePathProtocolGuid,
      (EFI_DEVICE_PATH_PROTOCOL *) DevicePath,
      NULL);

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "MvI2cDxe: Installing protocol interfaces failed!\n"));
    goto fail;
  }
  DEBUG((DEBUG_ERROR, "Succesfully installed controller %d at 0x%llx\n", Bus,
        I2cMasterContext->BaseAddress));

  Bus++;

  return EFI_SUCCESS;

fail:
  FreePool(I2cMasterContext);
  return Status;
}

STATIC
VOID
EFIAPI
OnEndOfDxe (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  MV_I2C_DEVICE_PATH        *DevicePath;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePathPointer;
  EFI_HANDLE                DeviceHandle;
  EFI_STATUS                Status;

  gBS->CloseEvent (Event);

  DevicePath = AllocateCopyPool (sizeof (MvI2cDevicePathProtocol),
                 &MvI2cDevicePathProtocol);
  if (DevicePath == NULL) {
    DEBUG ((DEBUG_ERROR, "MvI2cDxe: I2C device path allocation failed\n"));
    return;
  }

  do {
    DevicePathPointer = (EFI_DEVICE_PATH_PROTOCOL *)DevicePath;
    Status = gBS->LocateDevicePath (&gEfiI2cMasterProtocolGuid,
                    &DevicePathPointer,
                    &DeviceHandle);
    if (EFI_ERROR (Status)) {
      break;
    }

    Status = gBS->ConnectController (DeviceHandle, NULL, NULL, TRUE);
    DEBUG ((DEBUG_INFO,
      "%a: ConnectController () returned %r\n",
      __FUNCTION__,
      Status));

    DevicePath->Instance++;
  } while (TRUE);

  gBS->FreePool (DevicePath);
}


EFI_STATUS
EFIAPI
MvI2cInitialise (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  MARVELL_BOARD_DESC_PROTOCOL *BoardDescProtocol;
  EFI_EVENT EndOfDxeEvent;
  MV_BOARD_I2C_DESC *Desc;
  EFI_STATUS Status;
  UINTN Index;

  /* Obtain list of available controllers */
  Status = gBS->LocateProtocol (&gMarvellBoardDescProtocolGuid,
                  NULL,
                  (VOID **)&BoardDescProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = BoardDescProtocol->BoardDescI2cGet (BoardDescProtocol, &Desc);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /* Initialize enabled chips */
  for (Index = 0; Index < Desc->I2cDevCount; Index++) {
    Status = MvI2cInitialiseController(
        ImageHandle,
        SystemTable,
        Desc[Index].SoC->I2cBaseAddress
        );
    if (EFI_ERROR(Status))
      return Status;
  }

  BoardDescProtocol->BoardDescFree (Desc);

  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  OnEndOfDxe,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

STATIC
VOID
MvI2cControlClear (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT32 Mask)
{
  UINT32 Value;

  /* clears given bits in I2C_CONTROL register */
  Value = I2C_READ(I2cMasterContext, I2C_CONTROL);
  Value &= ~Mask;
  I2C_WRITE(I2cMasterContext, I2C_CONTROL, Value);
}

STATIC
VOID
MvI2cControlSet (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT32 Mask)
{
  UINT32 Value;

  /* sets given bits in I2C_CONTROL register */
  Value = I2C_READ(I2cMasterContext, I2C_CONTROL);
  Value |= Mask;
  I2C_WRITE(I2cMasterContext, I2C_CONTROL, Value);
}

STATIC
VOID
MvI2cClearIflg (
 IN I2C_MASTER_CONTEXT *I2cMasterContext
 )
{
  MvI2cPollCtrl (I2cMasterContext, I2C_OPERATION_TIMEOUT, I2C_CONTROL_IFLG);
  MvI2cControlClear(I2cMasterContext, I2C_CONTROL_IFLG);
}

/* Timeout is given in us */
STATIC
UINTN
MvI2cPollCtrl (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINTN Timeout,
  IN UINT32 Mask)
{
  Timeout /= 10;
  while (!(I2C_READ(I2cMasterContext, I2C_CONTROL) & Mask)) {
    gBS->Stall(10);
    if (--Timeout == 0)
      return (Timeout);
  }
  return (0);
}

/*
 * 'Timeout' is given in us. Note also that Timeout handling is not exact --
 * MvI2cLockedStart() total wait can be more than 2 x Timeout
 * (MvI2cPollCtrl() is called twice). 'Mask' can be either I2C_STATUS_START
 * or I2C_STATUS_RPTD_START
 */
STATIC
EFI_STATUS
MvI2cLockedStart (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN INT32 Mask,
  IN UINT8 Slave,
  IN UINTN Timeout
  )
{
  UINTN ReadAccess, IflgSet = 0;
  UINT32 I2cStatus;

  if (Mask == I2C_STATUS_RPTD_START) {
    /* read IFLG to know if it should be cleared later */
    IflgSet = I2C_READ(I2cMasterContext, I2C_CONTROL) & I2C_CONTROL_IFLG;
  }

  MvI2cControlSet(I2cMasterContext, I2C_CONTROL_START);

  if (Mask == I2C_STATUS_RPTD_START && IflgSet) {
    DEBUG((DEBUG_INFO, "MvI2cDxe: IFLG set, clearing\n"));
    MvI2cClearIflg(I2cMasterContext);
  }

  if (MvI2cPollCtrl(I2cMasterContext, Timeout, I2C_CONTROL_IFLG)) {
    DEBUG((DEBUG_ERROR, "MvI2cDxe: Timeout sending %sSTART condition\n",
        Mask == I2C_STATUS_START ? "" : "repeated "));
    return EFI_NO_RESPONSE;
  }

  I2cStatus = I2C_READ(I2cMasterContext, I2C_STATUS);
  if (I2cStatus != Mask) {
    DEBUG((DEBUG_ERROR, "MvI2cDxe: wrong I2cStatus (%02x) after sending %sSTART condition\n",
        I2cStatus, Mask == I2C_STATUS_START ? "" : "repeated "));
    return EFI_DEVICE_ERROR;
  }

  I2C_WRITE(I2cMasterContext, I2C_DATA, Slave);
  gBS->Stall(I2C_OPERATION_TIMEOUT);
  MvI2cClearIflg(I2cMasterContext);

  if (MvI2cPollCtrl(I2cMasterContext, Timeout, I2C_CONTROL_IFLG)) {
    DEBUG((DEBUG_ERROR, "MvI2cDxe: Timeout sending Slave address\n"));
    return EFI_NO_RESPONSE;
  }

  ReadAccess = (Slave & 0x1) ? 1 : 0;
  I2cStatus = I2C_READ(I2cMasterContext, I2C_STATUS);
  if (I2cStatus != (ReadAccess ?
      I2C_STATUS_ADDR_R_ACK : I2C_STATUS_ADDR_W_ACK)) {
    DEBUG((DEBUG_ERROR, "MvI2cDxe: no ACK (I2cStatus: %02x) after sending Slave address\n",
        I2cStatus));
    return EFI_NO_RESPONSE;
  }

  return EFI_SUCCESS;
}

#define  ABSSUB(a,b)  (((a) > (b)) ? (a) - (b) : (b) - (a))
STATIC
VOID
MvI2cCalBaudRate (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN CONST UINT32 target,
  IN OUT MV_I2C_BAUD_RATE *rate,
  UINT32 clk
  )
{
  UINT32 cur, diff, diff0, baud;
  UINTN m, n, m0, n0;

  /* Read initial m0, n0 values from register */
  baud = I2C_READ(I2cMasterContext, I2C_BAUD_RATE);
  m0 = I2C_M_FROM_BAUD(baud);
  n0 = I2C_N_FROM_BAUD(baud);
  /* Calculate baud rate. */
  diff0 = 0xffffffff;

  for (n = 0; n < 8; n++) {
    for (m = 0; m < 16; m++) {
      cur = I2C_BAUD_RATE_RAW(clk,m,n);
      diff = ABSSUB(target, cur);
      if (diff < diff0) {
        m0 = m;
        n0 = n;
        diff0 = diff;
      }
    }
  }
  rate->raw = I2C_BAUD_RATE_RAW(clk, m0, n0);
  rate->param = I2C_BAUD_RATE_PARAM(m0, n0);
  rate->m = m0;
  rate->n = n0;
}

EFI_STATUS
EFIAPI
MvI2cReset (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This
  )
{
  UINT32 param;
  I2C_MASTER_CONTEXT *I2cMasterContext = I2C_SC_FROM_MASTER(This);

  param = baud_rate.param;

  EfiAcquireLock (&I2cMasterContext->Lock);
  I2C_WRITE(I2cMasterContext, I2C_SOFT_RESET, 0x0);
  gBS->Stall(2 * I2C_OPERATION_TIMEOUT);
  I2C_WRITE(I2cMasterContext, I2C_BAUD_RATE, param);
  I2C_WRITE(I2cMasterContext, I2C_CONTROL, I2C_CONTROL_I2CEN | I2C_CONTROL_ACK);
  gBS->Stall(I2C_OPERATION_TIMEOUT);
  EfiReleaseLock (&I2cMasterContext->Lock);

  return EFI_SUCCESS;
}

/*
 * Timeout is given in us
 */
STATIC
EFI_STATUS
MvI2cRepeatedStart (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT8 Slave,
  IN UINTN Timeout
  )
{
  EFI_STATUS Status;

  EfiAcquireLock (&I2cMasterContext->Lock);
  Status = MvI2cLockedStart(I2cMasterContext, I2C_STATUS_RPTD_START, Slave,
      Timeout);
  EfiReleaseLock (&I2cMasterContext->Lock);

  if (EFI_ERROR(Status)) {
    MvI2cStop(I2cMasterContext);
  }
  return Status;
}

/*
 * Timeout is given in us
 */
STATIC
EFI_STATUS
MvI2cStart (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN UINT8 Slave,
  IN UINTN Timeout
  )
{
  EFI_STATUS Status;

  EfiAcquireLock (&I2cMasterContext->Lock);
  Status = MvI2cLockedStart(I2cMasterContext, I2C_STATUS_START, Slave, Timeout);
  EfiReleaseLock (&I2cMasterContext->Lock);

  if (EFI_ERROR(Status)) {
    MvI2cStop(I2cMasterContext);
  }
  return Status;
}

STATIC
EFI_STATUS
MvI2cStop (
  IN I2C_MASTER_CONTEXT *I2cMasterContext
  )
{
  EfiAcquireLock (&I2cMasterContext->Lock);
  MvI2cControlSet(I2cMasterContext, I2C_CONTROL_STOP);
  gBS->Stall(I2C_OPERATION_TIMEOUT);
  MvI2cClearIflg(I2cMasterContext);
  EfiReleaseLock (&I2cMasterContext->Lock);

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvI2cRead (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN OUT UINT8 *Buf,
  IN UINTN Length,
  IN OUT UINTN *read,
  IN UINTN last,
  IN UINTN delay
  )
{
  UINT32 I2cStatus;
  UINTN LastByte;
  EFI_STATUS Status;

  EfiAcquireLock (&I2cMasterContext->Lock);
  *read = 0;
  while (*read < Length) {
    /*
     * Check if we are reading last byte of the last Buffer,
     * do not send ACK then, per I2C specs
     */
    LastByte = ((*read == Length - 1) && last) ? 1 : 0;
    if (LastByte)
      MvI2cControlClear(I2cMasterContext, I2C_CONTROL_ACK);
    else
      MvI2cControlSet(I2cMasterContext, I2C_CONTROL_ACK);

    gBS->Stall (I2C_OPERATION_TIMEOUT);
    MvI2cClearIflg(I2cMasterContext);

    if (MvI2cPollCtrl(I2cMasterContext, delay, I2C_CONTROL_IFLG)) {
      DEBUG((DEBUG_ERROR, "MvI2cDxe: Timeout reading data\n"));
      Status = EFI_NO_RESPONSE;
      goto out;
    }

    I2cStatus = I2C_READ(I2cMasterContext, I2C_STATUS);
    if (I2cStatus != (LastByte ?
        I2C_STATUS_DATA_RD_NOACK : I2C_STATUS_DATA_RD_ACK)) {
      DEBUG((DEBUG_ERROR, "MvI2cDxe: wrong I2cStatus (%02x) while reading\n", I2cStatus));
      Status = EFI_DEVICE_ERROR;
      goto out;
    }

    *Buf++ = I2C_READ(I2cMasterContext, I2C_DATA);
    (*read)++;
  }
  Status = EFI_SUCCESS;
out:
  EfiReleaseLock (&I2cMasterContext->Lock);
  return (Status);
}

STATIC
EFI_STATUS
MvI2cWrite (
  IN I2C_MASTER_CONTEXT *I2cMasterContext,
  IN OUT CONST UINT8 *Buf,
  IN UINTN Length,
  IN OUT UINTN *Sent,
  IN UINTN Timeout
  )
{
  UINT32 status;
  EFI_STATUS Status;

  EfiAcquireLock (&I2cMasterContext->Lock);
  *Sent = 0;
  while (*Sent < Length) {
    I2C_WRITE(I2cMasterContext, I2C_DATA, *Buf++);

    MvI2cClearIflg(I2cMasterContext);
    if (MvI2cPollCtrl(I2cMasterContext, Timeout, I2C_CONTROL_IFLG)) {
      DEBUG((DEBUG_ERROR, "MvI2cDxe: Timeout writing data\n"));
      Status = EFI_NO_RESPONSE;
      goto out;
    }

    status = I2C_READ(I2cMasterContext, I2C_STATUS);
    if (status != I2C_STATUS_DATA_WR_ACK) {
      DEBUG((DEBUG_ERROR, "MvI2cDxe: wrong status (%02x) while writing\n", status));
      Status = EFI_DEVICE_ERROR;
      goto out;
    }
    (*Sent)++;
  }
  Status = EFI_SUCCESS;
out:
  EfiReleaseLock (&I2cMasterContext->Lock);
  return (Status);
}

/*
 * MvI2cStartRequest should be called only by I2cHost.
 * I2C device drivers ought to use EFI_I2C_IO_PROTOCOL instead.
 */
STATIC
EFI_STATUS
MvI2cStartRequest (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This,
  IN UINTN                         SlaveAddress,
  IN EFI_I2C_REQUEST_PACKET        *RequestPacket,
  IN EFI_EVENT                     Event      OPTIONAL,
  OUT EFI_STATUS                   *I2cStatus OPTIONAL
  )
{
  UINTN Count;
  UINTN ReadMode;
  UINTN Transmitted;
  I2C_MASTER_CONTEXT *I2cMasterContext = I2C_SC_FROM_MASTER(This);
  EFI_I2C_OPERATION *Operation;
  EFI_STATUS Status = EFI_SUCCESS;

  ASSERT (RequestPacket != NULL);
  ASSERT (I2cMasterContext != NULL);

  for (Count = 0; Count < RequestPacket->OperationCount; Count++) {
    Operation = &RequestPacket->Operation[Count];
    ReadMode = Operation->Flags & I2C_FLAG_READ;

    if (Count == 0) {
      Status = MvI2cStart (I2cMasterContext,
                 (SlaveAddress << 1) | ReadMode,
                 I2C_TRANSFER_TIMEOUT);
    } else if (!(Operation->Flags & I2C_FLAG_NORESTART)) {
      Status = MvI2cRepeatedStart (I2cMasterContext,
                 (SlaveAddress << 1) | ReadMode,
                 I2C_TRANSFER_TIMEOUT);
    }

    /* I2C transaction was aborted, so stop further transactions */
    if (EFI_ERROR (Status)) {
      MvI2cStop (I2cMasterContext);
      break;
    }

    /*
     * If sending the slave address was successful,
     * proceed to read or write section.
     */
    if (ReadMode) {
      Status = MvI2cRead (I2cMasterContext,
                 Operation->Buffer,
                 Operation->LengthInBytes,
                 &Transmitted,
                 Count == 1,
                 I2C_TRANSFER_TIMEOUT);
      Operation->LengthInBytes = Transmitted;
    } else {
      Status = MvI2cWrite (I2cMasterContext,
                 Operation->Buffer,
                 Operation->LengthInBytes,
                 &Transmitted,
                 I2C_TRANSFER_TIMEOUT);
      Operation->LengthInBytes = Transmitted;
    }

    /*
     * The I2C read or write transaction failed.
     * Stop the I2C transaction.
     */
    if (EFI_ERROR (Status)) {
      MvI2cStop (I2cMasterContext);
      break;
    }

    /* Check if there is any more data to be sent */
    if (Count == RequestPacket->OperationCount - 1) {
      MvI2cStop ( I2cMasterContext );
    }
  }

  if (I2cStatus != NULL)
    *I2cStatus = EFI_SUCCESS;
  if (Event != NULL)
    gBS->SignalEvent(Event);
  return EFI_SUCCESS;
}

STATIC CONST EFI_GUID DevGuid = I2C_GUID;

STATIC
EFI_STATUS
MvI2cAllocDevice (
  IN UINT8 SlaveAddress,
  IN UINT8 Bus,
  IN OUT CONST EFI_I2C_DEVICE **Device
  )
{
  EFI_STATUS Status;
  EFI_I2C_DEVICE *Dev;
  UINT32 *TmpSlaveArray;
  EFI_GUID *TmpGuidP;

  Status = gBS->AllocatePool ( EfiBootServicesData,
             sizeof(EFI_I2C_DEVICE),
             (VOID **) &Dev );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "MvI2cDxe: I2C device memory allocation failed\n"));
    return Status;
  }
  *Device = Dev;
  Dev->DeviceIndex = SlaveAddress;
  Dev->DeviceIndex = I2C_DEVICE_INDEX(Bus, SlaveAddress);
  Dev->SlaveAddressCount = 1;
  Dev->I2cBusConfiguration = 0;
  Status = gBS->AllocatePool ( EfiBootServicesData,
             sizeof(UINT32),
             (VOID **) &TmpSlaveArray);
  if (EFI_ERROR(Status)) {
    goto fail1;
  }
  TmpSlaveArray[0] = SlaveAddress;
  Dev->SlaveAddressArray = TmpSlaveArray;

  Status = gBS->AllocatePool ( EfiBootServicesData,
             sizeof(EFI_GUID),
             (VOID **) &TmpGuidP);
  if (EFI_ERROR(Status)) {
    goto fail2;
  }
  *TmpGuidP = DevGuid;
  Dev->DeviceGuid = TmpGuidP;

  DEBUG((DEBUG_INFO, "MvI2c: allocated device with address %x\n", (UINTN)SlaveAddress));
  return EFI_SUCCESS;

fail2:
  FreePool(TmpSlaveArray);
fail1:
  FreePool(Dev);

  return Status;
}

/*
 * It is called by I2cBus to enumerate devices on I2C bus. In this case,
 * enumeration is based on PCD configuration - all Slave addresses specified
 * in PCD get their corresponding EFI_I2C_DEVICE structures here.
 *
 * After enumeration succeeds, Supported() function of drivers that installed
 * DriverBinding protocol is called.
 */
STATIC
EFI_STATUS
EFIAPI
MvI2cEnumerate (
  IN CONST EFI_I2C_ENUMERATE_PROTOCOL *This,
  IN OUT CONST EFI_I2C_DEVICE         **Device
  )
{
  UINT8 *DevicesPcd;
  UINT8 *DeviceBusPcd;
  UINTN Index, NextIndex, DevCount;
  UINT8 NextDeviceAddress;
  I2C_MASTER_CONTEXT *I2cMasterContext = I2C_SC_FROM_ENUMERATE(This);

  DevCount = PcdGetSize (PcdI2cSlaveAddresses);
  DevicesPcd = PcdGetPtr (PcdI2cSlaveAddresses);
  DeviceBusPcd = PcdGetPtr (PcdI2cSlaveBuses);
  if (*Device == NULL) {
    for (Index = 0; Index < DevCount ; Index++) {
      if (DeviceBusPcd[Index] != I2cMasterContext->Bus)
        continue;
      if (Index < DevCount)
        MvI2cAllocDevice (DevicesPcd[Index], I2cMasterContext->Bus, Device);
      return EFI_SUCCESS;
    }
  } else {
    /* Device is not NULL, so something was already allocated */
    for (Index = 0; Index < DevCount; Index++) {
      if (DeviceBusPcd[Index] != I2cMasterContext->Bus)
        continue;
      if (DevicesPcd[Index] == I2C_DEVICE_ADDRESS((*Device)->DeviceIndex)) {
        for (NextIndex = Index + 1; NextIndex < DevCount; NextIndex++) {
          if (DeviceBusPcd[NextIndex] != I2cMasterContext->Bus)
            continue;
          NextDeviceAddress = DevicesPcd[NextIndex];
          if (NextIndex < DevCount)
            MvI2cAllocDevice(NextDeviceAddress, I2cMasterContext->Bus, Device);
          return EFI_SUCCESS;
        }
      }
    }
    *Device = NULL;
    return EFI_SUCCESS;
  }
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
MvI2cEnableConf (
  IN CONST EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL *This,
  IN UINTN                                               I2cBusConfiguration,
  IN EFI_EVENT                                           Event      OPTIONAL,
  IN EFI_STATUS                                          *I2cStatus OPTIONAL
  )
{
  /* do nothing */
  if (I2cStatus != NULL)
    I2cStatus = EFI_SUCCESS;
  if (Event != NULL)
    gBS->SignalEvent(Event);
  return EFI_SUCCESS;
}
