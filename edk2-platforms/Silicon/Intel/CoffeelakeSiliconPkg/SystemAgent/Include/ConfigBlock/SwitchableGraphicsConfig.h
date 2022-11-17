/** @file
  Switchable Graphics policy definitions

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SWITCHABLE_GRAPHICS_CONFIG_H_
#define _SWITCHABLE_GRAPHICS_CONFIG_H_

#define SWITCHABLE_GRAPHICS_CONFIG_REVISION 1

#define GP_ENABLE   1
#define GP_DISABLE  0

#pragma pack(push, 1)
///
/// GPIO Support
///
typedef enum {
  NotSupported = 0,
  PchGpio,
  I2CGpio,
} GPIO_SUPPORT;

///
/// SA GPIO Data Structure
///
typedef struct {
  UINT8   ExpanderNo; ///< Offset 0 Expander No For I2C based GPIO
  BOOLEAN Active;     ///< Offset 1 0=Active Low; 1=Active High
  UINT8 Rsvd0[2];     ///< Offset 2 Reserved
  UINT32  GpioNo;     ///< Offset 4 GPIO pad
} SA_GPIO_INFO;

/**
 SA PCIE RTD3 GPIO Data Structure
**/
typedef struct {
  SA_GPIO_INFO  HoldRst;      ///< Offset 0 This field contain PCIe HLD RESET GPIO value and level information
  SA_GPIO_INFO  PwrEnable;    ///< Offset 8 This field contain PCIe PWR Enable GPIO value and level information
  UINT32        WakeGpioNo;   ///< Offset 16 This field contain PCIe RTD3 Device Wake GPIO Number
  UINT8         GpioSupport;  ///< Offset 20 Depends on board design the GPIO configuration may be different: <b>0=Not Supported</b>, 1=PCH Based, 2=I2C based
  UINT8         Rsvd0[3];     ///< Offset 21
} SA_PCIE_RTD3_GPIO;

/**
  This Configuration block configures SA PCI Express 0/1/2 RTD3 GPIOs & Root Port.
  Swithable Gfx/Hybrid Gfx uses the same GPIOs & Root port as PCI Express 0/1/2 RTD3.
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER Header;             ///< Offset 0-27 Config Block Header
  SA_PCIE_RTD3_GPIO   SaRtd3Pcie0Gpio;    ///< Offset 28 RTD3 GPIOs used for PCIe0
  SA_PCIE_RTD3_GPIO   SaRtd3Pcie1Gpio;    ///< Offset 52 RTD3 GPIOs used for PCIe1
  SA_PCIE_RTD3_GPIO   SaRtd3Pcie2Gpio;    ///< Offset 76 RTD3 GPIOs used for PCIe2
  UINT8               RootPortIndex;      ///< Offset 124 Root Port Index number used for SG
  UINT8               Rsvd0[3];           ///< Offset 125 Reserved for DWORD Alignment
} SWITCHABLE_GRAPHICS_CONFIG;
#pragma pack(pop)
#endif // _SWITCHABLE_GRAPHICS_CONFIG_H_
