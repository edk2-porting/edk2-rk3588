/** @file
  CPU PEI Policy Update & initialization.


  Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiCpuPolicyUpdate.h"
#include <Library/ConfigBlockLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/CpuPlatformLib.h>

/**
  This function performs CPU PEI Policy initialization.

  @param[in] SiPolicyPpi           The SI Policy PPI instance

  @retval EFI_SUCCESS              The PPI is installed and initialized.
  @retval EFI ERRORS               The PPI is not successfully installed.
  @retval EFI_OUT_OF_RESOURCES     Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicy (
  IN OUT  SI_POLICY_PPI   *SiPolicyPpi
  )
{
  EFI_STATUS                       Status;
  CPU_CONFIG                       *CpuConfig;
  CPU_POWER_MGMT_BASIC_CONFIG      *CpuPowerMgmtBasicConfig;
  SI_PREMEM_POLICY_PPI             *SiPreMemPolicyPpi;
  CPU_POWER_MGMT_CUSTOM_CONFIG     *CpuPowerMgmtCustomConfig;
  CPU_POWER_MGMT_TEST_CONFIG      *CpuPowerMgmtTestConfig;
  CPU_TEST_CONFIG                 *CpuTestConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuConfigGuid, (VOID *) &CpuConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuPowerMgmtBasicConfigGuid, (VOID *) &CpuPowerMgmtBasicConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock((VOID *)SiPolicyPpi, &gCpuPowerMgmtCustomConfigGuid, (VOID *)&CpuPowerMgmtCustomConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *)SiPolicyPpi, &gCpuTestConfigGuid, (VOID *)&CpuTestConfig);
  ASSERT_EFI_ERROR(Status);

  Status = PeiServicesLocatePpi (
                &gSiPreMemPolicyPpiGuid,
                0,
                NULL,
                (VOID **) &SiPreMemPolicyPpi
                );
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock((VOID *)SiPolicyPpi, &gCpuPowerMgmtTestConfigGuid, (VOID *)&CpuPowerMgmtTestConfig);
  ASSERT_EFI_ERROR(Status);

  //
  // Init Power Management Policy Variables
  //
  CpuPowerMgmtBasicConfig->HwpInterruptControl = 1;
  CpuPowerMgmtCustomConfig->CustomRatioTable.MaxRatio = 0x4;
  CpuPowerMgmtBasicConfig->OneCoreRatioLimit = 0x22;
  CpuPowerMgmtBasicConfig->TwoCoreRatioLimit = 0x22;
  CpuPowerMgmtBasicConfig->ThreeCoreRatioLimit = 0x22;
  CpuPowerMgmtBasicConfig->FourCoreRatioLimit = 0x22;
  CpuPowerMgmtBasicConfig->FiveCoreRatioLimit = 0;
  CpuPowerMgmtBasicConfig->SixCoreRatioLimit = 0;
  CpuPowerMgmtBasicConfig->SevenCoreRatioLimit = 0;
  CpuPowerMgmtBasicConfig->EightCoreRatioLimit = 0;
  CpuPowerMgmtBasicConfig->Hwp = 0x1;
  CpuTestConfig->CpuWakeUpTimer = 1;
  CpuPowerMgmtTestConfig->AutoThermalReporting = 0;

  return EFI_SUCCESS;
}

