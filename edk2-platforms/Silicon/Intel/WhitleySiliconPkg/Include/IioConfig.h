/** @file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IIO_CONFIG_H
#define _IIO_CONFIG_H
#include <ConfigBlock/TraceHubConfig.h>

#pragma pack(1) //to align members on byte boundary
/**
The Silicon Policy allows the platform code to publish a set of configuration
information that the RC drivers will use to configure the silicon hardware.
**/
typedef struct {
  UINT8   ReservedAJ[MAX_TOTAL_PORTS];
  UINT8   ReservedAK[MAX_TOTAL_PORTS];
  UINT8   ReservedAL[MAX_TOTAL_PORTS];
  UINT8   ReservedAM[MAX_TOTAL_PORTS];
  UINT8   ReservedAN[MAX_TOTAL_PORTS];
  UINT8   ReservedAO[MAX_TOTAL_PORTS];
  UINT8   ReservedAP[MAX_TOTAL_PORTS];
  UINT8   ReservedAQ[MAX_TOTAL_PORTS];
  UINT8   ReservedAR[MAX_TOTAL_PORTS];
  UINT8   ReservedAS[MAX_TOTAL_PORTS];
  UINT8   ReservedAT[MAX_TOTAL_PORTS];
  UINT8   ReservedAU[MAX_TOTAL_PORTS];
  UINT8   ReservedAV[MAX_TOTAL_PORTS];
  UINT8   ReservedAW[MAX_TOTAL_PORTS];
  UINT8   ReservedAX[MAX_TOTAL_PORTS];
  UINT8   ReservedAY[MAX_TOTAL_PORTS];
  UINT8   ReservedE[MAX_TOTAL_PORTS];
  UINT8   ReservedF[MAX_TOTAL_PORTS];
  UINT8   ReservedG[MAX_TOTAL_PORTS];
  UINT8   ReservedAZ[MAX_TOTAL_PORTS];
  UINT8   ReservedBA[MAX_TOTAL_PORTS];

  UINT8   PciePortClkGateEnable[MAX_TOTAL_PORTS];
  UINT8   ExtendedSync[MAX_TOTAL_PORTS];
  UINT8   PciePortEnable[MAX_TOTAL_PORTS];
  UINT8   PcieMaxPayload[MAX_TOTAL_PORTS];
  UINT8   PcieAspm[MAX_SOCKET][NUMBER_PORTS_PER_SOCKET];   // On Setup
  UINT8   PcieTxRxDetPoll[MAX_SOCKET][NUMBER_PORTS_PER_SOCKET];
  UINT8   PciePortLinkSpeed[MAX_TOTAL_PORTS];
  UINT8   PciePtm;
  UINT8   PcieHotPlugEnable;
  UINT8   PCIe_LTR;
  UINT8   PcieUnsupportedRequests[MAX_TOTAL_PORTS];
  UINT8   RpCorrectableErrorEsc[MAX_SOCKET];           //on Setup
  UINT8   RpUncorrectableNonFatalErrorEsc[MAX_SOCKET]; //on Setup
  UINT8   RpUncorrectableFatalErrorEsc[MAX_SOCKET];    //on Setup
  UINT8   ComplianceMode[MAX_TOTAL_PORTS];
} IIO_PCIE_CONFIG_DATA;

typedef struct _IIO_RETIMER_DATA {
  UINT8       RetimerPresent;             // eq. 0 => there is no retimer data
  UINT32      GlParamReg0;                // current value of Global Param. Reg. 0
  UINT32      GlParamReg1;                // current value of Global Param. Reg. 1
  UINT32      PseudoPort0Reg2;            // current value of Pseudo Port0 Reg. 2
  UINT32      PseudoPort1Reg2;            // current value of Pseudo Port1 Reg. 2
  UINT32      GlParmReg0Override;         // value to write to Global Param. Reg. 0
  UINT32      PseudoPort0Reg2Override;    // value to write to Pseudo Port0 Reg. 2
  UINT32      PseudoPort1Reg2Override;    // value to write to Pseudo Port1 Reg. 2
} IIO_RETIMER_DATA;

typedef struct {
/**
==================================================================================================
==================================      VTd Setup Options       ==================================
==================================================================================================
**/
    UINT8    VTdSupport;
    UINT8    DmaCtrlOptIn;
    UINT8    InterruptRemap;
    UINT8    PostedInterrupt;
    UINT8    ATS;
    UINT8    CoherencySupport;
    UINT8    VtdAcsWa;
    UINT8    VtdPciAcsCtl;                // Value to set in PCIe ACSCTL register if reqeusted

/**
==================================================================================================
==================================      PCIE Setup Options       ==================================
==================================================================================================
**/
    // Platform data needs to update these PCI Configuration settings
    UINT8    SLOTEIP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];          // Electromechanical Interlock Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B17)
    UINT8    SLOTHPCAP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];        // Slot Hot Plug capable - Slot Capabilities (D0-10 / F0 / R0xA4 / B6)
    UINT8    SLOTHPSUP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];        // Hot Plug surprise supported - Slot Capabilities (D0-10 / F0 / R0xA4 / B5)
    UINT8    SLOTPIP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];          // Power Indicator Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B4)
    UINT8    SLOTAIP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];          // Attention Inductor Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B3)
    UINT8    SLOTMRLSP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];        // MRL Sensor Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B2)
    UINT8    SLOTPCP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];          // Power Controller Present - Slot Capabilities (D0-10 / F0 / R0xA4 /B1)
    UINT8    SLOTABP[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];          // Attention Button Present - Slot Capabilities (D0-10 / F0 / R0xA4 / B0)
    UINT8    PcieSSDCapable[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];   // Indicate if Port will PcieSSD capable.

    UINT8    PcieHotPlugOnPort[MAX_SOCKET][NUMBER_PORTS_PER_SOCKET];  // manual override of hotplug for port

    // General PCIE Configuration
    UINT8   PcieSubSystemMode[MAX_SOCKET][MAX_IOU_PER_SOCKET];  //on Setup

    UINT8   CompletionTimeoutGlobal;
    UINT8   CompletionTimeoutGlobalValue;
    UINT8   CompletionTimeout[MAX_SOCKET];  // On Setup
    UINT8   CompletionTimeoutValue[MAX_SOCKET]; // On Setup
    UINT8   CoherentReadPart;
    UINT8   CoherentReadFull;
    UINT8   PcieGlobalAspm;
    UINT8   StopAndScream;
    UINT8   SnoopResponseHoldOff;
    //
    // PCIE capability
    //
    UINT8   PcieExtendedTagField;
    UINT8   Pcie10bitTag;
    UINT8   PCIe_AtomicOpReq;
    UINT8   PcieMaxReadRequestSize;


    // mixc PCIE configuration
    UINT8    PcieLinkDis[MAX_TOTAL_PORTS];          // On Setup
    UINT8    PcieCommonClock[MAX_TOTAL_PORTS];  // On Setup
    UINT8    PcieDState[MAX_TOTAL_PORTS];           // On Setup
    UINT8    PcieL0sLatency[MAX_TOTAL_PORTS];       //On Setup
    UINT8    PcieL1Latency[MAX_TOTAL_PORTS];        //On Setup
    UINT8    MsiEn[MAX_TOTAL_PORTS];                // On Setup
    UINT8    IODC[MAX_TOTAL_PORTS];                 // On Setup
    //
    // VPP Control
    //
    BOOLEAN  VppEnabled[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];        // 00 -- Disable, 01 -- Enable  //no setup option defined- aj
    UINT8    VppPort[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];          // 00 -- Port 0, 01 -- Port 1   //no setup option defined- aj
    UINT8    VppAddress[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];       // 01-07 for SMBUS address of Vpp   //no setup option defined- aj

    //
    // Mux and channel for segment
    //
    UINT8    MuxAddress[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];        // SMBUS address of MUX              //no setup option defined
    UINT8    ChannelID[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];      // 00 -- channel 0, 01 -- channel 1  //no setup option defined

    //
    // PCIE setup options for Link Control2
    //
    UINT8    PciePortLinkMaxWidth[MAX_TOTAL_PORTS]; // On Setup
    UINT8    DeEmphasis[MAX_TOTAL_PORTS];       // On Setup

    //
    // PCIE RAS (Errors)
    //
    UINT8   Serr;
    UINT8   Perr;
    UINT8   IioErrorEn;
    UINT8   LerEn;
    UINT8   WheaPcieErrInjEn;

    //
    // PciePll
    //
    UINT8    PciePllSsc;                        //On Setup

    //
    // PCIE  Link Training Ctrl
    //
    UINT16   DelayBeforePCIeLinkTraining;       //On Setup

    //
    // Retimers related config
    //
    IIO_RETIMER_DATA Retimer[MAX_SOCKET][MAX_IIO_STACK][MAX_RETIMERS_PER_STACK];
    BOOLEAN  SkipRetimersDetection;  // Skip detection of retimers in UBA code

/**
==================================================================================================
==================================    Crystal Beach 3 Setup Options    ===========================
==================================================================================================
**/
      UINT8                 Cb3DmaEn[TOTAL_CB3_DEVICES];        // on setup
      UINT8                 Cb3NoSnoopEn[TOTAL_CB3_DEVICES];    // on setup
      UINT8                 DisableTPH;
      UINT8                 PrioritizeTPH;
      UINT8                 CbRelaxedOrdering;
      UINT8                 CbDmaMultiCastEnable;               // MultiCastEnable test enable

      UINT8                 DsaEn[NUM_DSA*MAX_SOCKET];                           // on setup
      UINT8                 IaxEn[NUM_IAX*MAX_SOCKET];                           // on setup
      UINT8                 CpmEn[NUM_CPM*MAX_SOCKET];                           // on setup
      UINT8                 HqmEn[NUM_HQM*MAX_SOCKET];                           // on setup

/**
==================================================================================================
==================================    MISC IOH Setup Options            ==========================
==================================================================================================
**/

    // The following are for hiding each individual device and function
    UINT8   PEXPHIDE[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];  // Hide any of the DMI or PCIE devices - SKT 0,1,2,3; Device 0-10 PRD
    UINT8   PCUF6Hide;                            // Hide Device PCU Device 30, Function 6
    UINT8   EN1K;                                 // Enable/Disable 1K granularity of IO for P2P bridges 0:20:0:98 bit 2
    UINT8   DualCvIoFlow;                         // Dual CV IO Flow
    UINT8   PcieBiosTrainEnable;                  // Used as a work around for A0 PCIe
    UINT8   MultiCastEnable;                      // MultiCastEnable test enable
    UINT8   McastBaseAddrRegion;                  // McastBaseAddrRegion
    UINT8   McastIndexPosition;                   // McastIndexPosition
    UINT8   McastNumGroup;                        // McastNumGroup
    UINT8   MctpEn[MAX_TOTAL_PORTS];              // Enable/Disable MCTP for each Root Port

    UINT8   LegacyVgaSoc;
    UINT8   LegacyVgaStack;

    UINT8   HidePEXPMenu[MAX_TOTAL_PORTS];          // to suppress /display the PCIe port menu

    BOOLEAN PoisonMmioReadEn[MAX_SOCKET][MAX_IIO_STACK]; // on setup

/**
==================================================================================================
==================================    NTB Related Setup Options ==========================
==================================================================================================
**/
  UINT8   NtbPpd[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];                   //on setup option
  UINT8   NtbBarSizeOverride[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];       //on setup option
  UINT8   NtbSplitBar[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];             //on setup option
  UINT8   NtbBarSizeImBar1[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];         //on setup option
  UINT8   NtbBarSizeImBar2[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];         //on setup option
  UINT8   NtbBarSizeImBar2_0[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];          //on setup option
  UINT8   NtbBarSizeImBar2_1[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];          //on setup option
  UINT8   NtbBarSizeEmBarSZ1[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];         //on setup option
  UINT8   NtbBarSizeEmBarSZ2[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];         //on setup option
  UINT8   NtbBarSizeEmBarSZ2_0[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];          //on setup option
  UINT8   NtbBarSizeEmBarSZ2_1[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];          //on setup option
  UINT8   NtbXlinkCtlOverride[MAX_SOCKET*NUMBER_NTB_PORTS_PER_SOCKET];      //on setup option

  UINT8   NtbLinkBiosTrainEn;                                                   // on setup option
/**
==================================================================================================
==================================    VMD Related Setup Options ==========================
==================================================================================================
**/
  UINT8   VMDEnabled[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDPortEnable[MAX_SOCKET][NUMBER_PORTS_PER_SOCKET];
  UINT8   VMDHotPlugEnable[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDCfgBarSz[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDCfgBarAttr[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDMemBarSz1[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDMemBar1Attr[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDMemBarSz2[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDMemBar2Attr[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];
  UINT8   VMDPchPortEnable[MAX_SOCKET][MAX_VMD_ROOTPORTS_PER_PCH];
  UINT8   VMDDirectAssign[MAX_SOCKET][MAX_VMD_STACKS_PER_SOCKET];

  /**
  ==================================================================================================
  ==================================    PcieSSD Related Setup Options ==========================
  ==================================================================================================
  **/
  UINT8   PcieAICEnabled[MAX_SOCKET*MAX_STACKS_PER_SOCKET];                         // Indicate if PCIE AIC Device will be connected behind an specific IOUx
  UINT8   PcieAICPortEnable[MAX_SOCKET*NUMBER_PORTS_PER_SOCKET];
  UINT8   PcieAICHotPlugEnable[MAX_SOCKET*MAX_STACKS_PER_SOCKET];


/**
==================================================================================================
==================================    Gen3 Related Setup Options ==========================
==================================================================================================
**/

  //PCIE Global Option
  UINT8   NoSnoopRdCfg;                             //on Setup
  UINT8   NoSnoopWrCfg;                             //on Setup
  UINT8   MaxReadCompCombSize;                      //on Setup
  UINT8   ProblematicPort;                          //on Setup
  UINT8   DmiAllocatingFlow;                        //on Setup
  UINT8   PcieAllocatingFlow;                       //on Setup
  UINT8   PcieAcpiHotPlugEnable;                    //on Setup
  BOOLEAN PcieLowLatencyRetimersEnabled;
  UINT8   HaltOnDmiDegraded;                        //on Setup
  UINT8   GlobalPme2AckTOCtrl;                      //on Setup

  UINT8   PcieSlotOprom1;                           //On Setup
  UINT8   PcieSlotOprom2;                           //On Setup
  UINT8   PcieSlotOprom3;                           //On Setup
  UINT8   PcieSlotOprom4;                           //On Setup
  UINT8   PcieSlotOprom5;                           //On Setup
  UINT8   PcieSlotOprom6;                           //On Setup
  UINT8   PcieSlotOprom7;                           //On Setup
  UINT8   PcieSlotOprom8;                           //On Setup
  UINT8   PcieSlotItemCtrl;                         //On Setup
  UINT8   PcieRelaxedOrdering;                      //On Setup
  UINT8   PciePhyTestMode;                          //On setup
  UINT8   PcieEnqCmdSupport;                        //On setup
/**
==================================================================================================
==================================    IOAPIC Related Setup Options ==========================
==================================================================================================
**/

  UINT8   DevPresIoApicIio[TOTAL_IIO_STACKS];
/**
==================================================================================================
==================================    Security Related Setup Options ==========================
==================================================================================================
**/
  UINT8   LockChipset;
  UINT8   PeciInTrustControlBit;
  UINT8   PeciAgtLegacyTrustBit;
  UINT8   PeciAgtSmbusTrustBit;
  UINT8   PeciAgtIeTrustBit;
  UINT8   PeciAgtGenericTrustBit;
  UINT8   PeciAgtEspiTrustBit;
  UINT8   ProcessorX2apic;
  UINT8   ProcessorMsrLockControl;
  UINT8   Xppdef;
  UINT8   Pci64BitResourceAllocation;
  UINT8   Imr2SupportEnable;
/**
==================================================================================================
==================================    Reserved Setup Options ==========================
==================================================================================================
**/
  UINT8   ReservedQ;                     // On Setup
  UINT8   ReservedR;
  UINT8   ReservedS;                // On Setup
  UINT8   ReservedT;            // On Setup
  UINT8   ReservedU;           // On Setup
  UINT8   ReservedV;            // On Setup
  UINT8   ReservedW;               // On Setup
  UINT8   ReservedX;                  // On Setup
  UINT8   ReservedY;            // On Setup
  UINT8   ReservedZ;        // On Setup
  UINT8   ReservedAA;       // On Setup
  UINT8   ReservedAB;          // On Setup

  UINT32  ReservedAC[MAX_SOCKET][NUM_DEVHIDE_UNCORE_STACKS][NUM_DEVHIDE_REGS_PER_STACK];
  UINT32  ReservedAD[MAX_SOCKET][NUM_DEVHIDE_IIO_STACKS][NUM_DEVHIDE_REGS_PER_STACK];

  UINT8   ReservedAE[MAX_TOTAL_PORTS];  // On Setup

  UINT8   ReservedAF[MAX_TOTAL_PORTS];
  UINT8   ReservedAG[MAX_TOTAL_PORTS];  // On Setup
  BOOLEAN ReservedAH;                // On Setup


/**
==================================================================================================
======================    IIO Global Performance Tuner Related Setup Options =====================
==================================================================================================
**/
  UINT8  PerformanceTuningMode;

/**
=================================================================================================
======================    PCI-E Data Link Feature Exchange Enable  ===============================
==================================================================================================
**/
  UINT8   PcieDataLinkFeatureExchangeEnable[MAX_TOTAL_PORTS];    //On Setup

/**
==================================================================================================
======================    IIO Trace Hub struct for setup options =================================
==================================================================================================
**/
  TRACE_HUB_CONFIG    CpuTraceHubConfig[MAX_SOCKET][NUMBER_TRACE_HUB_PER_SOCKET];

  UINT8   SLOTIMP[MAX_TOTAL_PORTS];
  UINT8   SLOTSPLS[MAX_TOTAL_PORTS];
  UINT8   SLOTSPLV[MAX_TOTAL_PORTS];
  UINT16  SLOTPSP[MAX_TOTAL_PORTS];
  UINT8   ConfigIOU[MAX_SOCKET][MAX_IOU_PER_SOCKET];         // 00-x4x4x4x4, 01-x4x4x8NA, 02-x8NAx4x4, 03-x8NAx8NA, 04-x16 (P1p2p3p4)

  UINT8   EOI[MAX_TOTAL_PORTS];
  UINT8   MSIFATEN[MAX_TOTAL_PORTS];
  UINT8   MSINFATEN[MAX_TOTAL_PORTS];
  UINT8   MSICOREN[MAX_TOTAL_PORTS];
  UINT8   ACPIPMEn[MAX_TOTAL_PORTS];
  UINT8   DISL0STx[MAX_TOTAL_PORTS];
  UINT8   P2PRdDis[MAX_TOTAL_PORTS];
  UINT8   DisPMETOAck[MAX_TOTAL_PORTS];
  UINT8   ACPIHP[MAX_TOTAL_PORTS];
  UINT8   ACPIPM[MAX_TOTAL_PORTS];
  UINT8   AltAttenTable[MAX_TOTAL_PORTS];
  UINT8   SRIS[MAX_TOTAL_PORTS];
  UINT8   TXEQ[MAX_TOTAL_PORTS];
  UINT8   EcrcGenEn[MAX_TOTAL_PORTS];
  UINT8   EcrcChkEn[MAX_TOTAL_PORTS];
  UINT8   SERRE[MAX_TOTAL_PORTS];

  //
  // Sierra Peak (SPK)
  //
  UINT8   SierraPeakMemBufferSize[MAX_SOCKET];        // on setup
  IIO_PCIE_CONFIG_DATA  IioPcieConfig;

  UINT32  VtdDisabledBitmask[MAX_SOCKET];
} IIO_CONFIG;
#pragma pack()

#endif // _IIO_CONFIG_H
