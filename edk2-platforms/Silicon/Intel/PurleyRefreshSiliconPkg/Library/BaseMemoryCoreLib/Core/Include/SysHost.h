/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _syshost_h
#define _syshost_h
//#define CCMRC       1


#define RC_SIM_FASTCADB 0



//
// Host reset states (0-7 are definable)
//
typedef enum BootMode {
  NormalBoot  = 0,  // Normal path through RC with full init, mem detection, init, training, etc.
                    //    Some of these MRC specific init routines can be skipped based on MRC input params
                    //    in addition to the sub-boot type (WarmBoot, WarmBootFast, etc).
  S3Resume    = 1   // S3 flow through RC. Should do the bare minimum required for S3
                    //    init and be optimized for speed.
} BootMode;


typedef struct sysHost SYSHOST, *PSYSHOST;

#include "DataTypes.h"
#include "SysHostChipCommon.h"
#include "Printf.h"
#include "PlatformHost.h"
#include "CpuHost.h"
#include "MemHost.h"
#include "KtiHost.h"
#include "UsbDebugPort.h"

#include "SysRegs.h"
#include "IioPlatformData.h"

//
// -------------------------------------
// Declarations and directives
// -------------------------------------
// Reference Code (RC) revision in BCD format:
// [31:24] = Major revision number
// [23:16] = Minor revision number
// [15:8] = Release Candidate number
//
#define CCMRC_REVISION  0x00500000

#define SUCCESS 0

#define SDBG_MIN        BIT0
#define SDBG_MAX        BIT1
#define SDBG_TRACE      BIT2
#define SDBG_MEM_TRAIN  BIT3 + SDBG_MAX
#define SDBG_TST        BIT4
#define SDBG_CPGC       BIT5
#define SDBG_RCWRITETAG BIT6
#define SDBG_REG_ACCESS BIT6                                  // Displays all register accesses.
#define SDBG_MINMAX     SDBG_MIN + SDBG_MAX

#define SDBG_BUF_ENABLE      1
#define SDBG_BUF_DISABLE     0
#define SDBG_BUF_EN_DEFAULT  SDBG_BUF_DISABLE                 // Default disable

#define SDBG_PIPE_ENABLE      1
#define SDBG_PIPE_DISABLE     0
#define SDBG_PIPE_DEFAULT     SDBG_PIPE_DISABLE

#define SDBG_PIPE_COMPRESS_ENABLE     1
#define SDBG_PIPE_COMPRESS_DISABLE    0
#define SDBG_PIPE_COMPRESS_DEFAULT    SDBG_PIPE_COMPRESS_DISABLE


//
// -----------------------------------------------------------------------------
// Variable structures
//

//
// Warning log
//
#define MAX_LOG 64

#define USB_BUF_LIMIT (4096-160)
#define USB_BUF_SIZE  (USB_BUF_LIMIT + 160)

//
// System previous boot error structure
//
#define MC_BANK_STATUS_REG    1
#define MC_BANK_ADDRESS_REG   2
#define MC_BANK_MISC_REG      3

#define MSR_LOG_VALID   BIT31
#define MSR_LOG_UC      BIT29
#define MSR_LOG_EN      BIT28

// Bit definitions for commonSetup.options
//                   ; PROMOTE_WARN_EN enables warnings to be treated as fatal error
//                   ; PROMOTE_MRC_WARN_EN enables MRC warnings to be treated as fatal error
//                   ; HALT_ON_ERROR_EN enables errors to loop forever
#define PROMOTE_WARN_EN           BIT0
#define PROMOTE_MRC_WARN_EN       BIT1
#define HALT_ON_ERROR_EN          BIT2


// -----------------------------------------------------------------------------
//

//
// Handle assertions with RC_ASSERT
//
#if defined(SIM_BUILD) || defined(IA32) || defined (HEADLESS_MRC)

#define RC_ASSERT(assertion, majorCode, minorCode) \
  if (!(assertion)) { \
    DebugPrintRc (host, 0xFF, "\n\nRC_ASSERT! %s: %u   %s ", __FILE__, __LINE__, #assertion);\
    FatalError (host, majorCode, minorCode);\
  }

#else

#define RC_ASSERT(assertion, majorCode, minorCode) \
  if (!(assertion)) { \
    CpuCsrAccessError (host, "\n\nRC_ASSERT! %s: %u   %s ", __FILE__, __LINE__, #assertion);\
  }

#endif


#endif // _syshost_h
