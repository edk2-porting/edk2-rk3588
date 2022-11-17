/** @file
  Data format for Universal Data Structure

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef   __SOCKET_MP_LINK_CONFIG_DATA_H__
#define   __SOCKET_MP_LINK_CONFIG_DATA_H__

#include <UncoreCommonIncludes.h>

extern EFI_GUID gEfiSocketMpLinkVariableGuid;
#define SOCKET_MP_LINK_CONFIGURATION_NAME L"SocketMpLinkConfig"

#pragma pack(1)
typedef struct {
  // SKXTODO: rename to Kti when removing HSX code
  UINT8  QpiSetupNvVariableStartTag;  // This must be the very first one of the whole KTI Setup NV variable!

  //
  // Used by the PciHostBridge DXE driver, these variables don't need to be exposed through setup options
  // The variables are used as a communication vehicle from the PciHostBridge DXE driver to an OEM hook
  // which updates the KTI resource map
  //
  //
  //  KTI Host structure inputs
  //
  UINT8  BusRatio[MAX_SOCKET];
  UINT8  D2KCreditConfig;      // 1 - Min, 2 - Med (Default), 3- Max
  UINT8  SnoopThrottleConfig;  // 0 - Disabled, 1 - Min, 2 - Med (Default), 3- Max
  UINT8  SnoopAllCores;        // 0 - Disabled, 1 - Enabled, 2 - Auto
  UINT8  LegacyVgaSoc;         // Socket that claims the legacy VGA range; valid values are 0-3; 0 is default.
  UINT8  LegacyVgaStack;       // Stack that claims the legacy VGA range; valid values are 0-3; 0 is default.
  UINT8  P2pRelaxedOrdering;   // 1 - Enable; 0 - Disable
  UINT8  DebugPrintLevel;      // Bit 0 - Fatal, Bit1 - Warning, Bit2 - Info Summary; Bit 3 - Info detailed. 1 - Enable; 0 - Disable
  UINT8  DegradePrecedence;    // Use DEGRADE_PRECEDENCE definition; TOPOLOGY_PRECEDENCE is default
  UINT8  Degrade4SPreference;  // 4S1LFullConnect topology is default; another option is 4S2LRing topology.

  //
  // Phy/Link Layer Options
  //
  UINT8  QpiLinkSpeedMode;         // Link speed mode selection; 0 - Slow Speed; 1- Full Speed
  UINT8  QpiLinkSpeed;             // One of SPEED_REC_96GT, SPEED_REC_104GT, MAX_KTI_LINK_SPEED (default), FREQ_PER_LINK
  UINT8  KtiLinkL0pEn;             // 0 - Disable, 1 - Enable, 2- Auto (default)
  UINT8  KtiLinkL1En;              // 0 - Disable, 1 - Enable, 2- Auto (default)
  UINT8  KtiFailoverEn;            // 0 - Disable, 1 - Enable, 2- Auto (default)
  UINT8  KtiLbEn;                  // 0 - Disable(default), 1 - Enable
  UINT8  KtiCrcMode;               // 0 - 8 bit CRC 1 - 16 bit CRC Mode
  UINT8  QpiCpuSktHotPlugEn;       // 0 - Disable (default), 1 - Enable
  UINT8  KtiCpuSktHotPlugTopology; // 0 - 4S Topology (default), 1 - 8S Topology
  UINT8  KtiSkuMismatchCheck;      // 0 - No, 1 - Yes (default)
  UINT8  KtiLinkVnaOverride;       // 0x100 - per link, 0xff - max (default), 0x00 - min
  UINT8  SncEn;                    // 0 - Disable (default), 1 - Enable
  UINT8  IoDcMode;                 // 0 - Disable IODC,  1 - AUTO (default), 2 - IODC_EN_REM_INVITOM_PUSH, 3 - IODC_EN_REM_INVITOM_ALLOCFLOW
                                   // 4 - IODC_EN_REM_INVITOM_ALLOC_NONALLOC, 5 - IODC_EN_REM_INVITOM_AND_WCILF
  UINT8  DirectoryModeEn;          // 0 - Disable; 1 - Enable (default)
  UINT8  XptPrefetchEn;            // XPT Prefetch :  1 - Enable (Default); 0 - Disable
  UINT8  KtiPrefetchEn;            // KTI Prefetch :  1 - Enable (Default); 0 - Disable
  UINT8  XptRemotePrefetchEn;      // XPT Remote Prefetch :  1 - Enable (Default); 0 - Disable
  UINT8  RdCurForXptPrefetchEn;    // RdCur for XPT Prefetch :  0 - Disable, 1 - Enable, 2- Auto (default)
  UINT8  IrqThreshold;             // KTI IRQ Threshold setting
  UINT8  TorThresLoctoremNorm;     // TOR threshold - Loctorem threshold normal
  UINT8  TorThresLoctoremEmpty;    // TOR threshold - Loctorem threshold empty
  UINT8  MbeBwCal;                 // 0 - Linear, 1 - Biased, 2 - Legacy, 3 - AUTO (default = Linear)
  UINT8  TscSyncEn;                // TSC Sync Enable: 0 - Disable; 1 - Enable; 2 - AUTO (default)
  UINT8  StaleAtoSOptEn;           // HA A to S directory optimization
  UINT8  LLCDeadLineAlloc;         // Never fill dead lines in LLC: 1 - Enable, 0 - Disable
  UINT8  SplitLock;                // Setting this bit removes the Ubox PCU handshake during stopreq and startreq only
                                   // for splitlocks: - Enable, 0 - Disable
  UINT8  KtiAdaptationEn;          // 0 - Disable, 1 - Enable
  UINT8  KtiAdaptationSpeed;       // Use KTI_LINK_SPEED definition; MAX_KTI_LINK_SPEED - Auto (i.e BIOS choosen speed)

  UINT8  ReservedS150;
  UINT8  ReservedS151;
  UINT8  ReservedS152;
  UINT8  ReservedS153;
  UINT8  ReservedS154;
  UINT8  ReservedS155;
  UINT8  ReservedS156;
  UINT8  ReservedS157;
  UINT8  ReservedS158;
  UINT8  ReservedS159;
  UINT8  ReservedS160;
  UINT8  ReservedS161;
  UINT8  ReservedS162;
  UINT8  ReservedS163;
  UINT8  ReservedS164;
  UINT8  ReservedS165;
  UINT8  ReservedS166;
  UINT8  ReservedS167;
  UINT8  ReservedS168;
  UINT8  ReservedS169;
  UINT8  ReservedS170;
  UINT8  ReservedS171;
  UINT8  ReservedS172;
  UINT8  ReservedS173;
  UINT8  ReservedS174;
  UINT8  ReservedS175;
  UINT8  ReservedS176;
  UINT8  ReservedS177;
  UINT8  ReservedS178;

  UINT8  ReservedS179;
  UINT8  ReservedS180;
  UINT8  ReservedS181;
  UINT8  ReservedS182;
  UINT8  ReservedS183;
  UINT8  ReservedS184;
  UINT8  ReservedS185;
  UINT32 ReservedS186;

  UINT8  ReservedS187;
  UINT8  ReservedS188;

#define  UPICPUPRTVARIABLE(x)       x##KtiPortDisable;x##KtiLinkSpeed;x##KtiLinkVnaOverride;

  UINT8 KtiCpuPerPortStartTag;
  UPICPUPRTVARIABLE (UINT8 Cpu0P0)
  UPICPUPRTVARIABLE (UINT8 Cpu0P1)
  UPICPUPRTVARIABLE (UINT8 Cpu0P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu0P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu0P4)
  UPICPUPRTVARIABLE (UINT8 Cpu0P5)
#endif
#if MAX_SOCKET > 1
  UPICPUPRTVARIABLE (UINT8 Cpu1P0)
  UPICPUPRTVARIABLE (UINT8 Cpu1P1)
  UPICPUPRTVARIABLE (UINT8 Cpu1P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu1P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu1P4)
  UPICPUPRTVARIABLE (UINT8 Cpu1P5)
#endif
#endif
#if MAX_SOCKET > 2
  UPICPUPRTVARIABLE (UINT8 Cpu2P0)
  UPICPUPRTVARIABLE (UINT8 Cpu2P1)
  UPICPUPRTVARIABLE (UINT8 Cpu2P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu2P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu2P4)
  UPICPUPRTVARIABLE (UINT8 Cpu2P5)
#endif
#endif
#if MAX_SOCKET > 3
  UPICPUPRTVARIABLE (UINT8 Cpu3P0)
  UPICPUPRTVARIABLE (UINT8 Cpu3P1)
  UPICPUPRTVARIABLE (UINT8 Cpu3P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu3P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu3P4)
  UPICPUPRTVARIABLE (UINT8 Cpu3P5)
#endif
#endif
#if (MAX_SOCKET > 4)
  UPICPUPRTVARIABLE (UINT8 Cpu4P0)
  UPICPUPRTVARIABLE (UINT8 Cpu4P1)
  UPICPUPRTVARIABLE (UINT8 Cpu4P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu4P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu4P4)
  UPICPUPRTVARIABLE (UINT8 Cpu4P5)
#endif
#endif
#if (MAX_SOCKET > 5)
  UPICPUPRTVARIABLE (UINT8 Cpu5P0)
  UPICPUPRTVARIABLE (UINT8 Cpu5P1)
  UPICPUPRTVARIABLE (UINT8 Cpu5P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu5P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu5P4)
  UPICPUPRTVARIABLE (UINT8 Cpu5P5)
#endif
#endif
#if (MAX_SOCKET > 6)
  UPICPUPRTVARIABLE (UINT8 Cpu6P0)
  UPICPUPRTVARIABLE (UINT8 Cpu6P1)
  UPICPUPRTVARIABLE (UINT8 Cpu6P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu6P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu6P4)
  UPICPUPRTVARIABLE (UINT8 Cpu6P5)
#endif
#endif
#if (MAX_SOCKET > 7)
  UPICPUPRTVARIABLE (UINT8 Cpu7P0)
  UPICPUPRTVARIABLE (UINT8 Cpu7P1)
  UPICPUPRTVARIABLE (UINT8 Cpu7P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTVARIABLE (UINT8 Cpu7P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTVARIABLE (UINT8 Cpu7P4)
  UPICPUPRTVARIABLE (UINT8 Cpu7P5)
#endif
#endif

#define UPICPUPRTDFXVARIABLE(x)       x##ReservedS189;x##ReservedS190;x##ReservedS191;x##ReservedS246;

  UINT8 DfxKtiCpuPerPortStartTag;
  UPICPUPRTDFXVARIABLE (UINT8 Cpu0P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu0P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu0P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu0P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu0P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu0P5)
#endif
#if MAX_SOCKET > 1
  UPICPUPRTDFXVARIABLE (UINT8 Cpu1P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu1P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu1P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu1P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu1P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu1P5)
#endif
#endif
#if MAX_SOCKET > 2
  UPICPUPRTDFXVARIABLE (UINT8 Cpu2P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu2P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu2P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu2P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu2P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu2P5)
#endif
#endif
#if MAX_SOCKET > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu3P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu3P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu3P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu3P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu3P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu3P5)
#endif
#endif
#if MAX_SOCKET > 4
  UPICPUPRTDFXVARIABLE (UINT8 Cpu4P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu4P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu4P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu4P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu4P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu4P5)
#endif
#endif
#if MAX_SOCKET > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu5P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu5P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu5P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu5P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu5P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu5P5)
#endif
#endif
#if MAX_SOCKET > 6
  UPICPUPRTDFXVARIABLE (UINT8 Cpu6P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu6P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu6P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu6P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu6P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu6P5)
#endif
#endif
#if MAX_SOCKET > 7
  UPICPUPRTDFXVARIABLE (UINT8 Cpu7P0)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu7P1)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu7P2)
#if MAX_KTI_PORTS > 3
  UPICPUPRTDFXVARIABLE (UINT8 Cpu7P3)
#endif
#if MAX_KTI_PORTS > 5
  UPICPUPRTDFXVARIABLE (UINT8 Cpu7P4)
  UPICPUPRTDFXVARIABLE (UINT8 Cpu7P5)
#endif
#endif

  UINT8  QpiSetupNvVariableEndTag;  // This must be the last one of the whole KTI Setup NV variable
} SOCKET_MP_LINK_CONFIGURATION;

#pragma pack()

#endif // __SOCKET_MP_LINK_CONFIG_DATA_H__

