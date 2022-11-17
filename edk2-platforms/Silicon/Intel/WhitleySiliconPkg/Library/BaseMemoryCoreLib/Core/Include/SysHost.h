/** @file
  SysHost.h

  @copyright
  Copyright 2006 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SYS_HOST_H_
#define _SYS_HOST_H_

#include "SysHostChipCommon.h"



#define MAX_LINE 256

#define RC_SIM_FASTCADB 0



typedef struct sysHost SYSHOST, *PSYSHOST;

#include "DataTypes.h"
#include "SysHostChipCommon.h"
#include "PlatformHost.h"
#include "MemHost.h"
#include <Ppi/MemoryPolicyPpi.h>

///
/// Enhanced Warning Log Header
///
typedef struct {
  EFI_GUID  EwlGuid;      /// GUID that uniquely identifies the EWL revision
  UINT32    Size;         /// Total size in bytes including the header and buffer
  UINT32    FreeOffset;   /// Offset of the beginning of the free space from byte 0
                          /// of the buffer immediately following this structure
                          /// Can be used to determine if buffer has sufficient space for next entry
  UINT32    Crc;          /// 32-bit CRC generated over the whole size minus this crc field
                          /// Note: UEFI 32-bit CRC implementation (CalculateCrc32) (References [7])
                          /// Consumers can ignore CRC check if not needed.
  UINT32    Reserved;     /// Reserved for future use, must be initialized to 0
} EWL_HEADER;

///
/// Enhanced Warning Log Spec defined data log structure
///
typedef struct {
  EWL_HEADER Header;          /// The size will vary by implementation and should not be assumed
  UINT8      Buffer[4 * 1024];  /// The spec requirement is that the buffer follow the header
} EWL_PUBLIC_DATA;

///
/// EWL private data structure.  This is going to be implementation dependent
///   When we separate OEM hooks via a PPI, we can remove this
///
typedef struct {
  UINT32            bufSizeOverflow;  // Number of bytes that could not be added to buffer
  UINT32            numEntries;       // Number of entries currently logged
  EWL_PUBLIC_DATA   status;           // Spec defined EWL
} EWL_PRIVATE_DATA;

#pragma pack(1)

///
/// System NVRAM structure
//
struct sysNvram {
  struct memNvram     mem;
  struct commonNvram  common;
}; //struct sysNvram

#pragma pack()

//
// -----------------------------------------------------------------------------
// Variable structures
//

typedef struct CpuidRegisterInfo {
  UINT32 Eax;
  UINT32 Ebx;
  UINT32 Ecx;
  UINT32 Edx;
} CPUID_REGISTER_INFO;

typedef struct processorCommon {
  UINT32  capid0;
  UINT32  capid1;
  UINT32  capid2;
  UINT32  capid3;
  UINT32  capid4;
  UINT32  capid5;
  UINT32  capid6lo;
  UINT32  capid6hi;
  CPUID_REGISTER_INFO  ExtCpuid7;
  CPUID_REGISTER_INFO  ExtCpuid1B;
} PROCESSOR_COMMON;

///
/// Common variable structure
///
struct commonVar {
  RC_VERSION                RcVersion;              ///< Version of the reference code last executed
  UINT32                    rcStatus;               ///< Non-zero indicates fatal error
  UINT8                     chopType[MAX_SOCKET];   ///< HCC, MCC, LCC, MCC-DE, LCC-DE (HSX family only)
  UINT8                     sbsp;                   ///< Non-zero value indicates that the socket is System BSP
  UINT16                    pmBase;                 ///< Power Management Base Address
  UINT32                    tohmLimit;
  UINT32                    JumpBuffer;

#ifdef COMPRESS_RC
  UINT32                    rcSrc;                  ///< Decompression source code pointer
  UINT32                    rcDest;                 ///< Decompression destination pointer
  UINT32                    rcDecompressSourceAddr; ///< Decompression routine address of type func(UINT8*Src, UINT8*Dest);
#endif  // #ifdef COMPRESS_RC

  UINT32                    heapBase;
  UINT32                    heapSize;

  UINT32                    oemVariable;
  EWL_PRIVATE_DATA          ewlPrivateData;         // implementation data for EWL
  struct processorCommon    procCom[MAX_SOCKET];
  UINT32                    MicroCodeRev;

#define MAX_PROMOTE_WARN_LIMIT  90
  UINT32                    promoteWarnLimit;
  UINT16                    promoteWarnList[MAX_PROMOTE_WARN_LIMIT];
  UINT32                    printfDepth;            ///< handle nested calls to get/releasePrintFControl
  UINT32                    meRequestedSize;         /// Size of the memory range requested by ME FW, in MB
  UINT32                    ieRequestedSize;         /// Size of the memory range requested by IE FW, in MB
};  //struct commonVar

///
/// System Variable structure
///
struct sysVar {
  struct memVar     mem;
  struct commonVar  common;
};  //struct sysVar

// Bit definitions for commonSetup.options
//                   ; PROMOTE_WARN_EN enables warnings to be treated as fatal error
//                   ; PROMOTE_MRC_WARN_EN enables MRC warnings to be treated as fatal error
//                   ; HALT_ON_ERROR_EN enables errors to loop forever
//                   ; HALT_ON_ERROR_AUTO auto reset with Maximum Serial port debug message level when fatal error is encountered.
#define PROMOTE_WARN_EN           BIT0
#define PROMOTE_MRC_WARN_EN       BIT1
#define HALT_ON_ERROR_EN          BIT2
#define HALT_ON_ERROR_AUTO        BIT3

typedef union _RMT_FLAGS {
  UINT8   Data;
  struct {
    UINT8   EnableShortFormat         :1;
    UINT8   SkipPerBitMargin          :1;
    UINT8   SkipDisplayPerBitEyes     :1;
    UINT8   SkipDisplayPerBitMargins  :1;
    UINT8   SkipRmtRxDqs              :1;
    UINT8   SkipRmtRxVref             :1;
    UINT8   SkipRmtTxDq               :1;
    UINT8   SkipRmtTxVref             :1;
  } Bits;
} RMT_FLAGS;

///
/// System Host (root structure)
///
struct sysHost {
  struct sysVar         var;    ///< variable, volatile data
  struct sysNvram       nvram;  ///< variable, non-volatile data for S3 and fast path
  UINT8   ChannelModeOperation;
  UINT32  cpuFlows; ///< Which flows to take
  UINT32  ktiFlows;
  UINT32  qpiFlows;
  UINT32  opioFlows;
  UINT32  memFlows;
  UINT32  memFlowsExt;
  UINT32  memFlowsExt2;
  UINT32  memFlowsExt3;


  UINT32  MrcHooksServicesPpi;
  UINT32  MrcHooksChipServicesPpi;

  BOOLEAN PrintCsr;                               // Enable / disable printing CSR
  BOOLEAN MtrrCheck;                              // Check number of used MTRR's
  INT32   DdrioUltSupport;
  RMT_FLAGS               RmtFlags;
};

#endif // #ifndef _SYS_HOST_H_
