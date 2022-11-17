/** @file

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "DxeMePolicyUpdate.h"

//
// Record version
//
#define RECORD_REVISION_1              0x01
#define MAX_FW_UPDATE_BIOS_SELECTIONS  2

//
// Function implementations executed during policy initialization phase
//

/**
  Update the ME Policy Library

  @param[in, out] DxeMePolicy           The pointer to get ME Policy protocol instance

  @retval EFI_SUCCESS                   Initialization complete.
  @retval EFI_UNSUPPORTED               The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES          Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR              Device error, driver exits abnormally.

**/
EFI_STATUS
EFIAPI
UpdateDxeMePolicy (
  IN OUT  ME_POLICY_PROTOCOL            *DxeMePolicy
  )
{
  EFI_STATUS              Status;
  EFI_EVENT               EndOfDxeEvent;

  DEBUG ((DEBUG_INFO, "UpdateDxeMePolicy\n"));
  UpdateMePolicyFromSetup (DxeMePolicy);
  UpdateMePolicyFromMeSetup (DxeMePolicy);

  //
  // Register End of DXE event
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  UpdateMeSetupCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  ASSERT_EFI_ERROR (Status);
  return Status;
}

/**
  Update ME Policy while MePlatformProtocol is installed.

  @param[in] MePolicyInstance     Instance of ME Policy Protocol

**/
VOID
UpdateMePolicyFromMeSetup (
  IN ME_POLICY_PROTOCOL           *MePolicyInstance
  )
{

}

/**
  Update ME Policy if Setup variable exists.

  @param[in, out] MePolicyInstance     Instance of ME Policy Protocol

**/
VOID
UpdateMePolicyFromSetup (
  IN OUT ME_POLICY_PROTOCOL     *MePolicyInstance
  )
{

}

/**
  Functions performs HECI exchange with FW to update MePolicy settings.

  @param[in] Event         A pointer to the Event that triggered the callback.
  @param[in] Context       A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
UpdateMeSetupCallback (
  IN  EFI_EVENT                   Event,
  IN  VOID                        *Context
  )
{
  gBS->CloseEvent (Event);

  return;
}


