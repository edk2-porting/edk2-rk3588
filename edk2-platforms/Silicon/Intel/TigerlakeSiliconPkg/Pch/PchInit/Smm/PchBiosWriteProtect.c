/** @file
  PCH BIOS Write Protect Driver.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "PchInitSmm.h"
#include <Register/PchRegs.h>
#include <Register/PchRegsLpc.h>
#include <Register/SpiRegs.h>
#include <Library/SpiAccessPrivateLib.h>
#include <Library/PchPciBdfLib.h>

//
// Global variables
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_TCO_SMI_DISPATCH_PROTOCOL     *mPchTcoSmiDispatchProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED PCH_ESPI_SMI_DISPATCH_PROTOCOL    *mEspiSmmDispatchProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED UINT64                            mLpcRegBase;

/**
  This hardware SMI handler will be run every time the BIOS Write Enable bit is set.

  @param[in] DispatchHandle       Not used
**/
VOID
EFIAPI
PchSpiBiosWpCallback (
  IN  EFI_HANDLE                              DispatchHandle
  )
{
  SpiClearBiosWriteProtectDisable ();
}

/**
  This hardware SMI handler will be run every time the BIOS Write Enable bit is set.

  @param[in] DispatchHandle       Not used

**/
VOID
EFIAPI
PchLpcBiosWpCallback (
  IN  EFI_HANDLE                              DispatchHandle
  )
{
  //
  // Disable BIOSWE bit to protect BIOS
  //
  PciSegmentAnd8 ((UINTN) (mLpcRegBase + R_LPC_CFG_BC), (UINT8) ~B_LPC_CFG_BC_WPD);
}

/**
  Entry point for Pch Bios Write Protect driver.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
**/
EFI_STATUS
EFIAPI
InstallPchBiosWriteProtect (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              Handle;

  DEBUG ((DEBUG_INFO, "InstallPchBiosWriteProtect()\n"));

  if (mPchConfigHob->LockDown.BiosLock != TRUE) {
    return EFI_SUCCESS;
  }

  mLpcRegBase = LpcPciCfgBase ();

  DEBUG ((DEBUG_INFO, "Installing BIOS Write Protect SMI handler\n"));
  //
  // Get the PCH TCO SMM dispatch protocol
  //
  mPchTcoSmiDispatchProtocol = NULL;
  Status = gSmst->SmmLocateProtocol (&gPchTcoSmiDispatchProtocolGuid, NULL, (VOID **) &mPchTcoSmiDispatchProtocol);
  ASSERT_EFI_ERROR (Status);
  //
  // Always register an SPI BiosWp callback function to handle TCO BIOSWR SMI
  // NOTE: No matter the BIOS resides behind SPI or not, it needs to handle the SPI BIOS WP SMI
  //       to avoid SMI deadloop on SPI WPD write.
  //
  Handle = NULL;
  Status = mPchTcoSmiDispatchProtocol->SpiBiosWpRegister (
                                         mPchTcoSmiDispatchProtocol,
                                         PchSpiBiosWpCallback,
                                         &Handle
                                         );
  ASSERT_EFI_ERROR (Status);

  //
  // Always register an LPC/eSPI BiosWp callback function to handle TCO BIOSWR SMI
  // NOTE: No matter the BIOS resides behind LPC/eSPI or not, it needs to handle the BIOS WP SMI
  //       to avoid SMI deadloop on LPC/eSPI WPD write.
  //
  if (IsEspiEnabled ()) {
    //
    // Get the PCH ESPI SMM dispatch protocol
    //
    mEspiSmmDispatchProtocol = NULL;
    Status = gSmst->SmmLocateProtocol (&gPchEspiSmiDispatchProtocolGuid, NULL, (VOID **) &mEspiSmmDispatchProtocol);
    ASSERT_EFI_ERROR (Status);

    //
    // Register an ESpiBiosWp callback function to handle BIOSWR SMI
    //
    Handle = NULL;
    Status = mEspiSmmDispatchProtocol->BiosWrProtectRegister (
                                         mEspiSmmDispatchProtocol,
                                         PchLpcBiosWpCallback,
                                         &Handle
                                         );
    ASSERT_EFI_ERROR (Status);
  } else {
    //
    // Register an LPC BiosWp callback function to handle TCO BIOSWR SMI
    //
    Handle = NULL;
    Status = mPchTcoSmiDispatchProtocol->LpcBiosWpRegister (
                                           mPchTcoSmiDispatchProtocol,
                                           PchLpcBiosWpCallback,
                                           &Handle
                                           );
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

