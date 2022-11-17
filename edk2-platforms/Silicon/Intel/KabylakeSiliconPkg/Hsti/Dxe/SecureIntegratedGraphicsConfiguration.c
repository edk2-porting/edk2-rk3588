/** @file
  This file contains the tests for the SecureIntegratedGraphics bit

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "HstiSiliconDxe.h"

/**
  Run tests for SecureIntegratedGraphicsConfiguration bit
**/
VOID
CheckSecureIntegratedGraphicsConfiguration (
  VOID
  )
{
  EFI_STATUS      Status;
  BOOLEAN         Result;
  UINT32          MchBar;
  UINT16          Ggc;
  UINT32          Pavpc;
  UINT32          Bdsm;
  UINT32          Bgsm;
  UINT32          Tolud;
  UINT32          GsmSize;
  UINT32          DsmSize;
  UINT32          Data32;
  UINT32          PcmBase;
  CHAR16          *HstiErrorString;

  if ((mFeatureImplemented[1] & HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION) == 0) {
    return;
  }

  Result = TRUE;

  MchBar = (UINT32) MmioRead64 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,SA_MC_DEV,SA_MC_FUN) + R_SA_MCHBAR) & B_SA_MCHBAR_MCHBAR_MASK;

  DEBUG ((DEBUG_INFO, "  Table 3-9. Processor Graphics Security Configuration\n"));

  DEBUG ((DEBUG_INFO, "    1. GGC\n"));

  Ggc = MmioRead16 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,SA_MC_DEV,SA_MC_FUN) + R_SA_GGC);
  if ((Ggc & B_SA_GGC_GGCLCK_MASK) == 0) {
    DEBUG ((DEBUG_INFO, "Fail: GGCLCK bit not set\n"));

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_CODE_1 ,HSTI_PROCESSOR_GRAPHICS_SECURITY_CONFIGURATION, HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_STRING_1);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    2. PAVPC\n"));

  Pavpc = MmioRead32 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,SA_MC_DEV,SA_MC_FUN) + R_SA_PAVPC);
  if ((Pavpc & B_SA_PAVPC_PAVPLCK_MASK) == 0) {
    DEBUG ((DEBUG_INFO, "Fail: PAVPLCK bit not set\n"));

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_CODE_1 ,HSTI_PROCESSOR_GRAPHICS_SECURITY_CONFIGURATION, HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_STRING_1);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    3. BDSM\n"));

  Bdsm = MmioRead32 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,SA_MC_DEV,SA_MC_FUN) + R_SA_BDSM);
  if ((Bdsm & B_SA_BDSM_LOCK_MASK) == 0) {
    DEBUG ((DEBUG_INFO, "Fail: BDSM not locked \n"));

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_CODE_1 ,HSTI_PROCESSOR_GRAPHICS_SECURITY_CONFIGURATION, HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_STRING_1);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    4. BGSM\n"));

  Bgsm = MmioRead32 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,SA_MC_DEV,SA_MC_FUN) + R_SA_BGSM);
  if ((Bdsm & B_SA_BGSM_LOCK_MASK) == 0) {
    DEBUG ((DEBUG_INFO, "Fail: BGSM not locked \n"));

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_CODE_1 ,HSTI_PROCESSOR_GRAPHICS_SECURITY_CONFIGURATION, HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_STRING_1);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    5. PAVP Settings\n"));

  Data32 = MmioRead32 (MchBar + 0x5500);
  if ((Data32 & BIT0) == 0) {
    DEBUG ((DEBUG_INFO, "Fail: LockPAVP Settings not set \n"));

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_CODE_1 ,HSTI_PROCESSOR_GRAPHICS_SECURITY_CONFIGURATION, HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_STRING_1);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    6. IGD stolen memory consistency\n"));

  Tolud = MmioRead32 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,SA_MC_DEV,SA_MC_FUN) + R_SA_TOLUD);

  switch ((Ggc & B_SKL_SA_GGC_GGMS_MASK) >> N_SKL_SA_GGC_GGMS_OFFSET) {
    case V_SKL_SA_GGC_GGMS_2MB:
      GsmSize = SIZE_2MB;
      break;
    case V_SKL_SA_GGC_GGMS_4MB:
      GsmSize = SIZE_4MB;
      break;
    case V_SKL_SA_GGC_GGMS_8MB:
      GsmSize = SIZE_8MB;
      break;
    default:
      GsmSize = 0;
      break;
  }

  DsmSize = ((Ggc & B_SKL_SA_GGC_GMS_MASK) >> N_SKL_SA_GGC_GMS_OFFSET) * SIZE_32MB;

  DEBUG ((DEBUG_INFO, "DSM:   0x%08x - 0x%08x\n", Bdsm & B_SA_BDSM_BDSM_MASK, DsmSize));
  DEBUG ((DEBUG_INFO, "GSM:   0x%08x - 0x%08x\n", Bgsm & B_SA_BGSM_BGSM_MASK, GsmSize));
  DEBUG ((DEBUG_INFO, "TOLUD: 0x%08x\n", Tolud & B_SA_TOLUD_TOLUD_MASK));

  if ((Bgsm & B_SA_BGSM_BGSM_MASK) != (Bdsm & B_SA_BDSM_BDSM_MASK) - GsmSize) {
    DEBUG ((DEBUG_INFO, "Fail: BGSM: 0x%x != %x - %x \n",(Bgsm & B_SA_BGSM_BGSM_MASK),(Bdsm & B_SA_BDSM_BDSM_MASK),GsmSize));

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_CODE_2 ,HSTI_PROCESSOR_GRAPHICS_SECURITY_CONFIGURATION, HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_STRING_2);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }
  if ((Bdsm & B_SA_BDSM_BDSM_MASK) != (Tolud & B_SA_TOLUD_TOLUD_MASK) - DsmSize) {
    DEBUG ((DEBUG_INFO, "Fail: BDSM: 0x%x != %x - %x \n",(Bdsm & B_SA_BDSM_BDSM_MASK),(Tolud & B_SA_TOLUD_TOLUD_MASK),DsmSize));

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_CODE_2 ,HSTI_PROCESSOR_GRAPHICS_SECURITY_CONFIGURATION, HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_STRING_2);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    7. Reserved Check\n"));
  if ((Pavpc & B_SA_PAVPC_PCME_MASK) != 0) {
    PcmBase = Pavpc & B_SA_PAVPC_PCMBASE_MASK;
    DEBUG ((DEBUG_INFO, "PCMBASE: 0x%08x - 0x%08x\n", PcmBase, SIZE_1MB));
    if ((PcmBase < (Bdsm & B_SA_BDSM_BDSM_MASK)) ||
        (PcmBase > (Tolud & B_SA_TOLUD_TOLUD_MASK)) ||
        ((PcmBase + SIZE_1MB) < (Bdsm & B_SA_BDSM_BDSM_MASK)) ||
        ((PcmBase + SIZE_1MB) > (Tolud & B_SA_TOLUD_TOLUD_MASK))) {
      DEBUG ((DEBUG_INFO, "Fail: Graphics security reserved check failed \n"));

      HstiErrorString = BuildHstiErrorString (HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_CODE_3 ,HSTI_PROCESSOR_GRAPHICS_SECURITY_CONFIGURATION, HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_ERROR_STRING_3);
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

  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               1,
               HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION
               );
    ASSERT_EFI_ERROR (Status);
  }

  return;
}
