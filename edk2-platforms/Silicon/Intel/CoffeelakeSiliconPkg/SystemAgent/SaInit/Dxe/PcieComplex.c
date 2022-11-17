/** @file
  This file will perform SA PCIE Root Complex initialization.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PciExpressInit.h"
#include <Private/Library/SaPcieLib.h>
#include "PcieComplex.h"
#include <Private/Protocol/SaIotrapSmi.h>
#include "SaInit.h"

///
/// Global variables
///
UINT16                                 mSaIotrapSmiAddress;
extern SA_CONFIG_HOB                   *mSaConfigHob;

///
/// Functions
///
/**
    This function gets registered as a callback to perform all SA late initialization

    @param[in] Event     - A pointer to the Event that triggered the callback.
    @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
SaLateInitSmiCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS                 Status;
  SA_IOTRAP_SMI_PROTOCOL     *SaIotrapSmiProtocol;

  if (mSaIotrapSmiAddress == 0) {
    //
    // Use global variable instead of protocol data since it maybe tampered in unsecure environment
    // Get IOTrap address when first time this routine calling (gEfiPciEnumerationCompleteProtocolGuid callback)
    //
    SaIotrapSmiProtocol = NULL;
    Status = gBS->LocateProtocol (&gSaIotrapSmiProtocolGuid, NULL, (VOID **) &SaIotrapSmiProtocol);
    ASSERT_EFI_ERROR (Status);
    if (SaIotrapSmiProtocol != NULL) {
      mSaIotrapSmiAddress = SaIotrapSmiProtocol->SaIotrapSmiAddress;
    }
  }

  ASSERT (mSaIotrapSmiAddress != 0);
  if (mSaIotrapSmiAddress != 0) {
    //
    // Generate IOTRAP SMI immediately
    //
    DEBUG ((DEBUG_INFO, "[SA] Issue IOTRAP SMI %X\n", mSaIotrapSmiAddress));
    IoWrite8 (mSaIotrapSmiAddress, 0);
  }
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }
  return;
}

/**
  This function performs Peg initialization before EndOfDxe.
  @note This function will be executed as gEfiPciEnumerationCompleteProtocolGuid protocol callback and assumed SA DXE/SMM drivers have been dispatched.

  @retval EFI_SUCCESS   - Always.
**/
EFI_STATUS
PegInitBeforeEndOfDxe (
  VOID
  )
{
  EFI_EVENT                       ReadyToBoot;
  EFI_STATUS                      Status;
  BOOLEAN                         AspmHasBeenHandled;

  DEBUG ((DEBUG_INFO, "[SA] Pcie before EndOfDxe callback.\n"));
  AspmHasBeenHandled = FALSE;
  ///
  /// SMM mode ASPM handling
  /// Check if supported and enabled
  ///
  if ((mSaConfigHob != NULL) && (mSaConfigHob->InitPcieAspmAfterOprom == TRUE)) {
    ///
    /// Do the Phase 1 SMI callback
    /// This will enumerate PCIe downstream devices
    ///
    SaLateInitSmiCallback (NULL, NULL);

    if (mSaIotrapSmiAddress != 0) {
      ///
      /// Create an ReadyToBoot call back event to do the Phase 3 SMI callback
      /// This will handle PEG ASPM programming after OROM execution
      /// Note: Phase 2 SMI callback will be triggered in EndOfDxe callback
      ///       to initialize rest of PCIe settings prior to OPROM
      ///
      Status = EfiCreateEventReadyToBootEx (
                 TPL_NOTIFY,
                 (EFI_EVENT_NOTIFY) SaLateInitSmiCallback,
                 NULL,
                 &ReadyToBoot
                 );
      ASSERT_EFI_ERROR (Status);
      AspmHasBeenHandled = TRUE;
    }
  }

  ///
  /// DXE mode ASPM handling
  /// Check if SMM mode already taken care all things
  /// TRUE to skip DXE mode task. Otherwise do DXE mode ASPM initialization
  ///
  if (AspmHasBeenHandled == FALSE) {

  }

  return EFI_SUCCESS;
}

/**
  This function performs SA registers Saving/Restoring in EndOfDxe callback

  @retval EFI_SUCCESS     - Save/restore has done
  @retval EFI_UNSUPPORTED - Save/restore not done successfully
**/
EFI_STATUS
SaSaveRestore (
  VOID
  )
{
  BOOLEAN                         SaveRestoreHasBeenHandled;
  UINT8                           SmiData;

  SaveRestoreHasBeenHandled = FALSE;

  if ((mSaConfigHob != NULL) && (mSaConfigHob->InitPcieAspmAfterOprom == TRUE)) {
    ///
    /// Generate the Phase 2 of SA SMI to do SA chipset save/restore and security lock
    ///
    SaLateInitSmiCallback (NULL, NULL);

    if (mSaIotrapSmiAddress != 0) {
      ///
      /// Store IOTRAP SMI address into Boot Script save table
      /// This is required to trigger this IOTRAP during S3 resume to restore all settings
      ///
      SmiData = 0;
      S3BootScriptSaveIoWrite (
        S3BootScriptWidthUint8,
        (UINTN) mSaIotrapSmiAddress,
        1,
        &SmiData
        );
      SaveRestoreHasBeenHandled = TRUE;
    }
  }

  ///
  /// Check if SMM mode already taken care this task
  ///
  if (SaveRestoreHasBeenHandled == TRUE) {
    return EFI_SUCCESS;
  } else {
    return EFI_UNSUPPORTED;
  }
}
