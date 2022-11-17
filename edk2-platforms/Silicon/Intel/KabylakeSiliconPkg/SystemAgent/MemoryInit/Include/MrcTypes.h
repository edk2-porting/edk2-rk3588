/** @file

  Include the the general MRC types

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef _MRC_TYPES_H
#define _MRC_TYPES_H

#ifdef MRC_MINIBIOS_BUILD
#include "MrcMiniBiosEfiDefs.h"
#else
#include <Base.h>
#endif // MRC_MINIBIOS_BUILD

//
// Data Types
//
typedef union {
  struct {
    UINT32  Low;
    UINT32  High;
  } Data32;
  UINT64 Data;
} UINT64_STRUCT;

typedef union {
  struct {
    INT32  Low;
    INT32  High;
  } Data32;
  INT64 Data;
} INT64_STRUCT;

#define UNSUPPORT 0
#define SUPPORT   1

typedef enum {
  mrcSuccess,
  mrcFail,
  mrcWrongInputParameter,
  mrcCasError,
  mrcTimingError,
  mrcSenseAmpErr,
  mrcReadMPRErr,
  mrcReadLevelingError,
  mrcWriteLevelingError,
  mrcDataTimeCentering1DErr,
  mrcWriteVoltage2DError,
  mrcReadVoltage2DError,
  mrcMiscTrainingError,
  mrcWrError,
  mrcDimmNotSupport,
  mrcChannelNotSupport,
  mrcPiSettingError,
  mrcDqsPiSettingError,
  mrcDeviceBusy,
  mrcFrequencyChange,
  mrcReutSequenceError,
  mrcCrcError,
  mrcFrequencyError,
  mrcDimmNotExist,
  mrcColdBootRequired,
  mrcRoundTripLatencyError,
  mrcMixedDimmSystem,
  mrcAliasDetected,
  mrcRetrain,
  mrcRtpError,
  mrcUnsupportedTechnology,
  mrcMappingError,
  mrcSocketNotSupported,
  mrcControllerNotSupported,
  mrcRankNotSupported,
  mrcTurnAroundTripError
} MrcStatus;

//
// general  macros
//
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(x)  (((x) < 0) ? (-(x)) : (x))
#endif

//
// Make sure x is inside the range of [a..b]
//
#ifndef RANGE
#define RANGE(x, a, b) (MIN ((b), MAX ((x), (a))))
#endif

#ifndef DIVIDECEIL
#define DIVIDECEIL(a, b)   (((a) + (b) - 1) / (b))
#endif

#ifndef DIVIDEROUND
#define DIVIDEROUND(a, b)  (((a) * (b) > 0) ? ((a) + (b) / 2) / (b) : ((a) - (b) / 2) / (b))
#endif

#ifndef DIVIDEFLOOR
#define DIVIDEFLOOR(a, b)  ((a) / (b))
#endif

//
// Number of elements in a 1D array
//
#ifndef ARRAY_COUNT
#define ARRAY_COUNT(a) (sizeof (a) / sizeof (a[0]))
#endif

//
//  use for ignore parames
//
// #define MRC_IGNORE_PARAM(x) ((x) = (x))
//
#if _MSC_EXTENSIONS
//
// Disable warning that make it impossible to compile at /W4
// This only works for Microsoft* tools
//
//
// Disabling bitfield type checking warnings.
//
#pragma warning (disable : 4214)
//
// Unreferenced formal parameter - We are object oriented, so we pass parameters even
//  if we don't need them.
//
#pragma warning (disable : 4100)
//
// ASSERT(FALSE) or while (TRUE) are legal constructs so supress this warning
//
#pragma warning(disable : 4127)
//
// The given function was selected for inline expansion, but the compiler did not perform the inlining.
//
#pragma warning(disable : 4710)

#endif // _MSC_EXTENSIONS
#define MRC_BIT0          0x00000001
#define MRC_BIT1          0x00000002
#define MRC_BIT2          0x00000004
#define MRC_BIT3          0x00000008
#define MRC_BIT4          0x00000010
#define MRC_BIT5          0x00000020
#define MRC_BIT6          0x00000040
#define MRC_BIT7          0x00000080
#define MRC_BIT8          0x00000100
#define MRC_BIT9          0x00000200
#define MRC_BIT10         0x00000400
#define MRC_BIT11         0x00000800
#define MRC_BIT12         0x00001000
#define MRC_BIT13         0x00002000
#define MRC_BIT14         0x00004000
#define MRC_BIT15         0x00008000
#define MRC_BIT16         0x00010000
#define MRC_BIT17         0x00020000
#define MRC_BIT18         0x00040000
#define MRC_BIT19         0x00080000
#define MRC_BIT20         0x00100000
#define MRC_BIT21         0x00200000
#define MRC_BIT22         0x00400000
#define MRC_BIT23         0x00800000
#define MRC_BIT24         0x01000000
#define MRC_BIT25         0x02000000
#define MRC_BIT26         0x04000000
#define MRC_BIT27         0x08000000
#define MRC_BIT28         0x10000000
#define MRC_BIT29         0x20000000
#define MRC_BIT30         0x40000000
#define MRC_BIT31         0x80000000
#define MRC_BIT32        0x100000000ULL
#define MRC_BIT33        0x200000000ULL
#define MRC_BIT34        0x400000000ULL
#define MRC_BIT35        0x800000000ULL
#define MRC_BIT36       0x1000000000ULL
#define MRC_BIT37       0x2000000000ULL
#define MRC_BIT38       0x4000000000ULL
#define MRC_BIT39       0x8000000000ULL
#define MRC_BIT40      0x10000000000ULL
#define MRC_BIT41      0x20000000000ULL
#define MRC_BIT42      0x40000000000ULL
#define MRC_BIT43      0x80000000000ULL
#define MRC_BIT44     0x100000000000ULL
#define MRC_BIT45     0x200000000000ULL
#define MRC_BIT46     0x400000000000ULL
#define MRC_BIT47     0x800000000000ULL
#define MRC_BIT48    0x1000000000000ULL
#define MRC_BIT49    0x2000000000000ULL
#define MRC_BIT50    0x4000000000000ULL
#define MRC_BIT51    0x8000000000000ULL
#define MRC_BIT52   0x10000000000000ULL
#define MRC_BIT53   0x20000000000000ULL
#define MRC_BIT54   0x40000000000000ULL
#define MRC_BIT55   0x80000000000000ULL
#define MRC_BIT56  0x100000000000000ULL
#define MRC_BIT57  0x200000000000000ULL
#define MRC_BIT58  0x400000000000000ULL
#define MRC_BIT59  0x800000000000000ULL
#define MRC_BIT60 0x1000000000000000ULL
#define MRC_BIT61 0x2000000000000000ULL
#define MRC_BIT62 0x4000000000000000ULL
#define MRC_BIT63 0x8000000000000000ULL

#define MRC_DEADLOOP() { volatile int __iii; __iii = 1; while (__iii); }

#ifndef ASM
#define ASM __asm
#endif

///
/// Type Max/Min Values
///
#define MRC_INT32_MAX   (0x7FFFFFFF)
#define MRC_INT32_MIN   (0x80000000)
#define MRC_INT64_MAX   (0x7FFFFFFFFFFFFFFFLL)
#define MRC_INT64_MIN   (0x8000000000000000LL)
#define MRC_UINT32_MAX  (0xFFFFFFFF)
#define MRC_UINT64_MAX  (0xFFFFFFFFFFFFFFFFULL)
#define MRC_UINT_MIN    (0x0)

#endif
