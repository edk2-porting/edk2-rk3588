/** @file
  File to contain all the hardware specific stuff for the Smm Sx dispatch protocol.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "PchSmmHelpers.h"

#define PROGRESS_CODE_S3_SUSPEND_END  PcdGet32 (PcdProgressCodeS3SuspendEnd)

GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_SMM_SOURCE_DESC SX_SOURCE_DESC = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_EN}
      },
      S_PCH_SMI_EN,
      N_PCH_SMI_EN_ON_SLP_EN
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        ACPI_ADDR_TYPE,
        {R_PCH_SMI_STS}
      },
      S_PCH_SMI_STS,
      N_PCH_SMI_STS_ON_SLP_EN
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_PCH_SMI_STS}
    },
    S_PCH_SMI_STS,
    N_PCH_SMI_STS_ON_SLP_EN
  }
};

/**
  Get the Sleep type

  @param[in] Record               No use
  @param[out] Context             The context that includes SLP_TYP bits to be filled

**/
VOID
EFIAPI
SxGetContext (
  IN  DATABASE_RECORD    *Record,
  OUT PCH_SMM_CONTEXT    *Context
  )
{
  UINT32  Pm1Cnt;

  Pm1Cnt = IoRead32 ((UINTN) (mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT));

  ///
  /// By design, the context phase will always be ENTRY
  ///
  Context->Sx.Phase = SxEntry;

  ///
  /// Map the PM1_CNT register's SLP_TYP bits to the context type
  ///
  switch (Pm1Cnt & B_PCH_ACPI_PM1_CNT_SLP_TYP) {
    case V_PCH_ACPI_PM1_CNT_S0:
      Context->Sx.Type = SxS0;
      break;

    case V_PCH_ACPI_PM1_CNT_S1:
      Context->Sx.Type = SxS1;
      break;

    case V_PCH_ACPI_PM1_CNT_S3:
      Context->Sx.Type = SxS3;
      break;

    case V_PCH_ACPI_PM1_CNT_S4:
      Context->Sx.Type = SxS4;
      break;

    case V_PCH_ACPI_PM1_CNT_S5:
      Context->Sx.Type = SxS5;
      break;

    default:
      ASSERT (FALSE);
      break;
  }
}

/**
  Check whether sleep type of two contexts match

  @param[in] Context1             Context 1 that includes sleep type 1
  @param[in] Context2             Context 2 that includes sleep type 2

  @retval FALSE                   Sleep types match
  @retval TRUE                    Sleep types don't match
**/
BOOLEAN
EFIAPI
SxCmpContext (
  IN PCH_SMM_CONTEXT     *Context1,
  IN PCH_SMM_CONTEXT     *Context2
  )
{
  return (BOOLEAN) (Context1->Sx.Type == Context2->Sx.Type);
}

/**
  For each PCIE RP clear PME SCI status and disable SCI, then PCIEXP_WAKE_STS from PMC.
  This prevents platform from waking more than one time due to a single PCIE wake event.
  Normally it's up to OS to clear SCI statuses. But in a scenario where platform wakes
  and goes to S5 instead of booting to OS, the SCI status would remain set and would trigger another wake.
**/
VOID
ClearPcieSci (
  VOID
  )
{
  UINT32 MaxPorts;
  UINT32 RpIndex;
  UINTN  RpBase;

  MaxPorts = GetPchMaxPciePortNum ();
  for (RpIndex = 0; RpIndex < MaxPorts; RpIndex++) {
    RpBase = PchPcieBase (RpIndex);
    if (MmioRead16 (RpBase + PCI_VENDOR_ID_OFFSET) != 0xFFFF) {
      MmioAnd8 ((RpBase + R_PCH_PCIE_MPC + 3), (UINT8)~((UINT8)(B_PCH_PCIE_MPC_PMCE >> 24)));
      MmioWrite32 (RpBase + R_PCH_PCIE_SMSCS, B_PCH_PCIE_SMSCS_PMCS);
    }
  }
  IoWrite16 (mAcpiBaseAddr + R_PCH_ACPI_PM1_STS, B_PCH_ACPI_PM1_STS_PCIEXP_WAKE_STS);
}


/**
  When we get an SMI that indicates that we are transitioning to a sleep state,
  we need to actually transition to that state.  We do this by disabling the
  "SMI on sleep enable" feature, which generates an SMI when the operating system
  tries to put the system to sleep, and then physically putting the system to sleep.


**/
VOID
PchSmmSxGoToSleep (
  VOID
  )
{
  UINT32      Pm1Cnt;
  UINT32      PchPwrmBase;

  ClearPcieSci ();

  PchPwrmBaseGet (&PchPwrmBase);

  ///
  /// Flush cache into memory before we go to sleep. It is necessary for S3 sleep
  /// because we may update memory in SMM Sx sleep handlers -- the updates are in cache now
  ///
  AsmWbinvd ();

  ///
  /// Disable SMIs
  ///
  PchSmmClearSource (&SX_SOURCE_DESC);
  PchSmmDisableSource (&SX_SOURCE_DESC);

  ///
  /// Get Power Management 1 Control Register Value
  ///
  Pm1Cnt = IoRead32 ((UINTN) (mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT));

  ///
  /// Record S3 suspend performance data
  ///
  if ((Pm1Cnt & B_PCH_ACPI_PM1_CNT_SLP_TYP) == V_PCH_ACPI_PM1_CNT_S3) {
    ///
    /// Report status code before goto S3 sleep
    ///
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PROGRESS_CODE_S3_SUSPEND_END);

    ///
    /// Flush cache into memory before we go to sleep.
    ///
    AsmWbinvd ();
  }

  ///
  /// Now that SMIs are disabled, write to the SLP_EN bit again to trigger the sleep
  ///
  Pm1Cnt |= B_PCH_ACPI_PM1_CNT_SLP_EN;

  IoWrite32 ((UINTN) (mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT), Pm1Cnt);

  ///
  /// Should only proceed if wake event is generated.
  ///
  if ((Pm1Cnt & B_PCH_ACPI_PM1_CNT_SLP_TYP) == V_PCH_ACPI_PM1_CNT_S1) {
    while (((IoRead16 ((UINTN) (mAcpiBaseAddr + R_PCH_ACPI_PM1_STS))) & B_PCH_ACPI_PM1_STS_WAK) == 0x0);
  } else {
    CpuDeadLoop ();
  }
  ///
  /// The system just went to sleep. If the sleep state was S1, then code execution will resume
  /// here when the system wakes up.
  ///
  Pm1Cnt = IoRead32 ((UINTN) (mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT));

  if ((Pm1Cnt & B_PCH_ACPI_PM1_CNT_SCI_EN) == 0) {
    ///
    /// An ACPI OS isn't present, clear the sleep information
    ///
    Pm1Cnt &= ~B_PCH_ACPI_PM1_CNT_SLP_TYP;
    Pm1Cnt |= V_PCH_ACPI_PM1_CNT_S0;

    IoWrite32 ((UINTN) (mAcpiBaseAddr + R_PCH_ACPI_PM1_CNT), Pm1Cnt);
  }

  PchSmmClearSource (&SX_SOURCE_DESC);
  PchSmmEnableSource (&SX_SOURCE_DESC);
}
