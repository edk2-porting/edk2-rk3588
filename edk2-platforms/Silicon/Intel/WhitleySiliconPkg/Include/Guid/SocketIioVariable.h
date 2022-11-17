/** @file
  Data format for Universal Data Structure

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SOCKET_IIO_VARIABLE_H_
#define _SOCKET_IIO_VARIABLE_H_

#include <UncoreCommonIncludes.h>

extern EFI_GUID gEfiSocketIioVariableGuid;
#define SOCKET_IIO_CONFIGURATION_NAME L"SocketIioConfig"

/*
  These vars are not used anywhere - it is left here for reference as this is used in comments below.
  Number must be used directly because of VFR compiler...

  #ifndef MAX_STACKS_PER_SOCKET_VAR
      #define MAX_STACKS_PER_SOCKET_VAR   6
      #define MAX_PORTS_PER_SOCKET_VAR    21
    #endif
  #endif
*/

#if MAX_SOCKET == 1
  #define TOTAL_VMD_STACKS_VAR        6             // MAX_STACKS_PER_SOCKET_VAR *  MAX_SOCKET

  #define TOTAL_NTB_PORTS_VAR         5             // NUMBER_NTB_PORTS_PER_SOCKET * MAX_SOCKET
  #define TOTAL_IOU_VAR               5             // MAX_IOU_PER_SOCKET * MAX_SOCKET
  #define TOTAL_VMD_PCH_ROOTPORTS_VAR 20            // MAX_PCH_ROOTPORTS * MAX_SOCKET
  #define TOTAL_RETIMERS_VAR          12            // MAX_SOCKET * MAX_STACKS_PER_SOCKET_VAR * MAX_RETIMERS_PER_STACK (ICX-SP related)

    #define TOTAL_NPK_VAR             1             // NUMBER_TRACE_HUB_PER_SOCKET * MAX_SOCKET
    #define TOTAL_PORTS_VAR           21            // MAX_PORTS_PER_SOCKET_VAR  *  MAX_SOCKET
    #define TOTAL_VMD_PORTS_VAR       24            // MAX_PORTS_PER_STACK * MAX_STACKS_PER_SOCKET_VAR * MAX_SOCKET
    #define TOTAL_DSA_VAR             1             // 1_DSA_PER_SOCKET * MAX_SOCKET
    #define TOTAL_IAX_VAR             1             // 1_IAX_PER_SOCKET * MAX_SOCKET
    #define TOTAL_CPM_VAR             1             // 1_CPM_PER_SOCKET * MAX_SOCKET
    #define TOTAL_HQM_VAR             1             // 1_HQM_PER_SOCKET * MAX_SOCKET

#endif

#if MAX_SOCKET == 4
  #define TOTAL_VMD_STACKS_VAR        24            // MAX_STACKS_PER_SOCKET_VAR *  MAX_SOCKET
  #define TOTAL_NTB_PORTS_VAR         20            // NUMBER_NTB_PORTS_PER_SOCKET * MAX_SOCKET
  #define TOTAL_IOU_VAR               20            // MAX_IOU_PER_SOCKET * MAX_SOCKET
  #define TOTAL_VMD_PCH_ROOTPORTS_VAR 80            // MAX_PCH_ROOTPORTS * MAX_SOCKET
  #define TOTAL_RETIMERS_VAR          48            // MAX_SOCKET * MAX_STACKS_PER_SOCKET_VAR * MAX_RETIMERS_PER_STACK (ICX-SP related)
    #define TOTAL_NPK_VAR             4             // NUMBER_TRACE_HUB_PER_SOCKET * MAX_SOCKET
    #define TOTAL_PORTS_VAR           84            // MAX_PORTS_PER_SOCKET_VAR  *  MAX_SOCKET
    #define TOTAL_VMD_PORTS_VAR       96            // MAX_PORTS_PER_STACK * MAX_STACKS_PER_SOCKET_VAR * MAX_SOCKET
    #define TOTAL_DSA_VAR             4             // 1_DSA_PER_SOCKET * MAX_SOCKET
    #define TOTAL_IAX_VAR             4             // 1_IAX_PER_SOCKET * MAX_SOCKET
    #define TOTAL_CPM_VAR             4             // 1_CPM_PER_SOCKET * MAX_SOCKET
    #define TOTAL_HQM_VAR             4             // 1_HQM_PER_SOCKET * MAX_SOCKET
#endif

#if MAX_SOCKET == 2
  #define TOTAL_VMD_STACKS_VAR        12            // MAX_STACKS_PER_SOCKET_VAR *  MAX_SOCKET
  #define TOTAL_NTB_PORTS_VAR         10            // NUMBER_NTB_PORTS_PER_SOCKET * MAX_SOCKET
  #define TOTAL_IOU_VAR               10            // MAX_IOU_PER_SOCKET * MAX_SOCKET
  #define TOTAL_VMD_PCH_ROOTPORTS_VAR 40            // MAX_PCH_ROOTPORTS * MAX_SOCKET
  #define TOTAL_RETIMERS_VAR          24            // MAX_SOCKET * MAX_STACKS_PER_SOCKET_VAR * MAX_RETIMERS_PER_STACK (ICX-SP related)
    #define TOTAL_NPK_VAR             2             // NUMBER_TRACE_HUB_PER_SOCKET * MAX_SOCKET
    #define TOTAL_PORTS_VAR           42            // MAX_PORTS_PER_SOCKET_VAR  *  MAX_SOCKET
    #define TOTAL_VMD_PORTS_VAR       48            // MAX_PORTS_PER_STACK * MAX_STACKS_PER_SOCKET_VAR * MAX_SOCKET
    #define TOTAL_DSA_VAR             2             // 1_DSA_PER_SOCKET * MAX_SOCKET
    #define TOTAL_IAX_VAR             2             // 1_IAX_PER_SOCKET * MAX_SOCKET
    #define TOTAL_CPM_VAR             2             // 1_CPM_PER_SOCKET * MAX_SOCKET
    #define TOTAL_HQM_VAR             2             // 1_HQM_PER_SOCKET * MAX_SOCKET
#endif

#if MAX_SOCKET > 4
  #define TOTAL_VMD_PCH_ROOTPORTS_VAR 160           // MAX_PCH_ROOTPORTS * MAX_SOCKET
  #define TOTAL_NTB_PORTS_VAR         40            // NUMBER_NTB_PORTS_PER_SOCKET * MAX_SOCKET
  #define TOTAL_IOU_VAR               40            // MAX_IOU_PER_SOCKET * MAX_SOCKET
  #define TOTAL_VMD_STACKS_VAR        48            // MAX_STACKS_PER_SOCKET_VAR *  MAX_SOCKET
  #define TOTAL_RETIMERS_VAR          96            // MAX_SOCKET * MAX_STACKS_PER_SOCKET_VAR * MAX_RETIMERS_PER_STACK (ICX-SP related)
    #define TOTAL_NPK_VAR             8             // NUMBER_TRACE_HUB_PER_SOCKET * MAX_SOCKET
    #define TOTAL_PORTS_VAR           168           // MAX_PORTS_PER_SOCKET_VAR  *  MAX_SOCKET
    #define TOTAL_VMD_PORTS_VAR       192           // MAX_PORTS_PER_STACK * MAX_STACKS_PER_SOCKET_VAR * MAX_SOCKET
    #define TOTAL_DSA_VAR             MAX_SOCKET    // 1_DSA_PER_SOCKET * MAX_SOCKET
    #define TOTAL_IAX_VAR             MAX_SOCKET    // 1_IAX_PER_SOCKET * MAX_SOCKET
    #define TOTAL_CPM_VAR             MAX_SOCKET    // 1_CPM_PER_SOCKET * MAX_SOCKET
    #define TOTAL_HQM_VAR             MAX_SOCKET    // 1_HQM_PER_SOCKET * MAX_SOCKET
#endif

#pragma pack(1)

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
/**
==================================================================================================
==================================      PCIE Setup Options       ==================================
==================================================================================================
**/
    // Vars used to configure (disable/greyout) parts of setup menu
    UINT8   IioPresent[MAX_SOCKET];
    UINT8   IioStackPresent[TOTAL_IIO_STACKS];  // based on sysInfo data
    UINT8   PchPresentOnStack[MAX_SOCKET];      // stub for multiPCH
    UINT8   RetimerPresent[TOTAL_RETIMERS_VAR]; // retimer detected in stack

    UINT8   VtdAcsWa;

    // Platform data needs to update these PCI Configuration settings
    UINT8    PcieHotPlugOnPort[TOTAL_PORTS_VAR];
    UINT8    SLOTHPSUP[TOTAL_PORTS_VAR];        // Hot Plug surprise supported - Slot Capabilities (D0-10 / F0 / R0xA4 / B5)

    // General PCIE Configuration
    UINT8   ConfigIOU0[MAX_SOCKET];
    UINT8   ConfigIOU1[MAX_SOCKET];
    UINT8   ConfigIOU2[MAX_SOCKET];
    UINT8   ConfigIOU3[MAX_SOCKET];
    UINT8   ConfigIOU4[MAX_SOCKET];
    UINT8   ConfigIOU5[MAX_SOCKET];
    UINT8   ConfigIOU6[MAX_SOCKET];
    UINT8   ConfigIOU7[MAX_SOCKET];
    UINT8   PcieSubSystemMode[TOTAL_IOU_VAR];
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
    UINT8   PCIe_LTR;
    UINT8   PcieExtendedTagField;
    UINT8   Pcie10bitTag;
    UINT8   PCIe_AtomicOpReq;
    UINT8   PcieMaxReadRequestSize;
    UINT8   PciePtm;


    UINT8   RpCorrectableErrorEsc[MAX_SOCKET];           //on Setup
    UINT8   RpUncorrectableNonFatalErrorEsc[MAX_SOCKET]; //on Setup
    UINT8   RpUncorrectableFatalErrorEsc[MAX_SOCKET];    //on Setup

    // mixc PCIE configuration
    UINT8    PcieLinkDis[TOTAL_PORTS_VAR];            // On Setup
    UINT8    PcieAspm[TOTAL_PORTS_VAR];               // On Setup
    UINT8    PcieCommonClock[TOTAL_PORTS_VAR];        // On Setup
    UINT8    PcieMaxPayload[TOTAL_PORTS_VAR];         // On Setup PRD
    UINT8    PcieDState[TOTAL_PORTS_VAR];             // On Setup
    UINT8    PcieL1Latency[TOTAL_PORTS_VAR];          // On Setup
    UINT8    MsiEn[TOTAL_PORTS_VAR];                  // On Setup
    UINT8    ExtendedSync[TOTAL_PORTS_VAR];           // On Setup
    UINT8    PciePortEnable[TOTAL_PORTS_VAR];        // On Setup
    UINT8    IODC[TOTAL_PORTS_VAR];                   // On Setup
    UINT8    MctpEn[TOTAL_PORTS_VAR];                 // On Setup

    //
    // PCIE setup options for Link Control2
    //
    UINT8    PciePortLinkSpeed[TOTAL_PORTS_VAR];    //on Setup
    UINT8    ComplianceMode[TOTAL_PORTS_VAR];       // On Setup  PRD
    UINT8    PciePortLinkMaxWidth[TOTAL_PORTS_VAR]; // On Setup
    UINT8    DeEmphasis[TOTAL_PORTS_VAR];           // On Setup

    //
    // PCIE setup options for MISCCTRLSTS
    //
    UINT8    EOI[TOTAL_PORTS_VAR];              // On Setup
    UINT8    MSIFATEN[TOTAL_PORTS_VAR];         //On Setup.
    UINT8    MSINFATEN[TOTAL_PORTS_VAR];        //On Setup.
    UINT8    MSICOREN[TOTAL_PORTS_VAR];         //On Setup.
    UINT8    ACPIPMEn[TOTAL_PORTS_VAR];         //On Setup
    UINT8    P2PRdDis[TOTAL_PORTS_VAR];         //On Setup Peer 2 peer
    UINT8    DisPMETOAck[TOTAL_PORTS_VAR];      //On Setup
    UINT8    ACPIHP[TOTAL_PORTS_VAR];           //On Setup
    UINT8    ACPIPM[TOTAL_PORTS_VAR];           //On Setup
    UINT8    SRIS[TOTAL_PORTS_VAR];             //On Setup
    UINT8    TXEQ[TOTAL_PORTS_VAR];             //On Setup
    UINT8    SERRE[TOTAL_PORTS_VAR];            //On Setup
    //
    // PCIE RAS (Errors)
    //

    UINT8   PcieUnsupportedRequests[TOTAL_PORTS_VAR];   // Unsupported Request per-port option

    //
    // PCIE  Link Training Ctrl
    //


    //
    // North Peak (NPK)
    //

    UINT8   NorthTraceHubMode[TOTAL_NPK_VAR];
    UINT32  NorthTraceHubMemReg0Size[TOTAL_NPK_VAR];
    UINT32  NorthTraceHubMemReg1Size[TOTAL_NPK_VAR];

    //
    // Sierra Peak (SPK)
    //
    UINT8   SierraPeakMemBufferSize[MAX_SOCKET];

    //
    // MMIO poison enabling per stack
    //
    UINT8   PoisonMmioReadEn[TOTAL_IIO_STACKS]; // on setup
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
      UINT8                 CbDmaMultiCastEnable;               // CbDmaMultiCastEnable test enable

      UINT8                 DsaEn[TOTAL_DSA_VAR];               // on setup
      UINT8                 IaxEn[TOTAL_IAX_VAR];               // on setup
      UINT8                 CpmEn[TOTAL_CPM_VAR];               // on setup
      UINT8                 HqmEn[TOTAL_HQM_VAR];               // on setup

/**
==================================================================================================
==================================    MISC IOH Setup Options            ==========================
==================================================================================================
**/

    // The following are for hiding each individual device and function
    UINT8   PEXPHIDE[TOTAL_PORTS_VAR];            // Hide any of the DMI or PCIE devices - SKT 0,1,2,3; Device 0-10 PRD
    UINT8   DevPresIoApicIio[TOTAL_IIO_STACKS];
                                                     // Hide IOAPIC Device 5, Function 4
    UINT8   PCUF6Hide;                            // Hide Device PCU Device 30, Function 6
    UINT8   EN1K;                                 // Enable/Disable 1K granularity of IO for P2P bridges 0:20:0:98 bit 2
    UINT8   DualCvIoFlow;                         // Dual CV IO Flow
    UINT8   Xppdef;
    UINT8   Pci64BitResourceAllocation;
    UINT8   PcieBiosTrainEnable;                  // Used as a work around for A0 PCIe
    UINT8   MultiCastEnable;                      // MultiCastEnable test enable
    UINT8   McastBaseAddrRegion;                  // McastBaseAddrRegion
    UINT8   McastIndexPosition;                   // McastIndexPosition
    UINT8   McastNumGroup;                        // McastNumGroup


    UINT8   HidePEXPMenu[TOTAL_PORTS_VAR];          // to suppress /display the PCIe port menu

/**
==================================================================================================
==================================    NTB Related Setup Options ==========================
==================================================================================================
**/
    UINT8   NtbPpd[TOTAL_NTB_PORTS_VAR];                   //on setup option
    UINT8   NtbBarSizeOverride[TOTAL_NTB_PORTS_VAR];       //on setup option
    UINT8   NtbSplitBar[TOTAL_NTB_PORTS_VAR];             //on setup option
    UINT8   NtbBarSizeImBar1[TOTAL_NTB_PORTS_VAR];         //on setup option
    UINT8   NtbBarSizeImBar2[TOTAL_NTB_PORTS_VAR];         //on setup option
    UINT8   NtbBarSizeImBar2_0[TOTAL_NTB_PORTS_VAR];          //on setup option
    UINT8   NtbBarSizeImBar2_1[TOTAL_NTB_PORTS_VAR];          //on setup option
    UINT8   NtbBarSizeEmBarSZ1[TOTAL_NTB_PORTS_VAR];         //on setup option
    UINT8   NtbBarSizeEmBarSZ2[TOTAL_NTB_PORTS_VAR];         //on setup option
    UINT8   NtbBarSizeEmBarSZ2_0[TOTAL_NTB_PORTS_VAR];          //on setup option
    UINT8   NtbBarSizeEmBarSZ2_1[TOTAL_NTB_PORTS_VAR];          //on setup option
    UINT8   NtbXlinkCtlOverride[TOTAL_NTB_PORTS_VAR];      //on setup option

    UINT8   NtbLinkBiosTrainEn;
/**
==================================================================================================
==================================    VMD Related Setup Options ==========================
==================================================================================================
**/
  UINT8   VMDEnabled[TOTAL_VMD_STACKS_VAR];                 // indicates if VMD is enabled on given stack
  UINT8   VMDPortEnable[TOTAL_VMD_PORTS_VAR];               // indicated if VMD is enabled on given port is enabled
  UINT8   VMDPchPortAllowed[TOTAL_VMD_PCH_ROOTPORTS_VAR];   // indicates if VMD CAN BE enabled on given PCH Rp
  UINT8   VMDPchPortEnable[TOTAL_VMD_PCH_ROOTPORTS_VAR];    // indicates if VMD IS enabled on given PCH Rp
  UINT8   VMDHotPlugEnable[TOTAL_VMD_STACKS_VAR];
  UINT8   VMDCfgBarSz[TOTAL_VMD_STACKS_VAR];
  UINT8   VMDCfgBarAttr[TOTAL_VMD_STACKS_VAR];
  UINT8   VMDMemBarSz1[TOTAL_VMD_STACKS_VAR];
  UINT8   VMDMemBar1Attr[TOTAL_VMD_STACKS_VAR];
  UINT8   VMDMemBarSz2[TOTAL_VMD_STACKS_VAR];
  UINT8   VMDMemBar2Attr[TOTAL_VMD_STACKS_VAR];
  UINT8   VMDDirectAssign[TOTAL_VMD_STACKS_VAR];

  /**
  ==================================================================================================
  ==================================    PCIe SSD Related Setup Options ==========================
  ==================================================================================================
  **/

    UINT8   PcieAICEnabled[TOTAL_VMD_STACKS_VAR];
    UINT8   PcieAICPortEnable[TOTAL_PORTS_VAR];
    UINT8   PcieAICHotPlugEnable[TOTAL_VMD_STACKS_VAR];


    /**
    ==================================================================================================
    ==================================    Retimers Related Setup Options ==========================
    ==================================================================================================
    **/

    UINT32   RetimerGlParmReg0Override[TOTAL_RETIMERS_VAR];
    UINT32   RetimerPseudoPort0Reg2Override[TOTAL_RETIMERS_VAR];
    UINT32   RetimerPseudoPort1Reg2Override[TOTAL_RETIMERS_VAR];

  /**
  ==================================================================================================
  ========================   PCI-E Port Clock Gating Related Setup Options  ========================
  ==================================================================================================
  **/
  UINT8   PciePortClkGateEnable[TOTAL_PORTS_VAR];              // Indicates Clock gating for this PCIe port is enabled or not
  /**
  ==================================================================================================
  ==================================    PCIe Global Related Setup Options ==========================
  ==================================================================================================
  **/
  UINT8   NoSnoopRdCfg;                             //on Setup
  UINT8   NoSnoopWrCfg;                             //on Setup
  UINT8   MaxReadCompCombSize;                      //on Setup
  UINT8   ProblematicPort;                          //on Setup
  UINT8   DmiAllocatingFlow;                        //on Setup
  UINT8   PcieAllocatingFlow;                       //on Setup
  UINT8   PcieHotPlugEnable;                        //on Setup
  UINT8   PcieAcpiHotPlugEnable;                    //on Setup
  UINT8   PcieLowLatencyRetimersEnabled;
  UINT8   HaltOnDmiDegraded;                        //on Setup
  UINT8   RxClockWA;
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
  UINT16  DelayBeforePCIeLinkTraining;              //On Setup
/**
==================================================================================================
==================================    Reserved Setup Options ==========================
==================================================================================================
**/
  UINT8  ReservedS9;
  UINT8  ReservedS10;
  UINT8  ReservedS11;                // On Setup
  UINT8  ReservedS12;            // On Setup
  UINT8  ReservedS13;           // On Setup
  UINT8  ReservedS14;            // On Setup
  UINT8  ReservedS15;               // On Setup
  UINT8  ReservedS16;                  // On Setup
  UINT8  ReservedS17;            // On Setup
  UINT8  ReservedS18;        // On Setup
  UINT8  ReservedS19;       // On Setup
  UINT8  ReservedS20;          // On Setup
  UINT32 ReservedS21[MAX_DEVHIDE_REGS_PER_SYSTEM]; // On Setup
  UINT8  ReservedS22[TOTAL_PORTS_VAR];                   // On Setup

  UINT8  ReservedS23[TOTAL_PORTS_VAR];          //On Setup
  UINT8  ReservedS24[TOTAL_PORTS_VAR];              //On Setup
  UINT8  ReservedS25[TOTAL_PORTS_VAR];   //On Setup
  UINT8  ReservedS26[TOTAL_PORTS_VAR];      //On Setup
  UINT8  ReservedS27[TOTAL_PORTS_VAR];  //On Setup
  UINT8  ReservedS28[TOTAL_PORTS_VAR];   //On Setup
  UINT8  ReservedS29[TOTAL_PORTS_VAR];      //On Setup
  UINT8  ReservedS30[TOTAL_PORTS_VAR];  //On Setup

  UINT8  ReservedS31[TOTAL_PORTS_VAR];          //On Setup
  UINT8  ReservedS32[TOTAL_PORTS_VAR];              //On Setup
  UINT8  ReservedS33[TOTAL_PORTS_VAR];   //On Setup
  UINT8  ReservedS34[TOTAL_PORTS_VAR];      //On Setup
  UINT8  ReservedS35[TOTAL_PORTS_VAR];  //On Setup
  UINT8  ReservedS36[TOTAL_PORTS_VAR];   //On Setup
  UINT8  ReservedS37[TOTAL_PORTS_VAR];      //On Setup
  UINT8  ReservedS38[TOTAL_PORTS_VAR];  //On Setup

  UINT8  ReservedS39[TOTAL_PORTS_VAR];                //On Setup
  UINT8  ReservedS40[TOTAL_PORTS_VAR];                  //On Setup
  UINT8  ReservedS41[TOTAL_PORTS_VAR];                //On Setup
  UINT8  ReservedS42[TOTAL_PORTS_VAR];            //On Setup
  UINT8  ReservedS43[TOTAL_PORTS_VAR];            //On Setup


  UINT8  ReservedS44[TOTAL_PORTS_VAR];
  UINT8  ReservedS45[TOTAL_PORTS_VAR];         //On Setup
  UINT8  ReservedS46;                       //On Setup

  UINT8  ReservedS47;                               //On Setup

/**
==================================================================================================
======================    IIO Global Performance Tuner Related Setup Options =====================
==================================================================================================
**/
  UINT8  PerformanceTuningMode;

  /**
  ==================================================================================================
  ======================    PCI-E Data Link Feature Exchange Enable  ===============================
  ==================================================================================================
  **/
  UINT8  PcieDataLinkFeatureExchangeEnable[TOTAL_PORTS_VAR];    //On Setup
  /**
  ==================================================================================================
  ======================       Variables added post Beta             ===============================
  ==================================================================================================
  **/
  UINT8    PcieTxRxDetPoll[TOTAL_PORTS_VAR];
  UINT8    EcrcGenEn[TOTAL_PORTS_VAR];        //On Setup
  UINT8    EcrcChkEn[TOTAL_PORTS_VAR];        //On Setup
  UINT8    ControlIommu;
  UINT32   VtdDisabledBitmask[MAX_SOCKET];
  UINT8    X2ApicOptOut;
  UINT8    SkipRetimersDetection;

  UINT8    VtdPciAcsCtlWaEn;          // Enables override of ACSCTL on PCIe root ports for VTd
  UINT8    VtdPciAcsCtlBit0;
  UINT8    VtdPciAcsCtlBit1;
  UINT8    VtdPciAcsCtlBit2;
  UINT8    VtdPciAcsCtlBit3;
  UINT8    VtdPciAcsCtlBit4;
  UINT8    AltAttenTable[TOTAL_PORTS_VAR];    //On Setup
} SOCKET_IIO_CONFIGURATION;
#pragma pack()

#endif // _SOCKET_IIO_VARIABLE_H_

