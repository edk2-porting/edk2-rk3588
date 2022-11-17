/** @file
  Tiger Lake U RVP SMM Silicon ACPI Enable library


  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/BoardAcpiEnableLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/MmPciLib.h>
#include <Library/PmcLib.h>
#include <Library/PchPciBdfLib.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsLpc.h>
#include <Register/PmcRegs.h>
#include <Register/RtcRegs.h>

/**
  Clear Port 80h

  Disables the SW SMI Timer.
  ACPI events are disabled and ACPI event status is cleared.

  Disable port 60/64 SMI trap if they are enabled

  Disable PM sources except power button
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

  UINT32                              SmiEn;
  UINT32                              SmiSts;
  UINT32                              ULKMC;
  UINTN                               LpcBaseAddress;
  UINT16                              AcpiBaseAddr;
  UINT32                              Pm1Cnt;

  LpcBaseAddress = LpcPciCfgBase ();

  //
  // Get the ACPI Base Address
  //
  AcpiBaseAddr = PmcGetAcpiBase();

  //
  // Clear Port 80h
  //
  IoWrite8 (0x80, 0);

  //
  // Disable SW SMI Timer and clean the status
  //
  SmiEn = IoRead32 (AcpiBaseAddr + R_ACPI_IO_SMI_EN);
  SmiEn &= ~(B_ACPI_IO_SMI_EN_LEGACY_USB2 | B_ACPI_IO_SMI_EN_SWSMI_TMR | B_ACPI_IO_SMI_EN_LEGACY_USB);
  IoWrite32 (AcpiBaseAddr + R_ACPI_IO_SMI_EN, SmiEn);

  SmiSts = IoRead32 (AcpiBaseAddr + R_ACPI_IO_SMI_STS);
  SmiSts |= B_ACPI_IO_SMI_EN_LEGACY_USB2 | B_ACPI_IO_SMI_EN_SWSMI_TMR | B_ACPI_IO_SMI_EN_LEGACY_USB;
  IoWrite32 (AcpiBaseAddr + R_ACPI_IO_SMI_STS, SmiSts);

  //
  // Disable port 60/64 SMI trap if they are enabled
  //
  ULKMC = MmioRead32 (LpcBaseAddress + R_LPC_CFG_ULKMC) & ~(B_LPC_CFG_ULKMC_60REN | B_LPC_CFG_ULKMC_60WEN | B_LPC_CFG_ULKMC_64REN | B_LPC_CFG_ULKMC_64WEN | B_LPC_CFG_ULKMC_A20PASSEN);
  MmioWrite32 (LpcBaseAddress + R_LPC_CFG_ULKMC, ULKMC);

  //
  // Disable PM sources except power button
  //
  IoWrite16 (AcpiBaseAddr + R_ACPI_IO_PM1_EN, B_ACPI_IO_PM1_EN_PWRBTN);

  //
  // Clear PM status except Power Button status for RapidStart Resume
  //
  IoWrite16 (AcpiBaseAddr + R_ACPI_IO_PM1_STS, 0xFEFF);

  //
  // Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)
  //
  IoWrite8 (R_RTC_IO_INDEX_ALT, R_RTC_IO_REGD);
  IoWrite8 (R_RTC_IO_TARGET_ALT, 0x0);

  //
  // Enable SCI
  //
  if (EnableSci) {
    Pm1Cnt = IoRead32 (AcpiBaseAddr + R_ACPI_IO_PM1_CNT);
    Pm1Cnt |= B_ACPI_IO_PM1_CNT_SCI_EN;
    IoWrite32 (AcpiBaseAddr + R_ACPI_IO_PM1_CNT, Pm1Cnt);
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
  AcpiBaseAddr = PmcGetAcpiBase();
  //
  // Disable SCI
  //
  if (DisableSci) {
    Pm1Cnt = IoRead32 (AcpiBaseAddr + R_ACPI_IO_PM1_CNT);
    Pm1Cnt &= ~B_ACPI_IO_PM1_CNT_SCI_EN;
    IoWrite32 (AcpiBaseAddr + R_ACPI_IO_PM1_CNT, Pm1Cnt);
  }

  return EFI_SUCCESS;
}
