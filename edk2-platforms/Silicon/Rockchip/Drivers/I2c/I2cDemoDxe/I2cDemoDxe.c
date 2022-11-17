/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include <Protocol/DriverBinding.h>
#include <Protocol/I2cIo.h>
#include <Protocol/I2cDemo.h>
#include <Protocol/I2c.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Pi/PiI2c.h>

#include "I2cDemoDxe.h"

STATIC CONST EFI_GUID I2cGuid = I2C_GUID;

EFI_DRIVER_BINDING_PROTOCOL gDriverBindingProtocol = {
  I2cDemoSupported,
  I2cDemoStart,
  I2cDemoStop
};

EFI_STATUS
EFIAPI
I2cDemoSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS Status = EFI_UNSUPPORTED;
  EFI_I2C_IO_PROTOCOL *TmpI2cIo;
  UINT8 *I2cDemoAddresses;
  UINT8 *I2cDemoBuses;
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
    DEBUG ((DEBUG_INFO, "I2cDemoSupported Error status: %d\n", Status));
    return EFI_UNSUPPORTED;
  }

  /* get I2CDEMO devices' addresses from PCD */
  I2cDemoAddresses = PcdGetPtr (PcdI2cDemoAddresses);
  I2cDemoBuses = PcdGetPtr (PcdI2cDemoBuses);
  if (I2cDemoAddresses == 0) {
    Status = EFI_UNSUPPORTED;
    DEBUG((DEBUG_INFO, "I2cDemoSupported: I2C device found, but it's not I2CDEMO\n"));
    goto out;
  }

  Status = EFI_UNSUPPORTED;
  for (i = 0; I2cDemoAddresses[i] != '\0'; i++) {
    /* I2C guid must fit and valid DeviceIndex must be provided */
    if (CompareGuid(TmpI2cIo->DeviceGuid, &I2cGuid) &&
        TmpI2cIo->DeviceIndex == I2C_DEVICE_INDEX(I2cDemoBuses[i],
          I2cDemoAddresses[i])) {
      DEBUG ((DEBUG_INFO,  "I2cDemoSupported: attached to I2CDEMO device\n"));
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
I2cDemoRead (
  IN CONST ROCKCHIP_I2CDEMO_PROTOCOL *This,
  IN UINT8 *RegAddress,
  IN UINT16 RegAddressLength,
  IN UINT8 *Buffer,
  IN UINT16 Length
  )
{
  EFI_I2C_REQUEST_PACKET *RequestPacket;
  UINTN RequestPacketSize;
  EFI_STATUS Status = EFI_SUCCESS;
  I2CDEMO_CONTEXT *I2cDemoContext = I2CDEMO_SC_FROM_I2CDEMO(This);

  ASSERT(I2cDemoContext != NULL);
  ASSERT(I2cDemoContext->I2cIo != NULL);

  RequestPacketSize = sizeof(UINTN) + sizeof (EFI_I2C_OPERATION) * 2;
  RequestPacket = AllocateZeroPool (RequestPacketSize);
  if (RequestPacket == NULL)
    return EFI_OUT_OF_RESOURCES;

  RequestPacket->OperationCount = 2;

  RequestPacket->Operation[0].Flags = 0;
  RequestPacket->Operation[0].LengthInBytes = RegAddressLength;
  RequestPacket->Operation[0].Buffer = RegAddress;

  RequestPacket->Operation[1].Flags = I2C_FLAG_READ;
  RequestPacket->Operation[1].LengthInBytes = Length;
  RequestPacket->Operation[1].Buffer = Buffer;

  Status = I2cDemoContext->I2cIo->QueueRequest(I2cDemoContext->I2cIo, 0, NULL, RequestPacket, NULL);
  if (EFI_ERROR(Status))
    DEBUG((DEBUG_INFO, "I2cDemoTransfer: error %d during transmission\n", Status));

  FreePool(RequestPacket);

  return Status;
}

EFI_STATUS
EFIAPI
I2cDemoWrite (
  IN CONST ROCKCHIP_I2CDEMO_PROTOCOL *This,
  IN UINT8 *RegAddress,
  IN UINT16 RegAddressLength,
  IN UINT8 *Buffer,
  IN UINT16 Length
  )
{
  EFI_I2C_REQUEST_PACKET *RequestPacket;
  UINTN RequestPacketSize;
  EFI_STATUS Status = EFI_SUCCESS;
  I2CDEMO_CONTEXT *I2cDemoContext = I2CDEMO_SC_FROM_I2CDEMO(This);
  UINT8 *Data;
  UINT16 i;

  ASSERT(I2cDemoContext != NULL);
  ASSERT(I2cDemoContext->I2cIo != NULL);

  RequestPacketSize = sizeof(UINTN) + sizeof (EFI_I2C_OPERATION);
  RequestPacket = AllocateZeroPool (RequestPacketSize);
  if (RequestPacket == NULL)
    return EFI_OUT_OF_RESOURCES;

  Data = AllocateZeroPool ( RegAddressLength + Length );
  if (Data == NULL)
    return EFI_OUT_OF_RESOURCES;

  for (i = 0; i < RegAddressLength; i++)
    Data[i] = RegAddress[i]  & 0xff;

  for (i = RegAddressLength; i < RegAddressLength + Length; i++)
    Data[i] = Buffer[i - RegAddressLength]  & 0xff;

  RequestPacket->OperationCount = 1;

  RequestPacket->Operation[0].Flags = 0;
  RequestPacket->Operation[0].LengthInBytes = RegAddressLength + Length;
  RequestPacket->Operation[0].Buffer = Data;

  Status = I2cDemoContext->I2cIo->QueueRequest(I2cDemoContext->I2cIo, 0, NULL, RequestPacket, NULL);
  if (EFI_ERROR(Status))
    DEBUG((DEBUG_INFO, "I2cDemoTransfer: error %d during transmission\n", Status));

  FreePool(Data);
  FreePool(RequestPacket);

  return Status;
}

EFI_STATUS
EFIAPI
I2cDemoStart (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN EFI_HANDLE ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  I2CDEMO_CONTEXT *I2cDemoContext;

  I2cDemoContext = AllocateZeroPool (sizeof(I2CDEMO_CONTEXT));
  if (I2cDemoContext == NULL) {
    DEBUG((DEBUG_ERROR, "I2cDemo: allocation fail\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  I2cDemoContext->ControllerHandle = ControllerHandle;
  I2cDemoContext->Signature = I2CDEMO_SIGNATURE;
  I2cDemoContext->I2cDemoProtocol.Read = I2cDemoRead;
  I2cDemoContext->I2cDemoProtocol.Write = I2cDemoWrite;

  Status = gBS->OpenProtocol (
      ControllerHandle,
      &gEfiI2cIoProtocolGuid,
      (VOID **) &I2cDemoContext->I2cIo,
      gImageHandle,
      ControllerHandle,
      EFI_OPEN_PROTOCOL_BY_DRIVER
      );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "I2cDemo: failed to open I2cIo\n"));
    FreePool(I2cDemoContext);
    return EFI_UNSUPPORTED;
  }

  I2cDemoContext->I2cDemoProtocol.Identifier = I2cDemoContext->I2cIo->DeviceIndex;
  Status = gBS->InstallMultipleProtocolInterfaces (
      &ControllerHandle,
      &gRockchipI2cDemoProtocolGuid, &I2cDemoContext->I2cDemoProtocol,
      NULL
      );
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "I2cDemo: failed to install I2CDEMO protocol\n"));
    goto fail;
  }

  return Status;

fail:
  FreePool(I2cDemoContext);
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
I2cDemoStop (
  IN EFI_DRIVER_BINDING_PROTOCOL *This,
  IN  EFI_HANDLE ControllerHandle,
  IN  UINTN NumberOfChildren,
  IN  EFI_HANDLE *ChildHandleBuffer OPTIONAL
  )
{
  ROCKCHIP_I2CDEMO_PROTOCOL *I2cDemoProtocol;
  EFI_STATUS Status;
  I2CDEMO_CONTEXT *I2cDemoContext;

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gRockchipI2cDemoProtocolGuid,
                  (VOID **) &I2cDemoProtocol,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );

  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }
  I2cDemoContext = I2CDEMO_SC_FROM_I2CDEMO(I2cDemoProtocol);

  gBS->UninstallMultipleProtocolInterfaces (
      &ControllerHandle,
      &gRockchipI2cDemoProtocolGuid, &I2cDemoContext->I2cDemoProtocol,
      &gEfiDriverBindingProtocolGuid, &gDriverBindingProtocol,
      NULL
      );
  gBS->CloseProtocol (
      ControllerHandle,
      &gEfiI2cIoProtocolGuid,
      gImageHandle,
      ControllerHandle
      );
  FreePool(I2cDemoContext);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
I2cDemoInitialise (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
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
