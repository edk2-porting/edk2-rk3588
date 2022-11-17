/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _IIO_PLATFORM_DATA_H_
#define _IIO_PLATFORM_DATA_H_

#include <SysRegs.h>
#include <KtiSi.h>
#include <IioRegs.h>
#include <IioConfig.h>
#ifndef MINIBIOS_BUILD
#ifndef IA32
#include <Uefi.h>
#include <PiDxe.h>
#include <Protocol/CpuCsrAccess.h>
#define IIO_CPU_CSR_ACCESS  EFI_CPU_CSR_ACCESS_PROTOCOL
#endif
#endif

#define IIO_HANDLE          VOID *
#define IIO_STATUS          UINT32

typedef struct {
  UINT8   Register;
  UINT8   Function;
  UINT8   Device;
  UINT8   Bus;
  UINT32  ExtendedRegister;
} PCI_ROOT_BRIDGE_PCI_ADDRESS;

typedef enum {
  DmiTypeVc0,
  DmiTypeVc1,
  DmiTypeVcm,
  MaxDmiVcType
} DMI_VC_TYPE;

typedef enum {
  IioDmiTc0,
  IioDmiTc1,
  IioDmiTc2,
  IioDmiTc3,
  IioDmiTc4,
  IioDmiTc5,
  IioDmiTc6,
  IioDmiTc7,
  IioMaxDmiTc
} IIO_DMI_TC;

typedef enum {
  IIOInitPhase1 = 1,
  IIOInitPhase2 = 2,
  IIOInitPhase3 = 4,
} IIO_INIT_PHASE;

typedef enum {
  IioBeforeBifurcation,            // Point before IOU Bi-fucuation and link training, no generic inbound access at this point
  IioAfterBifurcation,             // Point immediately after IOU bifurcation and link training but before any PCIe root port initialization
  IioPortEnumeration,              // Point before Port initialization, no generic inbound access at this point
  IioPortEnumProgramMISCCTRL,      // Inside IioPortInit.PcieSlotInit
  IioEnumEnd,
  IioVtDPreEn,
  IioVtDInit,
  IioVtDEn,                        // At this point it has been decided to enable VtD through setup IioVtdInit.VtdInitialization
  IioPostInitEnd,                  // this is the last stage of IIO PCIe port init
  IioBeforeResources,              // At this point IIO Ports configuration has been completed
  IioAfterResources,               // At this point PCIe Resources allocation has been completed
  IioReadyToBoot
} IIO_INIT_ENUMERATION;


extern const CHAR* IioPortLabel[];

#define IIO_PORT_LABEL(x)   ( ((x) < NUMBER_PORTS_PER_SOCKET) ? (IioPortLabel[(x)]) : IioPortLabel[NUMBER_PORTS_PER_SOCKET] )

#pragma pack(1)

typedef union{
  struct{
   UINT32  Value;
   UINT32  ValueHigh;
  }Address32bit;
  UINT64 Address64bit;
}IIO_PTR_ADDRESS;

typedef struct {
  UINT32 Device;
  UINT32 Function;
  UINT32 RegOffset;
  UINT32 AndMask;
  UINT32 OrMask;
} PCI_OP_STRUCT;

typedef struct {
  UINT32 Instance;
  UINT32 RegOffset;
  UINT32 AndMask;
  UINT32 OrMask;
} CSR_ACCESS_OP_STRUCT;

typedef struct {
  UINT8  Isoc;
  UINT32 meRequestedSize;
  UINT8  Vc1_pri_en;
  UINT8 Isoc_Enable;
} ISOC_VC_TABLE_STRUCT;

/*
 * Following are the data structure defined to support multiple CBDMA types on a system
 */

typedef struct{
  UINT32  DcaSupported : 1;
  UINT32  NoSnoopSupported : 1;
  UINT32  RelaxOrderSupported : 1;
}CB_CONFIG_CAPABILITY;

typedef struct{
  UINT8                   CB_VER;
  UINT8                   BusNo;
  UINT8                   DevNo;
  UINT8                   FunNo;
  UINT8                   MaxNoChannels;
  CB_CONFIG_CAPABILITY    CBConfigCap;
}CBDMA_CONTROLLER;

typedef struct{
  CBDMA_CONTROLLER     CbDmaDevice;
}DMA_HOST;

// <<<< end of CBDMA data structures >>>>

typedef union {
struct {
    UINT32 Dev0 : 1;
    UINT32 Dev1 : 1;
    UINT32 Dev2 : 1;
    UINT32 Dev3 : 1;
    UINT32 Dev4 : 1;
    UINT32 Dev5 : 1;
    UINT32 Dev6 : 1;
    UINT32 Dev7 : 1;
    UINT32 Dev8 : 1;
    UINT32 Dev9 : 1;
    UINT32 Dev10 : 1;
    UINT32 Dev11 : 1;
    UINT32 Dev12 : 1;
    UINT32 Dev13 : 1;
    UINT32 Dev14 : 1;
    UINT32 Dev15 : 1;
    UINT32 Dev16 : 1;
    UINT32 Dev17 : 1;
    UINT32 Dev18 : 1;
    UINT32 Dev19 : 1;
    UINT32 Dev20 : 1;
    UINT32 Dev21 : 1;
    UINT32 Dev22 : 1;
    UINT32 Dev23 : 1;
    UINT32 Dev24 : 1;
    UINT32 Dev25 : 1;
    UINT32 Dev26 : 1;
    UINT32 Dev27 : 1;
    UINT32 Dev28 : 1;
    UINT32 Dev29 : 1;
    UINT32 Dev30 : 1;
    UINT32 Dev31 : 1;
  } Bits;
  UINT32 Data;
} DEVHIDE_FIELD;

typedef struct{
  UINT32 DevToHide[NUM_DEVHIDE_REGS];
} IIO_DEVFUNHIDE;

typedef struct{
  IIO_DEVFUNHIDE IioStackDevHide[MAX_IIO_STACK];
}IIO_DEVFUNHIDE_TABLE;

typedef struct {
    UINT8       CpuType;
    UINT8       CpuStepping;
    UINT8       CpuSubType;
    UINT8       SystemRasType;
    UINT8       IsocEnable;
    UINT8       EVMode;
    UINT32      meRequestedSize;
    UINT8       DmiVc[MaxDmiVcType];
    UINT8       DmiVcId[MaxDmiVcType];
    DMI_VC_TYPE DmiTc[IioMaxDmiTc];
    UINT8       PlatformType;
    UINT8       IOxAPICCallbackBootEvent;
    UINT8       RasOperation;
    UINT8       SocketUnderOnline;
    UINT8       CompletedReadyToBootEventServices;
    UINT8       SocketPresent[MaxIIO];
    UINT8       SocketBaseBusNumber[MaxIIO];
    UINT8       SocketLimitBusNumber[MaxIIO];
    UINT8       StackPresentBitmap[MaxIIO];
    UINT64_STRUCT    SegMmcfgBase[MaxIIO];
    UINT8       SegmentSocket[MaxIIO];
    UINT8       SocketStackPersonality[MaxIIO][MAX_IIO_STACK];
    UINT8       SocketStackBus[MaxIIO][MAX_IIO_STACK];
    UINT8       SocketStackBaseBusNumber[MaxIIO][MAX_IIO_STACK];
    UINT8       SocketStackLimitBusNumber[MaxIIO][MAX_IIO_STACK];
    UINT8       SocketPortBusNumber[MaxIIO][NUMBER_PORTS_PER_SOCKET];
    UINT8       StackPerPort[MaxIIO][NUMBER_PORTS_PER_SOCKET];
    UINT8       SocketUncoreBusNumber[MaxIIO];
    UINT32      PchIoApicBase;
    UINT32      PciResourceMem32Base[MaxIIO];
    UINT32      PciResourceMem32Limit[MaxIIO];
    UINT8       Pci64BitResourceAllocation;
    UINT32      StackPciResourceMem32Limit[MaxIIO][MAX_IIO_STACK];
    UINT32      VtdBarAddress[MaxIIO][MAX_IIO_STACK];
    UINT32      IoApicBase[MaxIIO][MAX_IIO_STACK];
    UINT32      RcBaseAddress;
    UINT64      PciExpressBase;
    UINT32      PmBase;
    UINT32      PchSegRegBaseAddress;
    UINT8       PcieRiser1Type;
    UINT8       PcieRiser2Type;
    UINT8       DmiVc1;
    UINT8       DmiVcm;
    UINT8       Emulation;
    UINT8       SkuPersonality[MAX_SOCKET];
    UINT8       VMDStackEnable[MaxIIO][MAX_IIO_STACK];
    UINT8       IODC;
    UINT8       MultiPch;
    UINT8       FpgaActive[MaxIIO];
} IIO_V_DATA;

typedef struct {
    UINT8       Device;
    UINT8       Function;
} IIO_PORT_INFO;

typedef struct {
    UINT8                     Valid;
    UINT8                     IioUplinkPortIndex;   //defines platform specific uplink port index (if any else FF)
    IIO_PORT_INFO             UplinkPortInfo;
}IIO_UPLINK_PORT_INFO;

typedef struct _INTEL_IIO_PORT_INFO {
    UINT8   Device;
    UINT8   Function;
    UINT8   RtoDevice;
    UINT8   RtoFunction;
    UINT8   RtoClusterDevice;
    UINT8   RtoClusterFunction;
    UINT8   RtoReutLinkSel;
    UINT8   SuperClusterPort;
} INTEL_IIO_PORT_INFO;

typedef struct _INTEL_DMI_PCIE_INFO {
    INTEL_IIO_PORT_INFO         PortInfo[NUMBER_PORTS_PER_SOCKET];
} INTEL_DMI_PCIE_INFO;

typedef struct _INTEL_IIO_PRELINK_DATA {
    INTEL_DMI_PCIE_INFO         PcieInfo;
    IIO_UPLINK_PORT_INFO        UplinkInfo[MaxIIO];
} INTEL_IIO_PRELINK_DATA;

typedef struct {
  UINT8       PciePortPresent[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortConfig[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortOwnership[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       CurrentPXPMap[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       MaxPXPMap[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       LinkedPXPMap[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       SpeedPXPMap[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       LaneReversedPXPMap[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortMaxWidth[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortNegWidth[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortNegSpeed[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  IIO_PTR_ADDRESS   PtrAddress;
  IIO_PTR_ADDRESS   PtrPcieTopology;
  UINT64       McastRsvdMemory;
  DMA_HOST     DMAhost[MaxIIO];
  UINT8        resetRequired;
} IIO_OUT_DATA;

typedef struct {
  IIO_V_DATA   IioVData;
  INTEL_IIO_PRELINK_DATA PreLinkData;
  IIO_OUT_DATA IioOutData;
} IIO_VAR;

typedef struct {
  IIO_CONFIG   SetupData;
  IIO_VAR      IioVar;
} IIO_GLOBALS;

#pragma pack()

#endif //_IIO_PLATFORM_DATA_H_
