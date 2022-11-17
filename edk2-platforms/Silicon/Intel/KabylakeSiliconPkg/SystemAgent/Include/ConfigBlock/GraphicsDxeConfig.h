/** @file
  Graphics DXE Policy definitions

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _GRAPHICS_DXE_CONFIG_H_
#define _GRAPHICS_DXE_CONFIG_H_

#pragma pack(push, 1)

#define GRAPHICS_DXE_CONFIG_REVISION 2

/**
  This configuration block is to configure IGD related variables used in DXE.
  If Intel Gfx Device is not supported or disabled, all policies will be ignored.
  The data elements should be initialized by a Platform Module.\n
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Deprecated IgdBiaConfig and IgdSscConfig.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Offset 0-27: Config Block Header
  UINT32                Size;                     ///< Offset 28 - 31: This field gives the size of the GOP VBT Data buffer
  EFI_PHYSICAL_ADDRESS  VbtAddress;               ///< Offset 32 - 39: This field points to the GOP VBT data buffer
  UINT8                 PlatformConfig;           ///< Offset 40: This field gives the Platform Configuration Information (0=Platform is S0ix Capable for ULT SKUs only, <b>1=Platform is not S0ix Capable</b>, 2=Force Platform is S0ix Capable for All SKUs)
  UINT8                 AlsEnable;                ///< Offset 41: Ambient Light Sensor Enable: <b>0=Disable</b>, 2=Enable
  UINT8                 BacklightControlSupport;  ///< Offset 42: Backlight Control Support: 0=PWM Inverted, <b>2=PWM Normal</b>
  UINT8                 IgdBootType;              ///< Offset 43: IGD Boot Type CMOS option: <b>0=Default</b>, 0x01=CRT, 0x04=EFP, 0x08=LFP, 0x20=EFP3, 0x40=EFP2, 0x80=LFP2
  UINT32                IuerStatusVal;            ///< Offset 44 - 47: Offset 16 This field holds the current status of all the supported Ultrabook events (Intel(R) Ultrabook Event Status bits)
  CHAR16                GopVersion[0x10];         ///< Offset 48 - 79:This field holds the GOP Driver Version. It is an Output Protocol and updated by the Silicon code
  /**
    Offset 80: IGD Panel Type CMOS option\n
    <b>0=Default</b>, 1=640X480LVDS, 2=800X600LVDS, 3=1024X768LVDS, 4=1280X1024LVDS, 5=1400X1050LVDS1\n
    6=1400X1050LVDS2, 7=1600X1200LVDS, 8=1280X768LVDS, 9=1680X1050LVDS, 10=1920X1200LVDS, 13=1600X900LVDS\n
    14=1280X800LVDS, 15=1280X600LVDS, 16=2048X1536LVDS, 17=1366X768LVDS
  **/
  UINT8                 IgdPanelType;
  UINT8                 IgdPanelScaling;          ///< Offset 81: IGD Panel Scaling: <b>0=AUTO</b>, 1=OFF, 6=Force scaling
  UINT8                 IgdBlcConfig;             ///< Offset 82: Backlight Control Support: 0=PWM Inverted, <b>2=PWM Normal</b>
  UINT8                 IgdBiaConfig;             ///< Offset 83: IGD BIA Configuration: <b>0=AUTO</b>, 1=Disabled, 2=Level1, 3=Level2, 4=Level3, 5=Level4, 6=Level5 //@deprecated since Revision2
  UINT8                 IgdSscConfig;             ///< Offset 84: IGD SSC Configuration: <b>1=Off</b>, 2=Harware, 3=Software //@deprecated since Revision2
  UINT8                 LowPowerMode;             ///< Offset 85: IGD Power Conservation Feature Flag: 0=Disable, <b>2=Enable</b>
  UINT8                 IgdDvmtMemSize;           ///< Offset 86: IGD DVMT Memory Size: 1=128MB, <b>2=256MB</b>, 3=MAX
  UINT8                 GfxTurboIMON;             ///< Offset 87: IMON Current Value: 14=Minimal, <b>31=Maximum</b>
} GRAPHICS_DXE_CONFIG;
#pragma pack(pop)

#endif // _GRAPHICS_DXE_CONFIG_H_
