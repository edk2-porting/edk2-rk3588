/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include <Protocol/DriverBinding.h>
#include <Protocol/I2cIo.h>
#include <Protocol/Eeprom.h>
#include <Protocol/MvI2c.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Pi/PiI2c.h>

#include "MvEepromDxe.h"

STATIC CONST EFI_GUID I2cGuid = I2C_GUID;

EFI_DRIVER_BINDING_PROTOCOL gDriverBindingProtocol = {
  MvEepromSupported,
  MvEepromStart,
  MvEepromStop
};

EFI_STATUS
EFIAPI
MvEepromSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL            *This,
  IN EFI_HANDLE                             ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL               *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS Status = EFI_UNSUPPORTED;
  EFI_I2C_IO_PROTOCOL *TmpI2cIo;
  UINT8 *EepromAddresses;
  UINT8 *EepromBuses;
  UINTN i;

  Status = gBS->OpenProtocol (
      ControllerHandle,
      &gEfiI2cIoProtocolGuid,
      (VOID **) &TmpI2cIo,
      gImageHandle,
      ControllerHandle,
      EFI_OPEN_PROTOCOL_BY_DRIVER
      );
  if (EFI_ERROR(Status)) {
    return EFI_UNSUPPORTED;
  }

  /* get EEPROM devices' addresses from PCD */
  EepromAddresses = PcdGetPtr (PcdEepromI2cAddresses);
  EepromBuses = PcdGetPtr (PcdEepromI2cBuses);
  if (EepromAddresses == 0) {
    Status = EFI_UNSUPPORTED;
    DEBUG((DEBUG_INFO, "MvEepromSupported: I2C device found, but it's not EEPROM\n"));
    goto out;
  }

  Status = EFI_UNSUPPORTED;
  for (i = 0; EepromAddresses[i] != '\0'; i++) {
    /* I2C guid must fit and valid DeviceIndex must be provided */
    if (CompareGuid(TmpI2cIo->DeviceGuid, &I2cGuid) &&
        TmpI2cIo->DeviceIndex == I2C_DEVICE_INDEX(EepromBuses[i],
          EepromAddresses[i])) {
      DEBUG((DEBUG_INFO, "MvEepromSupported: attached to EEPROM device\n"));
      Status = EFI_SUCCESS;
      break;
    }
  }

out:
  gBS->CloseProtocol (
      ControllerHandle,
      &gEfiI2cIoProtocolGuid,
      gImageHandle,
      ControllerHandle
      );
  return Status;
}

EFI_STATUS
EFIAPI
MvEepromTransfer (
  IN CONST MARVELL_EEPROM_PROTOCOL *This,
  IN UINT16 Address,
  IN UINT32 Length,
  IN UINT8 *Buffer,
  IN UINT8 Operation
  )
{
  EFI_I2C_REQUEST_PACKET *RequestPacket;
  UINTN RequestPacketSize;
  EFI_STATUS Status = EFI_SUCCESS;
  EEPROM_CONTEXT *EepromContext = EEPROM_SC_FROM_EEPROM(This);
  UINT32 BufferLength;
  UINT32 Transmitted = 0;
  UINT32 CurrentAddress = Address;

  ASSERT(EepromContext != NULL);
  ASSERT(EepromContext->I2cIo != NULL);

  RequestPacketSize = sizeof(UINTN) + sizeof (EFI_I2C_OPERATION) * 2;
  RequestPacket = AllocateZeroPool (RequestPacketSize);
  if (RequestPacket == NULL)
    return EFI_OUT_OF_RESOURCES;
  /* First operation contains address, the second is buffer */
  RequestPacket->OperationCount = 2;
  RequestPacket->Operation[0].LengthInBytes = 2;
  RequestPacket->Operation[0].Buffer = AllocateZeroPool ( RequestPacket->Operation[0].LengthInBytes );
  if (RequestPacket->Operation[0].Buffer == NULL) {
    FreePool(RequestPacket);
    return EFI_OUT_OF_RESOURCES;
  }
  RequestPacket->Operation[1].Flags = (Operation == EEPROM_READ ? I2C_FLAG_READ : I2C_FLAG_NORESTART);

  while (Length > 0) {
    CurrentAddress = Address + Transmitted;
    BufferLength = (Length <= MAX_BUFFER_LENGTH ? Length : MAX_BUFFER_LENGTH);
    RequestPacket->Operation[0].Buffer[0] = (CurrentAddress >> 8) & 0xff;
    RequestPacket->Operation[0].Buffer[1] = CurrentAddress & 0xff;
    RequestPacket->Operation[1].LengthInBytes = BufferLength;
    RequestPacket->Operation[1].Buffer = Buffer + Transmitted;
    Status = EepromContext->I2cIo->QueueRequest(EepromContext->I2cIo, 0, NULL, RequestPacket, NULL);
    if (EFI_ERROR(Status)) {
      DEBUG((DEBUG_ERROR, "MvEepromTransfer: error %d during transmission\n", Status));
      break;
    }
    Length -= BufferLength;
    Transmitted += BufferLength;
  }

  FreePool(RequestPacket->Operation[0].Buffer);
  FreePool(RequestPacket);
  return Status;
}

EFI_STATUS
EFIAPI
MvEepromStart (
  IN EFI_DRIVER_BINDING_PROTOCOL            *This,
  IN EFI_HANDLE                             ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL               *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  EEPROM_CONTEXT *EepromContext;

  EepromContext = AllocateZeroPool (sizeof(EEPROM_CONTEXT));
  if (EepromContext == NULL) {
    DEBUG((DEBUG_ERROR, "MvEeprom: allocation fail\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  EepromContext->ControllerHandle = ControllerHandle;
  EepromContext->Signature = EEPROM_SIGNATURE;
  EepromContext->EepromProtocol.Transfer = MvEepromTransfer;

  Status = gBS->OpenProtocol (
      ControllerHandle,
      &gEfiI2cIoProtocolGuid,
      (VOID **) &EepromContext->I2cIo,
      gImageHandle,
      ControllerHandle,
      EFI_OPEN_PROTOCOL_BY_DRIVER
      );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "MvEeprom: failed to open I2cIo\n"));
    FreePool(EepromContext);
    return EFI_UNSUPPORTED;
  }

  EepromContext->EepromProtocol.Identifier = EepromContext->I2cIo->DeviceIndex;
  Status = gBS->InstallMultipleProtocolInterfaces (
      &ControllerHandle,
      &gMarvellEepromProtocolGuid, &EepromContext->EepromProtocol,
      NULL
      );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "MvEeprom: failed to install EEPROM protocol\n"));
    goto fail;
  }

  return Status;

fail:
  FreePool(EepromContext);
  gBS->CloseProtocol (
      ControllerHandle,
      &gEfiI2cIoProtocolGuid,
      gImageHandle,
      ControllerHandle
      );

  return Status;
}

EFI_STATUS
EFIAPI
MvEepromStop (
  IN EFI_DRIVER_BINDING_PROTOCOL            *This,
  IN  EFI_HANDLE                            ControllerHandle,
  IN  UINTN                                 NumberOfChildren,
  IN  EFI_HANDLE                            *ChildHandleBuffer OPTIONAL
  )
{
  MARVELL_EEPROM_PROTOCOL *EepromProtocol;
  EFI_STATUS Status;
  EEPROM_CONTEXT *EepromContext;

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gMarvellEepromProtocolGuid,
                  (VOID **) &EepromProtocol,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );

  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }
  EepromContext = EEPROM_SC_FROM_EEPROM(EepromProtocol);

  gBS->UninstallMultipleProtocolInterfaces (
      &ControllerHandle,
      &gMarvellEepromProtocolGuid, &EepromContext->EepromProtocol,
      &gEfiDriverBindingProtocolGuid, &gDriverBindingProtocol,
      NULL
      );
  gBS->CloseProtocol (
      ControllerHandle,
      &gEfiI2cIoProtocolGuid,
      gImageHandle,
      ControllerHandle
      );
  FreePool(EepromContext);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MvEepromInitialise (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;
  Status = gBS->InstallMultipleProtocolInterfaces (
      &ImageHandle,
      &gEfiDriverBindingProtocolGuid, &gDriverBindingProtocol,
      NULL
      );
  return Status;
}
