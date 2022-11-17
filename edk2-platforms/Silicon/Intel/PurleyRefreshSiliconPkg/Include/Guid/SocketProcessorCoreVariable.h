/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef   __SOCKET_PROCESSORCORE_CONFIGURATION_DATA_H__
#define   __SOCKET_PROCESSORCORE_CONFIGURATION_DATA_H__


#include <UncoreCommonIncludes.h>
#include "SocketConfiguration.h"

extern EFI_GUID gEfiSocketProcessorCoreVarGuid;
#define SOCKET_PROCESSORCORE_CONFIGURATION_NAME L"SocketProcessorCoreConfig"

#pragma pack(1)

typedef struct {

  UINT8   CpuidMaxValue;
  UINT8   ExecuteDisableBit;

  UINT8   PchTraceHubEn;                    // PCH TRACE HUB
  UINT8   C1AutoDemotion;                   // C1 Auto Demotion
  UINT8   C3AutoDemotion;                   // C3 Auto Demotion
  UINT8   ProcessorHyperThreadingDisable;   // Hyper Threading [ALL]
  UINT8   ProcessorLtsxEnable;              // Enabling TXT
  UINT8   ProcessorVmxEnable;               // Enabling VMX
  UINT8   ProcessorSmxEnable;               // Enabling SMX
  UINT8   ProcessorMsrLockControl;          // MSR Lock Bit Control
  UINT8   DebugInterface;                   // IA32_DEBUG_INTERFACE_MSR
  UINT8   ThreeStrikeTimer;                 // Disable 3strike timer
  UINT8   FastStringEnable;                 // Fast String
  UINT8   MachineCheckEnable;               // Machine Check
  UINT8   MlcStreamerPrefetcherEnable;      // Hardware Prefetch
  UINT8   MlcSpatialPrefetcherEnable;       // Adjacent Cache Line Prefetch
  UINT8   DCUStreamerPrefetcherEnable;      // DCU Streamer Prefetcher
  UINT8   DCUIPPrefetcherEnable;            // DCU IP Prefetcher
  UINT8   DCUModeSelection;                 // DCU Mode Selection
  UINT8   ProcessorX2apic;                  // Enable Processor XAPIC
  UINT8   ForceX2ApicIds;                   // Force to use > 8bit ApicId
  UINT8   BspSelection;                     // Select BSP
  UINT8   IedSize;                          // IED size
  UINT8   IedTraceSize;                     // IED trace size
  UINT8   TsegSize;                         // TSEG size
  UINT8   AllowMixedPowerOnCpuRatio;        // Allow Mixed PowerOn CpuRatio
  UINT8   CheckCpuBist;                     // check and disable BIST faile core or ignore
  UINT8   ProcessorFlexibleRatio;           // Non-Turbo Mode Processor Core Ratio Multiplier
  UINT8   ProcessorFlexibleRatioOverrideEnable;         // Non-Turbo Mode Processor Core Ratio Multiplier Enable
  UINT8   Reserved2;                        // Reserved 2
  UINT8   ForcePhysicalModeEnable;          // Force physical destionation mode
  UINT8   LlcPrefetchEnable;                // LLC Prefetch
  UINT8   ProcessorVirtualWireMode;

  UINT8   AesEnable;
  UINT8   PpinControl;                      // PPIN Control MSR
  UINT8   LockChipset;                      // Lock Chipset
  UINT8   SkipStopPbet;                     // Skip StopPbet

  UINT8   BiosAcmErrorReset;                // Disable LT-SX and reset system when BIOS ACM error occurs
  UINT8   AcmType;                          // 0x80 = debug signed ACM; 0x40 = NPW production signed ACM; 0x00 = PW production signed ACM


  UINT64  CoreDisableMask[MAX_SOCKET];      // one for each CPU socket
  // IOT/OCLA configs
#ifndef OCLA_TOR_ENTRY_MAX
    #define OCLA_TOR_ENTRY_MIN         0
    #define OCLA_TOR_ENTRY_MAX      0x11      // 15 or 17 depending on Isoch on/off
    #define OCLA_TOR_ENTRY_DEFAULT     1
    #define OCLA_WAY_MIN               0
    #define OCLA_WAY_MAX               8      // max 8 LLC ways out of 11 can be reserved for OCLA
    #define OCLA_WAY_DEFAULT           1
#endif
  UINT8   IotEn[MAX_SOCKET];
  UINT8   OclaMaxTorEntry[MAX_SOCKET];
  UINT8   OclaMinWay[MAX_SOCKET];
  UINT32  IioLlcWaysMask;                         // MSR CBO_SLICE0_CR_IIO_LLC_WAYS bitmask. - Only Bits[22:0] are used
  UINT32  ExpandedIioLlcWaysMask;                 // MSR INGRESS_SPARE[10:0] bitmask. - Only Bits[10:0] are used
  UINT32  RemoteWaysMask;                         // MSR INGRESS_SPARE[26:16] bitmask. - Only Bits[10:0] are used
  UINT32  QlruCfgMask_Lo;                         // MSR VIRTUAL_MSR_CR_QLRU_CONFIG bitmask - Lower 32-bit
  UINT32  QlruCfgMask_Hi;                         // MSR VIRTUAL_MSR_CR_QLRU_CONFIG bitmask - Higher 32-bit


  UINT8   PCIeDownStreamPECIWrite;

//
// Targeted Smi Support
//
  UINT8 TargetedSmi;
//
// eSMM Save State Mode
//
  UINT8 eSmmSaveState;

  UINT8 PeciInTrustControlBit; //On Setup

  UINT8  Poison;
  UINT8  Viral;
  UINT8  EVMode;
  UINT8  SmbusErrorRecovery;
  UINT8  RdtCatOpportunisticTuning;
  UINT8  CpuDbpEnable;         // Enable/Disable DBP-F
  UINT8  L2RfoPrefetchDisable;      // L2 RFO Prefetch
  UINT8  MonitorMwaitEnabled;
  UINT8  MonitorMwaitSwitchPresent;
} SOCKET_PROCESSORCORE_CONFIGURATION;
#pragma pack()

#endif




