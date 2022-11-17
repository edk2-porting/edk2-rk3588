/** @file
*
*  Copyright (c) 2018, Linaro Ltd. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Guid/EventGroup.h>

#include <Hi3660.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/SerialPortLib.h>
#include <Library/TimerLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include <Protocol/DevicePathFromText.h>
#include <Protocol/EmbeddedGpio.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/PlatformBootManager.h>
#include <Protocol/PlatformVirtualKeyboard.h>

#include "HiKey960Dxe.h"

STATIC EMBEDDED_GPIO   *mGpio;

STATIC
VOID
InitSdCard (
  IN VOID
  )
{
  UINT32        Data;

  //
  // LDO16
  // 000: 1.75V, 001: 1.8V, 010: 2.4V, 011: 2.6V, 100: 2.7V,
  // 101: 2.85V, 110: 2.95V, 111: 3.0V.
  //
  Data = MmioRead32 (PMIC_LDO16_VSET_REG) & LDO16_VSET_MASK;
  Data |= 6;
  MmioWrite32 (PMIC_LDO16_VSET_REG, Data);
  MmioOr32 (PMIC_LDO16_ONOFF_ECO_REG, LDO16_ONOFF_ECO_LDO16_ENABLE);
  //
  // wait regulator stable
  //
  MicroSecondDelay (100);

  //
  // LDO9
  // 000: 1.75V, 001: 1.8V, 010: 1.825V, 011: 2.8V, 100: 2.85V,
  // 101: 2.95V, 110: 3.0V, 111: 3.3V.
  //
  Data = MmioRead32 (PMIC_LDO9_VSET_REG) & LDO9_VSET_MASK;
  Data |= 5;
  MmioWrite32 (PMIC_LDO9_VSET_REG, Data);
  MmioOr32 (PMU_REG_BASE + (0x6a << 2), 2);
  //
  // wait regulator stable
  //
  MicroSecondDelay (100);

  //
  // GPIO203
  //
  MmioWrite32 (IOMG_AO_REG_BASE + (24 << 2), 0); // GPIO function

  //
  // SD pinmux
  //
  MmioWrite32 (IOMG_MMC0_000_REG, IOMG_FUNC1); // SD_CLK
  MmioWrite32 (IOMG_MMC0_001_REG, IOMG_FUNC1); // SD_CMD
  MmioWrite32 (IOMG_MMC0_002_REG, IOMG_FUNC1); // SD_DATA0
  MmioWrite32 (IOMG_MMC0_003_REG, IOMG_FUNC1); // SD_DATA1
  MmioWrite32 (IOMG_MMC0_004_REG, IOMG_FUNC1); // SD_DATA2
  MmioWrite32 (IOMG_MMC0_005_REG, IOMG_FUNC1); // SD_DATA3
  MmioWrite32 (IOCG_MMC0_000_REG, IOCG_DRIVE (15)); // SD_CLK float with 32mA
  MmioWrite32 (IOCG_MMC0_001_REG, IOCG_PULLUP | IOCG_DRIVE (8)); // SD_CMD
  MmioWrite32 (IOCG_MMC0_002_REG, IOCG_PULLUP | IOCG_DRIVE (8)); // SD_DATA0
  MmioWrite32 (IOCG_MMC0_003_REG, IOCG_PULLUP | IOCG_DRIVE (8)); // SD_DATA1
  MmioWrite32 (IOCG_MMC0_004_REG, IOCG_PULLUP | IOCG_DRIVE (8)); // SD_DATA2
  MmioWrite32 (IOCG_MMC0_005_REG, IOCG_PULLUP | IOCG_DRIVE (8)); // SD_DATA3

  //
  // SC_SEL_SD:
  //   0xx: 3.2MHz, 100: PPLL0, 101: PPLL1, 11x: PPLL2.
  // SC_DIV_SD:
  //   divider = value + 1
  //
  do {
    MmioOr32 (
      CRG_CLKDIV4,
      CLKDIV4_SC_SEL_SD (7) |
      (CLKDIV4_SC_SEL_SD_MASK << CLKDIV4_SC_MASK_SHIFT)
      );
    Data = MmioRead32 (CRG_CLKDIV4) & CLKDIV4_SC_SEL_SD_MASK;
  } while (Data != CLKDIV4_SC_SEL_SD (7));

  //
  // Unreset SD controller
  //
  MmioWrite32 (CRG_PERRSTDIS4, PERRSTEN4_SD);
  do {
    Data = MmioRead32 (CRG_PERRSTSTAT4);
  } while ((Data & PERRSTEN4_SD) == PERRSTEN4_SD);
  //
  // Enable SD controller clock
  //
  MmioOr32 (CRG_PEREN0, PEREN0_GT_HCLK_SD);
  MmioOr32 (CRG_PEREN4, PEREN4_GT_CLK_SD);
  do {
    Data = MmioRead32 (CRG_PERCLKEN4);
  } while ((Data & PEREN4_GT_CLK_SD) != PEREN4_GT_CLK_SD);
}

VOID
InitPeripherals (
  IN VOID
  )
{
  //
  // Enable FPLL0
  //
  MmioOr32 (SCTRL_SCFPLLCTRL0, SCTRL_SCFPLLCTRL0_FPLL0_EN);

  InitSdCard ();

  //
  // Enable wifi clock
  //
  MmioOr32 (PMIC_HARDWARE_CTRL0, PMIC_HARDWARE_CTRL0_WIFI_CLK);
  MmioOr32 (PMIC_OSC32K_ONOFF_CTRL, PMIC_OSC32K_ONOFF_CTRL_EN_32K);
}

/**
  Notification function of the event defined as belonging to the
  EFI_END_OF_DXE_EVENT_GROUP_GUID event group that was created in
  the entry point of the driver.

  This function is called when an event belonging to the
  EFI_END_OF_DXE_EVENT_GROUP_GUID event group is signalled. Such an
  event is signalled once at the end of the dispatching of all
  drivers (end of the so called DXE phase).

  @param[in]  Event    Event declared in the entry point of the driver whose
                       notification function is being invoked.
  @param[in]  Context  NULL
**/
STATIC
VOID
OnEndOfDxe (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  UINT32        BootMode;
  VOID          *RecoveryStr;
  VOID          *SwitchStr;

  BootMode = MmioRead32 (SCTRL_BAK_DATA0) & BOOT_MODE_MASK;
  if (BootMode == BOOT_MODE_RECOVERY) {
    RecoveryStr = "WARNING: CAN NOT BOOT KERNEL IN RECOVERY MODE!\r\n";
    SwitchStr = "Switch to normal boot mode, then reboot to boot kernel.\r\n";
    SerialPortWrite (RecoveryStr, AsciiStrLen (RecoveryStr));
    SerialPortWrite (SwitchStr, AsciiStrLen (SwitchStr));
  }
}

STATIC
EFI_STATUS
CreatePlatformBootOptionFromPath (
  IN     CHAR16                          *PathStr,
  IN     CHAR16                          *Description,
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION    *BootOption
  )
{
  EFI_STATUS                   Status;
  EFI_DEVICE_PATH              *DevicePath;

  DevicePath = (EFI_DEVICE_PATH *)ConvertTextToDevicePath (PathStr);
  ASSERT (DevicePath != NULL);
  Status = EfiBootManagerInitializeLoadOption (
             BootOption,
             LoadOptionNumberUnassigned,
             LoadOptionTypeBoot,
             LOAD_OPTION_ACTIVE,
             Description,
             DevicePath,
             NULL,
             0
             );
  FreePool (DevicePath);
  return Status;
}

STATIC
EFI_STATUS
CreatePlatformBootOptionFromGuid (
  IN     EFI_GUID                        *FileGuid,
  IN     CHAR16                          *Description,
  IN OUT EFI_BOOT_MANAGER_LOAD_OPTION    *BootOption
  )
{
  EFI_STATUS                             Status;
  EFI_DEVICE_PATH                        *DevicePath;
  EFI_DEVICE_PATH                        *TempDevicePath;
  EFI_LOADED_IMAGE_PROTOCOL              *LoadedImage;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH      FileNode;

  Status = gBS->HandleProtocol (
                  gImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (VOID **) &LoadedImage
                  );
  ASSERT_EFI_ERROR (Status);
  EfiInitializeFwVolDevicepathNode (&FileNode, FileGuid);
  TempDevicePath = DevicePathFromHandle (LoadedImage->DeviceHandle);
  ASSERT (TempDevicePath != NULL);
  DevicePath = AppendDevicePathNode (
                 TempDevicePath,
                 (EFI_DEVICE_PATH_PROTOCOL *) &FileNode
                 );
  ASSERT (DevicePath != NULL);
  Status = EfiBootManagerInitializeLoadOption (
             BootOption,
             LoadOptionNumberUnassigned,
             LoadOptionTypeBoot,
             LOAD_OPTION_ACTIVE,
             Description,
             DevicePath,
             NULL,
             0
             );
  FreePool (DevicePath);
  return Status;
}

STATIC
EFI_STATUS
GetPlatformBootOptionsAndKeys (
  OUT UINTN                              *BootCount,
  OUT EFI_BOOT_MANAGER_LOAD_OPTION       **BootOptions,
  OUT EFI_INPUT_KEY                      **BootKeys
  )
{
  EFI_GUID                               *FileGuid;
  CHAR16                                 *PathStr;
  EFI_STATUS                             Status;
  UINTN                                  Size;

  Size = sizeof (EFI_BOOT_MANAGER_LOAD_OPTION) * HIKEY960_BOOT_OPTION_NUM;
  *BootOptions = (EFI_BOOT_MANAGER_LOAD_OPTION *)AllocateZeroPool (Size);
  if (*BootOptions == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate memory for BootOptions\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  Size = sizeof (EFI_INPUT_KEY) * HIKEY960_BOOT_OPTION_NUM;
  *BootKeys = (EFI_INPUT_KEY *)AllocateZeroPool (Size);
  if (*BootKeys == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate memory for BootKeys\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Error;
  }

  PathStr = (CHAR16 *)PcdGetPtr (PcdSdBootDevicePath);
  ASSERT (PathStr != NULL);
  Status = CreatePlatformBootOptionFromPath (
             PathStr,
             L"Boot from SD",
             &(*BootOptions)[0]
             );
  ASSERT_EFI_ERROR (Status);

  PathStr = (CHAR16 *)PcdGetPtr (PcdAndroidBootDevicePath);
  ASSERT (PathStr != NULL);
  Status = CreatePlatformBootOptionFromPath (
             PathStr,
             L"Grub",
             &(*BootOptions)[1]
             );
  ASSERT_EFI_ERROR (Status);

  FileGuid = PcdGetPtr (PcdAndroidBootFile);
  ASSERT (FileGuid != NULL);
  Status = CreatePlatformBootOptionFromGuid (
             FileGuid,
             L"Android Boot",
             &(*BootOptions)[2]
             );
  ASSERT_EFI_ERROR (Status);

  FileGuid = PcdGetPtr (PcdAndroidFastbootFile);
  ASSERT (FileGuid != NULL);
  Status = CreatePlatformBootOptionFromGuid (
             FileGuid,
             L"Android Fastboot",
             &(*BootOptions)[3]
             );
  ASSERT_EFI_ERROR (Status);
  (*BootKeys)[3].ScanCode = SCAN_NULL;
  (*BootKeys)[3].UnicodeChar = 'f';

  *BootCount = 4;

  return EFI_SUCCESS;
Error:
  FreePool (*BootOptions);
  return Status;
}

PLATFORM_BOOT_MANAGER_PROTOCOL mPlatformBootManager = {
  GetPlatformBootOptionsAndKeys
};

EFI_STATUS
EFIAPI
VirtualKeyboardRegister (
  IN VOID
  )
{
  EFI_STATUS           Status;

  Status = gBS->LocateProtocol (
                  &gEmbeddedGpioProtocolGuid,
                  NULL,
                  (VOID **) &mGpio
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
VirtualKeyboardReset (
  IN VOID
  )
{
  EFI_STATUS           Status;

  if (mGpio == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Configure GPIO68 as GPIO function
  //
  MmioWrite32 (0xe896c108, 0);
  Status = mGpio->Set (mGpio, DETECT_SW_FASTBOOT, GPIO_MODE_INPUT);
  return Status;
}

BOOLEAN
EFIAPI
VirtualKeyboardQuery (
  IN VIRTUAL_KBD_KEY             *VirtualKey
  )
{
  EFI_STATUS           Status;
  UINTN                Value = 0;

  if ((VirtualKey == NULL) || (mGpio == NULL)) {
    return FALSE;
  }
  if (MmioRead32 (ADB_REBOOT_ADDRESS) == ADB_REBOOT_BOOTLOADER) {
    goto Done;
  } else {
    Status = mGpio->Get (mGpio, DETECT_SW_FASTBOOT, &Value);
    if (EFI_ERROR (Status) || (Value != 0)) {
      return FALSE;
    }
  }
Done:
  VirtualKey->Signature = VIRTUAL_KEYBOARD_KEY_SIGNATURE;
  VirtualKey->Key.ScanCode = SCAN_NULL;
  VirtualKey->Key.UnicodeChar = L'f';
  return TRUE;
}

EFI_STATUS
EFIAPI
VirtualKeyboardClear (
  IN VIRTUAL_KBD_KEY            *VirtualKey
  )
{
  if (VirtualKey == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (MmioRead32 (ADB_REBOOT_ADDRESS) == ADB_REBOOT_BOOTLOADER) {
    MmioWrite32 (ADB_REBOOT_ADDRESS, ADB_REBOOT_NONE);
    WriteBackInvalidateDataCacheRange ((VOID *)ADB_REBOOT_ADDRESS, 4);
  }
  return EFI_SUCCESS;
}

PLATFORM_VIRTUAL_KBD_PROTOCOL mVirtualKeyboard = {
  VirtualKeyboardRegister,
  VirtualKeyboardReset,
  VirtualKeyboardQuery,
  VirtualKeyboardClear
};

EFI_STATUS
EFIAPI
HiKey960EntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;
  EFI_EVENT             EndOfDxeEvent;

  InitPeripherals ();

  //
  // Create an event belonging to the "gEfiEndOfDxeEventGroupGuid" group.
  // The "OnEndOfDxe()" function is declared as the call back function.
  // It will be called at the end of the DXE phase when an event of the
  // same group is signalled to inform about the end of the DXE phase.
  // Install the INSTALL_FDT_PROTOCOL protocol.
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  OnEndOfDxe,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gPlatformVirtualKeyboardProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mVirtualKeyboard
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gPlatformBootManagerProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPlatformBootManager
                  );
  return Status;
}
