/********************************************************************************
Copyright (C) 2018 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#ifndef __SAMPLE_AT_RESET_LIB_H__
#define __SAMPLE_AT_RESET_LIB_H__

UINT32
EFIAPI
SampleAtResetGetCpuFrequency (
  VOID
  );

UINT32
EFIAPI
SampleAtResetGetDramFrequency (
  VOID
  );

#endif /* __SAMPLE_AT_RESET_LIB_H__ */
