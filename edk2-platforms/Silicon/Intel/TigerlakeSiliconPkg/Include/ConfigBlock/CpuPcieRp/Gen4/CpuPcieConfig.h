/** @file
  Pcie root port policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_PCIE_CONFIG_H_
#define _CPU_PCIE_CONFIG_H_

#include <Library/GpioLib.h>
#include <Library/CpuPcieInfoFruLib.h>
#include <PcieConfig.h>
#include <ConfigBlock.h>
#include <Register/SaRegsHostBridge.h>

#pragma pack(push, 1)

#define CPU_PCIE_PEI_PREMEM_CONFIG_REVISION 1
#define CPU_PCIE_RP_PREMEM_CONFIG_REVISION  4

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Add Gen3TxOverride and Gen4TxOverride
  <b>Revision 3</b>:  - Deprecate Dekel Suqelch Workaround Setup Variable
  <b>Revision 4</b>:  - Add FOMS Control Policy Setup Variable
  <b>Revision 5</b>:  - Add Gen3HwEqOverride and Gen4HwEqOverride
  <b>Revision 6</b>:  - Align revision with CPU_PCIE_RP_CONFIG_REVISION value
**/

#define CPU_PCIE_RP_CONFIG_REVISION         6

#define L0_SET                            BIT0
#define L1_SET                            BIT1




/**
 PCI Express and DMI controller configuration\n
 @note <b>Optional.</b> These policies will be ignored if there is no PEG port present on board.
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER    Header;                                      ///< Offset 0-27 Config Block Header
  /**
   Offset 28:0 :
   <b>(Test)</b> DMI Link Speed Control
  - <b>Auto</b> (0x0)  : Maximum possible link speed (Default)
  - Gen1        (0x1)  : Limit Link to Gen1 Speed
  - Gen2        (0x2)  : Limit Link to Gen2 Speed
  - Gen3        (0x3)  : Limit Link to Gen3 Speed
  **/
  UINT32                 DmiMaxLinkSpeed                 :  2;
  /**
   Offset 28:2 :
   <b>(Test)</b> DMI Equalization Phase 2 Enable Control
  - Disabled       (0x0) : Disable phase 2
  - Enabled        (0x1) : Enable phase 2
  - <b>Auto</b>    (0x2) : Use the current default method (Default)
  **/
  UINT32                 DmiGen3EqPh2Enable              :  2;
  /**
   Offset 28:4 :
   <b>(Test)</b> Selects the method for performing Phase3 of Gen3 Equalization on DMI
  - <b>Auto</b> (0x0)  : Use the current default method (Default)
  - HwEq        (0x1)  : Use Adaptive Hardware Equalization
  - SwEq        (0x2)  : Use Adaptive Software Equalization (Implemented in BIOS Reference Code)
  - Static      (0x3)  : Use the Static EQs provided in DmiGen3EndPointPreset array for Phase1 AND Phase3 (Instead of just Phase1)
  - Disabled    (0x4)  : Bypass Equalization Phase 3
  **/
  UINT32                 DmiGen3EqPh3Method              :  3;
  /**
   Offset 28:7 :
   <b>(Test)</b> Program DMI Gen3 EQ Phase1 Static Presets
  - Disabled        (0x0)  : Disable EQ Phase1 Static Presets Programming
  - <b>Enabled</b>  (0x1)  : Enable  EQ Phase1 Static Presets Programming (Default)
  **/
  UINT32                 DmiGen3ProgramStaticEq          :  1;
  UINT32                 RsvdBits0                       :  24;        ///< Offset 28:8 :Reserved for future use

  /**
  Offset 32:0 :
  Select when PCIe ASPM programming will happen in relation to the Oprom
  - <b>Before</b> (0x0) : Do PCIe ASPM programming before Oprom. (Default)
  - After         (0x1) : Do PCIe ASPM programming after Oprom. This will require an SMI handler to save/restore ASPM settings.
  **/
  UINT32                 InitPcieAspmAfterOprom : 1;
  UINT32                 RsvdBits1 : 31;        ///< Offset 32:1 :Reserved for future use

  UINT8                  DmiGen3RootPortPreset[SA_DMI_MAX_LANE];      ///< Offset 36 Used for programming DMI Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen3EndPointPreset[SA_DMI_MAX_LANE];      ///< Offset 40/44 Used for programming DMI Gen3 preset values per lane. Range: 0-9, 7 is default for each lane
  UINT8                  DmiGen3EndPointHint[SA_DMI_MAX_LANE];        ///< Offset 44/52 Hint value per lane for the DMI Gen3 End Point. Range: 0-6, 2 is default for each lane
  /**
   Offset 48/60 :
   DMI Gen3 RxCTLEp per-Bundle control. The range of the setting is (0-15). This setting
   has to be specified based upon platform design and must follow the guideline. Default is 12.
  **/

  UINT8                  DmiGen3RxCtlePeaking[SA_DMI_MAX_BUNDLE];

  UINT8                  DmiDeEmphasis;                               ///< Offset 64 This field is used to describe the DeEmphasis control for DMI (-6 dB and -3.5 dB are the options)
  UINT8                  Rsvd0[3];                                    ///< Offset 65
} PCIE_PEI_PREMEM_CONFIG;


/**
  CPU PCIe Root Port Pre-Memory Configuration
  Contains Root Port settings and capabilities
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Adding Dekel Suqelch Workaround Setup Variable
  <b>Revision 3</b>:  - Deprecate Dekel Suqelch Workaround Setup Variable
  <b>Revision 4</b>:  - Adding CDR Relock Setup Variable
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                                ///< Config Block Header
  /**
  Root Port enabling mask.
  Bit0 presents RP1, Bit1 presents RP2, and so on.
  0: Disable; <b>1: Enable</b>.
  **/
  UINT32                RpEnabledMask;
  /**
  Assertion on Link Down GPIOs
  - <b>Disabled</b> (0x0) : Disable assertion on Link Down GPIOs(Default)
  - Enabled         (0x1) : Enable assertion on Link Down GPIOs
  **/
  UINT8                 LinkDownGpios;
  /**
  Enable ClockReq Messaging
  - <b>Disabled</> (0x0) : Disable ClockReq Messaging(Default)
  - Enabled        (0x1) : Enable ClockReq Messaging
  **/
  UINT8                 ClkReqMsgEnable;
  /**
  Dekel Recipe Workaround
  <b>2</b>
  1=Minimal, 9=Maximum,
  **/
  UINT8                 DekelSquelchWa;  // Deprecated variable
  UINT8                 Rsvd0[1];
  /**
  Determines each PCIE Port speed capability.
  <b>0: Auto</b>; 1: Gen1; 2: Gen2; 3: Gen3; 4: Gen4 (see: CPU_PCIE_SPEED)
  **/
  UINT8                 PcieSpeed[CPU_PCIE_MAX_ROOT_PORTS];
  /**
  To Enable/Disable CDR Relock
  <b>0: Disable</b>; 1: Enable
  **/
  UINT8                 CdrRelock[CPU_PCIE_MAX_ROOT_PORTS];
  /**
  This policy is used while programming DEKEL Recipe
  <b>0: Disable</b>; 1: Enable
  **/
  UINT8                 Xl1el[CPU_PCIE_MAX_ROOT_PORTS];

} CPU_PCIE_RP_PREMEM_CONFIG;

typedef enum {
  CpuPcieOverrideDisabled             = 0,
  CpuPcieL1L2Override                 = 0x01,
  CpuPcieL1SubstatesOverride          = 0x02,
  CpuPcieL1L2AndL1SubstatesOverride   = 0x03,
  CpuPcieLtrOverride                  = 0x04
} CPU_PCIE_OVERRIDE_CONFIG;

/**
  PCIe device table entry entry

  The PCIe device table is being used to override PCIe device ASPM settings.
  To take effect table consisting of such entries must be instelled as PPI
  on gPchPcieDeviceTablePpiGuid.
  Last entry VendorId must be 0.
**/
typedef struct {
  UINT16  VendorId;                    ///< The vendor Id of Pci Express card ASPM setting override, 0xFFFF means any Vendor ID
  UINT16  DeviceId;                    ///< The Device Id of Pci Express card ASPM setting override, 0xFFFF means any Device ID
  UINT8   RevId;                       ///< The Rev Id of Pci Express card ASPM setting override, 0xFF means all steppings
  UINT8   BaseClassCode;               ///< The Base Class Code of Pci Express card ASPM setting override, 0xFF means all base class
  UINT8   SubClassCode;                ///< The Sub Class Code of Pci Express card ASPM setting override, 0xFF means all sub class
  UINT8   EndPointAspm;                ///< Override device ASPM (see: CPU_PCIE_ASPM_CONTROL)
                                       ///< Bit 1 must be set in OverrideConfig for this field to take effect
  UINT16  OverrideConfig;              ///< The override config bitmap (see: CPU_PCIE_OVERRIDE_CONFIG).
  /**
    The L1Substates Capability Offset Override. (applicable if bit 2 is set in OverrideConfig)
    This field can be zero if only the L1 Substate value is going to be override.
  **/
  UINT16  L1SubstatesCapOffset;
  /**
    L1 Substate Capability Mask. (applicable if bit 2 is set in OverrideConfig)
    Set to zero then the L1 Substate Capability [3:0] is ignored, and only L1s values are override.
    Only bit [3:0] are applicable. Other bits are ignored.
  **/
  UINT8   L1SubstatesCapMask;
  /**
    L1 Substate Port Common Mode Restore Time Override. (applicable if bit 2 is set in OverrideConfig)
    L1sCommonModeRestoreTime and L1sTpowerOnScale can have a valid value of 0, but not the L1sTpowerOnValue.
    If L1sTpowerOnValue is zero, all L1sCommonModeRestoreTime, L1sTpowerOnScale, and L1sTpowerOnValue are ignored,
    and only L1SubstatesCapOffset is override.
  **/
  UINT8   L1sCommonModeRestoreTime;
  /**
    L1 Substate Port Tpower_on Scale Override. (applicable if bit 2 is set in OverrideConfig)
    L1sCommonModeRestoreTime and L1sTpowerOnScale can have a valid value of 0, but not the L1sTpowerOnValue.
    If L1sTpowerOnValue is zero, all L1sCommonModeRestoreTime, L1sTpowerOnScale, and L1sTpowerOnValue are ignored,
    and only L1SubstatesCapOffset is override.
  **/
  UINT8   L1sTpowerOnScale;
  /**
    L1 Substate Port Tpower_on Value Override. (applicable if bit 2 is set in OverrideConfig)
    L1sCommonModeRestoreTime and L1sTpowerOnScale can have a valid value of 0, but not the L1sTpowerOnValue.
    If L1sTpowerOnValue is zero, all L1sCommonModeRestoreTime, L1sTpowerOnScale, and L1sTpowerOnValue are ignored,
    and only L1SubstatesCapOffset is override.
  **/
  UINT8   L1sTpowerOnValue;

  /**
    SnoopLatency bit definition
    Note: All Reserved bits must be set to 0

    BIT[15]     - When set to 1b, indicates that the values in bits 9:0 are valid
                  When clear values in bits 9:0 will be ignored
    BITS[14:13] - Reserved
    BITS[12:10] - Value in bits 9:0 will be multiplied with the scale in these bits
                  000b - 1 ns
                  001b - 32 ns
                  010b - 1024 ns
                  011b - 32,768 ns
                  100b - 1,048,576 ns
                  101b - 33,554,432 ns
                  110b - Reserved
                  111b - Reserved
    BITS[9:0]   - Snoop Latency Value. The value in these bits will be multiplied with
                  the scale in bits 12:10

    This field takes effect only if bit 3 is set in OverrideConfig.
  **/
  UINT16  SnoopLatency;
  /**
    NonSnoopLatency bit definition
    Note: All Reserved bits must be set to 0

    BIT[15]     - When set to 1b, indicates that the values in bits 9:0 are valid
                  When clear values in bits 9:0 will be ignored
    BITS[14:13] - Reserved
    BITS[12:10] - Value in bits 9:0 will be multiplied with the scale in these bits
                  000b - 1 ns
                  001b - 32 ns
                  010b - 1024 ns
                  011b - 32,768 ns
                  100b - 1,048,576 ns
                  101b - 33,554,432 ns
                  110b - Reserved
                  111b - Reserved
    BITS[9:0]   - Non Snoop Latency Value. The value in these bits will be multiplied with
                  the scale in bits 12:10

    This field takes effect only if bit 3 is set in OverrideConfig.
  **/
  UINT16  NonSnoopLatency;

  /**
    Forces LTR override to be permanent
    The default way LTR override works is:
      rootport uses LTR override values provided by BIOS until connected device sends an LTR message, then it will use values from the message
    This settings allows force override of LTR mechanism. If it's enabled, then:
      rootport will use LTR override values provided by BIOS forever; LTR messages sent from connected device will be ignored
  **/
  UINT8  ForceLtrOverride;
  UINT8  Reserved[3];
} CPU_PCIE_DEVICE_OVERRIDE;

enum CPU_PCIE_SPEED {
  CpuPcieAuto,
  CpuPcieGen1,
  CpuPcieGen2,
  CpuPcieGen3,
  CpuPcieGen4,
  CpuPcieGen5
};

///
/// The values before AutoConfig match the setting of PCI Express Base Specification 1.1, please be careful for adding new feature
///
typedef enum {
  CpuPcieAspmDisabled,
  CpuPcieAspmL0s,
  CpuPcieAspmL1,
  CpuPcieAspmL0sL1,
  CpuPcieAspmAutoConfig,
  CpuPcieAspmMax
} CPU_PCIE_ASPM_CONTROL;

/**
  Refer to SA EDS for the SA implementation values corresponding
  to below PCI-E spec defined ranges
**/
typedef enum {
  CpuPcieL1SubstatesDisabled,
  CpuPcieL1SubstatesL1_1,
  CpuPcieL1SubstatesL1_1_2,
  CpuPcieL1SubstatesMax
} CPU_PCIE_L1SUBSTATES_CONTROL;

enum CPU_PCIE_MAX_PAYLOAD {
  CpuPcieMaxPayload128 = 0,
  CpuPcieMaxPayload256,
  CpuPcieMaxPayload512,
  CpuPcieMaxPayloadMax
};

enum CPU_PCIE_COMPLETION_TIMEOUT {
  CpuPcieCompletionTO_Default,
  CpuPcieCompletionTO_50_100us,
  CpuPcieCompletionTO_1_10ms,
  CpuPcieCompletionTO_16_55ms,
  CpuPcieCompletionTO_65_210ms,
  CpuPcieCompletionTO_260_900ms,
  CpuPcieCompletionTO_1_3P5s,
  CpuPcieCompletionTO_4_13s,
  CpuPcieCompletionTO_17_64s,
  CpuPcieCompletionTO_Disabled
};


enum CPU_PCIE_GEN3_PRESET_COEFF_SELECTION {
  CpuPcieGen3PresetSelection,
  CpuPcieGen3CoefficientSelection
};

enum CPU_PCIE_GEN4_PRESET_COEFF_SELECTION {
  CpuPcieGen4PresetSelection,
  CpuPcieGen4CoefficientSelection
};

typedef enum {
  CpuPcieEqDefault      = 0,  ///< @deprecated since revision 3. Behaves as PchPcieEqHardware.
  CpuPcieEqHardware     = 1,  ///< Hardware equalization
  CpuPcieEqStaticCoeff  = 4   ///< Fixed equalization (requires Coefficient settings per lane)
} CPU_PCIE_EQ_METHOD;


/**
  Represent lane specific PCIe Gen3 equalization parameters.
**/
typedef struct {
  UINT8                  Cm;                 ///< Coefficient C-1
  UINT8                  Cp;                 ///< Coefficient C+1
  UINT8                  PegGen3RootPortPreset;      ///< <b>(Test)</b> Used for programming PEG Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  PegGen3EndPointPreset;      ///< <b>(Test)</b> Used for programming PEG Gen3 preset values per lane. Range: 0-9, 7 is default for each lane
  UINT8                  PegGen3EndPointHint;        ///< <b>(Test)</b> Hint value per lane for the PEG Gen3 End Point. Range: 0-6, 2 is default for each lane
  UINT8                  PegGen4RootPortPreset;      ///< <b>(Test)</b> Used for programming PEG Gen4 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  PegGen4EndPointPreset;      ///< <b>(Test)</b> Used for programming PEG Gen4 preset values per lane. Range: 0-9, 7 is default for each lane
  UINT8                  PegGen4EndPointHint;        ///< <b>(Test)</b> Hint value per lane for the PEG Gen4 End Point. Range: 0-6, 2 is default for each lane
} CPU_PCIE_EQ_LANE_PARAM;

/**
  The CPU_PCI_ROOT_PORT_CONFIG describe the feature and capability of each CPU PCIe root port.
**/
typedef struct {

  UINT32  ExtSync                         :  1;   ///< Indicate whether the extended synch is enabled. <b>0: Disable</b>; 1: Enable.
  UINT32  VcEnabled                       :  1;   ///< Virtual Channel. 0: Disable; <b>1: Enable</b>
  UINT32  MultiVcEnabled                  :  1;   ///< Multiple Virtual Channel. 0: Disable; <b>1: Enable</b>
  UINT32  PeerToPeer                      :  1;   ///< Peer to Peer Mode. <b>0: Disable</b>; 1: Enable.
  UINT32  RsvdBits0                       : 28;   ///< Reserved bits
  /**
  PCIe Gen4 Equalization Method
  - HwEq           (0x1) : Hardware Equalization (Default)
  - StaticEq       (0x2) : Static Equalization
  **/
  UINT8   Gen4EqPh3Method;
  UINT8   FomsCp;                                 ///< FOM Score Board Control Policy
  UINT8   RsvdBytes0[2];                          ///< Reserved bytes

  //
  // Gen3 Equalization settings
  //
  UINT32  Gen3Uptp            :  4;               ///< <b>(Test)</b> Upstream Port Transmitter Preset used during Gen3 Link Equalization. Used for all lanes.  Default is <b>7</b>.
  UINT32  Gen3Dptp            :  4;               ///< <b>(Test)</b> Downstream Port Transmiter Preset used during Gen3 Link Equalization. Used for all lanes.  Default is <b>7</b>.
  //
  // Gen4 Equalization settings
  //
  UINT32  Gen4Uptp            :  4;               ///< <b>(Test)</b> Upstream Port Transmitter Preset used during Gen4 Link Equalization. Used for all lanes.  Default is <b>7</b>.
  UINT32  Gen4Dptp            :  4;               ///< <b>(Test)</b> Downstream Port Transmiter Preset used during Gen4 Link Equalization. Used for all lanes.  Default is <b>7</b>.
  //
  // Gen5 Equalization settings
  //
  UINT32  Gen5Uptp            :  4;               ///< <b>(Test)</b> Upstream Port Transmitter Preset used during Gen5 Link Equalization. Used for all lanes.  Default is <b>7</b>.
  UINT32  Gen5Dptp            :  4;               ///< <b>(Test)</b> Downstream Port Transmiter Preset used during Gen5 Link Equalization. Used for all lanes.  Default is <b>7</b>.
  UINT32  RsvdBits1           :  8;               ///< Reserved Bits

  PCIE_ROOT_PORT_COMMON_CONFIG                    PcieRpCommonConfig;                       ///< <b>(Test)</b> Includes policies which are common to both SA and PCH RootPort

} CPU_PCIE_ROOT_PORT_CONFIG;

typedef struct {
  UINT8   PcieGen3PresetCoeffSelection;           ///<Gen3 Preset or Coefficient Selection
  UINT8   PcieGen4PresetCoeffSelection;           ///<Gen4 Preset or Coefficient Selection
  UINT8   RsvdBytes0[2];                          ///<Reserved bytes
} CPU_PCIE_ROOT_PORT_CONFIG2;

/**
  The CPU_PCIE_CONFIG block describes the expected configuration of the CPU PCI Express controllers
  <b>Revision 1< / b>:
  -Initial version.
  <b>Revision 2</b>:
  - SlotSelection policy added
  <b>Revision 3</b>
  - Deprecate PegGen3ProgramStaticEq and PegGen4ProgramStaticEq
  <b>Revision 4</b>:
  - Deprecating SetSecuredRegisterLock
  <b>Revision 5</b>:
  - Adding Serl
**/
typedef struct {
  CONFIG_BLOCK_HEADER               Header;                   ///< Config Block Header
  ///
  /// These members describe the configuration of each SA PCIe root port.
  ///
  CPU_PCIE_ROOT_PORT_CONFIG         RootPort[CPU_PCIE_MAX_ROOT_PORTS];
  ///
  /// Gen3 Equalization settings for physical PCIe lane, index 0 represents PCIe lane 1, etc.
  /// Corresponding entries are used when root port EqPh3Method is PchPcieEqStaticCoeff (default).
  ///
  CPU_PCIE_EQ_LANE_PARAM            EqPh3LaneParam[SA_PEG_MAX_LANE];
  ///
  /// List of coefficients used during equalization (applicable to both software and hardware EQ)
  ///
  PCIE_EQ_PARAM                     HwEqGen4CoeffList[PCIE_HWEQ_COEFFS_MAX];  // Deprecated Policy

  PCIE_COMMON_CONFIG                PcieCommonConfig;   /// < <b>(Test)</b> Includes policies which are common to both SA and PCH PCIe

  UINT32  FiaProgramming                  :  1;        /// < Skip Fia Configuration and lock if enable
  ///
  /// This member describes whether the PCI Express Clock Gating for each root port
  /// is enabled by platform modules. <b>0: Disable</b>; 1: Enable.
  ///
  UINT32  ClockGating                     :  1;
  ///
  /// This member describes whether the PCI Express Power Gating for each root port
  /// is enabled by platform modules. <b>0: Disable</b>; 1: Enable.
  ///
  UINT32  PowerGating                     :  1;
  // Deprecated Policy
  /**
    <b>(Test)</b> Program PEG Gen3 EQ Phase1 Static Presets
  - Disabled        (0x0)  : Disable EQ Phase1 Static Presets Programming
  - <b>Enabled</b>  (0x1)  : Enable  EQ Phase1 Static Presets Programming (Default)
  **/
  UINT32  PegGen3ProgramStaticEq          :  1;

  // Deprecated Policy
  /**
  <b>(Test)</b> Program PEG Gen4 EQ Phase1 Static Presets
  - Disabled        (0x0)  : Disable EQ Phase1 Static Presets Programming
  - <b>Enabled</b>  (0x1)  : Enable  EQ Phase1 Static Presets Programming (Default)
  **/
  UINT32  PegGen4ProgramStaticEq          :  1;
  /**
  <b>(Test)</b> Cpu Pcie Secure Register Lock
  - Disabled        (0x0)
  - <b>Enabled</b>  (0x1)
  **/
  UINT32  SetSecuredRegisterLock          :  1;  // Deprecated Policy
  ///
  /// This member allows to select between the PCI Express M2 or CEMx4 slot <b>1: PCIe M2</b>; 0: CEMx4 slot.
  ///
  UINT32  SlotSelection                   :  1;
  ///
  /// Set/Clear Serl(Secure Equalization Register Lock)
  ///
  UINT32  Serl                            :  1;

  UINT32  RsvdBits0                       : 24;

  /**
    PCIe device override table
    The PCIe device table is being used to override PCIe device ASPM settings.
    This is a pointer points to a 32bit address. And it's only used in PostMem phase.
    Please refer to PCH_PCIE_DEVICE_OVERRIDE structure for the table.
    Last entry VendorId must be 0.
    The prototype of this policy is:
    CPU_PCIE_DEVICE_OVERRIDE *PcieDeviceOverrideTablePtr;
  **/
  UINT32  PcieDeviceOverrideTablePtr;
  CPU_PCIE_ROOT_PORT_CONFIG2         RootPort2[CPU_PCIE_MAX_ROOT_PORTS];
  PCIE_COMMON_CONFIG2                PcieCommonConfig2;
} CPU_PCIE_CONFIG;

#pragma pack (pop)

#endif // _CPU_PCIE_CONFIG_H_
