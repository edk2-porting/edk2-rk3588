/** @file

  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _CPUSMM_H_
#define _CPUSMM_H_
#ifdef __cplusplus
extern "C" {
#endif

#define CPUSMM_GUID { 0x90d93e09, 0x4e91, 0x4b3d, { 0x8c, 0x77, 0xc8, 0x2f, 0xf1, 0xe, 0x3c, 0x81 }}
#define CPUSMM_SETUP_NAME             L"CpuSmm"

#pragma pack(1)
typedef struct {
  UINT8     CpuSmmMsrSaveStateEnable;
  UINT8     CpuSmmCodeAccessCheckEnable;
  UINT8     CpuSmmUseDelayIndication;
  UINT8     CpuSmmUseBlockIndication;
  UINT8     CpuSmmUseSmmEnableIndication;
  UINT8     CpuSmmProcTraceEnable;
} CPU_SMM;
#pragma pack()

#ifndef OFFSET_OF
#ifdef __GNUC__
#if __GNUC__ >= 4
#define OFFSET_OF(TYPE, Field) ((UINTN) __builtin_offsetof(TYPE, Field))
#endif
#endif
#endif

#ifndef OFFSET_OF
#define OFFSET_OF(TYPE, Field) ((UINTN) &(((TYPE *)0)->Field))
#endif

#define VERIFY_OFFSET(TYPE, Field, Offset) extern UINT8 _VerifyOffset##TYPE##Field[(OFFSET_OF(TYPE, Field) == Offset) / (OFFSET_OF(TYPE, Field) == Offset)]

//
// If TpmSupport/MorStae isn't in this offset, build failure (0 size array or divided by 0) will be generated.
// Platform DSC file maps the two field to HII PCD so the offset value is critical.
//
VERIFY_OFFSET (CPU_SMM, CpuSmmMsrSaveStateEnable, 0x0);
VERIFY_OFFSET (CPU_SMM, CpuSmmCodeAccessCheckEnable, 0x1);
VERIFY_OFFSET (CPU_SMM, CpuSmmUseDelayIndication, 0x2);
VERIFY_OFFSET (CPU_SMM, CpuSmmUseBlockIndication, 0x3);
VERIFY_OFFSET (CPU_SMM, CpuSmmUseSmmEnableIndication, 0x4);
VERIFY_OFFSET (CPU_SMM, CpuSmmProcTraceEnable, 0x5);

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif

