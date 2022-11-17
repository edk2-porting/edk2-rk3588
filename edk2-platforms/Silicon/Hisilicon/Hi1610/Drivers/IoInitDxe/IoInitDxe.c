/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Library/PlatformSysCtrlLib.h>
#include <Library/SerdesLib.h>

VOID
EFIAPI
ExitBootServicesEventSmmu (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  DEBUG((EFI_D_INFO,"SMMU ExitBootServicesEvent\n"));
}


EFI_STATUS
EFIAPI
IoInitDxeEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable)
{
  EFI_STATUS Status;
  EFI_EVENT  Event = NULL;

  (VOID) EfiSerdesInitWrap ();

  SmmuConfigForOS ();

  Status = gBS->CreateEvent (
      EVT_SIGNAL_EXIT_BOOT_SERVICES,
      TPL_CALLBACK,
      ExitBootServicesEventSmmu,
      NULL,
      &Event
      );

  if (EFI_ERROR(Status))
  {
    DEBUG ((EFI_D_ERROR, "[%a:%d] - CreateEvent failed: %r\n", __FUNCTION__,
        __LINE__, Status));
  }

  return Status;
}

