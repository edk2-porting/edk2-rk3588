/** @file
  Header file for GpioPrivateLib.
  All function in this library is available for PEI, DXE, and SMM,

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _GPIO_PRIVATE_LIB_H_
#define _GPIO_PRIVATE_LIB_H_

#include <GpioConfig.h>

/**
  This procedure will get value of selected gpio register

  @param[in]  Group               GPIO group number
  @param[in]  Offset              GPIO register offset
  @param[out] RegVal              Value of gpio register

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetReg (
  IN  GPIO_GROUP              Group,
  IN  UINT32                  Offset,
  OUT UINT32                  *RegVal
  );

/**
  This procedure will set value of selected gpio register

  @param[in] Group               GPIO group number
  @param[in] Offset              GPIO register offset
  @param[in] RegVal              Value of gpio register

  @retval EFI_SUCCESS            The function completed successfully
  @retval EFI_INVALID_PARAMETER  Invalid group or pad number
**/
EFI_STATUS
GpioSetReg (
  IN GPIO_GROUP              Group,
  IN UINT32                  Offset,
  IN UINT32                  RegVal
  );

/**
  This procedure is used by PchSmiDispatcher and will return information
  needed to register GPI SMI. Relation between Index and GpioPad number is:
  Index = GpioGroup + 24 * GpioPad

  @param[in]  Index               GPI SMI number
  @param[out] GpioPin             GPIO pin
  @param[out] GpiSmiBitOffset     GPI SMI bit position within GpiSmi Registers
  @param[out] GpiSmiEnRegAddress  Address of GPI SMI Enable register
  @param[out] GpiSmiStsRegAddress Address of GPI SMI status register

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioGetPadAndSmiRegs (
  IN UINT32            Index,
  OUT GPIO_PAD         *GpioPin,
  OUT UINT8            *GpiSmiBitOffset,
  OUT UINT32           *GpiSmiEnRegAddress,
  OUT UINT32           *GpiSmiStsRegAddress
  );

/**
  This procedure will clear GPIO_UNLOCK_SMI_STS

  @param[in]  None

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
GpioClearUnlockSmiSts (
  VOID
  );

/**
  This procedure will set GPIO Driver IRQ number

  @param[in]  Irq                 Irq number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid IRQ number
**/
EFI_STATUS
GpioSetIrq (
  IN  UINT8          Irq
  );

/**
  This procedure will perform special handling of GPP_A_12 on PCH-LP.

  @param[in]  None

  @retval None
**/
VOID
GpioA12SpecialHandling (
  VOID
  );

/**
  This function sets ISH I2C controller pins into native mode

  @param[in]  IshI2cControllerNumber   I2C controller

  @retval Status
**/
EFI_STATUS
GpioSetIshI2cPinsIntoNativeMode (
  IN  UINT32            IshI2cControllerNumber
  );

/**
  This function sets ISH UART controller pins into native mode

  @param[in]  IshUartControllerNumber   UART controller

  @retval Status
**/
EFI_STATUS
GpioSetIshUartPinsIntoNativeMode (
  IN  UINT32            IshUartControllerNumber
  );

/**
  This function sets ISH SPI controller pins into native mode

  @param[in]  none

  @retval Status
**/
EFI_STATUS
GpioSetIshSpiPinsIntoNativeMode (
  VOID
  );

/**
  This function sets ISH GP pins into native mode

  @param[in]  IshGpPinNumber   ISH GP pin number

  @retval Status
**/
EFI_STATUS
GpioSetIshGpPinsIntoNativeMode (
  IN  UINT32            IshGpPinNumber
  );

/**
  Returns pad for given CLKREQ# index.

  @param[in]  ClkreqIndex       CLKREQ# number

  @return CLKREQ# pad.
**/
GPIO_PAD
GpioGetClkreqPad (
  IN     UINT32   ClkreqIndex
  );

/**
  Enables CLKREQ# pad in native mode.

  @param[in]  ClkreqIndex       CLKREQ# number

  @return none
**/
VOID
GpioEnableClkreq (
  IN     UINT32   ClkreqIndex
  );

/**
  This function checks if GPIO pin for PCHHOTB is in NATIVE MODE

  @param[in]  none

  @retval TRUE                    Pin is in PCHHOTB native mode
          FALSE                   Pin is in gpio mode or is not owned by HOST
**/
BOOLEAN
GpioIsPchHotbPinInNativeMode (
  VOID
  );

/**
  This function sets CPU GP pins into native mode

  @param[in]  CpuGpPinNum               CPU GP pin number

  @retval Status
**/
EFI_STATUS
GpioSetCpuGpPinsIntoNativeMode (
  IN  UINT32                            CpuGpPinNum
  );

#endif // _GPIO_PRIVATE_LIB_H_
