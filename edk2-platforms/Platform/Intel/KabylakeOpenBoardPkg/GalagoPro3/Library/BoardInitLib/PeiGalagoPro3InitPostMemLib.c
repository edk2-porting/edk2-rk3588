/** @file
  System 76 GalagoPro3 board post-memory initialization.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <SaPolicyCommon.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PciLib.h>
#include <Library/PeiSaPolicyLib.h>
#include <Library/BoardInitLib.h>
#include <PchAccess.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioLib.h>
#include <GpioPinsSklLp.h>
#include <GpioPinsSklH.h>
#include <Library/GpioExpanderLib.h>
#include <SioRegs.h>
#include <Library/PchPcrLib.h>
#include <IoExpander.h>
#include <Library/PcdLib.h>
#include <Library/SiliconInitLib.h>

#include "PeiGalagoPro3InitLib.h"

/**
  N 1XX WU board configuration init function for PEI post memory phase.

  PEI_BOARD_CONFIG_PCD_INIT

  @param  Content  pointer to the buffer contain init information for board init.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_INVALID_PARAMETER   The parameter is NULL.
**/
EFI_STATUS
EFIAPI
GalagoPro3Init (
  VOID
  )
{
  PcdSet32S (PcdHdaVerbTable, (UINTN) &HdaVerbTableAlc286Rvp3);

  //
  // Assign the GPIO table with pin configs to be used for UCMC
  //
  PcdSet32S (PcdBoardUcmcGpioTable, (UINTN)mGpioTableGalagoPro3UcmcDevice);
  PcdSet16S (PcdBoardUcmcGpioTableSize, mGpioTableGalagoPro3UcmcDeviceSize);

  return EFI_SUCCESS;
}

#define EXPANDERS                                 2                    // defines expander's quantity

/**
  Configures GPIO

  @param[in]  GpioTable       Point to Platform Gpio table
  @param[in]  GpioTableCount  Number of Gpio table entries

**/
VOID
ConfigureGpio (
  IN GPIO_INIT_CONFIG                 *GpioDefinition,
  IN UINT16                           GpioTableCount
  )
{
  EFI_STATUS          Status;

  DEBUG ((DEBUG_INFO, "ConfigureGpio() Start\n"));

  Status = GpioConfigurePads (GpioTableCount, GpioDefinition);

  DEBUG ((DEBUG_INFO, "ConfigureGpio() End\n"));
}

VOID
SetBit (
  IN OUT UINT32  *Value,
  IN     UINT32  BitNumber,
  IN     BOOLEAN NewBitValue
  )
{
  if (NewBitValue) {
    *Value |= 1 << BitNumber;
  } else {
    *Value &= ~(1 << BitNumber);
  }
}

/**
  Configures IO Expander GPIO device

  @param[in]  IOExpGpioDefinition  Point to IO Expander Gpio table
  @param[in]  IOExpGpioTableCount  Number of Gpio table entries

**/
void
ConfigureIoExpanderGpio (
  IN IO_EXPANDER_GPIO_CONFIG        *IoExpGpioDefinition,
  IN UINT16                          IoExpGpioTableCount
  )
{
  UINT8               Index;
  UINT32              Direction[EXPANDERS] = {0x00FFFFFF, 0x00FFFFFF};
  UINT32              Level[EXPANDERS] = {0};
  UINT32              Polarity[EXPANDERS] = {0};

   // IoExpander {TCA6424A}
  DEBUG ((DEBUG_INFO, "IO Expander Configuration Start\n"));
  for (Index = 0; Index < IoExpGpioTableCount; Index++) {   //Program IO Expander as per the table defined in PeiPlatformHooklib.c
    SetBit(&Direction[IoExpGpioDefinition[Index].IoExpanderNumber], IoExpGpioDefinition[Index].GpioPinNumber, (BOOLEAN)IoExpGpioDefinition[Index].GpioDirection);
    SetBit(&Level[IoExpGpioDefinition[Index].IoExpanderNumber], IoExpGpioDefinition[Index].GpioPinNumber, (BOOLEAN)IoExpGpioDefinition[Index].GpioLevel);
    SetBit(&Polarity[IoExpGpioDefinition[Index].IoExpanderNumber], IoExpGpioDefinition[Index].GpioPinNumber, (BOOLEAN)IoExpGpioDefinition[Index].GpioInversion);
  }
  for (Index = 0; Index < EXPANDERS; Index++) {
    GpioExpBulkConfig(Index, Direction[Index], Polarity[Index], Level[Index]);
  }
  DEBUG ((DEBUG_INFO, "IO Expander Configuration End\n"));
  return;
}

/**
  Configure GPIO behind IoExpander.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor
  @param[in]  Interface

  @retval     EFI_SUCCESS       Operation success.
**/
VOID
ExpanderGpioInit (
  VOID
  )
{
  ConfigureIoExpanderGpio(mGpioTableIoExpander, mGpioTableIoExpanderSize);
}

/**
  Configure single GPIO pad for touchpanel interrupt

**/
VOID
TouchpanelGpioInit (
  VOID
  )
{
  GPIO_INIT_CONFIG*     TouchpanelPad;
  GPIO_PAD_OWN          PadOwnVal;

  PadOwnVal = 0;
  TouchpanelPad = &mGpioTableGalagoPro3Touchpanel;

  GpioGetPadOwnership (TouchpanelPad->GpioPad, &PadOwnVal);
  if (PadOwnVal == GpioPadOwnHost) {
    GpioConfigurePads (1, TouchpanelPad);
  }
}


/**
  Configure GPIO

**/
VOID
GpioInit (
  VOID
  )
{
  ConfigureGpio (mGpioTableGalagoPro3, mGpioTableGalagoPro3Size);

  TouchpanelGpioInit();

  return;
}


/**
  Configure GPIO and SIO

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
EFIAPI
GalagoPro3BoardInitBeforeSiliconInit (
  VOID
  )
{
  GalagoPro3Init ();

  GpioInit ();
  ExpanderGpioInit ();

  ///
  /// Do Late PCH init
  ///
  LateSiliconInit ();

  return EFI_SUCCESS;
}
