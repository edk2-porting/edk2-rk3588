/** I2cDxe.c
  I2c driver APIs for read, write, initialize, set speed and reset

  Copyright 2017-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/I2cLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Ppi/NxpPlatformGetClock.h>

#include "I2cDxe.h"

STATIC CONST EFI_I2C_CONTROLLER_CAPABILITIES mI2cControllerCapabilities = {
  0,
  0,
  0,
  0
};

/**
  Function to set i2c bus frequency

  @param   This            Pointer to I2c master protocol
  @param   BusClockHertz   value to be set

  @retval EFI_SUCCESS      Operation successfull
**/
STATIC
EFI_STATUS
EFIAPI
SetBusFrequency (
  IN CONST EFI_I2C_MASTER_PROTOCOL   *This,
  IN OUT UINTN                       *BusClockHertz
 )
{
  UINTN                    I2cBase;
  UINT64                   I2cClock;
  NXP_I2C_MASTER           *I2c;

  I2c = NXP_I2C_FROM_THIS (This);

  I2cBase = (UINTN)(I2c->Dev->Resources[0].AddrRangeMin);

  I2cClock = gPlatformGetClockPpi.PlatformGetClock (NXP_I2C_CLOCK, 0);

  I2cInitialize (I2cBase, I2cClock, *BusClockHertz);

  return EFI_SUCCESS;
}

/**
  Function to reset I2c Controller

  @param  This             Pointer to I2c master protocol

  @return EFI_SUCCESS      Operation successfull
**/
STATIC
EFI_STATUS
EFIAPI
Reset (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
StartRequest (
  IN CONST EFI_I2C_MASTER_PROTOCOL *This,
  IN UINTN                         SlaveAddress,
  IN EFI_I2C_REQUEST_PACKET        *RequestPacket,
  IN EFI_EVENT                     Event            OPTIONAL,
  OUT EFI_STATUS                   *I2cStatus       OPTIONAL
  )
{
  NXP_I2C_MASTER           *I2c;
  UINTN                    I2cBase;
  EFI_STATUS               Status;
  EFI_TPL                  Tpl;
  BOOLEAN                  AtRuntime;

  AtRuntime = EfiAtRuntime ();
  if (!AtRuntime) {
    Tpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
  }

  I2c = NXP_I2C_FROM_THIS (This);

  I2cBase = (UINTN)(I2c->Dev->Resources[0].AddrRangeMin);

  Status = I2cBusXfer (I2cBase, SlaveAddress, RequestPacket);

  if (!AtRuntime) {
    gBS->RestoreTPL (Tpl);
  }

  return Status;
}

EFI_STATUS
NxpI2cInit (
  IN EFI_HANDLE             DriverBindingHandle,
  IN EFI_HANDLE             ControllerHandle
  )
{
  EFI_STATUS                RetVal;
  NON_DISCOVERABLE_DEVICE   *Dev;
  NXP_I2C_MASTER            *I2c;

  RetVal = gBS->OpenProtocol (ControllerHandle,
                              &gEdkiiNonDiscoverableDeviceProtocolGuid,
                              (VOID **)&Dev, DriverBindingHandle,
                              ControllerHandle, EFI_OPEN_PROTOCOL_BY_DRIVER);
  if (EFI_ERROR (RetVal)) {
    return RetVal;
  }

  I2c = AllocateZeroPool (sizeof (NXP_I2C_MASTER));

  I2c->Signature                            = NXP_I2C_SIGNATURE;
  I2c->I2cMaster.SetBusFrequency            = SetBusFrequency;
  I2c->I2cMaster.Reset                      = Reset;
  I2c->I2cMaster.StartRequest               = StartRequest;
  I2c->I2cMaster.I2cControllerCapabilities  = &mI2cControllerCapabilities;
  I2c->Dev                                  = Dev;

  CopyGuid (&I2c->DevicePath.Vendor.Guid, &gEfiCallerIdGuid);
  I2c->DevicePath.MmioBase = I2c->Dev->Resources[0].AddrRangeMin;
  SetDevicePathNodeLength (&I2c->DevicePath.Vendor,
    sizeof (I2c->DevicePath) - sizeof (I2c->DevicePath.End));
  SetDevicePathEndNode (&I2c->DevicePath.End);

  RetVal = gBS->InstallMultipleProtocolInterfaces (&ControllerHandle,
                  &gEfiI2cMasterProtocolGuid, (VOID**)&I2c->I2cMaster,
                  &gEfiDevicePathProtocolGuid, &I2c->DevicePath,
                  NULL);

  if (EFI_ERROR (RetVal)) {
    FreePool (I2c);
    gBS->CloseProtocol (ControllerHandle,
                        &gEdkiiNonDiscoverableDeviceProtocolGuid,
                        DriverBindingHandle,
                        ControllerHandle);
  }

  return RetVal;
}

EFI_STATUS
NxpI2cRelease (
  IN EFI_HANDLE                 DriverBindingHandle,
  IN EFI_HANDLE                 ControllerHandle
  )
{
  EFI_I2C_MASTER_PROTOCOL       *I2cMaster;
  EFI_STATUS                    RetVal;
  NXP_I2C_MASTER                *I2c;

  RetVal = gBS->HandleProtocol (ControllerHandle,
                                &gEfiI2cMasterProtocolGuid,
                                (VOID **)&I2cMaster);
  ASSERT_EFI_ERROR (RetVal);
  if (EFI_ERROR (RetVal)) {
    return RetVal;
  }

  I2c = NXP_I2C_FROM_THIS (I2cMaster);

  RetVal = gBS->UninstallMultipleProtocolInterfaces (ControllerHandle,
                  &gEfiI2cMasterProtocolGuid, I2cMaster,
                  &gEfiDevicePathProtocolGuid, &I2c->DevicePath,
                  NULL);
  if (EFI_ERROR (RetVal)) {
    return RetVal;
  }

  RetVal = gBS->CloseProtocol (ControllerHandle,
                               &gEdkiiNonDiscoverableDeviceProtocolGuid,
                               DriverBindingHandle,
                               ControllerHandle);
  ASSERT_EFI_ERROR (RetVal);
  if (EFI_ERROR (RetVal)) {
    return RetVal;
  }

  gBS->FreePool (I2c);

  return EFI_SUCCESS;
}
