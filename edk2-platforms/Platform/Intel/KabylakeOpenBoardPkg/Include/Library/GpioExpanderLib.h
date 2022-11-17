/** @file
  Support for IO expander TCA6424.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _GPIO_EXPANDER_LIB_H_
#define _GPIO_EXPANDER_LIB_H_

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <PchAccess.h>
#include <Library/PchSerialIoLib.h>

/**
  Set the Direction value for the given Expander Gpio pin.

  This function is to Set the direction value for the GPIO
  Pin within the giving Expander.

  @param[in]  Expander    Expander Value with in the Contoller
  @param[in]  Pin         Pin with in the Expnader Value
  @param[in]  Value       none
**/
VOID
GpioExpSetDirection (
  IN UINT8 Expander,
  IN UINT8 Pin,
  IN UINT8 Direction
  );
/**
  Set the input value for the given Expander Gpio pin.

  This function is to get the input value for the GPIO
  Pin within the giving Expander.

  @param[in]  Expander    Expander Value with in the Contoller
  @param[in]  Pin         Pin with in the Expnader Value
  @param[in]  Value       none

**/
VOID
GpioExpSetPolarity  (
  IN UINT8 Expander,
  IN UINT8 Pin,
  IN UINT8 Polarity
  );
/**
  Set the Output value for the given Expander Gpio pin.

  This function is to Set the Output value for the GPIO
  Pin within the giving Expander.

  @param[in]  Expander    Expander Value with in the Contoller
  @param[in]  Pin         Pin with in the Expnader Value
  @param[in]  Value       none

**/
VOID
GpioExpSetOutput    (
  IN UINT8 Expander,
  IN UINT8 Pin,
  IN UINT8 Value
  );
/**
  Returns the data from register value giving in the input.

  This function is to get the data from the Expander
  Registers by following the I2C Protocol communication


  @param[in]  Bar0       Bar address of the SerialIo Controller
  @param[in]  Address    Expander Value with in the Contoller
  @param[in]  Register   Address of Input/Output/Configure/Polarity
                         registers with in the Expander

  @retval     UINT8      Value returned from the register
**/
UINT8
GpioExpGetInput     (
  IN UINT8 Expander,
  IN UINT8 Pin
  );

/**
  Configures all registers of a single IO Expander in one go.

  @param[in]  Expander    Expander number (0/1)
  @param[in]  Direction   Bit-encoded direction values. BIT0 is for pin0, etc. 0=output, 1=input
  @param[in]  Polarity    Bit-encoded input inversion values. BIT0 is for pin0, etc. 0=normal, 1=inversion
  @param[in]  Output      Bit-encoded output state, ignores polarity, only applicable if direction=INPUT. BIT0 is for pin0, etc. 0=low, 1=high

**/
VOID
GpioExpBulkConfig (
  IN UINT8  Expander,
  IN UINT32 Direction,
  IN UINT32 Polarity,
  IN UINT32 Output
  );

/**
  Returns the Controller on which GPIO expander is present.

  This function returns the Controller value

  @param[out] Controller              Pointer to a Controller value on
                                      which I2C expander is configured.

  @retval     EFI_SUCCESS              non.
**/
EFI_STATUS
GpioExpGetController (
  OUT UINT8 *Controller
  );

#endif
