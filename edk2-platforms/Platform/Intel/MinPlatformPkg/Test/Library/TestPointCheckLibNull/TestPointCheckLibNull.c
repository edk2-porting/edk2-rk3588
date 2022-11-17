/** @file

Copyright (c) 2017 - 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/TestPointCheckLib.h>

/**
  This service tests temporary memory functionality.

  Test subject: Temporary Memory
  Test overview: Reads/writes results on the stack and heap.
  Reporting mechanism: PPI

  @param[in]  TempRamStart    A pointer to the start of temporary memory.
  @param[in]  TempRamEnd      A pointer to the end of temporary memory.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointTempMemoryFunction (
  IN VOID   *TempRamStart,
  IN VOID   *TempRamEnd
  )
{
  return EFI_SUCCESS;
}

/**
  This service tests debug configuration after debug device initialization.

  Test subject: Debug Capability
  Test overview: Dumps a struct of debug configuration parameters to the debug log.
  Reporting mechanism: Serial port shows the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointDebugInitDone (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies MTRR settings after memory is discovered.

  Test subject: MTRRs after memory is discovered.
  Test overview: Verifies MTRR settings.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps results to the debug log.

  Examples of settings verified: No MTRR overlap, PEI data memory is writeback, and flash region is WP, MMIO is UC, etc.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointMemoryDiscoveredMtrrFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies discovered memory resources after memory is discovered.

  Test subject: Resource description HOBs.
  Test overview: No memory resource overlap.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps results to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointMemoryDiscoveredMemoryResourceFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the validity of published firmware volume resources.

  Test subject: FV HOB and FV Info PPI.
  Test overview: FV HOB and FV Info PPI.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps results to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointMemoryDiscoveredFvInfoFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies DMA protection configuration in PEI after memory is discovered.

  Test subject: DMA protection.
  Test overview: DMA protection in PEI.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointMemoryDiscoveredDmaProtectionEnabled (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies system resources at the end of PEI.

  Test subject: Resource HOB and SMRAM HOB.
  Test overview: SMRAM configuration and no system resource overlap.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps results to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointEndOfPeiSystemResourceFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies MTRR settings at the end of PEI.

  Test subject: MTRRs after end of PEI.
  Test overview: Verifies MTRR settings.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps results to the debug log.

  Examples of settings verified: No MTRR overlap, DXE data memory is writeback, flash region may be UC, MMIO is UC, etc.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointEndOfPeiMtrrFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies bus master enable (BME) is disabled at the end of PEI.

  Test subject: PCI device BME.
  Test overview: Verify BME is cleared.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps results to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointEndOfPeiPciBusMasterDisabled (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies bus master enable (BME) is disabled after PCI enumeration.

  Test subject: PCI device BME.
  Test overview: Verify BME is cleared.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps results to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointPciEnumerationDonePciBusMasterDisabled (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies PCI device resource assignment after PCI enumeration.

  Test subject: PCI device resources.
  Test overview: Verify all PCI devices have been assigned proper resources.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps PCI resource assignments to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointPciEnumerationDonePciResourceAllocated (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies no 3rd party PCI option ROMs (OPROMs) were dispatched prior to the end of DXE.

  Test subject: 3rd party OPROMs.
  Test overview: Verify no 3rd party PCI OPROMs were .
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps PCI resource assignments to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointEndOfDxeNoThirdPartyPciOptionRom (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the DMA ACPI table is reported at the end of DXE.

  Test subject: DMA protection.
  Test overview: DMA ACPI table is reported.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the DMA ACPI table to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointEndOfDxeDmaAcpiTableFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies DMA protection configuration at the end of DXE.

  Test subject: DMA protection.
  Test overview: DMA protection in DXE.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the DMA ACPI table to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointEndOfDxeDmaProtectionEnabled (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the validity of System Management RAM (SMRAM) alignment at SMM Ready To Lock.

  Test subject: SMRAM Information.
  Test overview: SMRAM is aligned.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the SMRAM region table to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointDxeSmmReadyToLockSmramAligned (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the validity of the Windows SMM Security Mitigation Table (WSMT) at SMM Ready To Lock.

  Test subject: Windows Security SMM Mitigation Table.
  Test overview: The table is reported in compliance with the Windows SMM Security Mitigations Table
                 ACPI table specification.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the WSMT to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointDxeSmmReadyToLockWsmtTableFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the validity of the SMM page table at Ready To Boot.

  Test subject: SMM page table.
  Test overview: The SMM page table settings matches the SmmMemoryAttribute table.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Reports an error if verification fails.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointDxeSmmReadyToBootSmmPageProtection (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This services verifies the validity of installed ACPI tables at Ready To Boot.

  Test subject: ACPI tables.
  Test overview: The ACPI table settings are valid.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the installed ACPI tables.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointReadyToBootAcpiTableFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This services verifies ACPI table resources are in the GCD.

  Test subject: ACPI memory resources.
  Test overview: Memory resources are described consistently in both ACPI and GCD.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the installed ACPI tables and GCD.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointReadyToBootGcdResourceFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the validity of the memory type information settings.

  Test subject: Memory type information.
  Test overview: Inspect an verify memory type information is correct.
                 Confirm no fragmentation exists in the ACPI/Reserved/Runtime regions.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the memory type information settings to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointReadyToBootMemoryTypeInformationFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the validity of the memory type information settings.

  Test subject: Memory type information.
  Test overview: Inspect an verify memory type information is correct.
                 Confirm no fragmentation exists in the ACPI/Reserved/Runtime regions.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the memory type information settings to the debug log.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointReadyToBootUefiMemoryAttributeTableFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the validity of the UEFI memory attribute table.

  Test subject: UEFI memory attribute table.
  Test overview: The UEFI memeory attribute table is reported. The image code/data is consistent with the table.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the UEFI image information and the UEFI memory attribute table.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointReadyToBootUefiBootVariableFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the consle variable information.

  Test subject: Console.
  Test overview: Inspect and verify the console variable information is correct.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the console variable information.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointReadyToBootUefiConsoleVariableFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the HSTI table.

  Test subject: HSTI table.
  Test overview: Verify the HSTI table is reported.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the HSTI table.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointReadyToBootHstiTableFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the ESRT table.

  Test subject: ESRT table.
  Test overview: Verify the ESRT table is reported.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the ESRT table.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointReadyToBootEsrtTableFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies UEFI Secure Boot is enabled.

  Test subject: UEFI Secure Boot.
  Test overview: Verify the SecureBoot variable is set.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the SecureBoot variable.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointReadyToBootUefiSecureBootEnabled (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies Platform Initialization (PI) Signed FV Boot is enabled.

  Test subject: PI Signed FV Boot.
  Test overview: Verify PI signed FV boot is enabled.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointReadyToBootPiSignedFvBootEnabled (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies TCG Trusted Boot is enabled.

  Test subject: TCG Trusted Boot.
  Test overview: Verify the TCG protocol is installed.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the TCG protocol capability.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointReadyToBootTcgTrustedBootEnabled (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies TCG Memory Overwrite Request (MOR) is enabled.

  Test subject: TCG MOR.
  Test overview: Verify the MOR UEFI variable is set.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the MOR UEFI variable.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointReadyToBootTcgMorEnabled (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies SMI handler profiling.

  Test subject: SMI handler profiling.
  Test overview:
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the SMI handler profile.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointDxeSmmReadyToBootSmiHandlerInstrument (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies SMRR configuration at the End of DXE.

  Test subject: SMRR.
  Test overview: Verify SMRR is aligned and SMRR matches SMRAM_INFO.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps SMRR and SMRAM_INFO.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointSmmEndOfDxeSmrrFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the validity of the SMM memory atttribute table at SMM Ready To Lock.

  Test subject: SMM memory attribute table.
  Test overview: Verify the SMM memory attribute table is reported.
                 Verify image code/data is consistent with the SMM memory attribute table.
                 Verify the GDT/IDT/PageTable is RO, data is NX, and code is RO.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Dumps the SMM memory attribute table and SMM image information.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointSmmReadyToLockSmmMemoryAttributeTableFunctional (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the security of SMM communication buffers at SMM Ready To Lock.

  Test subject: SMM communication buffer.
  Test overview: Verify only CommBuffer and MMIO are mapped in the page table.
  Reporting mechanism: Dumps the memory map and GCD map at SmmReadyToLock and checks at SmmReadyToBoot.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointSmmReadyToLockSecureSmmCommunicationBuffer (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the validity of the SMM page table at SMM Ready To Boot.

  Test subject: SMM page table.
  Test overview: Verify the SMM page table matches the SMM memory attribute table.
  Reporting mechanism: Set ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT.
                       Reports an error message upon checking.

  @retval EFI_SUCCESS         The test point check was performed successfully.
  @retval EFI_UNSUPPORTED     The test point check is not supported on this platform.
**/
EFI_STATUS
EFIAPI
TestPointSmmReadyToBootSmmPageProtection (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the system state after Exit Boot Services is invoked.

  @retval EFI_SUCCESS         The test point check was performed successfully.
**/
EFI_STATUS
EFIAPI
TestPointExitBootServices (
  VOID
  )
{
  return EFI_SUCCESS;
}

/**
  This service verifies the system state within SMM after Exit Boot Services is invoked.

  @retval EFI_SUCCESS         The test point check was performed successfully.
**/
EFI_STATUS
EFIAPI
TestPointSmmExitBootServices (
  VOID
  )
{
  return EFI_SUCCESS;
}
