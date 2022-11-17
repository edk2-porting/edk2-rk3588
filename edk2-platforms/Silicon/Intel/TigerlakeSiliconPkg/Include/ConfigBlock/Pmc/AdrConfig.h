/** @file
  ADR policy

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _ADR_CONFIG_H_
#define _ADR_CONFIG_H_

#include <ConfigBlock.h>

#define ADR_CONFIG_REVISION 1
extern EFI_GUID gAdrConfigGuid;

#pragma pack (push,1)

typedef enum {
  AdrScale1us,
  AdrScale10us,
  AdrScale100us,
  AdrScale1ms,
  AdrScale10ms,
  AdrScale100ms,
  AdrScale1s,
  AdrScale10s
} ADR_TIMER_SCALE;

/**
  ADR Source Enable
**/
typedef union {
  struct {
    UINT32 Reserved1           : 1;
    UINT32 AdrSrcPbo           : 1;
    UINT32 AdrSrcPmcUncErr     : 1;
    UINT32 AdrSrcPchThrm       : 1;
    UINT32 AdrSrcMePbo         : 1;
    UINT32 AdrSrcCpuThrm       : 1;
    UINT32 AdrSrcMegbl         : 1;
    UINT32 AdrSrcLtReset       : 1;
    UINT32 AdrSrcPmcWdt        : 1;
    UINT32 AdrSrcMeWdt         : 1;
    UINT32 AdrSrcPmcFw         : 1;
    UINT32 AdrSrcPchpwrFlr     : 1;
    UINT32 AdrSrcSyspwrFlr     : 1;
    UINT32 Reserved2           : 1;
    UINT32 AdrSrcMiaUxsErr     : 1;
    UINT32 AdrSrcMiaUxErr      : 1;
    UINT32 AdrSrcCpuThrmWdt    : 1;
    UINT32 AdrSrcMeUncErr      : 1;
    UINT32 AdrSrcAdrGpio       : 1;
    UINT32 AdrSrcOcwdtNoicc    : 1;
    UINT32 AdrSrcOcwdtIcc      : 1;
    UINT32 AdrSrcCseHecUncErr  : 1;
    UINT32 AdrSrcPmcSramUncErr : 1;
    UINT32 AdrSrcPmcIromParity : 1;
    UINT32 AdrSrcPmcRfFusaErr  : 1;
    UINT32 Reserved3           : 4;
    UINT32 AdrSrcPpbrParityErr : 1;
    UINT32 Reserved4           : 2;
  } Field;
  UINT32 Value;
} ADR_SOURCE_ENABLE;

/**
  ADR Configuration
  <b>Revision 1</b>:  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  UINT32        AdrEn                  : 2; ///< Determine if Adr is enabled - 0: PLATFORM_POR, 1: FORCE_ENABLE, 2: FORCE_DISABLE
  UINT32        AdrTimerEn             : 2; ///< Determine if Adr timer options are enabled - 0: PLATFORM_POR, 1: FORCE_ENABLE, 2: FORCE_DISABLE
  UINT32        AdrTimer1Val           : 2; ///< Determines the Timeout value used for the ADR timer 1. A value of zero bypasses the timer
  UINT32        AdrMultiplier1Val      : 8; ///< Specifies the tick frequency upon which the timer 1 will increment. ADR_TIMER_SCALE should be used to encode values
  UINT32        AdrTimer2Val           : 8; ///< Determines the Timeout value used for the ADR timer 2. A value of zero bypasses the timer
  UINT32        AdrMultiplier2Val      : 8; ///< Specifies the tick frequency upon which the timer 2 will increment. ADR_TIMER_SCALE should be used to encode values
  UINT32        AdrHostPartitionReset  : 2; ///< Determine if Host Partition Reset is enabled - 0: PLATFORM_POR, 1: FORCE_ENABLE, 2: FORCE_DISABLE
  UINT32        AdrSrcOverride         : 1; ///< Check if default ADR sources will be overriten with custom 0: Not overwritten, 1: Overwritten
  UINT32        ReservedBits           : 31;
  ADR_SOURCE_ENABLE   AdrSrcSel;            ///< Determine which ADR sources are enabled - 0: Enabled, 1: Disabled
} ADR_CONFIG;

#pragma pack (pop)

#endif // _ADR_CONFIG_H_
