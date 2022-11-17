/** @file
  This file is SampleCode of the library for Intel CPU PEI Policy initialization.


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiCpuPolicyUpdate.h"
#include <Library/ConfigBlockLib.h>
#include <Library/PmcLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/SpiLib.h>

/**
  Check on the processor if SGX is supported.

  @retval True if SGX supported or FALSE if not
**/
BOOLEAN
IsSgxCapSupported (
  VOID
  )
{
  EFI_CPUID_REGISTER CpuidRegs;

  ///
  /// Processor support SGX feature by reading CPUID.(EAX=7,ECX=0):EBX[2]
  ///
  AsmCpuidEx (CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, 0, &CpuidRegs.RegEax,&CpuidRegs.RegEbx,&CpuidRegs.RegEcx,&CpuidRegs.RegEdx);

  ///
  /// SGX feature is supported only on WHL and later,
  /// with CPUID.(EAX=7,ECX=0):EBX[2]=1
  /// PRMRR configuration enabled, MSR IA32_MTRRCAP (FEh) [12] == 1
  ///
  if ((CpuidRegs.RegEbx & BIT2) && (AsmReadMsr64 (MSR_IA32_MTRRCAP) & BIT12)) {
    return TRUE;
  }

  return FALSE;
}

/**
  This function performs CPU PEI Policy initialization in Pre-memory.

  @param[in] SiPreMemPolicyPpi     The SI Pre-Mem Policy PPI instance

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicyPreMem (
  IN OUT  SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                      Status;
  EFI_BOOT_MODE                   BootMode;
  CPU_CONFIG_LIB_PREMEM_CONFIG    *CpuConfigLibPreMemConfig;
  CPU_OVERCLOCKING_PREMEM_CONFIG  *CpuOverClockingPreMemConfig;
  UINT32                          PchSpiBar0;
  UINT32                          MaxLogicProcessors;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuConfigLibPreMemConfigGuid, (VOID *) &CpuConfigLibPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuOverclockingPreMemConfigGuid, (VOID *) &CpuOverClockingPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "UpdatePeiCpuPolicyPreMem Start\n"));

  //
  // Get current boot mode
  //
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  SpiServiceInit ();

  PchSpiBar0 = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (
                                   DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                                   DEFAULT_PCI_BUS_NUMBER_PCH,
                                   PCI_DEVICE_NUMBER_PCH_SPI,
                                   PCI_FUNCTION_NUMBER_PCH_SPI,
                                   R_SPI_CFG_BAR0
                                   ));
  PchSpiBar0 &= ~(B_SPI_CFG_BAR0_MASK);

  if (PchSpiBar0 == 0) {
    DEBUG ((DEBUG_ERROR, "ERROR : PchSpiBar0 is invalid!\n"));
    ASSERT (FALSE);
  }

  CpuConfigLibPreMemConfig->PeciC10Reset = 0;
  CpuConfigLibPreMemConfig->CpuRatio = 0;
  ///
  /// Set PcdCpuMaxLogicalProcessorNumber to max number of logical processors enabled
  /// Read MSR_CORE_THREAD_COUNT (0x35) to check the total active Threads
  ///
  MaxLogicProcessors = (UINT32) (AsmReadMsr64 (MSR_CORE_THREAD_COUNT) & B_THREAD_COUNT_MASK);
  DEBUG ((DEBUG_INFO, "MaxLogicProcessors = %d\n", MaxLogicProcessors));
  PcdSet32S (PcdCpuMaxLogicalProcessorNumber, MaxLogicProcessors);

  return EFI_SUCCESS;
}

