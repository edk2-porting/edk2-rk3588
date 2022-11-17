/** @file
  ISH policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _ISH_CONFIG_H_
#define _ISH_CONFIG_H_

#define ISH_CONFIG_REVISION 1
extern EFI_GUID gIshConfigGuid;

#pragma pack (push,1)

///
/// The PCH_ISH_CONFIG block describes Integrated Sensor Hub device.
///
typedef struct {
  CONFIG_BLOCK_HEADER   Header;     ///< Config Block Header
  UINT32    Enable          :  1;   ///< ISH Controler 0: Disable; <b>1: Enable</b>.
  UINT32    SpiGpioAssign   :  1;   ///< ISH SPI GPIO pins assigned: <b>0: False</b> 1: True
  UINT32    Uart0GpioAssign :  1;   ///< ISH UART0 GPIO pins assigned: <b>0: False</b> 1: True
  UINT32    Uart1GpioAssign :  1;   ///< ISH UART1 GPIO pins assigned: <b>0: False</b> 1: True
  UINT32    I2c0GpioAssign  :  1;   ///< ISH I2C0 GPIO pins assigned: <b>0: False</b> 1: True
  UINT32    I2c1GpioAssign  :  1;   ///< ISH I2C1 GPIO pins assigned: <b>0: False</b> 1: True
  UINT32    I2c2GpioAssign  :  1;   ///< ISH I2C2 GPIO pins assigned: <b>0: False</b> 1: True
  UINT32    Gp0GpioAssign   :  1;   ///< ISH GP_0 GPIO pin assigned: <b>0: False</b> 1: True
  UINT32    Gp1GpioAssign   :  1;   ///< ISH GP_1 GPIO pin assigned: <b>0: False</b> 1: True
  UINT32    Gp2GpioAssign   :  1;   ///< ISH GP_2 GPIO pin assigned: <b>0: False</b> 1: True
  UINT32    Gp3GpioAssign   :  1;   ///< ISH GP_3 GPIO pin assigned: <b>0: False</b> 1: True
  UINT32    Gp4GpioAssign   :  1;   ///< ISH GP_4 GPIO pin assigned: <b>0: False</b> 1: True
  UINT32    Gp5GpioAssign   :  1;   ///< ISH GP_5 GPIO pin assigned: <b>0: False</b> 1: True
  UINT32    Gp6GpioAssign   :  1;   ///< ISH GP_6 GPIO pin assigned: <b>0: False</b> 1: True
  UINT32    Gp7GpioAssign   :  1;   ///< ISH GP_7 GPIO pin assigned: <b>0: False</b> 1: True
  UINT32    PdtUnlock       :  1;   ///< ISH PDT Unlock Msg: <b>0: False</b> 1: True
  UINT32    RsvdBits0       : 16;   ///< Reserved Bits
} PCH_ISH_CONFIG;

#pragma pack (pop)

#endif // _ISH_CONFIG_H_
