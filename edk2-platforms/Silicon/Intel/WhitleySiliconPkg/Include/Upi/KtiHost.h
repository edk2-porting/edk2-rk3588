/** @file

  @copyright
  Copyright 2004 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

// Definition Flag:
//                  1. KTI_SW_SIMULATION -> run with KTIRC Simulation
//                  2. IA32              -> run with IA32 mode


#ifndef _KTI_HOST_H_
#define _KTI_HOST_H_

#include "DataTypes.h"
#include "PlatformHost.h"
#include <Upi/KtiSi.h>
#include <Upi/KtiDisc.h>
#include "MemHostChipCommon.h"

#pragma pack(1)

/*********************************************************
                KTIRC Host Structure Related
*********************************************************/

typedef enum {
  KTI_LINK0 =  0x0,
  KTI_LINK1,
  KTI_LINK2,
  KTI_LINK3,
  KTI_LINK4,
  KTI_LINK5
} KTI_LOGIC_LINK;

typedef struct {

  UINT8   Reserved1;
  UINT8   Reserved2;
  UINT8   Reserved3;
  UINT8   Reserved4;
  UINT8   Reserved5;
  UINT8   Reserved6;
  UINT8   Reserved7;
  UINT8   Reserved8;
  UINT8   Reserved9;
  UINT8   Reserved10;
  UINT8   Reserved11;
  UINT8   Reserved12;
  UINT8   Reserved13;
  UINT8   Reserved14;
  UINT8   Reserved15;
  UINT8   Reserved16;
  UINT8   Reserved17;
  UINT8   Reserved18;
  UINT8   Reserved19;
  UINT8   Reserved20;
  UINT8   Reserved21;
  UINT8   Reserved22;
  UINT8   Reserved23;
  UINT8   Reserved24;
  UINT8   Reserved25;
  UINT8   Reserved26;
  UINT8   Reserved27;
  UINT8   Reserved28;
  UINT8   Reserved29;

  UINT8   Reserved30;
  UINT8   Reserved31;
  UINT8   Reserved32;
  UINT8   Reserved33;
  UINT8   Reserved34;
  UINT8   Reserved35;
  UINT8   Reserved36;
  UINT8   Reserved37;
  UINT32  Reserved38;

  UINT8   Reserved39;
  UINT8   Reserved40;

} KTI_RESERVED_3;

typedef struct {
  UINT32  Reserved1:2;
  UINT32  Reserved2:2;
  UINT32  Reserved3:2;
  UINT32  Rsvd1 : 26;
} KTI_RESERVED_1;

typedef struct {
  UINT8  Reserved4 : 2;
  UINT8  Rsvd1 : 6;
} KTI_RESERVED_2;

typedef struct {
  KTI_RESERVED_1 Link[MAX_FW_KTI_PORTS];
  KTI_RESERVED_2  Phy[MAX_FW_KTI_PORTS];
} KTI_RESERVED_4;

/**
  Per Lane PHY Configuration

  These PHY settings are system dependent. Every socket/link/freq requires an instance of this structure.
**/
typedef struct {
  UINT8  SocketID;              ///< Socket ID
  UINT8  AllLanesUseSameTxeq;   ///< Use same TXEQ on all lanes
  UINT8  Freq;                  ///< The Link Speed these TXEQ settings should be used for
  UINT32 Link;                  ///< Port Number
  UINT32 TXEQL[20];             ///< TXEQ Settings
  UINT32 CTLEPEAK[5];           ///< CTLE Peaking Settings
} PER_LANE_EPARAM_LINK_INFO;

/**
  All Lanes PHY Configuration

  This is for full speed mode, all lanes have the same TXEQ setting
**/
typedef struct {
  UINT8  SocketID;              ///< Socket ID
  UINT8  Freq;                  ///< The Link Speed these TXEQ settings should be used for
  UINT32 Link;                  ///< Port Number
  UINT32 AllLanesTXEQ;          ///< TXEQ Setting
  UINT8  CTLEPEAK;              ///< CTLE Peaking Setting
} ALL_LANES_EPARAM_LINK_INFO;

#define ADAPTIVE_CTLE 0x3f

typedef enum {
  TYPE_UBOX = 0,
  TYPE_UBOX_IIO,
  TYPE_MCP,
  TYPE_FPGA,
  TYPE_HFI,
  TYPE_NAC,
  TYPE_GRAPHICS,
  TYPE_DINO,
  TYPE_RESERVED,
  TYPE_DISABLED,              // This item must be prior to stack specific disable types
  TYPE_UBOX_IIO_DIS,
  TYPE_MCP_DIS,
  TYPE_FPGA_DIS,
  TYPE_HFI_DIS,
  TYPE_NAC_DIS,
  TYPE_GRAPHICS_DIS,
  TYPE_DINO_DIS,
  TYPE_RESERVED_DIS,
  TYPE_NONE
} STACK_TYPE;

//
// Link layer settings, per link
//
typedef struct {
  UINT8 KtiPortDisable:1;     // TRUE - Port disabled;    FALSE- Port enabled (default)
  UINT8 KtiLinkVnaOverride:7; // Numeric value 0x00-0x7f
  UINT8 Rsvd:8;
} KTI_CPU_LINK_SETTING;


//
// Phy general setting, per link
//
typedef struct {
  UINT32  KtiLinkSpeed:3;
  UINT32  Rsvd:29;
} KTI_CPU_PHY_SETTING;

//
// Per CPU setting
//
typedef struct {
  KTI_CPU_LINK_SETTING  Link[MAX_FW_KTI_PORTS];
  KTI_CPU_PHY_SETTING   Phy[MAX_FW_KTI_PORTS];
} KTI_CPU_SETTING;

/**
  KTIRC input structure
**/
typedef struct {
  //
  // Protocol layer and other general options; note that "Auto" is provided only options whose value will change depending
  // on the topology, not for all options.
  //

  /**
    Indicates the ratio of Bus/MMIOL/IO resource to be allocated for each CPU's IIO.
    Value 0 indicates, that CPU is not relevant for the system. If resource is
    requested for an CPU that is not currently populated, KTIRC will assume
    that the ratio is 0 for that CPU and won't allocate any resources for it.
    If resource is not requested for a CPU that is populated, KTIRC will force
    the ratio for that CPU to 1.
  **/
  UINT8               BusRatio[MAX_SOCKET];

  UINT8               D2KCreditConfig;                ///< 1 - Min, 2 - Med (Default), 3- Max
  UINT8               SnoopThrottleConfig;            ///< 0 - Disabled (Default), 1 - Min, 2 - Med, 3- Max
  UINT8               SnoopAllCores;                  ///< 0 - Disabled, 1 - Enabled, 2 - Auto
  UINT8               LegacyVgaSoc;                   ///< Socket that claims the legacy VGA range; valid values are 0-7; 0 is default.
  UINT8               LegacyVgaStack;                 ///< Stack that claims the legacy VGA range; valid values are 0-3; 0 is default.
  UINT8               ColdResetRequestStart;          ///< @deprecated Reserved.
  UINT8               P2pRelaxedOrdering;             ///< 0 - Disable(default) 1 - Enable
  UINT8               DebugPrintLevel;                ///< Bit 0 - Fatal, Bit1 - Warning, Bit2 - Info Summary; Bit 3 - Info detailed. 1 - Enable; 0 - Disable
  UINT8               SncEn;                          ///< 0 - Disable, (default) 1 - Enable
  UINT8               UmaClustering;                  ///< 0 - Disable, 2 - 2Clusters UMA, 4 - 4Clusters UMA
  UINT8               IoDcMode;                       ///< 0 - Disable IODC,  1 - AUTO (default), 2 - IODC_EN_REM_INVITOM_PUSH, 3 - IODC_EN_REM_INVITOM_ALLOCFLOW
                                                      ///< 4 - IODC_EN_REM_INVITOM_ALLOC_NONALLOC, 5 - IODC_EN_REM_INVITOM_AND_WCILF
  UINT8               DegradePrecedence;              ///< Use DEGRADE_PRECEDENCE definition; TOPOLOGY_PRECEDENCE is default
  UINT8               Degrade4SPreference;            ///< 4S1LFullConnect topology is default; another option is 4S2LRing topology.
  UINT8               DirectoryModeEn;                ///< 0 - Disable; 1 - Enable (default)
  UINT8               XptPrefetchEn;                  ///< Xpt Prefetch :  1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8               KtiPrefetchEn;                  ///< Kti Prefetch :  1 - Enable; 0 - Disable; 2 - Auto (default)
  UINT8               XptRemotePrefetchEn;            ///< Xpt Remote Prefetch :  1 - Enable; 0 - Disable; 2 - Auto (default)   (ICX only)
  UINT8               RdCurForXptPrefetchEn;          ///< RdCur for XPT Prefetch :  0 - Disable, 1 - Enable, 2- Auto (default)
  UINT8               KtiFpgaEnable[MAX_SOCKET];      ///< Indicate if should enable Fpga device found in this socket :  0 - Disable, 1 - Enable, 2- Auto
  UINT8               DdrtQosMode;                    ///< DDRT QoS Feature:  0 - Disable (default), 1 - M2M QoS Enable, Cha QoS Disable
                                                      ///< 2 - M2M QoS Enable, Cha QoS Enable

  //
  // Phy/Link Layer Options (System-wide and per socket)
  //
  UINT8               KtiLinkSpeedMode;               ///< Link speed mode selection; 0 - Slow Speed; 1- Full Speed (default)
  UINT8               KtiLinkSpeed;                   ///< Use KTI_LINKSPEED definition
  UINT8               KtiAdaptationEn;                ///< 0 - Disable, 1 - Enable
  UINT8               KtiAdaptationSpeed;             ///< Use KTI_LINK_SPEED definition; MAX_KTI_LINK_SPEED - Auto (i.e BIOS choosen speed)
  UINT8               KtiLinkL0pEn;                   ///< 0 - Disable, 1 - Enable, 2- Auto  (default)
  UINT8               KtiLinkL1En;                    ///< 0 - Disable, 1 - Enable, 2- Auto  (default)
  UINT8               KtiFailoverEn;                  ///< 0 - Disable, 1 - Enable, 2- Auto (default)
  UINT8               KtiLbEn;                        ///< 0 - Disable(default), 1 - Enable
  UINT8               KtiCrcMode;                     ///< CRC_MODE_16BIT, CRC_MODE_ROLLING_32BIT, CRC_MODE_AUTO or CRC_MODE_PER_LINK

  UINT8               KtiCpuSktHotPlugEn;             ///< 0 - Disable (default), 1 - Enable
  UINT8               KtiCpuSktHotPlugTopology;       ///< 0 - 4S Topology (default), 1 - 8S Topology
  UINT8               KtiSkuMismatchCheck;            ///< 0 - No, 1 - Yes (default)
  UINT8               IrqThreshold;                   ///< IRQ Threshold setting
  UINT8               TorThresLoctoremNorm;           ///< TOR threshold - Loctorem threshold normal
  UINT8               TorThresLoctoremEmpty;          ///< TOR threshold - Loctorem threshold empty
  UINT8               MbeBwCal;                       ///< 0 - Linear, 1 - Biased, 2 - Legacy, 3 - AUTO (default = Linear)
  UINT8               TscSyncEn;                      ///< TSC sync in sockets: 0 - Disable, 1 - Enable, 2 - AUTO (Default)
  UINT8               StaleAtoSOptEn;                 ///< HA A to S directory optimization: 1 - Enable; 0 - Disable; 2 - Auto (Default)
  UINT8               LLCDeadLineAlloc;               ///< LLC dead line alloc: 1 - Enable(Default); 0 - Disable
  UINT8               SplitLock;                      ///< @deprecated Reserved, must be set to 0.
  UINT8               ColdResetRequestEnd;            ///< @deprecated Reserved.

  ///
  /// Phy/Link Layer Options (per Port)
  ///
  KTI_CPU_SETTING     PhyLinkPerPortSetting[MAX_SOCKET];


  UINT8               mmCfgBase;                      ///< MMCFG Base address, must be 64MB (SKX, HSX, BDX) / 256MB (GROVEPORT) aligned. Options: {0:1G, 1:1.5G, 2:1.75G, 3:2G, 4:2.25G, 5:3G, 6: Auto}
  UINT8               mmCfgSize;                      ///< MMCFG Size address, must be 64M, 128M or 256M. Options: {0:64M, 1:128M, 2:256M, 3:512M, 4:1G, 5:2G, 6: Auto}
  UINT32              mmiolBase;                      ///< MMIOL Base address, must be 64MB aligned
  UINT32              mmiolSize;                      ///< MMIOL Size address
  UINT32              mmiohBase;                      ///< Address bits above 4GB, i,e, the hex value here is address Bit[45:32] for SKX family, Bit[51:32] for ICX-SP
  UINT8               CpuPaLimit;                     ///< Limits the max address to 46bits. This will take precedence over mmiohBase
  UINT8               lowGap;                         ///< @deprecated Reserved.
  UINT8               highGap;                        ///< @deprecated Reserved.
  UINT16              mmiohSize;                      ///< Number of 1GB contiguous regions to be assigned for MMIOH space per CPU.  Range 1-1024
  UINT8               isocEn;                         ///< 1 - Enable; 0 - Disable (BIOS will force this for 4S)
  UINT8               dcaEn;                          ///< 1 - Enable; 0 - Disable

  /**
  BoardTypeBitmask:
  - Bits[3:0]   - Socket0
  - Bits[7:4]   - Socket1
  - Bits[11:8]  - Socket2
  - Bits[15:12] - Socket3
  - Bits[19:16] - Socket4
  - Bits[23:20] - Socket5
  - Bits[27:24] - Socket6
  - Bits[31:28] - Socket7

  Within each Socket-specific field, bits mean:
  - Bit0 = CPU_TYPE_STD support; always 1 on Socket0
  - Bit1 = CPU_TYPE_F support
  - Bit2 = CPU_TYPE_P support
  - Bit3 = reserved
  **/
  UINT32              BoardTypeBitmask;
  UINT32              AllLanesPtr;                    ///< Pointer to an array of ALL_LANES_EPARAM_LINK_INFO structures.
  UINT32              PerLanePtr;                     ///< Pointer to an array of PER_LANE_EPARAM_LINK_INFO structures.
  UINT32              AllLanesSizeOfTable;            ///< Number of elements in array pointed to by AllLanesPtr
  UINT32              PerLaneSizeOfTable;             ///< Number of elements in array pointed to by PerLanePtr
  UINT32              WaitTimeForPSBP;                ///< the wait time in units of 1000us for PBSP to check in.
  BOOLEAN             IsKtiNvramDataReady;            ///< Used internally, Reserved.
  UINT32              OemHookPostTopologyDiscovery;   ///< OEM_HOOK_POST_TOPOLOGY_DISCOVERY function pointer. Invoked at the end of topology discovery, used for error reporting.
  UINT32              OemGetResourceMapUpdate;        ///< OEM_GET_RESOURCE_MAP_UPDATE function pointer. Allows platform code to adjust the resource map.
  UINT32              OemGetAdaptedEqSettings;        ///< @deprecated Reserved, must be set to 0.
  UINT32              OemCheckCpuPartsChangeSwap;     ///< @deprecated Reserved, must be set to 0.

  BOOLEAN             WaSerializationEn;              ///< Enable BIOS serialization WA by PcdWaSerializationEn
  KTI_RESERVED_3      Reserved166;
  KTI_RESERVED_4      Reserved167[MAX_SOCKET];
  UINT8               KtiInEnableMktme;               ///< 0 - Disabled; 1 - Enabled; MkTme status decides D2Kti feature state
  /**
    Pointers to the location of the CFR/SINIT binaries.

    Contains a pointer to a 24 byte fixed length array.
    The array contains the 3 instances of the following c-struct
    ~~~
    typedef struct {
      UINT32  CfrImagePtr;
      UINT32  CfrImageSize;
    }
    ~~~
    This allows a maximum of 3 CFR/SINIT binaries to be provided by platform code.
  **/
  UINT32              CFRImagePtr;
  UINT8               S3mCFRCommit;                   ///< 0 - Disable S3m CFR flow.    1 - Provision S3m CFR but not Commit.    2 - Provision and Commit S3M CFR.
  UINT8               PucodeCFRCommit;                ///< 0 - Disable Pucode CFR flow. 1 - Provision Pucode CFR but not Commit. 2 - Provision and Commit Pucode CFR.
} KTI_HOST_IN;

#pragma pack()

#endif // _KTI_HOST_H_
