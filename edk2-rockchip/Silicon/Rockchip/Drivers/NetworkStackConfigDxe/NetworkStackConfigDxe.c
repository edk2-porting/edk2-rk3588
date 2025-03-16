/** @file

  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/HiiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

#include "NetworkStackConfigDxe.h"

extern UINT8  NetworkStackConfigDxeHiiBin[];
extern UINT8  NetworkStackConfigDxeStrings[];

typedef struct {
  VENDOR_DEVICE_PATH          VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    End;
} HII_VENDOR_DEVICE_PATH;

STATIC HII_VENDOR_DEVICE_PATH  mVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    NETWORK_STACK_CONFIG_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8)(END_DEVICE_PATH_LENGTH),
      (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

/**
  Installs HII page for user configuration.

  @retval EFI_SUCCESS        The operation completed successfully.

**/
STATIC
EFI_STATUS
EFIAPI
InstallHiiPages (
  VOID
  )
{
  EFI_STATUS      Status;
  EFI_HII_HANDLE  HiiHandle;
  EFI_HANDLE      DriverHandle;

  DriverHandle = NULL;
  Status       = gBS->InstallMultipleProtocolInterfaces (
                        &DriverHandle,
                        &gEfiDevicePathProtocolGuid,
                        &mVendorDevicePath,
                        NULL
                        );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HiiHandle = HiiAddPackages (
                &gNetworkStackConfigFormSetGuid,
                DriverHandle,
                NetworkStackConfigDxeStrings,
                NetworkStackConfigDxeHiiBin,
                NULL
                );

  if (HiiHandle == NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &mVendorDevicePath,
           NULL
           );
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}

STATIC
VOID
NetworkStackConfigApply (
  IN NETWORK_STACK_CONFIG_VARSTORE_DATA  *Config
  )
{
  EFI_STATUS  Status;
  UINT32      Index;
  EFI_HANDLE  Handle = NULL;

  struct {
    EFI_GUID    *Protocol;
    CHAR8       *Name;
    BOOLEAN     Enabled;
  } States[] = {
    { &gNetworkStackEnabledProtocolGuid,         "Network Stack", Config->Enabled         },
    { &gNetworkStackIpv4EnabledProtocolGuid,     "IPv4 Stack",    Config->Ipv4Enabled     },
    { &gNetworkStackIpv6EnabledProtocolGuid,     "IPv6 Stack",    Config->Ipv6Enabled     },
    { &gNetworkStackPxeBootEnabledProtocolGuid,  "PXE Boot",      Config->PxeBootEnabled  },
    { &gNetworkStackHttpBootEnabledProtocolGuid, "HTTP Boot",     Config->HttpBootEnabled },
  };

  for (Index = 0; Index < ARRAY_SIZE (States); Index++) {
    if (States[Index].Enabled) {
      Status = gBS->InstallMultipleProtocolInterfaces (
                      &Handle,
                      States[Index].Protocol,
                      NULL,
                      NULL
                      );
      ASSERT_EFI_ERROR (Status);
    } else {
      DEBUG ((DEBUG_INFO, "%a: %a disabled!\n", __func__, States[Index].Name));

      if (Index == 0) {
        break;
      }
    }
  }
}

/**
  The entry point for NetworkStackConfigDxe driver.

  @param[in] ImageHandle     The image handle of the driver.
  @param[in] SystemTable     The system table.

  @retval EFI_SUCCESS        The operation completed successfully.

**/
EFI_STATUS
EFIAPI
NetworkStackConfigInitialize (
  IN  EFI_HANDLE        ImageHandle,
  IN  EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                          Status;
  UINTN                               Size;
  NETWORK_STACK_CONFIG_VARSTORE_DATA  Config;

  Config.Enabled         = NETWORK_STACK_ENABLED_DEFAULT;
  Config.Ipv4Enabled     = NETWORK_STACK_IPV4_ENABLED_DEFAULT;
  Config.Ipv6Enabled     = NETWORK_STACK_IPV6_ENABLED_DEFAULT;
  Config.PxeBootEnabled  = NETWORK_STACK_PXE_BOOT_ENABLED_DEFAULT;
  Config.HttpBootEnabled = NETWORK_STACK_HTTP_BOOT_ENABLED_DEFAULT;

  Size   = sizeof (NETWORK_STACK_CONFIG_VARSTORE_DATA);
  Status = gRT->GetVariable (
                  NETWORK_STACK_CONFIG_DATA_VAR_NAME,
                  &gNetworkStackConfigFormSetGuid,
                  NULL,
                  &Size,
                  &Config
                  );
  if (EFI_ERROR (Status)) {
    Status = gRT->SetVariable (
                    NETWORK_STACK_CONFIG_DATA_VAR_NAME,
                    &gNetworkStackConfigFormSetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    Size,
                    &Config
                    );
    ASSERT_EFI_ERROR (Status);
  }

  NetworkStackConfigApply (&Config);

  return InstallHiiPages ();
}
