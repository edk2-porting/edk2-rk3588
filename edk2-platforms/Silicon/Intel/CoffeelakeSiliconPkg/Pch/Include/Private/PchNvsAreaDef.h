/** @file
  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

  //
  // Define PCH NVS Area operatino region.
  //

#ifndef _PCH_NVS_AREA_DEF_H_
#define _PCH_NVS_AREA_DEF_H_

#pragma pack (push,1)
typedef struct {
  UINT16   PchSeries;                               ///< Offset 0       PCH Series
  UINT16   PchGeneration;                           ///< Offset 2       PCH Generation
  UINT16   PchStepping;                             ///< Offset 4       PCH Stepping
  UINT32   RpAddress[24];                           ///< Offset 6       Root Port address 1
                                                    ///< Offset 10      Root Port address 2
                                                    ///< Offset 14      Root Port address 3
                                                    ///< Offset 18      Root Port address 4
                                                    ///< Offset 22      Root Port address 5
                                                    ///< Offset 26      Root Port address 6
                                                    ///< Offset 30      Root Port address 7
                                                    ///< Offset 34      Root Port address 8
                                                    ///< Offset 38      Root Port address 9
                                                    ///< Offset 42      Root Port address 10
                                                    ///< Offset 46      Root Port address 11
                                                    ///< Offset 50      Root Port address 12
                                                    ///< Offset 54      Root Port address 13
                                                    ///< Offset 58      Root Port address 14
                                                    ///< Offset 62      Root Port address 15
                                                    ///< Offset 66      Root Port address 16
                                                    ///< Offset 70      Root Port address 17
                                                    ///< Offset 74      Root Port address 18
                                                    ///< Offset 78      Root Port address 19
                                                    ///< Offset 82      Root Port address 20
                                                    ///< Offset 86      Root Port address 21
                                                    ///< Offset 90      Root Port address 22
                                                    ///< Offset 94      Root Port address 23
                                                    ///< Offset 98      Root Port address 24
  UINT64   NHLA;                                    ///< Offset 102     HD-Audio NHLT ACPI address
  UINT32   NHLL;                                    ///< Offset 110     HD-Audio NHLT ACPI length
  UINT32   ADFM;                                    ///< Offset 114     HD-Audio DSP Feature Mask
  UINT8    SWQ0;                                    ///< Offset 118     HD-Audio SoundWire Link #1 quirk mask
  UINT8    SWQ1;                                    ///< Offset 119     HD-Audio SoundWire Link #2 quirk mask
  UINT8    SWQ2;                                    ///< Offset 120     HD-Audio SoundWire Link #3 quirk mask
  UINT8    SWQ3;                                    ///< Offset 121     HD-Audio SoundWire Link #4 quirk mask
  UINT32   DSPM;                                    ///< Offset 122     HD-Audio DSP Stolen Memory Base Address
  UINT32   SBRG;                                    ///< Offset 126     SBREG_BAR
  UINT8    GEI0;                                    ///< Offset 130     GPIO GroupIndex mapped to GPE_DW0
  UINT8    GEI1;                                    ///< Offset 131     GPIO GroupIndex mapped to GPE_DW1
  UINT8    GEI2;                                    ///< Offset 132     GPIO GroupIndex mapped to GPE_DW2
  UINT8    GED0;                                    ///< Offset 133     GPIO DW part of group mapped to GPE_DW0
  UINT8    GED1;                                    ///< Offset 134     GPIO DW part of group mapped to GPE_DW1
  UINT8    GED2;                                    ///< Offset 135     GPIO DW part of group mapped to GPE_DW2
  UINT16   PcieLtrMaxSnoopLatency[24];              ///< Offset 136     PCIE LTR max snoop Latency 1
                                                    ///< Offset 138     PCIE LTR max snoop Latency 2
                                                    ///< Offset 140     PCIE LTR max snoop Latency 3
                                                    ///< Offset 142     PCIE LTR max snoop Latency 4
                                                    ///< Offset 144     PCIE LTR max snoop Latency 5
                                                    ///< Offset 146     PCIE LTR max snoop Latency 6
                                                    ///< Offset 148     PCIE LTR max snoop Latency 7
                                                    ///< Offset 150     PCIE LTR max snoop Latency 8
                                                    ///< Offset 152     PCIE LTR max snoop Latency 9
                                                    ///< Offset 154     PCIE LTR max snoop Latency 10
                                                    ///< Offset 156     PCIE LTR max snoop Latency 11
                                                    ///< Offset 158     PCIE LTR max snoop Latency 12
                                                    ///< Offset 160     PCIE LTR max snoop Latency 13
                                                    ///< Offset 162     PCIE LTR max snoop Latency 14
                                                    ///< Offset 164     PCIE LTR max snoop Latency 15
                                                    ///< Offset 166     PCIE LTR max snoop Latency 16
                                                    ///< Offset 168     PCIE LTR max snoop Latency 17
                                                    ///< Offset 170     PCIE LTR max snoop Latency 18
                                                    ///< Offset 172     PCIE LTR max snoop Latency 19
                                                    ///< Offset 174     PCIE LTR max snoop Latency 20
                                                    ///< Offset 176     PCIE LTR max snoop Latency 21
                                                    ///< Offset 178     PCIE LTR max snoop Latency 22
                                                    ///< Offset 180     PCIE LTR max snoop Latency 23
                                                    ///< Offset 182     PCIE LTR max snoop Latency 24
  UINT16   PcieLtrMaxNoSnoopLatency[24];            ///< Offset 184     PCIE LTR max no snoop Latency 1
                                                    ///< Offset 186     PCIE LTR max no snoop Latency 2
                                                    ///< Offset 188     PCIE LTR max no snoop Latency 3
                                                    ///< Offset 190     PCIE LTR max no snoop Latency 4
                                                    ///< Offset 192     PCIE LTR max no snoop Latency 5
                                                    ///< Offset 194     PCIE LTR max no snoop Latency 6
                                                    ///< Offset 196     PCIE LTR max no snoop Latency 7
                                                    ///< Offset 198     PCIE LTR max no snoop Latency 8
                                                    ///< Offset 200     PCIE LTR max no snoop Latency 9
                                                    ///< Offset 202     PCIE LTR max no snoop Latency 10
                                                    ///< Offset 204     PCIE LTR max no snoop Latency 11
                                                    ///< Offset 206     PCIE LTR max no snoop Latency 12
                                                    ///< Offset 208     PCIE LTR max no snoop Latency 13
                                                    ///< Offset 210     PCIE LTR max no snoop Latency 14
                                                    ///< Offset 212     PCIE LTR max no snoop Latency 15
                                                    ///< Offset 214     PCIE LTR max no snoop Latency 16
                                                    ///< Offset 216     PCIE LTR max no snoop Latency 17
                                                    ///< Offset 218     PCIE LTR max no snoop Latency 18
                                                    ///< Offset 220     PCIE LTR max no snoop Latency 19
                                                    ///< Offset 222     PCIE LTR max no snoop Latency 20
                                                    ///< Offset 224     PCIE LTR max no snoop Latency 21
                                                    ///< Offset 226     PCIE LTR max no snoop Latency 22
                                                    ///< Offset 228     PCIE LTR max no snoop Latency 23
                                                    ///< Offset 230     PCIE LTR max no snoop Latency 24
  UINT8    XHPC;                                    ///< Offset 232     Number of HighSpeed ports implemented in XHCI controller
  UINT8    XRPC;                                    ///< Offset 233     Number of USBR ports implemented in XHCI controller
  UINT8    XSPC;                                    ///< Offset 234     Number of SuperSpeed ports implemented in XHCI controller
  UINT8    XSPA;                                    ///< Offset 235     Address of 1st SuperSpeed port
  UINT32   HPTB;                                    ///< Offset 236     HPET base address
  UINT8    HPTE;                                    ///< Offset 240     HPET enable
  //SerialIo block
  UINT8    SMD[12];                                 ///< Offset 241     SerialIo controller 0 mode
                                                    ///< Offset 242     SerialIo controller 1 mode
                                                    ///< Offset 243     SerialIo controller 2 mode
                                                    ///< Offset 244     SerialIo controller 3 mode
                                                    ///< Offset 245     SerialIo controller 4 mode
                                                    ///< Offset 246     SerialIo controller 5 mode
                                                    ///< Offset 247     SerialIo controller 6 mode
                                                    ///< Offset 248     SerialIo controller 7 mode
                                                    ///< Offset 249     SerialIo controller 8 mode
                                                    ///< Offset 250     SerialIo controller 9 mode
                                                    ///< Offset 251     SerialIo controller A mode
                                                    ///< Offset 252     SerialIo controller B mode
  UINT8    SIR[12];                                 ///< Offset 253     SerialIo controller 0 irq number
                                                    ///< Offset 254     SerialIo controller 1 irq number
                                                    ///< Offset 255     SerialIo controller 2 irq number
                                                    ///< Offset 256     SerialIo controller 3 irq number
                                                    ///< Offset 257     SerialIo controller 4 irq number
                                                    ///< Offset 258     SerialIo controller 5 irq number
                                                    ///< Offset 259     SerialIo controller 6 irq number
                                                    ///< Offset 260     SerialIo controller 7 irq number
                                                    ///< Offset 261     SerialIo controller 8 irq number
                                                    ///< Offset 262     SerialIo controller 9 irq number
                                                    ///< Offset 263     SerialIo controller A irq number
                                                    ///< Offset 264     SerialIo controller B irq number
  UINT64   SB0[12];                                 ///< Offset 265     SerialIo controller 0 BAR0
                                                    ///< Offset 273     SerialIo controller 1 BAR0
                                                    ///< Offset 281     SerialIo controller 2 BAR0
                                                    ///< Offset 289     SerialIo controller 3 BAR0
                                                    ///< Offset 297     SerialIo controller 4 BAR0
                                                    ///< Offset 305     SerialIo controller 5 BAR0
                                                    ///< Offset 313     SerialIo controller 6 BAR0
                                                    ///< Offset 321     SerialIo controller 7 BAR0
                                                    ///< Offset 329     SerialIo controller 8 BAR0
                                                    ///< Offset 337     SerialIo controller 9 BAR0
                                                    ///< Offset 345     SerialIo controller A BAR0
                                                    ///< Offset 353     SerialIo controller B BAR0
  UINT64   SB1[12];                                 ///< Offset 361     SerialIo controller 0 BAR1
                                                    ///< Offset 369     SerialIo controller 1 BAR1
                                                    ///< Offset 377     SerialIo controller 2 BAR1
                                                    ///< Offset 385     SerialIo controller 3 BAR1
                                                    ///< Offset 393     SerialIo controller 4 BAR1
                                                    ///< Offset 401     SerialIo controller 5 BAR1
                                                    ///< Offset 409     SerialIo controller 6 BAR1
                                                    ///< Offset 417     SerialIo controller 7 BAR1
                                                    ///< Offset 425     SerialIo controller 8 BAR1
                                                    ///< Offset 433     SerialIo controller 9 BAR1
                                                    ///< Offset 441     SerialIo controller A BAR1
                                                    ///< Offset 449     SerialIo controller B BAR1
  //end of SerialIo block
  UINT8    SGIR;                                    ///< Offset 457     GPIO IRQ
  UINT8    GPHD;                                    ///< Offset 458     Hide GPIO ACPI device
  UINT8    RstPcieStorageInterfaceType[3];          ///< Offset 459     RST PCIe Storage Cycle Router#1 Interface Type
                                                    ///< Offset 460     RST PCIe Storage Cycle Router#2 Interface Type
                                                    ///< Offset 461     RST PCIe Storage Cycle Router#3 Interface Type
  UINT8    RstPcieStoragePmCapPtr[3];               ///< Offset 462     RST PCIe Storage Cycle Router#1 Power Management Capability Pointer
                                                    ///< Offset 463     RST PCIe Storage Cycle Router#2 Power Management Capability Pointer
                                                    ///< Offset 464     RST PCIe Storage Cycle Router#3 Power Management Capability Pointer
  UINT8    RstPcieStoragePcieCapPtr[3];             ///< Offset 465     RST PCIe Storage Cycle Router#1 PCIe Capabilities Pointer
                                                    ///< Offset 466     RST PCIe Storage Cycle Router#2 PCIe Capabilities Pointer
                                                    ///< Offset 467     RST PCIe Storage Cycle Router#3 PCIe Capabilities Pointer
  UINT16   RstPcieStorageL1ssCapPtr[3];             ///< Offset 468     RST PCIe Storage Cycle Router#1 L1SS Capability Pointer
                                                    ///< Offset 470     RST PCIe Storage Cycle Router#2 L1SS Capability Pointer
                                                    ///< Offset 472     RST PCIe Storage Cycle Router#3 L1SS Capability Pointer
  UINT8    RstPcieStorageEpL1ssControl2[3];         ///< Offset 474     RST PCIe Storage Cycle Router#1 Endpoint L1SS Control Data2
                                                    ///< Offset 475     RST PCIe Storage Cycle Router#2 Endpoint L1SS Control Data2
                                                    ///< Offset 476     RST PCIe Storage Cycle Router#3 Endpoint L1SS Control Data2
  UINT32   RstPcieStorageEpL1ssControl1[3];         ///< Offset 477     RST PCIe Storage Cycle Router#1 Endpoint L1SS Control Data1
                                                    ///< Offset 481     RST PCIe Storage Cycle Router#2 Endpoint L1SS Control Data1
                                                    ///< Offset 485     RST PCIe Storage Cycle Router#3 Endpoint L1SS Control Data1
  UINT16   RstPcieStorageLtrCapPtr[3];              ///< Offset 489     RST PCIe Storage Cycle Router#1 LTR Capability Pointer
                                                    ///< Offset 491     RST PCIe Storage Cycle Router#2 LTR Capability Pointer
                                                    ///< Offset 493     RST PCIe Storage Cycle Router#3 LTR Capability Pointer
  UINT32   RstPcieStorageEpLtrData[3];              ///< Offset 495     RST PCIe Storage Cycle Router#1 Endpoint LTR Data
                                                    ///< Offset 499     RST PCIe Storage Cycle Router#2 Endpoint LTR Data
                                                    ///< Offset 503     RST PCIe Storage Cycle Router#3 Endpoint LTR Data
  UINT16   RstPcieStorageEpLctlData16[3];           ///< Offset 507     RST PCIe Storage Cycle Router#1 Endpoint LCTL Data
                                                    ///< Offset 509     RST PCIe Storage Cycle Router#2 Endpoint LCTL Data
                                                    ///< Offset 511     RST PCIe Storage Cycle Router#3 Endpoint LCTL Data
  UINT16   RstPcieStorageEpDctlData16[3];           ///< Offset 513     RST PCIe Storage Cycle Router#1 Endpoint DCTL Data
                                                    ///< Offset 515     RST PCIe Storage Cycle Router#2 Endpoint DCTL Data
                                                    ///< Offset 517     RST PCIe Storage Cycle Router#3 Endpoint DCTL Data
  UINT16   RstPcieStorageEpDctl2Data16[3];          ///< Offset 519     RST PCIe Storage Cycle Router#1 Endpoint DCTL2 Data
                                                    ///< Offset 521     RST PCIe Storage Cycle Router#2 Endpoint DCTL2 Data
                                                    ///< Offset 523     RST PCIe Storage Cycle Router#3 Endpoint DCTL2 Data
  UINT16   RstPcieStorageRpDctl2Data16[3];          ///< Offset 525     RST PCIe Storage Cycle Router#1 RootPort DCTL2 Data
                                                    ///< Offset 527     RST PCIe Storage Cycle Router#2 RootPort DCTL2 Data
                                                    ///< Offset 529     RST PCIe Storage Cycle Router#3 RootPort DCTL2 Data
  UINT32   RstPcieStorageUniqueTableBar[3];         ///< Offset 531     RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X Table BAR
                                                    ///< Offset 535     RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X Table BAR
                                                    ///< Offset 539     RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X Table BAR
  UINT32   RstPcieStorageUniqueTableBarValue[3];    ///< Offset 543     RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X Table BAR value
                                                    ///< Offset 547     RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X Table BAR value
                                                    ///< Offset 551     RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X Table BAR value
  UINT32   RstPcieStorageUniquePbaBar[3];           ///< Offset 555     RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X PBA BAR
                                                    ///< Offset 559     RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X PBA BAR
                                                    ///< Offset 563     RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X PBA BAR
  UINT32   RstPcieStorageUniquePbaBarValue[3];      ///< Offset 567     RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X PBA BAR value
                                                    ///< Offset 571     RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X PBA BAR value
                                                    ///< Offset 575     RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X PBA BAR value
  UINT32   RstPcieStorageRootPortNum[3];            ///< Offset 579     RST PCIe Storage Cycle Router#1 Root Port number
                                                    ///< Offset 583     RST PCIe Storage Cycle Router#2 Root Port number
                                                    ///< Offset 587     RST PCIe Storage Cycle Router#3 Root Port number
  UINT8    EMH4;                                    ///< Offset 591     eMMC HS400 mode enabled
  UINT8    EMDS;                                    ///< Offset 592     eMMC Driver Strength
  UINT8    CpuSku;                                  ///< Offset 593     CPU SKU
  UINT16   IoTrapAddress[4];                        ///< Offset 594
  UINT8    IoTrapStatus[4];                         ///< Offset 602
  UINT16   PMBS;                                    ///< Offset 606     ACPI IO BASE address
  UINT32   PWRM;                                    ///< Offset 608     PWRM MEM BASE address
  // Cnvi specific
  UINT8    CnviMode;                                ///< Offset 612     CNVi mode
  UINT32   RmrrCsmeBaseAddress;                     ///< Offset 613     RMRR CSME Base Address
  //Voltage Margining
  UINT8    SlpS0VmRuntimeControl;                   ///< Offset 617     SLP_S0 Voltage Margining Runtime Control
  UINT8    SlpS0Vm070VSupport;                      ///< Offset 618     SLP_S0 0.70V Voltage Margining Support
  UINT8    SlpS0Vm075VSupport;                      ///< Offset 619     SLP_S0 0.75V Voltage Margining Support
  // PCH Trace Hub
  UINT8    PchTraceHubMode;                         ///< Offset 620     PCH Trace Hub Mode
  // PCH PS_ON support
  UINT8    PsOnEnable;                              ///< Offset 621     PCH PS_ON enable
  UINT32   TempRsvdMemBase;                         ///< Offset 622     Reserved memory base address for Temp MBAR
  //
  // These are for PchApciTablesSelfTest use
  //
  UINT8    LtrEnable[24];                           ///< Offset 626     Latency Tolerance Reporting Enable
                                                    ///< Offset 627     Latency Tolerance Reporting Enable
                                                    ///< Offset 628     Latency Tolerance Reporting Enable
                                                    ///< Offset 629     Latency Tolerance Reporting Enable
                                                    ///< Offset 630     Latency Tolerance Reporting Enable
                                                    ///< Offset 631     Latency Tolerance Reporting Enable
                                                    ///< Offset 632     Latency Tolerance Reporting Enable
                                                    ///< Offset 633     Latency Tolerance Reporting Enable
                                                    ///< Offset 634     Latency Tolerance Reporting Enable
                                                    ///< Offset 635     Latency Tolerance Reporting Enable
                                                    ///< Offset 636     Latency Tolerance Reporting Enable
                                                    ///< Offset 637     Latency Tolerance Reporting Enable
                                                    ///< Offset 638     Latency Tolerance Reporting Enable
                                                    ///< Offset 639     Latency Tolerance Reporting Enable
                                                    ///< Offset 640     Latency Tolerance Reporting Enable
                                                    ///< Offset 641     Latency Tolerance Reporting Enable
                                                    ///< Offset 642     Latency Tolerance Reporting Enable
                                                    ///< Offset 643     Latency Tolerance Reporting Enable
                                                    ///< Offset 644     Latency Tolerance Reporting Enable
                                                    ///< Offset 645     Latency Tolerance Reporting Enable
                                                    ///< Offset 646     Latency Tolerance Reporting Enable
                                                    ///< Offset 647     Latency Tolerance Reporting Enable
                                                    ///< Offset 648     Latency Tolerance Reporting Enable
                                                    ///< Offset 649     Latency Tolerance Reporting Enable
  UINT8    GBES;                                    ///< Offset 650     GbE Support
  UINT8    SataPortPresence;                        ///< Offset 651     Holds information from SATA PCS register about SATA ports which recieved COMINIT from connected devices.
  UINT8    SdPowerEnableActiveHigh;                 ///< Offset 652     SD PWREN# active high indication
  UINT8    EmmcEnabled;                             ///< Offset 653     Set to indicate that eMMC is enabled
  UINT8    SdCardEnabled;                           ///< Offset 654     Set to indicate that SD card is enabled
} PCH_NVS_AREA;

#pragma pack(pop)
#endif
