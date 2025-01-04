/** @file
 *
 *  Copyright (c) 2024-2025, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "ExitBootServicesHook.h"

STATIC EFI_EXIT_BOOT_SERVICES  mOriginalExitBootServices;

STATIC
EFI_STATUS
EFIAPI
RegisterHandler (
  IN EXIT_BOOT_SERVICES_OS_NOTIFY_PROTOCOL  *This,
  IN EXIT_BOOT_SERVICES_OS_HANDLER          Handler
  )
{
  EXIT_BOOT_SERVICES_OS_NOTIFY_INSTANCE  *Instance;
  LIST_ENTRY                             *Link;
  EXIT_BOOT_SERVICES_OS_HANDLER_ENTRY    *Entry;

  if (Handler == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Instance = EXIT_BOOT_SERVICES_OS_NOTIFY_INSTANCE_FROM_THIS (This);

  for ( Link = GetFirstNode (&Instance->Handlers)
        ; !IsNull (&Instance->Handlers, Link)
        ; Link = GetNextNode (&Instance->Handlers, Link)
        )
  {
    Entry = EXIT_BOOT_SERVICES_OS_HANDLER_ENTRY_FROM_LINK (Link);
    if (Entry->Handler == Handler) {
      return EFI_ALREADY_STARTED;
    }
  }

  ASSERT (IsNull (&Instance->Handlers, Link));
  Entry = AllocatePool (sizeof (*Entry));
  if (Entry == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Entry->Signature = EXIT_BOOT_SERVICES_OS_HANDLER_ENTRY_SIGNATURE;
  Entry->Handler   = Handler;
  InsertTailList (&Instance->Handlers, &Entry->Link);
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
UnregisterHandler (
  IN EXIT_BOOT_SERVICES_OS_NOTIFY_PROTOCOL  *This,
  IN EXIT_BOOT_SERVICES_OS_HANDLER          Handler
  )
{
  EXIT_BOOT_SERVICES_OS_NOTIFY_INSTANCE  *Instance;
  LIST_ENTRY                             *Link;
  EXIT_BOOT_SERVICES_OS_HANDLER_ENTRY    *Entry;

  if (Handler == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Instance = EXIT_BOOT_SERVICES_OS_NOTIFY_INSTANCE_FROM_THIS (This);

  for ( Link = GetFirstNode (&Instance->Handlers)
        ; !IsNull (&Instance->Handlers, Link)
        ; Link = GetNextNode (&Instance->Handlers, Link)
        )
  {
    Entry = EXIT_BOOT_SERVICES_OS_HANDLER_ENTRY_FROM_LINK (Link);
    if (Entry->Handler == Handler) {
      RemoveEntryList (&Entry->Link);
      FreePool (Entry);
      return EFI_SUCCESS;
    }
  }

  return EFI_INVALID_PARAMETER;
}

STATIC EXIT_BOOT_SERVICES_OS_NOTIFY_INSTANCE  mNotifyInstance = {
  EXIT_BOOT_SERVICES_OS_NOTIFY_INSTANCE_SIGNATURE,
  {
    RegisterHandler,
    UnregisterHandler
  },
  INITIALIZE_LIST_HEAD_VARIABLE (mNotifyInstance.Handlers)
};

STATIC
EFI_STATUS
EFIAPI
ExitBootServicesHook (
  IN EFI_HANDLE  ImageHandle,
  IN UINTN       MapKey
  )
{
  EXIT_BOOT_SERVICES_OS_CONTEXT        Context;
  LIST_ENTRY                           *Link;
  EXIT_BOOT_SERVICES_OS_HANDLER_ENTRY  *Entry;

  Context.ReturnAddress = (EFI_PHYSICAL_ADDRESS)RETURN_ADDRESS (0);
  ASSERT (Context.ReturnAddress != 0);

  Context.OsLoaderAddress = FindPeImageBase (Context.ReturnAddress);
  Context.OsType          = IdentifyOsType (Context.OsLoaderAddress);

  DEBUG ((
    DEBUG_INFO,
    "ExitBootServices: Booting %a OS at 0x%lx\n",
    OsTypeToString (Context.OsType),
    Context.OsLoaderAddress
    ));

  for ( Link = GetFirstNode (&mNotifyInstance.Handlers)
        ; !IsNull (&mNotifyInstance.Handlers, Link)
        ; Link = GetNextNode (&mNotifyInstance.Handlers, Link)
        )
  {
    Entry = EXIT_BOOT_SERVICES_OS_HANDLER_ENTRY_FROM_LINK (Link);
    Entry->Handler (&Context);
  }

  gBS->ExitBootServices = mOriginalExitBootServices;

  return gBS->ExitBootServices (ImageHandle, MapKey);
}

EFI_STATUS
EFIAPI
ExitBootServicesHookDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle;

  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gExitBootServicesOsNotifyProtocolGuid,
                  &mNotifyInstance.Notify,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  mOriginalExitBootServices = gBS->ExitBootServices;
  gBS->ExitBootServices     = ExitBootServicesHook;

  return EFI_SUCCESS;
}
