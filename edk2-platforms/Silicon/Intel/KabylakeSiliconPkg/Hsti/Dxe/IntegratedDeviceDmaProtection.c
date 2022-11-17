/** @file
  This file contains the tests for the IntegratedDeviceDMAProtection bit

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "HstiSiliconDxe.h"

/**
  Run tests for IntegratedDeviceDMAProtection bit
**/
VOID
CheckIntegratedDeviceDmaProtection (
  VOID
  )
{
  EFI_STATUS      Status;
  BOOLEAN         Result;
  UINT32          MchBar;
  UINT32          Vtd1;
  UINT32          Vtd2;
  CHAR16          *HstiErrorString;

  if ((mFeatureImplemented[0] & HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION) == 0) {
    return;
  }

  Result = TRUE;

  MchBar = (UINT32) MmioRead64 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,SA_MC_DEV,SA_MC_FUN) + R_SA_MCHBAR) & B_SA_MCHBAR_MCHBAR_MASK;

  DEBUG ((DEBUG_INFO, "  VTd check\n"));

  DEBUG ((DEBUG_INFO, "    1. VTd supported\n"));

  if ((MmioRead32 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,SA_MC_DEV,SA_MC_FUN) + R_SA_MC_CAPID0_A_OFFSET) & BIT23) != 0) {
    DEBUG ((DEBUG_INFO, "Fail: VTd not supported\n"));


    HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_ERROR_CODE_1 ,HSTI_INTEGRATED_DEVICE_DMA_PROTECTION, HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_ERROR_STRING_1);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  if ((MmioRead32 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,SA_MC_DEV,SA_MC_FUN) + R_SA_MC_CAPID0_A_OFFSET) & BIT23) == 0) {
    DEBUG ((DEBUG_INFO, "    2. VTd enabled\n"));

    if (MmioRead16 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,2,0) + R_SA_IGD_VID) != 0xFFFF) {
      Vtd1 = MmioRead32 (MchBar + R_SA_MCHBAR_VTD1_OFFSET);
      if (((Vtd1 & BIT0) == 0) ||
          ((Vtd1 & 0xfffffffe) == 0)) {
        DEBUG ((DEBUG_INFO, "Fail: VTd not enabled\n"));


        HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_ERROR_CODE_1 ,HSTI_INTEGRATED_DEVICE_DMA_PROTECTION, HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_ERROR_STRING_1);
        Status = HstiLibAppendErrorString (
                   PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                   NULL,
                   HstiErrorString
                   );
        ASSERT_EFI_ERROR (Status);
        Result = FALSE;
        FreePool (HstiErrorString);
      }

      if ((Vtd1 & 0xfffffffe) != 0) {
        DEBUG ((DEBUG_INFO, "    3. VTd configured correctly\n"));

        if ((MmioRead32 ((Vtd1 & 0xfffffffe) + 0xFF0) & BIT31) == 0) {
          DEBUG ((DEBUG_INFO, "Fail: VTd not configured correctly\n"));

          HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_ERROR_CODE_1 ,HSTI_INTEGRATED_DEVICE_DMA_PROTECTION, HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_ERROR_STRING_1);
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

    DEBUG ((DEBUG_INFO, "    2. VTd enabled\n"));

    Vtd2 = MmioRead32 (MchBar + R_SA_MCHBAR_VTD2_OFFSET);
    if (((Vtd2 & BIT0) == 0) ||
        ((Vtd2 & 0xfffffffe) == 0)) {
      DEBUG ((DEBUG_INFO, "Fail: VTd2 not enabled\n"));

      HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_ERROR_CODE_1 ,HSTI_INTEGRATED_DEVICE_DMA_PROTECTION, HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_ERROR_STRING_1);
      Status = HstiLibAppendErrorString (
                 PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
                 NULL,
                 HstiErrorString
                 );
      ASSERT_EFI_ERROR (Status);
      Result = FALSE;
      FreePool (HstiErrorString);
    }

    if ((Vtd2 & 0xfffffffe) != 0) {
      DEBUG ((DEBUG_INFO, "    3. VTd configured correctly\n"));

      if ((MmioRead32 ((Vtd2 & 0xfffffffe) + 0xFF0) & BIT31) == 0) {
        DEBUG ((DEBUG_INFO, "Fail: VTd2 not configured correctly\n"));

        HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_ERROR_CODE_1 ,HSTI_INTEGRATED_DEVICE_DMA_PROTECTION, HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION_ERROR_STRING_1);
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
               HSTI_BYTE0_INTEGRATED_DEVICE_DMA_PROTECTION
               );
    ASSERT_EFI_ERROR (Status);
  }

  return;
}
