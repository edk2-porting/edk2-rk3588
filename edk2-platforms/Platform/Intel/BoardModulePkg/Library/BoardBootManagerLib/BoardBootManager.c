/** @file
  This file include board specific boot manager callbacks

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/


#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PlatformBootManagerLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/PrintLib.h>
#include <Library/PerformanceLib.h>
#include <Library/BoardBootManagerLib.h>


BOOLEAN    mHotKeypressed = FALSE;
EFI_EVENT  HotKeyEvent    = NULL;
UINTN      mBootMenuOptionNumber;

/**
  This function is called each second during the boot manager waits timeout.

  @param TimeoutRemain  The remaining timeout.
**/
VOID
EFIAPI
BoardBootManagerWaitCallback (
  UINT16          TimeoutRemain
  )
{
  EFI_STATUS                    Status;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL  *TxtInEx;
  EFI_KEY_DATA                  KeyData;
  BOOLEAN                       PausePressed;

  //
  // Pause on PAUSE key
  //
  Status = gBS->HandleProtocol (gST->ConsoleInHandle, &gEfiSimpleTextInputExProtocolGuid, (VOID **) &TxtInEx);
  ASSERT_EFI_ERROR (Status);

  PausePressed = FALSE;

  while (TRUE) {
    Status = TxtInEx->ReadKeyStrokeEx (TxtInEx, &KeyData);
    if (EFI_ERROR (Status)) {
      break;
    }

    if (KeyData.Key.ScanCode == SCAN_PAUSE) {
      PausePressed = TRUE;
      break;
    }
  }

  //
  // Loop until non-PAUSE key pressed
  //
  while (PausePressed) {
    Status = TxtInEx->ReadKeyStrokeEx (TxtInEx, &KeyData);
    if (!EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_INFO, "[PauseCallback] %x/%x %x/%x\n",
        KeyData.Key.ScanCode, KeyData.Key.UnicodeChar,
        KeyData.KeyState.KeyShiftState, KeyData.KeyState.KeyToggleState
        ));
      PausePressed = (BOOLEAN) (KeyData.Key.ScanCode == SCAN_PAUSE);
    }
  }
}

/**
  The function is called when no boot option could be launched,
  including platform recovery options and options pointing to applications
  built into firmware volumes.

  If this function returns, BDS attempts to enter an infinite loop.
**/
VOID
EFIAPI
BoardBootManagerUnableToBoot (
  VOID
  )
{
  EFI_STATUS                   Status;
  EFI_BOOT_MANAGER_LOAD_OPTION BootDeviceList;
  CHAR16                       OptionName[sizeof ("Boot####")];

  if (mBootMenuOptionNumber == LoadOptionNumberUnassigned) {
    return;
  }
  UnicodeSPrint (OptionName, sizeof (OptionName), L"Boot%04x", mBootMenuOptionNumber);
  Status = EfiBootManagerVariableToLoadOption (OptionName, &BootDeviceList);
  if (EFI_ERROR (Status)) {
    return;
  }
  for (;;) {
    EfiBootManagerBoot (&BootDeviceList);
  }
}
