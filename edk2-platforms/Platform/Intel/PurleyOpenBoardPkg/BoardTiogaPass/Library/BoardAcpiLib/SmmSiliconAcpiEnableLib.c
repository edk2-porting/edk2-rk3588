/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
Copyright (c) 2021, American Megatrends International LLC.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BoardAcpiEnableLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <PchAccess.h>
#include <Library/PchCycleDecodingLib.h>

/**
  Clear Port 80h

  SMI handler to enable ACPI mode

  Dispatched on reads from APM port with value EFI_ACPI_ENABLE_SW_SMI

  Disables the SW SMI Timer.
  ACPI events are disabled and ACPI event status is cleared.
  SCI mode is then enabled.

  Clear SLP SMI status
  Enable SLP SMI

  Disable SW SMI Timer

  Clear all ACPI event status and disable all ACPI events

  Disable PM sources except power button
  Clear status bits

  Disable GPE0 sources
  Clear status bits

  Disable GPE1 sources
  Clear status bits

  Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)

  Enable SCI
**/
EFI_STATUS
EFIAPI
SiliconEnableAcpi (
  IN BOOLEAN  EnableSci
  )
{
  UINT32      SmiEn;
  UINT16      Pm1En;
  UINT16      Pm1Cnt;
  UINT16      PchPmBase;

  //
  // Init Power Management I/O Base aka ACPI Base
  //
  PchAcpiBaseGet (&PchPmBase);

  SmiEn = IoRead32 (PchPmBase + R_PCH_SMI_EN);

  //
  // Disable SW SMI Timer and legacy USB
  //
  SmiEn &= ~(B_PCH_SMI_EN_SWSMI_TMR | B_PCH_SMI_EN_LEGACY_USB | B_PCH_SMI_EN_LEGACY_USB2);

  //
  // And enable SMI on write to B_PCH_ACPI_PM1_CNT_SLP_EN when SLP_TYP is written
  //
  SmiEn |= B_PCH_SMI_EN_ON_SLP_EN;
  IoWrite32 (PchPmBase + R_PCH_SMI_EN, SmiEn);

  //
  // Disable PM sources except power button
  //
  Pm1En   = B_PCH_ACPI_PM1_EN_PWRBTN;
  IoWrite16 (PchPmBase + R_PCH_ACPI_PM1_EN, Pm1En);

  //
  // Enable SCI
  //
  Pm1Cnt = IoRead16 (PchPmBase + R_PCH_ACPI_PM1_CNT);
  Pm1Cnt |= B_PCH_ACPI_PM1_CNT_SCI_EN;
  IoWrite16 (PchPmBase + R_PCH_ACPI_PM1_CNT, Pm1Cnt);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SiliconDisableAcpi (
  IN BOOLEAN  DisableSci
  )
{
  UINT16      Pm1Cnt;
  UINT16      PchPmBase;

  //
  // Init Power Management I/O Base aka ACPI Base
  //
  PchAcpiBaseGet (&PchPmBase);

  Pm1Cnt = IoRead16 (PchPmBase + R_PCH_ACPI_PM1_CNT);

  //
  // Disable SCI
  //
  Pm1Cnt &= ~B_PCH_ACPI_PM1_CNT_SCI_EN;

  IoWrite16 (PchPmBase + R_PCH_ACPI_PM1_CNT, Pm1Cnt);

  return EFI_SUCCESS;
}
