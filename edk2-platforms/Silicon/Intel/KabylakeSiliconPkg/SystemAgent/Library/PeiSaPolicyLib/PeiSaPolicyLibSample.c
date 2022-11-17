/** @file
  This file provides services for Sample PEI policy default initialization.

Copyright (c) 2017 - 2020 Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <SaPolicyCommon.h>
#include "PeiSaPolicyLibrary.h"
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/SmbusLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MmPciLib.h>
#include <Library/MemoryAllocationLib.h>
#include "MrcOemPlatform.h"
#include <Library/GpioLib.h>
#include <GpioPinsSklH.h>
#include <Library/CpuPlatformLib.h>
#include <Library/RngLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/ConfigBlockLib.h>

//
// DQ byte mapping to CMD/CTL/CLK, from the CPU side - for SKL RVP, SKL SDS.
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mDqByteMapSkl[2][6][2] = {
  // Channel 0:
  {
    { 0x0F, 0xF0 }, // CLK0 goes to package 0 - Bytes[3:0], CLK1 goes to package 1 - Bytes[7:4]
    { 0x00, 0xF0 }, // CmdN does not have CAA, CAB goes to Bytes[7:4]
    { 0x0F, 0xF0 }, // CmdS CAA goes to Bytes[3:0], CmdS CAB goes to Byte[7:4]
    { 0x0F, 0x00 }, // CKE CAA goes to Bytes[3:0], CKE does not have CAB
    { 0xFF, 0x00 }, // CTL (CS) goes to all bytes
    { 0xFF, 0x00 }  // CA Vref is one for all bytes
  },
  // Channel 1:
  {
    { 0x33, 0xCC }, // CLK0 goes to package 0 - Bytes[3:0], CLK1 goes to package 1 - Bytes[7:4]
    { 0x00, 0xCC }, // CmdN does not have CAA, CAB goes to Bytes[7:4]
    { 0x33, 0xCC }, // CmdS CAA goes to Bytes[3:0], CmdS CAB goes to Byte[7:4]
    { 0x33, 0x00 }, // CKE CAA goes to Bytes[3:0], CKE does not have CAB
    { 0xFF, 0x00 }, // CTL (CS) goes to all bytes
    { 0xFF, 0x00 }  // CA Vref is one for all bytes
  }
};

//
// DQS byte swizzling between CPU and DRAM - for SKL RVP1, RVP3, RVP13
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mDqsMapCpu2DramSklRvp[2][8] = {
  { 0, 1, 3, 2, 4, 5, 6, 7 }, // Channel 0
  { 1, 0, 4, 5, 2, 3, 6, 7 }  // Channel 1
};

//
// Reference RCOMP resistors on motherboard - for SKL RVP1/RVP3
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT16 mRcompResistorSklRvp1[SA_MRC_MAX_RCOMP] = { 200, 81, 162 };
//
// RCOMP target values for RdOdt, WrDS, WrDSCmd, WrDSCtl, WrDSClk - for SKL RVP1/RVP3
//
GLOBAL_REMOVE_IF_UNREFERENCED const UINT16 mRcompTargetSklRvp1[SA_MRC_MAX_RCOMP_TARGETS] = { 100, 40, 40, 23, 40 };

/**
  Hynix H9CCNNN8JTMLAR-NTM_178b_DDP LPDDR3, 4Gb die (128Mx32), x32
  or Elpida  EDF8132A1MC-GD-F
  or Samsung K4E8E304EB-EGCE
  1600, 12-15-15-34
  2 rank per channel, 2 SDRAMs per rank, 4x4Gb = 2GB total per channel
**/
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 mSkylakeRvp3Spd[] = {
  0x24,                                 ///< 0   Number of Serial PD Bytes Written / SPD Device Size
  0x20,                                 ///< 1   SPD Revision
  0x0F,                                 ///< 2   DRAM Device Type
  0x0E,                                 ///< 3   Module Type
  0x14,                                 ///< 4   SDRAM Density and Banks: 8 Banks, 4 Gb SDRAM density
  0x11,                                 ///< 5   SDRAM Addressing: 14 Rows, 10 Columns
  0x95,                                 ///< 6   SDRAM Package Type: DDP, 1 Channel per die, Signal Loading Matrix 1
  0x00,                                 ///< 7   SDRAM Optional Features
  0x00,                                 ///< 8   SDRAM Thermal and Refresh Options
  0x00,                                 ///< 9   Other SDRAM Optional Features
  0x00,                                 ///< 10  Reserved - must be coded as 0x00
  0x03,                                 ///< 11  Module Nominal Voltage, VDD
  0x0B,                                 ///< 12  Module Organization, SDRAM width: 32 bits, 2 Ranks
  0x23,                                 ///< 13  Module Memory Bus Width: 2 channels, 64 bit channel bus width
  0x00,                                 ///< 14  Module Thermal Sensor
  0x00,                                 ///< 15  Extended Module Type
  0x00,                                 ///< 16  Reserved - must be coded as 0x00
  0x00,                                 ///< 17  Timebases
  0x0A,                                 ///< 18  SDRAM Minimum Cycle Time (tCKmin)
  0xFF,                                 ///< 19  SDRAM Minimum Cycle Time (tCKmax)
  0x54,                                 ///< 20  CAS Latencies Supported, First Byte (tCk): 12 10 8
  0x00,                                 ///< 21  CAS Latencies Supported, Second Byte
  0x00,                                 ///< 22  CAS Latencies Supported, Third Byte
  0x00,                                 ///< 23  CAS Latencies Supported, Fourth Byte
  0x78,                                 ///< 24  Minimum CAS Latency Time (tAAmin)
  0x00,                                 ///< 25  Read and Write Latency Set Options
  0x90,                                 ///< 26  Minimum RAS# to CAS# Delay Time (tRCDmin)
  0xA8,                                 ///< 27  Minimum Row Precharge Delay Time for all banks (tRPab)
  0x90,                                 ///< 28  Minimum Row Precharge Delay Time per bank (tRPpb)
  0x10,                                 ///< 29  Minimum Refresh Recovery Delay Time for all banks (tRFCab), Least Significant Byte
  0x04,                                 ///< 30  Minimum Refresh Recovery Delay Time for all banks (tRFCab), Most Significant Byte
  0xE0,                                 ///< 31  Minimum Refresh Recovery Delay Time for per bank (tRFCpb), Least Significant Byte
  0x01,                                 ///< 32  Minimum Refresh Recovery Delay Time for per bank (tRFCpb), Most Significant Byte
  0, 0, 0, 0, 0, 0, 0,                  ///< 33 - 39
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 40 - 49
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 50 - 59
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 60 - 69 Connector to SDRAM Bit Mapping
  0, 0, 0, 0, 0, 0, 0, 0,               ///< 70 - 77 Connector to SDRAM Bit Mapping
  0, 0,                                 ///< 78 - 79
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 80 - 89
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 90 - 99
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 100 - 109
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 110 - 119
  0x00,                                 ///< 120 Fine Offset for Minimum Row Precharge Delay Time per bank (tRPpb)
  0x00,                                 ///< 121 Fine Offset for Minimum Row Precharge Delay Time for all banks (tRPab)
  0x00,                                 ///< 122 Fine Offset for Minimum RAS# to CAS# Delay Time (tRCDmin)
  0x00,                                 ///< 123 Fine Offset for Minimum CAS Latency Time (tAAmin)
  0x7F,                                 ///< 124 Fine Offset for SDRAM Minimum Cycle Time (tCKmax)
  0x00,                                 ///< 125 Fine Offset for SDRAM Minimum Cycle Time (tCKmin)
  0x00,                                 ///< 126 CRC A
  0x00,                                 ///< 127 CRC B
  0, 0,                                 ///< 128 - 129
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 130 - 139
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 140 - 149
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 150 - 159
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 160 - 169
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 170 - 179
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 180 - 189
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 190 - 199
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 200 - 209
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 210 - 219
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 220 - 229
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 230 - 239
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 240 - 249
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 250 - 259
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 260 - 269
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 270 - 279
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 280 - 289
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 290 - 299
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 300 - 309
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 310 - 319
  0x00,                                 ///< 320 Module Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 321 Module Manufacturer ID Code, Most Significant Byte
  0x00,                                 ///< 322 Module Manufacturing Location
  0x00,                                 ///< 323 Module Manufacturing Date Year
  0x00,                                 ///< 324 Module Manufacturing Date Week
  0x55,                                 ///< 325 Module Serial Number A
  0x00,                                 ///< 326 Module Serial Number B
  0x00,                                 ///< 327 Module Serial Number C
  0x00,                                 ///< 328 Module Serial Number D
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 329 - 333 Module Part Number: Unused bytes coded as ASCII Blanks (0x20)
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 334 - 338 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 339 - 343 Module Part Number
  0x20, 0x20, 0x20, 0x20, 0x20,         ///< 344 - 348 Module Part Number
  0x00,                                 ///< 349 Module Revision Code
  0x00,                                 ///< 350 DRAM Manufacturer ID Code, Least Significant Byte
  0x00,                                 ///< 351 DRAM Manufacturer ID Code, Most Significant Byte
  0x00,                                 ///< 352 DRAM Stepping
  0, 0, 0, 0, 0, 0, 0,                  ///< 353 - 359
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 360 - 369
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 370 - 379
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 380 - 389
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 390 - 399
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 400 - 409
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 410 - 419
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 420 - 429
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 430 - 439
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 440 - 449
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 450 - 459
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 460 - 469
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 470 - 479
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 480 - 489
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 490 - 499
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,         ///< 500 - 509
  0, 0                                  ///< 510 - 511
};

#define SaIoRead8    IoRead8
#define SaIoRead16   IoRead16
#define SaIoRead32   IoRead32
#define SaIoWrite8   IoWrite8
#define SaIoWrite16  IoWrite16
#define SaIoWrite32  IoWrite32
#define SaCopyMem    CopyMem
#define SaSetMem     SetMem
#define SaLShiftU64  LShiftU64
#define SaRShiftU64  RShiftU64
#define SaMultU64x32 MultU64x32

/**
  SaLoadSamplePolicyPreMem - Load some policy default for reference board.

  @param[in] ConfigBlockTableAddress    The pointer for SA config blocks

**/
VOID
SaLoadSamplePolicyPreMem (
  IN VOID           *ConfigBlockTableAddress
  )
{
  SA_FUNCTION_CALLS     *MemCall;
  EFI_STATUS            Status;
  MEMORY_CONFIG_NO_CRC  *MemConfigNoCrc;

  MemConfigNoCrc = NULL;
  Status = GetConfigBlock (ConfigBlockTableAddress, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
  ASSERT_EFI_ERROR (Status);

  if (MemConfigNoCrc == NULL) {
    return;
  }

  DEBUG ((DEBUG_INFO, "Applying Sample policy defaults for RVP3\n"));
  MemCall                       = &MemConfigNoCrc->SaCall;
  MemCall->IoRead8              = &SaIoRead8;
  MemCall->IoRead16             = &SaIoRead16;
  MemCall->IoRead32             = &SaIoRead32;
  MemCall->IoWrite8             = &SaIoWrite8;
  MemCall->IoWrite16            = &SaIoWrite16;
  MemCall->IoWrite32            = &SaIoWrite32;
  MemCall->MmioRead8            = &MmioRead8;
  MemCall->MmioRead16           = &MmioRead16;
  MemCall->MmioRead32           = &MmioRead32;
  MemCall->MmioRead64           = &SaMmioRead64;
  MemCall->MmioWrite8           = &MmioWrite8;
  MemCall->MmioWrite16          = &MmioWrite16;
  MemCall->MmioWrite32          = &MmioWrite32;
  MemCall->MmioWrite64          = &SaMmioWrite64;
  MemCall->SmbusRead8           = &SmBusReadDataByte;
  MemCall->SmbusRead16          = &SmBusReadDataWord;
  MemCall->SmbusWrite8          = &SmBusWriteDataByte;
  MemCall->SmbusWrite16         = &SmBusWriteDataWord;
  MemCall->GetPciDeviceAddress  = &GetPciDeviceAddress;
  MemCall->GetPcieDeviceAddress = &GetPcieDeviceAddress;
  MemCall->GetRtcTime           = &GetRtcTime;
  MemCall->GetCpuTime           = &GetCpuTime;
  MemCall->CopyMem              = &SaCopyMem;
  MemCall->SetMem               = &SaSetMem;
  MemCall->SetMemWord           = &SetMemWord;
  MemCall->SetMemDword          = &SetMemDword;
  MemCall->LeftShift64          = &SaLShiftU64;
  MemCall->RightShift64         = &SaRShiftU64;
  MemCall->MultU64x32           = &SaMultU64x32;
  MemCall->DivU64x64            = &DivU64x64Remainder;
  MemCall->GetSpdData           = &GetSpdData;
  MemCall->GetRandomNumber      = &GetRandomNumber32;
  MemCall->CpuMailboxRead       = &MailboxRead;
  MemCall->CpuMailboxWrite      = &MailboxWrite;
  MemCall->GetMemoryVdd         = &GetMemoryVdd;
  MemCall->SetMemoryVdd         = &SetMemoryVdd;
  MemCall->CheckPoint           = &CheckPoint;
  MemCall->DebugHook            = &DebugHook;
  MemCall->DebugPrint           = &SaDebugPrint;
  MemCall->GetRtcCmos           = &PeiRtcRead;
  MemCall->ReadMsr64            = &AsmReadMsr64;
  MemCall->WriteMsr64           = &AsmWriteMsr64;
  MemCall->MrcReturnFromSmc     = &ReturnFromSmc;
  MemCall->MrcDramReset         = &SaDramReset;

  //
  // RCOMP resistors and target values: board-dependent
  //
  CopyMem ((VOID *) MemConfigNoCrc->RcompData->RcompResistor, mRcompResistorSklRvp1, sizeof (mRcompResistorSklRvp1));
  CopyMem ((VOID *) MemConfigNoCrc->RcompData->RcompTarget,   mRcompTargetSklRvp1,   sizeof (mRcompTargetSklRvp1));

  CopyMem ((VOID *) MemConfigNoCrc->SpdData->SpdData[0][0], mSkylakeRvp3Spd, sizeof (mSkylakeRvp3Spd));
  CopyMem ((VOID *) MemConfigNoCrc->SpdData->SpdData[1][0], mSkylakeRvp3Spd, sizeof (mSkylakeRvp3Spd));
  CopyMem ((VOID *) MemConfigNoCrc->DqByteMap, mDqByteMapSkl, sizeof (UINT8) * SA_MC_MAX_CHANNELS * SA_MRC_ITERATION_MAX *2);
  CopyMem ((VOID *) MemConfigNoCrc->DqsMap, mDqsMapCpu2DramSklRvp, sizeof (UINT8) * SA_MC_MAX_CHANNELS * SA_MC_MAX_BYTES_NO_ECC);
}

