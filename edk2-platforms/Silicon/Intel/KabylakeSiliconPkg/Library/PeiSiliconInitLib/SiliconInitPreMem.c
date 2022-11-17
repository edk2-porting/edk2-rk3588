/** @file
  Source code file for Platform Init Pre-Memory PEI module

Copyright (c) 2017 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <PiPei.h>
#include <PchAccess.h>
#include <SaPolicyCommon.h>
#include <CpuAccess.h>
#include <SaAccess.h>
#include <IndustryStandard/Pci30.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/MmPciLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchPmcLib.h>
#include <Ppi/MemoryDiscovered.h>
#include <Ppi/FirmwareVolumeInfo.h>
#include <Include/Library/ConfigBlockLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PchSerialIoLib.h>

/**
  Early Silicon initialization
**/
VOID
EarlySiliconInit (
  VOID
  )
{
  UINT16                          Data16;
  UINT8                           Data8;
  UINTN                           LpcBaseAddress;
  UINTN                           P2sbBase;

  LpcBaseAddress = MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_LPC,
                     PCI_FUNCTION_NUMBER_PCH_LPC
                     );

  ///
  /// Program bar
  ///
  P2sbBase = MmPciBase (
               DEFAULT_PCI_BUS_NUMBER_PCH,
               PCI_DEVICE_NUMBER_PCH_P2SB,
               PCI_FUNCTION_NUMBER_PCH_P2SB
               );

  MmioWrite32 (P2sbBase + R_PCH_P2SB_SBREG_BAR, PCH_PCR_BASE_ADDRESS);
  MmioOr8 (P2sbBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  ///
  /// Program ACPI BASE
  ///
  PchAcpiBaseSet (PcdGet16 (PcdAcpiBaseAddress));

  ///
  /// Program PWRM BASE
  ///
  PchPwrmBaseSet (PCH_PWRM_BASE_ADDRESS);

  ///
  /// Program TCO BASE if it is present and not locked
  ///
  if (PchIsTcoBaseSetValid ()) {
    PchTcoBaseSet (PcdGet16 (PcdTcoBaseAddress));
  }

  ///
  /// LPC I/O Configuration
  ///
  PchLpcIoDecodeRangesSet (
    (V_PCH_LPC_IOD_LPT_378  << N_PCH_LPC_IOD_LPT)  |
    (V_PCH_LPC_IOD_COMB_3E8 << N_PCH_LPC_IOD_COMB) |
    (V_PCH_LPC_IOD_COMA_3F8 << N_PCH_LPC_IOD_COMA)
    );

  PchLpcIoEnableDecodingSet (
    B_PCH_LPC_IOE_ME2  |
    B_PCH_LPC_IOE_SE   |
    B_PCH_LPC_IOE_ME1  |
    B_PCH_LPC_IOE_KE   |
    B_PCH_LPC_IOE_HGE  |
    B_PCH_LPC_IOE_LGE  |
    B_PCH_LPC_IOE_FDE  |
    B_PCH_LPC_IOE_PPE  |
    B_PCH_LPC_IOE_CBE  |
    B_PCH_LPC_IOE_CAE
    );


  ///
  /// Enable the upper 128-byte bank of RTC RAM
  ///
  PchPcrAndThenOr32 (PID_RTC, R_PCH_PCR_RTC_CONF, (UINT32)~0, B_PCH_PCR_RTC_CONF_UCMOS_EN);

  ///
  /// Disable the Watchdog timer expiration from causing a system reset
  ///
  PchPcrAndThenOr32 (PID_ITSS, R_PCH_PCR_ITSS_GIC, (UINT32)~0, B_PCH_PCR_ITSS_GIC_AME);

  ///
  /// Halt the TCO timer
  ///
  Data16 = IoRead16 (PcdGet16 (PcdTcoBaseAddress) + R_PCH_TCO1_CNT);
  Data16 |= B_PCH_TCO_CNT_TMR_HLT;
  IoWrite16 (PcdGet16 (PcdTcoBaseAddress) + R_PCH_TCO1_CNT, Data16);

  ///
  /// Clear the Second TO status bit
  ///
  IoWrite8 (PcdGet16 (PcdTcoBaseAddress) + R_PCH_TCO2_STS, B_PCH_TCO2_STS_SECOND_TO);

  ///
  /// Disable SERR NMI and IOCHK# NMI in port 61
  ///
  Data8 = IoRead8 (R_PCH_NMI_SC);
  Data8 |= (B_PCH_NMI_SC_PCI_SERR_EN | B_PCH_NMI_SC_IOCHK_NMI_EN);
  IoWrite8 (R_PCH_NMI_SC, Data8);

  PchPcrAndThenOr32 (PID_ITSS, R_PCH_PCR_ITSS_GIC, (UINT32)~B_PCH_PCR_ITSS_GIC_AME, 0);

  //
  // Program timer 1 as refresh timer
  //
  IoWrite8 (0x43, 0x54);
  IoWrite8 (0x41, 0x12);

}

// @todo: It should be moved Policy Init.
/**
  Initialize the GPIO IO selection, GPIO USE selection, and GPIO signal inversion registers

**/
VOID
SiliconInit (
  VOID
  )
{
  UINT16                          Data16;
  UINT8                           Data8;
  UINTN                           LpcBaseAddress;
  UINT16                            ABase;
  UINT16                            Pm1Sts;
  UINT32                            Pm1Cnt;

  LpcBaseAddress = MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_LPC,
                     PCI_FUNCTION_NUMBER_PCH_LPC
                     );

  ///
  /// LPC I/O Configuration
  ///
  PchLpcIoDecodeRangesSet (
    (V_PCH_LPC_IOD_LPT_378  << N_PCH_LPC_IOD_LPT)  |
    (V_PCH_LPC_IOD_COMB_3E8 << N_PCH_LPC_IOD_COMB) |
    (V_PCH_LPC_IOD_COMA_3F8 << N_PCH_LPC_IOD_COMA)
    );
  PchLpcIoEnableDecodingSet (
    B_PCH_LPC_IOE_ME2  |
    B_PCH_LPC_IOE_SE   |
    B_PCH_LPC_IOE_ME1  |
    B_PCH_LPC_IOE_KE   |
    B_PCH_LPC_IOE_HGE  |
    B_PCH_LPC_IOE_LGE  |
    B_PCH_LPC_IOE_FDE  |
    B_PCH_LPC_IOE_PPE  |
    B_PCH_LPC_IOE_CBE  |
    B_PCH_LPC_IOE_CAE
    );
  ///
  /// Enable the upper 128-byte bank of RTC RAM
  ///
  PchPcrAndThenOr32 (PID_RTC, R_PCH_PCR_RTC_CONF, (UINT32)~0, B_PCH_PCR_RTC_CONF_UCMOS_EN);
  ///
  /// Disable the Watchdog timer expiration from causing a system reset
  ///
  PchPcrAndThenOr32 (PID_SMB, R_PCH_PCR_SMBUS_GC, (UINT32)~0, B_PCH_PCR_SMBUS_GC_NR);

  ///
  /// Halt the TCO timer
  ///
  Data16 = IoRead16 (PcdGet16 (PcdTcoBaseAddress) + R_PCH_TCO1_CNT);
  Data16 |= B_PCH_TCO_CNT_TMR_HLT;
  IoWrite16 (PcdGet16 (PcdTcoBaseAddress) + R_PCH_TCO1_CNT, Data16);
  ///
  /// Clear the Second TO status bit
  ///
  IoWrite8 (PcdGet16 (PcdTcoBaseAddress) + R_PCH_TCO2_STS, B_PCH_TCO2_STS_SECOND_TO);
  ///
  /// Disable SERR NMI and IOCHK# NMI in port 61
  ///
  Data8 = IoRead8 (R_PCH_NMI_SC);
  Data8 |= (B_PCH_NMI_SC_PCI_SERR_EN | B_PCH_NMI_SC_IOCHK_NMI_EN);
  IoWrite8 (R_PCH_NMI_SC, Data8);


  ///
  /// Clear all pending SMI. On S3 clear power button enable so it will not generate an SMI.
  ///
  IoWrite16 (PcdGet16 (PcdAcpiBaseAddress) + R_PCH_ACPI_PM1_EN, 0);
  IoWrite32 (PcdGet16 (PcdAcpiBaseAddress) + R_PCH_ACPI_GPE0_EN_127_96, 0);
  ///----------------------------------------------------------------------------------
  ///
  /// BIOS should check the WAK_STS bit in PM1_STS[15] (PCH register ABASE+00h) before memory
  /// initialization to determine if ME has reset the system while the host was in a sleep state.
  /// If WAK_STS is not set, BIOS should ensure a non-sleep exit path is taken by overwriting
  /// PM1_CNT[12:10] (PCH register ABASE+04h) to 111b to force an s5 exit.
  ///
  PchAcpiBaseGet (&ABase);
  Pm1Sts = IoRead16 (ABase + R_PCH_ACPI_PM1_STS);
  if ((Pm1Sts & B_PCH_ACPI_PM1_STS_WAK) == 0) {
    Pm1Cnt = IoRead32 (ABase + R_PCH_ACPI_PM1_CNT);
    Pm1Cnt |= V_PCH_ACPI_PM1_CNT_S5;
    IoWrite32 (ABase + R_PCH_ACPI_PM1_CNT, Pm1Cnt);
  }

}
