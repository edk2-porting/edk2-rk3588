/** @file
  This is the Uefi driver that initializes the Intel PCH.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include "PchInit.h"
#include <PchPolicyCommon.h>
#include <Protocol/PcieIoTrap.h>
#include <Library/PchInfoLib.h>
#include <Library/PmcLib.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <PcieRegs.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/PmcRegs.h>

GLOBAL_REMOVE_IF_UNREFERENCED EFI_HANDLE               mImageHandle;
GLOBAL_REMOVE_IF_UNREFERENCED UINT16                   mPcieIoTrapAddress;

VOID
EFIAPI
PchOnBootToOs (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );


VOID
EFIAPI
PchOnExitBootServices (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );


VOID
EFIAPI
PchOnReadyToBoot (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

/**
  Process all the lock downs
**/
VOID
ProcessSmiLocks (
  VOID
  )
{
  UINT32        Data32And;
  UINT32        Data32Or;
  UINT16        ABase;

  ///
  /// PCH BIOS Spec Section 3.6 Flash Security Recommendation
  /// BIOS needs to enables SMI_LOCK (PMC PCI offset A0h[4] = 1b) which prevent writes
  /// to the Global SMI Enable bit (GLB_SMI_EN ABASE + 30h[0]). Enabling this bit will
  /// mitigate malicious software attempts to gain system management mode privileges.
  ///
  if (mPchConfigHob->LockDown.GlobalSmi == TRUE) {
    ///
    /// Save Global SMI Enable bit setting before BIOS enables SMI_LOCK during S3 resume
    ///
    ABase = PmcGetAcpiBase ();
    Data32Or = IoRead32 ((UINTN) (ABase + R_ACPI_IO_SMI_EN));
    if ((Data32Or & B_ACPI_IO_SMI_EN_GBL_SMI) != 0) {
      Data32And = 0xFFFFFFFF;
      Data32Or |= B_ACPI_IO_SMI_EN_GBL_SMI;
      S3BootScriptSaveIoReadWrite (
        S3BootScriptWidthUint32,
        (UINTN) (ABase + R_ACPI_IO_SMI_EN),
        &Data32Or,  // Data to be ORed
        &Data32And  // Data to be ANDed
        );
    }
      PmcLockSmiWithS3BootScript ();
  }
}

/**
  Do PCIE power management while resume from S3
**/
VOID
ReconfigurePciePowerManagementForS3 (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINT32                                Data32;
  PCH_PCIE_IOTRAP_PROTOCOL              *PchPcieIoTrapProtocol;

  Status = gBS->LocateProtocol (&gPchPcieIoTrapProtocolGuid, NULL, (VOID **) &PchPcieIoTrapProtocol);
  if (EFI_ERROR (Status)) {
    return;
  }
  mPcieIoTrapAddress = PchPcieIoTrapProtocol->PcieTrapAddress;
  DEBUG ((DEBUG_INFO, "PcieIoTrapAddress: %0x\n", mPcieIoTrapAddress));

  if (mPcieIoTrapAddress != 0) {
    //
    // Save PCH PCIE IoTrap address to re-config PCIE power management setting after resume from S3
    //
    Data32 = PchPciePmTrap;
    S3BootScriptSaveIoWrite (
      S3BootScriptWidthUint32,
      (UINTN) (mPcieIoTrapAddress),
      1,
      &Data32
      );
  } else {
    ASSERT (FALSE);
  }
}

/**
  This is the callback function for PCI ENUMERATION COMPLETE.
**/
VOID
EFIAPI
PchOnPciEnumComplete (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS          Status;
  VOID                *ProtocolPointer;

  ///
  /// Check if this is first time called by EfiCreateProtocolNotifyEvent() or not,
  /// if it is, we will skip it until real event is triggered
  ///
  Status = gBS->LocateProtocol (&gEfiPciEnumerationCompleteProtocolGuid, NULL, (VOID **) &ProtocolPointer);
  if (EFI_SUCCESS != Status) {
    return;
  }
  gBS->CloseEvent (Event);

  ReconfigurePciePowerManagementForS3 ();
  ProcessSmiLocks ();
  ConfigureSerialIoAtS3Resume ();
}

/**
  Register callback functions for PCH DXE.
**/
VOID
PchRegisterNotifications (
  VOID
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   ReadyToBoot;
  EFI_EVENT   LegacyBootEvent;
  EFI_EVENT   ExitBootServicesEvent;
  VOID        *Registration;

  ///
  /// Create PCI Enumeration Completed callback for PCH
  ///
  EfiCreateProtocolNotifyEvent (
    &gEfiPciEnumerationCompleteProtocolGuid,
    TPL_CALLBACK,
    PchOnPciEnumComplete,
    NULL,
    &Registration
    );

  //
  // Register a Ready to boot event to config PCIE power management setting after OPROM executed
  //
  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             PchOnReadyToBoot,
             NULL,
             &ReadyToBoot
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Create events for PCH to do the task before ExitBootServices/LegacyBoot.
  // It is guaranteed that only one of two events below will be signalled
  //
  Status = gBS->CreateEvent (
                  EVT_SIGNAL_EXIT_BOOT_SERVICES,
                  TPL_CALLBACK,
                  PchOnExitBootServices,
                  NULL,
                  &ExitBootServicesEvent
                  );
  ASSERT_EFI_ERROR (Status);

  Status = EfiCreateEventLegacyBootEx (
             TPL_CALLBACK,
             PchOnBootToOs,
             NULL,
             &LegacyBootEvent
             );
  ASSERT_EFI_ERROR (Status);
}

/**
  <b>PchInit DXE Module Entry Point</b>\n
  - <b>Introduction</b>\n
      The PchInit module is a DXE driver that initializes the Intel Platform Controller Hub
      following the PCH BIOS specification and EDS requirements and recommendations. It consumes
      the PCH_POLICY_HOB SI_POLICY_HOB for expected configurations per policy.
      This is the standard EFI driver point that detects whether there is an supported PCH in
      the system and if so, initializes the chipset.

  - <b>Details</b>\n
    This module is required for initializing the Intel Platform Controller Hub to
    follow the PCH BIOS specification and EDS.
    This includes some initialization sequences, enabling and disabling PCH devices,
    configuring clock gating, RST PCIe Storage Remapping, SATA controller, ASPM of PCIE devices. Right before end of DXE,
    it's responsible to lock down registers for security requirement.

  - @pre
    - PCH PCR base address configured
    - EFI_PCI_HOST_BRIDGE_RESOURCE_ALLOCATION_PROTOCOL
      - This is to ensure that PCI MMIO and IO resource has been prepared and available for this driver to allocate.

  - @result
    - Publishes the @link _PCH_INFO_PROTOCOL PCH_INFO_PROTOCOL @endlink
    - Publishes the @link _PCH_EMMC_TUNING_PROTOCOL PCH_EMMC_TUNING_PROTOCOL @endlink

  - <b>References</b>\n
    - @link _PCH_POLICY PCH_POLICY_HOB @endlink.
    - @link _SI_POLICY_STRUCT SI_POLICY_HOB @endlink.

  - <b>Integration Checklists</b>\n
    - Verify prerequisites are met. Porting Recommendations.
    - No modification of this module should be necessary
    - Any modification of this module should follow the PCH BIOS Specification and EDS

  @param[in] ImageHandle          Handle for the image of this driver
  @param[in] SystemTable          Pointer to the EFI System Table

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver
**/
EFI_STATUS
EFIAPI
PchInitEntryPointDxe (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;

  DEBUG ((DEBUG_INFO, "PchInitEntryPointDxe() Start\n"));

  mImageHandle = ImageHandle;

  PchInitEntryPointCommon ();

  Status = PchAcpiInit (ImageHandle);

  PchRegisterNotifications ();

  CreateSerialIoUartHiddenHandle ();

  DEBUG ((DEBUG_INFO, "PchInitEntryPointDxe() End\n"));

  return Status;
}

/**
  PCH initialization before ExitBootServices / LegacyBoot events
  Useful for operations which must happen later than at EndOfPost event

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
PchOnBootToOs (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  ///
  /// Closed the event to avoid call twice
  ///
  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  ConfigureSerialIoAtBoot ();

  return;
}

/**
  PCH initialization on ExitBootService. This event is used if only ExitBootService is used
  and not in legacy boot

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.

  @retval None
**/
VOID
EFIAPI
PchOnExitBootServices (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  PchOnBootToOs (NULL, NULL);

  return;
}

/**
  PCH initialization before boot to OS

  @param[in] Event                A pointer to the Event that triggered the callback.
  @param[in] Context              A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
PchOnReadyToBoot (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  DEBUG ((DEBUG_INFO, "Uefi PchOnReadyToBoot() Start\n"));

  if (Event != NULL) {
    gBS->CloseEvent (Event);
  }

  //
  // Trigger an Iotrap SMI to config PCIE power management setting after PCI enumrate is done
  //
  if (mPcieIoTrapAddress != 0) {
    IoWrite32 ((UINTN) mPcieIoTrapAddress, PchPciePmTrap);
  } else {
    ASSERT (FALSE);
  }

  DEBUG ((DEBUG_INFO, "Uefi PchOnReadyToBoot() End\n"));
}

