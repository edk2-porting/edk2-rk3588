/** @file
  This file contains the tests for the SecureCPUConfiguration bit

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "HstiSiliconDxe.h"

/**
  Run tests for SecureCPUConfiguration bit
**/
VOID
CheckSecureCpuConfiguration (
  VOID
  )
{
  EFI_STATUS                        Status;
  BOOLEAN                           Result;
  UINTN                             CpuNumber;
  UINTN                             CpuIndex;
  CPUID_VERSION_INFO_ECX            Ecx;
  UINT8                             SramC;
  UINT32                            TsegMB;
  UINT32                            Bgsm;
  UINT64                            SmrrBase;
  UINT64                            SmrrMask;
  UINT64                            MasterSmrrBase;
  UINT64                            MasterSmrrMask;
  CHAR16                            *HstiErrorString;
  UINT64                            SmmFeatureControl;
  MSR_IA32_FEATURE_CONTROL_REGISTER Ia32FeatureControlMsr;
  MSR_IA32_MTRRCAP_REGISTER         MtrrCapMsr;

  if ((mFeatureImplemented[0] & HSTI_BYTE0_SECURE_CPU_CONFIGURATION) == 0) {
    return;
  }

  Result = TRUE;

  DEBUG ((DEBUG_INFO, "  Table 3-1. CPU Security Configuration\n"));

  CpuNumber = GetCpuNumber ();
  for (CpuIndex = 0; CpuIndex < CpuNumber; CpuIndex++) {
    DEBUG ((DEBUG_INFO, "  [CPU - 0x%x]\n", CpuIndex));

    DEBUG ((DEBUG_INFO, "    1. Microcode Update Revision\n"));
    if ((ProcessorReadMsr64 (CpuIndex, MSR_IA32_BIOS_SIGN_ID) & 0xFFFFFFFF00000000) == 0) {

      HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_1 ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_1);
      Status = HstiLibAppendErrorString (
                 PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                 NULL,
                 HstiErrorString
                 );
      ASSERT_EFI_ERROR (Status);
      Result = FALSE;
      FreePool (HstiErrorString);
    }

    DEBUG ((DEBUG_INFO, "    2. Sample Part \n"));

    if ((ProcessorReadMsr64 (CpuIndex, MSR_PLATFORM_INFO) & BIT27) != 0) {
      DEBUG ((DEBUG_INFO, "Fail: This is a sample part\n"));

      HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_2 ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_2);
      Status = HstiLibAppendErrorString (
                 PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                 NULL,
                 HstiErrorString
                 );
      ASSERT_EFI_ERROR (Status);
      Result = FALSE;
      FreePool (HstiErrorString);
    }

    DEBUG ((DEBUG_INFO, "    3. IA32_FEATURE_CONTROL MSR Lock\n"));

    Ia32FeatureControlMsr.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_IA32_FEATURE_CONTROL);
    if (Ia32FeatureControlMsr.Bits.Lock == 0) {
      DEBUG ((DEBUG_INFO, "Fail: This is a sample part\n"));

      HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_3 ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_3);
      Status = HstiLibAppendErrorString (
                 PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                 NULL,
                 HstiErrorString
                 );
      ASSERT_EFI_ERROR (Status);
      Result = FALSE;
      FreePool (HstiErrorString);
    }

    DEBUG ((DEBUG_INFO, "    4. SMM_FEATURE_CONTROL MSR Lock\n"));

    SmmFeatureControl = ProcessorReadMsr64 (CpuIndex, MSR_SMM_FEATURE_CONTROL);

    DEBUG ((DEBUG_INFO, "    4. SMM_CODE_CHK_EN TEST\n"));

    if ((SmmFeatureControl & B_SMM_CODE_CHK_EN) == 0) {

      DEBUG ((DEBUG_INFO, "Fail: SMM Code Fetch outside SMRAM dectect feature disabled\n"));

      HstiErrorString = BuildHstiErrorString(HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_4 ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_B);
      Status = HstiLibAppendErrorString(
                 PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                 NULL,
                 HstiErrorString
                 );
      ASSERT_EFI_ERROR (Status);
      Result = FALSE;
      FreePool(HstiErrorString);
    }

    DEBUG ((DEBUG_INFO, "    SMM_FEATURE_CONTROL MSR Lock\n"));

    if ((SmmFeatureControl & B_SMM_FEATURE_CONTROL_LOCK) == 0) {

      DEBUG ((DEBUG_INFO, "Fail: Smm feature control msr not locked\n"));

      HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_4 ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_4);
      Status = HstiLibAppendErrorString (
                 PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                 NULL,
                 HstiErrorString
                 );
      ASSERT_EFI_ERROR (Status);
      Result = FALSE;
      FreePool (HstiErrorString);
    }

    DEBUG ((DEBUG_INFO, "    5. FEATURE_CONFIG MSR  Lock\n"));

    ProcessorCpuid (CpuIndex, CPUID_VERSION_INFO, NULL, NULL, &Ecx.Uint32, NULL);
    if (Ecx.Bits.AESNI == 1) {
      if ((ProcessorReadMsr64 (CpuIndex, MSR_IA32_FEATURE_CONFIG) & B_IA32_FEATURE_CONFIG_LOCK) == 0) {
        DEBUG ((DEBUG_INFO, "Fail: Feature control msr not locked\n"));

        HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_5 ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_5);
        Status = HstiLibAppendErrorString (
                   PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                   NULL,
                   HstiErrorString
                   );
        ASSERT_EFI_ERROR (Status);
        Result = FALSE;
        FreePool (HstiErrorString);
      }
    }
  }

  DEBUG ((DEBUG_INFO, "  Table 7-4. SMM Security Configuration\n"));

  DEBUG ((DEBUG_INFO, "    1. SMRAMC\n"));

  SramC = MmioRead8 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,SA_MC_DEV,SA_MC_FUN) + R_SA_SMRAMC);
  if ((SramC & B_SA_SMRAMC_D_LCK_MASK) == 0) {
    DEBUG ((DEBUG_INFO, "Fail: SMRAMC not locked\n"));

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_8 ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_8);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    2. TSEGMB\n"));

  TsegMB = MmioRead32 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,SA_MC_DEV,SA_MC_FUN) + R_SA_TSEGMB);
  if ((TsegMB & B_SA_TSEGMB_LOCK_MASK) == 0) {
    DEBUG ((DEBUG_INFO, "Fail: TSEGMB not locked\n"));

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_8 ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_8);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    3. TSEGMB Alignment\n"));

  if (((TsegMB & B_SA_TSEGMB_TSEGMB_MASK) & (LShiftU64 (1,20) - 1)) != 0) {
    DEBUG ((DEBUG_INFO, "Fail: TSEGMB not size aligned, TSEG size: 0x%x\n",(LShiftU64 (1,20) - 1)));

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_9 ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_9);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  MasterSmrrBase = 0;
  MasterSmrrMask = 0;
  for (CpuIndex = 0; CpuIndex < CpuNumber; CpuIndex++) {
    DEBUG ((DEBUG_INFO, "  [CPU - 0x%x]\n", CpuIndex));

    DEBUG ((DEBUG_INFO, "    4. SMRR1 are supported\n"));

    MtrrCapMsr.Uint64 = ProcessorReadMsr64 (CpuIndex, MSR_IA32_MTRRCAP);
    if (MtrrCapMsr.Bits.SMRR == 0) {
      DEBUG ((DEBUG_INFO, "Fail: SMRR1 not supported\n"));

      HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_A ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_A);
      Status = HstiLibAppendErrorString (
                 PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                 NULL,
                 HstiErrorString
                 );
      ASSERT_EFI_ERROR (Status);
      Result = FALSE;
      FreePool (HstiErrorString);
    } else {
      DEBUG ((DEBUG_INFO, "    5. SMRR1 programmed consistently on all cores\n"));

      SmrrBase = ProcessorReadMsr64 (CpuIndex, MSR_IA32_SMRR_PHYSBASE);
      SmrrMask = ProcessorReadMsr64 (CpuIndex, MSR_IA32_SMRR_PHYSMASK);

      if (CpuIndex == 0) {
        MasterSmrrBase = SmrrBase;
        MasterSmrrMask = SmrrMask;
      } else {
        if ((SmrrBase != MasterSmrrBase) || (SmrrMask != MasterSmrrMask)) {
          DEBUG ((DEBUG_INFO, "Fail: SMRR1 not programmed consistently across all cores\n"));

          HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_A ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_A);
          Status = HstiLibAppendErrorString (
                     PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                     NULL,
                     HstiErrorString
                     );
          ASSERT_EFI_ERROR (Status);
          Result = FALSE;
          FreePool (HstiErrorString);
        }
      }

      DEBUG ((DEBUG_INFO, "    6. SMRR1 enabled/correct\n"));

      if (((SmrrMask & BIT11) == 0x0) ||
          ((SmrrMask & BIT9) != 0x0) ||
          ((SmrrBase & 0x7) != 0x6)) {
        DEBUG ((DEBUG_INFO, "Fail: SMRR1 not enabled/correct\n"));

        HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_A ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_A);
        Status = HstiLibAppendErrorString (
                   PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                   NULL,
                   HstiErrorString
                   );
        ASSERT_EFI_ERROR (Status);
        Result = FALSE;
        FreePool (HstiErrorString);
      }

      DEBUG ((DEBUG_INFO, "    7. SMRR1 and TSEGMB match\n"));

      DEBUG ((DEBUG_INFO, "SMRR:   0x%08x - 0x%08x\n", SmrrBase & (SmrrMask & 0xFFFFF000), (UINT32) (~(SmrrMask & 0xFFFFF000) + 1)));
      DEBUG ((DEBUG_INFO, "TSEGMB: 0x%08x\n", TsegMB & B_SA_TSEGMB_TSEGMB_MASK));

      if ((SmrrBase & (SmrrMask & 0xFFFFF000)) != (TsegMB & B_SA_TSEGMB_TSEGMB_MASK)) {
        DEBUG ((DEBUG_INFO, "Fail: SMRR1 != TSEGMB\n"));

        HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_A ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_A);
        Status = HstiLibAppendErrorString (
                   PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                   NULL,
                   HstiErrorString
                   );
        ASSERT_EFI_ERROR (Status);
        Result = FALSE;
        FreePool (HstiErrorString);
      }

      DEBUG ((DEBUG_INFO, "    8. SMRR1 size\n"));
      Bgsm = MmioRead32 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,SA_MC_DEV,SA_MC_FUN) + R_SA_BGSM);

      if ((UINT32) (~(SmrrMask & 0xFFFFF000) + 1) !=
          ((Bgsm & B_SA_BGSM_BGSM_MASK) - (TsegMB & B_SA_TSEGMB_TSEGMB_MASK))) {
        DEBUG ((DEBUG_INFO, "Fail: SMRR1 size != BGSM-TSEGMB\n"));

        HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_A ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_A);
        Status = HstiLibAppendErrorString (
                   PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                   NULL,
                   HstiErrorString
                   );
        ASSERT_EFI_ERROR (Status);
        Result = FALSE;
        FreePool (HstiErrorString);
      }

      DEBUG ((DEBUG_INFO, "    9. SMRR1 work\n"));

      if (MmioRead32 (SmrrBase & SmrrMask) != 0xFFFFFFFF) {
        DEBUG ((DEBUG_INFO, "Fail: SMRR1 not working, read succeeded\n"));


        HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_CODE_A ,HSTI_CPU_SECURITY_CONFIGURATION, HSTI_BYTE0_SECURE_CPU_CONFIGURATION_ERROR_STRING_A);
        Status = HstiLibAppendErrorString (
                   PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                   NULL,
                   HstiErrorString
                   );
        ASSERT_EFI_ERROR (Status);
        Result = FALSE;
        FreePool (HstiErrorString);
      }
    }
  }

  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               0,
               HSTI_BYTE0_SECURE_CPU_CONFIGURATION
               );
    ASSERT_EFI_ERROR (Status);
  }

  return;
}
