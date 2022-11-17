/** @file
*
*  Copyright (c) 2015, Hisilicon Limited. All rights reserved.
*  Copyright (c) 2015, Linaro Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef _PROCESSOR_SUBCLASS_DRIVER_H
#define _PROCESSOR_SUBCLASS_DRIVER_H

#include <Uefi.h>
#include <Protocol/Smbios.h>
#include <IndustryStandard/SmBios.h>
#include <Library/HiiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/PcdLib.h>
#include <PlatformArch.h>
#include <Library/PlatformSysCtrlLib.h>
#include <Library/OemMiscLib.h>
#include <Library/ArmLib.h>

//
// This is the generated header file which includes whatever needs to be exported (strings + IFR)
//

extern UINT8 ProcessorSubClassStrings[];

#define CPU_CACHE_L1_Instruction        0
#define CPU_CACHE_L1_Data               1
#define CPU_CACHE_L2                    2
#define CPU_CACHE_L3                    3
#define MAX_CACHE_LEVEL                 4

#define EXTERNAL_CLOCK                  50     //50 MHz
#define CPU_MAX_SPEED                   2100   //2.1G

//
// Cache Info
//
typedef struct {
  UINT16                    InstalledSize;    //In KB
  CACHE_TYPE_DATA           SystemCacheType;
  CACHE_ASSOCIATIVITY_DATA  Associativity;
} CACHE_INFO;

//
// Cache Configuration
//
typedef union {
  struct {
    UINT16    Level           :3;
    UINT16    Socketed        :1;
    UINT16    Reserved1       :1;
    UINT16    Location        :2;
    UINT16    Enable          :1;
    UINT16    OperationalMode :2;
    UINT16    Reserved2       :6;
  } Bits;
  UINT16 Data;
}CACHE_CONFIGURATION;

//
// Processor Status
//
typedef union {
  struct {
    UINT8 CpuStatus       :3; // Indicates the status of the processor.
    UINT8 Reserved1       :3; // Reserved for future use. Should be set to zero.
    UINT8 SocketPopulated :1; // Indicates if the processor socket is populated or not.
    UINT8 Reserved2       :1; // Reserved for future use. Should be set to zero.
  } Bits;
  UINT8 Data;
}PROCESSOR_STATUS_DATA;

//
// Processor Characteristics
//
typedef union {
  struct {
    UINT16 Reserved                 :1;
    UINT16 Unknown                  :1;
    UINT16 Capable64Bit             :1;
    UINT16 MultiCore                :1;
    UINT16 HardwareThread           :1;
    UINT16 ExecuteProtection        :1;
    UINT16 EnhancedVirtualization   :1;
    UINT16 PowerPerformanceControl  :1;
    UINT16 Reserved2                :8;
  } Bits;
  UINT16 Data;
} PROCESSOR_CHARACTERISTICS_DATA;

#endif
