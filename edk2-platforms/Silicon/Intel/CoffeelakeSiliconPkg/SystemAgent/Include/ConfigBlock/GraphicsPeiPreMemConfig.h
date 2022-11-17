/** @file
  Policy definition for Internal Graphics Config Block (PreMem)

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _GRAPHICS_PEI_PREMEM_CONFIG_H_
#define _GRAPHICS_PEI_PREMEM_CONFIG_H_
#pragma pack(push, 1)

#define GRAPHICS_PEI_PREMEM_CONFIG_REVISION 2


/**
  This Configuration block is to configure GT related PreMem data/variables.\n
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added DeltaT12PowerCycleDelayPreMem.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Offset 0-27 Config Block Header
  /**
    Offset 28:0
    Selection of the primary display device: 0=iGFX, 1=PEG, 2=PCIe Graphics on PCH, <b>3=AUTO</b>, 4=Switchable Graphics\n
    When AUTO mode selected, the priority of display devices is: PCIe Graphics on PCH > PEG > iGFX
  **/
  UINT32                PrimaryDisplay    : 3;
  /**
    Offset 28:3
    Intel Gfx Support. It controls enabling/disabling iGfx device.
    When AUTO mode selected, iGFX will be turned off when external graphics detected.
    If FALSE, all other polices can be ignored.
    <b>2 = AUTO</b>;
    0 = FALSE;
    1 = TRUE.
  **/
  UINT32                InternalGraphics  : 2;
  /**
    Offset 28:5
    Pre-allocated memory for iGFX\n
    0   = 0MB,1 or 247 = 32MB,\n
    2   = 64MB,\n
    240 = 4MB,     241 = 8MB,\n
    242 = 12MB,    243 = 16MB,\n
    244 = 20MB,    245 = 24MB,\n
    246 = 28MB,    248 = 36MB,\n
    249 = 40MB,    250 = 44MB,\n
    251 = 48MB,    252 = 52MB,\n
    253 = 56MB,<b> 254 = 60MB</b>,\n
    <b>Note: enlarging pre-allocated memory for iGFX may need to reduce MmioSize because of 4GB boundary limitation</b>
  **/
  UINT32                IgdDvmt50PreAlloc : 8;
  UINT32                PanelPowerEnable  : 1;          ///< Offset 28:13 :<b>(Test)</b> Control for enabling/disabling VDD force bit (Required only for early enabling of eDP panel): 0=FALSE, <b>1=TRUE</b>
  UINT32                ApertureSize      : 7;          ///< Offser 28:14 :Graphics aperture size (256MB is the recommended size as per BWG) : 0=128MB, <b>1=256MB</b>, 3=512MB, 7=1024MB, 15=2048MB.
  UINT32                GtPsmiSupport     : 1;          ///< Offser 28:21 :PSMI support On/Off: <b>0=FALSE</b>, 1=TRUE
  UINT32                PsmiRegionSize    : 3;          ///< Offser 28:22 :Psmi region size: <b>0=32MB</b>, 1=288MB, 2=544MB, 3=800MB, 4=1056MB
  UINT32                RsvdBits0         : 7;          ///< Offser 28:25 :Reserved for future use
  UINT32                GttMmAdr;                       ///< Offset 32 Temp Address of System Agent GTTMMADR : Default is <b>0xCF000000< / b>
  UINT16                GttSize;                        ///< Offset 36 Selection of iGFX GTT Memory size: 1=2MB, 2=4MB, <b>3=8MB</b>
  UINT8                 Rsvd1[2];                       ///< Offset 38 Reserved for DWORD alignment
  UINT32                GmAdr;                          ///< Offset 40 Temp Address of System Agent GMADR : Default is <b>0xD0000000< / b>
  UINT16                DeltaT12PowerCycleDelayPreMem;  ///< Offset 44 Power Cycle Delay required for eDP as per VESA standard.<b>0 - 0 ms<\b>, 0xFFFF - Auto calculate to max 500 ms
  UINT8                 Reserved[2];                    ///< Offset 46 Reserved for future use.
} GRAPHICS_PEI_PREMEM_CONFIG;
#pragma pack(pop)

#endif // _GRAPHICS_PEI_PREMEM_CONFIG_H_
