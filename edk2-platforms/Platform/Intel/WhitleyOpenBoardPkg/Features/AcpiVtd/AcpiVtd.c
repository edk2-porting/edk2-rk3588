/** @file AcpiVtd.c

  @copyright
  Copyright 1996 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

//
// Statements that include other files
//
#include <AcpiVtd.h>
#include <Library/SetupLib.h>
#include <IioRegs.h>
#include <IioSetupDefinitions.h>
#include <Protocol/Tcg2Protocol.h>
#include <Library/TpmMeasurementLib.h>

VTD_SUPPORT_INSTANCE mPrivateData;

#define MAX_BUS_ADDR_WIDTH 45

/**

  Add DMAR entry

  @param This        - DMA Remap protocol pointer
  @param RemapType   - Type of DMA remapping structure to add
  @param RemapEntry  - Entry to add

  @retval EFI_INVALID_PARAMETER - DMA remapping support not initialized or entry is malformed
  @retval EFI_UNSUPPORTED       - Adding entries is not supported
  @retval EFI_SUCCESS           - The entry was inserted successfully.

**/
EFI_STATUS
EFIAPI
InsertDmaRemap (
  IN  DMA_REMAP_PROTOCOL                *This,
  IN  REMAP_TYPE                        RemapType,
  IN  VOID                              *RemapEntry
  )
{
  UINTN                                                       DevIndex;
  EFI_ACPI_DMAR_HEADER                                        *Dmar;
  EFI_ACPI_DMAR_DRHD_HEADER                                   *Drhd;
  EFI_ACPI_DMAR_RMRR_HEADER                                   *Rmrr;
  EFI_ACPI_DMAR_SATC_HEADER                                   *Atsr;
  EFI_ACPI_DMAR_RHSA_HEADER                                   *Rhsa;
  EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER                 *DevScope;
  DMAR_DRHD                                                   *DmaRemap;
  DMAR_RMRR                                                   *RevMemRegion;
  DMAR_ATSR                                                   *AtsrRegion;
  DMAR_RHSA                                                   *RhsaRegion;
  EFI_ACPI_DMAR_PCI_PATH                                      *PciPath;
  EFI_ACPI_DMAR_PCI_PATH                                      *PciInputPath;

  if (mPrivateData.Dmar == NULL) {
      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
  }

  Dmar = mPrivateData.Dmar;
  if (((UINT8 *) Dmar + Dmar->Header.Length) == NULL) {
      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
  }

  if (RemapType == DrhdType) {
    DmaRemap = (DMAR_DRHD *) RemapEntry;
    ASSERT (DmaRemap->Signature == DRHD_SIGNATURE);
    Drhd = (EFI_ACPI_DMAR_DRHD_HEADER *) ((UINT8 *) Dmar + Dmar->Header.Length);
    if (Drhd == NULL) {
      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }

    if (DmaRemap->RegisterBase == 0) {
      return EFI_UNSUPPORTED;
    }

    Drhd->Header.Type         = EFI_ACPI_DMAR_TYPE_DRHD;
    Drhd->Header.Length       = sizeof (EFI_ACPI_DMAR_DRHD_HEADER);
    Drhd->Flags               = DmaRemap->Flags;
    Drhd->SegmentNumber       = DmaRemap->SegmentNumber;
    Drhd->RegisterBaseAddress = DmaRemap->RegisterBase;
    DevScope                  = NULL;

    for (DevIndex = 0; DevIndex < DmaRemap->DeviceScopeNumber; DevIndex++) {
      if (((UINT8 *) Drhd + Drhd->Header.Length) == NULL) {
          ASSERT (FALSE);
          return EFI_INVALID_PARAMETER;
      }

      DevScope = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *) ((UINT8 *) Drhd + Drhd->Header.Length);
      if (DevScope != NULL) {
        DevScope->Type              = DmaRemap->DeviceScope[DevIndex].DeviceType;
        DevScope->Length            = sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER);
        DevScope->EnumerationId     = DmaRemap->DeviceScope[DevIndex].EnumerationID;
        DevScope->StartBusNumber    = DmaRemap->DeviceScope[DevIndex].StartBusNumber;
        if (((UINT8 *) DevScope + DevScope->Length) == NULL) {
          ASSERT (FALSE);
          return EFI_INVALID_PARAMETER;
        }

        PciPath = (EFI_ACPI_DMAR_PCI_PATH *) ((UINT8 *) DevScope + DevScope->Length);
        PciInputPath = (EFI_ACPI_DMAR_PCI_PATH *) DmaRemap->DeviceScope[DevIndex].PciNode;
        while (*(UINT8 *) PciInputPath != (UINT8) -1) {
          CopyMem(PciPath, PciInputPath, sizeof (EFI_ACPI_DMAR_PCI_PATH));
          DevScope->Length += sizeof (EFI_ACPI_DMAR_PCI_PATH);
          PciInputPath++;
          PciPath++;
        }
        Drhd->Header.Length = Drhd->Header.Length + (UINT16) DevScope->Length;
      } else {
        DEBUG ((DEBUG_ERROR, "DevScope Error. Invalid pointer.\n"));
      }
    }

    Dmar->Header.Length += Drhd->Header.Length;

  } else if (RemapType == RmrrType) {
    RevMemRegion = (DMAR_RMRR *) RemapEntry;
    ASSERT (RevMemRegion->Signature == RMRR_SIGNATURE);
    Rmrr = (EFI_ACPI_DMAR_RMRR_HEADER *) ((UINT8 *) Dmar + Dmar->Header.Length);
    if (Rmrr == NULL) {
      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }

    Rmrr->Header.Type                       = EFI_ACPI_DMAR_TYPE_RMRR;
    Rmrr->Header.Length                     = sizeof (EFI_ACPI_DMAR_RMRR_HEADER);
    Rmrr->SegmentNumber                     = RevMemRegion->SegmentNumber;
    Rmrr->ReservedMemoryRegionBaseAddress   = RevMemRegion->RsvdMemBase;
    Rmrr->ReservedMemoryRegionLimitAddress  = RevMemRegion->RsvdMemLimit;

    DevScope = NULL;
    for (DevIndex = 0; DevIndex < RevMemRegion->DeviceScopeNumber; DevIndex++) {
      if (((UINT8 *) Rmrr + Rmrr->Header.Length) == NULL) {
        ASSERT (FALSE);
        return EFI_INVALID_PARAMETER;
      }

      DevScope                        = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *) ((UINT8 *) Rmrr + Rmrr->Header.Length);
      if (DevScope != NULL) {
        DevScope->Type                  = RevMemRegion->DeviceScope[DevIndex].DeviceType;
        DevScope->StartBusNumber        = RevMemRegion->DeviceScope[DevIndex].StartBusNumber;
        DevScope->Length                = sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER);
        if (((UINT8 *) DevScope + DevScope->Length) == NULL) {
          ASSERT (FALSE);
          return EFI_INVALID_PARAMETER;
        }

        PciPath                         = (EFI_ACPI_DMAR_PCI_PATH *) ((UINT8 *) DevScope + DevScope->Length);
        PciInputPath                    = (EFI_ACPI_DMAR_PCI_PATH *) RevMemRegion->DeviceScope[DevIndex].PciNode;
        while (*(UINT8 *) PciInputPath != (UINT8) -1) {
          CopyMem (PciPath, PciInputPath, sizeof (EFI_ACPI_DMAR_PCI_PATH));
          DevScope->Length += sizeof (EFI_ACPI_DMAR_PCI_PATH);
          PciInputPath++;
          PciPath++;
        }
        Rmrr->Header.Length = Rmrr->Header.Length + (UINT16) DevScope->Length;
      } else {
        DEBUG ((DEBUG_ERROR, "DevScope Error. Invalid pointer.\n"));
      }
    }

    Dmar->Header.Length += Rmrr->Header.Length;

  } else if (RemapType == AtsrType) {
    AtsrRegion = (DMAR_ATSR *) RemapEntry;
    ASSERT (AtsrRegion->Signature == ATSR_SIGNATURE);
    Atsr = (EFI_ACPI_DMAR_SATC_HEADER *) ((UINT8 *) Dmar + Dmar->Header.Length);
    if (Atsr == NULL) {
      ASSERT (FALSE);
      return EFI_INVALID_PARAMETER;
    }

    Atsr->Header.Type   = EFI_ACPI_DMAR_TYPE_ATSR;
    Atsr->Flags         = AtsrRegion->Flags;
    Atsr->SegmentNumber = AtsrRegion->SegmentNumber;
    Atsr->Header.Length = sizeof (EFI_ACPI_DMAR_SATC_HEADER);

    DevScope = NULL;
    for (DevIndex = 0; DevIndex < AtsrRegion->DeviceScopeNumber; DevIndex++) {
      if ((AtsrRegion->ATSRPresentBit & (01 << DevIndex)) == 00) {
        continue;
      }
      if (((UINT8 *) Atsr + Atsr->Header.Length) == NULL) {
        ASSERT (FALSE);
        return EFI_INVALID_PARAMETER;
      }

      DevScope = (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER *) ((UINT8 *) Atsr + Atsr->Header.Length);
      if (DevScope != NULL) {
        DevScope->Type              = AtsrRegion->DeviceScope[DevIndex].DeviceType;
        DevScope->StartBusNumber    = AtsrRegion->DeviceScope[DevIndex].StartBusNumber;
        DevScope->Length            = sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER);
        if (((UINT8 *) DevScope + DevScope->Length) == NULL) {
          ASSERT (FALSE);
          return EFI_INVALID_PARAMETER;
        }

        PciPath = (EFI_ACPI_DMAR_PCI_PATH *) ((UINT8 *) DevScope + DevScope->Length);
        PciInputPath = (EFI_ACPI_DMAR_PCI_PATH *) AtsrRegion->DeviceScope[DevIndex].PciNode;
        while (*(UINT8 *) PciInputPath != (UINT8) -1) {
          CopyMem(PciPath, PciInputPath, sizeof (EFI_ACPI_DMAR_PCI_PATH));
          DevScope->Length += sizeof (EFI_ACPI_DMAR_PCI_PATH);
          PciInputPath++;
          PciPath++;
        }

        Atsr->Header.Length = Atsr->Header.Length + (UINT16) DevScope->Length;

      } else {
        DEBUG ((DEBUG_ERROR, "DevScope Error. Invalid pointer.\n"));
      }
    }

    Dmar->Header.Length += Atsr->Header.Length;

  } else if (RemapType == RhsaType) {
    RhsaRegion = (DMAR_RHSA *) RemapEntry;
    ASSERT (RhsaRegion->Signature == RHSA_SIGNATURE);

    Rhsa                        = (EFI_ACPI_DMAR_RHSA_HEADER *) ((UINT8 *) Dmar + Dmar->Header.Length);
    Rhsa->Header.Type           = EFI_ACPI_DMAR_TYPE_RHSA;
    Rhsa->ProximityDomain       = RhsaRegion->Domian;
    Rhsa->RegisterBaseAddress   = RhsaRegion->RegisterBase;
    Rhsa->Header.Length         = sizeof (EFI_ACPI_DMAR_RHSA_HEADER);
    Dmar->Header.Length         += Rhsa->Header.Length;
  } else {
    return EFI_INVALID_PARAMETER;
  }

  ASSERT (Dmar->Header.Length < TABLE_SIZE);
  return EFI_SUCCESS;
}

/**

    Returns info about the provided entry

    @param Entry      - DMA remapping entry
    @param Type       - DMA remapping type
    @param IncludeAll - Include all or all root port ASTR
    @param Length     - GC_TODO: add arg description

    @retval EFI_INVALID_PARAMETER - Null input pointer
    @retval EFI_SUCCESS           - Table info updated

**/
EFI_STATUS
GetTablesInfo (
  IN     UINT8       *Entry,
  IN OUT REMAP_TYPE  *Type,
  IN OUT BOOLEAN     *IncludeAll,
  IN OUT UINTN       *Length
  )
{
  EFI_ACPI_DMAR_DRHD_HEADER  *Comm;

  if (!Entry || !Type || !IncludeAll || !Length) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  Comm    = (EFI_ACPI_DMAR_DRHD_HEADER *) Entry;
  *Length = Comm->Header.Length;

  if (Comm->Header.Type == EFI_ACPI_DMAR_TYPE_RMRR) {
    *Type = RmrrType;
  } else if (Comm->Header.Type == EFI_ACPI_DMAR_TYPE_DRHD) {
    *Type = DrhdType;
  } else if (Comm->Header.Type == EFI_ACPI_DMAR_TYPE_ATSR) {
    *Type = AtsrType;
  } else if (Comm->Header.Type == EFI_ACPI_DMAR_TYPE_RHSA) {
    *Type = RhsaType;
  } else {
    *Type = 0xFF;
  }

  if (Comm->Flags & EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_PCI_ALL) {
    *IncludeAll = TRUE;
  } else {
    *IncludeAll = FALSE;
  }

  return EFI_SUCCESS;
}

/**

    Reorder the table entries

    @param None

    @retval EFI_SUCCESS - The table entries are ordered

**/
EFI_STATUS
ReorderTables (
  VOID
  )
{
  REMAP_TYPE  Type;
  BOOLEAN     IncludeAll;
  UINTN       Length;
  UINTN       CurrLength;
  UINTN       TableLength;
  UINT8       *Ptr;
  UINT8       *PtrOrder;

  Ptr       = (UINT8 *) mPrivateData.Dmar;
  PtrOrder  = (UINT8 *) mPrivateData.DmarOrder;

  CopyMem (PtrOrder, Ptr, sizeof (EFI_ACPI_DMAR_HEADER));
  PtrOrder += sizeof (EFI_ACPI_DMAR_HEADER);

  TableLength = mPrivateData.Dmar->Header.Length;

  CurrLength  = sizeof (EFI_ACPI_DMAR_HEADER);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == DrhdType && !IncludeAll) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  CurrLength  = sizeof (EFI_ACPI_DMAR_HEADER);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == DrhdType && IncludeAll) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  CurrLength  = sizeof (EFI_ACPI_DMAR_HEADER);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == RmrrType && !IncludeAll) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  CurrLength  = sizeof (EFI_ACPI_DMAR_HEADER);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == AtsrType && !IncludeAll) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  CurrLength  = sizeof (EFI_ACPI_DMAR_HEADER);
  Ptr         = (UINT8 *) mPrivateData.Dmar + CurrLength;
  while (CurrLength < TableLength) {
    GetTablesInfo (Ptr, &Type, &IncludeAll, &Length);
    if (Type == RhsaType) {
      CopyMem (PtrOrder, Ptr, Length);
      PtrOrder += Length;
    }

    Ptr += Length;
    CurrLength += Length;
  }

  return EFI_SUCCESS;
}

/**

  Return a reordered version of the DMAR table provided on input

  @param[in]      This      - DMA remap protocol
  @param[in][out] DmarTable - DMAR table

  @retval EFI_INVALID_PARAMETER - DmarTable NULL
  @retval EFI_UNSUPPORTED       - DMAR table length doesn't meet expected value
  @retval EFI_SUCCESS           - Updated DMAR table returned

**/
EFI_STATUS
EFIAPI
GetDmarTable (
  IN     DMA_REMAP_PROTOCOL                *This,
  IN OUT VOID                              **DmarTable
  )
{
  if (DmarTable == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (mPrivateData.Dmar->Header.Length <= sizeof (EFI_ACPI_DMAR_HEADER)) {
    return EFI_UNSUPPORTED;
  }

  ReorderTables ();
  *DmarTable = mPrivateData.DmarOrder;
  return EFI_SUCCESS;
}

#define TBT_SECURITY_EVENT_STRING      "DMA Protection Disabled"
#define TBT_SECURITY_EVENT_STRING_LEN  (sizeof (TBT_SECURITY_EVENT_STRING) - 1)

GLOBAL_REMOVE_IF_UNREFERENCED EFI_EVENT                         EndOfDxeEvent;

/**
  Security EndOfDxe CallBack Function
  If the firmware/BIOS has an option to enable and disable DMA protections via a VT-d switch in BIOS options, then the shipping configuration must be with VT-d protection enabled.
  On every boot where VT-d/DMA protection is disabled, or will be disabled, or configured to a lower security state, and a platform has a TPM enabled, then the platform SHALL
  extend an EV_EFI_ACTION event into PCR[7] before enabling external DMA
  The event string SHALL be "DMA Protection Disabled". The platform firmware MUST log this measurement in the event log using the string "DMA Protection Disabled" for the Event Data.
  Measure and log launch of TBT Security, and extend the measurement result into a specific PCR.
  Extend an EV_EFI_ACTION event into PCR[7] before enabling external DMA. The event string SHALL be "DMA Protection Disabled". The platform firmware MUST log this measurement
  in the event log using the string "DMA Protection Disabled" for the Event Data.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
ExtendPCR7CallBack (
  IN EFI_EVENT          Event,
  IN VOID               *Context
  )
{
  UINTN                 Status;
  UINT64                HashDataLen;

  DEBUG ((DEBUG_INFO, "ExtendPCR7CallBack START\n"));

  //
  // When VT-d/DMA protection is disabled and a platform has a TPM enabled,
  // the platform SHALL extend an EV_EFI_ACTION event into PCR[7].
  //
  HashDataLen = TBT_SECURITY_EVENT_STRING_LEN;

  Status = TpmMeasureAndLogData (
             7,
             EV_EFI_ACTION,
             TBT_SECURITY_EVENT_STRING,
             (UINT32) HashDataLen,
             TBT_SECURITY_EVENT_STRING,
             HashDataLen
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TpmMeasureAndLogData Status: %r\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "TpmMeasureAndLogData Successfully\n"));
  }

  DEBUG ((DEBUG_INFO, "ExtendPCR7CallBack END\n"));
}

/**
  Register an End of DXE event for extended a TPM log to PCR[7] when vtd is diable
  This feature is introduced by TBT Security requirment
**/
VOID
RegisterExtendPCR7CallBack (
  VOID
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  //
  // Register an End of DXE event for extended a TPM log to PCR[7].
  //
  DEBUG ((DEBUG_INFO, "Register an End of DXE event for extended a TPM log to PCR[7] when VTd/DMA protection is disabled.\n"));
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  ExtendPCR7CallBack,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Register an End of DXE event for extended a TPM log to PCR[7], Status: %r\n", Status));
  }
}

/**

  VT-D Driver entry point

  @param ImageHandle     The image handle.
  @param SystemTable     The system table.

  @retval Status - If not EFI_SUCCESS then an error occurred during initialization.

**/
EFI_STATUS
EFIAPI
VtdTableEntryPoint (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
  EFI_STATUS                            Status = EFI_SUCCESS;
  EFI_ACPI_DMAR_HEADER                  *Dmar;
  UINT64                                TempOemTableId;
  UINT8                                 VTdSupport;
  UINT8                                 DmaCtrlOptIn;
  UINT8                                 InterruptRemap;
  UINT8                                 X2ApicOptOut;
  UINT8                                 ATS;
  UINTN                                 Dmarlength;
  UINT8                                 ControlIommu;

  //
  // Initialize our protocol
  //
  ZeroMem (&mPrivateData, sizeof (VTD_SUPPORT_INSTANCE));

  Status = GetOptionData (&gEfiSocketIioVariableGuid, OFFSET_OF (SOCKET_IIO_CONFIGURATION, VTdSupport), &VTdSupport, sizeof (VTdSupport));
  Status |= GetOptionData (&gEfiSocketIioVariableGuid, OFFSET_OF (SOCKET_IIO_CONFIGURATION, DmaCtrlOptIn), &DmaCtrlOptIn, sizeof (DmaCtrlOptIn));
  Status |= GetOptionData (&gEfiSocketIioVariableGuid, OFFSET_OF (SOCKET_IIO_CONFIGURATION, InterruptRemap), &InterruptRemap, sizeof (InterruptRemap));
  Status |= GetOptionData (&gEfiSocketIioVariableGuid, OFFSET_OF (SOCKET_IIO_CONFIGURATION, ATS), &ATS, sizeof (ATS));
  Status |= GetOptionData (&gEfiSocketIioVariableGuid, OFFSET_OF (SOCKET_IIO_CONFIGURATION, X2ApicOptOut), &X2ApicOptOut, sizeof (X2ApicOptOut));

  if (!EFI_ERROR (Status)) {
    mPrivateData.DmaRemapProt.VTdSupport      = VTdSupport;
    mPrivateData.DmaRemapProt.DmaCtrlOptIn    = DmaCtrlOptIn;
    mPrivateData.DmaRemapProt.InterruptRemap  = VTdSupport && (InterruptRemap != IIO_OPTION_DISABLE);
    mPrivateData.DmaRemapProt.ATS             = ATS;
    mPrivateData.DmaRemapProt.X2ApicOptOut    = X2ApicOptOut;
  }

  Status = GetOptionData (&gEfiSocketIioVariableGuid, OFFSET_OF (SOCKET_IIO_CONFIGURATION, ControlIommu), &ControlIommu, sizeof (ControlIommu));
  if (EFI_ERROR (Status)) {
    ControlIommu = 0;
  }

  mPrivateData.Signature = EFI_ACPI_6_4_DMA_REMAPPING_TABLE_SIGNATURE;

  Dmarlength = MAX_SOCKET * NUMBER_PORTS_PER_SOCKET * ( sizeof (EFI_ACPI_DMAR_HEADER) + sizeof (EFI_ACPI_DMAR_DRHD_HEADER) +
                    sizeof (EFI_ACPI_DMAR_DEVICE_SCOPE_STRUCTURE_HEADER) + sizeof (EFI_ACPI_DMAR_PCI_PATH));

  mPrivateData.Dmar = (EFI_ACPI_DMAR_HEADER *) AllocateZeroPool (Dmarlength);

  mPrivateData.DmarOrder = (EFI_ACPI_DMAR_HEADER *) AllocateZeroPool (Dmarlength);

  mPrivateData.DmaRemapProt.InsertDmaRemap = InsertDmaRemap;
  mPrivateData.DmaRemapProt.GetDmarTable = GetDmarTable;

  if (mPrivateData.Dmar != NULL) {
    Dmar                          = mPrivateData.Dmar;
    Dmar->Header.Length           = sizeof (EFI_ACPI_DMAR_HEADER);
    Dmar->Header.Signature        = EFI_ACPI_6_4_DMA_REMAPPING_TABLE_SIGNATURE;
    Dmar->Header.Revision         = EFI_ACPI_DMAR_REVISION;
    Dmar->Header.OemRevision      = ACPI_DMAR_OEM_REVISION;
    Dmar->Header.CreatorId        = ACPI_DMAR_OEM_CREATOR_ID;
    Dmar->Header.CreatorRevision  = ACPI_DMAR_OEM_CREATOR_REVISION;
    Dmar->HostAddressWidth        = MAX_BUS_ADDR_WIDTH;

    TempOemTableId = PcdGet64 (PcdAcpiDefaultOemTableId);

    CopyMem (Dmar->Header.OemId, PcdGetPtr (PcdAcpiDefaultOemId), sizeof (Dmar->Header.OemId));
    CopyMem (&Dmar->Header.OemTableId, &TempOemTableId, sizeof (Dmar->Header.OemTableId));

    Status = gBS->InstallProtocolInterface (
                    &mPrivateData.Handle,
                    &gDmaRemapProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mPrivateData.DmaRemapProt
                    );
  } else {

    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    Status = EFI_OUT_OF_RESOURCES;
  }

  if (FixedPcdGetBool (PcdConditionallyExtendPcr7)) {
    if (!VTdSupport || !DmaCtrlOptIn || (ControlIommu == 0)) {
      //
      // Inform OS by TPM PCR7 when VTd/DMA protection is disabled.
      //
      RegisterExtendPCR7CallBack ();
    }
  }

  return Status;
}
