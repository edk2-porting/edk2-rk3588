/** @file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PLATFORM_INFO_H_
#define _PLATFORM_INFO_H_

#include <PlatformInfoTypes.h>

extern EFI_GUID gEfiPlatformInfoGuid;

#pragma pack(1)

typedef struct {
  UINT32 ReservedPci;
} EFI_PLATFORM_PCI_DATA;

typedef struct {
  UINT32 ReservedCpu;
} EFI_PLATFORM_CPU_DATA;

typedef struct {
  UINT64  SysIoApicEnable;
  UINT8   SysSioExist;
  UINT8   IsocEn;       // Isoc Enabled
  UINT8   MeSegEn;      // Me Seg Enabled.
  UINT8   DmiVc1;       // 0 - Disable, 1 - Enable
  UINT8   SocketConfig;
  UINT8   DmiVcm;       // 0 - Disable, 1 - Enable
  UINT8   DmiReserved;  // for alignment
} EFI_PLATFORM_SYS_DATA;

typedef struct {
  UINT32  BiosGuardPhysBase;
  UINT32  BiosGuardMemSize;
  UINT32  MemTsegSize;
  UINT32  MemIedSize;

#ifdef LT_FLAG
  UINT32  MemLtMemSize;     // Below TSEG, contains (high to low: (Heap, SinitMem, reserved)3M, LtDevMemNonDMA 2M)
  UINT32  MemDprMemSize;    // 6M DPR contained in LtMem
  UINT32  MemLtMemAddress;
#endif // #ifdef LT_FLAG

  UINT64  PsmiUcTraceRegionBase;
  UINT64  PsmiUcTraceRegionSize;
} EFI_PLATFORM_MEM_DATA;

typedef struct {
  UINT8   PchPciePortCfg1;   // PCIE Port Configuration Strap 1
  UINT8   PchPciePortCfg2;   // PCIE Port Configuration Strap 2
  UINT8   PchPciePortCfg3;   // PCIE Port Configuration Strap 3
  UINT8   PchPciePortCfg4;   // PCIE Port Configuration Strap 4
  UINT8   PchPciePortCfg5;   // PCIE Port Configuration Strap 5
  UINT8   PchPcieSBDE;
  UINT8   LomLanSupported;   // Indicates if PCH LAN on board is supported or not
  UINT8   GbePciePortNum;    // Indicates the PCIe port qhen PCH LAN on board is connnected.
  UINT8   GbeRegionInvalid;
  BOOLEAN GbeEnabled;        // Indicates if the GBE is SS disabled
  UINT8   PchStepping;
} EFI_PLATFORM_PCH_DATA;

//
// Platform Deep Sleep Feature
//
typedef struct {
  UINT8   PlatformDeepS5;
  UINT8   DeepS5DelayTime;
} EFI_PLATFORM_DEEPS5_DATA;

typedef struct {
  UINT8 EnableClockSpreadSpec;
} EFI_PLATFORM_EXTERNAL_CLOCK;

///
/// Enumeration of possible Wilson City interposer types
///
typedef enum {
  InterposerUnknown = 0,    ///< Type is unknown and should be retrieved from the Platform HOB
  InterposerA,              ///< Interposer Type A
  InterposerB,              ///< Interposer Type B
  InterposerMax             ///< Maximum number of members of this enum
} INTERPOSER_TYPE;

#pragma pack(1)
typedef struct {
  UINT8 MappedMcId[MAX_IMC];
} INTERPOSER;

typedef struct {
  INTERPOSER Interposer[InterposerMax];
} INTERPOSER_MAP;
#pragma pack()

//
// This HOB definition must be consistent with what is created in the
// PlatformInfo protocol definition.  This way the information in the
// HOB can be directly copied upon the protocol and only the strings
// will need to be updated.
//
typedef struct _EFI_PLATFORM_INFO {
  UINT8                       SystemUuid[16];         // 16 bytes
  CHAR8                       SerialNumber[64];       // 64 bytes
  UINT32                      Signature;              // "$PIT" 0x54495024
  UINT32                      Size;                   // Size of the table
  UINT16                      Revision;               // Revision of the table
  UINT16                      Type;                   // Platform Type
  UINT32                      CpuType;                // Cpu Type
  UINT8                       CpuStepping;            // Cpu Stepping
  UINT32                      TypeRevisionId;         // Board Revision ID
  UINT8                       BoardId;                // Board ID
  UINT16                      IioSku;
  UINT16                      IioRevision;
  UINT16                      PchSku;
  UINT8                       PchRevision;
  UINT8                       PchType;                // Retrive PCH SKU type installed
  UINT8                       MaxNumOfPchs;           // Maximum Number of installed PCHs
  BOOLEAN                     ExtendedInfoValid;      // If TRUE then below fields are Valid
  UINT8                       Checksum;               // Checksum minus SystemUuid is valid in DXE only.
  UINT64                      TypeStringPtr;
  UINT64                      IioStringPtr;
  UINT64                      PchStringPtr;
  EFI_PLATFORM_PCI_DATA       PciData;
  EFI_PLATFORM_CPU_DATA       CpuData;
  EFI_PLATFORM_MEM_DATA       MemData;
  EFI_PLATFORM_SYS_DATA       SysData;
  EFI_PLATFORM_PCH_DATA       PchData;
  UINT8                       IioRiserId;
  UINT8                       PcieRiser1Type;
  UINT8                       PcieRiser2Type;
  UINT8                       PlatformCapabilities;   // Platform capabilites describes platform is 2-socket modular board, 4S or 8S
  //
  // Wilson City Interposer Type
  //
  INTERPOSER_TYPE             InterposerType[MAX_SOCKET];                         // 0 - Unknown, 1 - InterposerA, 2 - InterposerB
  UINT32                      QATDis;                                             // 0 - QAT Enabled; 1 - Disabled
  UINT32                      QATSel;
  UINT8                       MemoryTopology[MAX_SOCKET][MAX_IMC*MAX_MC_CH];      // Specifies the memory topology per socket-per channel
  UINT8                       MemoryConnectorType[MAX_SOCKET][MAX_IMC*MAX_MC_CH]; // Specifies the memory connector type per socket-per channel, type EFI_MEMORY_DIMM_CONNECTOR_TYPE
  EFI_PLATFORM_DEEPS5_DATA    DeepS5Data;
  EFI_PLATFORM_EXTERNAL_CLOCK ExternalClock;
} EFI_PLATFORM_INFO;

#pragma pack()

#endif // #ifndef _PLATFORM_INFO_H_
