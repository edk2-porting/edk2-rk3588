/** @file
  Main file for Ps2 keyboard controller library.

  Copyright (c) 2019 - 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "BdsPs2KbcLib.h"

GLOBAL_REMOVE_IF_UNREFERENCED PLATFORM_KEYBOARD_DEVICE_PATH  gKeyboardDevicePath = {
  gPciRootBridge,
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8) (sizeof (PCI_DEVICE_PATH)),
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8)
      }
    },
    0, // Function, patched in EnumPs2Keyboard
    0  // Device, patched in EnumPs2Keyboard
  },
  {
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
        (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    EISA_PNP_ID(0x0303),
    0
  },
  gEndEntire
};

/**
  Check if PS2 keyboard is conntected, by sending ECHO command.
  @retval                       TRUE if connected FALSE otherwise
**/
BOOLEAN
DetectPs2Keyboard (
  VOID
  )
{
  UINT32                TimeOut;
  UINT8                 Data;
  UINT32                SumTimeOut;
  BOOLEAN               FoundPs2Kbc;

  TimeOut     = 0;
  FoundPs2Kbc       = FALSE;

  //
  // Wait for input buffer empty
  //
  for (TimeOut = 0; TimeOut < PS2_KEYBOARD_TIMEOUT; TimeOut += 30) {
    if ((IoRead8 (KEYBOARD_8042_STATUS_REGISTER) & 0x02) == 0) {
      FoundPs2Kbc = TRUE;
      break;
    }
    MicroSecondDelay (30);
  }

  if (!FoundPs2Kbc) {
    return FALSE;
  }

  //
  // Send echo command
  //
  IoWrite8 (KEYBOARD_8042_DATA_REGISTER, KBC_INPBUF_VIA60_KBECHO);

  //
  // Init variables
  //
  FoundPs2Kbc       = FALSE;
  TimeOut     = 0;
  SumTimeOut  = 0;
  Data = 0;

  //
  // Read from 8042 (multiple times if needed)
  // until the expected value appears
  // use SumTimeOut to control the iteration
  //
  while (1) {
    //
    // Perform a read
    //
    for (TimeOut = 0; TimeOut < PS2_KEYBOARD_TIMEOUT; TimeOut += 30) {
      if (IoRead8 (KEYBOARD_8042_STATUS_REGISTER) & 0x01) {
        Data = IoRead8 (KEYBOARD_8042_DATA_REGISTER);
        break;
      }
      MicroSecondDelay (30);
    }

    SumTimeOut += TimeOut;

    if (Data == KBC_INPBUF_VIA60_KBECHO) {
      FoundPs2Kbc = TRUE;
      break;
    }

    if (SumTimeOut >= PS2_KEYBOARD_WAITFORVALUE_TIMEOUT) {
      break;
    }
  }
  return FoundPs2Kbc;
}

/**
  Check if PS2 keyboard is conntected. If the result of first time is
  error, it will retry again.
  @retval                       TRUE if connected FALSE otherwise
**/
BOOLEAN
IsPs2KeyboardConnected (
  VOID
  )
{
  BOOLEAN Result;
  Result = DetectPs2Keyboard ();

  if (!Result) {
    //
    // If there is no ps2 keyboard detected for the 1st time, retry again.
    //
    Result = DetectPs2Keyboard ();
  }
  return Result;
}


/**
  Updates the ConIn variable with Ps2 Keyboard device path,
  if it doesn't already exists in ConIn and ConInDev.
**/
VOID
AddPs2Keyboard (
  VOID
  )
{
  SIO_PCI_ISA_BRIDGE_DEVICE_INFO *SioIsaInfo;

  DEBUG ((DEBUG_INFO, "[AddPs2Keyboard]\n"));

  SioIsaInfo = (SIO_PCI_ISA_BRIDGE_DEVICE_INFO*) FixedPcdGetPtr (PcdSuperIoPciIsaBridgeDevice);

  //
  // patch IsaBridge device and and function
  //
  gKeyboardDevicePath.IsaBridge.Device = SioIsaInfo->Device;
  gKeyboardDevicePath.IsaBridge.Function = SioIsaInfo->Funtion;

  //
  // Append Ps2 Keyboard into "ConIn"
  //
  EfiBootManagerUpdateConsoleVariable (ConIn, (EFI_DEVICE_PATH_PROTOCOL *) &gKeyboardDevicePath, NULL);

  //
  // Append Ps2 Keyboard into "ConInDev"
  //
  EfiBootManagerUpdateConsoleVariable (ConInDev, (EFI_DEVICE_PATH_PROTOCOL *) &gKeyboardDevicePath, NULL);
}


/**
  Constructor for the Ps2 keyboard controller library.

  @param ImageHandle    the image handle of the process
  @param SystemTable    the EFI System Table pointer

  @retval EFI_SUCCESS        the shell command handlers were installed sucessfully
  @retval EFI_UNSUPPORTED    the shell level required was not found.
**/
EFI_STATUS
EFIAPI
BdsPs2KbcLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINT8                               Ps2KbMsEnable;

  Ps2KbMsEnable = PcdGet8 (PcdPs2KbMsEnable);

  if (Ps2KbMsEnable == 0x1
    && IsPs2KeyboardConnected())
  {
      // add ps2 device path to ConIn and ConInDev
      AddPs2Keyboard ();
  }

  return EFI_SUCCESS;
}
