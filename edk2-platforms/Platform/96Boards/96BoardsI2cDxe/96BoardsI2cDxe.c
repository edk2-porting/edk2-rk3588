/** @file

  Copyright (c) 2018, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiDxe.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/I2cBusConfigurationManagement.h>
#include <Protocol/I2cEnumerate.h>
#include <Protocol/I2cMaster.h>
#include <Protocol/Mezzanine.h>

STATIC MEZZANINE_PROTOCOL     *mMezzanine;

typedef struct {
  EFI_I2C_ENUMERATE_PROTOCOL                    I2cEnumerate;
  EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL I2cConfigManagement;
  EFI_HANDLE                                    I2cMasterHandle;
  UINT32                                        BusFrequency;
  UINTN                                         NumDevices;
  CONST EFI_I2C_DEVICE                          *Devices;
} I2C_BUS;

STATIC
EFI_STATUS
EFIAPI
I2cEnumerate (
  IN CONST EFI_I2C_ENUMERATE_PROTOCOL *This,
  IN OUT CONST EFI_I2C_DEVICE         **Device
  )
{
  I2C_BUS     *Bus;

  if (Device == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Bus = BASE_CR (This, I2C_BUS, I2cEnumerate);

  if (Bus->NumDevices == 0 ||
      (Bus->NumDevices > 0 && *Device == &Bus->Devices[Bus->NumDevices - 1])) {
    *Device = NULL;
  } else if (*Device == NULL) {
    *Device = &Bus->Devices[0];
  } else if (Bus->NumDevices > 1 &&
             *Device >= &Bus->Devices[0] &&
             *Device < &Bus->Devices[Bus->NumDevices - 1]) {
    ++*Device;
  } else {
    return EFI_NO_MAPPING;
  }
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
I2cGetBusFrequency (
  IN CONST EFI_I2C_ENUMERATE_PROTOCOL *This,
  IN UINTN                            I2cBusConfiguration,
  OUT UINTN                           *BusClockHertz
  )
{
  I2C_BUS     *Bus;

  if (BusClockHertz == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (I2cBusConfiguration > 0) {
    return EFI_NO_MAPPING;
  }

  Bus = BASE_CR (This, I2C_BUS, I2cEnumerate);

  *BusClockHertz = Bus->BusFrequency;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
EnableI2cBusConfiguration (
  IN CONST EFI_I2C_BUS_CONFIGURATION_MANAGEMENT_PROTOCOL  *This,
  IN UINTN                                                I2cBusConfiguration,
  IN EFI_EVENT                                            Event       OPTIONAL,
  IN EFI_STATUS                                           *I2cStatus  OPTIONAL
  )
{
  EFI_I2C_MASTER_PROTOCOL *I2cMaster;
  EFI_STATUS              Status;
  UINTN                   BusClockHertz;
  I2C_BUS                 *Bus;

  if (I2cBusConfiguration > 0) {
    return EFI_NO_MAPPING;
  }

  Bus = BASE_CR (This, I2C_BUS, I2cConfigManagement);

  Status = gBS->HandleProtocol (Bus->I2cMasterHandle,
                  &gEfiI2cMasterProtocolGuid, (VOID **)&I2cMaster);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: gBS->HandleProtocol() failed - %r\n",
      __FUNCTION__, Status));
    return Status;
  }

  BusClockHertz = Bus->BusFrequency;
  Status = I2cMaster->SetBusFrequency (I2cMaster, &BusClockHertz);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: I2cMaster->SetBusFrequency() failed - %r\n",
      __FUNCTION__, Status));
    return Status;
  }

  if (Event != NULL) {
    *I2cStatus = EFI_SUCCESS;
    gBS->SignalEvent (Event);
  }
  return EFI_SUCCESS;
}

STATIC I2C_BUS mI2cBus0 = {
  { I2cEnumerate, I2cGetBusFrequency },
  { EnableI2cBusConfiguration },
  NULL,
  FixedPcdGet32 (PcdI2c0BusFrequencyHz),
  0,
  NULL,
};

STATIC I2C_BUS mI2cBus1 = {
  { I2cEnumerate, I2cGetBusFrequency },
  { EnableI2cBusConfiguration },
  NULL,
  FixedPcdGet32 (PcdI2c1BusFrequencyHz),
  0,
  NULL,
};

STATIC
VOID
RegisterI2cBus (
  IN  EFI_GUID                *Guid,
  IN  I2C_BUS                 *I2cBus,
  IN  UINTN                   NumDevices,
  IN  CONST EFI_I2C_DEVICE    *Devices
  )
{
  EFI_STATUS    Status;
  UINTN         BufferSize;

  BufferSize = sizeof (EFI_HANDLE);
  Status = gBS->LocateHandle (ByProtocol, Guid, NULL, &BufferSize,
                  &I2cBus->I2cMasterHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "%a: gBS->LocateHandle() failed - %r\n", __FUNCTION__,
      Status));
    return;
  }

  I2cBus->NumDevices = NumDevices;
  I2cBus->Devices = Devices;

  Status = gBS->InstallMultipleProtocolInterfaces (&I2cBus->I2cMasterHandle,
                  &gEfiI2cEnumerateProtocolGuid,
                  &I2cBus->I2cEnumerate,
                  &gEfiI2cBusConfigurationManagementProtocolGuid,
                  &I2cBus->I2cConfigManagement,
                  NULL);
  ASSERT_EFI_ERROR (Status);
}

STATIC
VOID
EFIAPI
OnEndOfDxe (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  gBS->CloseEvent (Event);
  gBS->ConnectController (mI2cBus0.I2cMasterHandle, NULL, NULL, TRUE);
  gBS->ConnectController (mI2cBus1.I2cMasterHandle, NULL, NULL, TRUE);
}

EFI_STATUS
EFIAPI
EntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     EndOfDxeEvent;

  Status = gBS->LocateProtocol (&g96BoardsMezzanineProtocolGuid, NULL,
                  (VOID **)&mMezzanine);
  ASSERT_EFI_ERROR (Status);

  RegisterI2cBus (&g96BoardsI2c0MasterGuid, &mI2cBus0,
    mMezzanine->I2c0NumDevices, mMezzanine->I2c0DeviceArray);
  RegisterI2cBus (&g96BoardsI2c1MasterGuid, &mI2cBus1,
    mMezzanine->I2c1NumDevices, mMezzanine->I2c1DeviceArray);

  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, OnEndOfDxe,
                  NULL, &gEfiEndOfDxeEventGroupGuid, &EndOfDxeEvent);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
