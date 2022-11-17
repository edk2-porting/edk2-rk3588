/** @file
  EDKII Device Security library for PCI device

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <IndustryStandard/Spdm.h>
#include <IndustryStandard/Pci.h>
#include <Protocol/PciIo.h>
#include <Protocol/DeviceSecurity.h>
#include <Protocol/PlatformDeviceSecurityPolicy.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

EDKII_DEVICE_SECURITY_POLICY           mDeviceSecurityPolicyNone = {
  EDKII_DEVICE_SECURITY_POLICY_REVISION,
  0,
  0,
};
EDKII_DEVICE_SECURITY_POLICY           mDeviceSecurityPolicyMeasurement = {
  EDKII_DEVICE_SECURITY_POLICY_REVISION,
  EDKII_DEVICE_MEASUREMENT_REQUIRED,
  0,
};

/**
  This function returns the device security policy associated with the device.

  The device security driver may call this interface to get the platform policy
  for the specific device and determine if the measurement or authentication
  is required.

  @param[in]  This                   The protocol instance pointer.
  @param[in]  DeviceId               The Identifier for the device.
  @param[out] DeviceSecurityPolicy   The Device Security Policy associated with the device.

  @retval EFI_SUCCESS                The device security policy is returned
  @retval EFI_UNSUPPORTED            The function is unsupported for the specific Device.
**/
EFI_STATUS
EFIAPI
GetDevicePolicy (
  IN  EDKII_DEVICE_SECURITY_POLICY_PROTOCOL  *This,
  IN  EDKII_DEVICE_IDENTIFIER                *DeviceId,
  OUT EDKII_DEVICE_SECURITY_POLICY           *DeviceSecurityPolicy
  )
{
  EFI_STATUS                  Status;
  EFI_PCI_IO_PROTOCOL         *PciIo;
  UINT16                      PciVendorId;
  UINT16                      PciDeviceId;

  CopyMem (DeviceSecurityPolicy, &mDeviceSecurityPolicyNone, sizeof(EDKII_DEVICE_SECURITY_POLICY));

  DEBUG ((DEBUG_INFO, "GetDevicePolicy - 0x%g\n", &DeviceId->DeviceType));

  if (!CompareGuid (&DeviceId->DeviceType, &gEdkiiDeviceIdentifierTypePciGuid)) {
    return EFI_SUCCESS;
  }

  Status = gBS->HandleProtocol (
                  DeviceId->DeviceHandle,
                  &gEdkiiDeviceIdentifierTypePciGuid,
                  (VOID **)&PciIo
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Locate - DeviceIdentifierTypePci - %r\n", Status));
    return EFI_SUCCESS;
  }

  Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_VENDOR_ID_OFFSET, 1, &PciVendorId);
  ASSERT_EFI_ERROR(Status);
  Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_DEVICE_ID_OFFSET, 1, &PciDeviceId);
  ASSERT_EFI_ERROR(Status);
  DEBUG ((DEBUG_INFO, "PCI Info - %04x:%04x\n", PciVendorId, PciDeviceId));

  if ((PciVendorId == 0x8086) && (PciDeviceId == 0x0B60)) {
    CopyMem (DeviceSecurityPolicy, &mDeviceSecurityPolicyMeasurement, sizeof(EDKII_DEVICE_SECURITY_POLICY));
  }

  return EFI_SUCCESS;
}

/**
  This function sets the device state based upon the authentication result.

  The device security driver may call this interface to give the platform
  a notify based upon the measurement or authentication result.
  If the authentication or measurement fails, the platform may choose:
  1) Do nothing.
  2) Disable this device or slot temporarily and continue boot.
  3) Reset the platform and retry again.
  4) Disable this device or slot permanently.
  5) Any other platform specific action.

  @param[in]  This                   The protocol instance pointer.
  @param[in]  DeviceId               The Identifier for the device.
  @param[in]  DeviceSecurityState    The Device Security state associated with the device.

  @retval EFI_SUCCESS                The device state is set
  @retval EFI_UNSUPPORTED            The function is unsupported for the specific Device.
**/
EFI_STATUS
EFIAPI
NotifyDeviceState (
  IN  EDKII_DEVICE_SECURITY_POLICY_PROTOCOL  *This,
  IN  EDKII_DEVICE_IDENTIFIER                *DeviceId,
  IN  EDKII_DEVICE_SECURITY_STATE            *DeviceSecurityState
  )
{
  EFI_STATUS                  Status;
  EFI_PCI_IO_PROTOCOL         *PciIo;
  UINT16                      PciVendorId;
  UINT16                      PciDeviceId;
  UINTN                       Segment;
  UINTN                       Bus;
  UINTN                       Device;
  UINTN                       Function;

  DEBUG ((DEBUG_INFO, "NotifyDeviceState - 0x%g\n", &DeviceId->DeviceType));

  if (!CompareGuid (&DeviceId->DeviceType, &gEdkiiDeviceIdentifierTypePciGuid)) {
    return EFI_SUCCESS;
  }

  Status = gBS->HandleProtocol (
                  DeviceId->DeviceHandle,
                  &gEdkiiDeviceIdentifierTypePciGuid,
                  (VOID **)&PciIo
                  );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Locate - DeviceIdentifierTypePci - %r\n", Status));
    return EFI_SUCCESS;
  }

  Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_VENDOR_ID_OFFSET, 1, &PciVendorId);
  ASSERT_EFI_ERROR(Status);
  Status = PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_DEVICE_ID_OFFSET, 1, &PciDeviceId);
  ASSERT_EFI_ERROR(Status);
  DEBUG ((DEBUG_INFO, "PCI Info - %04x:%04x\n", PciVendorId, PciDeviceId));

  Status = PciIo->GetLocation (
                    PciIo,
                    &Segment,
                    &Bus,
                    &Device,
                    &Function
                    );
  if (!EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "PCI Loc - %04x:%02x:%02x:%02x\n",
      Segment, Bus, Device, Function));
  }

  DEBUG ((DEBUG_INFO, "State - Measurement - 0x%08x, Authentication - 0x%08x\n",
    DeviceSecurityState->MeasurementState,
    DeviceSecurityState->AuthenticationState
    ));

  return EFI_SUCCESS;
}

EDKII_DEVICE_SECURITY_POLICY_PROTOCOL  mDeviceSecurityPolicy = {
  EDKII_DEVICE_SECURITY_POLICY_PROTOCOL_REVISION,
  GetDevicePolicy,
  NotifyDeviceState,
};

/**
  Entrypoint of the device security driver.

  @param[in]  ImageHandle  ImageHandle of the loaded driver
  @param[in]  SystemTable  Pointer to the System Table

  @retval  EFI_SUCCESS           The Protocol is installed.
  @retval  EFI_OUT_OF_RESOURCES  Not enough resources available to initialize driver.
  @retval  EFI_DEVICE_ERROR      A device error occurred attempting to initialize the driver.

**/
EFI_STATUS
EFIAPI
MainEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_HANDLE  Handle;
  EFI_STATUS  Status;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEdkiiDeviceSecurityPolicyProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mDeviceSecurityPolicy
                  );
  ASSERT_EFI_ERROR(Status);

  return Status;
}
