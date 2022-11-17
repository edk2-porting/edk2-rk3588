/** @file
  This file contains the tests for the SecuePCHConfiguration bit

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "HstiSiliconDxe.h"

/**
  Run tests for SecurePCHConfiguration bit
**/
VOID
CheckSecurePchConfiguration (
  VOID
  )
{

  EFI_STATUS      Status;
  BOOLEAN         Result;
  UINT16          PmBase;
  UINT64          Tbar;
  UINT32          TbarL;
  UINT32          TbarH;
  UINT8           Srdl;
  UINT32          GenPmConLock;
  UINT32          PmCfg;
  UINT32          SmiEn;
  UINT8           GenPmCon1;
  UINT16          Tco1Cnt;
  UINT32          Hostc;
  UINT16          Cmd;
  CHAR16          *HstiErrorString;
  UINT32          PchSpiBar0;
  UINT32          PchPwrmBase;
  UINT32          PcrRtc;
  UINT16          TcoBase;
  BOOLEAN         ThermalPciMode;
  BOOLEAN         ThermalAcpiMode;
  UINT32          ThermalPciBase;
  BOOLEAN         ThermalPciModeTbarConfigured;

  ThermalPciMode = FALSE;
  ThermalAcpiMode = FALSE;
  ThermalPciModeTbarConfigured = FALSE;

  if ((mFeatureImplemented[1] & HSTI_BYTE1_SECURE_PCH_CONFIGURATION) == 0) {
    return;
  }

  Result = TRUE;

  PchSpiBar0 = MmioRead32 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,PCI_DEVICE_NUMBER_PCH_SPI,PCI_FUNCTION_NUMBER_PCH_SPI) + R_PCH_SPI_BAR0) & ~B_PCH_SPI_BAR0_MASK;

  PchAcpiBaseGet (&PmBase);
  PchTcoBaseGet (&TcoBase);

  DEBUG ((DEBUG_INFO, "  Table 3-10. PCH Security Configuration\n"));

  DEBUG ((DEBUG_INFO, "    1. Thermal Throttling Locks\n"));

  //
  // Check Device VID and DID , if data is not 0xFFFFFFFF, then it's in PCI Mode
  //
  ThermalPciBase = (UINT32) (UINTN) MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,PCI_DEVICE_NUMBER_PCH_THERMAL,PCI_FUNCTION_NUMBER_PCH_THERMAL);
  if (MmioRead32 (ThermalPciBase + PCI_VENDOR_ID_OFFSET) != 0xFFFFFFFF) {
    ThermalPciMode = TRUE;
  }
  if (ThermalPciMode) {

    TbarL = MmioRead32 (ThermalPciBase + R_PCH_THERMAL_TBAR);
    TbarH = MmioRead32 (ThermalPciBase + R_PCH_THERMAL_TBARH);

    TbarL = TbarL & B_PCH_THERMAL_TBAR_MASK;
    Tbar = ((UINT64) TbarH << 32) | TbarL;

    //
    // Check if Tbar is initialized. If Tbar is 0 , then Tbar is not initialized , Use PCH_THERMAL_BASE_ADDRESS for Tbar
    // and write TbarL and TbarH back to register
    //
    if (Tbar == 0) {
      Tbar = (UINT64) PCH_THERMAL_BASE_ADDRESS;
      TbarL = (UINT32) Tbar;
      TbarH = (UINT32) ((UINT64) Tbar >> 32);
      MmioWrite32 (ThermalPciBase + R_PCH_THERMAL_TBAR, TbarL);
      MmioWrite32 (ThermalPciBase + R_PCH_THERMAL_TBARH, TbarH);
      ThermalPciModeTbarConfigured = TRUE;
    }
    //
    // Make sure MSE bit is enabled before trying to access Tbar MMIO
    //
    Cmd = MmioRead16 (ThermalPciBase + PCI_COMMAND_OFFSET);
    MmioWrite16 (ThermalPciBase + PCI_COMMAND_OFFSET, Cmd | EFI_PCI_COMMAND_MEMORY_SPACE);

  //
  // Not in Pci Mode
  //
  } else {
    Tbar = (UINT64) PCH_THERMAL_BASE_ADDRESS;
    //
    // This is to probe the Thermal in ACPI mode.
    // If Thermal is in ACPI mode, the TBAR must be PCH_THERMAL_BASE_ADDRESS and the offset 0 (TEMP) must return valid temperature.
    //
    if (MmioRead16 ((UINT32)Tbar + 0x00) != 0xFFFF) {
      ThermalAcpiMode = TRUE;
    }
  }

  if (ThermalPciMode || ThermalAcpiMode) {
    if ((MmioRead32 (Tbar + R_PCH_TBAR_TCFD) & B_PCH_TBAR_TCFD_TCD) == 0) {
      if (((MmioRead8 (Tbar + R_PCH_TBAR_TSC) & B_PCH_TBAR_TSC_PLD) == 0) ||
        ((MmioRead8 (Tbar + R_PCH_TBAR_TSEL) & B_PCH_TBAR_TSEL_PLD) == 0) ||
        ((MmioRead8 (Tbar + R_PCH_TBAR_TSMIC) & B_PCH_TBAR_TSMIC_PLD) == 0) ||
        ((MmioRead32 (Tbar + R_PCH_TBAR_TL) & B_PCH_TBAR_TL_LOCK) == 0) ||
        ((MmioRead8 (Tbar + R_PCH_TBAR_PHLC) & BIT0) == 0)) {

        HstiErrorString = BuildHstiErrorString (HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_CODE_1 ,HSTI_PCH_SECURITY_CONFIGURATION, HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_STRING_1);
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
  if (ThermalPciMode) {
    //
    // Restore CMD register
    //
    MmioWrite16 ((ThermalPciBase + PCI_COMMAND_OFFSET), Cmd);
    //
    // If Tbar had been configured because of initialization. Restore Tbar ,TbarH and TbarL
    //
    if (ThermalPciModeTbarConfigured) {
      MmioWrite32 (ThermalPciBase + R_PCH_THERMAL_TBAR, 0);
      MmioWrite32 (ThermalPciBase + R_PCH_THERMAL_TBARH, 0);
    }
  }

  DEBUG ((DEBUG_INFO, "    2. Set Strap Lock\n"));

  Srdl = MmioRead8 (PchSpiBar0 + R_PCH_SPI_SSML);
  if ((Srdl & B_PCH_SPI_SSML_SSL) == 0) {
    DEBUG ((DEBUG_INFO, "Set Strap Lock %x\n",Srdl));

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_CODE_2 ,HSTI_PCH_SECURITY_CONFIGURATION, HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_STRING_2);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    3. ACPI Base Lock\n"));

  GenPmConLock = MmioRead32 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,PCI_DEVICE_NUMBER_PCH_PMC,PCI_FUNCTION_NUMBER_PCH_PMC) + R_PCH_PMC_GEN_PMCON_B);

  if ((GenPmConLock & B_PCH_PMC_GEN_PMCON_B_ACPI_BASE_LOCK) == 0) {

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_CODE_2 ,HSTI_PCH_SECURITY_CONFIGURATION, HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_STRING_2);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    4. PM_CFG\n"));

  PchPwrmBaseGet (&PchPwrmBase);
  PmCfg = MmioRead32 (PchPwrmBase + R_PCH_PWRM_CFG);

  if (((PmCfg & BIT22) == 0) ||
      ((PmCfg & BIT27) == 0)) {

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_CODE_3 ,HSTI_PCH_SECURITY_CONFIGURATION, HSTI_BYTE1_SECURE_PCH_CONFIGURATION_ERROR_STRING_3);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    5. SMI Enable\n"));

  SmiEn = IoRead32 (PmBase + R_PCH_SMI_EN);
  if ((SmiEn & B_PCH_SMI_EN_GBL_SMI) == 0x0) {

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_D ,HSTI_PCH_SECURITY_CONFIGURATION, HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_D);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    6. SMI Lock\n"));

  GenPmCon1 = MmioRead8 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,PCI_DEVICE_NUMBER_PCH_PMC,PCI_FUNCTION_NUMBER_PCH_PMC) + R_PCH_PMC_GEN_PMCON_A);
  if ((GenPmCon1 & B_PCH_PMC_GEN_PMCON_A_SMI_LOCK) == 0x0) {
    HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_D ,HSTI_PCH_SECURITY_CONFIGURATION, HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_D);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    7. TCO SMI Lock\n"));

  Tco1Cnt = IoRead16 (TcoBase + R_PCH_TCO1_CNT);
  if ((Tco1Cnt & B_PCH_TCO_CNT_LOCK) == 0x0) {
    DEBUG ((DEBUG_INFO, "TCO SMI Lock %x\n",(Tco1Cnt & B_PCH_TCO_CNT_LOCK)));

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_E ,HSTI_PCH_SECURITY_CONFIGURATION, HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_E);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  DEBUG ((DEBUG_INFO, "    8. RTC Bios Interface Lock\n"));

  PcrRtc = MmioRead32 (PCH_PCR_ADDRESS (PID_RTC,R_PCH_PCR_RTC_CONF));

  if ((PcrRtc & BIT31) == 0x0) {
    DEBUG ((DEBUG_INFO, "Rtc BILD %x\n",(PcrRtc & BIT31)));

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_CODE_4 ,HSTI_PCH_SECURITY_CONFIGURATION, HSTI_BYTE0_BOOT_FIRMWARE_MEDIA_PROTECTION_ERROR_STRING_4);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }


  DEBUG ((DEBUG_INFO, "  Table 7-7. SPD Configuration and Write Protection\n"));

  Hostc = MmioRead32 (MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH,PCI_DEVICE_NUMBER_PCH_SMBUS,PCI_FUNCTION_NUMBER_PCH_SMBUS) + R_PCH_SMBUS_HOSTC);
  if ((Hostc & B_PCH_SMBUS_HOSTC_SPDWD) == 0) {

    HstiErrorString = BuildHstiErrorString (HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_ERROR_CODE_4 ,HSTI_PROCESSOR_SPD_SECURITY_CONFIGURATION, HSTI_BYTE1_SECURE_MEMORY_MAP_CONFIGURATION_ERROR_STRING_4);
    Status = HstiLibAppendErrorString (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               HstiErrorString
               );
    ASSERT_EFI_ERROR (Status);
    Result = FALSE;
    FreePool (HstiErrorString);
  }

  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               1,
               HSTI_BYTE1_SECURE_PCH_CONFIGURATION
               );
    ASSERT_EFI_ERROR (Status);
  }

  return;
}
