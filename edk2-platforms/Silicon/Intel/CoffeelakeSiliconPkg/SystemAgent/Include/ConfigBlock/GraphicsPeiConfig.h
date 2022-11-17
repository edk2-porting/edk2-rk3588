/** @file
  Policy definition for Internal Graphics Config Block (PostMem)

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _GRAPHICS_PEI_CONFIG_H_
#define _GRAPHICS_PEI_CONFIG_H_
#pragma pack(push, 1)

#define GRAPHICS_PEI_CONFIG_REVISION 4
#define DDI_DEVICE_NUMBER 4

//
// DDI defines
//
typedef enum {
  DdiDisable       = 0x00,
  DdiDdcEnable     = 0x01,
  DdiTbtLsxEnable  = 0x02,
} DDI_DDC_TBT_VAL;

typedef enum {
  DdiHpdDisable  = 0x00,
  DdiHpdEnable   = 0x01,
} DDI_HPD_VAL;

typedef enum {
  DdiPortADisabled = 0x00,
  DdiPortAEdp      = 0x01,
  DdiPortAMipiDsi  = 0x02,
} DDI_PORTA_SETTINGS;
/**
  This structure configures the Native GPIOs for DDI port per VBT settings.
**/
typedef struct {
  UINT8 DdiPortEdp;    /// The setting of eDP port, this settings must match VBT's settings. 0- Disable, <b>1- Enable</b>
  UINT8 DdiPortBHpd;   /// The HPD setting of DDI Port B, this settings must match VBT's settings. 0- Disable, <b>1- Enable</b>
  UINT8 DdiPortCHpd;   /// The HPD setting of DDI Port C, this settings must match VBT's settings. 0- Disable, <b>1- Enable</b>
  UINT8 DdiPortDHpd;   /// The HPD setting of DDI Port D, this settings must match VBT's settings. 0- Disable, <b>1- Enable</b>
  UINT8 DdiPortFHpd;   /// The HPD setting of DDI Port F, this settings must match VBT's settings. 0- Disable, <b>1- Enable</b>
  UINT8 DdiPortBDdc;   /// The DDC setting of DDI Port B, this settings must match VBT's settings. 0- Disable, <b>1- Enable</b>
  UINT8 DdiPortCDdc;   /// The DDC setting of DDI Port C, this settings must match VBT's settings. 0- Disable, <b>1- Enable</b>
  UINT8 DdiPortDDdc;   /// The DDC setting of DDI Port D, this settings must match VBT's settings. 0- Disable, <b>1- Enable</b>
  UINT8 DdiPortFDdc;   /// The DDC setting of DDI Port F, this settings must match VBT's settings. <b>0- Disable</b>, 1- Enable
  UINT8 Rsvd[3];       ///< Reserved for 4 bytes alignment
} DDI_CONFIGURATION;

/**
  This configuration block is to configure IGD related variables used in PostMem PEI.
  If Intel Gfx Device is not supported, all policies can be ignored.
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added SkipS3CdClockInit.
  <b>Revision 3</b>:
  - Added DeltaT12PowerCycleDelay, BltBufferAddress, BltBufferSize.
  <b>Revision 4</b>:
  - Deprecated DeltaT12PowerCycleDelay.

**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Offset 0-27 Config Block Header
  UINT32                RenderStandby     : 1;    ///< Offset 28:0 :<b>(Test)</b> This field is used to enable or disable RC6 (Render Standby): 0=FALSE, <b>1=TRUE</b>
  UINT32                PmSupport         : 1;    ///< Offset 28:1 :<b>(Test)</b> IGD PM Support TRUE/FALSE: 0=FALSE, <b>1=TRUE</b>
  UINT32                PavpEnable        : 1;    ///< Offset 28:2 :IGD PAVP TRUE/FALSE: 0=FALSE, <b>1=TRUE</b>
  /**
    Offset 28:3
    CdClock Frequency select\n
    CFL\n
    0   = 337.5 Mhz, 1 = 450 Mhz,\n
    2   = 540 Mhz,<b> 3 = 675 Mhz</b>,\n

  **/
  UINT32                CdClock            : 3;
  UINT32                PeiGraphicsPeimInit: 1;   ///< Offset 28:6 : This policy is used to enable/disable Intel Gfx PEIM.<b>0- Disable</b>, 1- Enable
  UINT32                CdynmaxClampEnable : 1;   ///< Offset 28:7 : This policy is used to enable/disable CDynmax Clamping Feature (CCF) <b>1- Enable</b>, 0- Disable
  UINT32                GtFreqMax          : 8;   ///< Offset 28:8 : <b>(Test)</b> Max GT frequency limited by user in multiples of 50MHz: Default value which indicates normal frequency is <b>0xFF</b>
  UINT32                DisableTurboGt     : 1;   ///< Offset 28:9 : This policy is used to enable/disable DisableTurboGt <b>0- Disable</b>, 1- Enable
  UINT32                RsvdBits0          : 15;  ///< Offser 28:15 :Reserved for future use
  VOID*                 LogoPtr;                  ///< Offset 32 Address of Intel Gfx PEIM Logo to be displayed
  UINT32                LogoSize;                 ///< Offset 36 Intel Gfx PEIM Logo Size
  VOID*                 GraphicsConfigPtr;        ///< Offset 40 Address of the Graphics Configuration Table
  DDI_CONFIGURATION     DdiConfiguration;         ///< Offset 44 DDI configuration, need to match with VBT settings.
  UINT32                SkipS3CdClockInit  : 1;   ///< Offset 56 SKip full CD clock initialization being done during S3 resume.<b>0- Disable<\b>, 1- Enable
  UINT32                ReservedBits       : 31;  ///< Offset 56: 1 : Reserved for future use.
  UINT16                DeltaT12PowerCycleDelay;  ///< Offset 60 @deprecated Power Cycle Delay required for eDP as per VESA standard.<b>0 - 0 ms<\b>, 0xFFFF - Auto calculate to max 500 ms
  UINT8                 Reserved[2];              ///< Offset 62 Reserved for future use.
  VOID*                 BltBufferAddress;         ///< Offset 64 Address of Blt buffer for PEIM Logo use
  UINT32                BltBufferSize;            ///< Offset 68 The size for Blt Buffer, calculating by PixelWidth * PixelHeight * 4 bytes (the size of EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
} GRAPHICS_PEI_CONFIG;
#pragma pack(pop)

#endif // _GRAPHICS_PEI_CONFIG_H_
