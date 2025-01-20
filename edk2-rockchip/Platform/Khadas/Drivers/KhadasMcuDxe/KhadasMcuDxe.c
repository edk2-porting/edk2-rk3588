/** @file
 *
 *  Khadas MCU platform driver
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Protocol/DriverBinding.h>
#include <Protocol/I2cIo.h>
#include <Protocol/I2c.h>
#include <Protocol/KhadasMcu.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Pi/PiI2c.h>

#include "KhadasMcuDxe.h"

STATIC CONST EFI_GUID  I2cGuid = I2C_GUID;

STATIC
EFI_STATUS
EFIAPI
KhadasMcuRead (
  IN CONST KHADAS_MCU_PROTOCOL  *This,
  IN UINT8                      *RegAddress,
  IN UINT16                     RegAddressLength,
  IN UINT8                      *Buffer,
  IN UINT16                     Length
  )
{
  EFI_STATUS              Status;
  EFI_I2C_REQUEST_PACKET  *RequestPacket;
  UINTN                   RequestPacketSize;
  KHADAS_MCU_CONTEXT      *KhadasMcuContext = KHADAS_MCU_FROM_PROTOCOL (This);

  ASSERT (KhadasMcuContext != NULL);
  ASSERT (KhadasMcuContext->I2cIo != NULL);

  RequestPacketSize = sizeof (UINTN) + sizeof (EFI_I2C_OPERATION) * 2;
  RequestPacket     = AllocateZeroPool (RequestPacketSize);
  if (RequestPacket == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  RequestPacket->OperationCount = 2;

  RequestPacket->Operation[0].Flags         = 0;
  RequestPacket->Operation[0].LengthInBytes = RegAddressLength;
  RequestPacket->Operation[0].Buffer        = RegAddress;

  RequestPacket->Operation[1].Flags         = I2C_FLAG_READ;
  RequestPacket->Operation[1].LengthInBytes = Length;
  RequestPacket->Operation[1].Buffer        = Buffer;

  Status = KhadasMcuContext->I2cIo->QueueRequest (KhadasMcuContext->I2cIo, 0, NULL, RequestPacket, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: error %d during transmission\n", __func__, Status));
  }

  FreePool (RequestPacket);

  return Status;
}

STATIC
EFI_STATUS
EFIAPI
KhadasMcuWrite (
  IN CONST KHADAS_MCU_PROTOCOL  *This,
  IN UINT8                      *RegAddress,
  IN UINT16                     RegAddressLength,
  IN UINT8                      *Buffer,
  IN UINT16                     Length
  )
{
  EFI_STATUS              Status;
  EFI_I2C_REQUEST_PACKET  *RequestPacket;
  UINTN                   RequestPacketSize;
  UINT8                   *Data;
  UINT16                  Index;
  KHADAS_MCU_CONTEXT      *KhadasMcuContext = KHADAS_MCU_FROM_PROTOCOL (This);

  ASSERT (KhadasMcuContext != NULL);
  ASSERT (KhadasMcuContext->I2cIo != NULL);

  RequestPacketSize = sizeof (UINTN) + sizeof (EFI_I2C_OPERATION);
  RequestPacket     = AllocateZeroPool (RequestPacketSize);
  if (RequestPacket == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Data = AllocateZeroPool (RegAddressLength + Length);
  if (Data == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < RegAddressLength; Index++) {
    Data[Index] = RegAddress[Index] & 0xff;
  }

  for (Index = RegAddressLength; Index < RegAddressLength + Length; Index++) {
    Data[Index] = Buffer[Index - RegAddressLength] & 0xff;
  }

  RequestPacket->OperationCount = 1;

  RequestPacket->Operation[0].Flags         = 0;
  RequestPacket->Operation[0].LengthInBytes = RegAddressLength + Length;
  RequestPacket->Operation[0].Buffer        = Data;

  Status = KhadasMcuContext->I2cIo->QueueRequest (KhadasMcuContext->I2cIo, 0, NULL, RequestPacket, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: error %d during transmission\n", __func__, Status));
  }

  FreePool (Data);
  FreePool (RequestPacket);

  return Status;
}

STATIC
EFI_STATUS
EFIAPI
KhadasMcuReadRegister (
  IN  CONST KHADAS_MCU_PROTOCOL  *This,
  IN  UINT8                      Address,
  OUT UINT8                      *Value
  )
{
  return KhadasMcuRead (This, &Address, sizeof (UINT8), Value, sizeof (UINT8));
}

STATIC
EFI_STATUS
EFIAPI
KhadasMcuWriteRegister (
  IN CONST KHADAS_MCU_PROTOCOL  *This,
  IN UINT8                      Address,
  IN UINT8                      Value
  )
{
  return KhadasMcuWrite (This, &Address, sizeof (UINT8), &Value, sizeof (UINT8));
}

STATIC
EFI_STATUS
EFIAPI
KhadasMcuSetFanSpeedPercentage (
  IN KHADAS_MCU_PROTOCOL  *This,
  IN UINT8                Percentage
  )
{
  return KhadasMcuWriteRegister (
           This,
           MCU_CMD_FAN_STATUS_CTRL_REGv2,
           MIN (Percentage, 100)
           );
}

EFI_DRIVER_BINDING_PROTOCOL  mDriverBindingProtocol = {
  KhadasMcuSupported,
  KhadasMcuStart,
  KhadasMcuStop
};

EFI_STATUS
EFIAPI
KhadasMcuSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS           Status;
  EFI_I2C_IO_PROTOCOL  *TmpI2cIo;
  UINT8                KhadasMcuAddress;
  UINT8                KhadasMcuBus;

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiI2cIoProtocolGuid,
                  (VOID **)&TmpI2cIo,
                  gImageHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  KhadasMcuAddress = PcdGet8 (PcdKhadasMcuAddress);
  KhadasMcuBus     = PcdGet8 (PcdKhadasMcuBus);

  Status = EFI_UNSUPPORTED;

  if (CompareGuid (TmpI2cIo->DeviceGuid, &I2cGuid) &&
      (TmpI2cIo->DeviceIndex == I2C_DEVICE_INDEX (KhadasMcuBus, KhadasMcuAddress)))
  {
    DEBUG ((DEBUG_INFO, "%a: attached to Khadas MCU device\n", __func__));
    Status = EFI_SUCCESS;
  }

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
KhadasMcuStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS           Status;
  KHADAS_MCU_CONTEXT   *KhadasMcuContext;
  KHADAS_MCU_PROTOCOL  *KhadasMcuProtocol;

  KhadasMcuContext = AllocateZeroPool (sizeof (KHADAS_MCU_CONTEXT));
  if (KhadasMcuContext == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: context allocation failed\n", __func__));
    return EFI_OUT_OF_RESOURCES;
  }

  KhadasMcuContext->ControllerHandle = ControllerHandle;
  KhadasMcuContext->Signature        = KHADAS_MCU_SIGNATURE;

  KhadasMcuProtocol                        = &KhadasMcuContext->KhadasMcuProtocol;
  KhadasMcuProtocol->SetFanSpeedPercentage = KhadasMcuSetFanSpeedPercentage;

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiI2cIoProtocolGuid,
                  (VOID **)&KhadasMcuContext->I2cIo,
                  gImageHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to open I2cIo\n", __func__));
    FreePool (KhadasMcuContext);
    return EFI_UNSUPPORTED;
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ControllerHandle,
                  &gKhadasMcuProtocolGuid,
                  KhadasMcuProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: failed to install KHADAS_MCU_PROTOCOL\n", __func__));
    goto fail;
  }

  return Status;

fail:
  FreePool (KhadasMcuContext);
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
KhadasMcuStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN UINTN                        NumberOfChildren,
  IN EFI_HANDLE                   *ChildHandleBuffer OPTIONAL
  )
{
  EFI_STATUS           Status;
  KHADAS_MCU_PROTOCOL  *KhadasMcuProtocol;
  KHADAS_MCU_CONTEXT   *KhadasMcuContext;

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gKhadasMcuProtocolGuid,
                  (VOID **)&KhadasMcuProtocol,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  KhadasMcuContext = KHADAS_MCU_FROM_PROTOCOL (KhadasMcuProtocol);

  gBS->UninstallMultipleProtocolInterfaces (
         &ControllerHandle,
         &gKhadasMcuProtocolGuid,
         &KhadasMcuContext->KhadasMcuProtocol,
         &gEfiDriverBindingProtocolGuid,
         &mDriverBindingProtocol,
         NULL
         );
  gBS->CloseProtocol (
         ControllerHandle,
         &gEfiI2cIoProtocolGuid,
         gImageHandle,
         ControllerHandle
         );
  FreePool (KhadasMcuContext);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
KhadasMcuDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiDriverBindingProtocolGuid,
                  &mDriverBindingProtocol,
                  NULL
                  );
  return Status;
}
