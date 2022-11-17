/** @file
  Support for IO expander TCA6424.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/GpioExpanderLib.h>
#include <Library/I2cAccessLib.h>

//
// Addresses of registers inside expander
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT8  mInputRegister[3]    = {0x0,0x1,0x2};
GLOBAL_REMOVE_IF_UNREFERENCED UINT8  mOutputRegister[3]   = {0x4,0x5,0x6};
GLOBAL_REMOVE_IF_UNREFERENCED UINT8  mConfigRegister[3]   = {0xC,0xD,0xE};
GLOBAL_REMOVE_IF_UNREFERENCED UINT8  mPolarityRegister[3] = {0x8,0x9,0xA};

#define PCH_SERIAL_IO_I2C4                 4
#define TCA6424_I2C_ADDRESS 0x22
#define PINS_PER_REGISTER                  8
#define GPIO_EXP_PIN_DIRECTION_OUT         1
#define GPIO_EXP_PIN_DIRECTION_IN          0
#define GPIO_EXP_PIN_POLARITY_NORMAL       0
#define GPIO_EXP_PIN_POLARITY_INVERTED     1
#define GPIO_EXP_SET_OUTPUT                0
#define GPIO_EXP_SET_DIR                   1
#define GPIO_EXP_GET_INPUT                 2
#define GPIO_EXP_SET_POLARITY              3
#define AUTO_INCREMENT 0x80

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
  )
{
  *Controller = PCH_SERIAL_IO_I2C4;
  return EFI_SUCCESS;
}

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
GpioExpGetRegister (
  IN UINTN Bar0,
  IN UINT8 Address,
  IN UINT8 Register
  )
{
  UINT8 WriBuf[1];
  UINT8 ReBuf[1] = {0};

  WriBuf[0] = Register;
  I2cWriteRead (Bar0, TCA6424_I2C_ADDRESS + Address, 1, WriBuf, 1, ReBuf, WAIT_1_SECOND);

  return ReBuf[0];
}
/**
  Set the input register to a give value mentioned in the function.

  This function is to Programm the data value to the Expander
  Register by following the I2C Protocol communication.

  @param[in]  Bar0       Bar address of the SerialIo Controller
  @param[in]  Address    Expander Value with in the Contoller
  @param[in]  Register   Address of Input/Output/Configure/Polarity
                         registers with in the Expander
  @param[in]  Value      Value to set in the mentioned the register
**/
VOID
GpioExpSetRegister (
  IN UINTN Bar0,
  IN UINT8 Address,
  IN UINT8 Register,
  IN UINT8 Value
  )
{
  UINT8 WriBuf[2];

  WriBuf[0] = Register;
  WriBuf[1] = Value;

  I2cWriteRead (Bar0, TCA6424_I2C_ADDRESS + Address, 2, WriBuf, 0, NULL, WAIT_1_SECOND);
}
/**
  Set the input register to a give value mentioned in the function.

  This function is to update the status of the Gpio Expander
  pin based on the input Operation value of the caller.This
  function calculates the exact address of the register with
  the help of the Register Bank

  @param[in]  Controller  SerialIo Controller value
  @param[in]  Expander    Expander Value with in the Contoller
  @param[in]  Pin         Pin with in the Expnader Value
  @param[in]  Value       none
  @param[in]  Operation   Type of operation (Setoutput/Setdirection
                          /Getinput/Setpolarity)
  @retval     UINT8       Final Value returned from the register
**/
UINT8
GpioExpDecodeRegAccess (
  IN UINT8 Controller,
  IN UINT8 Expander,
  IN UINT8 Pin,
  IN UINT8 Value,
  IN UINT8 Operation
  )
{
  UINT8* RegisterBank;
  UINT8 OldValue;
  UINT8 NewValue;
  UINT8 RegisterAddress;
  UINT8 PinNumber;
  UINT8 ReturnValue = 0;

  DEBUG ((DEBUG_INFO, "GpioExpDecodeRegAccess() %x:%x:%x:%x:%x\n", Controller, Expander, Pin, Value, Operation));
  ASSERT(Controller<6);
  ASSERT(Expander<2);
  ASSERT(Pin<24);
  ASSERT(Value<2);
  ASSERT(Operation<4);
  //
  // Find the register Address value based on the OPeration
  //
  switch(Operation) {
    case GPIO_EXP_SET_OUTPUT:
      RegisterBank = mOutputRegister;
      break;
    case GPIO_EXP_SET_DIR:
      RegisterBank = mConfigRegister;
      break;
    case GPIO_EXP_GET_INPUT:
      RegisterBank = mInputRegister;
      break;
    case GPIO_EXP_SET_POLARITY:
      RegisterBank = mPolarityRegister;
      break;
    default:
      ASSERT(FALSE);
      return 0;
    }
  //
  // Each bit of register represents each Pin
  // calaulate the register address and Pinnumber(offset with in register)
  //
  if (Pin >= 24) {
    //
    // Avoid out-of-bound usage of RegisterBank
    //
    return 0;
  }

  RegisterAddress = RegisterBank[(Pin/PINS_PER_REGISTER)];
  PinNumber = Pin%PINS_PER_REGISTER;

  OldValue = GpioExpGetRegister(FindSerialIoBar(Controller, 0), Expander, RegisterAddress);
  //
  // If it to get the data ,just returned otherwise mark the input value and write the register
  //
  if (Operation == GPIO_EXP_GET_INPUT) {
    ReturnValue = 0x1 & (OldValue>>PinNumber);
  } else {
    NewValue = OldValue;
    NewValue &= ~(BIT0<<PinNumber);
    NewValue |= (Value<<PinNumber);
    if(NewValue!=OldValue) {
      GpioExpSetRegister(FindSerialIoBar(Controller, 0), Expander, RegisterAddress, NewValue);
    }
  }
  return ReturnValue;
}
/**
  Set the Output value for the given Expander Gpio pin.

  This function is to Set the Output value for the GPIO
  Pin within the giving Expander.

  @param[in]  Expander    Expander Value with in the Contoller
  @param[in]  Pin         Pin with in the Expnader Value
  @param[in]  Value       none

**/
VOID
GpioExpSetOutput (
  IN UINT8 Expander,
  IN UINT8 Pin,
  IN UINT8 Value
  )
{
  UINT8 Controller;
  if(!EFI_ERROR(GpioExpGetController(&Controller))) {
    GpioExpDecodeRegAccess(Controller,Expander,Pin,Value,GPIO_EXP_SET_OUTPUT);
  }
}
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
  IN UINT8 Value
  )
{

  UINT8 Controller;
  if(!EFI_ERROR(GpioExpGetController(&Controller))) {
    GpioExpDecodeRegAccess(Controller,Expander,Pin,Value,GPIO_EXP_SET_DIR);
  }
}


/**
  Get the input value for the given Expander Gpio pin.

  This function is to get the input value for the GPIO
  Pin within the giving Expander.

  @param[in]  Expander    Expander Value with in the Contoller
  @param[in]  Pin         Pin with in the Expnader Value

  @retval     UINT8       Final Value returned from the register
**/
UINT8
GpioExpGetInput (
  IN UINT8 Expander,
  IN UINT8 Pin
  )
{
  UINT8 Controller;
  if(!EFI_ERROR(GpioExpGetController(&Controller))) {
    return GpioExpDecodeRegAccess(Controller,Expander,Pin,0,GPIO_EXP_GET_INPUT);
  }
  return 0;
}

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
  )
{
  UINT8 WriteBuf[4];
  UINT8 Controller;

  GpioExpGetController(&Controller);

  WriteBuf[0] = mOutputRegister[0] + AUTO_INCREMENT;
  WriteBuf[1] = Output & 0xFF;
  WriteBuf[2] = (Output>>8) & 0xFF;
  WriteBuf[3] = (Output>>16) & 0xFF;
  I2cWriteRead( FindSerialIoBar(Controller,0), TCA6424_I2C_ADDRESS+Expander, 4, WriteBuf, 0, NULL, WAIT_1_SECOND);
  WriteBuf[0] = mPolarityRegister[0] + AUTO_INCREMENT;
  WriteBuf[1] = Polarity & 0xFF;
  WriteBuf[2] = (Polarity>>8) & 0xFF;
  WriteBuf[3] = (Polarity>>16) & 0xFF;
  I2cWriteRead( FindSerialIoBar(Controller,0), TCA6424_I2C_ADDRESS+Expander, 4, WriteBuf, 0, NULL, WAIT_1_SECOND);
  WriteBuf[0] = mConfigRegister[0] + AUTO_INCREMENT;
  WriteBuf[1] = Direction & 0xFF;
  WriteBuf[2] = (Direction>>8) & 0xFF;
  WriteBuf[3] = (Direction>>16) & 0xFF;
  I2cWriteRead( FindSerialIoBar(Controller,0), TCA6424_I2C_ADDRESS+Expander, 4, WriteBuf, 0, NULL, WAIT_1_SECOND);

}


