/** @file
  Data format for Universal Data Structure

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IIO_UNIVERSAL_DATA_
#define _IIO_UNIVERSAL_DATA_

#define IIO_UNIVERSAL_DATA_GUID { 0x7FF396A1, 0xEE7D, 0x431E, { 0xBA, 0x53, 0x8F, 0xCA, 0x12, 0x7C, 0x44, 0xC0 } }
#include "SysHost.h"
#include "IioPlatformData.h"
#include "UncoreCommonIncludes.h"
#include <Guid/SocketVariable.h>

//--------------------------------------------------------------------------------------//
// Structure definitions for Universal Data Store (UDS)
//--------------------------------------------------------------------------------------//
#define UINT64  unsigned long long

#pragma pack(1)


typedef struct {
  UINT8                     Valid;         // TRUE, if the link is valid (i.e reached normal operation)
  UINT8                     PeerSocId;     // Socket ID
  UINT8                     PeerSocType;   // Socket Type (0 - CPU; 1 - IIO)
  UINT8                     PeerPort;      // Port of the peer socket
}QPI_PEER_DATA;

typedef struct {
  UINT8                     Valid;
  UINT32                    MmioBar[TYPE_MAX_MMIO_BAR];
  UINT8                     PcieSegment;
  UINT64_STRUCT             SegMmcfgBase;
  UINT16                    stackPresentBitmap;
  UINT16                    M2PciePresentBitmap;
  UINT8                     TotM3Kti;
  UINT8                     TotCha;
  UINT32                    ChaList[MAX_CHA_MAP];
  UINT32                    SocId;
  QPI_PEER_DATA             PeerInfo[MAX_FW_KTI_PORTS];    // QPI LEP info
} QPI_CPU_DATA;

typedef struct {
  UINT8                     Valid;
  UINT8                     SocId;
  QPI_PEER_DATA             PeerInfo[MAX_SOCKET];    // QPI LEP info
} QPI_IIO_DATA;

typedef struct {
    IIO_PORT_INFO           PortInfo[NUMBER_PORTS_PER_SOCKET];
} IIO_DMI_PCIE_INFO;

typedef struct _STACK_RES {
  UINT8                   Personality;
  UINT8                   BusBase;
  UINT8                   BusLimit;
  UINT16                  PciResourceIoBase;
  UINT16                  PciResourceIoLimit;
  UINT32                  IoApicBase;
  UINT32                  IoApicLimit;
  UINT32                  Mmio32Base;            // Base of low MMIO configured for this stack in memory map
  UINT32                  Mmio32Limit;           // Limit of low MMIO configured for this stack in memory map
  UINT64                  Mmio64Base;            // Base of high MMIO configured for this stack in memory map
  UINT64                  Mmio64Limit;           // Limit of high MMIO configured for this stack in memory map
  UINT32                  PciResourceMem32Base;  // Base of low MMIO resource available for PCI devices
  UINT32                  PciResourceMem32Limit; // Limit of low MMIO resource available for PCI devices
  UINT64                  PciResourceMem64Base;  // Base of high MMIO resource available for PCI devices
  UINT64                  PciResourceMem64Limit; // Limit of high MMIO resource available for PCI devices
  UINT32                  VtdBarAddress;
  UINT32                  Mmio32MinSize;         // Minimum required size of MMIO32 resource needed for this stack
} STACK_RES;

typedef struct {
    UINT8                   Valid;
    UINT8                   SocketID;            // Socket ID of the IIO (0..3)
    UINT8                   BusBase;
    UINT8                   BusLimit;
    UINT16                  PciResourceIoBase;
    UINT16                  PciResourceIoLimit;
    UINT32                  IoApicBase;
    UINT32                  IoApicLimit;
    UINT32                  Mmio32Base;          // Base of low MMIO configured for this socket in memory map
    UINT32                  Mmio32Limit;         // Limit of low MMIO configured for this socket in memory map
    UINT64                  Mmio64Base;          // Base of high MMIO configured for this socket in memory map
    UINT64                  Mmio64Limit;         // Limit of high MMIO configured for this socket in memory map
    STACK_RES               StackRes[MAX_LOGIC_IIO_STACK];
    UINT32                  RcBaseAddress;
    IIO_DMI_PCIE_INFO       PcieInfo;
    UINT8                   DmaDeviceCount;
} IIO_RESOURCE_INSTANCE;

typedef struct {
    UINT16                  PlatGlobalIoBase;       // Global IO Base
    UINT16                  PlatGlobalIoLimit;      // Global IO Limit
    UINT32                  PlatGlobalMmio32Base;   // Global Mmiol base
    UINT32                  PlatGlobalMmio32Limit;  // Global Mmiol limit
    UINT64                  PlatGlobalMmio64Base;   // Global Mmioh Base [43:0]
    UINT64                  PlatGlobalMmio64Limit;  // Global Mmioh Limit [43:0]
    QPI_CPU_DATA            CpuQpiInfo[MAX_SOCKET]; // QPI related info per CPU
    QPI_IIO_DATA            IioQpiInfo[MAX_SOCKET]; // QPI related info per IIO
    UINT32                  MemTsegSize;
    UINT32                  MemIedSize;
    UINT64                  PciExpressBase;
    UINT32                  PciExpressSize;
    UINT32                  MemTolm;
    IIO_RESOURCE_INSTANCE   IIO_resource[MAX_SOCKET];
    UINT8                   numofIIO;
    UINT8                   MaxBusNumber;
    UINT32                  packageBspApicID[MAX_SOCKET]; // This data array is valid only for SBSP, not for non-SBSP CPUs. <AS> for CpuSv
    UINT8                   EVMode;
    UINT8                   Pci64BitResourceAllocation;
    UINT8                   SkuPersonality[MAX_SOCKET];
    UINT8                   VMDStackEnable[MaxIIO][MAX_IIO_STACK];
    UINT16                  IoGranularity;
    UINT32                  MmiolGranularity;
    UINT64_STRUCT           MmiohGranularity;
    UINT8                   RemoteRequestThreshold;  //5370389
    UINT32                  UboxMmioSize;
    UINT32                  MaxAddressBits;
} PLATFORM_DATA;

typedef struct {
    UINT8                   CurrentUpiiLinkSpeed;// Current programmed UPI Link speed (Slow/Full speed mode)
    UINT8                   CurrentUpiLinkFrequency; // Current requested UPI Link frequency (in GT)
    UINT8                   OutKtiCpuSktHotPlugEn;            // 0 - Disabled, 1 - Enabled for PM X2APIC
    UINT32                  OutKtiPerLinkL1En[MAX_SOCKET];    // output kti link enabled status for PM
    UINT8                   IsocEnable;
    UINT32                  meRequestedSize; // Size of the memory range requested by ME FW, in MB
    UINT32                  ieRequestedSize; // Size of the memory range requested by IE FW, in MB
    UINT8                   DmiVc1;
    UINT8                   DmiVcm;
    UINT32                  CpuPCPSInfo;
    UINT8                   cpuSubType;
    UINT8                   SystemRasType;
    UINT8                   numCpus;                                        // 1,..4. Total number of CPU packages installed and detected (1..4)by QPI RC
    UINT16                  tolmLimit;
    UINT32                  tohmLimit;
    RC_VERSION              RcVersion;
    BOOLEAN                 MsrTraceEnable;
    UINT8                   DdrXoverMode;           // DDR 2.2 Mode
    // For RAS
    UINT8                   bootMode;
    UINT8                   OutClusterOnDieEn; // Whether RC enabled COD support
    UINT8                   OutSncEn;
    UINT8                   OutNumOfCluster;
    UINT8                   imcEnabled[MAX_SOCKET][MAX_IMC];
    UINT16                  LlcSizeReg;
    UINT8                   chEnabled[MAX_SOCKET][MAX_CH];
    UINT8                   memNode[MC_MAX_NODE];
    UINT8                   IoDcMode;
    UINT8                   ReservedBC;
} SYSTEM_STATUS;

typedef struct {
    PLATFORM_DATA           PlatformData;
    SYSTEM_STATUS           SystemStatus;
    UINT32                  OemValue;
} IIO_UDS;
#pragma pack()

#endif
