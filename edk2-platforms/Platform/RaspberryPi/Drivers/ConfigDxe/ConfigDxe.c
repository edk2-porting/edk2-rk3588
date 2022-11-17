/** @file
 *
 *  Copyright (c) 2019 - 2020, ARM Limited. All rights reserved.
 *  Copyright (c) 2018 - 2020, Andrei Warkentin <andrey.warkentin@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Uefi.h>
#include <IndustryStandard/Bcm2711.h>
#include <IndustryStandard/Bcm2836.h>
#include <IndustryStandard/Bcm2836Gpio.h>
#include <IndustryStandard/RpiMbox.h>
#include <IndustryStandard/SerialPortConsoleRedirectionTable.h>
#include <IndustryStandard/RpiDebugPort2Table.h>
#include <UartSelection.h>

#include <Library/AcpiLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/GpioLib.h>
#include <Library/HiiLib.h>
#include <Library/IoLib.h>
#include <Library/NetLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/BcmGenetPlatformDevice.h>
#include <Protocol/RpiFirmware.h>
#include <ConfigVars.h>
#include "ConfigDxeFormSetGuid.h"
#include "ConfigDxe.h"

#define FREQ_1_MHZ 1000000

extern UINT8 ConfigDxeHiiBin[];
extern UINT8 ConfigDxeStrings[];

STATIC RASPBERRY_PI_FIRMWARE_PROTOCOL *mFwProtocol;
STATIC UINT32 mModelFamily = 0;
STATIC UINT32 mModelInstalledMB = 0;
STATIC UINT32 mModelRevision = 0;

STATIC EFI_MAC_ADDRESS  mMacAddress;

/*
 * The GUID inside Platform/RaspberryPi/RPi3/AcpiTables/AcpiTables.inf and
 * Platform/RaspberryPi/RPi4/AcpiTables/AcpiTables.inf _must_ match below.
 */
STATIC CONST EFI_GUID mAcpiTableFile = {
  0x7E374E25, 0x8E01, 0x4FEE, { 0x87, 0xf2, 0x39, 0x0C, 0x23, 0xC6, 0x06, 0xCD }
};

typedef struct {
  VENDOR_DEVICE_PATH VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL End;
} HII_VENDOR_DEVICE_PATH;

#pragma pack (1)
typedef struct {
  MAC_ADDR_DEVICE_PATH            MacAddrDP;
  EFI_DEVICE_PATH_PROTOCOL        End;
} GENET_DEVICE_PATH;

typedef struct {
  GENET_DEVICE_PATH                   DevicePath;
  BCM_GENET_PLATFORM_DEVICE_PROTOCOL  PlatformDevice;
} GENET_DEVICE;
#pragma pack ()

STATIC HII_VENDOR_DEVICE_PATH mVendorDevicePath = {
  {
    {
      HARDWARE_DEVICE_PATH,
      HW_VENDOR_DP,
      {
        (UINT8)(sizeof (VENDOR_DEVICE_PATH)),
        (UINT8)((sizeof (VENDOR_DEVICE_PATH)) >> 8)
      }
    },
    CONFIGDXE_FORM_SET_GUID
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

STATIC GENET_DEVICE mGenetDevice = {
  {
    {
      {
        MESSAGING_DEVICE_PATH,
        MSG_MAC_ADDR_DP,
        {
          (UINT8)(sizeof (MAC_ADDR_DEVICE_PATH)),
          (UINT8)((sizeof (MAC_ADDR_DEVICE_PATH)) >> 8)
        }
      },
      {{ 0 }},
      NET_IFTYPE_ETHERNET
    },
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
    GENET_BASE_ADDRESS,
    {{ 0 }}
  }
};


STATIC
VOID
EFIAPI
RegisterDevices (
  EFI_EVENT           Event,
  VOID                *Context
  )
{
  EFI_HANDLE  Handle;
  EFI_STATUS  Status;

  if (mModelFamily == 4) {
    DEBUG ((DEBUG_INFO, "GENET: MAC address %02X:%02X:%02X:%02X:%02X:%02X\n",
            mMacAddress.Addr[0], mMacAddress.Addr[1], mMacAddress.Addr[2],
            mMacAddress.Addr[3], mMacAddress.Addr[4], mMacAddress.Addr[5]));

    CopyMem (&mGenetDevice.DevicePath.MacAddrDP.MacAddress, mMacAddress.Addr,
      NET_ETHER_ADDR_LEN);
    CopyMem (&mGenetDevice.PlatformDevice.MacAddress, mMacAddress.Addr,
      NET_ETHER_ADDR_LEN);

    Handle = NULL;
    Status = gBS->InstallMultipleProtocolInterfaces (&Handle,
                    &gEfiDevicePathProtocolGuid,          &mGenetDevice.DevicePath,
                    &gBcmGenetPlatformDeviceProtocolGuid, &mGenetDevice.PlatformDevice,
                    NULL);
    ASSERT_EFI_ERROR (Status);
  }
}

STATIC EFI_STATUS
InstallHiiPages (
  VOID
  )
{
  EFI_STATUS     Status;
  EFI_HII_HANDLE HiiHandle;
  EFI_HANDLE     DriverHandle;

  DriverHandle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (&DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mVendorDevicePath,
                  NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HiiHandle = HiiAddPackages (&gConfigDxeFormSetGuid,
                DriverHandle,
                ConfigDxeStrings,
                ConfigDxeHiiBin,
                NULL);

  if (HiiHandle == NULL) {
    gBS->UninstallMultipleProtocolInterfaces (DriverHandle,
           &gEfiDevicePathProtocolGuid,
           &mVendorDevicePath,
           NULL);
    return EFI_OUT_OF_RESOURCES;
  }
  return EFI_SUCCESS;
}


STATIC EFI_STATUS
SetupVariables (
  VOID
  )
{
  UINTN      Size;
  UINT8      Var8;
  UINT32     Var32;
  CHAR16     AssetTagVar[ASSET_TAG_STR_STORAGE_SIZE] = L"";
  EFI_STATUS Status;

  /*
   * Create the vars with default value.
   * If we don't, forms won't be able to update.
   */

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"CpuClock",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCpuClock, PcdGet32 (PcdCpuClock));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"CustomCpuClock",
                             &gConfigDxeFormSetGuid,
                             NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdCustomCpuClock, PcdGet32 (PcdCustomCpuClock));
    ASSERT_EFI_ERROR (Status);
  }

  if (mModelFamily >= 4 && mModelInstalledMB > 3 * 1024) {
    /*
     * This allows changing PcdRamLimitTo3GB in forms.
     */
    Status = PcdSet32S (PcdRamMoreThan3GB, 1);
    ASSERT_EFI_ERROR (Status);

    Size = sizeof (UINT32);
    Status = gRT->GetVariable (L"RamLimitTo3GB",
                               &gConfigDxeFormSetGuid,
                               NULL, &Size, &Var32);
    if (EFI_ERROR (Status)) {
      Status = PcdSet32S (PcdRamLimitTo3GB, PcdGet32 (PcdRamLimitTo3GB));
      ASSERT_EFI_ERROR (Status);
    }
  } else {
    Status = PcdSet32S (PcdRamMoreThan3GB, 0);
    ASSERT_EFI_ERROR (Status);
    Status = PcdSet32S (PcdRamLimitTo3GB, 0);
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"SystemTableMode",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdSystemTableMode, PcdGet32 (PcdSystemTableMode));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"FanOnGpio",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdFanOnGpio, PcdGet32 (PcdFanOnGpio));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"FanTemp",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdFanTemp, PcdGet32 (PcdFanTemp));
    ASSERT_EFI_ERROR (Status);
  }

  if (mModelFamily >= 4) {
    if (((mModelRevision >> 4) & 0xFF) == 0x14) {
      /*
       * Enable PCIe by default on CM4
       */
      Status = PcdSet32S (PcdXhciPci, 2);
      ASSERT_EFI_ERROR (Status);
    } else {
      Size = sizeof (UINT32);
      Status = gRT->GetVariable (L"XhciPci",
                                 &gConfigDxeFormSetGuid,
                                 NULL, &Size, &Var32);
      if (EFI_ERROR (Status) || (Var32 == 0)) {
        /*
         * Enable XHCI by default
         */
        Status = PcdSet32S (PcdXhciPci, 0);
        ASSERT_EFI_ERROR (Status);
      } else {
        /*
         * Enable PCIe
         */
        Status = PcdSet32S (PcdXhciPci, 1);
        ASSERT_EFI_ERROR (Status);
      }
    }
  } else {
    /*
     * Disable PCIe and XHCI
     */
    Status = PcdSet32S (PcdXhciPci, 0);
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (AssetTagVar);
  Status = gRT->GetVariable (L"AssetTag",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, AssetTagVar);

  if (EFI_ERROR (Status)) {
    Status = gRT->SetVariable (
                    L"AssetTag",
                    &gConfigDxeFormSetGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    sizeof (AssetTagVar),
                    AssetTagVar
                    );
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"SdIsArasan",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdSdIsArasan, PcdGet32 (PcdSdIsArasan));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"MmcDisableMulti",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdMmcDisableMulti, PcdGet32 (PcdMmcDisableMulti));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"MmcForce1Bit",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdMmcForce1Bit, PcdGet32 (PcdMmcForce1Bit));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"MmcForceDefaultSpeed",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdMmcForceDefaultSpeed, PcdGet32 (PcdMmcForceDefaultSpeed));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"MmcSdDefaultSpeedMHz",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdMmcSdDefaultSpeedMHz, PcdGet32 (PcdMmcSdDefaultSpeedMHz));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"MmcSdHighSpeedMHz",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdMmcSdHighSpeedMHz, PcdGet32 (PcdMmcSdHighSpeedMHz));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"MmcEnableDma",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdMmcEnableDma, PcdGet32 (PcdMmcEnableDma));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"DebugEnableJTAG",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdDebugEnableJTAG, PcdGet32 (PcdDebugEnableJTAG));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT8);
  Status = gRT->GetVariable (L"DisplayEnableScaledVModes",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var8);
  if (EFI_ERROR (Status)) {
    Status = PcdSet8S (PcdDisplayEnableScaledVModes, PcdGet8 (PcdDisplayEnableScaledVModes));
    ASSERT_EFI_ERROR (Status);
  }

  Size = sizeof (UINT32);
  Status = gRT->GetVariable (L"DisplayEnableSShot",
                  &gConfigDxeFormSetGuid,
                  NULL, &Size, &Var32);
  if (EFI_ERROR (Status)) {
    Status = PcdSet32S (PcdDisplayEnableSShot, PcdGet32 (PcdDisplayEnableSShot));
    ASSERT_EFI_ERROR (Status);
  }

  if (mModelFamily == 4) {
    //
    // Get the MAC address from the firmware.
    //
    Status = mFwProtocol->GetMacAddress (mMacAddress.Addr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "%a: failed to retrieve MAC address\n", __FUNCTION__));
    }
  }

  return EFI_SUCCESS;
}


STATIC VOID
ApplyVariables (
  VOID
  )
{
  UINTN Gpio34Group;
  UINTN Gpio48Group;
  EFI_STATUS Status;
  UINT32 CpuClock = PcdGet32 (PcdCpuClock);
  UINT32 CustomCpuClock = PcdGet32 (PcdCustomCpuClock);
  UINT32 Rate = 0;
  UINT32 FanOnGpio = PcdGet32 (PcdFanOnGpio);

  switch (CpuClock) {
  case CHIPSET_CPU_CLOCK_LOW:
    Rate = FixedPcdGet32 (PcdCpuLowSpeedMHz) * FREQ_1_MHZ;
    break;
  case CHIPSET_CPU_CLOCK_DEFAULT:
    /*
     * What the Raspberry Pi Foundation calls "max clock rate" is really the default value
     * from: https://www.raspberrypi.org/documentation/configuration/config-txt/overclocking.md
     */
    Status = mFwProtocol->GetMaxClockRate (RPI_MBOX_CLOCK_RATE_ARM, &Rate);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "Couldn't read default CPU speed %r\n", Status));
    }
    break;
  case CHIPSET_CPU_CLOCK_MAX:
    Rate = FixedPcdGet32 (PcdCpuMaxSpeedMHz) * FREQ_1_MHZ;
    break;
  case CHIPSET_CPU_CLOCK_CUSTOM:
    Rate = CustomCpuClock * FREQ_1_MHZ;
    break;
  }

  if (Rate != 0) {
    DEBUG ((DEBUG_INFO, "Setting CPU speed to %u MHz\n", Rate / FREQ_1_MHZ));
    Status = mFwProtocol->SetClockRate (RPI_MBOX_CLOCK_RATE_ARM, Rate, 1);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "Couldn't set the CPU speed: %r\n", Status));
    } else {
      Status = PcdSet32S (PcdCustomCpuClock, Rate / FREQ_1_MHZ);
      ASSERT_EFI_ERROR (Status);
    }
  }

  Status = mFwProtocol->GetClockRate (RPI_MBOX_CLOCK_RATE_ARM, &Rate);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Couldn't get the CPU speed: %r\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "Current CPU speed is %u MHz\n", Rate / FREQ_1_MHZ));
  }

  if (mModelFamily >= 4 && PcdGet32 (PcdRamMoreThan3GB) != 0 &&
      PcdGet32 (PcdRamLimitTo3GB) == 0) {
    UINT64 SystemMemorySize;
    UINT64 SystemMemorySizeBelow4GB;

    ASSERT (BCM2711_SOC_REGISTERS != 0);
    SystemMemorySize = (UINT64)mModelInstalledMB * SIZE_1MB;
    /*
     * Similar to how we compute the > 3 GB RAM segment's size in PlatformLib/
     * RaspberryPiMem.c, with some overlap protection for the Bcm2xxx register
     * spaces. SystemMemorySizeBelow4GB tracks the maximum memory below 4GB
     * line, factoring in the limit imposed by the SoC register range.
     */
    SystemMemorySizeBelow4GB = MIN (SystemMemorySize, 4UL * SIZE_1GB);
    SystemMemorySizeBelow4GB = MIN (SystemMemorySizeBelow4GB, BCM2836_SOC_REGISTERS);
    SystemMemorySizeBelow4GB = MIN (SystemMemorySizeBelow4GB, BCM2711_SOC_REGISTERS);

    ASSERT (SystemMemorySizeBelow4GB > 3UL * SIZE_1GB);

    Status = gDS->AddMemorySpace (EfiGcdMemoryTypeSystemMemory, 3UL * BASE_1GB,
                    SystemMemorySizeBelow4GB - (3UL * SIZE_1GB),
                    EFI_MEMORY_UC | EFI_MEMORY_WC | EFI_MEMORY_WT | EFI_MEMORY_WB);
    ASSERT_EFI_ERROR (Status);
    Status = gDS->SetMemorySpaceAttributes (3UL * BASE_1GB,
                    SystemMemorySizeBelow4GB - (3UL * SIZE_1GB), EFI_MEMORY_WB);
    ASSERT_EFI_ERROR (Status);

    if (SystemMemorySize > 4UL * SIZE_1GB) {
      //
      // Register any memory above 4GB.
      //
      Status = gDS->AddMemorySpace (EfiGcdMemoryTypeSystemMemory, 4UL * BASE_1GB,
                      SystemMemorySize - (4UL * SIZE_1GB),
                      EFI_MEMORY_UC | EFI_MEMORY_WC | EFI_MEMORY_WT | EFI_MEMORY_WB);
      ASSERT_EFI_ERROR (Status);
      Status = gDS->SetMemorySpaceAttributes (4UL * BASE_1GB,
                      SystemMemorySize - (4UL * SIZE_1GB), EFI_MEMORY_WB);
      ASSERT_EFI_ERROR (Status);
    }
  }

  if (mModelFamily == 3 || mModelFamily == 2) {
    /*
     * Pi 2B v1.2 / Pi 3: either Arasan or SdHost goes to SD card.
     *
     * Switching two groups around, so disable both first.
     *
     * No, I've not seen a problem, but having a group be
     * routed to two sets of pins seems like asking for trouble.
     */
    GpioPinFuncSet (34, GPIO_FSEL_INPUT);
    GpioPinFuncSet (35, GPIO_FSEL_INPUT);
    GpioPinFuncSet (36, GPIO_FSEL_INPUT);
    GpioPinFuncSet (37, GPIO_FSEL_INPUT);
    GpioPinFuncSet (38, GPIO_FSEL_INPUT);
    GpioPinFuncSet (39, GPIO_FSEL_INPUT);
    GpioPinFuncSet (48, GPIO_FSEL_INPUT);
    GpioPinFuncSet (49, GPIO_FSEL_INPUT);
    GpioPinFuncSet (50, GPIO_FSEL_INPUT);
    GpioPinFuncSet (51, GPIO_FSEL_INPUT);
    GpioPinFuncSet (52, GPIO_FSEL_INPUT);
    GpioPinFuncSet (53, GPIO_FSEL_INPUT);

    if (PcdGet32 (PcdSdIsArasan)) {
      DEBUG ((DEBUG_INFO, "Routing SD to Arasan\n"));
      Gpio48Group = GPIO_FSEL_ALT3;
      /*
       * Route SDIO to SdHost.
       */
      Gpio34Group = GPIO_FSEL_ALT0;
    } else {
      DEBUG ((DEBUG_INFO, "Routing SD to SdHost\n"));
      Gpio48Group = GPIO_FSEL_ALT0;
      /*
       * Route SDIO to Arasan.
       */
      Gpio34Group = GPIO_FSEL_ALT3;
    }
    GpioPinFuncSet (34, Gpio34Group);
    GpioPinFuncSet (35, Gpio34Group);
    GpioPinFuncSet (36, Gpio34Group);
    GpioPinFuncSet (37, Gpio34Group);
    GpioPinFuncSet (38, Gpio34Group);
    GpioPinFuncSet (39, Gpio34Group);
    GpioPinFuncSet (48, Gpio48Group);
    GpioPinFuncSet (49, Gpio48Group);
    GpioPinFuncSet (50, Gpio48Group);
    GpioPinFuncSet (51, Gpio48Group);
    GpioPinFuncSet (52, Gpio48Group);
    GpioPinFuncSet (53, Gpio48Group);

  } else if (mModelFamily == 4) {
    /*
     * Pi 4: either Arasan or eMMC2 goes to SD card.
     */
    if (PcdGet32 (PcdSdIsArasan)) {
      /*
       * WiFi disabled.
       */
      GpioPinFuncSet (34, GPIO_FSEL_INPUT);
      GpioPinFuncSet (35, GPIO_FSEL_INPUT);
      GpioPinFuncSet (36, GPIO_FSEL_INPUT);
      GpioPinFuncSet (37, GPIO_FSEL_INPUT);
      GpioPinFuncSet (38, GPIO_FSEL_INPUT);
      GpioPinFuncSet (39, GPIO_FSEL_INPUT);
      /*
       * SD card pins go to Arasan.
       */
      MmioOr32 (GPIO_BASE_ADDRESS + 0xD0, BIT1);
    } else {
      /*
       * SD card pins back to eMMC2.
       */
      MmioAnd32 (GPIO_BASE_ADDRESS + 0xD0, ~BIT1);
      /*
       * WiFi back to Arasan.
       */
      GpioPinFuncSet (34, GPIO_FSEL_ALT3);
      GpioPinFuncSet (35, GPIO_FSEL_ALT3);
      GpioPinFuncSet (36, GPIO_FSEL_ALT3);
      GpioPinFuncSet (37, GPIO_FSEL_ALT3);
      GpioPinFuncSet (38, GPIO_FSEL_ALT3);
      GpioPinFuncSet (39, GPIO_FSEL_ALT3);

      Status = mFwProtocol->SetPowerState (RPI_MBOX_POWER_STATE_SDHCI,
                                           TRUE, TRUE); //SD on with wait
      Status = mFwProtocol->SetGpioConfig (RPI_EXP_GPIO_SD_VOLT,
                                           RPI_EXP_GPIO_DIR_OUT, TRUE); //3.3v
      Status = mFwProtocol->SetClockState (RPI_MBOX_CLOCK_RATE_EMMC2, TRUE);
      Status = mFwProtocol->SetClockState (RPI_MBOX_CLOCK_RATE_EMMC, TRUE);
    }
  } else {
    DEBUG ((DEBUG_ERROR, "Model Family %d not supported...\n", mModelFamily));
  }

  /*
   * JTAG pin    JTAG sig    GPIO      Mode    Header pin
   * 1           VREF        N/A               1
   * 3           nTRST       GPIO22    ALT4    15
   * 4           GND         N/A               9
   * 5           TDI         GPIO26    ALT4    37
   * 7           TMS         GPIO27    ALT4    13
   * 9           TCK         GPIO25    ALT4    22
   * 11          RTCK        GPIO23    ALT4    16
   * 13          TDO         GPIO24    ALT4    18
   */
  if (PcdGet32 (PcdDebugEnableJTAG)) {
    GpioPinFuncSet (22, GPIO_FSEL_ALT4);
    GpioPinFuncSet (26, GPIO_FSEL_ALT4);
    GpioPinFuncSet (27, GPIO_FSEL_ALT4);
    GpioPinFuncSet (25, GPIO_FSEL_ALT4);
    GpioPinFuncSet (23, GPIO_FSEL_ALT4);
    GpioPinFuncSet (24, GPIO_FSEL_ALT4);
  } else {
    GpioPinFuncSet (22, GPIO_FSEL_INPUT);
    GpioPinFuncSet (26, GPIO_FSEL_INPUT);
    GpioPinFuncSet (27, GPIO_FSEL_INPUT);
    GpioPinFuncSet (25, GPIO_FSEL_INPUT);
    GpioPinFuncSet (23, GPIO_FSEL_INPUT);
    GpioPinFuncSet (24, GPIO_FSEL_INPUT);
  }

  if (FanOnGpio) {
    DEBUG ((DEBUG_INFO, "Fan enabled on GPIO %d\n", FanOnGpio));
    GpioPinFuncSet (FanOnGpio, GPIO_FSEL_OUTPUT);
  }

  //
  // Fake the CTS signal as we don't support HW flow control yet.
  // Pin 31 must be held LOW so that we can talk to the BT chip
  // without flow control
  //
  GpioPinFuncSet (31, GPIO_FSEL_OUTPUT);
  GpioPinConfigure (31, CLEAR_GPIO);

  //
  // Bluetooth pin muxing
  //
  if ((PcdGet32 (PcdUartInUse) == PL011_UART_IN_USE)) {
    DEBUG ((DEBUG_INFO, "Enable Bluetooth over MiniUART\n"));
    GpioPinFuncSet (32, GPIO_FSEL_ALT5);
    GpioPinFuncSet (33, GPIO_FSEL_ALT5);
  } else {
    DEBUG ((DEBUG_INFO, "Enable Bluetooth over PL011 UART\n"));
    GpioPinFuncSet (32, GPIO_FSEL_ALT3);
    GpioPinFuncSet (33, GPIO_FSEL_ALT3);
  }

}


typedef struct {
  CHAR8 Name[4];
  UINTN PcdToken;
} AML_NAME_OP_REPLACE;

typedef struct {
  UINT64                      OemTableId;
  UINTN                       PcdToken;
  UINTN                       PcdTokenNot;
  CONST AML_NAME_OP_REPLACE   *SdtNameOpReplace;
} NAMESPACE_TABLES;

#define SSDT_PATTERN_LEN 5
#define AML_NAMEOP_8   0x0A
#define AML_NAMEOP_16  0x0B
#define AML_NAMEOP_32  0x0C
#define AML_NAMEOP_STR 0x0D
//
// Scan the given namespace table (DSDT/SSDT) for AML NameOps
// listed in the NameOpReplace structure. If one is found then
// update the value in the table from the specified Pcd
//
// This allows us to have conditionals in AML controlled
// by options in the BDS or detected during firmware bootstrap.
// We could extend this concept for strings/etc but due to len
// variations its probably easier to encode the strings
// in the ASL and pick the correct one based off a variable.
//
STATIC
VOID
UpdateSdtNameOps (
  EFI_ACPI_DESCRIPTION_HEADER  *AcpiTable,
  CONST AML_NAME_OP_REPLACE    *NameOpReplace
  )
{
  UINTN  Idx;
  UINTN  Index;
  CHAR8  Pattern[SSDT_PATTERN_LEN];
  UINTN  PcdVal;
  UINT8  *SdtPtr;
  UINT32 SdtSize;

  SdtSize = AcpiTable->Length;

  if (SdtSize > 0) {
    SdtPtr = (UINT8 *)AcpiTable;

    for (Idx = 0; NameOpReplace && NameOpReplace[Idx].PcdToken; Idx++) {
      //
      // Do a single NameOp variable replacement these are of the
      // form 08 XXXX SIZE VAL, where SIZE is 0A=byte, 0B=word, 0C=dword
      // and XXXX is the name and VAL is the value
      //
      Pattern[0] = 0x08;
      Pattern[1] = NameOpReplace[Idx].Name[0];
      Pattern[2] = NameOpReplace[Idx].Name[1];
      Pattern[3] = NameOpReplace[Idx].Name[2];
      Pattern[4] = NameOpReplace[Idx].Name[3];

      for (Index = 0; Index < (SdtSize - SSDT_PATTERN_LEN); Index++) {
        if (CompareMem (SdtPtr + Index, Pattern, SSDT_PATTERN_LEN) == 0) {
          PcdVal = LibPcdGet32 (NameOpReplace[Idx].PcdToken);
          switch (SdtPtr[Index + SSDT_PATTERN_LEN]) {
          case AML_NAMEOP_32:
            SdtPtr[Index + SSDT_PATTERN_LEN + 4] = (PcdVal >> 24) & 0xFF;
            SdtPtr[Index + SSDT_PATTERN_LEN + 3] = (PcdVal >> 16) & 0xFF;
            // Fallthrough
          case AML_NAMEOP_16:
            SdtPtr[Index + SSDT_PATTERN_LEN + 2] = (PcdVal >> 8) & 0xFF;
            // Fallthrough
          case AML_NAMEOP_8:
            SdtPtr[Index + SSDT_PATTERN_LEN + 1] = PcdVal & 0xFF;
            break;
          case 0:
          case 1:
            SdtPtr[Index + SSDT_PATTERN_LEN + 1] = !!PcdVal;
            break;
          case AML_NAMEOP_STR:
            //
            // If the string val is added to the NameOpReplace, we can
            // dynamically update things like _HID too as long as the
            // string length matches.
            //
            break;
          }
          break;
        }
      }
    }
  }
}


STATIC
BOOLEAN
VerifyUpdateTable (
  IN  EFI_ACPI_DESCRIPTION_HEADER   *AcpiHeader,
  IN  CONST NAMESPACE_TABLES        *SdtTable
  )
{
  BOOLEAN Result;

  Result = TRUE;
  if (SdtTable->PcdToken && !LibPcdGet32 (SdtTable->PcdToken)) {
    Result = FALSE;
  }
  if (SdtTable->PcdTokenNot && LibPcdGet32 (SdtTable->PcdTokenNot)) {
    Result = FALSE;
  }
  if (Result && SdtTable->SdtNameOpReplace) {
    UpdateSdtNameOps (AcpiHeader, SdtTable->SdtNameOpReplace);
  }

  return Result;
}

STATIC CONST AML_NAME_OP_REPLACE SsdtNameOpReplace[] = {
  { "GIOP", PcdToken (PcdFanOnGpio) },
  { "FTMP", PcdToken (PcdFanTemp) },
  { }
};

STATIC CONST AML_NAME_OP_REPLACE SsdtEmmcNameOpReplace[] = {
  { "SDMA", PcdToken (PcdMmcEnableDma) },
  { }
};

STATIC CONST AML_NAME_OP_REPLACE DsdtNameOpReplace[] = {
  { "URIU", PcdToken (PcdUartInUse) },
  { }
};

STATIC CONST NAMESPACE_TABLES SdtTables[] = {
  {
    SIGNATURE_64 ('R', 'P', 'I', 'T', 'H', 'F', 'A', 'N'),
    PcdToken(PcdFanOnGpio),
    0,
    SsdtNameOpReplace
  },
  {
    SIGNATURE_64 ('R', 'P', 'I', '4', 'E', 'M', 'M', 'C'),
    0,
    PcdToken(PcdSdIsArasan),
    SsdtEmmcNameOpReplace
  },
#if (RPI_MODEL == 4)
  {
    SIGNATURE_64 ('R', 'P', 'I', '4', 'X', 'H', 'C', 'I'),
    0,
    PcdToken(PcdXhciPci),
    NULL
  },
  {
    SIGNATURE_64 ('R', 'P', 'I', '4', 'P', 'C', 'I', 'E'),
    PcdToken(PcdXhciPci),
    0,
    NULL
  },
#endif
  { // DSDT
    SIGNATURE_64 ('R', 'P', 'I', 0, 0, 0, 0, 0),
    0,
    0,
    DsdtNameOpReplace
  },
  { }
};

//
// Monitor the ACPI tables being installed and when
// a DSDT/SSDT is detected validate that we want to
// install it, and if so update any "NameOp" defined
// variables contained in the table from PCD values
//
STATIC
BOOLEAN
HandleDynamicNamespace (
  IN  EFI_ACPI_DESCRIPTION_HEADER   *AcpiHeader
  )
{
  UINTN Tables;

  EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE *SpcrTable;
  DBG2_TABLE                                     *Dbg2Table;

  switch (AcpiHeader->Signature) {
  case SIGNATURE_32 ('D', 'S', 'D', 'T'):
  case SIGNATURE_32 ('S', 'S', 'D', 'T'):
    for (Tables = 0; SdtTables[Tables].OemTableId; Tables++) {
      if (AcpiHeader->OemTableId == SdtTables[Tables].OemTableId) {
        return VerifyUpdateTable (AcpiHeader, &SdtTables[Tables]);
      }
    }
    DEBUG ((DEBUG_ERROR, "Found namespace table not in table list.\n"));
    return FALSE;

  case SIGNATURE_32 ('I', 'O', 'R', 'T'):
    // only enable the IORT on machines with >3G and no limit
    // to avoid problems with rhel/centos and other older OSs
    if (PcdGet32 (PcdRamLimitTo3GB) || !PcdGet32 (PcdRamMoreThan3GB)) {
      return FALSE;
    }
    return TRUE;

  case SIGNATURE_32 ('S', 'P', 'C', 'R'):
    SpcrTable = (EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE *)AcpiHeader;
    if ((PcdGet32 (PcdUartInUse) == PL011_UART_IN_USE) &&
        (SpcrTable->InterfaceType == EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_ARM_PL011_UART)) {
      return TRUE;
    } else if ((PcdGet32 (PcdUartInUse) == MINI_UART_IN_USE) &&
               (SpcrTable->InterfaceType == EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_INTERFACE_TYPE_BCM2835_UART)) {
      return TRUE;
    }
    return FALSE;

  case SIGNATURE_32 ('D', 'B', 'G', '2'):
    Dbg2Table = (DBG2_TABLE *)AcpiHeader;
    if ((PcdGet32 (PcdUartInUse) == PL011_UART_IN_USE) &&
        (Dbg2Table->Dbg2DeviceInfo[0].Dbg2Device.PortSubtype == EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_ARM_PL011_UART)) {
      return TRUE;
    } else if ((PcdGet32 (PcdUartInUse) == MINI_UART_IN_USE) &&
               (Dbg2Table->Dbg2DeviceInfo[0].Dbg2Device.PortSubtype == EFI_ACPI_DBG2_PORT_SUBTYPE_SERIAL_BCM2835_UART)) {
      return TRUE;
    }
    return FALSE;
  }

  return TRUE;
}


EFI_STATUS
EFIAPI
ConfigInitialize (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                      Status;
  EFI_EVENT                       EndOfDxeEvent;

  if ((MmioRead32(GPIO_GPFSEL1) & GPFSEL1_UART_MASK) == PL011_UART_IN_USE_REG_VALUE) {
    PcdSet32S (PcdUartInUse, PL011_UART_IN_USE);
  } else if ((MmioRead32(GPIO_GPFSEL1) & GPFSEL1_UART_MASK) == MINI_UART_IN_USE_REG_VALUE) {
    PcdSet32S (PcdUartInUse, MINI_UART_IN_USE);
  }

  Status = gBS->LocateProtocol (&gRaspberryPiFirmwareProtocolGuid,
                  NULL, (VOID**)&mFwProtocol);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = mFwProtocol->GetModelFamily (&mModelFamily);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Couldn't get the Raspberry Pi model family: %r\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "Current Raspberry Pi model family is %d\n", mModelFamily));
  }

  Status = mFwProtocol->GetModelInstalledMB (&mModelInstalledMB);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Couldn't get the Raspberry Pi installed RAM size: %r\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "Current Raspberry Pi installed RAM size is %d MB\n", mModelInstalledMB));
  }

  Status = mFwProtocol->GetModelRevision (&mModelRevision);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Couldn't get the Raspberry Pi revision: %r\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "Current Raspberry Pi revision %x\n", mModelRevision));
  }

  Status = SetupVariables ();
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Couldn't not setup NV vars: %r\n", Status));
  }

  ApplyVariables ();
  Status = gBS->InstallProtocolInterface (&ImageHandle,
                  &gRaspberryPiConfigAppliedProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL);
  ASSERT_EFI_ERROR (Status);

  Status = InstallHiiPages ();
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Couldn't install ConfigDxe configuration pages: %r\n", Status));
  }

  if (PcdGet32 (PcdSystemTableMode) == SYSTEM_TABLE_MODE_ACPI ||
      PcdGet32 (PcdSystemTableMode) == SYSTEM_TABLE_MODE_BOTH) {
     Status = LocateAndInstallAcpiFromFvConditional (&mAcpiTableFile,
                                                     &HandleDynamicNamespace);
     ASSERT_EFI_ERROR (Status);
  }

  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_NOTIFY, RegisterDevices,
                  NULL, &gEfiEndOfDxeEventGroupGuid, &EndOfDxeEvent);
  ASSERT_EFI_ERROR (Status);


  if (mModelFamily == 4) {
    RegisterXhciQuirkHandler (mFwProtocol);
  }

  return EFI_SUCCESS;
}
