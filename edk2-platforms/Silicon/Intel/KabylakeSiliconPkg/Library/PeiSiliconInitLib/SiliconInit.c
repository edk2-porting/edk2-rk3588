/** @file
  Source code file for Platform Init PEI module

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <CpuRegs.h>
#include <PchAccess.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MmPciLib.h>
#include <IndustryStandard/Pci30.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/GpioLib.h>
#include <Ppi/EndOfPeiPhase.h>
#include <Guid/SmramMemoryReserve.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <SaPolicyCommon.h>

#include <Guid/FirmwareFileSystem2.h>

#include <GpioPinsSklLp.h>
#include <Library/TimerLib.h>

/**
  Clear any SMI status or wake status left from boot.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
ClearIchSmiAndWake (
  VOID
  )
{
  EFI_STATUS          Status;
  EFI_BOOT_MODE       BootMode;
  UINT16              ABase;
  UINT16              Pm1Sts;
  UINT32              Pm1Cnt;
  UINT32              Data32;
  UINT32              Gpe0Sts;
  UINT32              SmiSts;
  UINT16              DevActSts;
  UINT16              TcoBase;
  UINT16              Tco1Sts;
  UINT16              LpcDeviceId;
  UINTN               LanBaseAddress;
  UINT16              LanPmcs;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);
  if (BootMode == BOOT_ON_S3_RESUME) {
    return EFI_SUCCESS;
  }

  PchAcpiBaseGet (&ABase);

  PchTcoBaseGet (&TcoBase);

  //
  // Read the ACPI registers
  //
  Pm1Sts  = IoRead16 (ABase + R_PCH_ACPI_PM1_STS);
  Pm1Cnt  = IoRead32 (ABase + R_PCH_ACPI_PM1_CNT);

  Gpe0Sts = IoRead32 (ABase + R_PCH_ACPI_GPE0_STS_127_96);
  SmiSts  = IoRead32 (ABase + R_PCH_SMI_STS);

  Data32 = 0;
  //
  // Before clear B_PCH_ACPI_GPE0_STS_127_96_PME_B0, PMES bit of internal device must be cleared at first.
  //
  Data32 = (Gpe0Sts & B_PCH_ACPI_GPE0_STS_127_96_PME_B0);
  if (Data32 != 0) {
    //
    // Check if Internal LAN waked up system
    //
    LanBaseAddress = MmPciBase (
                       DEFAULT_PCI_BUS_NUMBER_PCH,
                       PCI_DEVICE_NUMBER_PCH_LAN,
                       PCI_FUNCTION_NUMBER_PCH_LAN
                       );
    LanPmcs = MmioRead16 (LanBaseAddress + R_PCH_LAN_PMCS);
    if ((LanPmcs & B_PCH_LAN_PMCS_PMES) != 0) {
      //
      // Clear PMES bit
      //
      MmioOr16 (LanBaseAddress + R_PCH_LAN_PMCS, B_PCH_LAN_PMCS_PMES);
      DEBUG ((DEBUG_INFO, "Clear LAN PMES bit!\n"));
    }
  }

  LpcDeviceId = GetLpcDid ();
  //
  // Clear any SMI or wake state from the boot
  //
  Pm1Sts |=
    (
      B_PCH_ACPI_PM1_STS_WAK |
      B_PCH_ACPI_PM1_STS_PRBTNOR |
      B_PCH_ACPI_PM1_STS_RTC |
      B_PCH_ACPI_PM1_STS_PWRBTN |
      B_PCH_ACPI_PM1_STS_GBL |
      B_PCH_ACPI_PM1_STS_TMROF
    );

  Gpe0Sts |=
    (
      B_PCH_ACPI_GPE0_STS_127_96_PME_B0 |
      B_PCH_ACPI_GPE0_STS_127_96_PME |
      B_PCH_ACPI_GPE0_STS_127_96_PCI_EXP |
      B_PCH_ACPI_GPE0_STS_127_96_RI |
      B_PCH_ACPI_GPE0_STS_127_96_SMB_WAK |
      B_PCH_ACPI_GPE0_STS_127_96_TC0SCI |
      B_PCH_ACPI_GPE0_STS_127_96_HOT_PLUG |
      B_PCH_ACPI_GPE0_STS_127_96_LAN_WAKE
    );

  //
  // PCH Desktop
  //
  if (IS_PCH_LPC_DEVICE_ID_MOBILE (LpcDeviceId)) {
    Gpe0Sts |= B_PCH_ACPI_GPE0_STS_127_96_BATLOW;
  }

  SmiSts |=
    (
      B_PCH_SMI_STS_SMBUS |
      B_PCH_SMI_STS_PERIODIC |
      B_PCH_SMI_STS_TCO |
      B_PCH_SMI_STS_MCSMI |
      B_PCH_SMI_STS_SWSMI_TMR |
      B_PCH_SMI_STS_APM |
      B_PCH_SMI_STS_ON_SLP_EN |
      B_PCH_SMI_STS_BIOS
    );
  //
  // Write them back
  //
  IoWrite16 (ABase + R_PCH_ACPI_PM1_STS,  Pm1Sts);
  IoWrite32 (ABase + R_PCH_ACPI_GPE0_STS_127_96, Gpe0Sts);
  IoWrite32 (ABase + R_PCH_SMI_STS,  SmiSts);

  DevActSts = IoRead16 (ABase + R_PCH_DEVACT_STS);
  Tco1Sts = IoRead16 (TcoBase + R_PCH_TCO1_STS);

  DevActSts |=
    (
      B_PCH_DEVACT_STS_KBC |
      B_PCH_DEVACT_STS_PIRQDH |
      B_PCH_DEVACT_STS_PIRQCG |
      B_PCH_DEVACT_STS_PIRQBF |
      B_PCH_DEVACT_STS_PIRQAE
    );
  Tco1Sts |=
    (
      B_PCH_TCO1_STS_DMISERR |
      B_PCH_TCO1_STS_DMISMI |
      B_PCH_TCO1_STS_DMISCI |
      B_PCH_TCO1_STS_BIOSWR |
      B_PCH_TCO1_STS_NEWCENTURY |
      B_PCH_TCO1_STS_TIMEOUT |
      B_PCH_TCO1_STS_TCO_INT |
      B_PCH_TCO1_STS_SW_TCO_SMI
    );

  //
  // clear GPI SMI STS
  //
  GpioClearAllGpiSmiSts ();

  IoWrite16 (TcoBase + R_PCH_TCO1_STS, Tco1Sts);

  //
  // We do not want to write 1 to clear INTRD_DET bit.
  //
  IoWrite16 ((UINTN) (TcoBase + R_PCH_TCO2_STS), (UINT16) ~B_PCH_TCO2_STS_INTRD_DET);

  IoWrite16 (ABase + R_PCH_DEVACT_STS, DevActSts);

  //
  // Clear Power Failure (PWR_FLR), It was used to check S3_Resume State.
  //
  // MmioOr8 (
  //   LpcBaseAddress + R_PCH_PMC_GEN_PMCON_B,
  //   B_PCH_PMC_GEN_PMCON_B_PWR_FLR
  //   );

  return EFI_SUCCESS;
}

/**
  Late Silicon Initialization
**/
VOID
LateSiliconInit (
  VOID
  )
{
  ClearIchSmiAndWake ();
}

