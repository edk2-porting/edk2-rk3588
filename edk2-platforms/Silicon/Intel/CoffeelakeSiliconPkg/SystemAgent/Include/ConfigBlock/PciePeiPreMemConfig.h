/** @file
  Policy definition for PCIe Config Block

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCIE_PEI_PREMEM_CONFIG_H_
#define _PCIE_PEI_PREMEM_CONFIG_H_

#include <Library/GpioLib.h>
#include <SaAccess.h>

#pragma pack(push, 1)

#define SA_PCIE_PEI_PREMEM_CONFIG_REVISION 3

///
/// SA GPIO Data Structure
///
typedef struct {
  GPIO_PAD      GpioPad;        ///< Offset 0: GPIO Pad
  UINT8         Value;          ///< Offset 4: GPIO Value
  UINT8         Rsvd0[3];       ///< Offset 5: Reserved for 4 bytes alignment
  UINT32        Active  :1;     ///< Offset 8: 0=Active Low; 1=Active High
  UINT32        RsvdBits0:31;
} SA_GPIO_INFO_PCIE;

///
/// SA Board PEG GPIO Info
///
typedef struct {
  SA_GPIO_INFO_PCIE  SaPeg0ResetGpio;    ///< Offset 0:  PEG0 PERST# GPIO assigned, must be a PCH GPIO pin
  SA_GPIO_INFO_PCIE  SaPeg3ResetGpio;    ///< Offset 12: PEG3 PERST# GPIO assigned, must be a PCH GPIO pin
  BOOLEAN            GpioSupport;        ///< Offset 24: 1=Supported; 0=Not Supported
  UINT8              Rsvd0[3];           ///< Offset 25: Reserved for 4 bytes alignment
} PEG_GPIO_DATA;


/**
 PCI Express and DMI controller configuration\n
 @note <b>Optional.</b> These policies will be ignored if there is no PEG port present on board.
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Change PegGen3RxCtleOverride of PCIE_PEI_PREMEM_CONFIG from one bit to UINT8
  - Change DmiGen3RxCtlePeaking default to 0
  <b>Revision 3</b>:
  - Added PEG IMR support
  - Added UINT8 PegImrEnable
  - Added UINT16 PegImrSize
  - Added UINT8 ImrRpSelection
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

  /**
   Offset 28:8 to 28:15 :
   <b>(Test)</b> PEG Enable Control
  - Disabled    (0x0)  : Disable PEG Port
  - Enabled     (0x1)  : Enable PEG Port (If Silicon SKU permits it)
  - <b>Auto</b> (0x2)  : If an endpoint is present, enable the PEG Port, Disable otherwise (Default)
  **/
  UINT32                 Peg0Enable                      :  2;        ///< Enable/Disable PEG 0:1:0 Root Port
  UINT32                 Peg1Enable                      :  2;        ///< <b>(Test)</b> Enable/Disable PEG 0:1:1 Root Port
  UINT32                 Peg2Enable                      :  2;        ///< <b>(Test)</b> Enable/Disable PEG 0:1:2 Root Port
  UINT32                 Peg3Enable                      :  2;        ///< <b>(Test)</b> Enable/Disable PEG 0:6:0 Root Port.

  /**
   Offset 28:16 :
   <b>(Test)</b> PCIe Link Speed Control
  - <b>Auto</b> (0x0)  : Maximum possible Link speed (Default)
  - Gen1        (0x1)  : Limit Link to Gen1 Speed
  - Gen2        (0x2)  : Limit Link to Gen2 Speed
  - Gen3        (0x3)  : Limit Link to Gen3 Speed
  **/
  UINT32                 Peg0MaxLinkSpeed                :  2;        ///< PCIe Link Speed Control for PEG 0:1:0 Root Port.
  UINT32                 Peg1MaxLinkSpeed                :  2;        ///< <b>(Test)</b> PCIe Link Speed Control for PEG 0:1:1 Root Port.
  UINT32                 Peg2MaxLinkSpeed                :  2;        ///< <b>(Test)</b> PCIe Link Speed Control for PEG 0:1:2 Root Port.
  UINT32                 Peg3MaxLinkSpeed                :  2;        ///< <b>(Test)</b> PCIe Link Speed Control for PEG 0:6:0 Root Port.
  UINT32                 RsvdBits0                       :  8;        ///< Offset 28:24 :Reserved for future use

  /**
   Offset 32:0 :
   <b>(Test)</b> PCIe Link Width Control
  - <b>Auto</b> (0x0)  : Maximum possible Link width (Default)
  - X1          (0x1)  : Limit Link to X1 Width
  - X2          (0x2)  : Limit Link to X2 Width
  - X4          (0x3)  : Limit Link to X4 Width
  - X8          (0x4)  : Limit Link to X8 Width
  **/
  UINT32                 Peg0MaxLinkWidth                :  3;        ///< PCIe Link Width Control for PEG 0:1:0 Root Port.
  UINT32                 Peg1MaxLinkWidth                :  3;        ///< <b>(Test)</b> PCIe Link Width Control for PEG 0:1:1 Root Port.
  UINT32                 Peg2MaxLinkWidth                :  3;        ///< <b>(Test)</b> PCIe Link Width Control for PEG 0:1:2 Root Port.
  UINT32                 Peg3MaxLinkWidth                :  3;        ///< <b>(Test)</b> PCIe Link Width Control for PEG 0:6:0 Root Port.
  /**
    Offset 32:12 to 32:15 :
    Power down unused lanes on the PEG Root Port.
  - Disabled     (0x0) : No power saving.
  - <b>Auto</b>  (0x1) : Bios will power down unused lanes based on the max possible link width
  **/
  UINT32                 Peg0PowerDownUnusedLanes        :  1;        ///< Power down unused lanes on the PEG 0:1:0 Root Port.
  UINT32                 Peg1PowerDownUnusedLanes        :  1;        ///< Power down unused lanes on the PEG 0:1:1 Root Port.
  UINT32                 Peg2PowerDownUnusedLanes        :  1;        ///< Power down unused lanes on the PEG 0:1:2 Root Port.
  UINT32                 Peg3PowerDownUnusedLanes        :  1;        ///< Power down unused lanes on the PEG 0:6:0 Root Port.

  /**
   Offset 32:16 to 32:23 :
   <b>(Test)</b> PCIe Equalization Phase 2 Enable Control
  - Disabled       (0x0) : Disable phase 2
  - Enabled        (0x1) : Enable phase 2
  - <b>Auto</b>    (0x2) : Use the current default method (Default)
  **/
  UINT32                 Peg0Gen3EqPh2Enable             :  2;        ///< Phase2 EQ enable on the PEG 0:1:0 Root Port.
  UINT32                 Peg1Gen3EqPh2Enable             :  2;        ///< <b>(Test)</b> Phase2 EQ enable on the PEG 0:1:1 Root Port.
  UINT32                 Peg2Gen3EqPh2Enable             :  2;        ///< <b>(Test)</b> Phase2 EQ enable on the PEG 0:1:2 Root Port.
  UINT32                 Peg3Gen3EqPh2Enable             :  2;        ///< <b>(Test)</b> Phase2 EQ enable on the PEG 0:6:0 Root Port.
  UINT32                 RsvdBits1                       :  8;        ///< Offset 32:24 :Reserved for future use
  /**
   Offset 36:0 to 36:11 :
   <b>(Test)</b> Select the method for performing Phase3 of Gen3 Equalization.
  - <b>Auto</b> (0x0)  : Use the current default method (Default)
  - HwEq        (0x1)  : Use Adaptive Hardware Equalization
  - SwEq        (0x2)  : Use Adaptive Software Equalization (Implemented in BIOS Reference Code)
  - Static      (0x3)  : Use the Static EQs provided in PegGen3EndPointPreset array for Phase1 AND Phase3 (Instead of just Phase1)
  - Disabled    (0x4)  : Bypass Equalization Phase 3
  **/
  UINT32                 Peg0Gen3EqPh3Method             :  3;        ///< Phase3 EQ method on the PEG 0:1:0 Root Port.
  UINT32                 Peg1Gen3EqPh3Method             :  3;        ///< <b>(Test)</b> Phase3 EQ method on the PEG 0:1:1 Root Port.
  UINT32                 Peg2Gen3EqPh3Method             :  3;        ///< <b>(Test)</b> Phase3 EQ method on the PEG 0:1:2 Root Port.
  UINT32                 Peg3Gen3EqPh3Method             :  3;        ///< <b>(Test)</b> Phase3 EQ method on the PEG 0:6:0 Root Port.
  /**
   Offset 36:12 :
   <b>(Test)</b> Program PEG Gen3 EQ Phase1 Static Presets
  - Disabled        (0x0)  : Disable EQ Phase1 Static Presets Programming
  - <b>Enabled</b>  (0x1)  : Enable  EQ Phase1 Static Presets Programming (Default)
  **/
  UINT32                 PegGen3ProgramStaticEq          :  1;
  /**
   Offset 36:13 :
   <b>(Test)</b> Always Attempt Gen3 Software Equalization

   When enabled, Gen3 Software Equalization will be executed every boot.  When disabled, it will be only executed if the CPU
   or EP is changed, otherwise it is skipped and the previous EQ value will be re-used.

   This setting will only have an effect if Software Equalization is enabled and OEM Platform Code implements
   save/restore of the PegDataPtr data (see below).  If PegDataPtr is not saved/restored RC forces this to be enabled.

  - <b>Disabled</b> (0x0)  : Reuse EQ settings saved/restored from NVRAM whenever possible (Default)
  - Enabled         (0x1)  : Re-test and generate new EQ values every boot, not recommended
  **/
  UINT32                 Gen3SwEqAlwaysAttempt           :  1;
  /**
   Offset 36:14 to 36:16 :
   <b>(Test)</b> Select number of TxEq presets to test in the PCIe/DMI Software Equalization Algorithm
  - P7,P3,P5,P8 (0x0)  : Test Presets 7, 3, 5, and 8
  - P0-P9       (0x1)  : Test Presets 0-9
  - <b>Auto</b> (0x2)  : Use the current default method (Default)
  Auto will test Presets 7, 3, 5, and 8.  It is possible for this default to change over time;
  using "Auto" will ensure Reference Code always uses the latest default settings.
  @warning Do not change from the default.  Hard to detect issues are likely.
  **/
  UINT32                 Gen3SwEqNumberOfPresets         :  3;
  /**
   Offset 36:17 to 36:18:
   <b>(Test)</b> Offset 36 Enable use of the Voltage Offset and Centering Test in the PCIe Software Equalization Algorithm
  - Disabled     (0x0) : Disable VOC Test
  - Enabled      (0x1) : Enable VOC Test
  - <b>Auto</b>  (0x2) : Use the current default (Default)
  **/
  UINT32                 Gen3SwEqEnableVocTest           :  2;
  /**
    Offset 36:19 :
    Select when PCIe ASPM programming will happen in relation to the Oprom
  - <b>Before</b> (0x0) : Do PCIe ASPM programming before Oprom. (Default)
  - After         (0x1) : Do PCIe ASPM programming after Oprom. This will require an SMI handler to save/restore ASPM settings.
  **/
  UINT32                 InitPcieAspmAfterOprom          :  1;
  /**
   Offset 36:20 :
   <b>(Test)</b> PCIe Rx Compliance Testing Mode
  - <b>Disabled</b> (0x0) : Normal Operation             - Disable PCIe Rx Compliance testing (Default)
  - Enabled         (0x1) : PCIe Rx Compliance Test Mode - PEG controller is in Rx Compliance Testing Mode; it should only be set when doing PCIe compliance testing
  **/
  UINT32                 PegRxCemTestingMode             :  1;

  /**
    Offset 36:21 to 36:24 :
    <b>(Test)</b> PCIe Rx Compliance Loopback Lane

    When PegRxCemTestingMode is Enabled, the specificied Lane (0 - 15) will be
    used for RxCEMLoopback.

    Default is Lane 0.
  **/
  UINT32                 PegRxCemLoopbackLane            :  4;
  /**
   Offset 36:25 to 36:28 :
   <b>(Test)</b> Generate PCIe BDAT Margin Table. Set this policy to enable the generation and addition of PCIe margin data to the BDAT table.
  - <b>Disabled</b> (0x0) : Normal Operation          - Disable PCIe BDAT margin data generation (Default)
  - PortData        (0x1) : Port Data                 - Generate PCIe BDAT margin data
  **/
  UINT32                 PegGenerateBdatMarginTable      :  4;
   /**
   Offset 36:29 :
   <b>(Test)</b> PCIe Non-Protocol Awareness for Rx Compliance Testing
  - <b>Disabled</b> (0x0) : Normal Operation                - Disable non-protocol awareness (Default)
  - Enabled         (0x1) : Non-Protocol Awareness Enabled  - Enable non-protocol awareness for compliance testing
  **/
  UINT32                 PegRxCemNonProtocolAwareness    :  1;
   /**
   Offset 36:30 :
   <b>(Test)</b> PCIe Disable Spread Spectrum Clocking. This feature should be TRUE only for compliance testing
  - <b>False</b>          (0x0) : Normal Operation                 - SSC enabled  (Default)
  - True                  (0x1) : Disable SSC                      - Disable SSC for compliance testing
  **/
  UINT32                 PegDisableSpreadSpectrumClocking :  1;

  UINT32                 RsvdBits2                        :  1;

  UINT8                  DmiGen3RootPortPreset[SA_DMI_MAX_LANE];      ///< Offset 40 Used for programming DMI Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen3EndPointPreset[SA_DMI_MAX_LANE];      ///< Offset 44 Used for programming DMI Gen3 preset values per lane. Range: 0-9, 7 is default for each lane
  UINT8                  DmiGen3EndPointHint[SA_DMI_MAX_LANE];        ///< Offset 48 Hint value per lane for the DMI Gen3 End Point. Range: 0-6, 2 is default for each lane
  /**
   Offset 52 :
   DMI Gen3 RxCTLEp per-Bundle control. The range of the setting is (0-15). This setting
   has to be specified based upon platform design and must follow the guideline. Default is 0.
  **/

  UINT8                  DmiGen3RxCtlePeaking[SA_DMI_MAX_BUNDLE];

  UINT8                  PegGen3RootPortPreset[SA_PEG_MAX_LANE];      ///< Offset 54 <b>(Test)</b> Used for programming PEG Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  PegGen3EndPointPreset[SA_PEG_MAX_LANE];      ///< Offset 70 <b>(Test)</b> Used for programming PEG Gen3 preset values per lane. Range: 0-9, 7 is default for each lane
  UINT8                  PegGen3EndPointHint[SA_PEG_MAX_LANE];        ///< Offset 86 <b>(Test)</b> Hint value per lane for the PEG Gen3 End Point. Range: 0-6, 2 is default for each lane
  /**
   Offset 102:
   PCIe Gen3 RxCTLEp per-Bundle control. The range of the setting is (0-15). This setting
   has to be specified based upon platform design and must follow the guideline. Default is 12.
  **/
  UINT8                  PegGen3RxCtlePeaking[SA_PEG_MAX_BUNDLE];
  /**
  Offset 110:
  <b>(Test)</b>Used for PCIe Gen3 Software Equalization. Range: 0-65535, default is 1000.
  @warning Do not change from the default.  Hard to detect issues are likely.
  @note An attack on this policy could result in an apparent hang,
    but the system will eventually boot.  This variable should be protected.
  **/
  UINT16                 Gen3SwEqJitterDwellTime;
  /**
   Offset 112:
   This is a memory data pointer for saved preset search results. The reference code will store
   the Gen3 Preset Search results in the SaPegHob. In order to skip the Gen3
   preset search on boots where the PEG card configuration has not changed since the previous boot,
   platform code can save the contents of the SaPegHob in DXE (When it present and for size reported by Header.HobLength)
   and provide a pointer to a restored copy of that data. Default value is NULL, which results in a full
   preset search every boot.

   @note An attack on this policy could prevent the PCIe display from working until a boot when
   PegDataPtr is NULL or Gen3SwEqAlwaysAttempt is enabled.  The variable used to save the
   preset search results should be protected in a way that it can only be modified by the
   platform manufacturer.
  **/
  VOID                   *PegDataPtr;
  /**
  Offset 116:
  <b>(Test)</b>Used for PCIe Gen3 Software Equalization. Range: 0-65535, default is 1.
  @warning Do not change from the default.  Hard to detect issues are likely.
  **/
  UINT16                 Gen3SwEqJitterErrorTarget;

  /**
  Offset 118:
  <b>(Test)</b>Used for PCIe Gen3 Software Equalization. Range: 0-65535, default is 10000.
  @warning Do not change from the default.  Hard to detect issues are likely.
  @note An attack on this policy could result in an apparent hang,
    but the system will eventually boot.  This variable should be protected.
  **/
  UINT16                 Gen3SwEqVocDwellTime;

  /**
  Offset 120:
  <b>(Test)</b>Used for PCIe Gen3 Software Equalization. Range: 0-65535, default is 2.
  @warning Do not change from the default.  Hard to detect issues are likely.
  **/
  UINT16                 Gen3SwEqVocErrorTarget;
  /**
  Offset 122:
    PCIe Hot Plug Enable/Disable. It has 2 policies.
  - Disabled (0x0)     : No hotplug.
  - Enabled (0x1)      : Bios assist hotplug.
  **/
  UINT8                  PegRootPortHPE[SA_PEG_MAX_FUN];
  UINT8                  DmiDeEmphasis;                               ///< Offset 125 This field is used to describe the DeEmphasis control for DMI (-6 dB and -3.5 dB are the options)
  UINT8                  Rsvd0[2];                                    ///< Offset 126
  /**
   Offset 128:
   This contains the PCIe PERST# GPIO information.  This structure is required
   for PCIe Gen3 operation. The reference code will use the information in this structure in
   order to reset PCIe Gen3 devices during equalization, if necessary.  Refer to the Platform
   Developer's Guide (PDG) for additional details.
  **/
  PEG_GPIO_DATA          PegGpioData;

   /**
   Offset 156
   <b>(Test)</b> PCIe Override RxCTLE. This feature should only be true to disable RxCTLE adaptive behavior for compliance testing
  - <b>False</b>          (0x0) : Normal Operation                 - RxCTLE adaptive behavior enabled  (Default)
  - True                  (0x1) : Override RxCTLE                  - Disable RxCTLE adaptive behavior to keep the configured RxCTLE peak values unmodified
  From CFL onwards, modularity is introduced to this setup option so that the RxCTLE adaptive behavior could be controlled at the controller level.
  Making this variable a UINT8 to accomodate the values of all controllers as bit definition
  **/
  UINT8                 PegGen3RxCtleOverride;
  UINT8                 Reserved1;
  UINT16                Reserved2;

} PCIE_PEI_PREMEM_CONFIG;
#pragma pack(pop)

#endif // _PCIE_PEI_PREMEM_CONFIG_H_
