/** @file
  This file provides required platform data structure for IOH.

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IIO_PLATFORM_DATA_H_
#define _IIO_PLATFORM_DATA_H_

#include <Upi/KtiSi.h>
#include <IioRegs.h>
#include <IioConfig.h>

typedef enum {
  DmiTypeVc0,
  DmiTypeVc1,
  DmiTypeVcm,
  MaxDmiVcType
} DMI_VC_TYPE;

typedef enum {
  DmiTypeTc0,
  DmiTypeTc1,
  DmiTypeTc2,
  DmiTypeTc3,
  DmiTypeTc4,
  DmiTypeTc5,
  DmiTypeTc6,
  DmiTypeTc7,
  MaxDmiTcType
}DMI_TC_TYPE;

#pragma pack(1)

typedef union {
  struct {
   UINT32  Value;
   UINT32  ValueHigh;
  } Address32bit;
  UINT64 Address64bit;
} IIO_PTR_ADDRESS;

/*
 * Following are the data structure defined to support multiple CBDMA types on a system
 */

typedef struct {
  UINT32  NoSnoopSupported : 1;
  UINT32  RelaxOrderSupported : 1;
} CB_CONFIG_CAPABILITY;

typedef struct {
  UINT8                   CB_VER;
  UINT8                   BusNo;
  UINT8                   DevNo;
  UINT8                   FunNo;
  UINT8                   MaxNoChannels;
  CB_CONFIG_CAPABILITY    CBConfigCap;
} CBDMA_CONTROLLER;

typedef struct {
  CBDMA_CONTROLLER     CbDmaDevice;
} DMA_HOST;

// <<<< end of CBDMA data structures >>>>

typedef struct {
  UINT8   LinkWidth;
  UINT8   LinkSpeed;
} IIO_DMI_DATA;

typedef struct {
    UINT8       SystemRasType;
    BOOLEAN     IsocEnable;
    UINT8       EVMode;
    UINT32      meRequestedSize;
    UINT32      ieRequestedSize;
    UINT8       DmiVc[MaxDmiVcType];
    UINT8       DmiVcId[MaxDmiVcType];
    UINT8       DmiTc[MaxDmiTcType];
    UINT8       PlatformType;
    UINT8       IOxAPICCallbackBootEvent;
    UINT8       RasOperation;
    UINT8       SocketUnderOnline;
    UINT8       CompletedReadyToBootEventServices;
    UINT8       SocketPresent[MaxIIO];
    UINT8       SocketBaseBusNumber[MaxIIO];
    UINT8       SocketLimitBusNumber[MaxIIO];
    UINT32      StackPresentBitmap[MaxIIO];
    UINT64_STRUCT    SegMmcfgBase[MaxIIO];
    UINT8       SegmentSocket[MaxIIO];
    UINT8       SocketStackPersonality[MaxIIO][MAX_IIO_STACK];
    UINT8       SocketStackBus[MaxIIO][MAX_IIO_STACK];
    UINT8       SocketStackBaseBusNumber[MaxIIO][MAX_IIO_STACK];
    UINT8       SocketStackLimitBusNumber[MaxIIO][MAX_IIO_STACK];
    UINT32      SocketStackMmiolBase[MaxIIO][MAX_IIO_STACK];
    UINT32      SocketStackMmiolLimit[MaxIIO][MAX_IIO_STACK];
    UINT8       SocketPortBusNumber[MaxIIO][NUMBER_PORTS_PER_SOCKET];
    UINT8       StackPerPort[MaxIIO][NUMBER_PORTS_PER_SOCKET];
    UINT8       SocketUncoreBusNumber[MaxIIO][MAX_UNCORE_STACK]; // 10nm only
    UINT32      PchIoApicBase;
    UINT32      PciResourceMem32Base[MaxIIO];
    UINT32      PciResourceMem32Limit[MaxIIO];
    UINT8       Pci64BitResourceAllocation;
    UINT32      StackPciResourceMem32Limit[MaxIIO][MAX_IIO_STACK];
    UINT32      VtdBarAddress[MaxIIO][MAX_IIO_STACK];
    UINT32      IoApicBase[MaxIIO][MAX_IIO_STACK];
    UINT32      RcBaseAddress;
    UINT64      PciExpressBase;
    UINT32      PmBase[MaxIIO];
    UINT32      PchSegRegBaseAddress[MaxIIO];
    UINT8       PcieRiser1Type;
    UINT8       PcieRiser2Type;
    UINT8       DmiVc1;
    UINT8       DmiVcm;
    UINT8       Emulation;
    UINT8       SkuPersonality[MAX_SOCKET];
    UINT8       StackPersonality[MaxIIO][MAX_IIO_STACK];
    UINT8       StackId[MaxIIO][MAX_IIO_STACK];
    UINT8       VMDStackEnable[MaxIIO][MAX_IIO_STACK];
    UINT8       IODC;
    UINT8       MultiPch;
    UINT8       FpgaActive[MaxIIO];
    UINT32      TraceHubMemBase;
    UINT8       DmiSocketMap;
    IIO_DMI_DATA DmiSocketData[MAX_SOCKET];
    UINT64      PciTrainingStartTime; // time in microseconds
} IIO_V_DATA;

typedef struct {
    UINT8       Device;
    UINT8       Function;
} IIO_PORT_INFO;

typedef struct {
    UINT8                     Valid;
    UINT8                     IioUplinkPortIndex;   //defines platform specific uplink port index (if any else FF)
    IIO_PORT_INFO             UplinkPortInfo;
} IIO_UPLINK_PORT_INFO;

typedef struct _INTEL_IIO_PORT_INFO {
    UINT8   Device;
    UINT8   Function;
    UINT8   Reserved137;
    UINT8   Reserved138;
    UINT8   Reserved139;
    UINT8   Reserved140;
    UINT8   Reserved141;
    UINT8   SuperClusterPort;
    UINT8   NtbDevice;
    UINT8   NtbFunction;
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
  UINT8       MaxPXPMap[MaxIIO * NUMBER_PORTS_PER_SOCKET];          // Max link width
  BOOLEAN     LaneReversedPXPMap[MaxIIO][MAX_IOU_PER_SOCKET];
  UINT8       PciePortMaxWidth[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortNegWidth[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       PciePortNegSpeed[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  UINT8       RetimerConnectCount[MaxIIO*NUMBER_PORTS_PER_SOCKET];
  IIO_PTR_ADDRESS   PtrAddress;
  IIO_PTR_ADDRESS   PtrPcieTopology;
  UINT64      McastRsvdMemory;
  DMA_HOST    DMAhost[MaxIIO];
  UINT8       resetRequired;
  UINT8       MaxPciePortNumberPerSocket[MaxIIO];

  //
  // IsSocketSmbEnabled and TimeoutOnVppOccured are needed only as a WA for SMB issue in socket
  //
  BOOLEAN     IsSocketSmbEnabled[MaxIIO];    // contains TRUE if socket smb controller was enabled for given IIO (socket)
  BOOLEAN     TimeoutOnVppOccurred[MaxIIO];   // contains TRUE if there was a timeout after VPP programming
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
