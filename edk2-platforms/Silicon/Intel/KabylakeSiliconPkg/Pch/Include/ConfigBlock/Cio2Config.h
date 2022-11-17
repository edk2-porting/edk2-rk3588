/** @file
  CIO2 policy

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _CIO2_CONFIG_H_
#define _CIO2_CONFIG_H_

#define CIO2_CONFIG_REVISION 1
extern EFI_GUID gCio2ConfigGuid;

#pragma pack (push,1)

///
/// The PCH_SKYCAM_CIO2_CONFIG block describes SkyCam CIO2 device.
///
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  /**
    NOTE: For SKL PCH, while CIO2 is enabled,
    RC will configure CIO2 controller as ACPI mode when PCH stepping < C0, and configure to PCI mode for C0 onwards.
  **/
  UINT32 DeviceEnable              :  2;          ///< 0: Disabled, <b>1: Enabled</b>

  UINT32 SkyCamPortATermOvrEnable  :  1;          ///< <b>0: Disable</b>, 1: Enable - Termination override on port A
  UINT32 SkyCamPortBTermOvrEnable  :  1;          ///< <b>0: Disable</b>, 1: Enable - Termination override on port B
  UINT32 SkyCamPortCTermOvrEnable  :  1;          ///< <b>0: Disable</b>, 1: Enable - Termination override on port C
  UINT32 SkyCamPortDTermOvrEnable  :  1;          ///< <b>0: Disable</b>, 1: Enable - Termination override on port D
  UINT32 RsvdBits                  : 26;

  //
  // CIO2 FLS registers configuration.
  //

  UINT32 PortATrimEnable     :  1;          ///< 0: Disable, <b>1: Enable</b> - Port A Clk Trim
  UINT32 PortBTrimEnable     :  1;          ///< 0: Disable, <b>1: Enable</b> - Port B Clk Trim
  UINT32 PortCTrimEnable     :  1;          ///< 0: Disable, <b>1: Enable</b> - Port C Clk Trim
  UINT32 PortDTrimEnable     :  1;          ///< 0: Disable, <b>1: Enable</b> - Port D Clk Trim
  UINT32 PortACtleEnable     :  1;          ///< 0: Disable, <b>1: Enable</b> - Port A Ctle
  UINT32 PortBCtleEnable     :  1;          ///< 0: Disable, <b>1: Enable</b> - Port B Ctle
  UINT32 PortCDCtleEnable    :  1;          ///< 0: Disable, <b>1: Enable</b> - Port C/D Ctle
  UINT32 RsvdBits0           : 25;

  UINT32 PortACtleCapValue   :  4;          /// Port A Ctle Cap Value. Default is <b>0xE</b>
  UINT32 PortBCtleCapValue   :  4;          /// Port B Ctle Cap Value. Default is <b>0xE</b>
  UINT32 PortCDCtleCapValue  :  4;          /// Port C/D Ctle Cap Value. Default is <b>0xE</b>
  UINT32 PortACtleResValue   :  5;          /// Port A Ctle Res Value. Default is <b>0xD</b>
  UINT32 PortBCtleResValue   :  5;          /// Port B Ctle Res Value. Default is <b>0xD</b>
  UINT32 PortCDCtleResValue  :  5;          /// Port C/D Ctle Res Value. Default is <b>0xD</b>
  UINT32 RsvdBits1           :  5;

  UINT32 PortAClkTrimValue   :  4;          /// Port A Clk Trim Value. Default is <b>0xA</b>
  UINT32 PortBClkTrimValue   :  4;          /// Port B Clk Trim Value. Default is <b>0xA</b>
  UINT32 PortCClkTrimValue   :  4;          /// Port C Clk Trim Value. Default is <b>0x9</b>
  UINT32 PortDClkTrimValue   :  4;          /// Port D Clk Trim Value. Default is <b>0xA</b>
  UINT32 PortADataTrimValue  : 16;          /// Port A Data Trim Value. Default is <b>0xBBBB</b>
  UINT32 PortBDataTrimValue  : 16;          /// Port B Data Trim Value. Default is <b>0xBBBB</b>
  UINT32 PortCDDataTrimValue : 16;          /// Port C/D Data Trim Value. Default is <b>0xCCCC</b>

} PCH_CIO2_CONFIG;

#pragma pack (pop)

#endif // _CIO2_CONFIG_H_
