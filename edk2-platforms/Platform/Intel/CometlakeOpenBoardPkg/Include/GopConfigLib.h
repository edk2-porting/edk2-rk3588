/** @file
Header file for GOP Configuration Library


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _GOP_CONFIG_LIB_H_
#define _GOP_CONFIG_LIB_H_

#include <Library/DebugLib.h>
#include <Uefi/UefiBaseType.h>
#pragma pack(1)
#define GOP_CONFIG_VBT_REVISION 0xC1

#define ChildStruct_MAX                       8         ///< Maximum number of child structures in VBT
#define CompressionStruct_MAX                 2         ///< Maximum number of compression parameter structures in VBT.
#define NO_DEVICE                             0x00      ///< Defines a null display class.
#define DISPLAY_PORT_ONLY                     0x68C6    ///< Defines a display class of Integrated Display Port Only
#define DISPLAY_PORT_HDMI_DVI_COMPATIBLE      0x60D6    ///< Defines a display class of Integrated DisplayPort with HDMI/DVI Compatible
#define DISPLAY_PORT_DVI_COMPATIBLE           0x68D6    ///< Defines a display class of Integrated DisplayPort with DVI Compatible
#define HDMI_DVI                              0x60D2    ///< Defines a display class of Integrated HDMI/DVI
#define DVI_ONLY                              0x68D2    ///< Defines a display class of Integrated DVI Only
#define MIPI_ONLY                             0x1400
#define eDP_ONLY                              0x1806    ///< Defines a display class of eDP only
#define AUX_CHANNEL_A                         0x40
#define AUX_CHANNEL_B                         0x10
#define AUX_CHANNEL_C                         0x20
#define AUX_CHANNEL_D                         0x30
#define NO_PORT                               0x00      ///< Defines a output port NA
#define HDMI_B                                0x01      ///< Defines a output port HDMI-B
#define HDMI_C                                0x02      ///< Defines a output port HDMI-C
#define HDMI_D                                0x03      ///< Defines a output port HDMI-D
#define HDMI_F                                0x0E      ///< Defines a output port HDMI-D
#define DISPLAY_PORT_A                        0x0A      ///< Defines a output port DisplayPort A
#define DISPLAY_PORT_B                        0x07      ///< Defines a output port DisplayPort B
#define DISPLAY_PORT_C                        0x08      ///< Defines a output port DisplayPort C
#define DISPLAY_PORT_D                        0x09      ///< Defines a output port DisplayPort D
#define DISPLAY_PORT_E                        0x0B      ///< Defines a output port DisplayPort E
#define DISPLAY_PORT_F                        0x0D      ///< Defines a output port DisplayPort F
#define PORT_MIPI_A                           0x15      ///< Mipi Port A
#define PORT_MIPI_C                           0x17      ///< Mipi Port C

typedef struct {
  UINT16  Dclk;                         // DClk in 10 KHz
  UINT8   HActive;                      // HActive [7:0]
  UINT8   HBlank;                       // HBlank [7:0]
  UINT8   HA_HB_UpperNibble;            // Upper nibble = HActive [11:8]
  UINT8   VActive;                      // VActive [7:0]
  UINT8   VBlank;                       // VBlank [7:0]
  UINT8   VA_VB_UpperNibble;            // Upper nibble = VActive [11:8]
  UINT8   HSyncOffset;                  // HSync offset from blank start LSB
  UINT8   HPulseWidth;                  // HSync Pulse Width, LSB
  UINT8   VsyncOffset_VpulseWidth_LSB;  // Bits 7:4 = VSync offset [3:0]
  UINT8   HSO_HSPW_V_High;              // Bits 7:6 = HSync Offset [9:8]
  UINT8   HorImageSize;                 // Horizontal Image Size
  UINT8   VerImageSize;                 // Vertical Image Size
  UINT8   HIS_VIS_High;                 // UpperLmtH_V Upper limits of H. and V. image size
  UINT8   HBorder;                      // Horizontal Border
  UINT8   VBorder;                      // Vertical Border
  UINT8   Flags;                        // Flags
} DTD_STRUCTURE;                        // 18 Bytes

typedef struct {
  UINT16  XRes;
  UINT16  YRes;
  UINT32  SerialNo;
  UINT8   Week;
  UINT8   Year;
} PID_DATA;                             // 10 Bytes

//
// VBT Header
//
/**
  This structure defines the VBT Header.
**/
typedef struct {
  UINT8   Product_String[20]; ///< "$VBT_Cannonlake" is the product string
  UINT16  Version_Num;        ///< Defines the VBT Header version number.
  UINT16  Header_Size;        ///< Defines the size of VBT Header.
  UINT16  Table_Size;         ///< Defines the size of complete VBT.
  UINT8   Checksum;           ///< Defines the checksum of entire VBT
  UINT8   Reserved1;          ///< Reserved field 1 of 1 byte.
  UINT32  Bios_Data_Offset;   ///< Defines the offset of VBT Data block.
  UINT32  Aim_Data_Offset[4]; ///< 4 reserved pointers to VBT data blocks.
} VBT_HEADER;

/**
  This structure defines the VBT BIOS Data Block Header
**/
typedef struct {
  UINT8   BDB_Signature[16];  ///< Defines the Bios Data Block signature "BIOS_DATA_BLOCK".
  UINT16  BDB_Version;        ///< Defines the VBT (data) version.
  UINT16  BDB_Header_Size;    ///< Defines the size of VBT Bios data block header.
  UINT16  BDB_Size;           ///< Defines the size of Bios data block.
} VBT_BIOS_DATA_HEADER;

/**
  This structure defines the BMP Signon Message and Copyright Message Structure
**/
typedef struct {
  UINT8   BlockId;            ///< Defines Block ID : 254
  UINT16  BlockSize;          ///< Defines the size of BMP Signon block.

  UINT16  Bmp_BIOS_Size;      ///< Defines the BIOS size 32k/48k/64k.
  UINT8   BIOS_Type;          ///< Defines the type of BIOS desktop or mobile.
  UINT8   RelStatus;          ///< Defines the release status of the current GOP driver.
  UINT8   BIOS_HW;            ///< Defines the Hardware i.e. Cannonlake.
  UINT8   INT_HW;             ///< Defines the integrated hardware supported eDP/HDMI/DP.
  UINT8   Build_Number[4];    ///< Defines the build number string.
  UINT8   SignOn[155];        ///< Defines the sign on message.
  UINT8   CopyRight[61];      ///< Defines the copyright message.
} BMP_STRUCTURE_SIGNON;

/**
  This structure defines the BMP General Bits
**/
typedef struct {
  UINT16  bmp_BIOS_CS;          ///< Defines the start of BIOS code segment
  UINT8   bmp_DOS_Boot_Mode;    ///< Defines the mode number to set when DOS is boot
  UINT8   bmp_BW_Percent;       ///< Set percentage of total memory BW
  UINT8   bmp_Popup_Mem_Size;   ///< Default Popup memory size in KB
  UINT8   bmp_Resize_PCI_BIOS;  ///< BIOS size granularity in 0.5 KB
  UINT8   Switch_CRT_To_DDC2;   ///< Obsolete field: Is the CRT already switched to DDC2
  UINT8   bmp_Allow_Config;     ///< Bit 1 : 1, Enable aspect ratio for DOS
                                ///< Bit 0 : 1, Allow boot to DVI even if it is not attached.
} BMPGEN;

/**
  This structure defines Block 254 (BMP structure)
**/
typedef struct {
  BMP_STRUCTURE_SIGNON    bmp_Signon_Message;   ///< Instance of signon and copyright message structure
  BMPGEN                  bmp_General_Bytes;    ///< Instance of BMP General Bits structure.
} BLOCK254_BMP_Structure;

/**
  This structure defines Block 1 (General Bytes Definitions)
**/
typedef struct {
  UINT8   BlockId;        ///< Defines the Block ID (1)
  UINT16  BlockSize;      ///< Defines the size of General bytes definition block.

  /**
  BMP General Bit Definitions 1\n
  Bit 7 = DVO A color flip bit
    = 0, No DVO A color flip
    = 1, Flip DVO A color
  Bits 6:4 = Clear screen (CLS) after Signon
      = 000, No CLS
      = 001, 0.5 sec pause and then CLS
      = 010, 1.0 sec pause and then CLS
      = 011, 1.5 sec pause and then CLS
      = 100, 2.0 sec pause and then CLS
      = 101, 2.5 sec pause and then CLS
      = 110, 3.0 sec pause and then CLS
      = 111, 3.5 sec pause and then CLS
  Bit 3 = 1  Enable Display Signon
  Bit 2 = 1  Enable Flex-aim Support
  Bits 1:0 = Flat panel fitting enabling
      = 00, Centering
      = 01, Reserved
      = 10, Aspect scaling
      = 11, Fullscreen
  **/
  union {
    UINT8  Value;
    struct {
      UINT8 PanelFitterEnabling:2;
      UINT8 FlexAimSupportEnable:1;
      UINT8 DisplaySignonEnable:1;
      UINT8 ClearScreenTime:3;
      UINT8 DvoAColorFlipEnable:1;
    } Bits;
  } bmp_Bits_1;

  /**
  BMP General Bit Definitions\n
  Bit 7 = Hot plug support
    = 0, Hot plug disabled
    = 1, Hot plug enabled
  Bit 6 = Dynamic CD clock feature
    = 0, Dynamic CD clock feature is disabled
    = 1, Dynamic CD clock feature is enabled
  Bit 5 = Underscan support for VGA timings
  Bit 4 = Disable SSC in Dual Display Twin Mode. (This field is obsolete now. Kept for VBIOS only.)
    = 0, No
    = 1, Yes
  Bit 3 = LFP power state on override by 5f64h,08h
    = 0, No Override
    = 1, Override
  Bit 2 = Internal LVDS SSC frequency. (This field is obsolete now. Kept for VBIOS only.)
    = 0, 96/120MHz
    = 1, 100MHz
  Bit 1 = internal LVDS SSC (Spread Spectrum Clock) (This field is obsolete now. Kept for VBIOS only.)
    = 0, Disabled
    = 1, Enabled
  Bit 0 = KvmrSessionEnable.
    = 0, Disabled
    = 1, Enabled
  **/
  union {
    UINT8 Value;
    struct {
      UINT8 KvmrSessionEnable:1;
      UINT8 Reserved_1:5;
      UINT8 DynamicCdClockEnable:1;
      UINT8 HotPlugEnable:1;
    } Bits;
  } bmp_Bits_2;

  /**
  BMP General Bit Definitions 3\n
  Bit 7 = Ignore strap status
      = 0 Do not ignore
      = 1 Ignore
  Bit 6 = Panel Timing algorithm
      = 0 Preferred timings
      = 1 Best fit timings
  Bit 5 Copy iLFP DTD to SDVO LVDS DTD
      = 0 Don't copy DTD
      = 1 Copy DTD to
  Bit 4 = VBIOS normal/extd. DT mode
      = 0 Normal mode
      = 1 DUAL mode
  Bit 3 = FDI RX Polarity
      = 0 Normal
      = 1 Inverted
  Bit 2 = Enable 180 Degree Rotation
      = 0  Disable
      = 1  Enable
  Bit 1 = Single DVI-I connector for CRT and DVI display: Obsolete field
      = 0 Disabled
      = 1 Enabled
  Bit 0 = Smooth Vision
      = 0  Disabled
      = 1  Enabled
  **/
  union {
    UINT8 Value;
    struct {
      UINT8 Reserved1:1;
      UINT8 SingleDviiCrtConnector:1;
      UINT8 Enable180DegRotation:1;
      UINT8 FdiRxPolarity:1;
      UINT8 Reserved2:4;
    } Bits;
  } bmp_Bits_3;

  UINT8   Reserved;     ///< Reserved field. It was Legacy_Monitor_Detect in previous platforms.

  /**
  Integrated display device support\n
  Bits 7:6 = Reserved
  Bit 5 = DP SSC Dongle Enable/Disable
  Bit 4 = DP SSC Frequency. (This field is obsolete now. Kept for VBIOS only.)
    = 0, 96 MHz
    = 1, 100 MHz
  Bit 3 = DP SSC Enable
    = 0, Disable
    = 1, Enable
  Bit 2 = Integrated EFP support
    = 0, Disable
    = 1, Enable
  Bit 1 = Integrated TV support. (This field is obsolete now. Kept for VBIOS only.)
    = 0, Disable
    = 1, Enable
  Bit 0 = Integrated CRT support: Obsolete field
    = 0, Disable
    = 1, Enable
  **/
  union {
    UINT8 Value;
    struct {
      UINT8 CrtSupported:1;
      UINT8 TvSupported:1;
      UINT8 EfpSupported:1;
      UINT8 DpSscEnable:1;
      UINT8 DpSscFrequency:1;
      UINT8 DpDongleSscEnable:1;
      UINT8 Reserved1:2;
    } Bits;
  } Int_Displays_Support;
} VBT_GENERAL1_INFO;

/**
  This defines the Structure of PRD Boot Table Entry
**/
typedef struct {
  UINT8 AttachBits;     ///< Bitmap representing the displays attached currently.
  UINT8 BootDev_PipeA;  ///< Bitmap representing the display to boot on Pipe A.
  UINT8 BootDev_PipeB;  ///< Bitmap representing the display to boot on Pipe B.
} PRD_TABLE;

/**
  This defines the structure of Block 254 (PRD Boot Table/Child Device List)
**/
typedef struct {
  UINT8     BlockId;    ///< Defines the Block ID (254)
  UINT16    BlockSize;  ///< Defines the size of Block 254

  PRD_TABLE PRDTable[16];                     ///< Defines the Child device list for enumerating child handles.
  UINT16    PRD_Boot_Table_Number_Of_Entries; ///< Number of entries in child device list.
} PRD_BOOT_TABLE;

/**
  This defines the Structure for a CHILD_STRUCT (used for all the displays).
**/
typedef struct {
  UINT16  DeviceHandle;         ///< Unique ID indicating the group of display device (LFP/EFP1/EFP2/EFP3/EFP4).
  UINT16  DeviceClass;          ///< Indicates the class of display device.
  UINT8   I2CSpeed;             ///< Defines the I2C speed to be used for I2C transaction.
  /**
  Defines the DP on board redriver configuration.
  BIT[7]    : Reserved
  BIT[6]    : Is On Board DP Redriver Present
          0 : No
          1 : Yes
  BIT[5:3]  : On Board Redriver VSwing Level
          0 : Level 0
          1 : Level 1
          2 : Level 2
          3 : Level 3
     Others : Reserved
  BIT[2:0]  : On Board Redriver PreEmph Level
          0 : Level 0
          1 : Level 1
          2 : Level 2
          3 : Level 3
  Others    : Reserved
  **/
  union{
  UINT8 Value;
  struct {
    UINT8 OnBoardPreEmphLevel:3;
    UINT8 OnBoardVSwingLevel:3;
    UINT8 OnBoardRedriverPresent:1;
    UINT8 Reserved:1;
    } Bits;
  } DpOnBoardRedriver;

  /**
  Defines the DP on dock redriver configuration.
  BIT[7]    : Reserved
  BIT[6]    : Is On Dock DP Redriver Present
          0 : No
          1 : Yes
  BIT[5:3]  : On Dock Redriver VSwing Level
          0 : Level 0
          1 : Level 1
          2 : Level 2
          3 : Level 3
     Others : Reserved
  BIT[2:0]  : On Dock Redriver PreEmph Level
          0 : Level 0
          1 : Level 1
          2 : Level 2
          3 : Level 3
  Others    : Reserved
  **/
  union {
  UINT8 Value;
  struct {
    UINT8 OnDockPreEmphLevel:3;
    UINT8 OnDockVSwingLevel:3;
    UINT8 OnDockRedriverPresent:1;
    UINT8 Reserved:1;
    } Bits;
  } DpOnDockRedriver;

  /**

  Defines the HDMI level shifter configuration.
  BIT[7:5]  : Hdmi Maximum data rate
  BIT[4:0]  : Hdmi Level shifter value

  **/
  union{
  UINT8 Value;
  struct {
    UINT8 HdmiLevelShifterValue:5;
    UINT8 HdmiMaxDataRateBits:3;
    } Bits;
  } HdmiLevelShifterConfig;

  UINT16  EFPDTDBufferPointer;  ///< Pointer to the DTD timing to be used in case of edidless EFP.

  /**
  Defines the first set of flags.
  BIT[7-4]  : Reserved
  BIT[3]    : Dual pipe ganged display support
          0 : Display uses a single pipe/port
          1 : Display uses two distinct pipes/ports.
  BIT[2]    : Compression Method Select
          0 : Compression using picture parameter set (PPS)
          1 : Compression using Capability parameter set (CPS)
  BIT[1]    : Compression enable/disable for this display.
          0 : Disabled
          1 : Enabled
  BIT[0]    : EDID less EFP Enable
          0 : Enable support for EDID less EFP.
          1 : Disable support for EDID less EFP.
  **/
  union {
  UINT8 Value;
  struct {
    UINT8 EdidlessEfpEnable:1;
    UINT8 CompressionEnable:1;
    UINT8 CompressionMethod:1;
    UINT8 IsDualPortEnabled:1;
    UINT8 Reserved:4;
    } Bits;
  } Flags0;

  /**
  Defines the compression index field for the display.
  BITS[7-4]  : Reserved
  BITS[3-0]  : Compression Structure index in the block 55.
        0x0  : Index 0 in block 55
        0x1  : Index 1 in block 55
        0xF  : Not Applicable.
      Others : Reserved
  **/
  union {
  UINT8 Value;
  struct {
    UINT8 IndexInBlock55:4;
    UINT8 Reserved:4;
    } Bits;
  } CompressionStructureIndex;

  UINT8   SlaveDdiPort;         ///< The DVO port number of slave DDI to be used in case Flags0[3] = 1.

  UINT8   Reserved_1;           ///< Reserved and might be used in other platforms.
  UINT16  AddInOffset;          ///< Obsolete field.
  UINT8   DVOPort;              ///< Specifies the port number of the display device represented in the device class.
  UINT8   I2CBus;               ///< Specifies the GMBUS or I2C pin pair for add in card.
  UINT8   SlaveAddr;            ///< Specifies the I2C address of the add in card.
  UINT8   DDCBus;               ///< Specifies the GMBUS pin pair for EDID read.
  UINT16  TimingInfoPtr;        ///< Pointer to the buffer where VBIOS stores the EDID of device.
  UINT8   DVOCfg;               ///< Obsolete field.

  /**
  Flags 1\n
  Bits 7:5  : Reserved
  Bit 4     : HPD Sense Invert
          0 : Invert not required (default)
          1 : Invert required
  Bit 3     : IBoost feature enable/disable.
          0 : IBoost feature is disabled.
          1 : IBoost feature is enabled.
  Bit 2     : Hdmi 2.0 Motherboard Downsuppotred options
          0 : Motherboard Down chip not supported
          1 : Motherboard Down Chip Supported on the Board
  Bit 1     : Lane Reversal feature.
          0 : Disable
          1 : Enable
  Bit 0     : DP/HDMI routed to dock.
          0 : Disable
          1 : Enable
  **/
  union {
  UINT8 Value;
  struct {
    UINT8 DockablePort:1;
    UINT8 EnableLaneReversal:1;
    UINT8 OnBoardLsPconDonglePresent:1;
    UINT8 IBoostEnable:1;
    UINT8 IsHpdInverted:1;
    UINT8 Reserved:3;
    } Bits;
  } Flags_1;

  UINT8   Compatibility;        ///< Compatibility is used in VBIOS only. It was used before device class was defined.
  UINT8   AUX_Channel;          ///< Specifies the aux channel to be used for display port devices.
  UINT8   Dongle_Detect;        ///< Indicates whether dongle detect is enabled or not.
  UINT8   Capabilities;         ///< Bits 1-0 indicate pipe capabilities whether display can be used on one pipe or both the pipes.
  UINT8   DVOWiring;            ///< Obsolete field.
  UINT8   MipiBridgeType;       ///< MIPI bridge type
  UINT16  DeviceClassExtension; ///< Obsolete.
  UINT8   DVOFunction;          ///< Obsolete.

  /**
  Flags 2
  Bits 7:4  : DP Port trace length from silicon to output port on the board
          0 : Default RVP length
          1 : Short trace length
          2 : Long trace length
  Bits 3:2  : Reserved
  Bit 1     : Indicates whether this port is Thunderbolt port or not.
          0 : No
          1 : Yes
  Bit 0     : DP 2 lane RCR# 1024829: USB type C to enable 2 lane DP display
          0 : Disable
          1 : Enable
  **/
  union {
    UINT8   Value;
    struct {
      UINT8   UsbTypeCDongleEnabled:1;  ///< Indicates whether this port is USB type C.
      UINT8   IsThunderboltPort:1;      ///< Indicates whether this port is Thunderbolt. (ICL+)
      UINT8   Reserved:2;               ///< Reserved for future use.
      UINT8   DpPortTraceLength:4;      ///< Dp port trace length from silicon to port.
    } Bits;
  } Flags_2;
  UINT8   DP2XGpioIndex;        ///< GPIO index number for the USB type C.
  UINT16  DP2XGpioNumber;       ///< GPIO number for USB type C.

  /**
  IBoost magnitude field.
  Bits 7:4  : DP Boost magnitude
          0 : 1
          1 : 3
          2 : 7
     Others : Reserved for CML.
  Bits 3:0  : HDMI Boost magnitude
          0 : 1
          1 : 3
          2 : 7
  Others : Reserved.
  **/
  union {
    UINT8   Value;
    struct {
      UINT8   DpEdpBoostMagnitude:4;
      UINT8   HdmiBoostMagnitude:4;
    } Bits;
  } BoostMagnitude;
} CHILD_STRUCT;

/**
  This structure defines Block 2 (General Bytes Definitions)
**/
typedef struct {
  UINT8         BlockId;          ///< Defines the Block ID : 2.
  UINT16        BlockSize;        ///< Defines the size of VBT General Info 2 Block.

  UINT8         bmp_CRT_DDC_GMBUS_Pin;  ///< Obsolete field: Selects the CRT DDC GMBUS pin pair.
  UINT8         bmp_DPMS_Bits;          ///< BMP DPMS Bit Definitions.
  UINT16        bmp_Boot_Dev_Bits;      ///< BMP Boot Device Bit Definitions.
  UINT8         SizeChild_Struct;       ///< Size of the ChildStruc structure.

  CHILD_STRUCT  Child_Struct[ChildStruct_MAX];  ///< This array defines all the supported child structures.
} VBT_GENERAL2_INFO;

/**
  This defines the structure of Block 3 (Original Display Toggle List)
**/
typedef struct {
  UINT8   BlockId;                ///< Defines the Block ID : 3
  UINT16  BlockSize;              ///< Defines the size of Original Display Toggle List Block
  UINT8   bmp_Display_Detect;     ///< Display must be attached or not
} BLOCK03_ORIGINAL_DISPLAY_TOGGLE_LIST;

/**
  This defines structure of a pointer table.
**/
typedef struct {
  UINT16  Offset;       ///< Defines the offset of the table from start of BIOS Data block.
  UINT16  Size;         ///< Defines the size of an entry of the table.
} BMP_TABLE_PTR;

/**
  This structure defines Block 252 (SBIOS Hooks and BMP Table Pointers).
**/
typedef struct {
  UINT8           BlockId;          ///< Defines the Block ID : 252.
  UINT16          BlockSize;        ///< Defines the size of SBIOS Hooks block.
  UINT8           SbiosHooks[18];   ///< This array defines a series of SBIOS hooks. Each entry represents one hook.
  BMP_TABLE_PTR   BmpTablePtr[26];  ///< This array defines pointers to all the important tables in the VBT.
} BLOCK252_SBIOS_Hook;

/**
  This defines the structure of MMIO boot table entry
**/
typedef struct {
  UINT32  Register;   ///< Defines the MMIO offset of the register.
  UINT32  Value;      ///< Defines the default value of the register.
} MMIO_BOOT_TABLE;

/**
  This structure defines Block 6 (MMIO Register Block)
**/
typedef struct {
  UINT8           BlockId;              ///< Defines the Block ID : 6
  UINT16          BlockSize;            ///< Defines the size of MMIO Register Table block.
  UINT16          RegTableId;           ///< Defines the ID for MMIO register table (0xFFFC).
  UINT8           AccessFlag;           ///< Defines the flag for data access size (02 for 4 byte read/write).
  MMIO_BOOT_TABLE MMIOBootTable[14];    ///< Array containing the MMIO register table.
  UINT16          TableEnd;             ///< Special value describing End of table (0xFFFF).
} BLOCK06_MMIO_REG_TABLE;

/**
  This structure defines Block 7 (IO SW Flag Register Table)
**/
typedef struct {
  UINT8   BlockId;          ///< Defines Block ID (7).
  UINT16  BlockSize;        ///< Defines the size of IO SW Flag register table block.
  UINT16  RegTableId;       ///< Defines the ID for IO SW Flag register table (0xFFFE).
  UINT8   GRIndexRegLsb;    ///< Defines the read/write size. Value is 0xCE meaning 1 byte without mask.
  UINT8   IOSWFlagReg;      ///< Defines the offset for the IO SW Flag register.
  UINT8   Value;            ///< Defines the data/value for the register.
  UINT16  TableEnd;         ///< Special value describing the end of table (0xFFFF).
} BLOCK07_IOSWFLAG_REG_TABLE;

/**
  This structure defines the entry of SWF table.
**/
typedef struct {
  UINT32  Register;   ///< Defines the MMIO offset of the SWF register.
  UINT32  Value;      ///< Defines the default value for the SWF register.
} SWF_TABLE;

/**
  This defines the structure of Block 8 (MMIO SW Flag Block).
**/
typedef struct {
  UINT8     BlockId;      ///< Defines the Block ID : 8.
  UINT16    BlockSize;    ///< Defines the size of MMIO SWF register table block.
  UINT16    RegTableId;   ///< Defines the ID for MMIO SWF register table (0xFFFC).
  UINT8     AccessFlag;   ///< Defines the data access size. Value is 0x02 meaning 4 bytes read/write.
  SWF_TABLE SWFTable[7];  ///< Array containing the MMIO SWF register table.
  UINT16    TableEnd;     ///< Special value describing end of table (0xFFFF).
} BLOCK08_MMIOSWFLAG_REG_TABLE;

/**
  This structure defines the PSR feature table entry.
**/
typedef struct {
  UINT8   SRD_Enables;        ///< Defines PSR features such as full link enable/disable and whether aux is required to wake up.
  UINT8   SRD_WaitTimes;      ///< Defines lines to wait before link standby and idle frames to wait before SRD enable.
  UINT16  SRD_TP1_WakeupTime; ///< TP 1 wake up time in multiples of 100.
  UINT16  SRD_TP2_WakeupTime; ///< TP2/TP3 wake up time in multiples of 100
} PSR_FEATURE_TABLE;

/**
  This defines the structure of Block 9 (PSR Features Block)
**/
typedef struct {
  UINT8             BlockId;              ///< Defines the block ID : 9
  UINT16            BlockSize;            ///< Defines the size of PSR Feature block.
  PSR_FEATURE_TABLE PSRFeatureTable[16];  ///< Array containing the PSR Feature table.
} BLOCK09_PSR_FEATURE;

/**
  This structure defines an entry of Mode Removal table.
**/
typedef struct {
  UINT16  XRes;         ///< X resolution of the mode.
  UINT16  YRes;         ///< Y resolution of the mode.
  UINT8   Bpp;          ///< Bits per pixel of the mode.
  UINT16  RRate;        ///< Refresh rate of the mode.
  UINT8   RFlags;       ///< Flags specifying display type and functional area where the mode is to be removed.
  UINT16  PanelFlags;   ///< Applicable to LFP only. Indicates which LFP panels the mode is to be removed.
} MODE_REMOVAL_TABLE_ENTRY;

/**
  This defines the structure of Block 10 (Mode Removal Block)
**/
typedef struct {
  UINT8                     BlockId;              ///< Defines the Block ID : 10.
  UINT16                    BlockSize;            ///< Defines the size of Mode Removal table block.
  UINT8                     EntrySize;            ///< Defines the size of one entry of mode removal table.
  MODE_REMOVAL_TABLE_ENTRY  ModeRemovalTable[20]; ///< Array containing the mode removal table.
  UINT16                    Terminator;           ///< Special value indicating end of mode removal table (0xFFFF).
} BLOCK10_MODE_REMOVAL_TABLE;

/**
  This defines the structure of Block 12 (Driver Features Data Block)
**/
typedef struct {
  UINT8   BlockId;                  ///< Defines the unique Block ID : 12
  UINT16  BlockSize;                ///< Defines the size of Driver features block.

  /**
  This field defines the various driver related bits:\n
  Bit 7 = Use 00000110h ID for Primary LFP
        = 0, No
        = 1, Yes
  Bit 6 = Enable/Disable Sprite in Clone Mode
        = 0, Disable
        = 1, Enable
  Bit 5 = Driver INT 15h hook
        = 0, Disable
        = 1, Enable
  Bit 4 = Dual View Zoom
        = 0, Disable
        = 1, Enable
  Bit 3 = Hot Plug DVO
        = 0, Disable
        = 1, Enable
  Bit 2 = Allow display switching when in Full Screen DOS.
        = 0, Block Display Switching
        = 1, Allow Display Switching
  Bit 1 = Block display switching when DVD active
        = 0, No Block Display Switching
        = 1, Block Display Switching
  Bit 0 = Boot device algorithm
        = 0, OS Default
        = 1, Driver Default
  **/
  UINT8   bmp_Driver_Bits;
  UINT16  bmp_Driver_Boot_Mode_X;   ///< X resolution of driver boot mode.
  UINT16  bmp_Driver_Boot_Mode_Y;   ///< Y resolution of driver boot mode.
  UINT8   bmp_Driver_Boot_Mode_BPP; ///< Bits per pixel of driver boot mode.
  UINT8   bmp_Driver_Boot_Mode_RR;  ///< Refresh rate of driver boot mode.

  /**
  This field defines the extended driver bits 1.\n
  Bits [15:14] = Integrated HDMI configuration
              = 00b,  No Integrated HDMI
              = 01b,  Port-B Only
              = 10b,  Port-C Only
              = 11b,  Both Port-B and Port-C
  Bits 13 = TV Hotplug
  Bits [12:11]  = LFP configuration
                = 00b,  No LVDS
                = 01b,  Integrated LVDS
                = 10b,  SDVO LVDS
                = 11b,  eDP
  Bit 10 = Obsolete field: CRT hotplug
          = 0, Disabled
          = 1, Enabled (Default)
  Bit 9 = SDVO device power down
        = 0, Disabled (Default)
        = 1, Enabled
  Bit 8 = Preserve Aspect Ratio
        = 0, Disabled (Default)
        = 1, Enabled
  Bit 7 = Display "Maintain Aspect Scaling" via CUI
        = 0, No
        = 1, Yes (Default)
  Bit 6 = Sprite Display Assignment when Overlay is Active in Clone Mode:
        = 0, Secondary
        = 1, Primary
  Bit 5 = Default Power Scheme user interface
        = 0, CUI
        = 1, 3rd Party Application
  Bit 4 = NT 4.0 Dual Display Clone Support
        = 0, Disable
        = 1, Enable
  Bit 3 = Default Render Clock Frequency
        = 0, High Frequency
        = 1, Low Frequency
  Bit 2 = Dual-Frequency Graphics Technology
        = 0, No
        = 1, Yes
  Bit 1 = Selective Mode Pruning
        = 0, No
        = 1, Yes
  Bit 0 = Enable LFP as primary
        = 0, Disable
        = 1, Enable
**/
  UINT16  bmp_Ext_Driver_Bits;

  /**
  This defines the driver flags related to CUI Hot key.\n
  Bits [7:3] - Reserved
  Bit 2 = Display Subsystem Enable/Disable
        = 0, Enable (default Value)
        = 1, Disable
  Bit 1 = Embedded Platform
        = 0, False
        = 1, True
  Bit 0 = Define CUI HotK Displays Statically
        = 0, No
        = 1, Yes
  **/
  UINT8   bmp_Display_Detect_CUIHotK;

  UINT16  bmp_Legacy_CRT_Max_X;         ///< Obsolete field: Defines the legacy CRT X resolution for driver boot mode.
  UINT16  bmp_Legacy_CRT_Max_Y;         ///< Obsolete field: Defines the legacy CRT Y resolution for driver boot mode.
  UINT8   bmp_Legacy_CRT_Max_RR;        ///< Obsolete field: Defines the legacy CRT refresh rate for driver boot mode.

  /**
  This field defines the extended driver bits 2.\n
  Bits [7:1] - Reserved
  Bit 0 = Enable Internal Source Termination for HDMI
        = 0, External Termination
        = 1, Internal Termination
  **/
  UINT8   bmp_Ext2_Driver_Bits;

  UINT8   bmp_VBT_Customization_Version;  ///< Defines the customized VBT version number.

  /**
  This field defines all the driver feature flags.\n
  Bit 15 = PC Features Field's Validity
         = 0, Invalid
         = 1, Valid
  Bit 14 = Hpd_wake - HPD events are routed to display driver when system is in S0ix/DC9
         = 0, Disable
         = 1, Enable
  Bit 13 = Assertive Display Technology (ADT)
         = 0, Disable
         = 1, Enable
  Bit 12 = Dynamic Media Refresh Rate Switching (DMRRS)
         = 0, Disable
         = 1, Enable
  Bit 11 = Dynamic Frames Per Second (DFPS)
         = 0, Disable
         = 1, Enable
  Bit 10 = Intermediate Pixel Storage (IPS)
         = 0, Disable
         = 1, Enable
  Bit 9 = Panel Self Refresh (PSR)
        = 0, Disable
        = 1, Enable
  Bit 8 = Intel Turbo Boost Technology
        = 0, Disable
        = 1, Enable
  Bit 7 = Graphics Power Modulation Technology (GPMT)
        = 0, Disable
        = 1, Enable
  Bit 6 = Graphics Render Standby (RS)
        = 0, Disable
        = 1, Enable
  Bit 5 = Intel Display Refresh Rate Switching (DRRS)
        = 0, Disable
        = 1, Enable
  Bit 4 = Intel Automatic Display Brightness (ADB)
        = 0, Disable
        = 1, Enable
  Bit 3 = DxgkDDI Backlight Control (DxgkDdiBLC)
        = 0, Disable
        = 1, Enable
  Bit 2 = Intel Display Power Saving Technology (DPST)
        = 0, Disable
        = 1, Enable
  Bit 1 = Intel Smart 2D Display Technology (S2DDT)
        = 0, Disable
        = 1, Enable
  Bit 0 = Intel Rapid Memory Power Management (RMPM)
        = 0, Disable
        = 1, Enable
  **/
  UINT16  bmp_Driver_Feature_Flags;
} BLOCK12_DRIVER_FEATURES;

/**
  This defines the structure of Block 13 (Driver Persistence Options)
**/
typedef struct {
  UINT8   BlockId;                ///< Defines the unique Block ID : 13
  UINT16  BlockSize;              ///< Defines the size of Driver Persistence options block.

  /**
  Defines the various persistence options.\n
  Bits [15:10] - Reserved
  Bit 9 = Docking Persistence Algorithm
        = 0, OS Default
        = 1, Driver Default
  Bit 8 = DVO Hot Plug Persistence on Mode
  Bit 7 = EDID Persistence on Mode
  Bit 6 = Hot Key Persistence on Mode
        = 0, No
        = 1, Yes
  Bit 5 = Hot Key Persistence on RestorePipe
        = 0, No
        = 1, Yes
  Bit 4 = Hot Key Persistence on RefreshRate
        = 0, No
        = 1, Yes
  Bit 3 = Hot Key Persistence on MDS/Twin
        = 0, No
        = 1, Yes
  Bit 2 = Power Management Persistence Algorithm
        = 0, OS Default
        = 1, Driver Default
  Bit 1 = Lid Switch Persistence Algorithm
        = 0, OS Default
        = 1, Driver Default
  Bit 0 = Hot Key Persistence Algorithm
        = 0, OS Default
        = 1, Driver Default
  **/
  UINT16  PersistenceAlgorithm;

  UINT8   PersistMaxconfig;       ///< Maximum mode persistence configurations (10-200)
} BLOCK13_DRIVER_PERSISTENCE;

/**
  This defines the structure of Block 17 (SV Bits)
**/
typedef struct {
  UINT8   BlockId;      ///< Defnies the unique Block ID : 17
  UINT16  BlockSize;    ///< Defines the size of SV Bits block.

  /**
  Bits [7:4] = Reserved
    Bit3  = Allow VBlank/VblankScanline timeout hang
          = 0, Disable
          = 1, Enable
    Bit2  = Special GMBus support
          = 0, Disable
          = 1, Enable
    Bit1  = Skip program pipe timings when set VGA modes
          = 0, Setmode skip DVO Update
          = 1, Setmode updates DVO
    Bit0  = Disable VGA fast arbiter
          = 0, Enabled
          = 1, Disabled
  **/
  UINT8   SvBits1;
  UINT8   SvBits2;      ///< Reserved for future use.
  UINT8   SvBits3;      ///< Reserved for future use.
  UINT8   SvBits4;      ///< Reserved for future use.
  UINT8   SvBits5;      ///< Reserved for future use.
  UINT8   SvBits6;      ///< Reserved for future use.
  UINT8   SvBits7;      ///< Reserved for future use.
  UINT8   SvBits8;      ///< Reserved for future use.
} BLOCK17_SV_BITS;

/**
  This defines the structure of Block 18 (Driver Rotation)
**/
typedef struct {
  UINT8   BlockId;                    ///< Defines the unique Block ID : 18
  UINT16  BlockSize;                  ///< Defines the size of Driver Rotation block.
  UINT8   RotationFeatureSupport;     ///< Rotation feature support field used by driver.
  UINT8   Reserved1;                  ///< Reserved for future use.
  UINT16  Reserved2;                  ///< Reserved for future use.
  UINT32  Reserved3;                  ///< Reserved for future use.
  UINT32  Reserved4;                  ///< Reserved for future use.
} BLOCK18_DRIVER_ROTATION;

/**
  This structure defines an entry of OEM mode table.
**/
typedef struct {
  /**
  Mode Flags:
    Bits[7:3] = Reserved
    Bit 2 = Enable/disable this OEM mode in GOP driver.
    Bit 1 = Enable/disable this mode in Driver
    Bit 0 = Enable/disable this mode in VBIOS
  **/
  UINT8   ModeFlags;

  /**
  Display Device Flags:
    Bit 7 = LFP2
    Bit 6 = EFP2
    Bit 5 = EFP3
    Bit 4 = EFP4
    Bit 3 = LFP
    Bit 2 = EFP
    Bit 1 = Rsvd
    Bit 0 = Rsvd
  **/
  UINT8   DisplayFlags;
  UINT16  XRes;         ///< Defines the X resolution of the mode.
  UINT16  YRes;         ///< Defines the Y resolution of the mode.

  /**
  Defines the bits per pixel of the mode.
    Bit 7:3 = Reserved
    Bit 2 = 32 BPP
    Bit 1 = 16 BPP
    Bit 0 = 8 BPP
  **/
  UINT8   Bpp;
  UINT8   RRate;        ///< Defines the refresh rate of the mode.
  DTD_STRUCTURE Dtd;    ///< Defines the 18 byte timing config for the mode.
} OEM_MODE_ENTRY;

/**
  This defines the structure of Block 20 (OEM Mode Customization Block)
**/
typedef struct {
  UINT8           BlockId;          ///< Defines the unique block ID : 20
  UINT16          BlockSize;        ///< Defines the size of OEM customization block.
  UINT8           NumOfEntry;       ///< Defines the number of entries in OEM Mode table.
  UINT8           EntrySize;        ///< Defines the size of one entry of OEM Mode table.
  OEM_MODE_ENTRY  OemModeTable[6];  ///< Array defining the OEM mode table.
} BLOCK20_OEM_CUSTOMIZATION;

/**
  This defines the structure of Block 26 (TV options)
**/
typedef struct {
  UINT8   BlockId;                  ///< Defines the unique Block ID : 26
  UINT16  BlockSize;                ///< Defines the size of TV Options block.

  /**
  Defines the TV options:
    Bit 15  = D-Conector Support
            = 0, Disable
            = 1, Enable
    Bit 14 = Add 1776x1000 when 1080i is selected and add 1184x666 when 720p is selected
            = 0, Disable
            = 1, Enable
    Bit 13:12 Underscan/overscan for HDTV via DVI
            = 00b, Enable Underscan and Overscan modes (Default)
            = 01b, Enable only overscan modes
            = 10b, Enable only underscan modes
    Bits 11:2 = Reserved
    Bit 1:0 = Underscan/overscan for HDTV via Component (YPrPb)
            = 00b, Enable Underscan and Overscan modes (Default)
            = 01b, Enable only overscan modes
            = 10b, Enable only underscan modes
  **/
  UINT16  bmp_TV_Options_1;
} BLOCK26_TV_OPTIONS;

/**
  This structure defines the eDP panel power sequencing parameters.
**/
typedef struct {
  UINT16  T3;         ///< Panel Power-Up Delay.
  UINT16  T8;         ///< Panel Power-On to backlight Enable Delay.
  UINT16  T9;         ///< Backlight-Off to Power-Down Delay.
  UINT16  T10;        ///< Power-Down Delay.
  UINT16  T12;        ///< Power Cycle Delay.
} EDP_PWR_SEQ;

/**
  This structure defines the PWM<-->Backlight delays for a single eDP panel.
**/
typedef struct {
  UINT16  PwmOnToBacklightEnableDelay;      ///< PWM on to backight enable delay.
  UINT16  BacklightDisableToPwmOffDelay;    ///< Backlight disable to PWM off delay.
} EDP_PWM_BACKLIGHT_DELAYS;

/**
  This defines FLT parameters for a single eDP panel.
  Bits[15:12] : VSwing level
            0 : 0.4V (default)
            1 : 0.6V
            2 : 0.8V
            3 : 1.2V
       Others : Reserved
  Bits[11:8]  : Pre-emphasis level
            0 : no pre-emphasis (default)
            1 : 3.5dB
            2 : 6dB
            3 : 9.5dB
       Others : Reserved
  Bits[7:4]   : Lane count (port width)
            0 : x1 mode (default)
            1 : x2 mode
            2 : Reserved
            3 : x4 mode
       Others : Reserved
  Bits[3:0]   : data rate
            0 : 1.62 Gbps
            1 : 2.7 Gbps
            2 : 5.4 Gbps
       Others : Reserved
**/
typedef union {
  UINT16 Value;
  struct {
    UINT16 DataRate:4;
    UINT16 LaneCount:4;
    UINT16 PreEmphasisLevel:4;
    UINT16 VSwingLevel:4;
  } Bits;
} EDP_FAST_LINK_TRAINING_PARAMS;

/**
  This defines Full link training parameters for a single eDP panel.
  Bits[7:4] : VSwing level
          0 : 0.4V (default)
          1 : 0.6V
          2 : 0.8V
          3 : 1.2V
     Others : Reserved
  Bits[3:0] : Pre-emphasis level
          0 : no pre-emphasis (default)
          1 : 3.5dB
          2 : 6dB
          3 : 9.5dB
     Others : Reserved
**/
typedef union {
  UINT8   Value;
  struct {
    UINT8   PreEmphasisLevel:4;
    UINT8   VSwingLevel:4;
  } Bits;
} EDP_FULL_LINK_TRAINING_PARAMS;

/**
  This defines the structure of Apical Parameters for a single eDP panel.
**/
typedef struct {
  UINT32      PanelOui;             ///< Apical IP specific field for Panel OUI
  UINT32      DPCDBaseAddress;      ///< Apical IP specific field for DPCD Base address
  UINT32      DPCDIrdidixControl0;  ///< Apical IP specific field for DPCD Idridix Control 0
  UINT32      DPCDOptionSelect;     ///< Apical IP specific field for DPCD option select
  UINT32      DPCDBacklight;        ///< Apical IP specific field for DPCD backlight
  UINT32      AmbientLight;         ///< Apical IP specific field for Ambient light
  UINT32      BacklightScale;       ///< Apical IP specific field for backlight scale
} EDP_APICAL_PARAMS;

/**
  This defines the structure of Block 27 (eDP Display Block)
**/
typedef struct {
  UINT8       BlockId;            ///< Defines the unique Block ID : 27
  UINT16      BlockSize;          ///< Defines the size of eDP display VBT block.

  EDP_PWR_SEQ eDP_PWR_SEQ[16];    ///< Array defining the panel power sequencing for all 16 eDP panels.

  /**
  Defines the panel color depth in bits per pixel. 2 Bits for each Panel.
    Bits[1:0] Panel color depth for Panel #1
      = 00, 18bpp
      = 01, 24bpp
      = 10, 30bpp
      = 11, 36bpp
  **/
  UINT32      eDP_Panel_Color_Depth;

  /**
    Array containing the FLT parameters of 16 eDP panels.
  **/
  EDP_FAST_LINK_TRAINING_PARAMS      eDP_Fast_Link_Training_Params[16];

  /**
  This field defines the eDP sDRRS MSA Timing Delay for all 16 eDP panels. 2 Bits for Each Panel.
  Bits[1:0] for Panel #1
    = 00, Line 1
    = 01, Line 2
    = 10, Line 3
    = 11, Line 4
  **/
  UINT32      eDP_sDRRS_MSA_Delay;

  /**
  Defines the S3D feature enable/disable for all 16 eDP panels. 1 Bit for Each Panel.
  Bits[0] for Panel #1
    = 0, S3D disabled for this panel
    = 1, S3D enabled for this panel
  **/
  UINT16      eDP_S3D_Feature;

  /**
  Defines the T3 optimization enable/disable for all 16 panels. 1 Bit for each panel.
  Bits[0] = Panel #1
    = 0, T3 optimization disabled for this panel
    = 1, T3 optimization enabled for this panel
  **/
  UINT16      eDP_T3_Optmization;

  /**
  Defines the Edp vswing and pre-emphasis for all 16 panels. 4 Bits for Each Panel
  Bits[3:0] = Panel #1
    = 0, Use table 1 for this panel.
    = 1, Use table 2 for this panel.
  **/
  UINT64       VswingPreEmphasisTableNum;

  /**
  Defines the Edp fast link training support on all 16 panels. 1 Bit for Each Panel
  Bits[0] = Panel #1
    = 0, FastLinkTraining feature is disabled for this panel
    = 1, FastLinkTraining feature is enabled for this panel
  **/
  UINT16     EdpFastLinkTrainingSupportOnPanel;

  /**
  Defines whether the Set power state at DPCD 600h is to be done in eDP enable/disable sequence.
  Bits[0] = Panel #1
    = 0, Set power state at DPCD 600h feature is disabled for this panel
    = 1, Set power state at DPCD 600h feature is enabled for this panel
  **/
  UINT16     SetPowerStateAtDPCD600h; //This is not used currently

  /**
    Array defining the PWM <--> Backlight related delays for 16 panels.
  **/
  EDP_PWM_BACKLIGHT_DELAYS eDP_Pwm_BackLight_Delays[16];

  /**
  Defines the Edp full link training support on all 16 panels. 1 Bit for Each Panel.
  \verbatim
  Bits[0] : Panel #1
        0 : Initial vswing and pre-emphasis levels are not provided for Full link training
        1 : Initial vswing and pre-emphasis levels are provided for Full link training
  Bits 1 to 15 are for panel # 2 to 16.
  \endverbatim
  **/
  UINT16     InitialFullLinkTrainingParamsProvidedInVbt;

  /**
    Array containing the initial Vswing and Pre-emphasis parameters for Full link training.
  **/
  EDP_FULL_LINK_TRAINING_PARAMS    eDP_Full_Link_Training_Params[16];

  /**
  Defines the Edp Apical assertive display IP support on all 16 panels. 1 Bit for Each Panel.
  Bit 0   : Panel #1
        0 : Apical assertive display IP is disabled for this panel.
        1 : Apical assertive display IP is enabled for this panel.
  Bits 1 to 15 are for panel # 2 to 16.
  **/
  UINT16                           IsApicalAssertiveDisplayIpEnable;

  /**
    Array containing the Apical parameters for all 16 panels
  **/
  EDP_APICAL_PARAMS                eDP_Apcial_Params[16];
} BLOCK27_EDP_FEATURES;

/**
  This defines the structure of Block 28 (Edidless EFP support DTD timings)
**/
typedef struct {
  UINT8                 BlockId;                    ///< Defines the unique Block ID : 28
  UINT16                BlockSize;                  ///< Defines the size of Edidless EFP support block.
  DTD_STRUCTURE         Edidless_EFP_DTD_Struc[4];  ///< Array defining the DTD timing for 3 EFP devices.
} BLOCK28_EDIDLESS_EFP;

/**
This defines the structure of toggle list entry.
**/
typedef struct {
  /**
  Defines the display device selection for toggling
  Bit 15 = EFP4.3 (Reserved for CML)
  Bit 14 = EFP3.3
  Bit 13 = EFP2.3
  Bit 12 = EFP1.3
  Bit 11 = EFP4.2 (Reserved for CML)
  Bit 10 = EFP3.2
  Bit 9  = EFP2.2
  Bit 8  = EFP1.2
  Bit 7  = LFP2
  Bit 6  = EFP2
  Bit 5  = EFP3
  Bit 4  = EFP4
  Bit 3  = LFP
  Bit 2  = EFP
  Bit 1  = TV
  Bit 0  = CRT
  **/
  UINT16  DisplayDevice;
} CNL_TOGGLE_LIST_ENTRY;

/**
  This defines the structure of Block 31 (Toggle Lists for Cannonlake)
**/
typedef struct {
  UINT8                   BlockId;              ///< Defines the unique Block ID : 31
  UINT16                  BlockSize;            ///< Defines the size of Toggle List Block.
  UINT16                  NumOfEntry1;          ///< Defines the number of entries in toggle list 1.
  UINT8                   EntrySize1;           ///< Defines the size of toggle list entry present in list 1.
  CNL_TOGGLE_LIST_ENTRY   ToggleList1Entry[16]; ///< Array defining the toggle list 1.
  UINT16                  NumOfEntry2;          ///< Defines the number of entries in toggle list 2.
  UINT8                   EntrySize2;           ///< Defines the size of toggle list entry present in list 2.
  CNL_TOGGLE_LIST_ENTRY   ToggleList2Entry[8];  ///< Array defining the toggle list 2.
  UINT16                  NumOfEntry3;          ///< Defines the number of entries in toggle list 3.
  UINT8                   EntrySize3;           ///< Defines the size of toggle list entry present in list 3.
  CNL_TOGGLE_LIST_ENTRY   ToggleList3Entry[8];  ///< Array defining the toggle list 3.
  UINT16                  NumOfEntry4;          ///< Defines the number of entries in toggle list 4.
  UINT8                   EntrySize4;           ///< Defines the size of toggle list entry present in list 4.
  CNL_TOGGLE_LIST_ENTRY   ToggleList4Entry[8];  ///< Array defining the toggle list 4.
} BLOCK31_TOGGLE_LIST;

/**
  This defines the structure of Display device removal configuration entry.
**/
typedef struct {
  /**
  Defines the display device configuration to be removed.
  Bit 15 = EFP4.3 (Reserved for CML)
  Bit 14 = EFP3.3
  Bit 13 = EFP2.3
  Bit 12 = EFP1.3
  Bit 11 = EFP4.2 (Reserved for CML)
  Bit 10 = EFP3.2
  Bit 9  = EFP2.2
  Bit 8  = EFP1.2
  Bit 7  = LFP2
  Bit 6  = EFP2
  Bit 5  = EFP3
  Bit 4  = EFP4
  Bit 3  = LFP
  Bit 2  = EFP
  Bit 1  = TV
  Bit 0  = CRT
  **/
  UINT16  DisplayDeviceConfiguration;
} CNL_DISPLAY_CONFIGURATION_ENTRY;

/**
  This defines the structure of Block 32 (Display removal configuration Block)
**/
typedef struct {
  UINT8                                BlockId;       ///< Defines the unique Block ID = 32
  UINT16                               BlockSize;     ///< Defines the size of Display removal configuration block.
  UINT8                                NumOfEntry;    ///< Defines the number of entries in display removal configuraion table.
  UINT8                                EntrySize;     ///< Defines the size of 1 entry in display removal configuration table.
  CNL_DISPLAY_CONFIGURATION_ENTRY      RemoveDisplayConfiguration[15];    ///< Array defining the display removal configuration table.
}BLOCK32_DISPLAY_CONFIGURATION_REMOVAL;

/**
  This defines the Local Flat panel basic details such as resolution and the various registers.
**/
typedef struct {
  UINT16  XRes;                   ///< X resolution of the panel.
  UINT16  YRes;                   ///< Y resolution of the panel.
  UINT32  LVDSDigDisReg;          ///< MMIO offset of LFP digital display port register.
  UINT32  LVDSDigDisVal;          ///< Value of LFP digital display port register.
  UINT32  OnSeqDelayReg;          ///< MMIO offset of Panel power on sequencing delay register.
  UINT32  OnSeqDelayVal;          ///< Value of Panel power on sequencing delay register.
  UINT32  OffSeqDelayReg;         ///< MMIO offset of Panel power off sequencing delay register.
  UINT32  OffSeqDelayVal;         ///< Value of Panel power off sequencing delay register.
  UINT32  CycleDelay_RefDivReg;   ///< MMIO offset of Panel power cycle delay and reference divider register.
  UINT32  CycleDelay_RefDivVal;   ///< Value of Panel power cycle delay and reference divider register.
  UINT16  Terminate;              ///< Special value 0xFFFF indicating end of data.
} FP_DATA;

/**
  This defines the structure consisting of all details for a single Local Flat panel.
**/
typedef struct {
  FP_DATA       FP_Data;      ///< Instance of ::FP_DATA structure.
  DTD_STRUCTURE DTD_Data;     ///< Instance of ::DTD_STRUCTURE which contains the DTD timings for the panel.
  PID_DATA      PID_Data;     ///< Instance of ::PID_DATA structure which contains panel related information used by driver.
} LVDS_FP_TABLE;

/**
  This structure defines all the details regarding Backlight control for LFP.
**/
typedef struct {
  /**
  Defines the backlight features for the panel.
  Bits 7:6  = GMBus Speed:
            = 00, 100 KHz
            = 01, 50 KHz
            = 10, 400 KHz
            = 11, 1 MHz
  Bits 5:3  = Inverter GPIO Pins
            = 0, None
            = 1, I2C GPIO pins
            = 2, Analog CRT DDC pins
            = 3, DVI/LVDS DDC GPIO pins
            = 5, sDVO I2C GPIO pins
  Bit 2     = Inverter Polarity (i2c & PWM)
            = 0, Normal (0 = Minimum brightness)
            = 1, Inverted (0 = Maximum brightness)
  Bits 1:0  = BLC Inverter Type
            = 00, None/External
            = 01, i2c
            = 10, PWM
            = 11, Reserved
  **/
  UINT8   BLC_Ftr;

  UINT16  PWM_Freq;       ///< PWM inverter frequency in KHz
  UINT8   Min_Brightness; ///< Minimum brightness in the range 0-255
  UINT8   I2C_Add;        ///< I2C Inverter Slave Address
  UINT8   I2C_Command;    ///< I2C Inverter command code
} BLC;

/**
  This defines the structure of Block 40 (LFP Features)
**/
typedef struct {
  UINT8   BlockId;          ///< Defines the unique Block ID : 40
  UINT16  BlockSize;        ///< Defines the size of LFP Features block.

  UINT8   bmp_Panel_type;   ///< Defines the panel type of LFP.
  UINT8   Skip1;            ///< Obsoleted.

  /**
  Capabilities byte:
  Bit 15:7  = SW Workaround bits
  Bit 6     = Panel EDID support
            = 0, Disable
            = 1, Enable
  Bit 5     = Pixel dithering
            = 0, Disable
            = 1, Enable
  Bit 4     = Panel Fitting ratio calc.
            = 0 - Manual
            = 1 - Automatic
  Bit 3     = Panel Fitting Graphics mode
            = 0, Bilinear
            = 1, Enhanced
  Bit 2     = Panel Fitting Text mode
            = 0, Bilinear
            = 1, Enhanced
  Bit 1:0   = Panel Fitting Support
            = 00, No panel fitting
            = 01, Text panel fitting
            = 10, GFX panel fitting
            = 11, Text+GFX panel fitting
  **/
  UINT16  bmp_LVDS_Capabilities;

  /**
  Defines the channel type of LFP. 2 Bits for each Panel.
  Bits [0:1] for Panel #1
    = 00, Automatic (algorithm)
    = 01, Single Channel
    = 10, Dual Channel
    = 11, Reserved
  **/
  UINT32  INT_LVDS_Panel_Channel_Bits;

  UINT16  Enable_SSC_Bit;         ///< LVDS Spread Spectrum Clock
  UINT16  SSC_Freq_Bit;           ///< LVDS Spread Spectrum Clock Frequency
  UINT16  Disable_SSC_DDT_Bit;    ///< Disable SSC in Dual Display Twin

  /**
  Defines the panel color depth. 1 Bits for each Panel.
  Bits[0] for Panel #01
    = 0, 18bpp
    = 1, 24bpp
  **/
  UINT16  INT_Panel_Color_Depth;

  /**
  Defines the Panel type. 2 Bits for each Panel.
  Bits [0:1] for Panel #1
    = 00, Static DRRS
    = 01, D2PO
    = 10, Seamless
    = 11, Reserved
  **/
  UINT32  DPS_Panel_Type_Bits;

  /**
  Defines the type of backlight control for the LFP. 2 bits for each Panel.
  Bits [0:1] for Panel #1
    = 00, Default
    = 01, CCFL backlight
    = 10, LED backlight
    = 11, Reserved
  **/
  UINT32  BLT_Control_Type_Bits;
  /**
  Defines the LFP power enable flag in S0 state for all 16 panels. 1 Bit for Each Panel.
  Bits[0] : Panel #1
        0 : Do not keep LCDVCC on during S0 state.
        1 : Keep LCDVCC on during S0 state.
  Bits 1 to 15 are for panel # 2 to 16.
  **/
  UINT16     LcdvccOnDuringS0State;
} BLOCK40_LVDS_FEATURES;

/**
  This structure defines the second type of BMP table pointers.
  This is used to store pointers to LFP Flat panel data, DTD and PID information.
**/
typedef struct {
  UINT16  Offset;       ///< Offset of the table.
  UINT8   Size;         ///< Size of the table.
} BMP_TABLE_TYPE2_PTR;

/**
  This structure defines a set of 3 pointers for LFP display.
  These pointers point to FP data, DTD and PID information respectively.
**/
typedef struct {
  BMP_TABLE_TYPE2_PTR   FpTablePtr;   ///< Pointer to FP Data of the LFP panel.
  BMP_TABLE_TYPE2_PTR   DtdTablePtr;  ///< Pointer to DTD of the LFP panel.
  BMP_TABLE_TYPE2_PTR   PidTablePtr;  ///< Pointer to the PID data of the LFP panel.
} LFP_TABLE_POINTERS;

/**
  This defines the structure of Block 41 (LFP Table Pointers for FPDATA, DTD and PID)
**/
typedef struct {
  UINT8               BlockId;                  ///< Defines the unique Block ID:41
  UINT16              BlockSize;                ///< Defines the size of LFP Table Pointer Block.
  UINT8               NumOfEntries;             ///< Defines the number of entries in the Table.
  LFP_TABLE_POINTERS  LfpTablePointers[16];     ///< Array of ::LFP_TABLE_POINTERS for all 16 panels.
  UINT16              LfpPanelNameTableOffset;  ///< Offset of LFP panel names table.
  UINT8               LfpPanelNameLength;       ///< Length of a single LFP panel's name.
} BLOCK41_LFP_TABLE_POINTERS;

/**
  This defines the structure of Block 42 (Complete LFP Panel Information)
**/
typedef struct {
  UINT8         BlockId;                ///< Defines the unique block ID : 42
  UINT16        BlockSize;              ///< Defines the size of Complete LFP panel information for all 16 panels.
  LVDS_FP_TABLE LVDS_FP_Table[16];      ///< Array of ::LVDS_FP_TABLE containing data of 16 panels.
  UINT8         LFP_PANEL_NAMES[16][13];///< Array defining the panel names for all 16 panels.

  /**
  1 Bit for Each Panel
  Bit0  = Scaling feature for panel 1.
        = 0, Scaling feature is disabled for this panel.
        = 1, Scaling feature is enabled for this panel.
  **/
  UINT16        EnableScaling; //This is not used currently

  /**
    Array defining DRRS minimum refresh rate. 1 Byte for Each Panel.
  **/
  UINT8         Seamless_DRRS_Min_RR[16];

  /**
    Array defining Pixel Overlap Count. 1 Byte for Each Panel.
  **/
  UINT8         PixelOverlapCount[16];
} BLOCK42_LVDS_PANEL_INFO;

typedef union {
  /**
  Backlight control parameters.\n
  Bits 7:4  : PWM Controller Selection
          0 : Controller 0
          1 : Controller 1
          2 : Controller 2
          3 : Controller 3
     Others : Reserved.
  Bits 3:0  : PWM Source Selection
          0 : PMIC PWM
          1 : LPSS PWM
          2 : DISPLAY PWM
          3 : CABC PWM
     Others : Reserved.
  **/
  UINT8 Value;
  struct {
    UINT8 PwmSourceSelection:4;
    UINT8 PwmControllerSelection:4;
  } Bits;
} BKLT_CTRL_PARAMS;

/**
  This defines the structure of Block 43 (LFP Brightness Control)
**/
typedef struct {
  UINT8             BlockId;                ///< Defines the unique Block ID : 43
  UINT16            BlockSize;              ///< Defines the size of Brightness control block.

  UINT8             SIZE_BLCStruc;          ///< Defines the size of single entry in Backlight control table for LFP.
  BLC               BLC_Struct[16];         ///< Array defining the backlight control for 16 LFP panels.
  UINT8             Post_Brightness[16];    ///< Array defining the initial brightness for all 16 panels.
  BKLT_CTRL_PARAMS  Brightness_Control[16]; ///< Array defining the brightness control method for all 16 panels
} BLOCK43_LVDS_BLC;

/**
  This defines the structure of Block 44 (LFP Power Conservation Features)
**/
typedef struct {
  UINT8   BlockId;        ///< Defines the unique block ID : 44
  UINT16  BlockSize;      ///< Defines the size of LFP Power Conservation Features block.
  union {
  /**
  Bit[7]        : ALS Enable/Disable
               0 - Disable
               1 - Enable
  Bit[6]        : Display LACE support
               0 - Not supported
               1 - Supported
  Bit[5]        : Default Display LACE enabled status
               0 - Disabled
               1 - Enabled
  Bit[4]        : Reserved
  Bit[3:1]      : Power conservation preference level.
                 4 is default in a range of 1 to 6.
  Bit[0]        : Reserved
  **/
    UINT8  Value;
    struct {
      UINT8 Reserved:1;
      UINT8 PwrConservation:3;
      UINT8 Reserved_1:1;
      UINT8 DefalutDisplayLaceEnable:1;
      UINT8 DisplayLaceSupport:1;
      UINT8 AlsEnable:1;
    } Bits;
  } LfpFeatureBits;

  UINT16  AlsData[10];    ///< Defines the main ALS data.

  union {
  /**
  Bit[7:3]      : Reserved
  Bit[2:0]      : Aggressiveness Level Profile.
            000 - Minimum
            001 - Moderate
            010 - High
  **/
    UINT8  Value;
    struct {
      UINT8 AggressionProfileLevel:3;
      UINT8 Reserved:5;
    } Bits;
  } LaceAggressivenessProfile; ///< Defines the LACE Aggressiveness Profile
} BLOCK44_ALS;

/**
  This defines the structure of Black Frame Insertion table entry.
**/
typedef struct {
  /**
  BFI Features\n
  Bit[7-2]  : Reserved\n
  Bit[1]    : Enable Brightness control in CUI\n
  Bit[0]    : Enable BFI in driver
  **/
  UINT8         EnableBits;
  UINT8         BrightnessNonBFI;   ///< Brightness percentage in non BFI mode
} BFI;

/**
  This defines the structure of Block 45 (Black Frame insertion Support for LFP)
**/
typedef struct {
  UINT8              BlockId;         ///< Defines the unique Block ID : 45
  UINT16             BlockSize;       ///< Defines the size of Black frame insertion support block.
  UINT8              SIZE_BFIStruc;   ///< Defines the size of 1 entry of black frame data.
  BFI                BFI_Struct[16];  ///< Array defining the data of black frame insertion for all 16 panels.
} BLOCK45_BFI_SUPPORT;

/**
  This structure defines the chromaticity information for a single LFP panel.
**/
typedef struct {
  /**
  Defines the chromaticity feature enable bits
  Bits 7:2  = Reserved
  Bit 1     = Override EDID values for chromaticity if enabled, Instead Use VBT values
            = 0, Disable, Use the EDID values
            = 1, Enable, Use the values from the VBT
  Bit 0     = Enable chromaticity feature. EDID values will be used when this feature is enabled.
            = 0, Disable
            = 1, Enable
  **/
  UINT8        EnableBits;

  UINT8        Red_Green_1;   ///< Red/green chormaticity coordinates at EDID offset 19h
  UINT8        Blue_White_1;  ///< Blue/white chromatiity coordinates at EDID offset 1Ah
  UINT8        Red_X1;        ///< Red x coordinate at EDID offset 1Bh
  UINT8        Red_Y1;        ///< Red x coordinate at EDID offset 1Ch
  UINT8        Green_X1;      ///< Green x coordinate at EDID offset 1Dh
  UINT8        Green_Y1;      ///< Green x coordinate at EDID offset 1Eh
  UINT8        Blue_X1;       ///< Blue x coordinate at EDID offset 1Fh
  UINT8        Blue_Y1;       ///< Blue x coordinate at EDID offset 20h
  UINT8        White_X1;      ///< White x coordinate at EDID offset 21h
  UINT8        White_Y1;      ///< White x coordinate at EDID offset 22h
} CHROMATICITY;

/**
  This structure defines the Luminance information for a single LFP panel.
**/
typedef struct {
  /**
  Defines the chromaticity feature enable bits
  Bits 7:2  : Reserved
  Bit 1     : Enable Gamma feature.
            : if enabled, use gamma values from this block.
          0 : Disable
          1 : Enable
  Bit 0     : Enable Luminance feature.
            : if enabled, use values from this block.
          0 : Disable
          1 : Enable
  **/
  UINT8        EnableBits;
  /**
    Luminance info (refer DisplayID 2.0)
    2 byte value, encoded in IEEE 754 half-precision binary floating point format
  **/
  UINT16      MinLuminance;           ///< Native minimum luminance
  UINT16      MaxFullFrameLuminance;  ///< Native maximum luminance (Full Frame)
  UINT16      MaxLuminance;           ///< Native Maximum Luminance (1% Rectangular Coverage)
  /**
    Gamma EOTF
    Gamma values range from 00h through FFh which will come from VBT.
    Value shall define the gamma range, from 1.00 to 3.54.
    Field Value = (Gamma (value from VBT) + 100) / 100

    FFh = No gamma information shall be provided
  **/
  UINT8 Gamma;

}LUMINANCE_AND_GAMMA;

/**
  This defines the structure of Block 46 (Chromaticity Support)
**/
typedef struct {
  UINT8              BlockId;                 ///< Defines the unique Block ID : 46
  UINT16             BlockSize;               ///< Defines the size of Chromaticity Block.
  CHROMATICITY       Chromaticity_Struct[16]; ///< Defines the chromaticity information for all 16 panels.
  LUMINANCE_AND_GAMMA  Luminance_Gamma_Struct[16];    ///< Defines the lumianance information for all 16 panels.
} BLOCK46_CHROMATICITY_SUPPORT;

/**
  This defines the structure of Block 51 (Fixed Mode Set)
**/
typedef struct{
  UINT8       BlockId;        ///< Defines the unique block ID : 51.
  UINT16      BlockSize;      ///< Defines the size of Fixed mode set feature block.
  UINT8       FeatureEnable;  ///< Whether the fixed mode set feature is enabled/disabled.
  UINT32      XRes;           ///< X resolution of the fixed mode.
  UINT32      YRes;           ///< Y resolution of the fixed mode.
} BLOCK51_FIXED_MODE_SET;

/**
  This defines the Complete VBT Structure for generation purpose
**/
typedef struct {
  VBT_HEADER                                VbtHeader;
  VBT_BIOS_DATA_HEADER                      VbtBdbHeader;
  BLOCK254_BMP_Structure                    Block254BMPStructure;
  VBT_GENERAL1_INFO                         VbtGen1Info;
  PRD_BOOT_TABLE                            PrdBootTable;
  VBT_GENERAL2_INFO                         VbtGen2Info;
  BLOCK03_ORIGINAL_DISPLAY_TOGGLE_LIST      Block03OriginalDisplayToggleList;
  BLOCK252_SBIOS_Hook                       Block252SbiosHook;
  BLOCK06_MMIO_REG_TABLE                    Block06MmioRegTable;
  BLOCK07_IOSWFLAG_REG_TABLE                Block07IoswflagRegTable;
  BLOCK08_MMIOSWFLAG_REG_TABLE              Block08MmioswflagRegTable;
  BLOCK09_PSR_FEATURE                       Block09PsrFeature;
  BLOCK10_MODE_REMOVAL_TABLE                Block10ModeRemovalTable;
  BLOCK12_DRIVER_FEATURES                   Block12DriverFeatures;
  BLOCK13_DRIVER_PERSISTENCE                Block13DriverPersistence;
  BLOCK17_SV_BITS                           Block17SvBits;
  BLOCK18_DRIVER_ROTATION                   Block18DriverRotation;
  BLOCK20_OEM_CUSTOMIZATION                 Block20OemCustomization;
  BLOCK26_TV_OPTIONS                        Block26TVOptions;
  BLOCK27_EDP_FEATURES                      Block27EDPFeatures;
  BLOCK28_EDIDLESS_EFP                      Block28EdidlessEFP;
  BLOCK31_TOGGLE_LIST                       Block31ToggleList;
  BLOCK32_DISPLAY_CONFIGURATION_REMOVAL     Block32DisplayConfigurationRemoval;
  BLOCK40_LVDS_FEATURES                     Block40LVDSFeatures;
  BLOCK41_LFP_TABLE_POINTERS                Block41LfpTablePointers;
  BLOCK42_LVDS_PANEL_INFO                   Block42LvdsPanelInfo;
  BLOCK43_LVDS_BLC                          Block43LVDSBlc;
  BLOCK44_ALS                               Block44Als;
  BLOCK46_CHROMATICITY_SUPPORT              Block46ChromaticitySupport;
  BLOCK51_FIXED_MODE_SET                    Block51FixedModeSet;
} VBT_TABLE_DATA;

#pragma pack()

/**
  This function will update the VBT checksum.

  @param[in out] VbtPtr - Pointer to VBT table

  @retval none
**/
VOID
UpdateVbtChecksum(
  VBT_TABLE_DATA *VbtPtr
);

/**
  This function will update the VBT.

  @param[in] VbtPtr - Pointer to VBT Table

  @retval none
**/
VOID
UpdateGopVbt (
  IN  VBT_TABLE_DATA    *VbtPtr
);
#endif

