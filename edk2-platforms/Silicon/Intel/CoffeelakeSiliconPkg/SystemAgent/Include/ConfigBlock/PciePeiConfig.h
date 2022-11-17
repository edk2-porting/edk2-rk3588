/** @file
  Policy definition for PCIe Config Block

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCIE_PEI_CONFIG_H_
#define _PCIE_PEI_CONFIG_H_

#include <Library/GpioLib.h>
#include <SaAccess.h>

#pragma pack(push, 1)

#define SA_PCIE_PEI_CONFIG_REVISION 1

/**
 PCI Express and DMI controller configuration - PostMem\n
 @note <b>Optional.</b> These policies will be ignored if there is no PEG port present on board.
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER    Header;                  ///< Offset 0-27 Config Block Header
  /**
    Offset 28:0
   <b>(Test)</b>DMI Extended Sync Control
  - <b>Disabled</b> (0x0)  : Disable DMI Extended Sync (Default)
  - Enabled         (0x1)  : Enable DMI Extended Sync
  **/
  UINT32                 DmiExtSync                      :  1;
  /**
    Offset 28:1
   <b>(Test)</b>DMI IOT Control
  - <b>Disabled</b> (0x0)  : Disable DMI IOT (Default)
  - Enabled         (0x1)  : Enable DMI IOT
  **/
  UINT32                 DmiIot                          :  1;
  UINT32                 RsvdBits1                       :  30;       ///< Offset 28:2-31 :Reserved for future use.
  UINT8                  DmiAspm;                                     ///< Offset 32 This field is used to describe the ASPM control for DMI: <b>3=PcieAspmL0sL1</b>, 2=PcieAspmL1, 1=PcieAspmL0s, 0=PcieAspmDisabled.
  UINT8                  Rsvd1[3];                                    ///< Offset 33 to 35
  UINT8                  PegDeEmphasis[SA_PEG_MAX_FUN];               ///< Offset 36 This field is used to describe the DeEmphasis control for PEG (-6 dB and -3.5 dB are the options)SA_PEG_MAX_FUN = 3 for CFL and SA_PEG_MAX_FUN = 4 for CNL, offsets are adjusted accordingly
  UINT8                  PegMaxPayload[SA_PEG_MAX_FUN];               ///< <b>(Test)</b> Offset 39/40 This field is used to describe the PEG Max Pay Load Size (0xFF: Auto, 0:128B, 1:256B)
  /**
   PCIe Slot Power Capabilities. SlotPowerLimitValue in combination with SlotPowerLimitScale specifies the upper limit on power supplied by slot.
  **/
  UINT8                  PegSlotPowerLimitValue[SA_PEG_MAX_FUN];      ///< Offset 42/44 8 bit value
  UINT8                  PegSlotPowerLimitScale[SA_PEG_MAX_FUN];      ///< Offset 45/48 2 bit value: <b>00 = 1.0x</b>, 01 = 0.1x, 10 = 0.01x and 11 = 0.001x
  /**
   Offset 48/52
   PCIe Physical Slot Number (13 bit value). Indicates the physical slot number attached to the port.
  **/
  UINT16                 PegPhysicalSlotNumber[SA_PEG_MAX_FUN];
  UINT8                  Rsvd2[2];
} PCIE_PEI_CONFIG;
#pragma pack(pop)

#endif // _PCIE_PEI_CONFIG_H_
