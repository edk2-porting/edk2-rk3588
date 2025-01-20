/** @file
 *
 *  Synopsys DesignWare MSHC platform driver for Rockchip SDMMC
 *
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/RkSdmmcPlatformLib.h>

#include <Protocol/NonDiscoverableDevice.h>
#include <Protocol/PlatformDwMmc.h>

#define DW_MMC_BASE  FixedPcdGet32(PcdRkSdmmcBaseAddress)
#define DW_MMC_SIZE  SIZE_16KB

#pragma pack (1)
typedef struct {
  VENDOR_DEVICE_PATH          Vendor;
  UINT64                      BaseAddress;
  UINT8                       ResourceType;
  EFI_DEVICE_PATH_PROTOCOL    End;
} NON_DISCOVERABLE_DEVICE_PATH;

typedef struct {
  NON_DISCOVERABLE_DEVICE_PATH    DevicePath;
  NON_DISCOVERABLE_DEVICE         Device;
} DW_MMC_DEVICE;
#pragma pack ()

STATIC EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR  mDwMmcDeviceDesc[] = {
  {
    ACPI_ADDRESS_SPACE_DESCRIPTOR,                    // Desc
    sizeof (EFI_ACPI_ADDRESS_SPACE_DESCRIPTOR) - 3,   // Len
    ACPI_ADDRESS_SPACE_TYPE_MEM,                      // ResType
    0,                                                // GenFlag
    0,                                                // SpecificFlag
    32,                                               // AddrSpaceGranularity
    DW_MMC_BASE,                                      // AddrRangeMin
    DW_MMC_SIZE + DW_MMC_SIZE - 1,                    // AddrRangeMax
    0,                                                // AddrTranslationOffset
    DW_MMC_SIZE                                       // AddrLen
  }
};

STATIC DW_MMC_DEVICE  mDwMmcDevice = {
  {
    {
      {
        HARDWARE_DEVICE_PATH,
        HW_VENDOR_DP,
        {
          (UINT8)(OFFSET_OF (NON_DISCOVERABLE_DEVICE_PATH, End)),
          (UINT8)((OFFSET_OF (NON_DISCOVERABLE_DEVICE_PATH, End)) >> 8)
        }
      },
      EDKII_NON_DISCOVERABLE_DEVICE_PROTOCOL_GUID
    },
    DW_MMC_BASE,
    ACPI_ADDRESS_SPACE_TYPE_MEM,
    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      {
        sizeof (EFI_DEVICE_PATH_PROTOCOL),
        0
      }
    }
  },
  {
    &gDwMmcHcNonDiscoverableDeviceGuid,
    NonDiscoverableDeviceDmaTypeNonCoherent,
    NULL,
    mDwMmcDeviceDesc
  }
};

STATIC DW_MMC_HC_SLOT_CAP  mDwMmcCapability = {
  .HighSpeed   = 1,
  .BusWidth    = 4,
  .SlotType    = RemovableSlot,
  .CardType    = SdCardType,
  .Voltage30   = 1,
  .BaseClkFreq = 52000
};

STATIC
EFI_STATUS
EFIAPI
RkSdmmcGetCapability (
  IN     EFI_HANDLE          Controller,
  IN     UINT8               Slot,
  OUT    DW_MMC_HC_SLOT_CAP  *Capability
  )
{
  if ((Controller != mDwMmcCapability.Controller) || (Capability == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  CopyMem (Capability, &mDwMmcCapability, sizeof (DW_MMC_HC_SLOT_CAP));

  return EFI_SUCCESS;
}

STATIC
BOOLEAN
EFIAPI
RkSdmmcCardDetect (
  IN EFI_HANDLE  Controller,
  IN UINT8       Slot
  )
{
  RKSDMMC_CARD_PRESENCE_STATE  PresenceState;

  if (Controller != mDwMmcCapability.Controller) {
    return FALSE;
  }

  PresenceState = RkSdmmcGetCardPresenceState ();

  if ((PresenceState == RkSdmmcCardPresenceUnsupported) ||
      FixedPcdGetBool (PcdRkSdmmcCardDetectBroken))
  {
    return TRUE; // let the driver do software detection
  }

  return PresenceState == RkSdmmcCardPresent;
}

STATIC PLATFORM_DW_MMC_PROTOCOL  mDwMmcDeviceProtocol = {
  RkSdmmcGetCapability,
  RkSdmmcCardDetect
};

EFI_STATUS
EFIAPI
RkSdmmcDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  Handle;

  RkSdmmcSetIoMux ();

  RkSdmmcSetClockRate (mDwMmcCapability.BaseClkFreq * 1000);

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mDwMmcCapability.Controller,
                  &gEfiDevicePathProtocolGuid,
                  &mDwMmcDevice.DevicePath,
                  &gEdkiiNonDiscoverableDeviceProtocolGuid,
                  &mDwMmcDevice.Device,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gPlatformDwMmcProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mDwMmcDeviceProtocol
                  );
  return Status;
}
