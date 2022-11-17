/** @file
  Policy definition for Internal Graphics Config Block (PostMem)

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _GRAPHICS_PEI_CONFIG_H_
#define _GRAPHICS_PEI_CONFIG_H_
#pragma pack(push, 1)

#define GRAPHICS_PEI_CONFIG_REVISION 1

/**
  This configuration block is to configure IGD related variables used in PostMem PEI.
  If Intel Gfx Device is not supported, all policies can be ignored.
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Offset 0-27 Config Block Header
  UINT32                RenderStandby     : 1;    ///< Offset 28:0 :<b>(Test)</b> This field is used to enable or disable RC6 (Render Standby): 0=FALSE, <b>1=TRUE</b>
  UINT32                PmSupport         : 1;    ///< Offset 28:1 :<b>(Test)</b> IGD PM Support TRUE/FALSE: 0=FALSE, <b>1=TRUE</b>
  UINT32                PavpEnable        : 1;    ///< Offset 28:2 :IGD PAVP TRUE/FALSE: 0=FALSE, <b>1=TRUE</b>
  /**
    Offset 28:3
    CdClock Frequency select\n
    0   = 337.5 Mhz, 1 = 450 Mhz,\n
    2   = 540 Mhz,<b> 3 = 675 Mhz</b>,\n
  **/
  UINT32                CdClock            : 3;
  UINT32                PeiGraphicsPeimInit: 1;   ///< Offset 28:6 :This policy is used to enable/disable Intel Gfx PEIM.<b>0- Disable</b>, 1- Enable
  UINT32                CdynmaxClampEnable : 1;   ///< Offset 28:7 : This policy is used to enable/disable CDynmax Clamping Feature (CCF) <b>1- Enable</b>, 0- Disable
  UINT32                GtFreqMax          : 8;   ///< Offset 28:8 : <b>(Test)</b> Max GT frequency limited by user in multiples of 50MHz: Default value which indicates normal frequency is <b>0xFF</b>
  UINT32                RsvdBits0          : 16;  ///< Offser 28:16 :Reserved for future use
  VOID*                 LogoPtr;                  ///< Offset 32 Address of Logo to be displayed in PEI
  UINT32                LogoSize;                 ///< Offset 36 Logo Size
  VOID*                 GraphicsConfigPtr;        ///< Offset 40 Address of the Graphics Configuration Table
} GRAPHICS_PEI_CONFIG;
#pragma pack(pop)

#endif // _GRAPHICS_PEI_CONFIG_H_
