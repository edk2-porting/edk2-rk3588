/** @file
*
*  Copyright (c) 2021, Rockchip Limited. All rights reserved.
*  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Base.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/GpioLib.h>
#include <Library/RK806.h>
#include <Library/Rk3588Pcie.h>
#include <Soc.h>
#include <VarStoreData.h>

#include <Protocol/KhadasMcu.h>

STATIC VOID                 *mKhadasMcuEventRegistration;
STATIC KHADAS_MCU_PROTOCOL  *mKhadasMcu;
STATIC UINT8                mTargetFanSpeed;

static struct regulator_init_data  rk806_init_data[] = {
  /* Master PMIC */
  RK8XX_VOLTAGE_INIT (MASTER_BUCK1,  750000),
  RK8XX_VOLTAGE_INIT (MASTER_BUCK3,  750000),
  RK8XX_VOLTAGE_INIT (MASTER_BUCK4,  750000),
  RK8XX_VOLTAGE_INIT (MASTER_BUCK5,  850000),
  // RK8XX_VOLTAGE_INIT(MASTER_BUCK6, 750000),
  RK8XX_VOLTAGE_INIT (MASTER_BUCK7,  2000000),
  RK8XX_VOLTAGE_INIT (MASTER_BUCK8,  3300000),
  RK8XX_VOLTAGE_INIT (MASTER_BUCK10, 1800000),

  RK8XX_VOLTAGE_INIT (MASTER_NLDO1,  750000),
  RK8XX_VOLTAGE_INIT (MASTER_NLDO2,  850000),
  RK8XX_VOLTAGE_INIT (MASTER_NLDO3,  750000),
  RK8XX_VOLTAGE_INIT (MASTER_NLDO4,  850000),
  RK8XX_VOLTAGE_INIT (MASTER_NLDO5,  750000),

  RK8XX_VOLTAGE_INIT (MASTER_PLDO1,  1800000),
  RK8XX_VOLTAGE_INIT (MASTER_PLDO2,  1800000),
  RK8XX_VOLTAGE_INIT (MASTER_PLDO3,  1200000),
  RK8XX_VOLTAGE_INIT (MASTER_PLDO4,  3300000),
  RK8XX_VOLTAGE_INIT (MASTER_PLDO5,  3300000),
  RK8XX_VOLTAGE_INIT (MASTER_PLDO6,  1800000),

  /* No dual PMICs on this platform */
};

VOID
EFIAPI
SdmmcIoMux (
  VOID
  )
{
  /* sdmmc0 iomux (microSD socket) */
  BUS_IOC->GPIO4D_IOMUX_SEL_L  = (0xFFFFUL << 16) | (0x1111); // SDMMC_D0,SDMMC_D1,SDMMC_D2,SDMMC_D3
  BUS_IOC->GPIO4D_IOMUX_SEL_H  = (0x00FFUL << 16) | (0x0011); // SDMMC_CLK,SDMMC_CMD
  PMU1_IOC->GPIO0A_IOMUX_SEL_H = (0x000FUL << 16) | (0x0001); // SDMMC_DET
}

VOID
EFIAPI
SdhciEmmcIoMux (
  VOID
  )
{
  /* sdmmc0 iomux */
  BUS_IOC->GPIO2A_IOMUX_SEL_L = (0xFFFFUL << 16) | (0x1111); // EMMC_CMD,EMMC_CLKOUT,EMMC_DATASTROBE,EMMC_RSTN
  BUS_IOC->GPIO2D_IOMUX_SEL_L = (0xFFFFUL << 16) | (0x1111); // EMMC_D0,EMMC_D1,EMMC_D2,EMMC_D3
  BUS_IOC->GPIO2D_IOMUX_SEL_H = (0xFFFFUL << 16) | (0x1111); // EMMC_D4,EMMC_D5,EMMC_D6,EMMC_D7
}

#define NS_CRU_BASE       0xFD7C0000
#define CRU_CLKSEL_CON59  0x03EC
#define CRU_CLKSEL_CON78  0x0438

VOID
EFIAPI
Rk806SpiIomux (
  VOID
  )
{
  /* io mux */
  // BUS_IOC->GPIO1A_IOMUX_SEL_H = (0xFFFFUL << 16) | 0x8888;
  // BUS_IOC->GPIO1B_IOMUX_SEL_L = (0x000FUL << 16) | 0x0008;
  PMU1_IOC->GPIO0A_IOMUX_SEL_H = (0x0FF0UL << 16) | 0x0110;
  PMU1_IOC->GPIO0B_IOMUX_SEL_L = (0xF0FFUL << 16) | 0x1011;
  MmioWrite32 (NS_CRU_BASE + CRU_CLKSEL_CON59, (0x00C0UL << 16) | 0x0080);
}

VOID
EFIAPI
Rk806Configure (
  VOID
  )
{
  UINTN  RegCfgIndex;

  RK806Init ();

  for (RegCfgIndex = 0; RegCfgIndex < ARRAY_SIZE (rk806_init_data); RegCfgIndex++) {
    RK806RegulatorInit (rk806_init_data[RegCfgIndex]);
  }
}

VOID
EFIAPI
SetCPULittleVoltage (
  IN UINT32  Microvolts
  )
{
  struct regulator_init_data  Rk806CpuLittleSupply =
    RK8XX_VOLTAGE_INIT (MASTER_BUCK2, Microvolts);

  RK806RegulatorInit (Rk806CpuLittleSupply);
}

VOID
EFIAPI
NorFspiIomux (
  VOID
  )
{
  /* io mux */
  MmioWrite32 (
    NS_CRU_BASE + CRU_CLKSEL_CON78,
    (((0x3 << 12) | (0x3f << 6)) << 16) | (0x0 << 12) | (0x3f << 6)
    );
  #define FSPI_M1
 #if defined (FSPI_M0)
  /*FSPI M0*/
  BUS_IOC->GPIO2A_IOMUX_SEL_L = ((0xF << 0) << 16) | (2 << 0);   // FSPI_CLK_M0
  BUS_IOC->GPIO2D_IOMUX_SEL_L = (0xFFFFUL << 16) | (0x2222);     // FSPI_D0_M0,FSPI_D1_M0,FSPI_D2_M0,FSPI_D3_M0
  BUS_IOC->GPIO2D_IOMUX_SEL_H = ((0xF << 8) << 16) | (0x2 << 8); // FSPI_CS0N_M0
 #elif defined (FSPI_M1)
  /*FSPI M1*/
  BUS_IOC->GPIO2A_IOMUX_SEL_H = (0xFF00UL << 16) | (0x3300); // FSPI_D0_M1,FSPI_D1_M1
  BUS_IOC->GPIO2B_IOMUX_SEL_L = (0xF0FFUL << 16) | (0x3033); // FSPI_D2_M1,FSPI_D3_M1,FSPI_CLK_M1
  BUS_IOC->GPIO2B_IOMUX_SEL_H = (0xF << 16) | (0x3);         // FSPI_CS0N_M1
 #else
  /*FSPI M2*/
  BUS_IOC->GPIO3A_IOMUX_SEL_L = (0xFFFFUL << 16) | (0x5555); // [FSPI_D0_M2-FSPI_D3_M2]
  BUS_IOC->GPIO3A_IOMUX_SEL_H = (0xF0UL << 16) | (0x50);     // FSPI_CLK_M2
  BUS_IOC->GPIO3C_IOMUX_SEL_H = (0xF << 16) | (0x2);         // FSPI_CS0_M2
 #endif
}

VOID
EFIAPI
GmacIomux (
  IN UINT32  Id
  )
{
  /* No GMAC here */
}

VOID
EFIAPI
NorFspiEnableClock (
  UINT32  *CruBase
  )
{
  UINTN  BaseAddr = (UINTN)CruBase;

  MmioWrite32 (BaseAddr + 0x087C, 0x0E000000);
}

VOID
EFIAPI
I2cIomux (
  UINT32  id
  )
{
  switch (id) {
    case 0:
      GpioPinSetFunction (0, GPIO_PIN_PD1, 3); // i2c0_scl_m2
      GpioPinSetFunction (0, GPIO_PIN_PD2, 3); // i2c0_sda_m2
      break;
    case 1:
      break;
    case 2:
      GpioPinSetFunction (0, GPIO_PIN_PB7, 9); // i2c2_scl_m0
      GpioPinSetFunction (0, GPIO_PIN_PC0, 9); // i2c2_sda_m0
      break;
    case 3:
      GpioPinSetFunction (1, GPIO_PIN_PC1, 9); // i2c3_scl_m0
      GpioPinSetFunction (1, GPIO_PIN_PC0, 9); // i2c3_sda_m0
      break;
    case 4:
      GpioPinSetFunction (1, GPIO_PIN_PA3, 9); // i2c4_scl_m3
      GpioPinSetFunction (1, GPIO_PIN_PA2, 9); // i2c4_sda_m3
      break;
    case 5:
      break;
    case 6:
      GpioPinSetFunction (0, GPIO_PIN_PD0, 9); // i2c6_scl_m0
      GpioPinSetFunction (0, GPIO_PIN_PC7, 9); // i2c6_sda_m0
      break;
    case 7:
      break;
    default:
      break;
  }
}

VOID
EFIAPI
UsbPortPowerEnable (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "UsbPortPowerEnable called\n"));

  /* Set VCC_5V0_PWREN_H */
  GpioPinWrite (4, GPIO_PIN_PA2, TRUE);
  GpioPinSetDirection (4, GPIO_PIN_PA2, GPIO_PIN_OUTPUT);

  /* Set USB_HOST_PWREN_H */
  GpioPinWrite (1, GPIO_PIN_PB1, TRUE);
  GpioPinSetDirection (1, GPIO_PIN_PB1, GPIO_PIN_OUTPUT);

  /* Set TYPEC0_PWR_EN */
  GpioPinWrite (3, GPIO_PIN_PA4, TRUE);
  GpioPinSetDirection (3, GPIO_PIN_PA4, GPIO_PIN_OUTPUT);
}

VOID
EFIAPI
Usb2PhyResume (
  VOID
  )
{
  MmioWrite32 (0xfd5d0008, 0x20000000);
  MmioWrite32 (0xfd5d4008, 0x20000000);
  MmioWrite32 (0xfd5d8008, 0x20000000);
  MmioWrite32 (0xfd5dc008, 0x20000000);
  MmioWrite32 (0xfd7f0a10, 0x07000700);
  MmioWrite32 (0xfd7f0a10, 0x07000000);
}

VOID
EFIAPI
PcieIoInit (
  UINT32  Segment
  )
{
  /* Set reset to gpio output mode */
  if (Segment == PCIE_SEGMENT_PCIE20L2) {
    // AP6275P Wi-Fi
    GpioPinSetDirection (3, GPIO_PIN_PD1, GPIO_PIN_OUTPUT);

    /* wifi_poweren_gpio */
    GpioPinSetDirection (0, GPIO_PIN_PC4, GPIO_PIN_OUTPUT);
  }
}

VOID
EFIAPI
PciePowerEn (
  UINT32   Segment,
  BOOLEAN  Enable
  )
{
  if (Segment == PCIE_SEGMENT_PCIE20L2) {
    /* wifi_poweren_gpio */
    GpioPinWrite (0, GPIO_PIN_PC4, Enable);
  }
}

VOID
EFIAPI
PciePeReset (
  UINT32   Segment,
  BOOLEAN  Enable
  )
{
  if (Segment == PCIE_SEGMENT_PCIE20L2) {
    GpioPinWrite (3, GPIO_PIN_PD1, !Enable);
  }
}

STATIC
VOID
KhadasMcuRegistrationEventHandler (
  IN  EFI_EVENT  Event,
  IN  VOID       *Context
  )
{
  EFI_HANDLE  Handle;
  UINTN       BufferSize;
  EFI_STATUS  Status;

  BufferSize = sizeof (EFI_HANDLE);
  Status     = gBS->LocateHandle (
                      ByRegisterNotify,
                      NULL,
                      mKhadasMcuEventRegistration,
                      &BufferSize,
                      &Handle
                      );
  if (EFI_ERROR (Status)) {
    if (Status != EFI_NOT_FOUND) {
      DEBUG ((
        DEBUG_WARN,
        "%a: Failed to locate gKhadasMcuProtocol. Status=%r\n",
        __func__,
        Status
        ));
    }

    return;
  }

  Status = gBS->HandleProtocol (
                  Handle,
                  &gKhadasMcuProtocolGuid,
                  (VOID **)&mKhadasMcu
                  );
  ASSERT_EFI_ERROR (Status);

  PwmFanSetSpeed (mTargetFanSpeed);

  gBS->CloseEvent (Event);
}

VOID
EFIAPI
PwmFanIoSetup (
  VOID
  )
{
  EfiCreateProtocolNotifyEvent (
    &gKhadasMcuProtocolGuid,
    TPL_CALLBACK,
    KhadasMcuRegistrationEventHandler,
    NULL,
    &mKhadasMcuEventRegistration
    );
}

VOID
EFIAPI
PwmFanSetSpeed (
  UINT32  Percentage
  )
{
  mTargetFanSpeed = (UINT8)Percentage;

  //
  // If the protocol is installed, set the speed now.
  // Otherwise it will be set by KhadasMcuRegistrationEventHandler
  // when ready.
  //
  if (mKhadasMcu != NULL) {
    mKhadasMcu->SetFanSpeedPercentage (mKhadasMcu, mTargetFanSpeed);
  }
}

VOID
EFIAPI
PlatformInitLeds (
  VOID
  )
{
  /* Red off, Green for status, Blue for power */
  GpioPinWrite (4, GPIO_PIN_PB4, FALSE);
  GpioPinSetDirection (4, GPIO_PIN_PB4, GPIO_PIN_OUTPUT);
  GpioPinWrite (4, GPIO_PIN_PB2, FALSE);
  GpioPinSetDirection (4, GPIO_PIN_PB2, GPIO_PIN_OUTPUT);
  GpioPinWrite (4, GPIO_PIN_PB3, TRUE);
  GpioPinSetDirection (4, GPIO_PIN_PB3, GPIO_PIN_OUTPUT);
}

VOID
EFIAPI
PlatformSetStatusLed (
  IN BOOLEAN  Enable
  )
{
  GpioPinWrite (4, GPIO_PIN_PB2, Enable);
}

CONST EFI_GUID *
EFIAPI
PlatformGetDtbFileGuid (
  IN UINT32  CompatMode
  )
{
  STATIC CONST EFI_GUID  VendorDtbFileGuid = {
    // DeviceTree/Vendor.inf
    0xd58b4028, 0x43d8, 0x4e97, { 0x87, 0xd4, 0x4e, 0x37, 0x16, 0x13, 0x65, 0x80 }
  };
  STATIC CONST EFI_GUID  MainlineDtbFileGuid = {
    // DeviceTree/Mainline.inf
    0x84492e97, 0xa10f, 0x49a7, { 0x85, 0xe9, 0x02, 0x5d, 0x19, 0x66, 0xb3, 0x43 }
  };

  switch (CompatMode) {
    case FDT_COMPAT_MODE_VENDOR:
      return &VendorDtbFileGuid;
    case FDT_COMPAT_MODE_MAINLINE:
      return &MainlineDtbFileGuid;
  }

  return NULL;
}

VOID
EFIAPI
PlatformEarlyInit (
  VOID
  )
{
  // Configure various things specific to this platform
  GpioPinSetFunction (1, GPIO_PIN_PD3, 0); // jdet
  GpioPinSetFunction (1, GPIO_PIN_PD0, 0); // spk_con
}
