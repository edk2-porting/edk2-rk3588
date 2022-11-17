/** @file
  System Infor Var Hearder File

  @copyright
  Copyright 2017 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __SYSTEM_INFO_VAR_INCLUDES__
#define __SYSTEM_INFO_VAR_INCLUDES__

#include <MemCommon.h>
#include <Upi/KtiSi.h>
#include <Pi/PiBootMode.h>

#define SYS_INFO_NVRAM_VAR_NAME L"InfoVarNvramData"

#pragma pack (push,1)

typedef enum BootMode {
  NormalBoot  = 0,  // Normal path through RC with full init, mem detection, init, training, etc.
                    //    Some of these MRC specific init routines can be skipped based on MRC input params
                    //    in addition to the sub-boot type (WarmBoot, WarmBootFast, etc).
  S3Resume    = 1   // S3 flow through RC. Should do the bare minimum required for S3
                    //    init and be optimized for speed.
} BootMode;

//
// This is used to determine what type of die is connected to a UPI link
//
typedef enum {
  UpiConnectionTypeCpu,
  UpiConnectionTypePcieGen4,
  UpiConnectionTypeFpga,
  UpiConnectionTypeMax
} UPI_CONNECTION_TYPE;

typedef struct {
  UINT16 stackPresentBitmap[MAX_SOCKET];           ///< bitmap of present stacks per socket
  UINT8  StackBus[MAX_SOCKET][MAX_LOGIC_IIO_STACK];///< Bus of each stack
  UINT32 StackMmiol[MAX_SOCKET][MAX_IIO_STACK];    ///< mmiol of each IIO stack, if it works as CXL, the mmiol base is RCRBBAR
  UINT8  SocketFirstBus[MAX_SOCKET];
  UINT8  Socket10nmUboxBus0[MAX_SOCKET];           //10nm CPU use only
  UINT8  SocketLastBus[MAX_SOCKET];
  UINT8  segmentSocket[MAX_SOCKET];
  UINT8  KtiPortCnt;
  UINT32 socketPresentBitMap;
  UINT32 SecondaryDieBitMap;
  UINT32 FpgaPresentBitMap;
  UINT32 mmCfgBase;
  UINT32 mmCfgBaseH[MAX_SOCKET];
  UINT32 mmCfgBaseL[MAX_SOCKET];
  UINT8  DdrMaxCh;
  UINT8  DdrMaxImc;
  UINT8  DdrNumChPerMc;
  UINT8  imcEnabled[MAX_SOCKET][MAX_IMC];
  UINT8  mcId[MAX_SOCKET][MAX_CH];
  MRC_MST MemSsType[MAX_SOCKET];
  UINT32  MmioBar[MAX_SOCKET][TYPE_MAX_MMIO_BAR];
  UINT8   HbmMaxCh;
  UINT8   HbmMaxIoInst;
  UINT8   HbmNumChPerMc;
  UINT8   HbmNumChPerIo;
  UINT32  LastCsrAddress[2];
  UINT32  LastCsrMmioAddr;
  UINT8   CsrCachingEnable;
  UINT32  LastCsrMcAddress[2];
  UINT32  LastCsrMcMmioPhyAddr;
  UINT8   CsrPciBarCachingEnable;
  UINT32  LastCsrPciBarAddr[2];
  UINT64  LastCsrPciBarPhyAddr;
  UINT32  LastSBPortId[MAX_SOCKET];
  UPI_CONNECTION_TYPE         UpiConnectionType[MAX_SOCKET];
  BOOLEAN PostedCsrAccessAllowed;                   // SW is allowed to use posted CSR writes method when TRUE
  BOOLEAN PostedWritesEnabled;                      // All CSR writes use posted method when TRUE, non-posted when FALSE
  BOOLEAN DataPopulated;                            // CPU_CSR_ACCESS_VAR is unavailable when FALSE
  BOOLEAN HbmSku;
  UINT8   SocketConfig;
  UINT8   HcxType[MAX_SOCKET];
} CPU_CSR_ACCESS_VAR;

typedef struct {
  UINT32 MeRequestedSizeNv;
  UINT32 MeRequestedAlignmentNv;
  UINT32 IeRequestedSizeNv;
  UINT32 IeRequestedAlignmentNv;
  UINT8  SbspSocketIdNv;
} SYS_INFO_VAR_NVRAM;

#pragma pack (pop)

#endif //#ifndef __SYSTEM_INFO_VAR_INCLUDES__
