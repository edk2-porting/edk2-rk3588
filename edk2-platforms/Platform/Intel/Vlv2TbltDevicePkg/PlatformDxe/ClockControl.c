/** @file

  Copyright (c) 2004  - 2019, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

Module Name:


  ClockControl.c

Abstract:

  Sets platform/SKU specific clock routing information.



--*/

#include "PlatformDxe.h"
#include <Protocol/CK505ClockPlatformInfo.h>

//
// Default clock routing informtion (All On)
//
EFI_CLOCK_PLATFORM_INFO mDefClockPolicy = {NULL, 0, NULL, 0, NULL, 0};

//
// Clock Settings
//
// Static clock table.
//  This should be used to define any clock settings that are static
//  (Always On or Always Off).  Dynamic clocks should be set to enabled
//  in this table.
//
EFI_STATIC_SIGNALS mAtxStaticClocks[] = {
  {SrcClk8,   Enabled,  All},
  {SrcClk7,   Enabled,  All},
  {SrcClk6,   Enabled,  All},
  {SrcClk5,   Enabled,  All},
  {SrcClk4,   Enabled,  All},
  {SrcClk3,   Enabled,  All},
  {SrcClk2,   Enabled,  All},
  {SrcClk1,   Enabled,  All},
  {SrcClk0,   Enabled,  All},
  {Ref0,      Enabled,  All},
  {Dot96,     Enabled,  All},
  {Usb48,     Enabled,  All},
  {PciClkF5,  Enabled,  All},
  {PciClk0,   Enabled,  All},
  {PciClk2,   Enabled,  All},
  {PciClk3,   Enabled,  All},
  {PciClk4,   Disabled,  All},
  {Cr_B,   EnabledWithSwitch,  All},
};

//
// ClockSxInfo Table
//  This is a list of clocks that need to be set to a known state when the
//  system enters S4 or S5.
//
EFI_STATIC_SIGNALS mAtxSxClocks[] = {
  {SaveClockConfiguration, Disabled, All}
};

//
// ATX settings structure
//
EFI_CLOCK_PLATFORM_INFO mAtxClockSettings = {
  mAtxStaticClocks,
  sizeof(mAtxStaticClocks) / sizeof(mAtxStaticClocks[0]),
  mAtxSxClocks,
  sizeof(mAtxSxClocks) / sizeof(mAtxSxClocks[0])
};

VOID
InitializeClockRouting(
  )
{
  EFI_STATUS                      Status;
  UINTN                           BoardIdVarSize;
  EFI_BOARD_FEATURES              BoardIdVar;
  EFI_CLOCK_PLATFORM_INFO         *ClockPolicy;
  EFI_HANDLE                      Handle;

  ClockPolicy = &mDefClockPolicy;

  //
  // Do modifications based on board type
  //
  BoardIdVarSize = sizeof (EFI_BOARD_FEATURES);
  Status = gRT->GetVariable (
                  BOARD_FEATURES_NAME,
                  &gEfiBoardFeaturesGuid,
                  NULL,
                  &BoardIdVarSize,
                  &BoardIdVar
                  );
  if (!EFI_ERROR (Status)) {

    //
    // Isolate board type information
    //
    BoardIdVar = BoardIdVar & (B_BOARD_FEATURES_FORM_FACTOR_ATX |
                               B_BOARD_FEATURES_FORM_FACTOR_BTX |
                               B_BOARD_FEATURES_FORM_FACTOR_MICRO_ATX |
                               B_BOARD_FEATURES_FORM_FACTOR_MICRO_BTX);

    if (BoardIdVar == B_BOARD_FEATURES_FORM_FACTOR_ATX ||
        BoardIdVar == B_BOARD_FEATURES_FORM_FACTOR_MICRO_ATX) {
      ClockPolicy = &mAtxClockSettings;
    }
  }

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiCk505ClockPlatformInfoGuid,
                  EFI_NATIVE_INTERFACE,
                  ClockPolicy
                  );
  ASSERT_EFI_ERROR(Status);

}


