/** @file
 *
 *  Boot Manager device description handler.
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <IndustryStandard/Sd.h>
#include <IndustryStandard/Emmc.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PrintLib.h>
#include <Library/RkAtagsLib.h>
#include <Protocol/NonDiscoverableDevice.h>
#include <Protocol/DiskInfo.h>

RKATAG_BOOTDEV_TYPE  mBootDeviceType;

CHAR16               mBootDescUefiPrefix[]           = L"UEFI ";
CHAR16               mBootDescGenericManufacturer[]  = L"Generic";
CHAR16               mBootDescEmmc[]                 = L" eMMC";
CHAR16               mBootDescEmmcUserData[]         = L" eMMC User Data";
CHAR16               mBootDescEmmcBoot1[]            = L" eMMC Boot 1";
CHAR16               mBootDescEmmcBoot2[]            = L" eMMC Boot 2";
CHAR16               mBootDescEmmcGp1[]              = L" eMMC GP 1";
CHAR16               mBootDescEmmcGp2[]              = L" eMMC GP 2";
CHAR16               mBootDescEmmcGp3[]              = L" eMMC GP 3";
CHAR16               mBootDescEmmcGp4[]              = L" eMMC GP 4";
CHAR16               mBootDescSd[]                   = L" SD Device";
CHAR16               mBootDescFirmwareSuffix[]       = L" [Fw]";

typedef struct {
  UINT8   Id;
  CHAR16  *Name;
} SDMMC_MANUFACTURER;

//
// List based on:
//  - https://www.cameramemoryspeed.com/sd-memory-card-faq/reading-sd-card-cid-serial-psn-internal-numbers/
//  - https://git.kernel.org/pub/scm/utils/mmc/mmc-utils.git/tree/lsmmc.c
//
STATIC SDMMC_MANUFACTURER mSdManufacturers[] = {
  { 0x01, L"Panasonic"                 },
  { 0x02, L"Toshiba/Kingston/Viking"   },
  { 0x03, L"SanDisk"                   },
  { 0x08, L"Silicon Power"             },
  { 0x18, L"Infineon"                  },
  { 0x1b, L"Transcend/Samsung"         },
  { 0x1c, L"Transcend"                 },
  { 0x1d, L"Corsair/AData"             },
  { 0x1e, L"Transcend"                 },
  { 0x1f, L"Kingston"                  },
  { 0x27, L"Delkin/Phison"             },
  { 0x28, L"Lexar"                     },
  { 0x30, L"SanDisk"                   },
  { 0x31, L"Silicon Power"             },
  { 0x33, L"STMicroelectronics"        },
  { 0x41, L"Kingston"                  },
  { 0x6f, L"STMicroelectronics"        },
  { 0x74, L"Transcend"                 },
  { 0x76, L"Patriot"                   },
  { 0x82, L"Gobe/Sony"                 },
  { 0x9c, L"Angelbird/Hoodman"         },
};

STATIC SDMMC_MANUFACTURER mMmcManufacturers[] = {
  { 0x00, L"SanDisk"                   },
  { 0x02, L"Kingston/SanDisk"          },
  { 0x03, L"Toshiba"                   },
  { 0x11, L"Toshiba"                   },
  { 0x13, L"Micron"                    },
  { 0x15, L"Samsung"                   },
  { 0x37, L"KingMax"                   },
  { 0x44, L"ATP"                       },
  { 0x45, L"SanDisk"                   },
  { 0x2c, L"Kingston"                  },
  { 0x70, L"Kingston"                  },
  { 0x88, L"Foresee"                   },
  { 0x9B, L"YMTC"                      },
  { 0xfe, L"Micron"                    },
};

STATIC
NON_DISCOVERABLE_DEVICE *
GetNonDiscoverableDevice (
  IN EFI_DEVICE_PATH_PROTOCOL *DevicePath
  )
{
  EFI_STATUS                 Status;
  EFI_HANDLE                 DeviceHandle;
  NON_DISCOVERABLE_DEVICE    *Device;
  EFI_DEVICE_PATH_PROTOCOL   *CurrentDevicePath;

  CurrentDevicePath = DevicePath;

  if (DevicePath->Type != HARDWARE_DEVICE_PATH
      || DevicePath->SubType != HW_VENDOR_DP) {
    return NULL;
  }

  Status = gBS->LocateDevicePath (&gEdkiiNonDiscoverableDeviceProtocolGuid,
                   &CurrentDevicePath, &DeviceHandle);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  Status = gBS->HandleProtocol (DeviceHandle,
                   &gEdkiiNonDiscoverableDeviceProtocolGuid, (VOID **) &Device);
  if (EFI_ERROR (Status)) {
    return NULL;
  }

  return Device;
}

STATIC
BOOLEAN
CheckIsBootDevice (
  IN NON_DISCOVERABLE_DEVICE *Device
  )
{
  EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  *Descriptor;
  Descriptor = &Device->Resources[0];

  if (Descriptor->Desc != ACPI_ADDRESS_SPACE_DESCRIPTOR ||
      Descriptor->ResType != ACPI_ADDRESS_SPACE_TYPE_MEM) {
    return FALSE;
  }

  if (mBootDeviceType == RkAtagBootDevTypeEmmc) {
    return Descriptor->AddrRangeMin == PcdGet32 (PcdDwcSdhciBaseAddress);
  }
  if (mBootDeviceType == RkAtagBootDevTypeSd0) {
    return Descriptor->AddrRangeMin == PcdGet32 (PcdRkSdmmcBaseAddress);
  }

  return FALSE;
}

STATIC
EFI_STATUS
GetDiskInfoInquiryData (
  IN  EFI_HANDLE   Handle,
  OUT VOID        *InquiryData,
  IN  UINT32      InquiryDataSize
  )
{
  EFI_STATUS Status;
  EFI_DISK_INFO_PROTOCOL *DiskInfo;

  if (InquiryData == NULL || InquiryDataSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->HandleProtocol (Handle,
                   &gEfiDiskInfoProtocolGuid, (VOID **) &DiskInfo);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // InquiryDataSize is set to the expected size if EFI_BUFFER_TOO_SMALL
  // is returned but we'll always call with the correct size.
  Status = DiskInfo->Inquiry (DiskInfo, InquiryData, &InquiryDataSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
CHAR16 *
GetSdMmcManufacturerName (
  IN UINT8    Id,
  IN BOOLEAN  IsMmc
  )
{
  SDMMC_MANUFACTURER  *List;
  UINT8               Count;

  List = IsMmc ? mMmcManufacturers : mSdManufacturers;
  Count = IsMmc ? ARRAY_SIZE (mMmcManufacturers)
                : ARRAY_SIZE (mSdManufacturers);

  for (UINT8 Index; Index < Count; ++Index) {
    if (List[Index].Id == Id) {
      return List[Index].Name;
    }
  }
  return mBootDescGenericManufacturer;
}

STATIC
EFI_STATUS
AsciiSwapAppendToUnicode (
  IN CHAR8   *Source,
  IN UINTN   Length,
  IN CHAR16  *Destination,
  IN UINTN   DestMax
  )
{
  EFI_STATUS    Status;
  CHAR8         Temp;
  UINTN         ConvertedLength;

  if (StrLen (Destination) + Length + 1 > DestMax) {
    return EFI_BUFFER_TOO_SMALL;
  }

  for (UINTN Index = 0; Index < Length / 2; ++Index) {
    Temp = Source[Index];
    Source[Index] = Source[Length - 1 - Index];
    Source[Length - 1 - Index] = Temp;
  }

  Status = AsciiStrnToUnicodeStrS (Source, Length,
                Destination + StrLen (Destination),
                DestMax, &ConvertedLength);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

STATIC
CHAR16 *
GetEmmcDescription (
  CONTROLLER_DEVICE_PATH *DevicePath
  )
{
  switch (DevicePath->ControllerNumber) {
    case EmmcPartitionUserData:
      return mBootDescEmmcUserData;
    case EmmcPartitionBoot1:
      return mBootDescEmmcBoot1;
    case EmmcPartitionBoot2:
      return mBootDescEmmcBoot2;
    case EmmcPartitionGP1:
      return mBootDescEmmcGp1;
    case EmmcPartitionGP2:
      return mBootDescEmmcGp2;
    case EmmcPartitionGP3:
      return mBootDescEmmcGp3;
    case EmmcPartitionGP4:
      return mBootDescEmmcGp4;
  }
  return mBootDescEmmc;
}

STATIC
CHAR16 *
GetSdMmcUefiBootDescription (
  IN CHAR16    *ManufacturerName,
  IN UINT8     *ProductName,
  IN UINT8     ProductNameLength,
  IN UINT8     SerialNumber[4],
  IN CHAR16    *DeviceType,
  IN BOOLEAN   IsBootDevice
  )
{
  CHAR16     *Desc;
  UINTN      DescSize;
  UINT32     Serial32;

  Desc = mBootDescUefiPrefix;

  DescSize = StrSize (Desc) - sizeof (CHAR16)                         // "UEFI "
              + StrSize (ManufacturerName) - sizeof (CHAR16)          // "Samsung"
              + sizeof (CHAR16)                                       // " "
              + ProductNameLength * sizeof (CHAR16)                   // "BJTD4R"
              + sizeof (CHAR16)                                       // " "
              + sizeof (UINT32) * 2 * sizeof (CHAR16)                 // "00000000"
              + StrSize (DeviceType) - sizeof (CHAR16);               // " EMMC User Data"
  if (IsBootDevice) {
    DescSize += StrSize (mBootDescFirmwareSuffix) - sizeof (CHAR16);  // " [Fw]"
  }
  DescSize += sizeof (CHAR16);                                        // '\0'

  Desc = AllocateCopyPool (DescSize, Desc);
  if (Desc == NULL) {
    return NULL;
  }

  StrCatS (Desc, DescSize / sizeof (CHAR16), ManufacturerName);
  StrCatS (Desc, DescSize / sizeof (CHAR16), L" ");

  AsciiSwapAppendToUnicode ((CHAR8 *) ProductName,
                            ProductNameLength,
                            Desc,
                            DescSize / sizeof (CHAR16));

  Serial32 = (SerialNumber[0]
              | (SerialNumber[1]) << 8
              | (SerialNumber[2]) << 16
              | (SerialNumber[3] << 24));

  StrCatS (Desc, DescSize / sizeof (CHAR16), L" ");

  UnicodeValueToStringS (Desc + StrLen (Desc),
                         DescSize - StrSize (Desc),
                         RADIX_HEX | PREFIX_ZERO,
                         Serial32, 8);

  StrCatS (Desc, DescSize / sizeof (CHAR16), DeviceType);

  if (IsBootDevice) {
     StrCatS (Desc, DescSize / sizeof (CHAR16), mBootDescFirmwareSuffix);
  }

  return Desc;
}

STATIC
CHAR16 *
EFIAPI
PlatformBootDescriptionHandler (
  IN EFI_HANDLE    Handle,
  IN CONST CHAR16  *DefaultDescription
  )
{
  EFI_STATUS                Status;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;
  NON_DISCOVERABLE_DEVICE   *NonDiscoverableDevice;
  BOOLEAN                   IsBootDevice;
  CHAR16                    *Desc;
  EMMC_CID                  EmmcCid;
  CHAR16                    *EmmcDesc;
  SD_CID                    SdCid;

  DevicePath = DevicePathFromHandle (Handle);

  NonDiscoverableDevice = GetNonDiscoverableDevice (DevicePath);
  if (NonDiscoverableDevice == NULL) {
    return NULL;
  }

  IsBootDevice = CheckIsBootDevice (NonDiscoverableDevice);

  DevicePath = NextDevicePathNode (DevicePath);

  if (DevicePath->SubType == MSG_EMMC_DP) {
    Status = GetDiskInfoInquiryData (Handle, &EmmcCid, sizeof (EMMC_CID));
    if (EFI_ERROR (Status)) {
      return NULL;
    }

    EmmcDesc = mBootDescEmmc;

    DevicePath = NextDevicePathNode (DevicePath);
    if (DevicePath->SubType == HW_CONTROLLER_DP) {
      if (((CONTROLLER_DEVICE_PATH *) DevicePath)->ControllerNumber != EmmcPartitionUserData) {
        //
        // There can only be one boot device.
        // eMMC has multiple partitions, but I don't know if it's possible
        // to load UEFI from anything other than UserData.
        // If it turns out to be a possibility, we'd need to create a protocol
        // for RkFvbDxe that would allow getting the Device Path of the block
        // device that holds the NVRAM.
        //
        IsBootDevice = FALSE;
      }

      EmmcDesc = GetEmmcDescription ((CONTROLLER_DEVICE_PATH *) DevicePath);
    }

    Desc = GetSdMmcUefiBootDescription (
              GetSdMmcManufacturerName (EmmcCid.ManufacturerId, TRUE),
              EmmcCid.ProductName,
              ARRAY_SIZE (EmmcCid.ProductName),
              EmmcCid.ProductSerialNumber,
              EmmcDesc,
              IsBootDevice);

  } else if (DevicePath->SubType == MSG_SD_DP) {
    Status = GetDiskInfoInquiryData (Handle, &SdCid, sizeof (SD_CID));
    if (EFI_ERROR (Status)) {
      return NULL;
    }

    Desc = GetSdMmcUefiBootDescription (
              GetSdMmcManufacturerName (SdCid.ManufacturerId, FALSE),
              SdCid.ProductName,
              ARRAY_SIZE (SdCid.ProductName),
              SdCid.ProductSerialNumber,
              mBootDescSd,
              IsBootDevice);
  } else {
    return NULL;
  }

  return Desc;
}

EFI_STATUS
EFIAPI
PlatformBootDescriptionLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  RKATAG_BOOTDEV *BootDevice;

  BootDevice = RkAtagsGetBootDev ();
  mBootDeviceType = BootDevice != NULL ? BootDevice->DevType : RkAtagBootDevTypeUnknown;

  return EfiBootManagerRegisterBootDescriptionHandler (PlatformBootDescriptionHandler);
}
