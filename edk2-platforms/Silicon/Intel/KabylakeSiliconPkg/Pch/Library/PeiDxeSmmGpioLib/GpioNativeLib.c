/** @file
  This file contains routines for GPIO native and chipset specific usage

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "GpioLibrary.h"

//
// Chipset specific data
//
//SerialIo
extern GPIO_PAD_NATIVE_FUNCTION mPchLpI2cGpio[PCH_LP_SERIALIO_MAX_I2C_CONTROLLERS][PCH_SERIAL_IO_PINS_PER_I2C_CONTROLLER];
extern GPIO_PAD_NATIVE_FUNCTION mPchHI2cGpio[PCH_H_SERIALIO_MAX_I2C_CONTROLLERS][PCH_SERIAL_IO_PINS_PER_I2C_CONTROLLER];
extern GPIO_PAD_NATIVE_FUNCTION mPchLpUartGpio[PCH_SERIALIO_MAX_UART_CONTROLLERS][PCH_SERIAL_IO_PINS_PER_UART_CONTROLLER];
extern GPIO_PAD_NATIVE_FUNCTION mPchHUartGpio[PCH_SERIALIO_MAX_UART_CONTROLLERS][PCH_SERIAL_IO_PINS_PER_UART_CONTROLLER];
extern GPIO_PAD_NATIVE_FUNCTION mPchLpSpiGpio[PCH_SERIALIO_MAX_SPI_CONTROLLERS][PCH_SERIAL_IO_PINS_PER_SPI_CONTROLLER];
extern GPIO_PAD_NATIVE_FUNCTION mPchHSpiGpio[PCH_SERIALIO_MAX_SPI_CONTROLLERS][PCH_SERIAL_IO_PINS_PER_SPI_CONTROLLER];

//SATA
extern GPIO_PAD_NATIVE_FUNCTION mPchLpSataPortResetToGpioMap[PCH_LP_AHCI_MAX_PORTS];
extern GPIO_PAD_NATIVE_FUNCTION mPchHSataPortResetToGpioMap[PCH_H_AHCI_MAX_PORTS];
extern GPIO_PAD_NATIVE_FUNCTION mPchLpSataDevSlpPinToGpioMap[PCH_LP_AHCI_MAX_PORTS];
extern GPIO_PAD_NATIVE_FUNCTION mPchHSataDevSlpPinToGpioMap[PCH_H_AHCI_MAX_PORTS];

//
// SKL specific
//
extern GPIO_GROUP_INFO mPchLpGpioGroupInfo[V_PCH_LP_GPIO_GROUP_MAX];
extern GPIO_GROUP_INFO mPchHGpioGroupInfo[V_PCH_H_GPIO_GROUP_MAX];

/**
  This procedure will set GPIO mode

  @param[in]  GpioPad             GPIO pad
  @param[out] PadModeValue        GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
SetGpioPadMode (
  IN GPIO_PAD                GpioPad,
  IN GPIO_PAD_MODE           PadModeValue
  )
{
  UINT32               PadCfgOrMask;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  if (PadModeValue != (GPIO_PAD_MODE)GpioHardwareDefault) {

    PadCfgOrMask = (((PadModeValue & B_GPIO_PAD_MODE_MASK) >> (N_GPIO_PAD_MODE_BIT_POS + 1)) << N_PCH_GPIO_PAD_MODE);

    GpioWritePadCfgReg (
      GpioPad,
      0,
      (UINT32)~B_PCH_GPIO_PAD_MODE,
      PadCfgOrMask
      );
  }

  return EFI_SUCCESS;
}

/**
  This procedure will get GPIO mode

  @param[in]  GpioPad             GPIO pad
  @param[out] PadModeValue        GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GpioPad
**/
EFI_STATUS
GetGpioPadMode (
  IN  GPIO_PAD                 GpioPad,
  OUT GPIO_PAD_MODE            *PadModeValue
  )
{
  UINT32        PadCfgRegValue;

  if (!GpioIsPadValid (GpioPad)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (!GpioIsPadHostOwned (GpioPad)) {
    return EFI_UNSUPPORTED;
  }

  GpioReadPadCfgReg (
    GpioPad,
    0,
    &PadCfgRegValue
    );

  *PadModeValue = (GPIO_PAD_MODE)(((PadCfgRegValue & B_PCH_GPIO_PAD_MODE) >> (N_PCH_GPIO_PAD_MODE - (N_GPIO_PAD_MODE_BIT_POS + 1))) | (0x1 << N_GPIO_PAD_MODE_BIT_POS));

  return EFI_SUCCESS;
}


/**
  This procedure will retrieve address and length of GPIO info table

  @param[out]  GpioGroupInfoTableLength   Length of GPIO group table

  @retval Pointer to GPIO group table

**/
GPIO_GROUP_INFO*
GpioGetGroupInfoTable (
  OUT UINTN               *GpioGroupInfoTableLength
  )
{
  if (GetPchSeries () == PchLp) {
    *GpioGroupInfoTableLength = sizeof (mPchLpGpioGroupInfo) / sizeof (GPIO_GROUP_INFO);
    return mPchLpGpioGroupInfo;
  } else {
    *GpioGroupInfoTableLength = sizeof (mPchHGpioGroupInfo) / sizeof (GPIO_GROUP_INFO);
    return mPchHGpioGroupInfo;
  }
}


/**
  This procedure is used to check if GpioPad is valid for certain chipset

  @param[in]  GpioPad             GPIO pad

  @retval TRUE                    This pin is valid on this chipset
          FALSE                   Incorrect pin
**/
BOOLEAN
GpioIsCorrectPadForThisChipset (
  IN  GPIO_PAD        GpioPad
  )
{
  PCH_SERIES PchSeries;

  PchSeries = GetPchSeries ();

  if ((PchSeries == PchH) && (GPIO_GET_CHIPSET_ID (GpioPad) == GPIO_SKL_H_CHIPSET_ID)) {
    return TRUE;
  } else if ((PchSeries == PchLp) && (GPIO_GET_CHIPSET_ID (GpioPad) == GPIO_SKL_LP_CHIPSET_ID)) {
    return TRUE;
  }

  return FALSE;
}


/**
  This procedure will get number of pads for certain GPIO group

  @param[in] Group            GPIO group number

  @retval Value               Pad number for group
                              If illegal group number then return 0
**/
UINT32
GpioGetPadPerGroup (
  IN GPIO_GROUP      Group
  )
{
  GPIO_GROUP_INFO     *GpioGroupInfo;
  UINTN               GpioGroupInfoLength;
  UINT32              GroupIndex;
  //
  // Check if group argument exceeds GPIO GROUP INFO array
  //
  GpioGroupInfo = GpioGetGroupInfoTable (&GpioGroupInfoLength);
  GroupIndex = GpioGetGroupIndexFromGroup (Group);

  if ((UINTN) GroupIndex >= GpioGroupInfoLength) {
    ASSERT (FALSE);
    return 0;
  } else {
    return GpioGroupInfo[GroupIndex].PadPerGroup;
  }
}

/**
  This procedure will get number of groups

  @param[in] none

  @retval Value               Group number
**/
UINT8
GpioGetNumberOfGroups (
  VOID
  )
{
  if (GetPchSeries () == PchLp) {
    return V_PCH_LP_GPIO_GROUP_MAX;
  } else {
    return V_PCH_H_GPIO_GROUP_MAX;
  }
}
/**
  This procedure will get lowest group

  @param[in] none

  @retval Value               Lowest Group
**/
GPIO_GROUP
GpioGetLowestGroup (
  VOID
  )
{
  if (GetPchSeries () == PchLp) {
    return (UINT32) GPIO_SKL_LP_GROUP_GPP_A;
  } else {
    return (UINT32) GPIO_SKL_H_GROUP_GPP_A;
  }
}
/**
  This procedure will get highest group

  @param[in] none

  @retval Value               Highest Group
**/
GPIO_GROUP
GpioGetHighestGroup (
  VOID
  )
{
  if (GetPchSeries () == PchLp) {
    return (UINT32) GPIO_SKL_LP_GROUP_GPD;
  } else {
    return (UINT32) GPIO_SKL_H_GROUP_GPD;
  }
}

/**
  This procedure will get group number

  @param[in] GpioPad          Gpio Pad

  @retval Value               Group number
**/
GPIO_GROUP
GpioGetGroupFromGpioPad (
  IN GPIO_PAD         GpioPad
  )
{
  return GPIO_GET_GROUP_FROM_PAD (GpioPad);
}

/**
  This procedure will get group index (0 based)

  @param[in] GpioPad          Gpio Pad

  @retval Value               Group Index
**/
UINT32
GpioGetGroupIndexFromGpioPad (
  IN GPIO_PAD        GpioPad
  )
{
  return (UINT32) GPIO_GET_GROUP_INDEX_FROM_PAD (GpioPad);
}

/**
  This procedure will get group index (0 based) from group

  @param[in] GpioGroup        Gpio Group

  @retval Value               Group Index
**/
UINT32
GpioGetGroupIndexFromGroup (
  IN GPIO_GROUP        GpioGroup
  )
{
  return (UINT32) GPIO_GET_GROUP_INDEX (GpioGroup);
}

/**
  This procedure will get pad number (0 based) from Gpio Pad

  @param[in] GpioPad          Gpio Pad

  @retval Value               Pad Number
**/
UINT32
GpioGetPadNumberFromGpioPad (
  IN GPIO_PAD        GpioPad
  )
{
  return (UINT32) GPIO_GET_PAD_NUMBER (GpioPad);
}
/**
  This procedure will return GpioPad from Group and PadNumber

  @param[in] Group              GPIO group
  @param[in] PadNumber          GPIO PadNumber

  @retval GpioPad               GpioPad
**/
GPIO_PAD
GpioGetGpioPadFromGroupAndPadNumber (
  IN GPIO_GROUP      Group,
  IN UINT32          PadNumber
  )
{
  if (GetPchSeries () == PchLp) {
    return GPIO_PAD_DEF (Group,PadNumber);
  } else {
    return GPIO_PAD_DEF (Group,PadNumber);
  }
}

/**
  This procedure will return GpioPad from GroupIndex and PadNumber

  @param[in] GroupIndex         GPIO GroupIndex
  @param[in] PadNumber          GPIO PadNumber

  @retval GpioPad               GpioPad
**/
GPIO_PAD
GpioGetGpioPadFromGroupIndexAndPadNumber (
  IN UINT32          GroupIndex,
  IN UINT32          PadNumber
  )
{
  GPIO_GROUP Group;

  if (GetPchSeries () == PchLp) {
    Group = GPIO_GROUP_DEF (GroupIndex, GPIO_SKL_LP_CHIPSET_ID);
    return GPIO_PAD_DEF (Group,PadNumber);
  } else {
    Group = GPIO_GROUP_DEF (GroupIndex, GPIO_SKL_H_CHIPSET_ID);
    return GPIO_PAD_DEF (Group,PadNumber);
  }
}


/**
  This function sets SerialIo I2C controller pins into native mode

  @param[in]  SerialIoI2cControllerNumber   I2C controller

  @retval Status
**/
EFI_STATUS
GpioSetSerialIoI2cPinsIntoNativeMode (
  IN  UINT32            SerialIoI2cControllerNumber
  )
{
  EFI_STATUS               Status;
  UINTN                    Index;
  GPIO_PAD_NATIVE_FUNCTION (*I2cGpio) [PCH_SERIAL_IO_PINS_PER_I2C_CONTROLLER];

  Status = EFI_SUCCESS;

  if (GetPchSeries () == PchLp) {
    ASSERT (SerialIoI2cControllerNumber < PCH_LP_SERIALIO_MAX_I2C_CONTROLLERS);
    I2cGpio = mPchLpI2cGpio;
  } else {
    ASSERT (SerialIoI2cControllerNumber < PCH_H_SERIALIO_MAX_I2C_CONTROLLERS);
    I2cGpio = mPchHI2cGpio;
  }

  for (Index = 0; Index < PCH_SERIAL_IO_PINS_PER_I2C_CONTROLLER; Index++) {
    Status = SetGpioPadMode (I2cGpio[SerialIoI2cControllerNumber][Index].Pad, I2cGpio[SerialIoI2cControllerNumber][Index].Mode);
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
    GpioSetInputInversion (I2cGpio[SerialIoI2cControllerNumber][Index].Pad, 0);
  }
  return Status;
}

/**
  This function sets SerialIo I2C controller pins tolerance

  @param[in]  SerialIoI2CControllerNumber   I2C controller
  @param[in]  Pad1v8Tolerance               TRUE:  Enable  1v8 Pad tolerance
                                            FALSE: Disable 1v8 Pad tolerance

  @retval Status
**/
EFI_STATUS
GpioSetSerialIoI2CPinsTolerance (
  IN  UINT32            SerialIoI2cControllerNumber,
  IN  BOOLEAN           Pad1v8Tolerance
  )
{
  EFI_STATUS               Status;
  UINTN                    Index;
  GPIO_PAD_OWN             PadOwnership;
  GPIO_PAD_NATIVE_FUNCTION (*I2cGpio) [PCH_SERIAL_IO_PINS_PER_I2C_CONTROLLER];
  GPIO_CONFIG              GpioData;

  Status = EFI_SUCCESS;

  if (GetPchSeries () == PchLp) {
    ASSERT (SerialIoI2cControllerNumber < PCH_LP_SERIALIO_MAX_I2C_CONTROLLERS);
    I2cGpio = mPchLpI2cGpio;
  } else {
    ASSERT (SerialIoI2cControllerNumber < PCH_H_SERIALIO_MAX_I2C_CONTROLLERS);
    I2cGpio = mPchHI2cGpio;
  }

  ZeroMem (&GpioData, sizeof (GPIO_CONFIG));
  if (Pad1v8Tolerance) {
    GpioData.ElectricalConfig = GpioTolerance1v8;
  } else {
    GpioData.ElectricalConfig = GpioNoTolerance1v8;
  }
  for (Index = 0; Index < PCH_SERIAL_IO_PINS_PER_I2C_CONTROLLER; Index++) {
    GpioGetPadOwnership (I2cGpio[SerialIoI2cControllerNumber][Index].Pad , &PadOwnership);
    if (PadOwnership == GpioPadOwnHost) {
      Status = GpioSetPadConfig (I2cGpio[SerialIoI2cControllerNumber][Index].Pad, &GpioData);
    } else {
      return EFI_UNSUPPORTED;
    }
  }
  return Status;
}


/**
  This function sets SerialIo UART controller pins into native mode

  @param[in]  SerialIoI2CControllerNumber   UART controller
  @param[in]  HardwareFlowControl           Hardware Flow control

  @retval Status
**/
EFI_STATUS
GpioSetSerialIoUartPinsIntoNativeMode (
  IN  UINT32            SerialIoUartControllerNumber,
  IN  BOOLEAN           HardwareFlowControl
  )
{
  EFI_STATUS    Status;
  UINTN         Index;
  UINTN         PinsUsed;
  GPIO_PAD_NATIVE_FUNCTION (*UartGpio) [PCH_SERIAL_IO_PINS_PER_UART_CONTROLLER];

  Status = EFI_SUCCESS;

  ASSERT (SerialIoUartControllerNumber < PCH_SERIALIO_MAX_UART_CONTROLLERS);

  if (GetPchSeries () == PchLp) {
    UartGpio = mPchLpUartGpio;
  } else {
    UartGpio = mPchHUartGpio;
  }

  if (HardwareFlowControl) {
    PinsUsed = PCH_SERIAL_IO_PINS_PER_UART_CONTROLLER;
  } else {
    PinsUsed = PCH_SERIAL_IO_PINS_PER_UART_CONTROLLER_NO_FLOW_CTRL;
  }

  for (Index = 0; Index < PinsUsed; Index++) {
    Status = SetGpioPadMode (UartGpio[SerialIoUartControllerNumber][Index].Pad, UartGpio[SerialIoUartControllerNumber][Index].Mode);
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
    GpioSetInputInversion (UartGpio[SerialIoUartControllerNumber][Index].Pad, 0);
  }
  return Status;
}

/**
  This function sets SerialIo SPI controller pins into native mode

  @param[in]  SerialIoI2CControllerNumber   SPI controller

  @retval Status
**/
EFI_STATUS
GpioSetSerialIoSpiPinsIntoNativeMode (
  IN  UINT32            SerialIoSpiControllerNumber
  )
{
  EFI_STATUS    Status;
  UINTN         Index;
  GPIO_PAD_NATIVE_FUNCTION (*SpiGpio) [PCH_SERIAL_IO_PINS_PER_SPI_CONTROLLER];

  Status = EFI_SUCCESS;

  ASSERT (SerialIoSpiControllerNumber < PCH_SERIALIO_MAX_SPI_CONTROLLERS);

  if (GetPchSeries () == PchLp) {
    SpiGpio = mPchLpSpiGpio;
  } else {
    SpiGpio = mPchHSpiGpio;
  }

  for (Index = 0; Index < PCH_SERIAL_IO_PINS_PER_SPI_CONTROLLER; Index++) {
    Status = SetGpioPadMode (SpiGpio[SerialIoSpiControllerNumber][Index].Pad, SpiGpio[SerialIoSpiControllerNumber][Index].Mode);
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
    GpioSetInputInversion (SpiGpio[SerialIoSpiControllerNumber][Index].Pad, 0);
  }
  return Status;
}


/**
  This function checks if GPIO pin is a GSPI chip select pin

  @param[in]  GpioPad             GPIO pad
  @param[in]  PadMode             GPIO pad mode

  @retval TRUE                    Pin is in GPIO mode
          FALSE                   Pin is in native mode
**/
BOOLEAN
GpioIsGpioPadAGSpiCsbPin (
  IN  GPIO_PAD        GpioPad,
  IN  GPIO_PAD_MODE   PadMode
  )
{
  UINT32                   ControllerMax;
  UINT32                   ControllerIndex;
  GPIO_PAD_OWN             PadOwnership;
  GPIO_PAD_NATIVE_FUNCTION (*SpiGpio) [PCH_SERIAL_IO_PINS_PER_SPI_CONTROLLER];

  if (GetPchSeries () == PchLp) {
    SpiGpio = mPchLpSpiGpio;
    ControllerMax = sizeof (mPchLpSpiGpio) / sizeof (GPIO_PAD_NATIVE_FUNCTION) / PCH_SERIAL_IO_PINS_PER_SPI_CONTROLLER;
  } else {
    SpiGpio = mPchHSpiGpio;
    ControllerMax = sizeof (mPchHSpiGpio) / sizeof (GPIO_PAD_NATIVE_FUNCTION) / PCH_SERIAL_IO_PINS_PER_SPI_CONTROLLER;
  }

  for (ControllerIndex = 0; ControllerIndex < ControllerMax; ControllerIndex++) {
    if ((GpioPad == SpiGpio[ControllerIndex][0].Pad) &&
        (PadMode == SpiGpio[ControllerIndex][0].Mode)) {
      GpioGetPadOwnership (SpiGpio[ControllerIndex][0].Pad , &PadOwnership);
      if (PadOwnership == GpioPadOwnHost) {
        return TRUE;
      } else {
        return FALSE;
      }
    }
  }
  return FALSE;
}

/**
  This function checks if GPIO pin for SATA reset port is in GPIO MODE

  @param[in]  SataPort            SATA port number

  @retval TRUE                    Pin is in GPIO mode
          FALSE                   Pin is in native mode
**/
BOOLEAN
GpioIsSataResetPortInGpioMode (
  IN  UINTN           SataPort
  )
{
  EFI_STATUS     Status;
  UINT32         GpioPin;
  GPIO_PAD_MODE  GpioMode;


  if (GetPchSeries () == PchLp) {
    ASSERT (SataPort < PCH_LP_AHCI_MAX_PORTS);
    GpioPin = mPchLpSataPortResetToGpioMap[SataPort].Pad;
  } else {
    ASSERT (SataPort < PCH_H_AHCI_MAX_PORTS);
    GpioPin = mPchHSataPortResetToGpioMap[SataPort].Pad;
  }

  Status =  GetGpioPadMode (GpioPin, &GpioMode);
  if ((EFI_ERROR (Status)) || (GpioMode != GpioPadModeGpio)) {
    return FALSE;
  } else {
    return TRUE;
  }
}


/**
  This function checks if GPIO pin is a SataDevSlp pin

  @param[in]  GpioPad             GPIO pad
  @param[in]  PadMode             GPIO pad mode

  @retval TRUE                    Pin is in GPIO mode
          FALSE                   Pin is in native mode
**/
BOOLEAN
GpioIsPadASataDevSlpPin (
  IN  GPIO_PAD        GpioPad,
  IN  GPIO_PAD_MODE   PadMode
  )
{
  UINT32                    SataDevSlpPinMax;
  UINT32                    SataDevSlpPinIndex;
  GPIO_PAD_OWN              PadOwnership;
  GPIO_PAD_NATIVE_FUNCTION  *SataDevSlpPinToGpioMap;

  if (GetPchSeries () == PchLp) {
    SataDevSlpPinToGpioMap = mPchLpSataDevSlpPinToGpioMap;
    SataDevSlpPinMax = sizeof (mPchLpSataDevSlpPinToGpioMap) /sizeof (GPIO_PAD_NATIVE_FUNCTION);
  } else {
    SataDevSlpPinToGpioMap = mPchHSataDevSlpPinToGpioMap;
    SataDevSlpPinMax = sizeof (mPchHSataDevSlpPinToGpioMap) /sizeof (GPIO_PAD_NATIVE_FUNCTION);
  }

  for (SataDevSlpPinIndex = 0; SataDevSlpPinIndex < SataDevSlpPinMax; SataDevSlpPinIndex++) {
    if ((GpioPad == SataDevSlpPinToGpioMap[SataDevSlpPinIndex].Pad) &&
        (PadMode == SataDevSlpPinToGpioMap[SataDevSlpPinIndex].Mode)) {
      GpioGetPadOwnership (SataDevSlpPinToGpioMap[SataDevSlpPinIndex].Pad , &PadOwnership);
      if (PadOwnership == GpioPadOwnHost) {
        return TRUE;
      } else {
        return FALSE;
      }
    }
  }
  return FALSE;
}

/**
  This function checks if SataDevSlp pin is in native mode

  @param[in]  SataPort            SATA port
  @param[out] DevSlpPad           DevSlpPad

  @retval TRUE                    DevSlp is in native mode
          FALSE                   DevSlp is not in native mode
**/
BOOLEAN
GpioIsSataDevSlpPinEnabled (
  IN  UINTN           SataPort,
  OUT GPIO_PAD        *DevSlpPad
  )
{
  GPIO_PAD_MODE  DevSlpPadMode;
  GPIO_PAD       DevSlpGpioPad;
  GPIO_PAD_MODE  GpioMode;
  EFI_STATUS     Status;

  if (GetPchSeries () == PchLp) {
    ASSERT (SataPort < PCH_LP_AHCI_MAX_PORTS);
    DevSlpGpioPad = mPchLpSataDevSlpPinToGpioMap[SataPort].Pad;
    DevSlpPadMode = mPchLpSataDevSlpPinToGpioMap[SataPort].Mode;
  } else {
    ASSERT (SataPort < PCH_H_AHCI_MAX_PORTS);
    DevSlpGpioPad = mPchHSataDevSlpPinToGpioMap[SataPort].Pad;
    DevSlpPadMode = mPchHSataDevSlpPinToGpioMap[SataPort].Mode;
  }

  Status = GetGpioPadMode (DevSlpGpioPad, &GpioMode);

  if (EFI_ERROR (Status) || (GpioMode != DevSlpPadMode)) {
    *DevSlpPad = 0x0;
    return FALSE;
  } else {
    *DevSlpPad = DevSlpGpioPad;
    return TRUE;
  }
}

