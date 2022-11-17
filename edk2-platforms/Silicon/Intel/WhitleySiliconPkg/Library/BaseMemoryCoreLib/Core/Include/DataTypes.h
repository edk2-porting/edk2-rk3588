/** @file
  DataTypes.h

  @copyright
  Copyright 2006 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _datatypes_h
#define _datatypes_h

#include <Base.h>

typedef struct u64_struct {
  UINT32  lo;
  UINT32  hi;
} UINT64_STRUCT, *PUINT64_STRUCT;

typedef union {
  struct {
    UINT32  Low;
    UINT32  High;
  } Data32;
  UINT64 Data;
} UINT64_DATA;


typedef struct u128_struct {
  UINT32  one;
  UINT32  two;
  UINT32  three;
  UINT32  four;
} UINT128;

#endif // _datatypes_h
