/** @file
  This is the driver that initializes the Intel System Agent.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "SaInitDxe.h"
#include "SaInit.h"
#include <SaConfigHob.h>
#include <Protocol/SaNvsArea.h>
#include <Library/PchInfoLib.h>
#include <CpuPcieInfo.h>
#include <Library/DxeCpuPcieRpLib.h>
#include <SaConfigHob.h>
#include <CpuPcieHob.h>
#include <HostBridgeDataHob.h>
#include <CpuDataStruct.h>

///
/// Global Variables
///
GLOBAL_REMOVE_IF_UNREFERENCED SYSTEM_AGENT_NVS_AREA_PROTOCOL  mSaNvsAreaProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED SA_POLICY_PROTOCOL              *mSaPolicy;
extern SA_CONFIG_HOB                                          *mSaConfigHob;

/**
  A protocol callback which updates 64bits MMIO Base and Length in SA GNVS area
**/
VOID
UpdateSaGnvsForMmioResourceBaseLength (
  VOID
  )
{
  EFI_PHYSICAL_ADDRESS      PciBaseAddress;
  UINT32                    Tolud;
  UINT64                    Length;
  UINT64                    McD0BaseAddress;
  UINTN                     ResMemLimit1;
  UINT8                     EnableAbove4GBMmioBiosAssignemnt;
  HOST_BRIDGE_DATA_HOB      *HostBridgeDataHob;

  PciBaseAddress = 0;
  Tolud = 0;
  Length = 0;
  ResMemLimit1 = 0;
  EnableAbove4GBMmioBiosAssignemnt = 0;
  HostBridgeDataHob = NULL;
  //
  // Read memory map registers
  //
  McD0BaseAddress        = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 0, 0, 0);
  Tolud                  = PciSegmentRead32 (McD0BaseAddress + R_SA_TOLUD) & B_SA_TOLUD_TOLUD_MASK;
  PciBaseAddress         = Tolud;

  ResMemLimit1 = (UINTN) PcdGet64 (PcdSiPciExpressBaseAddress);

  Length = ResMemLimit1 - PciBaseAddress;

  //
  // Get HostBridgeData HOB and see if above 4GB MMIO BIOS assignment enabled
  //
  HostBridgeDataHob = (HOST_BRIDGE_DATA_HOB *) GetFirstGuidHob (&gHostBridgeDataHobGuid);
  if ((HostBridgeDataHob != NULL) && (HostBridgeDataHob->EnableAbove4GBMmio == 1)) {
    EnableAbove4GBMmioBiosAssignemnt = 1;
  }

  //
  // Enable Above 4GB MMIO when Aperture Size is 2GB or higher
  //
  if ((mSaConfigHob != NULL) && (mSaConfigHob->ApertureSize >= 15)) {
    EnableAbove4GBMmioBiosAssignemnt = 1;
  }

  //
  // Check Enable Above 4GB MMIO or not
  //

  DEBUG ((DEBUG_INFO, "Update SA GNVS Area.\n"));
  mSaNvsAreaProtocol.Area->Mmio32Base   = (UINT32) PciBaseAddress;
  mSaNvsAreaProtocol.Area->Mmio32Length = (UINT32) Length;
  if (EnableAbove4GBMmioBiosAssignemnt == 1) {
    mSaNvsAreaProtocol.Area->Mmio64Base   = BASE_256GB;
    mSaNvsAreaProtocol.Area->Mmio64Length = SIZE_256GB;
  }
  DEBUG ((DEBUG_INFO, "SaNvsAreaProtocol.Area->Mmio64Base = %lx\n", mSaNvsAreaProtocol.Area->Mmio64Base));
  DEBUG ((DEBUG_INFO, "SaNvsAreaProtocol.Area->Mmio64Length = %lx\n", mSaNvsAreaProtocol.Area->Mmio64Length));
  DEBUG ((DEBUG_INFO, "SaNvsAreaProtocol.Area->Mmio32Base = %lx\n", mSaNvsAreaProtocol.Area->Mmio32Base));
  DEBUG ((DEBUG_INFO, "SaNvsAreaProtocol.Area->Mmio32Length = %lx\n", mSaNvsAreaProtocol.Area->Mmio32Length));
}

/**
  Install SSDT Table

  @retval EFI_SUCCESS - SSDT Table load successful.
**/
EFI_STATUS
InstallSsdtAcpiTable (
  IN GUID   SsdtTableGuid,
  IN UINT64 Signature
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    *HandleBuffer;
  BOOLEAN                       LoadTable;
  UINTN                         NumberOfHandles;
  UINTN                         Index;
  INTN                          Instance;
  UINTN                         Size;
  UINT32                        FvStatus;
  UINTN                         TableHandle;
  EFI_FV_FILETYPE               FileType;
  EFI_FV_FILE_ATTRIBUTES        Attributes;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  EFI_ACPI_COMMON_HEADER        *Table;

  FwVol         = NULL;
  Table         = NULL;

  DEBUG ((DEBUG_INFO, "Loading SSDT Table GUID: %g\n", SsdtTableGuid));

  ///
  /// Locate FV protocol.
  ///
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Look for FV with ACPI storage file
  ///
  for (Index = 0; Index < NumberOfHandles; Index++) {
    ///
    /// Get the protocol on this handle
    /// This should not fail because of LocateHandleBuffer
    ///
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &FwVol
                    );
    ASSERT_EFI_ERROR (Status);
    if (FwVol == NULL) {
      return EFI_NOT_FOUND;
    }
    ///
    /// See if it has the ACPI storage file
    ///
    Size      = 0;
    FvStatus  = 0;
    Status = FwVol->ReadFile (
                      FwVol,
                      &SsdtTableGuid,
                      NULL,
                      &Size,
                      &FileType,
                      &Attributes,
                      &FvStatus
                      );

    ///
    /// If we found it, then we are done
    ///
    if (!EFI_ERROR (Status)) {
      break;
    }
  }
  ///
  /// Our exit status is determined by the success of the previous operations
  /// If the protocol was found, Instance already points to it.
  ///
  ///
  /// Free any allocated buffers
  ///
  FreePool (HandleBuffer);

  ///
  /// Sanity check that we found our data file
  ///
  ASSERT (FwVol);

  ///
  /// Locate ACPI tables
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &AcpiTable);

  ///
  /// Read tables from the storage file.
  ///
  if (FwVol == NULL) {
    ASSERT_EFI_ERROR (EFI_NOT_FOUND);
    return EFI_NOT_FOUND;
  }
  Instance = 0;

  while (Status == EFI_SUCCESS) {
    ///
    /// Read the ACPI tables
    ///
    Status = FwVol->ReadSection (
                      FwVol,
                      &SsdtTableGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &Table,
                      &Size,
                      &FvStatus
                      );
    if (!EFI_ERROR (Status)) {
      ///
      /// check and load HybridGraphics SSDT table
      ///
      LoadTable   = FALSE;
      TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) Table;

      if (((EFI_ACPI_DESCRIPTION_HEADER *) TableHeader)->OemTableId == Signature) {
        ///
        /// This is the SSDT table that match the Signature
        ///
        DEBUG ((DEBUG_INFO, "Found out SSDT Table GUID: %g\n", SsdtTableGuid));
        LoadTable = TRUE;
      }

      ///
      /// Add the table
      ///
      if (LoadTable) {
        TableHandle = 0;
        Status = AcpiTable->InstallAcpiTable (
                              AcpiTable,
                              TableHeader,
                              TableHeader->Length,
                              &TableHandle
                              );
      }
      ///
      /// Increment the instance
      ///
      Instance++;
      Table = NULL;
    }
  }

  return EFI_SUCCESS;
}

/**
  This function gets registered as a callback to perform Dmar Igd

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
SaAcpiEndOfDxeCallback (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  )
{
  EFI_STATUS          Status;

  if (PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, IGD_BUS_NUM, IGD_DEV_NUM, IGD_FUN_NUM, PCI_VENDOR_ID_OFFSET)) != 0xFFFF) {
    Status = PostPmInitEndOfDxe ();
    if (EFI_SUCCESS != Status) {
      DEBUG ((DEBUG_WARN, "[SA] EndOfDxe GraphicsInit Error, Status = %r \n", Status));
      ASSERT_EFI_ERROR (Status);
    }
  }

  if (PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, IGD_BUS_NUM, IGD_DEV_NUM, IGD_FUN_NUM, PCI_VENDOR_ID_OFFSET)) != 0xFFFF) {
    Status = GetVBiosVbtEndOfDxe ();
    if (EFI_SUCCESS != Status) {
      DEBUG ((DEBUG_WARN, "[SA] EndOfDxe Op Region Error, Status = %r \n", Status));
    }

    Status = UpdateIgdOpRegionEndOfDxe ();
    if (EFI_SUCCESS != Status) {
      DEBUG ((DEBUG_WARN, "[SA] EndOfDxe Update Op Region Error, Status = %r \n", Status));
    }
  }

  return;
}

/**
  SystemAgent Acpi Initialization.

  @param[in] ImageHandle             Handle for the image of this driver

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_OUT_OF_RESOURCES    No enough buffer to allocate
**/
EFI_STATUS
EFIAPI
SaAcpiInit (
  IN EFI_HANDLE         ImageHandle
  )
{
  EFI_STATUS                Status;
  EFI_CPUID_REGISTER        CpuidRegs;
  EFI_EVENT                 EndOfDxeEvent;

  CPU_PCIE_HOB              *CpuPcieHob;

  AsmCpuid (1, &CpuidRegs.RegEax, 0, 0, 0);
  ///
  /// Get the platform setup policy.
  ///
  Status = gBS->LocateProtocol (&gSaPolicyProtocolGuid, NULL, (VOID **) &mSaPolicy);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Install System Agent Global NVS protocol
  ///
  DEBUG ((DEBUG_INFO, "Install SA GNVS protocol\n"));
  Status = (gBS->AllocatePool) (EfiACPIMemoryNVS, sizeof (SYSTEM_AGENT_NVS_AREA), (VOID **) &mSaNvsAreaProtocol.Area);
  ASSERT_EFI_ERROR (Status);
  ZeroMem ((VOID *) mSaNvsAreaProtocol.Area, sizeof (SYSTEM_AGENT_NVS_AREA));
  mSaNvsAreaProtocol.Area->XPcieCfgBaseAddress  = (UINT32) (PcdGet64 (PcdSiPciExpressBaseAddress));
  mSaNvsAreaProtocol.Area->CpuIdInfo            = CpuidRegs.RegEax;

  ///
  /// Get CpuPcieHob HOB
  ///
  CpuPcieHob = NULL;
  CpuPcieHob = (CPU_PCIE_HOB *) GetFirstGuidHob (&gCpuPcieHobGuid);
  if (CpuPcieHob == NULL) {
    DEBUG((DEBUG_ERROR, "CpuPcieHob not found\n"));
    // @todo: Will add it back once it will get add into NVS library since currently it is failing for JSL
    //ASSERT(CpuPcieHob != NULL);
    //return EFI_NOT_FOUND;
  } else {
    mSaNvsAreaProtocol.Area->SlotSelection = CpuPcieHob->SlotSelection;
    DEBUG((DEBUG_INFO, "RpEnabledMask == %x\n", CpuPcieHob->RpEnabledMask));
    if (CpuPcieHob->RpEnabledMask == 0) {
      DEBUG ((DEBUG_ERROR, "All CPU PCIe root ports are disabled!!\n"));
    } else {
      if (CpuPcieHob->RpEnabledMask & BIT0) {
        mSaNvsAreaProtocol.Area->CpuPcieRp0Enable = 1;
      }
      if (CpuPcieHob->RpEnabledMask & BIT1) {
        mSaNvsAreaProtocol.Area->CpuPcieRp1Enable = 1;
      }
      if (CpuPcieHob->RpEnabledMask & BIT2) {
        mSaNvsAreaProtocol.Area->CpuPcieRp2Enable = 1;
      }
      if (CpuPcieHob->RpEnabledMask & BIT3) {
        mSaNvsAreaProtocol.Area->CpuPcieRp3Enable = 1;
      }
    }
    mSaNvsAreaProtocol.Area->MaxPegPortNumber = GetMaxCpuPciePortNum ();
  }

  mSaNvsAreaProtocol.Area->SimicsEnvironment = 0;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gSaNvsAreaProtocolGuid,
                  &mSaNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// GtPostInit Initialization
  ///
  DEBUG ((DEBUG_INFO, "Initializing GT ACPI tables\n"));
  GraphicsInit (ImageHandle, mSaPolicy);

  ///
  /// Audio (dHDA) Initialization
  ///
  ///
  /// Vtd Initialization
  ///
  DEBUG ((DEBUG_INFO, "Initializing VT-d ACPI tables\n"));
  VtdInit (mSaPolicy);

  ///
  /// IgdOpRegion Install Initialization
  ///
  DEBUG ((DEBUG_INFO, "Initializing IGD OpRegion\n"));
  IgdOpRegionInit ();

  ///
  /// Register an end of DXE event for SA ACPI to do tasks before invoking any UEFI drivers,
  /// applications, or connecting consoles,...
  ///
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  SaAcpiEndOfDxeCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );

  ///
  /// Install System Agent Global NVS ACPI table
  ///
  Status = InstallSsdtAcpiTable (gSaSsdtAcpiTableStorageGuid, SIGNATURE_64 ('S', 'a', 'S', 's', 'd', 't', ' ', 0));
  ASSERT_EFI_ERROR (Status);

  ///
  /// Update CPU PCIE RP NVS AREA
  ///
  UpdateCpuPcieNVS();
  ///
  /// Install Intel Graphics SSDT
  ///
  Status = InstallSsdtAcpiTable (gGraphicsAcpiTableStorageGuid, SIGNATURE_64 ('I','g','f','x','S','s','d','t'));
  ASSERT_EFI_ERROR (Status);

  ///
  /// Install IPU SSDT if IPU is present.
  ///
  if (PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, IPU_BUS_NUM, IPU_DEV_NUM, IPU_FUN_NUM, 0)) != V_SA_DEVICE_ID_INVALID) {
      Status = InstallSsdtAcpiTable (gIpuAcpiTableStorageGuid, SIGNATURE_64 ('I','p','u','S','s','d','t',0));
      ASSERT_EFI_ERROR (Status);

  }

  return EFI_SUCCESS;
}

