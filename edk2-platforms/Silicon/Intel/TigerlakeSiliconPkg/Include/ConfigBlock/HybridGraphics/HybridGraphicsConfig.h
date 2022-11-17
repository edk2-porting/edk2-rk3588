/** @file
  Hybrid Graphics policy definitions

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _HYBRID_GRAPHICS_CONFIG_H_
#define _HYBRID_GRAPHICS_CONFIG_H_

#define HYBRID_GRAPHICS_CONFIG_REVISION 2

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
/// CPU PCIe GPIO Data Structure
///
typedef struct {
  UINT8   ExpanderNo; ///< Offset 0 Expander No For I2C based GPIO
  BOOLEAN Active;     ///< Offset 1 0=Active Low; 1=Active High
  UINT8   Rsvd0[2];   ///< Offset 2 Reserved
  UINT32  GpioNo;     ///< Offset 4 GPIO pad
} CPU_PCIE_GPIO_INFO;

/**
 CPU PCIE RTD3 GPIO Data Structure
**/
typedef struct {
  CPU_PCIE_GPIO_INFO  HoldRst;      ///< Offset 0 This field contain PCIe HLD RESET GPIO value and level information
  CPU_PCIE_GPIO_INFO  PwrEnable;    ///< Offset 8 This field contain PCIe PWR Enable GPIO value and level information
  UINT32              WakeGpioNo;   ///< Offset 16 This field contain PCIe RTD3 Device Wake GPIO Number
  UINT8               GpioSupport;  ///< Offset 20 Depends on board design the GPIO configuration may be different: <b>0=Not Supported</b>, 1=PCH Based, 2=I2C based
  UINT8               Rsvd0[3];     ///< Offset 21
} CPU_PCIE_RTD3_GPIO;

/**
  This Configuration block configures CPU PCI Express 0/1/2 RTD3 GPIOs & Root Port.
  Hybrid Gfx uses the same GPIOs & Root port as PCI Express 0/1/2 RTD3.
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Add HgSlot Policy: PEG or PCH Slot Slection for Hybrid Graphics
**/
typedef struct {
  CONFIG_BLOCK_HEADER Header;                ///< Offset 0-27 Config Block Header
  CPU_PCIE_RTD3_GPIO  CpuPcie0Rtd3Gpio;      ///< Offset 28 RTD3 GPIOs used for PCIe
  UINT8               RootPortIndex;         ///< Offset 52 Root Port Index number used for HG
  UINT8               HgMode;                ///< Offset 53 HgMode: <b>0=Disabled</b>, 1=HG Muxed, 2=HG Muxless, 3=PEG
  UINT16              HgSubSystemId;         ///< Offset 54 Hybrid Graphics Subsystem ID: <b>2212</b>
  UINT16              HgDelayAfterPwrEn;     ///< Offset 56 Dgpu Delay after Power enable using Setup option: 0=Minimal, 1000=Maximum, <b>300=300 microseconds</b>
  UINT16              HgDelayAfterHoldReset; ///< Offset 58 Dgpu Delay after Hold Reset using Setup option: 0=Minimal, 1000=Maximum, <b>100=100 microseconds</b>
  CPU_PCIE_RTD3_GPIO  CpuPcie1Rtd3Gpio;      ///< Offset 60 RTD3 GPIOs used for PCIe
  CPU_PCIE_RTD3_GPIO  CpuPcie2Rtd3Gpio;      ///< Offset 84 RTD3 GPIOs used for PCIe
  CPU_PCIE_RTD3_GPIO  CpuPcie3Rtd3Gpio;      ///< Offset 108 RTD3 GPIOs used for PCIe
  UINT8               HgSlot;                ///< Offset 132 Slot selection between PEG and PCH
  UINT8               Rsvd0[3];              ///< Offset 133 Reserved Bytes
} HYBRID_GRAPHICS_CONFIG;
#pragma pack(pop)
#endif // _HYBRID_GRAPHICS_CONFIG_H_
