/** @file
  Policy definition for Internal Graphics Config Block.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _GRAPHICS_CONFIG_H_
#define _GRAPHICS_CONFIG_H_
#pragma pack(push, 1)

#define GRAPHICS_PEI_PREMEM_CONFIG_REVISION 3
#define GRAPHICS_PEI_CONFIG_REVISION        7
#define GRAPHICS_DXE_CONFIG_REVISION        1

#define DDI_DEVICE_NUMBER   4
#define MAX_BCLM_ENTRIES    20


//
// DDI defines
//
typedef enum {
  DdiDisable       = 0x00,
  DdiDdcEnable     = 0x01,
} DDI_DDC_TBT_VAL;

typedef enum {
  DdiHpdDisable  = 0x00,
  DdiHpdEnable   = 0x01,
} DDI_HPD_VAL;

typedef enum {
  DdiPortDisabled = 0x00,
  DdiPortEdp      = 0x01,
  DdiPortMipiDsi  = 0x02,
} DDI_PORT_SETTINGS;

/**
  This structure configures the Native GPIOs for DDI port per VBT settings.
**/
typedef struct {
  UINT8 DdiPortAConfig; /// The Configuration of DDI port A, this settings must match VBT's settings. DdiPortDisabled - No LFP is connected on DdiPortA, <b>DdiPortEdp - Set DdiPortA to eDP</b>, DdiPortMipiDsi - Set DdiPortA to MIPI DSI
  UINT8 DdiPortBConfig; /// The Configuration of DDI port B, this settings must match VBT's settings. DdiPortDisabled - No LFP is connected on DdiPortB, <b>DdiPortEdp - Set DdiPortB to eDP</b>, DdiPortMipiDsi - Set DdiPortB to MIPI DSI
  UINT8 DdiPortAHpd;    /// The HPD setting of DDI Port A, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8 DdiPortBHpd;    /// The HPD setting of DDI Port B, this settings must match VBT's settings. DdiHpdDisable - Disable HPD, <b>DdiHpdEnable - Enable HPD</b>
  UINT8 DdiPortCHpd;    /// The HPD setting of DDI Port C, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8 DdiPort1Hpd;    /// The HPD setting of DDI Port 1, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8 DdiPort2Hpd;    /// The HPD setting of DDI Port 2, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8 DdiPort3Hpd;    /// The HPD setting of DDI Port 3, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8 DdiPort4Hpd;    /// The HPD setting of DDI Port 4, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8 DdiPortADdc;    /// The DDC setting of DDI Port A, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8 DdiPortBDdc;    /// The DDC setting of DDI Port B, this settings must match VBT's settings. DdiDisable - Disable DDC, <b>DdiDdcEnable - Enable DDC </b>
  UINT8 DdiPortCDdc;    /// The DDC setting of DDI Port C, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8 DdiPort1Ddc;    /// The DDC setting of DDI Port 1, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8 DdiPort2Ddc;    /// The DDC setting of DDI Port 2, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8 DdiPort3Ddc;    /// The DDC setting of DDI Port 3, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8 DdiPort4Ddc;    /// The DDC setting of DDI Port 4, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
} DDI_CONFIGURATION;

/**
  This Configuration block is to configure GT related PreMem data/variables.\n
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added DfdRestoreEnable.
  <b>Revision 3</b>:
  - Added DdiConfiguration.
  <b>Revision 4</b>:
  - Added GmAdr64 and made GmAdr obselete
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Offset 0-27 Config Block Header
  /**
    Offset 28
    Selection of the primary display device: 0=iGFX, 1=PEG, 2=PCIe Graphics on PCH, <b>3=AUTO</b>, 4=Switchable Graphics\n
    When AUTO mode selected, the priority of display devices is: PCIe Graphics on PCH > PEG > iGFX
  **/
  UINT8                PrimaryDisplay;
  /**
    Offset 29
    Intel Gfx Support. It controls enabling/disabling iGfx device.
    When AUTO mode selected, iGFX will be turned off when external graphics detected.
    If FALSE, all other polices can be ignored.
    <b>2 = AUTO</b>;
    0 = FALSE;
    1 = TRUE.
  **/
  UINT8                InternalGraphics;
  /**
    Offset 30
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
  UINT16               IgdDvmt50PreAlloc;
  UINT8                PanelPowerEnable;    ///< Offset 32 :<b>(Test)</b> Control for enabling/disabling VDD force bit (Required only for early enabling of eDP panel): 0=FALSE, <b>1=TRUE</b>
  UINT8                ApertureSize;        ///< Offset 33 :Graphics aperture size (256MB is the recommended size as per BWG) : 0=128MB, <b>1=256MB</b>, 3=512MB, 7=1024MB, 15=2048MB.
  UINT8                GtPsmiSupport;       ///< Offset 34 :PSMI support On/Off: <b>0=FALSE</b>, 1=TRUE
  UINT8                PsmiRegionSize;      ///< Offset 35 :Psmi region size: <b>0=32MB</b>, 1=288MB, 2=544MB, 3=800MB, 4=1056MB
  UINT8                DismSize;            ///< Offset 36 :DiSM Size for 2LM Sku: <b>0=0GB</b>, 1=1GB, 2=2GB, 3=3GB, 4=4GB, 5=5GB, 6=6GB, 7=7GB
  UINT8                DfdRestoreEnable;    ///< Offset 37 :Display memory map programming for DFD Restore <b>0- Disable</b>, 1- Enable
  UINT16               GttSize;             ///< Offset 38 :Selection of iGFX GTT Memory size: 1=2MB, 2=4MB, <b>3=8MB</b>
  /**
  Offset 40
  Temp Address of System Agent GTTMMADR: Default is <b>0xAF000000</b>
  **/
  UINT32               GttMmAdr;
  UINT32               GmAdr;               ///< Offset 44 Obsolete not to be used, use GmAdr64
  DDI_CONFIGURATION    DdiConfiguration;    ///< Offset 48 DDI configuration, need to match with VBT settings.

  UINT8                GtClosEnable;        ///< Offset 50 Gt ClOS
  UINT8                Rsvd0[7];            ///< Offset 51 Reserved for 4 bytes of alignment
  /**
  Offset 58
  Temp Address of System Agent GMADR: Default is <b>0xB0000000</b>
  **/
  UINT64               GmAdr64;
} GRAPHICS_PEI_PREMEM_CONFIG;

/**
  This configuration block is to configure IGD related variables used in PostMem PEI.
  If Intel Gfx Device is not supported, all policies can be ignored.
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Removed DfdRestoreEnable.
  <b>Revision 3</b>:
  - Removed DdiConfiguration.
  <b>Revision 4</b>:
  - Added new CdClock frequency
  <b>Revision 5</b>:
  - Added GT Chicket bits
  <b>Revision 6</b>:
  - Added LogoPixelHeight and LogoPixelWidth
  <b>Revision 7</b>:
  - Added SkipFspGop

**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
  UINT8                RenderStandby;        ///< Offset 28 :<b>(Test)</b> This field is used to enable or disable RC6 (Render Standby): 0=FALSE, <b>1=TRUE</b>
  UINT8                PmSupport;            ///< Offset 29 :<b>(Test)</b> IGD PM Support TRUE/FALSE: 0=FALSE, <b>1=TRUE</b>
  /**
    Offset 30
    CdClock Frequency select\n
    <b>0xFF = Auto. Max CdClock freq based on Reference Clk</b> \n
     0: 192 Mhz, 1: 307.2 Mhz, 2: 312 Mhz, 3: 324 Mhz, 4: 326.4 Mhz, 5: 552 Mhz, 6: 556.8 Mhz, 7: 648 Mhz, 8: 652.8 Mhz

  **/
  UINT16               CdClock;
  UINT8                PeiGraphicsPeimInit;  ///< Offset 32 : This policy is used to enable/disable Intel Gfx PEIM.<b>0- Disable</b>, 1- Enable
  UINT8                CdynmaxClampEnable;   ///< Offset 33 : This policy is used to enable/disable CDynmax Clamping Feature (CCF) <b>1- Enable</b>, 0- Disable
  UINT16               GtFreqMax;            ///< Offset 34 : <b>(Test)</b> Max GT frequency limited by user in multiples of 50MHz: Default value which indicates normal frequency is <b>0xFF</b>
  UINT8                DisableTurboGt;       ///< Offset 36 : This policy is used to enable/disable DisableTurboGt <b>0- Disable</b>, 1- Enable
  UINT8                SkipCdClockInit;      ///< Offset 37 : SKip full CD clock initialization. <b>0- Disable</b>, 1- Enable
  UINT8                RC1pFreqEnable;       ///< Offset 38 : This policy is used to enable/disable RC1p Frequency. <b>0- Disable</b>, 1- Enable
  UINT8                PavpEnable;           ///< Offset 39 :IGD PAVP TRUE/FALSE: 0=FALSE, <b>1=TRUE</b>
  VOID*                LogoPtr;              ///< Offset 40 Address of Intel Gfx PEIM Logo to be displayed
  UINT32               LogoSize;             ///< Offset 44 Intel Gfx PEIM Logo Size
  VOID*                GraphicsConfigPtr;    ///< Offset 48 Address of the Graphics Configuration Table
  VOID*                BltBufferAddress;     ///< Offset 52 Address of Blt buffer for PEIM Logo use
  UINT32               BltBufferSize;        ///< Offset 56 The size for Blt Buffer, calculating by PixelWidth * PixelHeight * 4 bytes (the size of EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
  UINT8                ProgramGtChickenBits; ///< Offset 60 Program GT Chicket bits in GTTMMADR + 0xD00 BITS [3:1].
  UINT8                SkipFspGop;           ///< Offset 61 This policy is used to skip PEIM GOP in FSP.<b>0- Use FSP provided GOP driver</b>, 1- Skip FSP provided GOP driver
  UINT8                Rsvd1[2];             ///< Offset 62 Reserved for 4 bytes alignment
  UINT32               LogoPixelHeight;      ///< Offset 64 Address of LogoPixelHeight for PEIM Logo use
  UINT32               LogoPixelWidth;       ///< Offset 68 Address of LogoPixelWidth for PEIM Logo use
} GRAPHICS_PEI_CONFIG;

/**
  This configuration block is to configure IGD related variables used in DXE.
  If Intel Gfx Device is not supported or disabled, all policies will be ignored.
  The data elements should be initialized by a Platform Module.\n
  <b>Revision 1</b>:
  - Initial version.
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
  UINT8                 IgdDvmtMemSize;           ///< Offset 83: IGD DVMT Memory Size: 1=128MB, <b>2=256MB</b>, 3=MAX
  UINT8                 GfxTurboIMON;             ///< Offset 84: IMON Current Value: 14=Minimal, <b>31=Maximum</b>
  UINT8                 Reserved[3];              ///< Offset 85: Reserved for DWORD alignment.
  UINT16                BCLM[MAX_BCLM_ENTRIES];   ///< Offset 88: IGD Backlight Brightness Level Duty cycle Mapping Table.
} GRAPHICS_DXE_CONFIG;
#pragma pack(pop)

#endif // _GRAPHICS_CONFIG_H_
