/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef   __SOCKET_MP_LINK_CONFIG_DATA_H__
#define   __SOCKET_MP_LINK_CONFIG_DATA_H__

#include <UncoreCommonIncludes.h>
#include "SocketConfiguration.h"

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
  //  KTI host structure inputs
  //
  UINT8  BusRatio[MAX_SOCKET];
  UINT8  LegacyVgaSoc;         // Socket that claims the legacy VGA range; valid values are 0-3; 0 is default.
  UINT8  LegacyVgaStack;       // Stack that claims the legacy VGA range; valid values are 0-3; 0 is default.
  UINT8  MmioP2pDis;           // 1 - Disable; 0 - Enable
  UINT8  DebugPrintLevel;      // Bit 0 - Fatal, Bit1 - Warning, Bit2 - Info Summary; Bit 3 - Info detailed. 1 - Enable; 0 - Disable
  UINT8  DegradePrecedence;    // Use DEGRADE_PRECEDENCE definition; TOPOLOGY_PRECEDENCE is default

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
  UINT8  RdCurForXptPrefetchEn;    // RdCur for XPT Prefetch :  0 - Disable, 1 - Enable, 2- Auto (default)
  UINT8  IrqThreshold;             // KTI IRQ Threshold setting
  UINT8  TscSyncEn;                // TSC Sync Enable: 0 - Disable; 1 - Enable; 2 - AUTO (default)
  UINT8  StaleAtoSOptEn;           // HA A to S directory optimization
  UINT8  LLCDeadLineAlloc;         // Never fill dead lines in LLC: 1 - Enable, 0 - Disable

  UINT8  Reserved1;
  UINT8  Reserved2;
  UINT8  Reserved3;
  UINT8  Reserved4;
  UINT8  Reserved5;
  UINT8  Reserved6;
  UINT8  Reserved7;
  UINT8  Reserved8;
  UINT8  Reserved9;
  UINT8  Reserved10;
  UINT8  Reserved11;
  UINT8  Reserved12;
  UINT8  Reserved13;
  UINT8  Reserved14;
  UINT8  Reserved15;
  UINT8  Reserved16;
  UINT8  Reserved17;
  UINT8  Reserved18;


#define  CSICPUPRTVARIABLE(x)       x##KtiPortDisable;x##KtiLinkSpeed;x##KtiLinkVnaOverride;

  UINT8 KtiCpuPerPortStartTag;
  CSICPUPRTVARIABLE(UINT8 Cpu0P0)
  CSICPUPRTVARIABLE(UINT8 Cpu0P1)
  CSICPUPRTVARIABLE(UINT8 Cpu0P2)
#if MAX_SOCKET > 1
  CSICPUPRTVARIABLE(UINT8 Cpu1P0)
  CSICPUPRTVARIABLE(UINT8 Cpu1P1)
  CSICPUPRTVARIABLE(UINT8 Cpu1P2)
#endif
#if MAX_SOCKET > 2
  CSICPUPRTVARIABLE(UINT8 Cpu2P0)
  CSICPUPRTVARIABLE(UINT8 Cpu2P1)
  CSICPUPRTVARIABLE(UINT8 Cpu2P2)
#endif
#if MAX_SOCKET > 3
  CSICPUPRTVARIABLE(UINT8 Cpu3P0)
  CSICPUPRTVARIABLE(UINT8 Cpu3P1)
  CSICPUPRTVARIABLE(UINT8 Cpu3P2)
#endif
#if (MAX_SOCKET > 4)
  CSICPUPRTVARIABLE(UINT8 Cpu4P0)
  CSICPUPRTVARIABLE(UINT8 Cpu4P1)
  CSICPUPRTVARIABLE(UINT8 Cpu4P2)
#endif
#if (MAX_SOCKET > 5)
  CSICPUPRTVARIABLE(UINT8 Cpu5P0)
  CSICPUPRTVARIABLE(UINT8 Cpu5P1)
  CSICPUPRTVARIABLE(UINT8 Cpu5P2)
#endif
#if (MAX_SOCKET > 6)
  CSICPUPRTVARIABLE(UINT8 Cpu6P0)
  CSICPUPRTVARIABLE(UINT8 Cpu6P1)
  CSICPUPRTVARIABLE(UINT8 Cpu6P2)
#endif
#if (MAX_SOCKET > 7)
  CSICPUPRTVARIABLE(UINT8 Cpu7P0)
  CSICPUPRTVARIABLE(UINT8 Cpu7P1)
  CSICPUPRTVARIABLE(UINT8 Cpu7P2)
#endif

#define CSICPUPRTDFXVARIABLE(x)    x##ReservedA;x##ReservedB;x##ReservedC;x##ReservedD;

  UINT8 Reserved19;
  CSICPUPRTDFXVARIABLE(UINT8 Cpu0P0)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu0P1)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu0P2)
#if MAX_SOCKET > 1
  CSICPUPRTDFXVARIABLE(UINT8 Cpu1P0)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu1P1)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu1P2)
#endif
#if MAX_SOCKET > 2
  CSICPUPRTDFXVARIABLE(UINT8 Cpu2P0)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu2P1)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu2P2)
#endif
#if MAX_SOCKET > 3
  CSICPUPRTDFXVARIABLE(UINT8 Cpu3P0)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu3P1)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu3P2)
#endif
#if MAX_SOCKET > 4
  CSICPUPRTDFXVARIABLE(UINT8 Cpu4P0)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu4P1)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu4P2)
#endif
#if MAX_SOCKET > 5
  CSICPUPRTDFXVARIABLE(UINT8 Cpu5P0)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu5P1)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu5P2)
#endif
#if MAX_SOCKET > 6
  CSICPUPRTDFXVARIABLE(UINT8 Cpu6P0)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu6P1)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu6P2)
#endif
#if MAX_SOCKET > 7
  CSICPUPRTDFXVARIABLE(UINT8 Cpu7P0)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu7P1)
  CSICPUPRTDFXVARIABLE(UINT8 Cpu7P2)
#endif

  UINT8  QpiSetupNvVariableEndTag;  // This must be the last one of the whole KTI Setup NV variable
} SOCKET_MP_LINK_CONFIGURATION;

#pragma pack()

#endif // __SOCKET_MP_LINK_CONFIG_DATA_H__

