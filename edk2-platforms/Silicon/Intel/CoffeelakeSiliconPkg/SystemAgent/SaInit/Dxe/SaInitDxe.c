/** @file
  This is the driver that initializes the Intel System Agent.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "SaInitDxe.h"
#include "SaInit.h"
#include <Private/SaConfigHob.h>
#include <Protocol/PciEnumerationComplete.h>
#include <MemInfoHob.h>

///
/// Global Variables
///
extern SA_CONFIG_HOB         *mSaConfigHob;

/**
  SystemAgent Dxe Initialization.

  @param[in] ImageHandle             Handle for the image of this driver
  @param[in] SystemTable             Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
SaInitEntryPointDxe (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                Status;
  VOID                      *Registration;

  DEBUG ((DEBUG_INFO, "SaInitDxe Start\n"));

  SaInitEntryPoint ();

  Status = SaAcpiInit (ImageHandle);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Create PCI Enumeration Completed callback for SA
  ///
  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_CALLBACK,
    SaPciEnumCompleteCallback,
    NULL,
    &Registration
    );

  DEBUG ((DEBUG_INFO, "SaInitDxe End\n"));

  return EFI_SUCCESS;
}

/**
  This function gets registered as a callback to perform SA initialization before EndOfDxe

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
SaPciEnumCompleteCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS          Status;
  VOID                *ProtocolPointer;

  DEBUG ((DEBUG_INFO, "SaPciEnumCompleteCallback Start\n"));
  ///
  /// Check if this is first time called by EfiCreateProtocolNotifyEvent() or not,
  /// if it is, we will skip it until real event is triggered
  ///
  Status = gBS->LocateProtocol (&gEfiPciEnumerationCompleteProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }

  gBS->CloseEvent (Event);

  Status = PegInitBeforeEndOfDxe ();
  if (EFI_SUCCESS != Status) {
    DEBUG ((DEBUG_WARN, "[SA] Pcie initialization before EndOfDxe Error, Status = %r \n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  SaSaveRestore ();
  SaSecurityInit ();
  UpdateDmarPciEnumCompleteCallback ();

  DEBUG ((DEBUG_INFO, "SaPciEnumCompleteCallback End\n"));
  return;
}

/**
  This function locks the PAM register as part of the SA Security requirements.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.

**/
VOID
EFIAPI
SaPamLockDxe (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  DEBUG ((DEBUG_INFO, "SaPamLockDxe Start\n"));

  SaPamLock ();

  DEBUG ((DEBUG_INFO, "SaPamLockDxe End\n"));
}
