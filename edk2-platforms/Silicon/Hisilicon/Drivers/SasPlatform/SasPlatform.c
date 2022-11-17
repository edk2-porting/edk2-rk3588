/** @file
*
*  Copyright (c) 2017, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2017, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <PiDxe.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiLib.h>

#include <Protocol/HisiPlatformSasProtocol.h>

#define SAS0BusAddr 0xc3000000
#define SAS1BusAddr 0xa2000000
#define SAS2BusAddr 0xa3000000

#define SAS0ResetAddr 0xc0000000
#define SAS1ResetAddr 0xa0000000
#define SAS2ResetAddr 0xa0000000

typedef struct {
  UINTN                       Signature;
  EFI_HANDLE                  Handle;
  HISI_PLATFORM_SAS_PROTOCOL  SasPlatformProtocol;
} SAS_PLATFORM_INSTANCE;


STATIC HISI_PLATFORM_SAS_PROTOCOL mSasPlatformProtocol[] = {
  {
    0,
    FALSE,
    SAS0BusAddr,
    SAS0ResetAddr
  },
  {
    1,
    TRUE,
    SAS1BusAddr,
    SAS1ResetAddr
  },
  {
    2,
    FALSE,
    SAS2BusAddr,
    SAS2ResetAddr
  }
};

EFI_STATUS
EFIAPI
SasPlatformInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  UINTN                    Loop;
  SAS_PLATFORM_INSTANCE    *PrivateData;
  EFI_STATUS               Status;

  for (Loop = 0; Loop < ARRAY_SIZE (mSasPlatformProtocol); Loop++) {
    if (mSasPlatformProtocol[Loop].Enable != TRUE) {
      continue;
    }
    PrivateData = AllocateZeroPool (sizeof(SAS_PLATFORM_INSTANCE));
    if (PrivateData == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }

    PrivateData->SasPlatformProtocol = mSasPlatformProtocol[Loop];

    Status = gBS->InstallMultipleProtocolInterfaces (
                    &PrivateData->Handle,
                    &gHisiPlatformSasProtocolGuid,
                    &PrivateData->SasPlatformProtocol,
                    NULL
                    );
    if (EFI_ERROR (Status)) {
      FreePool (PrivateData);
      DEBUG ((DEBUG_ERROR,
              "[%a]:[%dL] InstallProtocolInterface fail. %r\n",
              __FUNCTION__,
              __LINE__,
              Status));
      continue;
    }
  }

  DEBUG ((DEBUG_INFO, "sas platform init driver Ok!!!\n"));
  return EFI_SUCCESS;
}

