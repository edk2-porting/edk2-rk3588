/**@file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

  //
  // Define SA NVS Area operatino region.
  //
  
#ifndef _SA_NVS_H_
#define _SA_NVS_H_

#pragma pack (push,1)
typedef struct {
  UINT32   IgdOpRegionAddress;                      ///< Offset 0       IGD OpRegion base address
  UINT8    GfxTurboIMON;                            ///< Offset 4       IMON Current Value
  UINT8    IgdState;                                ///< Offset 5       IGD State (Primary Display = 1)
  UINT8    IgdBootType;                             ///< Offset 6       IGD Boot Display Device
  UINT8    IgdPanelType;                            ///< Offset 7       IGD Panel Type CMOS option
  UINT8    IgdPanelScaling;                         ///< Offset 8       IGD Panel Scaling
  UINT8    IgdBiaConfig;                            ///< Offset 9       IGD BIA Configuration
  UINT8    IgdSscConfig;                            ///< Offset 10      IGD SSC Configuration
  UINT8    IgdDvmtMemSize;                          ///< Offset 11      IGD DVMT Memory Size
  UINT8    IgdFunc1Enable;                          ///< Offset 12      IGD Function 1 Enable
  UINT8    IgdHpllVco;                              ///< Offset 13      HPLL VCO
  UINT8    IgdSciSmiMode;                           ///< Offset 14      GMCH SMI/SCI mode (0=SCI)
  UINT8    IgdPAVP;                                 ///< Offset 15      IGD PAVP data
  UINT8    CurrentDeviceList;                       ///< Offset 16      Current Attached Device List
  UINT16   CurrentDisplayState;                     ///< Offset 17      Current Display State
  UINT16   NextDisplayState;                        ///< Offset 19      Next Display State
  UINT8    NumberOfValidDeviceId;                   ///< Offset 21      Number of Valid Device IDs
  UINT32   DeviceId1;                               ///< Offset 22      Device ID 1
  UINT32   DeviceId2;                               ///< Offset 26      Device ID 2
  UINT32   DeviceId3;                               ///< Offset 30      Device ID 3
  UINT32   DeviceId4;                               ///< Offset 34      Device ID 4
  UINT32   DeviceId5;                               ///< Offset 38      Device ID 5
  UINT32   DeviceId6;                               ///< Offset 42      Device ID 6
  UINT32   DeviceId7;                               ///< Offset 46      Device ID 7
  UINT32   DeviceId8;                               ///< Offset 50      Device ID 8
  UINT32   DeviceId9;                               ///< Offset 54      Device ID 9
  UINT32   DeviceId10;                              ///< Offset 58      Device ID 10
  UINT32   DeviceId11;                              ///< Offset 62      Device ID 11
  UINT32   DeviceId12;                              ///< Offset 66      Device ID 12
  UINT32   DeviceId13;                              ///< Offset 70      Device ID 13
  UINT32   DeviceId14;                              ///< Offset 74      Device ID 14
  UINT32   DeviceId15;                              ///< Offset 78      Device ID 15
  UINT32   DeviceIdX;                               ///< Offset 82      Device ID for eDP device
  UINT32   NextStateDid1;                           ///< Offset 86      Next state DID1 for _DGS
  UINT32   NextStateDid2;                           ///< Offset 90      Next state DID2 for _DGS
  UINT32   NextStateDid3;                           ///< Offset 94      Next state DID3 for _DGS
  UINT32   NextStateDid4;                           ///< Offset 98      Next state DID4 for _DGS
  UINT32   NextStateDid5;                           ///< Offset 102     Next state DID5 for _DGS
  UINT32   NextStateDid6;                           ///< Offset 106     Next state DID6 for _DGS
  UINT32   NextStateDid7;                           ///< Offset 110     Next state DID7 for _DGS
  UINT32   NextStateDid8;                           ///< Offset 114     Next state DID8 for _DGS
  UINT32   NextStateDidEdp;                         ///< Offset 118     Next state DID for eDP
  UINT8    LidState;                                ///< Offset 122     Lid State (Lid Open = 1)
  UINT32   AKsv0;                                   ///< Offset 123     First four bytes of AKSV (manufacturing mode)
  UINT8    AKsv1;                                   ///< Offset 127     Fifth byte of AKSV (manufacturing mode)
  UINT8    BrightnessPercentage;                    ///< Offset 128     Brightness Level Percentage
  UINT8    AlsEnable;                               ///< Offset 129     Ambient Light Sensor Enable
  UINT8    AlsAdjustmentFactor;                     ///< Offset 130     Ambient Light Adjusment Factor
  UINT8    LuxLowValue;                             ///< Offset 131     LUX Low Value
  UINT8    LuxHighValue;                            ///< Offset 132     LUX High Value
  UINT8    ActiveLFP;                               ///< Offset 133     Active LFP
  UINT8    ImguAcpiMode;                            ///< Offset 134     IMGU ACPI device type
  UINT8    EdpValid;                                ///< Offset 135     Check for eDP display device
  UINT8    SgMode;                                  ///< Offset 136     SG Mode (0=Disabled, 1=SG Muxed, 2=SG Muxless, 3=DGPU Only)
  UINT8    SgFeatureList;                           ///< Offset 137     SG Feature List
  UINT8    Pcie0GpioSupport;                        ///< Offset 138     PCIe0 GPIO Support (0=Disabled, 1=PCH Based, 2=I2C Based)
  UINT8    Pcie0HoldRstExpanderNo;                  ///< Offset 139     PCIe0 HLD RST IO Expander Number
  UINT32   Pcie0HoldRstGpioNo;                      ///< Offset 140     PCIe0 HLD RST GPIO Number
  UINT8    Pcie0HoldRstActiveInfo;                  ///< Offset 144     PCIe0 HLD RST GPIO Active Information
  UINT8    Pcie0PwrEnExpanderNo;                    ///< Offset 145     PCIe0 PWR Enable IO Expander Number
  UINT32   Pcie0PwrEnGpioNo;                        ///< Offset 146     PCIe0 PWR Enable GPIO Number
  UINT8    Pcie0PwrEnActiveInfo;                    ///< Offset 150     PCIe0 PWR Enable GPIO Active Information
  UINT8    Pcie1GpioSupport;                        ///< Offset 151     PCIe1 GPIO Support (0=Disabled, 1=PCH Based, 2=I2C Based)
  UINT8    Pcie1HoldRstExpanderNo;                  ///< Offset 152     PCIe1 HLD RST IO Expander Number
  UINT32   Pcie1HoldRstGpioNo;                      ///< Offset 153     PCIe1 HLD RST GPIO Number
  UINT8    Pcie1HoldRstActiveInfo;                  ///< Offset 157     PCIe1 HLD RST GPIO Active Information
  UINT8    Pcie1PwrEnExpanderNo;                    ///< Offset 158     PCIe1 PWR Enable IO Expander Number
  UINT32   Pcie1PwrEnGpioNo;                        ///< Offset 159     PCIe1 PWR Enable GPIO Number
  UINT8    Pcie1PwrEnActiveInfo;                    ///< Offset 163     PCIe1 PWR Enable GPIO Active Information
  UINT8    Pcie2GpioSupport;                        ///< Offset 164     PCIe2 GPIO Support (0=Disabled, 1=PCH Based, 2=I2C Based)
  UINT8    Pcie2HoldRstExpanderNo;                  ///< Offset 165     PCIe2 HLD RST IO Expander Number
  UINT32   Pcie2HoldRstGpioNo;                      ///< Offset 166     PCIe2 HLD RST GPIO Number
  UINT8    Pcie2HoldRstActiveInfo;                  ///< Offset 170     PCIe2 HLD RST GPIO Active Information
  UINT8    Pcie2PwrEnExpanderNo;                    ///< Offset 171     PCIe2 PWR Enable IO Expander Number
  UINT32   Pcie2PwrEnGpioNo;                        ///< Offset 172     PCIe2 PWR Enable GPIO Number
  UINT8    Pcie2PwrEnActiveInfo;                    ///< Offset 176     PCIe2 PWR Enable GPIO Active Information
  UINT16   DelayAfterPwrEn;                         ///< Offset 177     Delay after power enable for PCIe
  UINT16   DelayAfterHoldReset;                     ///< Offset 179     Delay after Hold Reset for PCIe
  UINT8    Pcie0EpCapOffset;                        ///< Offset 181     PCIe0 Endpoint Capability Structure Offset
  UINT32   XPcieCfgBaseAddress;                     ///< Offset 182     Any Device's PCIe Config Space Base Address
  UINT16   GpioBaseAddress;                         ///< Offset 186     GPIO Base Address
  UINT32   NvIgOpRegionAddress;                     ///< Offset 188     NVIG opregion address
  UINT32   NvHmOpRegionAddress;                     ///< Offset 192     NVHM opregion address
  UINT32   ApXmOpRegionAddress;                     ///< Offset 196     AMDA opregion address
  UINT8    Peg0LtrEnable;                           ///< Offset 200     Latency Tolerance Reporting Enable
  UINT8    Peg0ObffEnable;                          ///< Offset 201     Optimized Buffer Flush and Fill
  UINT8    Peg1LtrEnable;                           ///< Offset 202     Latency Tolerance Reporting Enable
  UINT8    Peg1ObffEnable;                          ///< Offset 203     Optimized Buffer Flush and Fill
  UINT8    Peg2LtrEnable;                           ///< Offset 204     Latency Tolerance Reporting Enable
  UINT8    Peg2ObffEnable;                          ///< Offset 205     Optimized Buffer Flush and Fill
  UINT16   PegLtrMaxSnoopLatency;                   ///< Offset 206     SA Peg Latency Tolerance Reporting Max Snoop Latency
  UINT16   PegLtrMaxNoSnoopLatency;                 ///< Offset 208     SA Peg Latency Tolerance Reporting Max No Snoop Latency
  UINT8    Peg0PowerDownUnusedBundles;              ///< Offset 210     Peg0 Unused Bundle Control
  UINT8    Peg1PowerDownUnusedBundles;              ///< Offset 211     Peg1 Unused Bundle Control
  UINT8    Peg2PowerDownUnusedBundles;              ///< Offset 212     Peg2 Unused Bundle Control
  UINT8    PackageCstateLimit;                      ///< Offset 213     The lowest C-state for the package
  UINT8    PwrDnBundlesGlobalEnable;                ///< Offset 214     Pegx Unused Bundle Control Global Enable (0=Disabled, 1=Enabled)
  UINT64   Mmio64Base;                              ///< Offset 215     Base of above 4GB MMIO resource
  UINT64   Mmio64Length;                            ///< Offset 223     Length of above 4GB MMIO resource
  UINT32   CpuIdInfo;                               ///< Offset 231     CPU ID info to get Family Id or Stepping
  UINT8    Pcie1EpCapOffset;                        ///< Offset 235     PCIe1 Endpoint Capability Structure Offset
  UINT8    Pcie2EpCapOffset;                        ///< Offset 236     PCIe2 Endpoint Capability Structure Offset
  UINT8    Pcie0SecBusNum;                          ///< Offset 237     PCIe0 Secondary Bus Number (PCIe0 Endpoint Bus Number)
  UINT8    Pcie1SecBusNum;                          ///< Offset 238     PCIe1 Secondary Bus Number (PCIe0 Endpoint Bus Number)
  UINT8    Pcie2SecBusNum;                          ///< Offset 239     PCIe2 Secondary Bus Number (PCIe0 Endpoint Bus Number)
  UINT32   Mmio32Base;                              ///< Offset 240     Base of below 4GB MMIO resource
  UINT32   Mmio32Length;                            ///< Offset 244     Length of below 4GB MMIO resource
  UINT32   Pcie0WakeGpioNo;                         ///< Offset 248     PCIe0 RTD3 Device Wake GPIO Number
  UINT32   Pcie1WakeGpioNo;                         ///< Offset 252     PCIe1 RTD3 Device Wake GPIO Number
  UINT32   Pcie2WakeGpioNo;                         ///< Offset 256     PCIe2 RTD3 Device Wake GPIO Number
  UINT8    Reserved0[240];                          ///< Offset 260:499
  UINT8    Reserved1[3];                            ///< Offset 500:502
} SYSTEM_AGENT_GLOBAL_NVS_AREA;

#pragma pack(pop)
#endif
