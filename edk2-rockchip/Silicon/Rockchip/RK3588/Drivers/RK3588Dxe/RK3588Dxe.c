/** @file
*
*  Copyright (c) 2021, Rockchip Limited. All rights reserved.
*  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Uefi.h>
#include <Protocol/MemoryAttribute.h>
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
#include <Library/UefiLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>
#include <Library/CruLib.h>
#include <Library/RK806.h>
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

extern UINT8 RK3588DxeHiiBin[];
extern UINT8 RK3588DxeStrings[];

typedef struct {
  VENDOR_DEVICE_PATH VendorDevicePath;
  EFI_DEVICE_PATH_PROTOCOL End;
} HII_VENDOR_DEVICE_PATH;

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

#define SATA_CAP            0x0000
#define  SATA_CAP_SSS       BIT27
#define SATA_PI             0x000C
#define SATA_CMD            0x0118
#define  SATA_CMD_FBSCP     BIT22

static UINTN AhciReg[3] = {
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
  UINT32 Index;
  UINT32 ComPhyMode[] = { PcdGet32 (PcdComboPhy0Mode),
                          PcdGet32 (PcdComboPhy1Mode),
                          PcdGet32 (PcdComboPhy2Mode) };

  for (Index = 0; Index < ARRAY_SIZE (ComPhyMode); Index++) {
    if (ComPhyMode[Index] == COMBO_PHY_MODE_SATA) {
      /* Set port implemented flag */
      MmioWrite32 (AhciReg[Index] + SATA_PI, 0x1);

      /* Supports staggered spin-up */
      /* Disable for now, otherwise NetBSD seems to not pick up any drive? */
      MmioAndThenOr32 (AhciReg[Index] + SATA_CAP, ~SATA_CAP_SSS, 0);

      /* Supports FIS-based switching */
      MmioOr32 (AhciReg[Index] + SATA_CMD, SATA_CMD_FBSCP);

      RegisterNonDiscoverableMmioDevice (NonDiscoverableDeviceTypeAhci,
                                         NonDiscoverableDeviceDmaTypeNonCoherent,
                                         NULL,
                                         NULL,
                                         1,
                                         AhciReg[Index], SIZE_4KB);
    }
  }
}

STATIC
EFI_STATUS
EFIAPI
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

  HiiHandle = HiiAddPackages (&gRK3588DxeFormSetGuid,
                DriverHandle,
                RK3588DxeStrings,
                RK3588DxeHiiBin,
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
  EFI_HANDLE Handle = NULL;
  EFI_STATUS Status;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gRockchipPlatformConfigAppliedProtocolGuid,
                  NULL,
                  NULL);
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

  InstallConfigAppliedProtocol ();

  AfterApplyVariablesInit ();
}

STATIC
VOID
UartInit (
  IN VOID
  )
{
  //UINT32     Val;

  DEBUG((DEBUG_INIT, "RK3588InitPeripherals: UartInit()\n"));
  /* make UART1 out of reset */
  //MmioWrite32 (CRU_BASE + SC_PERIPH_RSTDIS3, PERIPH_RST3_UART1);
  //MmioWrite32 (CRU_BASE + SC_PERIPH_CLKEN3, PERIPH_RST3_UART1);
  /* make UART2 out of reset */
  //MmioWrite32 (CRU_BASE + SC_PERIPH_RSTDIS3, PERIPH_RST3_UART2);
  //MmioWrite32 (CRU_BASE + SC_PERIPH_CLKEN3, PERIPH_RST3_UART2);
  /* make UART3 out of reset */
  //MmioWrite32 (CRU_BASE + SC_PERIPH_RSTDIS3, PERIPH_RST3_UART3);
  //MmioWrite32 (CRU_BASE + SC_PERIPH_CLKEN3, PERIPH_RST3_UART3);
  /* make UART4 out of reset */
  //MmioWrite32 (CRU_BASE + SC_PERIPH_RSTDIS3, PERIPH_RST3_UART4);
  //MmioWrite32 (CRU_BASE + SC_PERIPH_CLKEN3, PERIPH_RST3_UART4);

  /* make DW_MMC2 out of reset */
  //MmioWrite32 (CRU_BASE + SC_PERIPH_RSTDIS0, PERIPH_RST0_MMC2);

  /* enable clock for BT/WIFI */
  //Val = MmioRead32 (PMUSSI_ONOFF8_REG) | PMUSSI_ONOFF8_EN_32KB;
  //MmioWrite32 (PMUSSI_ONOFF8_REG, Val);
}

STATIC
VOID
MtcmosInit (
  IN VOID
  )
{
  //UINT32     Data;

  DEBUG((DEBUG_INIT, "RK3588InitPeripherals: MtcmosInit()\n"));
  /* enable MTCMOS for GPU */
  //MmioWrite32 (AO_CTRL_BASE + SC_PW_MTCMOS_EN0, PW_EN0_G3D);
  //do {
  //  Data = MmioRead32 (AO_CTRL_BASE + SC_PW_MTCMOS_ACK_STAT0);
  //} while ((Data & PW_EN0_G3D) == 0);
}

EFI_STATUS
RK3588InitPeripherals (
  IN VOID
  )
{
  //UINT32     Data, Bits;

  DEBUG((DEBUG_INIT, "RK3588InitPeripherals: Entry\n"));

  /* make I2C0/I2C1/I2C2/SPI0 out of reset */
  //Bits = PERIPH_RST3_I2C0 | PERIPH_RST3_I2C1 | PERIPH_RST3_I2C2 | PERIPH_RST3_SSP;
  //MmioWrite32 (CRU_BASE + SC_PERIPH_RSTDIS3, Bits);

  //do {
  //  Data = MmioRead32 (CRU_BASE + SC_PERIPH_RSTSTAT3);
  //} while (Data & Bits);

  // UartInit ();

  /* MTCMOS -- Multi-threshold CMOS */
  // MtcmosInit ();

  Rk806Configure();

  return EFI_SUCCESS;
}

STATIC VOID SetFlashAttributeToUncache(VOID)
{
  EFI_STATUS Status;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR desp = {0};

  Status = gDS->AddMemorySpace (
                     EfiGcdMemoryTypeMemoryMappedIo,
                     PcdGet64(FspiBaseAddr),
                     SIZE_64KB,
                     EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                     );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RTC: Failed to add memory space Status = %r\n", Status));
    return;
  }

  Status = gDS->GetMemorySpaceDescriptor(PcdGet64(FspiBaseAddr),&desp);
  if(EFI_ERROR(Status)){
    DEBUG ((DEBUG_ERROR, "%a: GetMemorySpaceDescriptor failed = %x\n", __FUNCTION__, Status));
    return;
  }

  Status = gDS->SetMemorySpaceAttributes (
                     PcdGet64(FspiBaseAddr),
                     SIZE_64KB,
                     EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                     );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to set memory attributes Status = %x\n",__FUNCTION__, Status));
  }

  Status = gDS->AddMemorySpace (
                     EfiGcdMemoryTypeMemoryMappedIo,
                     PcdGet64(CruBaseAddr),
                     SIZE_64KB,
                     EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                     );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "RTC: Failed to add memory space Status = %r\n", Status));
    return;
  }

  Status = gDS->GetMemorySpaceDescriptor(PcdGet64(CruBaseAddr),&desp);
  if(EFI_ERROR(Status)){
    DEBUG ((DEBUG_ERROR, "%a: GetMemorySpaceDescriptor failed = %x\n", __FUNCTION__, Status));
    return;
  }

  Status = gDS->SetMemorySpaceAttributes (
                     PcdGet64(CruBaseAddr),
                     SIZE_64KB,
                     EFI_MEMORY_UC | EFI_MEMORY_RUNTIME
                     );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to set memory attributes Status = %x\n",__FUNCTION__, Status));
  }
}

STATIC
VOID
EFIAPI
OnEfiVariableWriteArchRegistrationEvent (
  IN  EFI_EVENT   Event,
  IN  VOID        *Context
  )
{
  EFI_STATUS                  Status;
  EFI_HANDLE                  *HandleBuffer;
  UINTN                       NumProtocols;

  Status = gBS->LocateHandleBuffer (ByProtocol,
                                    &gEfiVariableWriteArchProtocolGuid,
                                    NULL,
                                    &NumProtocols,
                                    &HandleBuffer);
  if (EFI_ERROR(Status)) {
    if (Status != EFI_NOT_FOUND) {
        DEBUG((DEBUG_WARN, "Couldn't locate gEfiVariableWriteArchProtocolGuid. Status=%r\n", Status));
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


/**
  This function uninstalls the recently added EFI_MEMORY_ATTRIBUTE_PROTOCOL
  to workaround older versions of OS loaders/shims using it incorrectly and
  throwing a Synchronous Exception.

  See:
    - https://github.com/microsoft/mu_silicon_arm_tiano/issues/124
    - https://edk2.groups.io/g/devel/topic/99631663
**/
STATIC
VOID
EFIAPI
UninstallMemoryAttributeProtocol (
  VOID
  )
{
  EFI_STATUS                      Status;
  EFI_HANDLE                      *Handles;
  UINTN                           HandleCount;
  EFI_MEMORY_ATTRIBUTE_PROTOCOL   *MemoryAttributeProtocol;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiMemoryAttributeProtocolGuid,
                  NULL,
                  &HandleCount,
                  &Handles
                  );
  ASSERT_EFI_ERROR (Status);
  ASSERT (HandleCount == 1);

  Status = gBS->HandleProtocol (
                  Handles[0],
                  &gEfiMemoryAttributeProtocolGuid,
                  (VOID **)&MemoryAttributeProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->UninstallMultipleProtocolInterfaces (
                  Handles[0],
                  &gEfiMemoryAttributeProtocolGuid,
                  MemoryAttributeProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  gBS->FreePool (Handles);
}

STATIC
VOID
EFIAPI
RK3588NotifyEndOfDxe (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  gBS->CloseEvent (Event);

  UninstallMemoryAttributeProtocol ();
}

EFI_STATUS
EFIAPI
RK3588EntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;
  VOID                  *EfiVariableArchRegistrationEvent;
  EFI_EVENT             Event;

  PlatformEarlyInit();

  //
  // We actually depend on gEfiVariableWriteArchProtocolGuid but don't want to
  // delay the entire driver, so we create a notify event on protocol arrival instead
  // and set up the variables & HII data in the callback.
  //
  EfiCreateProtocolNotifyEvent (&gEfiVariableWriteArchProtocolGuid,
                                TPL_CALLBACK,
                                OnEfiVariableWriteArchRegistrationEvent,
                                NULL,
                                &EfiVariableArchRegistrationEvent);

  Status = EfiCreateEventReadyToBootEx (TPL_CALLBACK,
                                        RK3588NotifyReadyToBoot,
                                        NULL,
                                        &Event);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    RK3588NotifyEndOfDxe,
                    NULL,
                    &gEfiEndOfDxeEventGroupGuid,
                    &Event);
  ASSERT_EFI_ERROR (Status);

  Status = RK3588InitPeripherals ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  SetFlashAttributeToUncache();

  return Status;
}
