/** @file

  Copyright (c) 2014, Applied Micro Curcuit Corporation. All rights reserved.<BR>
  Copyright (c) 2015 - 2020, Hisilicon Limited. All rights reserved.<BR>
  Copyright (c) 2015, Linaro Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

  This driver is called to initialize the FW part of the PHY in preparation
  for the OS.

**/

#include <Guid/ShellVariableGuid.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>

#include <PiDxe.h>
#include <Guid/EventGroup.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PrintLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/AcpiAml.h>

#include <Protocol/HisiBoardNicProtocol.h>
#include <Protocol/HisiSasConfig.h>

// Turn on debug message by enabling below define
//#define ACPI_DEBUG

#ifdef ACPI_DEBUG
#define DBG(arg...) DEBUG((EFI_D_ERROR,## arg))
#else
#define DBG(arg...)
#endif

#define EFI_ACPI_MAX_NUM_TABLES         20
#define DSDT_SIGNATURE                  0x54445344

#define ACPI_ETH_MAC_KEY                "local-mac-address"
#define ACPI_ETH_SAS_KEY                "sas-addr"

#define PREFIX_VARIABLE_NAME            L"MAC"
#define PREFIX_VARIABLE_NAME_COMPAT     L"RGMII_MAC"
#define ADDRESS_MAX_LEN                 30

CHAR8 *mHisiAcpiDevId[] = {"HISI00C1","HISI00C2","HISI0162"};

typedef enum {
  DsdtDeviceUnknown,
  DsdtDeviceLan,
  DsdtDeviceSas
} DSDT_DEVICE_TYPE;

STATIC
EFI_STATUS
GetEnvMac(
  IN     UINTN    MacNextID,
  IN OUT UINT8    *MacBuffer
  )
{
  EFI_MAC_ADDRESS Mac;
  EFI_STATUS Status;
  HISI_BOARD_NIC_PROTOCOL *OemNic = NULL;

  Status = gBS->LocateProtocol(&gHisiBoardNicProtocolGuid, NULL, (VOID **)&OemNic);
  if(EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[%a]:[%dL] LocateProtocol failed %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  Status = OemNic->GetMac(&Mac, MacNextID);
  if(EFI_ERROR(Status))
  {
    DEBUG((EFI_D_ERROR, "[%a]:[%dL] GetMac failed %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  CopyMem (MacBuffer, &Mac, 6);
  DEBUG((EFI_D_ERROR, "Port %d MAC %02x:%02x:%02x:%02x:%02x:%02x\n",
        MacNextID,
        MacBuffer[0],
        MacBuffer[1],
        MacBuffer[2],
        MacBuffer[3],
        MacBuffer[4],
        MacBuffer[5]
        ));

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
GetSasAddress (
  IN UINT8        Index,
  IN OUT UINT8    *SasAddrBuffer
  )
{
  EFI_STATUS Status;
  HISI_SAS_CONFIG_PROTOCOL *HisiSasConf;

  if (SasAddrBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (&gHisiSasConfigProtocolGuid, NULL, (VOID **)&HisiSasConf);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Locate Sas Config Protocol failed %r\n", Status));
    SasAddrBuffer[0] = 0x00;
    SasAddrBuffer[1] = 0x00;
    SasAddrBuffer[2] = 0x00;
    SasAddrBuffer[3] = 0x00;
    SasAddrBuffer[4] = 0x00;
    SasAddrBuffer[5] = 0x00;
    SasAddrBuffer[6] = 0x00;
    SasAddrBuffer[7] = Index;
    return Status;
  }

  return HisiSasConf->GetAddr (Index, SasAddrBuffer);
}

STATIC
EFI_STATUS
UpdateAddressInOption (
  IN EFI_ACPI_SDT_PROTOCOL  *AcpiTableProtocol,
  IN EFI_ACPI_HANDLE        ChildHandle,
  IN UINTN                  DevNextID,
  IN DSDT_DEVICE_TYPE       FoundDev
  )
{
  EFI_STATUS          Status;
  EFI_ACPI_DATA_TYPE  DataType;
  CONST VOID          *Buffer;
  UINTN               DataSize;
  UINTN               Count;
  EFI_ACPI_HANDLE     CurrentHandle;
  UINT8               *AddressBuffer;
  UINT8               AddressByte;

  AddressByte = 0;
  AddressBuffer = AllocateZeroPool (ADDRESS_MAX_LEN);
  if (AddressBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "%a:%d AllocateZeroPool failed\n", __FILE__, __LINE__));
    return EFI_OUT_OF_RESOURCES;
  }

  switch (FoundDev) {
    case DsdtDeviceLan:
      //Update the MAC
      Status = GetEnvMac (DevNextID, AddressBuffer);
      AddressByte = 6;
      break;
    case DsdtDeviceSas:
      //Update SAS Address.
      Status = GetSasAddress (DevNextID, AddressBuffer);
      AddressByte = 8;
      break;
    default:
      Status = EFI_INVALID_PARAMETER;
  }
  if (EFI_ERROR (Status)) {
    FreePool (AddressBuffer);
    return Status;
  }

  for (Count = 0; Count < AddressByte; Count++) {
    Status = AcpiTableProtocol->GetOption (CurrentHandle, 1, &DataType, &Buffer, &DataSize);
    if (EFI_ERROR (Status)) {
      break;
    }

    if (DataType != EFI_ACPI_DATA_TYPE_UINT)
      break;

    // only need one byte.
    // FIXME: Assume the CPU is little endian
    Status = AcpiTableProtocol->SetOption (
                                  CurrentHandle,
                                  1,
                                  AddressBuffer + Count,
                                  sizeof(UINT8));
    if (EFI_ERROR (Status)) {
      break;
    }

    Status = AcpiTableProtocol->GetChild (ChildHandle, &CurrentHandle);
    if (EFI_ERROR (Status) || CurrentHandle == NULL) {
      break;
    }
  }

  FreePool (AddressBuffer);
  return Status;
}

STATIC
EFI_STATUS
UpdateAddressInPackage (
  IN EFI_ACPI_SDT_PROTOCOL  *AcpiTableProtocol,
  IN EFI_ACPI_HANDLE        ChildHandle,
  IN UINTN                  Level,
  IN OUT BOOLEAN            *Found,
  IN UINTN                  DevNextID,
  IN DSDT_DEVICE_TYPE       FoundDev
  )
{
  // ASL template for ethernet driver:
/*
 *   Name (_DSD, Package () {
 *   ToUUID("daffd814-6eba-4d8c-8a91-bc9bbf4aa301"),
 *   Package () {
 *     Package (2) {"mac-address", Package (6) { 00, 11, 22, 33, 44, 55 }}
 *     Package (2) {"phy-channel", 0},
 *     Package (2) {"phy-mode", "rgmii"},
 *     Package (2) {"max-transfer-unit", 0x5dc},   // MTU of 1500
 *     Package (2) {"max-speed", 0x3e8},            // 1000 Mbps
 *   }
 * })
 */
  EFI_STATUS          Status;
  EFI_ACPI_DATA_TYPE  DataType;
  CONST UINT8         *Data;
  CONST VOID          *Buffer;
  UINTN               DataSize;
  EFI_ACPI_HANDLE     CurrentHandle;
  EFI_ACPI_HANDLE     NextHandle;
  EFI_ACPI_HANDLE     Level1Handle;

  DBG("In Level:%d\n", Level);
  Level1Handle = NULL;
  Status = EFI_SUCCESS;
  for (CurrentHandle = NULL; ;) {
    Status = AcpiTableProtocol->GetChild(ChildHandle, &CurrentHandle);
    if (Level == 1) {
      Level1Handle = CurrentHandle;
    }
    if (Level != 3 && (EFI_ERROR(Status) || CurrentHandle == NULL))
       break;

    Status = AcpiTableProtocol->GetOption(CurrentHandle, 0, &DataType, &Buffer, &DataSize);
    Data = Buffer;
    DBG("_DSD Child Subnode Store Op Code 0x%02X 0x%02X %02X\n",
        DataSize, Data[0], DataSize > 1 ? Data[1] : 0);

    if (Level < 2 && Data[0] != AML_PACKAGE_OP)
      continue;

    if (Level == 2 && Data[0] == AML_STRING_PREFIX) {
      Status = AcpiTableProtocol->GetOption(CurrentHandle, 1, &DataType, &Buffer, &DataSize);
      if (EFI_ERROR(Status))
        break;

      DBG("  _DSD Child Subnode Store Op Code 0x%02X 0x%02X %02X\n",
              DataSize, Data[0], DataSize > 1 ? Data[1] : 0);

      Data = Buffer;
      if ((DataType != EFI_ACPI_DATA_TYPE_STRING) ||
          ((AsciiStrCmp ((CHAR8 *) Data, ACPI_ETH_MAC_KEY) != 0) &&
           (AsciiStrCmp ((CHAR8 *) Data, ACPI_ETH_SAS_KEY) != 0))) {
        ChildHandle = Level1Handle;
        continue;
      }

      DBG("_DSD Key Type %d. Found address key\n", DataType);

      //
      // We found the node.
      //
      *Found = TRUE;
      continue;
    }

    if (Level == 3 && *Found) {
      Status = UpdateAddressInOption (AcpiTableProtocol, ChildHandle, DevNextID, FoundDev);
      break;
    }

    if (Level > 3)
      break;

    //Search next package
    AcpiTableProtocol->Open((VOID *) Buffer, &NextHandle);
    Status = UpdateAddressInPackage (
               AcpiTableProtocol,
               NextHandle,
               Level + 1,
               Found,
               DevNextID,
               FoundDev);
    AcpiTableProtocol->Close(NextHandle);
    if (!EFI_ERROR(Status))
      break;
  }

  return Status;
}

STATIC
EFI_STATUS
SearchReplacePackageAddress (
  IN EFI_ACPI_SDT_PROTOCOL  *AcpiTableProtocol,
  IN EFI_ACPI_HANDLE        ChildHandle,
  IN UINTN                  DevNextID,
  IN DSDT_DEVICE_TYPE       FoundDev
  )
{
  BOOLEAN Found = FALSE;
  UINTN Level = 0;

  return UpdateAddressInPackage (AcpiTableProtocol, ChildHandle, Level,
                                 &Found, DevNextID, FoundDev);
}

EFI_STATUS
GetDeviceInfo (
  EFI_ACPI_SDT_PROTOCOL   *AcpiTableProtocol,
  EFI_ACPI_HANDLE         ChildHandle,
  UINTN                   *DevID,
  DSDT_DEVICE_TYPE        *FoundDev
  )
{
  EFI_STATUS Status;
  EFI_ACPI_DATA_TYPE  DataType;
  CHAR8               Data[5];
  CONST VOID          *Buffer;
  UINTN               DataSize;

  // Get NameString
  Status = AcpiTableProtocol->GetOption (ChildHandle, 1, &DataType, &Buffer, &DataSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[%a:%d] Get NameString failed: %r\n", __FUNCTION__, __LINE__, Status));
    return Status;
  }

  CopyMem (Data, Buffer, 4);
  DBG("Size %p Data %02x %02x %02x %02x\n", DataSize, Data[0], Data[1], Data[2], Data[3]);

  Data[4] = '\0';
  if ((DataSize != 4) ||
    (Data[3] > '9' || Data[3] < '0')) {
    DEBUG ((DEBUG_ERROR, "The NameString %a is not ETHn or SASn\n", Data));
    return EFI_INVALID_PARAMETER;
  }

  if (AsciiStrnCmp ("ETH", Data, 3) == 0) {
    *FoundDev = DsdtDeviceLan;
  } else if (AsciiStrnCmp ("SAS", Data, 3) == 0) {
    *FoundDev = DsdtDeviceSas;
  } else {
    DEBUG ((DEBUG_ERROR, "[%a:%d] The NameString %a is not ETHn or SASn\n",
            __FUNCTION__, __LINE__, Data));
    return EFI_INVALID_PARAMETER;
  }

  *DevID = Data[3] - '0';
  return EFI_SUCCESS;
}

EFI_STATUS ProcessDSDTDevice (
  EFI_ACPI_SDT_PROTOCOL *AcpiTableProtocol,
  EFI_ACPI_HANDLE ChildHandle)
{
  EFI_STATUS          Status;
  EFI_ACPI_DATA_TYPE  DataType;
  CONST UINT8         *Data;
  CONST VOID          *Buffer;
  UINTN               DataSize;
  EFI_ACPI_HANDLE     DevHandle;
  DSDT_DEVICE_TYPE    FoundDev = DsdtDeviceUnknown;
  UINTN               DevNextID;
  BOOLEAN             HisiAcpiDevNotFound;
  UINTN               Index;

  Status = AcpiTableProtocol->GetOption(ChildHandle, 0, &DataType, &Buffer, &DataSize);
  if (EFI_ERROR(Status))
    return EFI_SUCCESS;

  Data = Buffer;
  //
  // Skip all non-device type
  //
  if (DataSize != 2 || Data[0] != AML_EXT_OP || Data[1] != AML_EXT_DEVICE_OP)
    return EFI_SUCCESS;

  //
  // Walk the device type node
  //
  for (DevHandle = NULL; ; ) {
    Status = AcpiTableProtocol->GetChild(ChildHandle, &DevHandle);
    if (EFI_ERROR(Status) || DevHandle == NULL)
      break;

    //
    // Search for _HID with Device ID
    //
    Status = AcpiTableProtocol->GetOption(DevHandle, 0, &DataType, &Buffer, &DataSize);
    if (EFI_ERROR(Status))
      break;

    Data = Buffer;
    DBG("Data Type 0x%02X %02X\n", Data[0], DataSize > 1 ? Data[1] : 0);
    if (DataSize == 1 && Data[0] == AML_NAME_OP) {
      Status = AcpiTableProtocol->GetOption(DevHandle, 1, &DataType, &Buffer, &DataSize);
      if (EFI_ERROR(Status))
        break;

      Data = Buffer;
      if (DataType == EFI_ACPI_DATA_TYPE_NAME_STRING) {
        if (AsciiStrnCmp((CHAR8 *) Data, "_HID", 4) == 0) {
          EFI_ACPI_HANDLE ValueHandle;

          Status = AcpiTableProtocol->GetOption(DevHandle, 2, &DataType, &Buffer, &DataSize);
          if (EFI_ERROR(Status))
            break;

          if (DataType != EFI_ACPI_DATA_TYPE_CHILD)
            continue;

          AcpiTableProtocol->Open((VOID *) Buffer, &ValueHandle);
          Status = AcpiTableProtocol->GetOption(ValueHandle, 1, &DataType, &Buffer, &DataSize);

          Data = Buffer;
          DBG("[%a:%d] - _HID = %a\n", __FUNCTION__, __LINE__, Data);

          if (EFI_ERROR(Status) ||
              DataType != EFI_ACPI_DATA_TYPE_STRING) {
            AcpiTableProtocol->Close (ValueHandle);
            FoundDev = DsdtDeviceUnknown;
            continue;
          }

          HisiAcpiDevNotFound = TRUE;
          for (Index = 0; Index < ARRAY_SIZE (mHisiAcpiDevId); Index++) {
            if (AsciiStrCmp ((CHAR8 *)Data, mHisiAcpiDevId[Index]) == 0) {
              HisiAcpiDevNotFound = FALSE;
              break;
            }
          }
          if (HisiAcpiDevNotFound) {
            AcpiTableProtocol->Close (ValueHandle);
            FoundDev = DsdtDeviceUnknown;
            continue;
          }

          DBG("Found device\n");
          AcpiTableProtocol->Close(ValueHandle);
          Status = GetDeviceInfo (AcpiTableProtocol, ChildHandle, &DevNextID, &FoundDev);
          if (EFI_ERROR (Status)) {
            continue;
          }
        } else if ((FoundDev != DsdtDeviceUnknown) && AsciiStrnCmp((CHAR8 *) Data, "_DSD", 4) == 0) {
          //
          // Patch DSD data
          //
          EFI_ACPI_HANDLE    PkgHandle;
          Status = AcpiTableProtocol->GetOption(DevHandle, 2, &DataType, &Buffer, &DataSize);
          if (EFI_ERROR(Status))
            break;

          if (DataType != EFI_ACPI_DATA_TYPE_CHILD)
            continue;

          //
          // Open package data
          //
          AcpiTableProtocol->Open((VOID *) Buffer, &PkgHandle);
          Status = AcpiTableProtocol->GetOption(PkgHandle, 0, &DataType, &Buffer, &DataSize);

          Data = Buffer;
          DBG("_DSD Subnode Store Op Code 0x%02X %02X\n",
                Data[0], DataSize > 1 ? Data[1] : 0);

          //
          // Walk the _DSD node
          //
          if (DataSize == 1 && Data[0] == AML_PACKAGE_OP) {
            Status = SearchReplacePackageAddress (AcpiTableProtocol, PkgHandle, DevNextID, FoundDev);
          }

          AcpiTableProtocol->Close(PkgHandle);
        } else if (AsciiStrnCmp ((CHAR8 *) Data, "_ADR", 4) == 0) {
          Status = AcpiTableProtocol->GetOption (DevHandle, 2, &DataType, &Buffer, &DataSize);
          if (EFI_ERROR (Status)) {
            break;
          }

          if (DataType != EFI_ACPI_DATA_TYPE_CHILD) {
            continue;
          }

          Status = GetDeviceInfo (AcpiTableProtocol, ChildHandle, &DevNextID, &FoundDev);

          if (EFI_ERROR (Status)) {
            continue;
          }
        }
      }
    } else if ((DataSize == 2) && (Data[0] == AML_EXT_OP) && (Data[1] == AML_EXT_DEVICE_OP)) {
      ProcessDSDTDevice (AcpiTableProtocol, DevHandle);
    }
  }

  return EFI_SUCCESS;
}


BOOLEAN
IsSbScope (
  EFI_ACPI_SDT_PROTOCOL   *AcpiTableProtocol,
  EFI_ACPI_HANDLE         ChildHandle
  )
{
  EFI_STATUS          Status;
  EFI_ACPI_DATA_TYPE  DataType;
  CONST UINT8         *Data;
  CONST VOID          *Buffer;
  UINTN               DataSize;

  Status = AcpiTableProtocol->GetOption (ChildHandle, 0, &DataType, &Buffer, &DataSize);
  if (EFI_ERROR(Status)) return FALSE;

  Data = Buffer;
  if (DataSize != 1 || Data[0] != AML_SCOPE_OP) {
    return FALSE;
  }

  return TRUE;
}

EFI_STATUS ProcessDSDTChild(
  EFI_ACPI_SDT_PROTOCOL *AcpiTableProtocol,
  EFI_ACPI_HANDLE ChildHandle)
{
  EFI_STATUS          Status;
  EFI_ACPI_HANDLE     DevHandle;

  // Check Scope(_SB) at first
  if (!IsSbScope (AcpiTableProtocol, ChildHandle)) {
    return ProcessDSDTDevice (AcpiTableProtocol, ChildHandle);
  }

  for (DevHandle = NULL; ; ) {
    Status = AcpiTableProtocol->GetChild (ChildHandle, &DevHandle);
    if (EFI_ERROR(Status) || DevHandle == NULL) {
      break;
    }

    ProcessDSDTDevice (AcpiTableProtocol, DevHandle);
  }

  return EFI_SUCCESS;
}

static EFI_STATUS ProcessDSDT(
  EFI_ACPI_SDT_PROTOCOL *AcpiTableProtocol,
  EFI_ACPI_HANDLE TableHandle)
{
  EFI_STATUS              Status;
  EFI_ACPI_HANDLE         ChildHandle;
  //
  // Parse table for device type
  DBG ("[%a:%d] - TableHandle=%p\n", __FUNCTION__, __LINE__, TableHandle);
  for (ChildHandle = NULL; ; ) {
    Status = AcpiTableProtocol->GetChild(TableHandle, &ChildHandle);
    DBG ("[%a:%d] - Child=%p, %r\n", __FUNCTION__, __LINE__, ChildHandle, Status);
    if (EFI_ERROR(Status))
      break;
    if (ChildHandle == NULL)
      break;

    ProcessDSDTChild(AcpiTableProtocol, ChildHandle);
  }

  return EFI_SUCCESS;
}

STATIC
VOID
AcpiCheckSum (
  IN OUT  EFI_ACPI_SDT_HEADER *Table
  )
{
  UINTN ChecksumOffset;
  UINT8 *Buffer;

  ChecksumOffset = OFFSET_OF (EFI_ACPI_DESCRIPTION_HEADER, Checksum);
  Buffer = (UINT8 *)Table;

  //
  // set checksum to 0 first
  //
  Buffer[ChecksumOffset] = 0;

  //
  // Update checksum value
  //
  Buffer[ChecksumOffset] = CalculateCheckSum8 (Buffer, Table->Length);
}

EFI_STATUS
UpdateAcpiDsdtTable (
  VOID
  )
{
  EFI_STATUS              Status;
  EFI_ACPI_SDT_PROTOCOL   *AcpiTableProtocol;
  EFI_ACPI_SDT_HEADER     *Table;
  EFI_ACPI_TABLE_VERSION  TableVersion;
  UINTN                   TableKey;
  EFI_ACPI_HANDLE         TableHandle;
  UINTN                   i;

  DEBUG ((EFI_D_ERROR, "Updating Ethernet MAC in ACPI DSDT...\n"));

  //
  // Find the AcpiTable protocol
  Status = gBS->LocateProtocol(&gEfiAcpiSdtProtocolGuid, NULL, (VOID**) &AcpiTableProtocol);
  if (EFI_ERROR(Status)) {
    DBG("Unable to locate ACPI table protocol\n");
    return EFI_SUCCESS;
  }

  //
  // Search for DSDT Table
  for (i = 0; i < EFI_ACPI_MAX_NUM_TABLES; i++) {
    Status = AcpiTableProtocol->GetAcpiTable(i, &Table, &TableVersion, &TableKey);
    if (EFI_ERROR(Status))
      break;
    if (Table->Signature != DSDT_SIGNATURE)
      continue;

    Status = AcpiTableProtocol->OpenSdt(TableKey, &TableHandle);
    if (EFI_ERROR(Status))
      break;

    ProcessDSDT(AcpiTableProtocol, TableHandle);

    AcpiTableProtocol->Close(TableHandle);
    AcpiCheckSum (Table);
  }

  return EFI_SUCCESS;
}
