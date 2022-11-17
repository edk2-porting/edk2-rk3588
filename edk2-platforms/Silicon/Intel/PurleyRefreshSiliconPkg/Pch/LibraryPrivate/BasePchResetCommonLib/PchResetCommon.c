/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <PchAccess.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/MmPciBaseLib.h>
#include <IncludePrivate/Library/PchResetCommonLib.h>


EFI_STATUS
EFIAPI
PchResetCallback (
  IN     PCH_RESET_TYPE           PchResetType
  );

/**
  Initialize an Pch Reset ppi/protocol instance.

  @param[in] PchResetInstance     Pointer to PchResetInstance to initialize

  @retval EFI_SUCCESS             The protocol instance was properly initialized
  @exception EFI_UNSUPPORTED      The PCH is not supported by this module
**/
EFI_STATUS
PchResetConstructor (
  PCH_RESET_INSTANCE          *PchResetInstance
  )
{
  UINTN   PmcBaseAddress;

  ///
  /// Initialize the Reset protocol instance
  ///
  PchResetInstance->Signature               = PCH_RESET_SIGNATURE;
  PchResetInstance->Handle                  = NULL;

  ///
  /// Sanity check to ensure PMC ACPI/PM BASE initialization has occurred previously.
  ///
  PmcBaseAddress = MmPciBase (
                     DEFAULT_PCI_BUS_NUMBER_PCH,
                     PCI_DEVICE_NUMBER_PCH_PMC,
                     PCI_FUNCTION_NUMBER_PCH_PMC
                     );
  PchResetInstance->PchPmcBase = PmcBaseAddress;
  PchPwrmBaseGet (&(PchResetInstance->PchPwrmBase));
  ASSERT (PchResetInstance->PchPwrmBase != 0);
  PchAcpiBaseGet (&(PchResetInstance->PchAcpiBase));
  ASSERT (PchResetInstance->PchAcpiBase != 0);


  return EFI_SUCCESS;
}

/**
  Execute Pch Reset from the host controller.
  @param[in] PchResetInstance     Pointer to PchResetInstance to initialize
  @param[in] PchResetType         Pch Reset Types which includes ColdReset, WarmReset, ShutdownReset,
                                  PowerCycleReset, GlobalReset, GlobalResetWithEc

  @retval EFI_SUCCESS             Successfully completed.
  @retval EFI_INVALID_PARAMETER   If ResetType is invalid.
**/
EFI_STATUS
PchReset (
  IN  PCH_RESET_INSTANCE *PchResetInstance,
  IN  PCH_RESET_TYPE     PchResetType
  )
{
  UINTN      PmcBaseAddress;
  UINT16     ABase;
  UINT8      OutputData;
  UINT32     Data32;
  UINT16     Data16;
  EFI_STATUS Status;

  PmcBaseAddress = PchResetInstance->PchPmcBase;
  ABase          = PchResetInstance->PchAcpiBase;
  switch (PchResetType) {
    case ColdReset:
      IoWrite8 ((UINTN) R_PCH_RST_CNT, (UINT8) V_PCH_RST_CNT_HARDSTARTSTATE);
      OutputData = V_PCH_RST_CNT_FULLRESET;
      break;

    case WarmReset:
      IoWrite8 ((UINTN) R_PCH_RST_CNT, (UINT8) V_PCH_RST_CNT_SOFTSTARTSTATE);
      OutputData = V_PCH_RST_CNT_HARDRESET;
      break;

    case ShutdownReset:
      ///
      /// Firstly, ACPI decode must be enabled
      ///
      MmioOr8 (
        PmcBaseAddress + R_PCH_PMC_ACPI_CNT,
        (UINT8) (B_PCH_PMC_ACPI_CNT_ACPI_EN)
        );

      ///
      /// Then, GPE0_EN should be disabled to avoid any GPI waking up the system from S5
      ///
      IoWrite32 ((UINTN) (ABase + R_PCH_ACPI_GPE0_EN_127_96), 0);

      ///
      /// Secondly, PwrSts register must be cleared
      ///
      /// Write a "1" to bit[8] of power button status register at
      /// (PM_BASE + PM1_STS_OFFSET) to clear this bit
      ///
      Data16 = B_PCH_SMI_STS_PM1_STS_REG;
      IoWrite16 ((UINTN) (ABase + R_PCH_SMI_STS), Data16);

      ///
      /// Finally, transform system into S5 sleep state
      ///
      Data32  = IoRead32 ((UINTN) (ABase + R_PCH_ACPI_PM1_CNT));

      Data32  = (UINT32) ((Data32 &~(B_PCH_ACPI_PM1_CNT_SLP_TYP + B_PCH_ACPI_PM1_CNT_SLP_EN)) | V_PCH_ACPI_PM1_CNT_S5);

      IoWrite32 ((UINTN) (ABase + R_PCH_ACPI_PM1_CNT), Data32);

      Data32 = Data32 | B_PCH_ACPI_PM1_CNT_SLP_EN;

      IoWrite32 ((UINTN) (ABase + R_PCH_ACPI_PM1_CNT), Data32);
      return EFI_SUCCESS;

    case PowerCycleReset:
    case GlobalReset:
    case GlobalResetWithEc:
      ///
      /// PCH BIOS Spec Section 4.6 GPIO Reset Requirement
      ///

      if ((PchResetType == GlobalReset) || (PchResetType == GlobalResetWithEc)) {
        MmioOr32 (
          PmcBaseAddress + R_PCH_PMC_ETR3,
          (UINT32) (B_PCH_PMC_ETR3_CF9GR)
          );
      }
      OutputData = V_PCH_RST_CNT_FULLRESET;
      break;

    default:
      return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_ERROR, "Resetting the platform (%02x)...\n", OutputData));

  Status = PchResetCallback (PchResetType);

  if ((Status == EFI_SUCCESS) || (Status == EFI_NOT_FOUND)) {
    IoWrite8 ((UINTN) R_PCH_RST_CNT, OutputData);
    ///
    /// Waiting for system reset
    ///
    CpuDeadLoop ();
  }

  return Status;
}
