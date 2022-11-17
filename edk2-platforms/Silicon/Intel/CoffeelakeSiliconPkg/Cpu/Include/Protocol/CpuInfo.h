/** @file
  Protocol used to report CPU information

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPU_INFO_H_
#define _CPU_INFO_H_

#include <CpuDataStruct.h>

typedef struct _CPU_INFO_PROTOCOL CPU_INFO_PROTOCOL;


extern EFI_GUID gCpuInfoProtocolGuid;

//
// DXE_CPU_INFO_PROTOCOL revisions
//
#define CPU_INFO_PROTOCOL_REVISION 1

//
// Processor feature definitions.
//
#define TXT_SUPPORT        BIT0
#define VMX_SUPPORT        BIT1
#define XD_SUPPORT         BIT2
#define DCA_SUPPORT        BIT3
#define X2APIC_SUPPORT     BIT4
#define AES_SUPPORT        BIT5
#define HT_SUPPORT         BIT6
#define DEBUG_SUPPORT      BIT7
#define DEBUG_LOCK_SUPPORT BIT8
#define PROC_TRACE_SUPPORT BIT9
#define HDC_SUPPORT        BIT10


#pragma pack(1)
///
/// Cache descriptor information
///
typedef struct {
  UINT8   Desc;                                    ///< Cache Descriptor
  UINT8   Level;                                   ///< Cache Level
  UINT8   Type;                                    ///< Cache Type. 0: Data, 1: Instruction, 3: Unified
  UINT32  Size;                                    ///< Cache Size.
  UINT16  Associativity;                           ///< Cache Ways of Associativity.
} CACHE_DESCRIPTOR_INFO;

///
/// Processor information
///
typedef struct {
  UINT32                CpuSignature;               ///< Processor signature and version information.
  UINT64                Features;                   ///< Features availability in the CPU based on reading ECX after doing Asmcpuid(EAX=1).
  CHAR8                 *BrandString;               ///< Processor Brand String.
  UINT8                 NumSupportedCores;          ///< Total Number of Supported Cores in CPU Package. If Dual core, 2 cores.
  UINT8                 NumSupportedThreadsPerCore; ///< Number of Supported Threads per Core.
  UINT8                 NumCores;                   ///< Number of Enabled or Active Cores.
  UINT8                 NumHts;                     ///< Number of Enabled Threads per Core. This will be 1 or 2.
  UINT32                IntendedFreq;               ///< Maximum non turbo ratio in MHz
  UINT32                ActualFreq;                 ///< Actual frequency in MHz
  UINT32                Voltage;                    ///< Current operating voltage.
  CACHE_DESCRIPTOR_INFO *CacheInfo;                 ///< Cache descriptor information.
  UINT8                 MaxCacheSupported;          ///< Maximum cache supported.
  UINT8                 SmmbaseSwSmiNumber;         ///< Software SMI Number from Smbase. @Note: This is unused.
  UINT16                NumberOfPStates;            ///< Number of P-States.
} CPU_INFO;

///
/// This HOB is data structure representing two different address location in SMRAM to hold SMRAM CPU DATA.
///
typedef struct {
  EFI_PHYSICAL_ADDRESS LockBoxData;  ///< First location (address) of SMRAM CPU DATA.
  EFI_PHYSICAL_ADDRESS SmramCpuData; ///< Second location (Address) of SMRAM CPU DATA.
  UINT64               LockBoxSize;  ///< Size of SMRAM CPU DATA.
} SMRAM_CPU_INFO;

///
/// SGX Information
///
typedef struct {
  UINT64  SgxSinitNvsData;  ///< Sinit SE SVN Version saved and passed back in next boot
} SGX_INFO;

#pragma pack()

///
/// This protocol provides information about the common features available in this CPU.
///
struct _CPU_INFO_PROTOCOL {
  /**
  Revision for the protocol structure.
  Any backwards compatible changes to this protocol will result in an update in the revision number.
  Major changes will require publication of a new protocol

  <b>Revision 1</b>:
   -  Initial version
  **/
  UINT8  Revision;
  /**
  CPU Supported Feature.
   - BIT0:  If set then processor supports TXT.
   - BIT1:  If set then processor supports virtual mode extensions.
   - BIT2:  If set then processor supports execute disable bit.
   - BIT3:  If set then processor supports DCA.
   - BIT4:  If set then processor supports X2APIC.
   - BIT5:  If set then processor supports Advanced Encryption Standard.
   - BIT6:  If set then processor supports hyperthreading.
   - BIT7:  If set then processor supports debug interface.
   - BIT8:  If set then processor supports debug interface lock.
   - BIT9:  If set then processor supports processor trace.
   - BIT10: If Set then processor supports supports HDC.
  **/
  UINT64         CpuCommonFeatures;
  CPU_INFO       *CpuInfo;      ///< Processor Basic Information
  SMRAM_CPU_INFO *SmramCpuInfo; ///< SMRAM CPU Information
  SGX_INFO       *SgxInfo;      ///< SGX Information
};

#endif
