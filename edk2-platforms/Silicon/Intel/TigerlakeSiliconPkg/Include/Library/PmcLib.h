/** @file
  Header file for PmcLib.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#ifndef _PMC_LIB_H_
#define _PMC_LIB_H_

#pragma pack(1)

typedef enum {
  PmcTPch25_10us = 0,
  PmcTPch25_100us,
  PmcTPch25_1ms,
  PmcTPch25_10ms,
} PMC_TPCH25_TIMING;

typedef enum {
  PmcNotASleepState,
  PmcInS0State,
  PmcS1SleepState,
  PmcS2SleepState,
  PmcS3SleepState,
  PmcS4SleepState,
  PmcS5SleepState,
  PmcUndefinedState,
} PMC_SLEEP_STATE;

typedef struct {
  UINT32    Buf0;
  UINT32    Buf1;
  UINT32    Buf2;
  UINT32    Buf3;
} PMC_IPC_COMMAND_BUFFER;

//
// Structure to Check different attributes for CrashLog supported by PMC.
//
typedef union {
  struct {
    UINT32  Avail      : 1;        ///< CrashLog feature availability bit
    UINT32  Dis        : 1;        ///< CrasLog Disable bit
    UINT32  Rsvd       : 2;        ///< Reserved
    UINT32  Size       : 12;       ///< CrasLog data size. (If it is zero, use default size 0xC00)
    UINT32  BaseOffset : 16;       ///< Start offset of CrashLog in PMC SSRAM
  } Bits;
  struct {
    UINT32  Avail      : 1;        ///< CrashLog feature availability bit
    UINT32  Dis        : 1;        ///< CrasLog Disable bit
    UINT32  Mech       : 2;        ///< CrashLog mechanism
    UINT32  ManuTri    : 1;        ///< Manul trigger command.
    UINT32  Clr        : 1;        ///< Clear Command
    UINT32  AllReset   : 1;        ///< Trigger on all reset command
    UINT32  ReArm      : 1;        ///< Re-arm command
    UINT32  Rsvd       : 20;       ///< Pch Specific reserved
    UINT32  CrashLogReq: 1;        ///< Crash log requestor flow
    UINT32  TriArmedSts: 1;        ///< Trigger armed status, re-arm indication bit.
    UINT32  TriAllReset: 1;        ///< Trigger on all resets status
    UINT32  CrashDisSts: 1;        ///< Crash log disabled status
    UINT32  PchRsvd    : 16;       ///< Pch Specific reserved
    UINT32  DesTableOffset: 16;    ///< Descriptor Table offset
  } Bits64;
  UINT32  Uint32;
  UINT64  Uint64;
} PMC_IPC_DISCOVERY_BUF;

typedef union {
  struct {
    UINT32 Offset   : 16;
    UINT32 Size     : 16;
  } Info;
  UINT32 Uint32;
} PMC_CRASHLOG_RECORDS;

typedef struct PmcCrashLogLink {
  PMC_CRASHLOG_RECORDS      Record;
  UINT64                    AllocateAddress;
  struct PmcCrashLogLink    *Next;
} PMC_CRASHLOG_LINK;

#pragma pack()

/**
  Get PCH ACPI base address.

  @retval Address                   Address of PWRM base address.
**/
UINT16
PmcGetAcpiBase (
  VOID
  );

/**
  Get PCH PWRM base address.

  @retval Address                   Address of PWRM base address.
**/
UINT32
PmcGetPwrmBase (
  VOID
  );

/**
  This function sets tPCH25 timing

  @param[in] TimingValue       tPCH25 timing value (10us, 100us, 1ms, 10ms)
**/
VOID
PmcSetTPch25Timing (
  IN PMC_TPCH25_TIMING    TimingValue
  );

/**
  This function checks if RTC Power Failure occurred by
  reading RTC_PWR_FLR bit

  @retval RTC Power Failure state: TRUE  - Battery is always present.
                                   FALSE - CMOS is cleared.
**/
BOOLEAN
PmcIsRtcBatteryGood (
  VOID
  );

/**
  This function checks if Power Failure occurred by
  reading PWR_FLR bit

  @retval Power Failure state
**/
BOOLEAN
PmcIsPowerFailureDetected (
  VOID
  );

/**
  This function checks if Power Failure occurred by
  reading SUS_PWR_FLR bit

  @retval SUS Power Failure state
**/
BOOLEAN
PmcIsSusPowerFailureDetected (
  VOID
  );

/**
  This function clears Power Failure status (PWR_FLR)
**/
VOID
PmcClearPowerFailureStatus (
  VOID
  );

/**
  This function clears Global Reset status (GBL_RST_STS)
**/
VOID
PmcClearGlobalResetStatus (
  VOID
  );

/**
  This function clears Host Reset status (HOST_RST_STS)
**/
VOID
PmcClearHostResetStatus (
  VOID
  );

/**
  This function clears SUS Power Failure status (SUS_PWR_FLR)
**/
VOID
PmcClearSusPowerFailureStatus (
  VOID
  );

/**
  This function sets state to which platform will get after power is reapplied

  @param[in] PowerStateAfterG3          0: S0 state (boot)
                                        1: S5/S4 State
**/
VOID
PmcSetPlatformStateAfterPowerFailure (
  IN UINT8 PowerStateAfterG3
  );

/**
  This function enables Power Button SMI
**/
VOID
PmcEnablePowerButtonSmi (
  VOID
  );

/**
  This function disables Power Button SMI
**/
VOID
PmcDisablePowerButtonSmi (
  VOID
  );

/**
  This function reads PM Timer Count driven by 3.579545 MHz clock

  @retval PM Timer Count
**/
UINT32
PmcGetTimerCount (
  VOID
  );

/**
  Get Sleep Type that platform has waken from

  @retval SleepType                Sleep Type
**/
PMC_SLEEP_STATE
PmcGetSleepTypeAfterWake (
  VOID
  );

/**
  Clear PMC Wake Status
**/
VOID
PmcClearWakeStatus (
  VOID
  );

/**
  Configure sleep state

  @param[in] SleepState         S0/S1/S3/S4/S5, refer to PMC_SLEEP_STATE
**/
VOID
PmcSetSleepState (
  PMC_SLEEP_STATE  SleepState
  );

/**
  Check if platform boots after shutdown caused by power button override event

  @retval  TRUE   Power Button Override occurred in last system boot
  @retval  FALSE  Power Button Override didn't occur
**/
BOOLEAN
PmcIsPowerButtonOverrideDetected (
  VOID
  );

/**
  This function will set the DISB - DRAM Initialization Scratchpad Bit.
**/
VOID
PmcSetDramInitScratchpad (
  VOID
  );

/**
  Check global SMI enable is set

  @retval TRUE  Global SMI enable is set
          FALSE Global SMI enable is not set
**/
BOOLEAN
PmcIsGblSmiEn (
  VOID
  );

/**
  This function checks if SMI Lock is set

  @retval SMI Lock state
**/
BOOLEAN
PmcIsSmiLockSet (
  VOID
  );

/**
  This function checks if Debug Mode is locked

  @retval Debug Mode Lock state
**/
BOOLEAN
PmcIsDebugModeLocked (
  VOID
  );

/**
  Check TCO second timeout status.

  @retval  TRUE   TCO reboot happened.
  @retval  FALSE  TCO reboot didn't happen.
**/
BOOLEAN
TcoSecondToHappened (
  VOID
  );

/**
  This function clears the Second TO status bit
**/
VOID
TcoClearSecondToStatus (
  VOID
  );

/**
  Check TCO SMI ENABLE is locked

  @retval TRUE  TCO SMI ENABLE is locked
          FALSE TCO SMI ENABLE is not locked
**/
BOOLEAN
TcoIsSmiLock (
  VOID
  );

/**
  Check if user wants to turn off in PEI phase

**/
VOID
CheckPowerOffNow(
  VOID
  );


/**
  Clear any SMI status or wake status left from boot.
**/
VOID
ClearSmiAndWake (
  VOID
  );

/**
  Function to check if Dirty Warm Reset occurs
  (Global Reset has been converted to Host Reset)

  @reval TRUE DWR occurs
  @reval FALSE Normal boot flow
**/
BOOLEAN
PmcIsDwrBootMode (
  VOID
  );

#endif // _PMC_LIB_H_
