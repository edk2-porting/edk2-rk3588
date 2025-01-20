/** @file
*
*  Copyright (C) 2016 Marvell International Ltd.
*  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Protocol/DriverBinding.h>
#include <Protocol/I2cIo.h>
#include <Protocol/I2c.h>
#include <Protocol/Rk860xRegulator.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Pi/PiI2c.h>

#include "Rk860xRegulatorDxe.h"

STATIC CONST EFI_GUID  I2cGuid = I2C_GUID;

EFI_DRIVER_BINDING_PROTOCOL  gDriverBindingProtocol = {
  Rk860xRegulatorSupported,
  Rk860xRegulatorStart,
  Rk860xRegulatorStop
};

EFI_STATUS
EFIAPI
Rk860xRegulatorSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS           Status = EFI_UNSUPPORTED;
  EFI_I2C_IO_PROTOCOL  *TmpI2cIo;
  UINT8                *Rk860xRegulatorAddresses;
  UINT8                *Rk860xRegulatorBuses;
  UINTN                i;

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

  /* get RK860X_REGULATOR devices' addresses from PCD */
  Rk860xRegulatorAddresses = PcdGetPtr (PcdRk860xRegulatorAddresses);
  Rk860xRegulatorBuses     = PcdGetPtr (PcdRk860xRegulatorBuses);
  if (Rk860xRegulatorAddresses == 0) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_INFO, "Rk860xRegulatorSupported: I2C device found, but it's not RK860X_REGULATOR\n"));
    goto out;
  }

  Status = EFI_UNSUPPORTED;
  for (i = 0; Rk860xRegulatorAddresses[i] != '\0'; i++) {
    /* I2C guid must fit and valid DeviceIndex must be provided */
    if (CompareGuid (TmpI2cIo->DeviceGuid, &I2cGuid) &&
        (TmpI2cIo->DeviceIndex == I2C_DEVICE_INDEX (
                                    Rk860xRegulatorBuses[i],
                                    Rk860xRegulatorAddresses[i]
                                    )))
    {
      DEBUG ((DEBUG_INFO, "Rk860xRegulatorSupported: attached to RK860X_REGULATOR device\n"));
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

STATIC
EFI_STATUS
EFIAPI
Rk860xRegulatorRead (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This,
  IN UINT8                            *RegAddress,
  IN UINT16                           RegAddressLength,
  IN UINT8                            *Buffer,
  IN UINT16                           Length
  )
{
  EFI_I2C_REQUEST_PACKET    *RequestPacket;
  UINTN                     RequestPacketSize;
  EFI_STATUS                Status                  = EFI_SUCCESS;
  RK860X_REGULATOR_CONTEXT  *Rk860xRegulatorContext = RK860X_REGULATOR_SC_FROM_PROTOCOL (This);

  ASSERT (Rk860xRegulatorContext != NULL);
  ASSERT (Rk860xRegulatorContext->I2cIo != NULL);

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

  Status = Rk860xRegulatorContext->I2cIo->QueueRequest (Rk860xRegulatorContext->I2cIo, 0, NULL, RequestPacket, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Rk860xRegulatorRead: error %d during transmission\n", Status));
  }

  FreePool (RequestPacket);

  return Status;
}

STATIC
EFI_STATUS
EFIAPI
Rk860xRegulatorWrite (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This,
  IN UINT8                            *RegAddress,
  IN UINT16                           RegAddressLength,
  IN UINT8                            *Buffer,
  IN UINT16                           Length
  )
{
  EFI_I2C_REQUEST_PACKET    *RequestPacket;
  UINTN                     RequestPacketSize;
  EFI_STATUS                Status                  = EFI_SUCCESS;
  RK860X_REGULATOR_CONTEXT  *Rk860xRegulatorContext = RK860X_REGULATOR_SC_FROM_PROTOCOL (This);
  UINT8                     *Data;
  UINT16                    i;

  ASSERT (Rk860xRegulatorContext != NULL);
  ASSERT (Rk860xRegulatorContext->I2cIo != NULL);

  RequestPacketSize = sizeof (UINTN) + sizeof (EFI_I2C_OPERATION);
  RequestPacket     = AllocateZeroPool (RequestPacketSize);
  if (RequestPacket == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Data = AllocateZeroPool (RegAddressLength + Length);
  if (Data == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (i = 0; i < RegAddressLength; i++) {
    Data[i] = RegAddress[i] & 0xff;
  }

  for (i = RegAddressLength; i < RegAddressLength + Length; i++) {
    Data[i] = Buffer[i - RegAddressLength] & 0xff;
  }

  RequestPacket->OperationCount = 1;

  RequestPacket->Operation[0].Flags         = 0;
  RequestPacket->Operation[0].LengthInBytes = RegAddressLength + Length;
  RequestPacket->Operation[0].Buffer        = Data;

  Status = Rk860xRegulatorContext->I2cIo->QueueRequest (Rk860xRegulatorContext->I2cIo, 0, NULL, RequestPacket, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Rk860xRegulatorWrite: error %d during transmission\n", Status));
  }

  FreePool (Data);
  FreePool (RequestPacket);

  return Status;
}

STATIC
EFI_STATUS
EFIAPI
Rk860xRegulatorReadRegister (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This,
  IN UINT8                            Address,
  OUT UINT8                           *Value
  )
{
  return Rk860xRegulatorRead (
           This,
           &Address,
           sizeof (UINT8),
           Value,
           sizeof (UINT8)
           );
}

STATIC
EFI_STATUS
EFIAPI
Rk860xRegulatorWriteRegister (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This,
  IN UINT8                            Address,
  IN UINT8                            Value
  )
{
  return Rk860xRegulatorWrite (
           This,
           &Address,
           sizeof (UINT8),
           &Value,
           sizeof (UINT8)
           );
}

STATIC
EFI_STATUS
EFIAPI
Rk860xRegulatorClearAndSetRegister (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This,
  IN UINT8                            Address,
  IN UINT8                            Clear,
  IN UINT8                            Set
  )
{
  UINT8       Value;
  EFI_STATUS  Status = EFI_SUCCESS;

  Status = Rk860xRegulatorReadRegister (This, Address, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Value = (Value & ~Clear) | Set;

  return Rk860xRegulatorWriteRegister (This, Address, Value);
}

EFI_STATUS
EFIAPI
Rk860xRegulatorGetVoltage (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This,
  OUT UINT32                          *Voltage,
  IN BOOLEAN                          Suspend
  )
{
  UINT8                     Value;
  RK860X_REGULATOR_CONTEXT  *Rk860xRegulatorContext = RK860X_REGULATOR_SC_FROM_PROTOCOL (This);
  EFI_STATUS                Status                  = EFI_SUCCESS;

  Status = Rk860xRegulatorReadRegister (
             This,
             Suspend ? Rk860xRegulatorContext->Config.SleepReg : Rk860xRegulatorContext->Config.VolReg,
             &Value
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Value   &= Rk860xRegulatorContext->Config.VolMask;
  *Voltage = (Value * Rk860xRegulatorContext->Config.VselStep) + Rk860xRegulatorContext->Config.VselMin;

  return Status;
}

EFI_STATUS
EFIAPI
Rk860xRegulatorSetVoltage (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This,
  IN UINT32                           Voltage,
  IN BOOLEAN                          Suspend
  )
{
  UINT8                     Value;
  RK860X_REGULATOR_CONTEXT  *Rk860xRegulatorContext = RK860X_REGULATOR_SC_FROM_PROTOCOL (This);

  Value = (Voltage - Rk860xRegulatorContext->Config.VselMin) / Rk860xRegulatorContext->Config.VselStep;

  return Rk860xRegulatorClearAndSetRegister (
           This,
           Suspend ? Rk860xRegulatorContext->Config.SleepReg : Rk860xRegulatorContext->Config.VolReg,
           Rk860xRegulatorContext->Config.VolMask,
           Value
           );
}

EFI_STATUS
EFIAPI
Rk860xRegulatorGetEnable (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This,
  OUT BOOLEAN                         *Enable,
  IN BOOLEAN                          Suspend
  )
{
  UINT8                     Value;
  RK860X_REGULATOR_CONTEXT  *Rk860xRegulatorContext = RK860X_REGULATOR_SC_FROM_PROTOCOL (This);
  EFI_STATUS                Status                  = EFI_SUCCESS;

  Status = Rk860xRegulatorReadRegister (
             This,
             Suspend ? Rk860xRegulatorContext->Config.SleepEnReg : Rk860xRegulatorContext->Config.EnReg,
             &Value
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *Enable = Value & RK860X_VSEL_BUCK_EN;

  return Status;
}

EFI_STATUS
EFIAPI
Rk860xRegulatorSetEnable (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This,
  IN BOOLEAN                          Enable,
  IN BOOLEAN                          Suspend
  )
{
  UINT8                     Value;
  RK860X_REGULATOR_CONTEXT  *Rk860xRegulatorContext = RK860X_REGULATOR_SC_FROM_PROTOCOL (This);

  Value = Enable ? RK860X_VSEL_BUCK_EN : 0;

  return Rk860xRegulatorClearAndSetRegister (
           This,
           Suspend ? Rk860xRegulatorContext->Config.SleepEnReg : Rk860xRegulatorContext->Config.EnReg,
           RK860X_VSEL_BUCK_EN,
           Value
           );
}

STATIC
EFI_STATUS
EFIAPI
Rk860xRegulatorInitConfig (
  IN CONST RK860X_REGULATOR_PROTOCOL  *This
  )
{
  UINT8                     ChipId;
  RK860X_REGULATOR_CONTEXT  *Rk860xRegulatorContext = RK860X_REGULATOR_SC_FROM_PROTOCOL (This);
  EFI_STATUS                Status                  = EFI_SUCCESS;

  Status = Rk860xRegulatorReadRegister (This, RK860X_ID1, &ChipId);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ChipId &= RK860X_DIE_ID_MASK;

  switch (ChipId) {
    case RK860X_CHIP_ID_00_01:
      Rk860xRegulatorContext->Config.VselMin     = 712500;
      Rk860xRegulatorContext->Config.VselStep    = 12500;
      Rk860xRegulatorContext->Config.NumVoltages = RK860X_NVOLTAGES_64;
      Rk860xRegulatorContext->Config.VolMask     = RK860X_VSEL_A_NSEL_MASK;
      Rk860xRegulatorContext->Config.SleepReg    = RK860X_VSEL1_A;
      Rk860xRegulatorContext->Config.VolReg      = RK860X_VSEL0_A;
      Rk860xRegulatorContext->Config.EnReg       = RK860X_VSEL0_A;
      Rk860xRegulatorContext->Config.SleepEnReg  = RK860X_VSEL1_A;
      break;
    case RK860X_CHIP_ID_02_03:
      Rk860xRegulatorContext->Config.VselMin     = 500000;
      Rk860xRegulatorContext->Config.VselStep    = 6250;
      Rk860xRegulatorContext->Config.NumVoltages = RK860X_NVOLTAGES_160;
      Rk860xRegulatorContext->Config.VolMask     = RK860X_VSEL_B_NSEL_MASK;
      Rk860xRegulatorContext->Config.SleepReg    = RK860X_VSEL1_B;
      Rk860xRegulatorContext->Config.VolReg      = RK860X_VSEL0_B;
      Rk860xRegulatorContext->Config.EnReg       = RK860X_VSEL0_A;
      Rk860xRegulatorContext->Config.SleepEnReg  = RK860X_VSEL1_A;
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Rk860xRegulatorInitConfig: unsupported chip id %u\n", ChipId));
      return EFI_UNSUPPORTED;
  }

  return Status;
}

EFI_STATUS
EFIAPI
Rk860xRegulatorStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  )
{
  EFI_STATUS                 Status = EFI_SUCCESS;
  RK860X_REGULATOR_CONTEXT   *Rk860xRegulatorContext;
  RK860X_REGULATOR_PROTOCOL  *Rk860xRegulatorProtocol;
  UINT8                      *Rk860xRegulatorAddresses;
  UINTN                      Rk860xRegulatorAddressesSize;
  UINT8                      *Rk860xRegulatorBuses;
  UINT8                      *Rk860xRegulatorTags;
  UINT8                      *Rk860xRegulatorMinVoltagesPtr;
  UINTN                      Rk860xRegulatorMinVoltagesSize;
  UINT8                      *Rk860xRegulatorMaxVoltagesPtr;
  UINTN                      Rk860xRegulatorMaxVoltagesSize;
  UINT32                     Index;

  Rk860xRegulatorContext = AllocateZeroPool (sizeof (RK860X_REGULATOR_CONTEXT));
  if (Rk860xRegulatorContext == NULL) {
    DEBUG ((DEBUG_ERROR, "Rk860xRegulator: allocation fail\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Rk860xRegulatorProtocol = &Rk860xRegulatorContext->Rk860xRegulatorProtocol;

  Rk860xRegulatorContext->ControllerHandle = ControllerHandle;
  Rk860xRegulatorContext->Signature        = RK860X_REGULATOR_SIGNATURE;

  Rk860xRegulatorProtocol->GetVoltage = Rk860xRegulatorGetVoltage;
  Rk860xRegulatorProtocol->SetVoltage = Rk860xRegulatorSetVoltage;
  Rk860xRegulatorProtocol->GetEnable  = Rk860xRegulatorGetEnable;
  Rk860xRegulatorProtocol->SetEnable  = Rk860xRegulatorSetEnable;

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gEfiI2cIoProtocolGuid,
                  (VOID **)&Rk860xRegulatorContext->I2cIo,
                  gImageHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rk860xRegulator: failed to open I2cIo\n"));
    FreePool (Rk860xRegulatorContext);
    return EFI_UNSUPPORTED;
  }

  Rk860xRegulatorProtocol->Identifier = Rk860xRegulatorContext->I2cIo->DeviceIndex;
  Status                              = gBS->InstallMultipleProtocolInterfaces (
                                               &ControllerHandle,
                                               &gRk860xRegulatorProtocolGuid,
                                               Rk860xRegulatorProtocol,
                                               NULL
                                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rk860xRegulator: failed to install RK860X_REGULATOR_PROTOCOL\n"));
    goto fail;
  }

  Status = Rk860xRegulatorInitConfig (Rk860xRegulatorProtocol);
  if (EFI_ERROR (Status)) {
    goto fail;
  }

  Rk860xRegulatorProtocol->SupportedVoltageRange.Min = Rk860xRegulatorContext->Config.VselMin;
  Rk860xRegulatorProtocol->SupportedVoltageRange.Max = RK860X_MAX_VOLTAGE;

  Rk860xRegulatorProtocol->PreferredVoltageRange = Rk860xRegulatorProtocol->SupportedVoltageRange;

  Rk860xRegulatorAddresses       = PcdGetPtr (PcdRk860xRegulatorAddresses);
  Rk860xRegulatorAddressesSize   = PcdGetSize (PcdRk860xRegulatorAddresses);
  Rk860xRegulatorBuses           = PcdGetPtr (PcdRk860xRegulatorBuses);
  Rk860xRegulatorTags            = PcdGetPtr (PcdRk860xRegulatorTags);
  Rk860xRegulatorMinVoltagesPtr  = PcdGetPtr (PcdRk860xRegulatorMinVoltages);
  Rk860xRegulatorMinVoltagesSize = PcdGetSize (PcdRk860xRegulatorMinVoltages);
  Rk860xRegulatorMaxVoltagesPtr  = PcdGetPtr (PcdRk860xRegulatorMaxVoltages);
  Rk860xRegulatorMaxVoltagesSize = PcdGetSize (PcdRk860xRegulatorMaxVoltages);

  for (Index = 0; Rk860xRegulatorAddresses[Index] != '\0'; Index++) {
    if (Rk860xRegulatorContext->I2cIo->DeviceIndex == I2C_DEVICE_INDEX (
                                                        Rk860xRegulatorBuses[Index],
                                                        Rk860xRegulatorAddresses[Index]
                                                        ))
    {
      Rk860xRegulatorProtocol->Tag = Rk860xRegulatorTags[Index];

      Index *= sizeof (UINT32);

      if (Rk860xRegulatorMinVoltagesSize / sizeof (UINT32) == Rk860xRegulatorAddressesSize) {
        Rk860xRegulatorProtocol->PreferredVoltageRange.Min = Rk860xRegulatorMinVoltagesPtr[Index] |
                                                             Rk860xRegulatorMinVoltagesPtr[Index + 1] << 8 |
                                                             Rk860xRegulatorMinVoltagesPtr[Index + 2] << 16 |
                                                             Rk860xRegulatorMinVoltagesPtr[Index + 3] << 24;
      }

      if (Rk860xRegulatorMaxVoltagesSize / sizeof (UINT32) == Rk860xRegulatorAddressesSize) {
        Rk860xRegulatorProtocol->PreferredVoltageRange.Max = Rk860xRegulatorMaxVoltagesPtr[Index] |
                                                             Rk860xRegulatorMaxVoltagesPtr[Index + 1] << 8 |
                                                             Rk860xRegulatorMaxVoltagesPtr[Index + 2] << 16 |
                                                             Rk860xRegulatorMaxVoltagesPtr[Index + 3] << 24;
      }

      break;
    }
  }

  return Status;

fail:
  FreePool (Rk860xRegulatorContext);
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
Rk860xRegulatorStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                  ControllerHandle,
  IN  UINTN                       NumberOfChildren,
  IN  EFI_HANDLE                  *ChildHandleBuffer OPTIONAL
  )
{
  RK860X_REGULATOR_PROTOCOL  *Rk860xRegulatorProtocol;
  EFI_STATUS                 Status;
  RK860X_REGULATOR_CONTEXT   *Rk860xRegulatorContext;

  Status = gBS->OpenProtocol (
                  ControllerHandle,
                  &gRk860xRegulatorProtocolGuid,
                  (VOID **)&Rk860xRegulatorProtocol,
                  This->DriverBindingHandle,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );

  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Rk860xRegulatorContext = RK860X_REGULATOR_SC_FROM_PROTOCOL (Rk860xRegulatorProtocol);

  gBS->UninstallMultipleProtocolInterfaces (
         &ControllerHandle,
         &gRk860xRegulatorProtocolGuid,
         &Rk860xRegulatorContext->Rk860xRegulatorProtocol,
         &gEfiDriverBindingProtocolGuid,
         &gDriverBindingProtocol,
         NULL
         );
  gBS->CloseProtocol (
         ControllerHandle,
         &gEfiI2cIoProtocolGuid,
         gImageHandle,
         ControllerHandle
         );
  FreePool (Rk860xRegulatorContext);
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
Rk860xRegulatorInitialise (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiDriverBindingProtocolGuid,
                  &gDriverBindingProtocol,
                  NULL
                  );

  return Status;
}
