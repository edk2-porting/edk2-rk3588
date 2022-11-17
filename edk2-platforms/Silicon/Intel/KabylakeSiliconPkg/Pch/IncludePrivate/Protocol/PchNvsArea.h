/**@file

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

  //
  // Define PCH NVS Area operatino region.
  //
  
#ifndef _PCH_NVS_AREA_H_
#define _PCH_NVS_AREA_H_

#pragma pack (push,1)
typedef struct {
  UINT16   PchSeries;                               ///< Offset 0       PCH Series
  UINT16   PchGeneration;                           ///< Offset 2       PCH Generation
  UINT32   RpAddress[24];                           ///< Offset 4       Root Port address 1
                                                    ///< Offset 8       Root Port address 2
                                                    ///< Offset 12      Root Port address 3
                                                    ///< Offset 16      Root Port address 4
                                                    ///< Offset 20      Root Port address 5
                                                    ///< Offset 24      Root Port address 6
                                                    ///< Offset 28      Root Port address 7
                                                    ///< Offset 32      Root Port address 8
                                                    ///< Offset 36      Root Port address 9
                                                    ///< Offset 40      Root Port address 10
                                                    ///< Offset 44      Root Port address 11
                                                    ///< Offset 48      Root Port address 12
                                                    ///< Offset 52      Root Port address 13
                                                    ///< Offset 56      Root Port address 14
                                                    ///< Offset 60      Root Port address 15
                                                    ///< Offset 64      Root Port address 16
                                                    ///< Offset 68      Root Port address 17
                                                    ///< Offset 72      Root Port address 18
                                                    ///< Offset 76      Root Port address 19
                                                    ///< Offset 80      Root Port address 20
                                                    ///< Offset 84      Root Port address 21
                                                    ///< Offset 88      Root Port address 22
                                                    ///< Offset 92      Root Port address 23
                                                    ///< Offset 96      Root Port address 24
  UINT64   NHLA;                                    ///< Offset 100     HD-Audio NHLT ACPI address
  UINT32   NHLL;                                    ///< Offset 108     HD-Audio NHLT ACPI length
  UINT32   ADFM;                                    ///< Offset 112     HD-Audio DSP Feature Mask
  UINT32   SBRG;                                    ///< Offset 116     SBREG_BAR
  UINT32   GPEM;                                    ///< Offset 120     GPP_X to GPE_DWX mapping
  UINT32   GP2T[10];                                ///< Offset 124     GPE 2-tier level edged enabled Gpio pads (Group Index 0)
                                                    ///< Offset 128     GPE 2-tier level edged enabled Gpio pads (Group Index 1)
                                                    ///< Offset 132     GPE 2-tier level edged enabled Gpio pads (Group Index 2)
                                                    ///< Offset 136     GPE 2-tier level edged enabled Gpio pads (Group Index 3)
                                                    ///< Offset 140     GPE 2-tier level edged enabled Gpio pads (Group Index 4)
                                                    ///< Offset 144     GPE 2-tier level edged enabled Gpio pads (Group Index 5)
                                                    ///< Offset 148     GPE 2-tier level edged enabled Gpio pads (Group Index 6)
                                                    ///< Offset 152     GPE 2-tier level edged enabled Gpio pads (Group Index 7)
                                                    ///< Offset 156     GPE 2-tier level edged enabled Gpio pads (Group Index 8)
                                                    ///< Offset 160     GPE 2-tier level edged enabled Gpio pads (Group Index 9)
  UINT16   PcieLtrMaxSnoopLatency[24];              ///< Offset 164     PCIE LTR max snoop Latency 1
                                                    ///< Offset 166     PCIE LTR max snoop Latency 2
                                                    ///< Offset 168     PCIE LTR max snoop Latency 3
                                                    ///< Offset 170     PCIE LTR max snoop Latency 4
                                                    ///< Offset 172     PCIE LTR max snoop Latency 5
                                                    ///< Offset 174     PCIE LTR max snoop Latency 6
                                                    ///< Offset 176     PCIE LTR max snoop Latency 7
                                                    ///< Offset 178     PCIE LTR max snoop Latency 8
                                                    ///< Offset 180     PCIE LTR max snoop Latency 9
                                                    ///< Offset 182     PCIE LTR max snoop Latency 10
                                                    ///< Offset 184     PCIE LTR max snoop Latency 11
                                                    ///< Offset 186     PCIE LTR max snoop Latency 12
                                                    ///< Offset 188     PCIE LTR max snoop Latency 13
                                                    ///< Offset 190     PCIE LTR max snoop Latency 14
                                                    ///< Offset 192     PCIE LTR max snoop Latency 15
                                                    ///< Offset 194     PCIE LTR max snoop Latency 16
                                                    ///< Offset 196     PCIE LTR max snoop Latency 17
                                                    ///< Offset 198     PCIE LTR max snoop Latency 18
                                                    ///< Offset 200     PCIE LTR max snoop Latency 19
                                                    ///< Offset 202     PCIE LTR max snoop Latency 20
                                                    ///< Offset 204     PCIE LTR max snoop Latency 21
                                                    ///< Offset 206     PCIE LTR max snoop Latency 22
                                                    ///< Offset 208     PCIE LTR max snoop Latency 23
                                                    ///< Offset 210     PCIE LTR max snoop Latency 24
  UINT16   PcieLtrMaxNoSnoopLatency[24];            ///< Offset 212     PCIE LTR max no snoop Latency 1
                                                    ///< Offset 214     PCIE LTR max no snoop Latency 2
                                                    ///< Offset 216     PCIE LTR max no snoop Latency 3
                                                    ///< Offset 218     PCIE LTR max no snoop Latency 4
                                                    ///< Offset 220     PCIE LTR max no snoop Latency 5
                                                    ///< Offset 222     PCIE LTR max no snoop Latency 6
                                                    ///< Offset 224     PCIE LTR max no snoop Latency 7
                                                    ///< Offset 226     PCIE LTR max no snoop Latency 8
                                                    ///< Offset 228     PCIE LTR max no snoop Latency 9
                                                    ///< Offset 230     PCIE LTR max no snoop Latency 10
                                                    ///< Offset 232     PCIE LTR max no snoop Latency 11
                                                    ///< Offset 234     PCIE LTR max no snoop Latency 12
                                                    ///< Offset 236     PCIE LTR max no snoop Latency 13
                                                    ///< Offset 238     PCIE LTR max no snoop Latency 14
                                                    ///< Offset 240     PCIE LTR max no snoop Latency 15
                                                    ///< Offset 242     PCIE LTR max no snoop Latency 16
                                                    ///< Offset 244     PCIE LTR max no snoop Latency 17
                                                    ///< Offset 246     PCIE LTR max no snoop Latency 18
                                                    ///< Offset 248     PCIE LTR max no snoop Latency 19
                                                    ///< Offset 250     PCIE LTR max no snoop Latency 20
                                                    ///< Offset 252     PCIE LTR max no snoop Latency 21
                                                    ///< Offset 254     PCIE LTR max no snoop Latency 22
                                                    ///< Offset 256     PCIE LTR max no snoop Latency 23
                                                    ///< Offset 258     PCIE LTR max no snoop Latency 24
  UINT32   SerialIoDebugUart0Bar0;                  ///< Offset 260     SerialIo Hidden UART0 BAR 0
  UINT32   SerialIoDebugUart1Bar0;                  ///< Offset 264     SerialIo Hidden UART1 BAR 0
  UINT8    XHPC;                                    ///< Offset 268     Number of HighSpeed ports implemented in XHCI controller
  UINT8    XRPC;                                    ///< Offset 269     Number of USBR ports implemented in XHCI controller
  UINT8    XSPC;                                    ///< Offset 270     Number of SuperSpeed ports implemented in XHCI controller
  UINT8    XSPA;                                    ///< Offset 271     Address of 1st SuperSpeed port
  UINT32   HPTB;                                    ///< Offset 272     HPET base address
  UINT8    HPTE;                                    ///< Offset 276     HPET enable
  //SerialIo block
  UINT8    SMD[11];                                 ///< Offset 277     SerialIo controller 0 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
                                                    ///< Offset 278     SerialIo controller 1 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
                                                    ///< Offset 279     SerialIo controller 2 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
                                                    ///< Offset 280     SerialIo controller 3 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
                                                    ///< Offset 281     SerialIo controller 4 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
                                                    ///< Offset 282     SerialIo controller 5 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
                                                    ///< Offset 283     SerialIo controller 6 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
                                                    ///< Offset 284     SerialIo controller 7 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
                                                    ///< Offset 285     SerialIo controller 8 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
                                                    ///< Offset 286     SerialIo controller 9 mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
                                                    ///< Offset 287     SerialIo controller A mode (0: disabled, 1: pci, 2: acpi, 3: debug port)
  UINT8    SIR[11];                                 ///< Offset 288     SerialIo controller 0 irq number
                                                    ///< Offset 289     SerialIo controller 1 irq number
                                                    ///< Offset 290     SerialIo controller 2 irq number
                                                    ///< Offset 291     SerialIo controller 3 irq number
                                                    ///< Offset 292     SerialIo controller 4 irq number
                                                    ///< Offset 293     SerialIo controller 5 irq number
                                                    ///< Offset 294     SerialIo controller 6 irq number
                                                    ///< Offset 295     SerialIo controller 7 irq number
                                                    ///< Offset 296     SerialIo controller 8 irq number
                                                    ///< Offset 297     SerialIo controller 9 irq number
                                                    ///< Offset 298     SerialIo controller A irq number
  UINT64   SB0[11];                                 ///< Offset 299     SerialIo controller 0 BAR0
                                                    ///< Offset 307     SerialIo controller 1 BAR0
                                                    ///< Offset 315     SerialIo controller 2 BAR0
                                                    ///< Offset 323     SerialIo controller 3 BAR0
                                                    ///< Offset 331     SerialIo controller 4 BAR0
                                                    ///< Offset 339     SerialIo controller 5 BAR0
                                                    ///< Offset 347     SerialIo controller 6 BAR0
                                                    ///< Offset 355     SerialIo controller 7 BAR0
                                                    ///< Offset 363     SerialIo controller 8 BAR0
                                                    ///< Offset 371     SerialIo controller 9 BAR0
                                                    ///< Offset 379     SerialIo controller A BAR0
  UINT64   SB1[11];                                 ///< Offset 387     SerialIo controller 0 BAR1
                                                    ///< Offset 395     SerialIo controller 1 BAR1
                                                    ///< Offset 403     SerialIo controller 2 BAR1
                                                    ///< Offset 411     SerialIo controller 3 BAR1
                                                    ///< Offset 419     SerialIo controller 4 BAR1
                                                    ///< Offset 427     SerialIo controller 5 BAR1
                                                    ///< Offset 435     SerialIo controller 6 BAR1
                                                    ///< Offset 443     SerialIo controller 7 BAR1
                                                    ///< Offset 451     SerialIo controller 8 BAR1
                                                    ///< Offset 459     SerialIo controller 9 BAR1
                                                    ///< Offset 467     SerialIo controller A BAR1
  //end of SerialIo block
  UINT8    GPEN;                                    ///< Offset 475     GPIO enabled
  UINT8    SGIR;                                    ///< Offset 476     GPIO IRQ
  UINT8    RstPcieStorageInterfaceType[3];          ///< Offset 477     RST PCIe Storage Cycle Router#1 Interface Type
                                                    ///< Offset 478     RST PCIe Storage Cycle Router#2 Interface Type
                                                    ///< Offset 479     RST PCIe Storage Cycle Router#3 Interface Type
  UINT8    RstPcieStoragePmCapPtr[3];               ///< Offset 480     RST PCIe Storage Cycle Router#1 Power Management Capability Pointer
                                                    ///< Offset 481     RST PCIe Storage Cycle Router#2 Power Management Capability Pointer
                                                    ///< Offset 482     RST PCIe Storage Cycle Router#3 Power Management Capability Pointer
  UINT8    RstPcieStoragePcieCapPtr[3];             ///< Offset 483     RST PCIe Storage Cycle Router#1 PCIe Capabilities Pointer
                                                    ///< Offset 484     RST PCIe Storage Cycle Router#2 PCIe Capabilities Pointer
                                                    ///< Offset 485     RST PCIe Storage Cycle Router#3 PCIe Capabilities Pointer
  UINT16   RstPcieStorageL1ssCapPtr[3];             ///< Offset 486     RST PCIe Storage Cycle Router#1 L1SS Capability Pointer
                                                    ///< Offset 488     RST PCIe Storage Cycle Router#2 L1SS Capability Pointer
                                                    ///< Offset 490     RST PCIe Storage Cycle Router#3 L1SS Capability Pointer
  UINT8    RstPcieStorageEpL1ssControl2[3];         ///< Offset 492     RST PCIe Storage Cycle Router#1 Endpoint L1SS Control Data2
                                                    ///< Offset 493     RST PCIe Storage Cycle Router#2 Endpoint L1SS Control Data2
                                                    ///< Offset 494     RST PCIe Storage Cycle Router#3 Endpoint L1SS Control Data2
  UINT32   RstPcieStorageEpL1ssControl1[3];         ///< Offset 495     RST PCIe Storage Cycle Router#1 Endpoint L1SS Control Data1
                                                    ///< Offset 499     RST PCIe Storage Cycle Router#2 Endpoint L1SS Control Data1
                                                    ///< Offset 503     RST PCIe Storage Cycle Router#3 Endpoint L1SS Control Data1
  UINT16   RstPcieStorageLtrCapPtr[3];              ///< Offset 507     RST PCIe Storage Cycle Router#1 LTR Capability Pointer
                                                    ///< Offset 509     RST PCIe Storage Cycle Router#2 LTR Capability Pointer
                                                    ///< Offset 511     RST PCIe Storage Cycle Router#3 LTR Capability Pointer
  UINT32   RstPcieStorageEpLtrData[3];              ///< Offset 513     RST PCIe Storage Cycle Router#1 Endpoint LTR Data
                                                    ///< Offset 517     RST PCIe Storage Cycle Router#2 Endpoint LTR Data
                                                    ///< Offset 521     RST PCIe Storage Cycle Router#3 Endpoint LTR Data
  UINT16   RstPcieStorageEpLctlData16[3];           ///< Offset 525     RST PCIe Storage Cycle Router#1 Endpoint LCTL Data
                                                    ///< Offset 527     RST PCIe Storage Cycle Router#2 Endpoint LCTL Data
                                                    ///< Offset 529     RST PCIe Storage Cycle Router#3 Endpoint LCTL Data
  UINT16   RstPcieStorageEpDctlData16[3];           ///< Offset 531     RST PCIe Storage Cycle Router#1 Endpoint DCTL Data
                                                    ///< Offset 533     RST PCIe Storage Cycle Router#2 Endpoint DCTL Data
                                                    ///< Offset 535     RST PCIe Storage Cycle Router#3 Endpoint DCTL Data
  UINT16   RstPcieStorageEpDctl2Data16[3];          ///< Offset 537     RST PCIe Storage Cycle Router#1 Endpoint DCTL2 Data
                                                    ///< Offset 539     RST PCIe Storage Cycle Router#2 Endpoint DCTL2 Data
                                                    ///< Offset 541     RST PCIe Storage Cycle Router#3 Endpoint DCTL2 Data
  UINT16   RstPcieStorageRpDctl2Data16[3];          ///< Offset 543     RST PCIe Storage Cycle Router#1 RootPort DCTL2 Data
                                                    ///< Offset 545     RST PCIe Storage Cycle Router#2 RootPort DCTL2 Data
                                                    ///< Offset 547     RST PCIe Storage Cycle Router#3 RootPort DCTL2 Data
  UINT32   RstPcieStorageUniqueTableBar[3];         ///< Offset 549     RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X Table BAR
                                                    ///< Offset 553     RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X Table BAR
                                                    ///< Offset 557     RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X Table BAR
  UINT32   RstPcieStorageUniqueTableBarValue[3];    ///< Offset 561     RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X Table BAR value
                                                    ///< Offset 565     RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X Table BAR value
                                                    ///< Offset 569     RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X Table BAR value
  UINT32   RstPcieStorageUniquePbaBar[3];           ///< Offset 573     RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X PBA BAR
                                                    ///< Offset 577     RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X PBA BAR
                                                    ///< Offset 581     RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X PBA BAR
  UINT32   RstPcieStorageUniquePbaBarValue[3];      ///< Offset 585     RST PCIe Storage Cycle Router#1 Endpoint unique MSI-X PBA BAR value
                                                    ///< Offset 589     RST PCIe Storage Cycle Router#2 Endpoint unique MSI-X PBA BAR value
                                                    ///< Offset 593     RST PCIe Storage Cycle Router#3 Endpoint unique MSI-X PBA BAR value
  UINT32   RstPcieStorageRootPortNum[3];            ///< Offset 597     RST PCIe Storage Cycle Router#1 Root Port number
                                                    ///< Offset 601     RST PCIe Storage Cycle Router#2 Root Port number
                                                    ///< Offset 605     RST PCIe Storage Cycle Router#3 Root Port number
  UINT8    ExitBootServicesFlag;                    ///< Offset 609     Flag indicating Exit Boot Service, to inform SMM
  UINT32   SxMemBase;                               ///< Offset 610     Sx handler reserved MMIO base
  UINT32   SxMemSize;                               ///< Offset 614     Sx handler reserved MMIO size
  UINT8    Cio2EnabledAsAcpiDevice;                 ///< Offset 618     Cio2 Device Enabled as ACPI device
  UINT8    Cio2IrqNumber;                           ///< Offset 619     Cio2 Interrupt Number
  UINT8    ThermalDeviceAcpiEnabled;                ///< Offset 620     Thermal Device Acpi mode enabled
  UINT8    ThermalDeviceInterruptLine;              ///< Offset 621     Thermal Device IRQ number
  UINT32   XhciRsvdMemBase;                         ///< Offset 622     XHCI memory base address
  UINT8    EMH4;                                    ///< Offset 626     eMMC HS400 mode enabled
  UINT8    EMDS;                                    ///< Offset 627     eMMC Driver Strength
  UINT8    CpuSku;                                  ///< Offset 628     CPU SKU
  UINT16   IoTrapAddress[4];                        ///< Offset 629     
                                                    ///< Offset 631     
                                                    ///< Offset 633     
                                                    ///< Offset 635     
  UINT8    IoTrapStatus[4];                         ///< Offset 637     
                                                    ///< Offset 638     
                                                    ///< Offset 639     
                                                    ///< Offset 640     
  UINT16   PMBS;                                    ///< Offset 641     ACPI IO BASE address
  UINT32   PWRM;                                    ///< Offset 643     PWRM MEM BASE address
} PCH_NVS_AREA;

#pragma pack(pop)
#endif
