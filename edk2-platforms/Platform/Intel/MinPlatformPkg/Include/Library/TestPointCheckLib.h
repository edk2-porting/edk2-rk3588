/** @file

Copyright (c) 2017 - 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _TEST_POINT_CHECK_LIB_H_
#define _TEST_POINT_CHECK_LIB_H_

#include <PiPei.h>
#include <Uefi.h>

//
// Below is Test Point Hook Point.
//
// Naming: TestPoint<Phase/Event><Function>
//
// Phase/Event(PEI) = MemoryDiscovered|EndOfPei
// Phase/Event(DXE) = PciEnumerationDone|EndOfDxe|DxeSmmReadyToLock|ReadyToBoot
// Phase/Event(SMM) = SmmEndOfDxe|SmmReadyToLock|SmmReadyToBoot
//

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  This service verifies the system state after Exit Boot Services is invoked.

  @retval EFI_SUCCESS         The test point check was performed successfully.
**/
EFI_STATUS
EFIAPI
TestPointExitBootServices (
  VOID
  );

/**
  This service verifies the system state within SMM after Exit Boot Services is invoked.

  @retval EFI_SUCCESS         The test point check was performed successfully.
**/
EFI_STATUS
EFIAPI
TestPointSmmExitBootServices (
  VOID
  );

//
// Below is detail definition for MinPlatform implementation
//

#define TEST_POINT_IMPLEMENTATION_ID_PLATFORM         L"Minimum Platform TestPoint"
#define TEST_POINT_IMPLEMENTATION_ID_PLATFORM_PEI     TEST_POINT_IMPLEMENTATION_ID_PLATFORM L" (PEI)"
#define TEST_POINT_IMPLEMENTATION_ID_PLATFORM_DXE     TEST_POINT_IMPLEMENTATION_ID_PLATFORM L" (DXE)"
#define TEST_POINT_IMPLEMENTATION_ID_PLATFORM_SMM     TEST_POINT_IMPLEMENTATION_ID_PLATFORM L" (SMM)"

#define TEST_POINT_FEATURE_SIZE        0x10

#define TEST_POINT_ERROR                                                                    L"Error "
#define TEST_POINT_PLATFORM_TEST_POINT                                                      L" Platform TestPoint"

// Byte 0 - SEC/PEI
#define TEST_POINT_TEMP_MEMORY_INIT_DONE                                                    L" - Temp Memory Init Done - "
#define TEST_POINT_DEBUG_INIT_DONE                                                          L" - Debug Init Done - "

#define TEST_POINT_BYTE0_TEMP_INIT_DONE                                                     BIT0
#define TEST_POINT_BYTE0_DEBUG_INIT_DONE                                                    BIT1

// Byte 1/2 - PEI
#define TEST_POINT_MEMORY_DISCOVERED                                                        L" - Memory Discovered - "
#define TEST_POINT_END_OF_PEI                                                               L" - End Of PEI - "

#define TEST_POINT_BYTE1_MEMORY_DISCOVERED_MTRR_FUNCTIONAL                                  BIT0
#define TEST_POINT_BYTE1_MEMORY_DISCOVERED_MEMORY_RESOURCE_FUNCTIONAL                       BIT1
#define TEST_POINT_BYTE1_MEMORY_DISCOVERED_FV_INFO_FUNCTIONAL                               BIT2
#define TEST_POINT_BYTE1_MEMORY_DISCOVERED_DMA_PROTECTION_ENABLED                           BIT3
#define   TEST_POINT_BYTE1_MEMORY_DISCOVERED_MTRR_FUNCTIONAL_ERROR_CODE                          L"0x01000000"
#define   TEST_POINT_BYTE1_MEMORY_DISCOVERED_MTRR_FUNCTIONAL_ERROR_STRING                        L"Invalid MTRR Setting\r\n"
#define   TEST_POINT_BYTE1_MEMORY_DISCOVERED_MEMORY_RESOURCE_FUNCTIONAL_ERROR_CODE               L"0x01010000"
#define   TEST_POINT_BYTE1_MEMORY_DISCOVERED_MEMORY_RESOURCE_FUNCTIONAL_ERROR_STRING             L"Invalid Memory Resource\r\n"
#define   TEST_POINT_BYTE1_MEMORY_DISCOVERED_FV_INFO_FUNCTIONAL_ERROR_CODE                       L"0x01020000"
#define   TEST_POINT_BYTE1_MEMORY_DISCOVERED_FV_INFO_FUNCTIONAL_ERROR_STRING                     L"Invalid FV Information\r\n"
#define   TEST_POINT_BYTE1_MEMORY_DISCOVERED_DMA_PROTECTION_ENABLED_ERROR_CODE                   L"0x01030000"
#define   TEST_POINT_BYTE1_MEMORY_DISCOVERED_DMA_PROTECTION_ENABLED_ERROR_STRING                 L"DMA protection disabled\r\n"

#define TEST_POINT_BYTE2_END_OF_PEI_SYSTEM_RESOURCE_FUNCTIONAL                              BIT0
#define TEST_POINT_BYTE2_END_OF_PEI_MTRR_FUNCTIONAL                                         BIT1
#define TEST_POINT_BYTE2_END_OF_PEI_PCI_BUS_MASTER_DISABLED                                 BIT2
#define   TEST_POINT_BYTE2_END_OF_PEI_SYSTEM_RESOURCE_FUNCTIONAL_ERROR_CODE                      L"0x02000000"
#define   TEST_POINT_BYTE2_END_OF_PEI_SYSTEM_RESOURCE_FUNCTIONAL_ERROR_STRING                    L"Invalid System Resource\r\n"
#define   TEST_POINT_BYTE2_END_OF_PEI_MTRR_FUNCTIONAL_ERROR_CODE                                 L"0x02010000"
#define   TEST_POINT_BYTE2_END_OF_PEI_MTRR_FUNCTIONAL_ERROR_STRING                               L"Invalid MTRR Setting\r\n"
#define   TEST_POINT_BYTE2_END_OF_PEI_PCI_BUS_MASTER_DISABLED_ERROR_CODE                         L"0x02020000"
#define   TEST_POINT_BYTE2_END_OF_PEI_PCI_BUS_MASTER_DISABLED_ERROR_STRING                       L"PCI Bus Master Enabled\r\n"

// Byte 3/4/5 - DXE
#define TEST_POINT_PCI_ENUMERATION_DONE                                                     L" - PCI Enumeration Done - "
#define TEST_POINT_END_OF_DXE                                                               L" - End Of DXE - "
#define TEST_POINT_DXE_SMM_READY_TO_LOCK                                                    L" - DXE SMM Ready To Lock - "
#define TEST_POINT_READY_TO_BOOT                                                            L" - Ready To Boot - "
#define TEST_POINT_EXIT_BOOT_SERVICES                                                       L" - Exit Boot Services - "

#define TEST_POINT_BYTE3_PCI_ENUMERATION_DONE_RESOURCE_ALLOCATED                            BIT0
#define TEST_POINT_BYTE3_PCI_ENUMERATION_DONE_BUS_MASTER_DISABLED                           BIT1
#define TEST_POINT_BYTE3_END_OF_DXE_NO_THIRD_PARTY_PCI_OPTION_ROM                           BIT2
#define TEST_POINT_BYTE3_END_OF_DXE_DMA_ACPI_TABLE_FUNCTIONAL                               BIT3
#define TEST_POINT_BYTE3_END_OF_DXE_DMA_PROTECTION_ENABLED                                  BIT4
#define   TEST_POINT_BYTE3_PCI_ENUMERATION_DONE_RESOURCE_ALLOCATED_ERROR_CODE                    L"0x03000000"
#define   TEST_POINT_BYTE3_PCI_ENUMERATION_DONE_RESOURCE_ALLOCATED_ERROR_STRING                  L"Invalid PCI Resource\r\n"
#define   TEST_POINT_BYTE3_PCI_ENUMERATION_DONE_BUS_MASTER_DISABLED_ERROR_CODE                   L"0x03010000"
#define   TEST_POINT_BYTE3_PCI_ENUMERATION_DONE_BUS_MASTER_DISABLED_ERROR_STRING                 L"PCI Bus Master Enabled\r\n"
#define   TEST_POINT_BYTE3_END_OF_DXE_NO_THIRD_PARTY_PCI_OPTION_ROM_ERROR_CODE                   L"0x03020000"
#define   TEST_POINT_BYTE3_END_OF_DXE_NO_THIRD_PARTY_PCI_OPTION_ROM_ERROR_STRING                 L"Third Party Option ROM dispatched\r\n"
#define   TEST_POINT_BYTE3_END_OF_DXE_DMA_ACPI_TABLE_FUNCTIONAL_ERROR_CODE                       L"0x03030000"
#define   TEST_POINT_BYTE3_END_OF_DXE_DMA_ACPI_TABLE_FUNCTIONAL_ERROR_STRING                     L"No DMA ACPI table\r\n"
#define   TEST_POINT_BYTE3_END_OF_DXE_DMA_PROTECTION_ENABLED_ERROR_CODE                          L"0x03040000"
#define   TEST_POINT_BYTE3_END_OF_DXE_DXE_DMA_PROTECTION_ENABLED_ERROR_STRING                    L"DMA protection disabled\r\n"

#define TEST_POINT_BYTE4_READY_TO_BOOT_MEMORY_TYPE_INFORMATION_FUNCTIONAL                   BIT0
#define TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_MEMORY_ATTRIBUTE_TABLE_FUNCTIONAL               BIT1
#define TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_BOOT_VARIABLE_FUNCTIONAL                        BIT2
#define TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_CONSOLE_VARIABLE_FUNCTIONAL                     BIT3
#define TEST_POINT_BYTE4_READY_TO_BOOT_ACPI_TABLE_FUNCTIONAL                                BIT4
#define TEST_POINT_BYTE4_READY_TO_BOOT_GCD_RESOURCE_FUNCTIONAL                              BIT5
#define   TEST_POINT_BYTE4_READY_TO_BOOT_MEMORY_TYPE_INFORMATION_FUNCTIONAL_ERROR_CODE           L"0x04000000"
#define   TEST_POINT_BYTE4_READY_TO_BOOT_MEMORY_TYPE_INFORMATION_FUNCTIONAL_ERROR_STRING         L"Invalid Memory Type Information\r\n"
#define   TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_MEMORY_ATTRIBUTE_TABLE_FUNCTIONAL_ERROR_CODE       L"0x04010000"
#define   TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_MEMORY_ATTRIBUTE_TABLE_FUNCTIONAL_ERROR_STRING     L"Invalid Memory Attribute Table\r\n"
#define   TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_BOOT_VARIABLE_FUNCTIONAL_ERROR_CODE                L"0x04020000"
#define   TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_BOOT_VARIABLE_FUNCTIONAL_ERROR_STRING              L"Invalid Boot Variable\r\n"
#define   TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_CONSOLE_VARIABLE_FUNCTIONAL_ERROR_CODE             L"0x04030000"
#define   TEST_POINT_BYTE4_READY_TO_BOOT_UEFI_CONSOLE_VARIABLE_FUNCTIONAL_ERROR_STRING           L"Invalid Console Variable\r\n"
#define   TEST_POINT_BYTE4_READY_TO_BOOT_ACPI_TABLE_FUNCTIONAL_ERROR_CODE                        L"0x04040000"
#define   TEST_POINT_BYTE4_READY_TO_BOOT_ACPI_TABLE_FUNCTIONAL_ERROR_STRING                      L"Invalid ACPI Table\r\n"
#define   TEST_POINT_BYTE4_READY_TO_BOOT_GCD_RESOURCE_FUNCTIONAL_ERROR_CODE                      L"0x04050000"
#define   TEST_POINT_BYTE4_READY_TO_BOOT_GCD_RESOURCE_FUNCTIONAL_ERROR_STRING                    L"Invalid GCD Resource\r\n"

#define TEST_POINT_BYTE5_READY_TO_BOOT_UEFI_SECURE_BOOT_ENABLED                             BIT0
#define TEST_POINT_BYTE5_READY_TO_BOOT_PI_SIGNED_FV_BOOT_ENABLED                            BIT1
#define TEST_POINT_BYTE5_READY_TO_BOOT_TCG_TRUSTED_BOOT_ENABLED                             BIT2
#define TEST_POINT_BYTE5_READY_TO_BOOT_TCG_MOR_ENABLED                                      BIT3
#define   TEST_POINT_BYTE5_READY_TO_BOOT_UEFI_SECURE_BOOT_ENABLED_ERROR_CODE                     L"0x05000000"
#define   TEST_POINT_BYTE5_READY_TO_BOOT_UEFI_SECURE_BOOT_ENABLED_ERROR_STRING                   L"UEFI Secure Boot Disable\r\n"
#define   TEST_POINT_BYTE5_READY_TO_BOOT_PI_SIGNED_FV_BOOT_ENABLED_ERROR_CODE                    L"0x05010000"
#define   TEST_POINT_BYTE5_READY_TO_BOOT_PI_SIGNED_FV_BOOT_ENABLED_ERROR_STRING                  L"PI Signed FV Boot Disable\r\n"
#define   TEST_POINT_BYTE5_READY_TO_BOOT_TCG_TRUSTED_BOOT_ENABLED_ERROR_CODE                     L"0x05020000"
#define   TEST_POINT_BYTE5_READY_TO_BOOT_TCG_TRUSTED_BOOT_ENABLED_ERROR_STRING                   L"TCG Trusted Boot Disable\r\n"
#define   TEST_POINT_BYTE5_READY_TO_BOOT_TCG_MOR_ENABLED_ERROR_CODE                              L"0x05030000"
#define   TEST_POINT_BYTE5_READY_TO_BOOT_TCG_MOR_ENABLED_ERROR_STRING                            L"TCG MOR not enabled\r\n"

// Byte 6/7 - SMM
#define TEST_POINT_SMM_END_OF_DXE                                                           L" - SMM End Of DXE - "
#define TEST_POINT_SMM_READY_TO_LOCK                                                        L" - SMM Ready To Lock - "
#define TEST_POINT_SMM_READY_TO_BOOT                                                        L" - SMM Ready To Boot - "
#define TEST_POINT_SMM_EXIT_BOOT_SERVICES                                                   L" - SMM Exit Boot Services - "

#define TEST_POINT_INDEX_BYTE6_SMM                                                          6
#define TEST_POINT_BYTE6_SMM_END_OF_DXE_SMRR_FUNCTIONAL                                     BIT0
#define TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SMM_MEMORY_ATTRIBUTE_TABLE_FUNCTIONAL            BIT1
#define TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SECURE_SMM_COMMUNICATION_BUFFER                  BIT2
#define TEST_POINT_BYTE6_SMM_READY_TO_BOOT_SMM_PAGE_LEVEL_PROTECTION                        BIT3
#define   TEST_POINT_BYTE6_SMM_END_OF_DXE_SMRR_FUNCTIONAL_ERROR_CODE                             L"0x06000000"
#define   TEST_POINT_BYTE6_SMM_END_OF_DXE_SMRR_FUNCTIONAL_ERROR_STRING                           L"Invalid SMRR\r\n"
#define   TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SMM_MEMORY_ATTRIBUTE_TABLE_FUNCTIONAL_ERROR_CODE    L"0x06010000"
#define   TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SMM_MEMORY_ATTRIBUTE_TABLE_FUNCTIONAL_ERROR_STRING  L"Invalid SMM Memory Attribute Table\r\n"
#define   TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SECURE_SMM_COMMUNICATION_BUFFER_ERROR_CODE          L"0x06020000"
#define   TEST_POINT_BYTE6_SMM_READY_TO_LOCK_SECURE_SMM_COMMUNICATION_BUFFER_ERROR_STRING        L"Unsecure SMM communication buffer\r\n"
#define   TEST_POINT_BYTE6_SMM_READY_TO_BOOT_SMM_PAGE_LEVEL_PROTECTION_ERROR_CODE                L"0x06030000"
#define   TEST_POINT_BYTE6_SMM_READY_TO_BOOT_SMM_PAGE_LEVEL_PROTECTION_ERROR_STRING              L"SMM page level protection disabled\r\n"

#define TEST_POINT_BYTE7_DXE_SMM_READY_TO_LOCK_SMRAM_ALIGNED                                BIT0
#define TEST_POINT_BYTE7_DXE_SMM_READY_TO_LOCK_WSMT_TABLE_FUNCTIONAL                        BIT1
#define TEST_POINT_BYTE7_DXE_SMM_READY_TO_BOOT_SMI_HANDLER_INSTRUMENT                       BIT2
#define   TEST_POINT_BYTE7_DXE_SMM_READY_TO_LOCK_SMRAM_ALIGNED_ERROR_CODE                        L"0x07000000"
#define   TEST_POINT_BYTE7_DXE_SMM_READY_TO_LOCK_SMRAM_ALIGNED_ERROR_STRING                      L"Invalid SMRAM Information\r\n"
#define   TEST_POINT_BYTE7_DXE_SMM_READY_TO_LOCK_WSMT_TABLE_FUNCTIONAL_ERROR_CODE                L"0x07010000"
#define   TEST_POINT_BYTE7_DXE_SMM_READY_TO_LOCK_WSMT_TABLE_FUNCTIONAL_ERROR_STRING              L"No WSMT table\r\n"
#define   TEST_POINT_BYTE7_DXE_SMM_READY_TO_BOOT_SMI_HANDLER_INSTRUMENT_ERROR_CODE               L"0x07020000"
#define   TEST_POINT_BYTE7_DXE_SMM_READY_TO_BOOT_SMI_HANDLER_INSTRUMENT_ERROR_STRING             L"No SMI Instrument\r\n"

// Byte 8 - Advanced
#define TEST_POINT_BYTE8_READY_TO_BOOT_ESRT_TABLE_FUNCTIONAL                                BIT0
#define TEST_POINT_BYTE8_READY_TO_BOOT_HSTI_TABLE_FUNCTIONAL                                BIT1
#define   TEST_POINT_BYTE8_READY_TO_BOOT_ESRT_TABLE_FUNCTIONAL_ERROR_CODE                        L"0x08000000"
#define   TEST_POINT_BYTE8_READY_TO_BOOT_ESRT_TABLE_FUNCTIONAL_ERROR_STRING                      L"No ESRT\r\n"
#define   TEST_POINT_BYTE8_READY_TO_BOOT_HSTI_TABLE_FUNCTIONAL_ERROR_CODE                        L"0x08010000"
#define   TEST_POINT_BYTE8_READY_TO_BOOT_HSTI_TABLE_FUNCTIONAL_ERROR_STRING                      L"No HSTI\r\n"

#pragma pack (1)

typedef struct {
  UINT32  Version;
  UINT32  Role;
  CHAR16  ImplementationID[256];
  UINT32  FeaturesSize;
  UINT8   FeaturesImplemented[TEST_POINT_FEATURE_SIZE];
  UINT8   FeaturesVerified[TEST_POINT_FEATURE_SIZE];
  CHAR16  End;
} ADAPTER_INFO_PLATFORM_TEST_POINT_STRUCT;

#pragma pack ()

#endif
