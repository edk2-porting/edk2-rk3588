/** @file
    SMM ACPI enable library.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
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
#include <Library/MmPciLib.h>
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
  UINT32                              OutputValue;
  UINT32                              SmiEn;
  UINT32                              SmiSts;
  UINT32                              ULKMC;
  UINTN                               LpcBaseAddress;
  UINT16                              AcpiBaseAddr;
  UINT32                              Pm1Cnt;

  LpcBaseAddress = MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_LPC,
                     PCI_FUNCTION_NUMBER_PCH_LPC
                     );

  //
  // Get the ACPI Base Address
  //
  PchAcpiBaseGet (&AcpiBaseAddr);

  //
  // BIOS must also ensure that CF9GR is cleared and locked before handing control to the
  // OS in order to prevent the host from issuing global resets and resetting ME
  //
  // EDK2: To match PCCG current BIOS behavior, do not lock CF9 Global Reset
  // MmioWrite32 (
  //     PmcBaseAddress + R_PCH_PMC_ETR3),
  //     PmInit);

  //
  // Clear Port 80h
  //
  IoWrite8 (0x80, 0);

  //
  // Disable SW SMI Timer and clean the status
  //
  SmiEn = IoRead32 (AcpiBaseAddr + R_PCH_SMI_EN);
  SmiEn &= ~(B_PCH_SMI_EN_LEGACY_USB2 | B_PCH_SMI_EN_SWSMI_TMR | B_PCH_SMI_EN_LEGACY_USB);
  IoWrite32 (AcpiBaseAddr + R_PCH_SMI_EN, SmiEn);

  SmiSts = IoRead32 (AcpiBaseAddr + R_PCH_SMI_STS);
  SmiSts |= B_PCH_SMI_EN_LEGACY_USB2 | B_PCH_SMI_EN_SWSMI_TMR | B_PCH_SMI_EN_LEGACY_USB;
  IoWrite32 (AcpiBaseAddr + R_PCH_SMI_STS, SmiSts);

  //
  // Disable port 60/64 SMI trap if they are enabled
  //
  ULKMC = MmioRead32 (LpcBaseAddress + R_PCH_LPC_ULKMC) & ~(B_PCH_LPC_ULKMC_60REN | B_PCH_LPC_ULKMC_60WEN | B_PCH_LPC_ULKMC_64REN | B_PCH_LPC_ULKMC_64WEN | B_PCH_LPC_ULKMC_A20PASSEN);
  MmioWrite32 (LpcBaseAddress + R_PCH_LPC_ULKMC, ULKMC);

  //
  // Disable PM sources except power button
  //
  IoWrite16 (AcpiBaseAddr + R_PCH_ACPI_PM1_EN, B_PCH_ACPI_PM1_EN_PWRBTN);

  //
  // Clear PM status except Power Button status for RapidStart Resume
  //
  IoWrite16 (AcpiBaseAddr + R_PCH_ACPI_PM1_STS, 0xFEFF);

  //
  // Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)
  //
  IoWrite8 (R_PCH_RTC_INDEX_ALT, R_PCH_RTC_REGD);
  IoWrite8 (R_PCH_RTC_TARGET_ALT, 0x0);

  //
  // Write ALT_GPI_SMI_EN to disable GPI1 (SMC_EXTSMI#)
  //
  OutputValue = IoRead32 (AcpiBaseAddr + 0x38);
  OutputValue = OutputValue & ~(1 << (UINTN) PcdGet8 (PcdSmcExtSmiBitPosition));
  IoWrite32 (AcpiBaseAddr + 0x38, OutputValue);


  //
  // Enable SCI
  //
  if (EnableSci) {
    Pm1Cnt = IoRead32 (AcpiBaseAddr + R_PCH_ACPI_PM1_CNT);
    Pm1Cnt |= B_PCH_ACPI_PM1_CNT_SCI_EN;
    IoWrite32 (AcpiBaseAddr + R_PCH_ACPI_PM1_CNT, Pm1Cnt);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SiliconDisableAcpi (
  IN BOOLEAN  DisableSci
  )
{
  UINT16                              AcpiBaseAddr;
  UINT32                              Pm1Cnt;

  //
  // Get the ACPI Base Address
  //
  PchAcpiBaseGet (&AcpiBaseAddr);

  //
  // Disable SCI
  //
  if (DisableSci) {
    Pm1Cnt = IoRead32 (AcpiBaseAddr + R_PCH_ACPI_PM1_CNT);
    Pm1Cnt &= ~B_PCH_ACPI_PM1_CNT_SCI_EN;
    IoWrite32 (AcpiBaseAddr + R_PCH_ACPI_PM1_CNT, Pm1Cnt);
  }

  return EFI_SUCCESS;
}
