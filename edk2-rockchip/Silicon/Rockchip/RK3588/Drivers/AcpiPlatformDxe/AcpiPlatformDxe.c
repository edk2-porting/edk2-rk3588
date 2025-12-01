/** @file
 *
 *  ACPI platform driver
 *
 *  Copyright (c) 2020, Jeremy Linton
 *  Copyright (c) 2024, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <IndustryStandard/AcpiAml.h>
#include <Protocol/ExitBootServicesOsNotify.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/NonDiscoverableDevice.h>
#include <Library/AcpiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <AcpiTables.h>
#include <VarStoreData.h>

STATIC CONST EFI_GUID  mAcpiTableFile = {
  0x7E374E25, 0x8E01, 0x4FEE, { 0x87, 0xf2, 0x39, 0x0C, 0x23, 0xC6, 0x06, 0xCD }
};

STATIC EFI_ACPI_SDT_PROTOCOL        *mAcpiSdtProtocol;
STATIC EFI_ACPI_DESCRIPTION_HEADER  *mDsdtTable;

STATIC BOOLEAN  mIsSdmmcBoot = FALSE;

#define SDT_PATTERN_LEN  (AML_NAME_SEG_SIZE + 1)

//
// Simple NameOp integer patcher.
// Does not allocate memory and can be safely used at ExitBootServices.
//
STATIC
EFI_STATUS
AcpiUpdateSdtNameInteger (
  IN  EFI_ACPI_DESCRIPTION_HEADER  *AcpiTable,
  IN  CHAR8                        Name[AML_NAME_SEG_SIZE],
  IN  UINTN                        Value
  )
{
  UINTN   Index;
  CHAR8   Pattern[SDT_PATTERN_LEN];
  UINT8   *SdtPtr;
  UINT32  DataSize;
  UINT32  ValueOffset;

  if (AcpiTable->Length <= SDT_PATTERN_LEN) {
    return EFI_INVALID_PARAMETER;
  }

  SdtPtr = (UINT8 *)AcpiTable;
  //
  // Do a single NameOp variable replacement. These are of the
  // form "08 XXXX SIZE VAL", where SIZE is: 0A=byte, 0B=word, 0C=dword,
  // XXXX is the name and VAL is the value.
  //
  Pattern[0] = AML_NAME_OP;
  CopyMem (Pattern + 1, Name, AML_NAME_SEG_SIZE);

  ValueOffset = SDT_PATTERN_LEN + 1;

  for (Index = 0; Index < (AcpiTable->Length - SDT_PATTERN_LEN); Index++) {
    if (CompareMem (SdtPtr + Index, Pattern, SDT_PATTERN_LEN) == 0) {
      switch (SdtPtr[Index + SDT_PATTERN_LEN]) {
        case AML_QWORD_PREFIX:
          DataSize = sizeof (UINT64);
          break;
        case AML_DWORD_PREFIX:
          DataSize = sizeof (UINT32);
          break;
        case AML_WORD_PREFIX:
          DataSize = sizeof (UINT16);
          break;
        case AML_ONE_OP:
        case AML_ZERO_OP:
          ValueOffset--;
        // Fallthrough
        case AML_BYTE_PREFIX:
          DataSize = sizeof (UINT8);
          break;
        default:
          return EFI_UNSUPPORTED;
      }

      CopyMem (SdtPtr + Index + ValueOffset, &Value, DataSize);
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

STATIC
VOID
EFIAPI
AcpiDsdtFixupStatus (
  IN EFI_ACPI_SDT_PROTOCOL  *AcpiSdtProtocol,
  IN EFI_ACPI_HANDLE        TableHandle
  )
{
  EFI_STATUS  Status;
  UINTN       Index;

  struct {
    CHAR8      *ObjectPath;
    BOOLEAN    Enabled;
  } DevStatus[] = {
    { "\\_SB.PCI0._STA", PcdGet32 (PcdPcie30State) == PCIE30_STATE_ENABLED  },
    { "\\_SB.PCI1._STA", PcdGet32 (PcdPcie30State) == PCIE30_STATE_ENABLED &&
      FixedPcdGetBool (PcdPcie30x2Supported) &&
      PcdGet8 (PcdPcie30PhyMode) != PCIE30_PHY_MODE_AGGREGATION },
    { "\\_SB.PCI2._STA", PcdGet32 (PcdComboPhy1Mode) == COMBO_PHY_MODE_PCIE },
    { "\\_SB.PCI3._STA", PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_PCIE },
    { "\\_SB.PCI4._STA", PcdGet32 (PcdComboPhy0Mode) == COMBO_PHY_MODE_PCIE },
    { "\\_SB.ATA0._STA", PcdGet32 (PcdComboPhy0Mode) == COMBO_PHY_MODE_SATA },
    { "\\_SB.ATA1._STA", PcdGet32 (PcdComboPhy1Mode) == COMBO_PHY_MODE_SATA },
    { "\\_SB.ATA2._STA", PcdGet32 (PcdComboPhy2Mode) == COMBO_PHY_MODE_SATA },
  };

  for (Index = 0; Index < ARRAY_SIZE (DevStatus); Index++) {
    if (DevStatus[Index].Enabled == FALSE) {
      Status = AcpiAmlObjectUpdateInteger (
                 AcpiSdtProtocol,
                 TableHandle,
                 DevStatus[Index].ObjectPath,
                 0x0
                 );
      if (EFI_ERROR (Status)) {
        DEBUG ((
          DEBUG_ERROR,
          "AcpiPlatform: Failed to patch %a. Status=%r\n",
          DevStatus[Index].ObjectPath,
          Status
          ));
      }
    }
  }
}

STATIC
VOID
EFIAPI
NotifyEndOfDxeEvent (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS       Status;
  UINTN            TableKey;
  UINTN            TableIndex;
  EFI_ACPI_HANDLE  TableHandle;

  Status = gBS->LocateProtocol (
                  &gEfiAcpiSdtProtocolGuid,
                  NULL,
                  (VOID **)&mAcpiSdtProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AcpiPlatform: Couldn't locate gEfiAcpiSdtProtocolGuid! Status=%r\n", Status));
    return;
  }

  Status = LocateAndInstallAcpiFromFvConditional (&mAcpiTableFile, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_WARN,
      "AcpiPlatform: Failed to install firmware ACPI as config table. Status=%r\n",
      Status
      ));
  }

  TableIndex = 0;
  Status     = AcpiLocateTableBySignature (
                 mAcpiSdtProtocol,
                 EFI_ACPI_6_3_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE,
                 &TableIndex,
                 &mDsdtTable,
                 &TableKey
                 );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AcpiPlatform: Couldn't locate ACPI DSDT table!\n", __func__));
    return;
  }

  Status = mAcpiSdtProtocol->OpenSdt (TableKey, &TableHandle);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "AcpiPlatform: Couldn't open ACPI DSDT table!\n", __func__));
    mAcpiSdtProtocol->Close (TableHandle);
    return;
  }

  AcpiDsdtFixupStatus (mAcpiSdtProtocol, TableHandle);

  mAcpiSdtProtocol->Close (TableHandle);
}

STATIC
EFI_STATUS
EFIAPI
AcpiFixupPcieEcam (
  IN EXIT_BOOT_SERVICES_OS_TYPE  OsType
  )
{
  EFI_STATUS                   Status;
  UINTN                        Index;
  RK3588_MCFG_TABLE            *McfgTable;
  EFI_ACPI_DESCRIPTION_HEADER  *FadtTable;
  UINTN                        TableKey;
  UINT32                       PcieEcamMode;
  UINT8                        PcieBusMin;
  UINT8                        PcieBusMax;
  BOOLEAN                      PcieBusOffset;
  BOOLEAN                      McfgDeviceFiltering;
  BOOLEAN                      McfgSplitConfigSpaces;

  Index  = 0;
  Status = AcpiLocateTableBySignature (
             mAcpiSdtProtocol,
             EFI_ACPI_6_4_PCI_EXPRESS_MEMORY_MAPPED_CONFIGURATION_SPACE_BASE_ADDRESS_DESCRIPTION_TABLE_SIGNATURE,
             &Index,
             (EFI_ACPI_DESCRIPTION_HEADER **)&McfgTable,
             &TableKey
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "AcpiPlatform: Couldn't locate ACPI MCFG table! Status=%r\n",
      Status
      ));
    return Status;
  }

  PcieEcamMode = PcdGet32 (PcdAcpiPcieEcamCompatMode);

  if (PcieEcamMode == ACPI_PCIE_ECAM_COMPAT_MODE_AUTO) {
    switch (OsType) {
      case ExitBootServicesOsWindows:
        PcieEcamMode = ACPI_PCIE_ECAM_COMPAT_MODE_NXPMX6;
        break;
      case ExitBootServicesOsLinux:
        PcieEcamMode = ACPI_PCIE_ECAM_COMPAT_MODE_GRAVITON;
        break;
      default:
        PcieEcamMode = ACPI_PCIE_ECAM_COMPAT_MODE_SINGLE_DEV;
        break;
    }
  }

  switch (PcieEcamMode) {
    case ACPI_PCIE_ECAM_COMPAT_MODE_NXPMX6:
      //
      // This workaround ought to filter duplicate devices on the primary
      // and secondary buses, but it appears that recent Windows versions
      // only do so for bus numbers 0 and 1.
      // Not all segments start at bus 0 anymore, since they need to be made
      // distinct to the SMMU & ITS (see Rk3588Pcie.h). However, these features
      // can't be enabled in Windows anyway, so we'll disable the bus offset and
      // let it reassign bus numbers from 0.
      // It's possible to enable PcieBusOffset by disabling McfgDeviceFiltering,
      // but this will hide the root port and may cause the system to lock-up
      // with certain endpoints that have broken CFG0 filtering.
      //
      PcieBusOffset         = FALSE;
      McfgDeviceFiltering   = !PcieBusOffset;
      McfgSplitConfigSpaces = TRUE;

      Index  = 0;
      Status = AcpiLocateTableBySignature (
                 mAcpiSdtProtocol,
                 EFI_ACPI_6_3_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE,
                 &Index,
                 &FadtTable,
                 &TableKey
                 );
      if (EFI_ERROR (Status)) {
        DEBUG ((
          DEBUG_ERROR,
          "AcpiPlatform: Couldn't locate ACPI FADT table! Status=%r\n",
          Status
          ));
        return Status;
      }

      CopyMem (FadtTable->OemId, "NXPMX6", sizeof (FadtTable->OemId));
      AcpiUpdateChecksum ((UINT8 *)FadtTable, FadtTable->Length);
      break;

    case ACPI_PCIE_ECAM_COMPAT_MODE_GRAVITON:
      PcieBusOffset         = TRUE;
      McfgDeviceFiltering   = TRUE;
      McfgSplitConfigSpaces = FALSE;

      CopyMem (McfgTable->Header.Header.OemId, "AMAZON", sizeof (McfgTable->Header.Header.OemId));
      McfgTable->Header.Header.OemTableId = SIGNATURE_64 ('G', 'R', 'A', 'V', 'I', 'T', 'O', 'N');
      break;

    default: // ACPI_PCIE_ECAM_COMPAT_MODE_SINGLE_DEV
      PcieBusOffset         = TRUE;
      McfgDeviceFiltering   = FALSE;
      McfgSplitConfigSpaces = FALSE;
      break;
  }

  PcieBusMin = 0;
  PcieBusMax = PCIE_BUS_COUNT - 1;

  if (!McfgDeviceFiltering) {
    //
    // Skip the primary bus as it only spans a single function (root port)
    // and the remaining DBI region has to be filtered out.
    //
    PcieBusMin = 1;

    if (!McfgSplitConfigSpaces) {
      //
      // It's not possible to have more than a single bus due to the
      // the device offset workaround below.
      //
      PcieBusMax = PcieBusMin;
    }
  }

  for (Index = 0; Index < NUM_PCIE_CONTROLLER; Index++) {
    McfgTable->ConfigSpaces[0][Index].PciSegmentGroupNumber = Index;
    McfgTable->ConfigSpaces[0][Index].BaseAddress           = PCIE_CFG_BASE (Index) + PCIE_BUS_BASE_OFFSET (Index);
    McfgTable->ConfigSpaces[0][Index].StartBusNumber        = PcieBusMin;
    McfgTable->ConfigSpaces[0][Index].EndBusNumber          = PcieBusMax;

    if (PcieBusOffset) {
      McfgTable->ConfigSpaces[0][Index].BaseAddress    -= PCIE_BUS_BASE_OFFSET (Index);
      McfgTable->ConfigSpaces[0][Index].StartBusNumber += PCIE_BUS_BASE (Index);
      McfgTable->ConfigSpaces[0][Index].EndBusNumber   += PCIE_BUS_BASE (Index);
    }

    if (McfgSplitConfigSpaces) {
      McfgTable->ConfigSpaces[1][Index].PciSegmentGroupNumber = McfgTable->ConfigSpaces[0][Index].PciSegmentGroupNumber;
      McfgTable->ConfigSpaces[1][Index].StartBusNumber        = McfgTable->ConfigSpaces[0][Index].StartBusNumber;

      if (McfgDeviceFiltering) {
        //
        // The OS is expected to filter devices on the primary and secondary
        // buses, so we can expose the root port in this entry (primary bus)
        // and remaining buses in the first entry.
        //
        McfgTable->ConfigSpaces[1][Index].BaseAddress  = PCIE_DBI_BASE (Index);
        McfgTable->ConfigSpaces[1][Index].EndBusNumber = McfgTable->ConfigSpaces[1][Index].StartBusNumber;

        McfgTable->ConfigSpaces[0][Index].StartBusNumber += 1;
      } else {
        //
        // The OS will not filter devices on any bus, so we can't expose
        // the root port. Use this entry for buses following secondary and
        // limit the first entry to the secondary bus only, so the device
        // offset workaround below can work.
        //
        McfgTable->ConfigSpaces[1][Index].BaseAddress     = McfgTable->ConfigSpaces[0][Index].BaseAddress;
        McfgTable->ConfigSpaces[1][Index].StartBusNumber += 1;
        McfgTable->ConfigSpaces[1][Index].EndBusNumber    = McfgTable->ConfigSpaces[0][Index].EndBusNumber;

        McfgTable->ConfigSpaces[0][Index].EndBusNumber = McfgTable->ConfigSpaces[0][Index].StartBusNumber;
      }
    }

    if (!McfgDeviceFiltering &&
        ((PcdGet32 (PcdPcieEcamCompliantSegmentsMask) & (1 << Index)) == 0))
    {
      McfgTable->ConfigSpaces[0][Index].BaseAddress += 0x8000;
    }
  }

  if (!McfgSplitConfigSpaces) {
    McfgTable->Header.Header.Length = OFFSET_OF (RK3588_MCFG_TABLE, ConfigSpaces[1]);
  }

  AcpiUpdateChecksum ((UINT8 *)McfgTable, McfgTable->Header.Header.Length);

  AcpiUpdateSdtNameInteger (mDsdtTable, "PBMI", PcieBusMin);
  AcpiUpdateSdtNameInteger (mDsdtTable, "PBMA", PcieBusMax);
  AcpiUpdateSdtNameInteger (mDsdtTable, "PBOF", PcieBusOffset);

  return EFI_SUCCESS;
}

STATIC
VOID
EFIAPI
AcpiPlatformExitBootServicesOsHandler (
  IN EXIT_BOOT_SERVICES_OS_CONTEXT  *Context
  )
{
  EXIT_BOOT_SERVICES_OS_TYPE  OsType = Context->OsType;

  if ((mAcpiSdtProtocol == NULL) || (mDsdtTable == NULL)) {
    return;
  }

  //
  // Hide EHCI PNP ID for Windows to avoid binding to the inbox driver,
  // which by default uses atomics on uncached memory and would crash
  // the system.
  //
  if (OsType == ExitBootServicesOsWindows) {
    AcpiUpdateSdtNameInteger (mDsdtTable, "EHID", 0);
  }

  //
  // If the boot device is SDMMC, mark the slot as non-removable.
  // This allows Windows to create a page file on it.
  //
  if (mIsSdmmcBoot) {
    AcpiUpdateSdtNameInteger (mDsdtTable, "SDRM", 0);
  }

  AcpiFixupPcieEcam (OsType);

  AcpiUpdateChecksum ((UINT8 *)mDsdtTable, mDsdtTable->Length);
}

STATIC
BOOLEAN
IsDeviceSdmmc (
  IN EFI_HANDLE  Handle
  )
{
  EFI_STATUS                         Status;
  EFI_DEVICE_PATH_PROTOCOL           *DevicePath;
  EFI_HANDLE                         DeviceHandle;
  NON_DISCOVERABLE_DEVICE            *Device;
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  *Descriptor;

  DevicePath = DevicePathFromHandle (Handle);

  if (  (DevicePath == NULL)
     || (DevicePath->Type != HARDWARE_DEVICE_PATH)
     || (DevicePath->SubType != HW_VENDOR_DP))
  {
    return FALSE;
  }

  Status = gBS->LocateDevicePath (
                  &gEdkiiNonDiscoverableDeviceProtocolGuid,
                  &DevicePath,
                  &DeviceHandle
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Status = gBS->HandleProtocol (
                  DeviceHandle,
                  &gEdkiiNonDiscoverableDeviceProtocolGuid,
                  (VOID **)&Device
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  Descriptor = &Device->Resources[0];

  if ((Descriptor->Desc != ACPI_ADDRESS_SPACE_DESCRIPTOR) ||
      (Descriptor->ResType != ACPI_ADDRESS_SPACE_TYPE_MEM))
  {
    return FALSE;
  }

  return Descriptor->AddrRangeMin == PcdGet32 (PcdRkSdmmcBaseAddress);
}

STATIC  VOID  *mLoadedImageEventRegistration;

STATIC
VOID
EFIAPI
NotifyLoadedImage (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                 Status;
  EFI_HANDLE                 *Handles;
  UINTN                      HandleCount;
  EFI_LOADED_IMAGE_PROTOCOL  *LoadedImage;

  while (TRUE) {
    Status = gBS->LocateHandleBuffer (
                    ByRegisterNotify,
                    NULL,
                    mLoadedImageEventRegistration,
                    &HandleCount,
                    &Handles
                    );
    if (EFI_ERROR (Status)) {
      if (Status != EFI_NOT_FOUND) {
        DEBUG ((
          DEBUG_ERROR,
          "AcpiPlatform: Failed to locate gEfiLoadedImageProtocolGuid. Status=%r\n",
          Status
          ));
      }

      break;
    }

    ASSERT (HandleCount == 1);

    Status = gBS->HandleProtocol (
                    Handles[0],
                    &gEfiLoadedImageProtocolGuid,
                    (VOID **)&LoadedImage
                    );
    FreePool (Handles);
    if (EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_ERROR,
        "AcpiPlatform: Failed to get gEfiLoadedImageProtocolGuid. Status=%r\n",
        Status
        ));
      break;
    }

    if (LoadedImage->DeviceHandle == NULL) {
      continue;
    }

    //
    // If the last image was loaded from SDMMC, then assume that's
    // the boot device.
    //
    mIsSdmmcBoot = IsDeviceSdmmc (LoadedImage->DeviceHandle);
  }
}

STATIC
VOID
EFIAPI
NotifyReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  gBS->CloseEvent (Event);

  EfiCreateProtocolNotifyEvent (
    &gEfiLoadedImageProtocolGuid,
    TPL_CALLBACK,
    NotifyLoadedImage,
    NULL,
    &mLoadedImageEventRegistration
    );
}

EFI_STATUS
EFIAPI
AcpiPlatformDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                             Status;
  EFI_EVENT                              Event;
  EXIT_BOOT_SERVICES_OS_NOTIFY_PROTOCOL  *ExitBootServicesOsNotify;

  if ((PcdGet32 (PcdConfigTableMode) & CONFIG_TABLE_MODE_ACPI) == 0) {
    DEBUG ((DEBUG_WARN, "AcpiPlatform: ACPI support is disabled by the settings.\n"));
    return EFI_UNSUPPORTED;
  }

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,                  // Type
                  TPL_CALLBACK,                       // NotifyTpl
                  NotifyEndOfDxeEvent,                // NotifyFunction
                  NULL,                               // NotifyContext
                  &gEfiEndOfDxeEventGroupGuid,        // EventGroup
                  &Event                              // Event
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,                  // Type
                  TPL_CALLBACK,                       // NotifyTpl
                  NotifyReadyToBoot,                  // NotifyFunction
                  NULL,                               // NotifyContext
                  &gEfiEventReadyToBootGuid,          // EventGroup
                  &Event                              // Event
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (
                  &gExitBootServicesOsNotifyProtocolGuid,
                  NULL,
                  (VOID **)&ExitBootServicesOsNotify
                  );
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    Status = ExitBootServicesOsNotify->RegisterHandler (
                                         ExitBootServicesOsNotify,
                                         AcpiPlatformExitBootServicesOsHandler
                                         );
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}
