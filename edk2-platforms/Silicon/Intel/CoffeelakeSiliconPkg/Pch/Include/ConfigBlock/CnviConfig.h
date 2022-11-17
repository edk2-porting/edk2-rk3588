/** @file
  CNVI policy

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CNVI_CONFIG_H_
#define _CNVI_CONFIG_H_

#define CNVI_CONFIG_REVISION 2
extern EFI_GUID gCnviConfigGuid;

#pragma pack (push,1)

/**
  CNVi Mode options
**/
typedef enum {
  CnviModeDisabled = 0,
  CnviModeAuto
} CNVI_MODE;

/**
  CNVi MfUart1 connection options
**/
typedef enum {
  CnviMfUart1Ish = 0,
  CnviMfUart1SerialIo,
  CnviBtUart1ExtPads,
  CnviBtUart1NotConnected
} CNVI_MFUART1_TYPE;


/**
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Remove BtInterface and BtUartType.

**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;               ///< Config Block Header
  /**
    This option allows for automatic detection of Connectivity Solution.
    Auto Detection assumes that CNVi will be enabled when available;
    Disable allows for disabling CNVi.
    CnviModeDisabled = Disabled,
    <b>CnviModeAuto = Auto Detection</b>
  **/
  UINT32 Mode                  :  1;
  /**
    <b>(Test)</b> This option configures Uart type which connects to MfUart1
    For production configuration ISH is the default, for tests SerialIO Uart0 or external pads can be used
    Use CNVI_MFUART1_TYPE enum for selection
    <b>CnviMfUart1Ish = MfUart1 over ISH Uart0</b>,
    CnviMfUart1SerialIo = MfUart1 over SerialIO Uart2,
    CnviBtUart1ExtPads = MfUart1 over exteranl pads,
    CnviBtUart1NotConnected = MfUart1 not connected
  **/
  UINT32 MfUart1Type           :  2;
  UINT32 RsvdBits              : 29;
} PCH_CNVI_CONFIG;

#pragma pack (pop)

#endif // _CNVI_CONFIG_H_

