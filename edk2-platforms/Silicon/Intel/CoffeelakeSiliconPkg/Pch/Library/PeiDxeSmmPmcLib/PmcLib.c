/** @file
  PCH PMC Library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PmcLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Private/Library/PmcPrivateLib.h>
#include <PchReservedResources.h>
#include <Register/PchRegsPmc.h>

/**
  Get PCH ACPI base address.

  @retval Address                   Address of PWRM base address.
**/
UINT16
PmcGetAcpiBase (
  VOID
  )
{
  return PcdGet16 (PcdAcpiBaseAddress);
}

/**
  Get PCH PWRM base address.

  @retval Address                   Address of PWRM base address.
**/
UINT32
PmcGetPwrmBase (
  VOID
  )
{
  return PCH_PWRM_BASE_ADDRESS;
}

/**
  This function enables Power Button SMI
**/
VOID
PmcEnablePowerButtonSmi (
  VOID
  )
{
  IoOr16 (PmcGetAcpiBase () + R_ACPI_IO_PM1_EN, B_ACPI_IO_PM1_EN_PWRBTN);
}

/**
  This function disables Power Button SMI
**/
VOID
PmcDisablePowerButtonSmi (
  VOID
  )
{
  IoAnd16 (PmcGetAcpiBase () + R_ACPI_IO_PM1_EN, (UINT16)~B_ACPI_IO_PM1_EN_PWRBTN);
}

/**
  This function reads PM Timer Count driven by 3.579545 MHz clock

  @retval PM Timer Count
**/
UINT32
PmcGetTimerCount (
  VOID
  )
{
  return IoRead32 (PmcGetAcpiBase () + R_ACPI_IO_PM1_TMR) & B_ACPI_IO_PM1_TMR_VAL;
}

/**
  Get Sleep Type that platform has waken from

  @retval SleepType                Sleep Type
**/
PMC_SLEEP_STATE
PmcGetSleepTypeAfterWake (
  VOID
  )
{
  UINT16  AcpiBase;
  UINT32  PmconA;

  AcpiBase = PmcGetAcpiBase ();
  PmconA   = MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A);

  DEBUG ((DEBUG_INFO, "PWRM_PMCON_A = 0x%x\n", PmconA));

  //
  // If Global Reset Status, Power Failure. Host Reset Status bits are set, return S5 State
  //
  if ((PmconA & (B_PMC_PWRM_GEN_PMCON_A_GBL_RST_STS | B_PMC_PWRM_GEN_PMCON_A_PWR_FLR | B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS)) != 0) {
    return PmcNotASleepState;
  }

  if (IoRead16 (AcpiBase + R_ACPI_IO_PM1_STS) & B_ACPI_IO_PM1_STS_WAK) {
    switch (IoRead16 (AcpiBase + R_ACPI_IO_PM1_CNT) & B_ACPI_IO_PM1_CNT_SLP_TYP) {
      case V_ACPI_IO_PM1_CNT_S0:
        return PmcInS0State;

      case V_ACPI_IO_PM1_CNT_S1:
        return PmcS1SleepState;

      case V_ACPI_IO_PM1_CNT_S3:
        return PmcS3SleepState;

      case V_ACPI_IO_PM1_CNT_S4:
        return PmcS4SleepState;

      case V_ACPI_IO_PM1_CNT_S5:
        return PmcS5SleepState;

      default:
        ASSERT (FALSE);
        return PmcUndefinedState;
    }
  } else {
    return PmcNotASleepState;
  }
}

/**
  Clear PMC Wake Status
**/
VOID
PmcClearWakeStatus (
  VOID
  )
{
  IoWrite16 (PmcGetAcpiBase () + R_ACPI_IO_PM1_STS, B_ACPI_IO_PM1_STS_WAK);
}

/**
  Check if platform boots after shutdown caused by power button override event

  @retval  TRUE   Power Button Override occurred in last system boot
  @retval  FALSE  Power Button Override didn't occur
**/
BOOLEAN
PmcIsPowerButtonOverrideDetected (
  VOID
  )
{
  return ((IoRead16 (PmcGetAcpiBase () + R_ACPI_IO_PM1_STS) & B_ACPI_IO_PM1_STS_PRBTNOR) != 0);
}

/**
  This function checks if RTC Power Failure occurred by
  reading RTC_PWR_FLR bit

  @retval RTC Power Failure state: TRUE  - Battery is always present.
                                   FALSE - CMOS is cleared.
**/
BOOLEAN
PmcIsRtcBatteryGood (
  VOID
  )
{
  return ((MmioRead8 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_B) & B_PMC_PWRM_GEN_PMCON_B_RTC_PWR_STS) == 0);
}

/**
  This function checks if Power Failure occurred by
  reading PWR_FLR bit

  @retval Power Failure state
**/
BOOLEAN
PmcIsPowerFailureDetected (
  VOID
  )
{
  return ((MmioRead16 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A) & B_PMC_PWRM_GEN_PMCON_A_PWR_FLR) != 0);
}

/**
  This function checks if RTC Power Failure occurred by
  reading SUS_PWR_FLR bit

  @retval SUS Power Failure state
**/
BOOLEAN
PmcIsSusPowerFailureDetected (
  VOID
  )
{
  return ((MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_B) & B_PMC_PWRM_GEN_PMCON_A_SUS_PWR_FLR) != 0);
}

/**
  This function clears Power Failure status (PWR_FLR)
**/
VOID
PmcClearPowerFailureStatus (
  VOID
  )
{
  //
  // Write 1 to clear PWR_FLR
  // Avoid clearing other W1C bits
  //
  MmioAndThenOr8 (
    PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 1,
    (UINT8) ~(B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS >> 8),
    B_PMC_PWRM_GEN_PMCON_A_PWR_FLR >> 8
    );
}

/**
  This function clears Global Reset status (GBL_RST_STS)
**/
VOID
PmcClearGlobalResetStatus (
  VOID
  )
{
  //
  // Write 1 to clear GBL_RST_STS
  // Avoid clearing other W1C bits
  //
  MmioAndThenOr8 (
    PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 3,
    (UINT8) ~0,
    B_PMC_PWRM_GEN_PMCON_A_GBL_RST_STS >> 24
    );
}

/**
  This function clears Host Reset status (HOST_RST_STS)
**/
VOID
PmcClearHostResetStatus (
  VOID
  )
{
  //
  // Write 1 to clear HOST_RST_STS
  // Avoid clearing other W1C bits
  //
  MmioAndThenOr8 (
    PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 1,
    (UINT8) ~(B_PMC_PWRM_GEN_PMCON_A_PWR_FLR >> 8),
    B_PMC_PWRM_GEN_PMCON_A_HOST_RST_STS >> 8
    );
}

/**
  This function clears SUS Power Failure status (SUS_PWR_FLR)
**/
VOID
PmcClearSusPowerFailureStatus (
  VOID
  )
{
  //
  // BIOS clears this bit by writing a '1' to it.
  // Take care of other fields, so we don't clear them accidentally.
  //
  MmioAndThenOr8 (
    PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A + 2,
    (UINT8) ~(B_PMC_PWRM_GEN_PMCON_A_MS4V >> 16),
    B_PMC_PWRM_GEN_PMCON_A_SUS_PWR_FLR >> 16
    );
}

/**
  This function sets state to which platform will get after power is reapplied

  @param[in] PowerStateAfterG3          0: S0 state (boot)
                                        1: S5/S4 State
**/
VOID
PmcSetPlatformStateAfterPowerFailure (
  IN UINT8 PowerStateAfterG3
  )
{
  UINT32                PchPwrmBase;

  PchPwrmBase = PmcGetPwrmBase ();

  if (PowerStateAfterG3) {
    MmioOr8 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A, B_PMC_PWRM_GEN_PMCON_A_AFTERG3_EN);
  } else {
    MmioAnd8 (PchPwrmBase + R_PMC_PWRM_GEN_PMCON_A, (UINT8)~B_PMC_PWRM_GEN_PMCON_A_AFTERG3_EN);
  }
}

/**
  This function checks if SMI Lock is set

  @retval SMI Lock state
**/
BOOLEAN
PmcIsSmiLockSet (
  VOID
  )
{
  return ((MmioRead8 ((UINTN) (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_B)) & B_PMC_PWRM_GEN_PMCON_B_SMI_LOCK) != 0);
}

/**
  Check global SMI enable is set

  @retval TRUE  Global SMI enable is set
          FALSE Global SMI enable is not set
**/
BOOLEAN
PmcIsGblSmiEn (
  VOID
  )
{
  return !!(IoRead32 (PmcGetAcpiBase () + R_ACPI_IO_SMI_EN) & B_ACPI_IO_SMI_EN_GBL_SMI);
}

