/** @file
*
*  Copyright (c) 2021, Rockchip Limited. All rights reserved.
*  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>
#include <Library/ArmLib.h>
#include <Library/CacheMaintenanceLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/HiiLib.h>
#include <Protocol/FormBrowserEx2.h>
#include <Library/UefiLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>
#include <Library/CruLib.h>
#include <Library/GpioLib.h>
#include <Library/RK806.h>
#include <Library/Rk3588Pcie.h>
#include <VarStoreData.h>
#include <Soc.h>
#include <RK3588RegsPeri.h>

#include "RK3588Dxe.h"
#include "RK3588DxeFormSetGuid.h"
#include "CpuPerformance.h"
#include "ComboPhy.h"
#include "PciExpress30.h"
#include "ConfigTable.h"
#include "FanControl.h"
#include "UsbDpPhy.h"
#include "DebugSerialPort.h"
#include "Display.h"

extern UINT8  RK3588DxeHiiBin[];
extern UINT8  RK3588DxeStrings[];

typedef struct {
  VENDOR_DEVICE_PATH          VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL    End;
} HII_VENDOR_DEVICE_PATH;

STATIC HII_VENDOR_DEVICE_PATH  mVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    RK3588DXE_FORMSET_GUID
  },
  {
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8)(END_DEVICE_PATH_LENGTH),
      (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
    }
  }
};

#define SATA_CAP         0x0000
#define  SATA_CAP_SSS    BIT27
#define SATA_PI          0x000C
#define SATA_CMD         0x0118
#define  SATA_CMD_FBSCP  BIT22

static UINTN  AhciReg[3] = {
  0xFE210000,
  0xFE220000,
  0xFE230000,
};

STATIC
VOID
InstallSataDevices (
  VOID
  )
{
  UINT32  Index;
  UINT32  PcieSlotIndex;
  UINT32  ComPhyMode[] = {
    PcdGet32 (PcdComboPhy0Mode),
    PcdGet32 (PcdComboPhy1Mode),
    PcdGet32 (PcdComboPhy2Mode)
  };

  for (Index = 0; Index < ARRAY_SIZE (ComPhyMode); Index++) {
    if (ComPhyMode[Index] == COMBO_PHY_MODE_SATA) {
      /* Enable power at the M.2 PCIe/SATA slots */
      switch (Index) {
        case 0:
          PcieSlotIndex = PCIE_SEGMENT_PCIE20L2;
          break;
        case 1:
          PcieSlotIndex = PCIE_SEGMENT_PCIE20L0;
          break;
        case 2:
          PcieSlotIndex = PCIE_SEGMENT_PCIE20L1;
          break;
        default:
          ASSERT (FALSE);
          continue;
      }

      PcieIoInit (PcieSlotIndex);
      PciePowerEn (PcieSlotIndex, TRUE);

      /* Set port implemented flag */
      MmioWrite32 (AhciReg[Index] + SATA_PI, 0x1);

      /* Supports staggered spin-up */
      /* Disable for now, otherwise NetBSD seems to not pick up any drive? */
      MmioAndThenOr32 (AhciReg[Index] + SATA_CAP, ~SATA_CAP_SSS, 0);

      /* Supports FIS-based switching */
      MmioOr32 (AhciReg[Index] + SATA_CMD, SATA_CMD_FBSCP);

      RegisterNonDiscoverableMmioDevice (
        NonDiscoverableDeviceTypeAhci,
        NonDiscoverableDeviceDmaTypeNonCoherent,
        NULL,
        NULL,
        1,
        AhciReg[Index],
        SIZE_4KB
        );
    }
  }
}

//
// Kept so the hotkey help text can be fetched once the browser is up.
//
STATIC EFI_HII_HANDLE  mHiiHandle = NULL;

/**
  Make F10 save and reset rather than just save.

  Nearly every setting here is marked as requiring a reset before it takes
  effect, so saving one is almost always followed by rebooting. Bundling the
  two removes a step that was never really optional.

  The display engine registers its own F10 as plain Save, and the browser
  refuses to register a key that already exists, so the original registration
  has to be removed first. That has to happen after the display engine has
  been through, hence End of Dxe rather than driver start.

  RESET does not reboot by itself: it records that a reset is owed, which the
  firmware acts on when setup is left, after SUBMIT has written the settings
  and EXIT has closed the form.

**/
STATIC
VOID
EFIAPI
MakeF10SaveAndReset (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                              Status;
  EDKII_FORM_BROWSER_EXTENSION2_PROTOCOL  *FormBrowserEx2;
  EFI_INPUT_KEY                           HotKey;
  EFI_STRING                              Help;

  gBS->CloseEvent (Event);

  if (mHiiHandle == NULL) {
    return;
  }

  Status = gBS->LocateProtocol (
                  &gEdkiiFormBrowserEx2ProtocolGuid,
                  NULL,
                  (VOID **)&FormBrowserEx2
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // The footer lists hotkeys in registration order, so both have to come out
  // and go back in: rebinding F10 alone would leave it sitting after F9.
  //
  HotKey.UnicodeChar = CHAR_NULL;

  HotKey.ScanCode = SCAN_F10;
  FormBrowserEx2->RegisterHotKey (&HotKey, BROWSER_ACTION_UNREGISTER, 0, NULL);

  HotKey.ScanCode = SCAN_F9;
  FormBrowserEx2->RegisterHotKey (&HotKey, BROWSER_ACTION_UNREGISTER, 0, NULL);

  HotKey.ScanCode = SCAN_F10;
  Help            = HiiGetString (mHiiHandle, STRING_TOKEN (STR_SAVE_AND_RESET), NULL);
  if (Help != NULL) {
    Status = FormBrowserEx2->RegisterHotKey (
                               &HotKey,
                               BROWSER_ACTION_SUBMIT | BROWSER_ACTION_RESET | BROWSER_ACTION_EXIT,
                               0,
                               Help
                               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a: could not rebind F10 (%r)\n", __func__, Status));
    }

    FreePool (Help);
  }

  //
  // Put F9 back exactly as the display engine had it.
  //
  HotKey.ScanCode = SCAN_F9;
  Help            = HiiGetString (mHiiHandle, STRING_TOKEN (STR_RESET_TO_DEFAULTS), NULL);
  if (Help != NULL) {
    Status = FormBrowserEx2->RegisterHotKey (
                               &HotKey,
                               BROWSER_ACTION_DEFAULT,
                               EFI_HII_DEFAULT_CLASS_STANDARD,
                               Help
                               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a: could not restore F9 (%r)\n", __func__, Status));
    }

    FreePool (Help);
  }
}

STATIC
EFI_STATUS
EFIAPI
InstallHiiPages (
  VOID
  )
{
  EFI_STATUS      Status;
  EFI_HII_HANDLE  HiiHandle;
  EFI_HANDLE      DriverHandle;
  EFI_EVENT       EndOfDxeEvent;

  DriverHandle = NULL;
  Status       = gBS->InstallMultipleProtocolInterfaces (
                        &DriverHandle,
                        &gEfiDevicePathProtocolGuid,
                        &mVendorDevicePath,
                        NULL
                        );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HiiHandle = HiiAddPackages (
                &gRK3588DxeFormSetGuid,
                DriverHandle,
                RK3588DxeStrings,
                RK3588DxeHiiBin,
                NULL
                );

  if (HiiHandle == NULL) {
    gBS->UninstallMultipleProtocolInterfaces (
           DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &mVendorDevicePath,
           NULL
           );
    return EFI_OUT_OF_RESOURCES;
  }

  mHiiHandle = HiiHandle;

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  MakeF10SaveAndReset,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &EndOfDxeEvent
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "%a: could not hook End of Dxe (%r)\n", __func__, Status));
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
SetupVariables (
  VOID
  )
{
  SetupCpuPerfVariables ();
  SetupComboPhyVariables ();
  SetupPcie30Variables ();
  SetupConfigTableVariables ();
  SetupCoolingFanVariables ();
  SetupUsbDpPhyVariables ();
  SetupDebugSerialPortVariables ();
  SetupDisplayVariables ();

  return EFI_SUCCESS;
}

STATIC
VOID
EFIAPI
AfterApplyVariablesInit (
  VOID
  )
{
  InstallSataDevices ();
}

STATIC
VOID
EFIAPI
InstallConfigAppliedProtocol (
  VOID
  )
{
  EFI_HANDLE  Handle = NULL;
  EFI_STATUS  Status;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gRockchipPlatformConfigAppliedProtocolGuid,
                  NULL,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
}

STATIC
VOID
EFIAPI
ApplyVariables (
  VOID
  )
{
  ApplyCpuClockVariables ();
  ApplyComboPhyVariables ();
  ApplyPcie30Variables ();
  ApplyConfigTableVariables ();
  ApplyCoolingFanVariables ();
  ApplyUsbDpPhyVariables ();
  ApplyDebugSerialPortVariables ();
  ApplyDisplayVariables ();

  InstallConfigAppliedProtocol ();

  AfterApplyVariablesInit ();
}

STATIC
VOID
RK3588SetupAudio (
  IN VOID
  )
{
  // Source PLL, ACPI expects this exact rate.
  HAL_CRU_ClkSetFreq (PLL_AUPLL, 786432000);

  // Warning: Only enable I2S if present.
  // E.g. Enabling I2S1 on OPI5+ causes PCIe devices to disappear
  if (FixedPcdGetBool (PcdI2S0Supported)) {
    // Configure I2S0 (e.g. Orange Pi 5 Plus)
    GpioPinSetFunction (1, GPIO_PIN_PD4, 2); // i2s0_sdi0
    GpioPinSetFunction (1, GPIO_PIN_PC7, 1); // i2s0_sdo0

    GpioPinSetFunction (1, GPIO_PIN_PC5, 1); // i2s0_lrck
    GpioPinSetFunction (1, GPIO_PIN_PC3, 1); // i2s0_sclk
    GpioPinSetFunction (1, GPIO_PIN_PC2, 1); // i2s0_mclk

    HAL_CRU_ClkSetMux (CLK_I2S0_8CH_TX_SRC, 0x1); // clk_aupll_mux
    HAL_CRU_ClkSetMux (MCLK_I2S0_8CH_TX, 0x1);    // clk_i2s0_8ch_tx_frac
  }

  if (FixedPcdGetBool (PcdI2S1Supported)) {
    // Configure I2S1 (e.g. Orange Pi 5)
    GpioPinSetFunction (4, GPIO_PIN_PA6, 3); // i2s1m0_sdi1
    GpioPinSetFunction (4, GPIO_PIN_PB4, 3); // i2s1m0_sdo3

    GpioPinSetFunction (4, GPIO_PIN_PA2, 3); // i2s1m0_lrck
    GpioPinSetFunction (4, GPIO_PIN_PA1, 3); // i2s1m0_sclk
    GpioPinSetFunction (4, GPIO_PIN_PA0, 3); // i2s1m0_mclk

    // CLK_I2S1_8CH_TX_SRC fixed parent: PLL_CPLL
    HAL_CRU_ClkSetMux (MCLK_I2S1_8CH_TX, 0x1); // clk_i2s1_8ch_tx_frac
  }
}

EFI_STATUS
RK3588InitPeripherals (
  IN VOID
  )
{
  DEBUG ((DEBUG_INIT, "RK3588InitPeripherals: Entry\n"));

  RK3588SetupAudio ();

  Rk806Configure ();

  return EFI_SUCCESS;
}

STATIC VOID
SetFlashAttributeToUncache (
  VOID
  )
{
  EFI_STATUS                       Status;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR  desp = { 0 };

  Status = gDS->AddMemorySpace (
                  EfiGcdMemoryTypeMemoryMappedIo,
                  PcdGet64 (FspiBaseAddr),
                  SIZE_64KB,
                  EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RTC: Failed to add memory space Status = %r\n", Status));
    return;
  }

  Status = gDS->GetMemorySpaceDescriptor (PcdGet64 (FspiBaseAddr), &desp);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: GetMemorySpaceDescriptor failed = %x\n", __FUNCTION__, Status));
    return;
  }

  Status = gDS->SetMemorySpaceAttributes (
                  PcdGet64 (FspiBaseAddr),
                  SIZE_64KB,
                  EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to set memory attributes Status = %x\n", __FUNCTION__, Status));
  }

  Status = gDS->AddMemorySpace (
                  EfiGcdMemoryTypeMemoryMappedIo,
                  PcdGet64 (CruBaseAddr),
                  SIZE_64KB,
                  EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RTC: Failed to add memory space Status = %r\n", Status));
    return;
  }

  Status = gDS->GetMemorySpaceDescriptor (PcdGet64 (CruBaseAddr), &desp);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: GetMemorySpaceDescriptor failed = %x\n", __FUNCTION__, Status));
    return;
  }

  Status = gDS->SetMemorySpaceAttributes (
                  PcdGet64 (CruBaseAddr),
                  SIZE_64KB,
                  EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to set memory attributes Status = %x\n", __FUNCTION__, Status));
  }
}

STATIC
VOID
EFIAPI
OnEfiVariableWriteArchRegistrationEvent (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  EFI_STATUS  Status;
  EFI_HANDLE  *HandleBuffer;
  UINTN       NumProtocols;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiVariableWriteArchProtocolGuid,
                  NULL,
                  &NumProtocols,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      DEBUG ((DEBUG_WARN, "Couldn't locate gEfiVariableWriteArchProtocolGuid. Status=%r\n", Status));
    }

    return;
  }

  gBS->CloseEvent (Event);

  Status = SetupVariables ();
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Couldn't setup NV vars. Status=%r\n", Status));
  }

  ApplyVariables ();

  Status = InstallHiiPages ();
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Couldn't install RK3588Dxe HII pages. Status=%r\n", Status));
  }
}

VOID
EFIAPI
RK3588NotifyReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  //
  // We set CPU voltages late on first notification of the ReadyToBoot event because
  // the user should have a chance to dial settings back in case they've set values that are
  // too high and/or the power supply is insufficient. Setting them earlier may prevent this.
  // The default values provide enough performance for the UEFI environment, so there's
  // really no need to set them early.
  //

  DEBUG ((DEBUG_INFO, "%a: called. Configure CPU voltages once.\n", __FUNCTION__));

  ApplyCpuVoltageVariables ();

  gBS->CloseEvent (Event);
}

EFI_STATUS
EFIAPI
RK3588EntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS  Status;
  VOID        *EfiVariableArchRegistrationEvent;
  EFI_EVENT   Event;

  PlatformEarlyInit ();

  //
  // We actually depend on gEfiVariableWriteArchProtocolGuid but don't want to
  // delay the entire driver, so we create a notify event on protocol arrival instead
  // and set up the variables & HII data in the callback.
  //
  EfiCreateProtocolNotifyEvent (
    &gEfiVariableWriteArchProtocolGuid,
    TPL_CALLBACK,
    OnEfiVariableWriteArchRegistrationEvent,
    NULL,
    &EfiVariableArchRegistrationEvent
    );

  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             RK3588NotifyReadyToBoot,
             NULL,
             &Event
             );
  ASSERT_EFI_ERROR (Status);

  Status = RK3588InitPeripherals ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SetFlashAttributeToUncache ();

  return Status;
}
