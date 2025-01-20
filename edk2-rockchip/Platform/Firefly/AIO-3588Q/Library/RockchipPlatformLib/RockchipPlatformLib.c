/** @file
*
*  Copyright (c) 2021, Rockchip Limited. All rights reserved.
*  Copyright (c) 2023-2024, Mario Bălănică <mariobalanica02@gmail.com>
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/GpioLib.h>
#include <Library/RK806.h>
#include <Library/Rk3588Pcie.h>
#include <Library/PWMLib.h>
#include <Soc.h>
#include <VarStoreData.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/Pca9555.h>

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

EFI_STATUS
EFIAPI
GetPca9555Protocol (
  IN OUT PCA95XX_PROTOCOL  **Pca95xxProtocl
  )
{
  EFI_HANDLE  *HandleBuffer;
  EFI_STATUS  Status;
  UINTN       HandleCount;

  /* Locate Handles of all PCA95XX_PROTOCOL producers */
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gPca95xxProtocolGuid,
                  NULL,
                  &HandleCount,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Unable to locate handles\n", __FUNCTION__));
    return Status;
  }

  DEBUG ((
    DEBUG_INFO,
    "%a: got %d PCA95XX_PROTOCOLs\n",
    __FUNCTION__,
    HandleCount
    ));

  /*
   * Open Pca95xxProtocl. With EFI_OPEN_PROTOCOL_GET_PROTOCOL attribute
   * the consumer is not obliged to call CloseProtocol.
   */
  Status = gBS->OpenProtocol (
                  HandleBuffer[0],
                  &gPca95xxProtocolGuid,
                  (VOID **)Pca95xxProtocl,
                  HandleBuffer[0],
                  NULL,
                  EFI_OPEN_PROTOCOL_GET_PROTOCOL
                  );

  return Status;
}

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
  /* Do not override, set by earlier boot stages. */
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
  /* Do not override, set by earlier boot stages. */
}

VOID
EFIAPI
GmacIomux (
  IN UINT32  Id
  )
{
  switch (Id) {
    case 0:
      /* gmac0 iomux */
      BUS_IOC->GPIO2A_IOMUX_SEL_H = (0xFF00UL << 16) | 0x1100; // GMAC0_RXD2, GMAC0_RXD3
      BUS_IOC->GPIO2B_IOMUX_SEL_L = (0xFFFFUL << 16) | 0x1111; // GMAC0_RXCLK, GMAC0_TXD2, GMAC0_TXD3, GMAC0_TXCLK
      BUS_IOC->GPIO2B_IOMUX_SEL_H = (0xFF00UL << 16) | 0x1100; // GMAC0_TXD0, GMAC0_TXD1
      BUS_IOC->GPIO2C_IOMUX_SEL_L = (0x0FFFUL << 16) | 0x0111; // GMAC0_TXEN, GMAC0_RXD0, GMAC0_RXD1
      BUS_IOC->GPIO4C_IOMUX_SEL_L = (0xFF00UL << 16) | 0x1100; // GMAC0_RXDV_CRS, GMAC0_MCLKINOUT
      BUS_IOC->GPIO4C_IOMUX_SEL_H = (0x00FFUL << 16) | 0x0011; // GMAC0_MDC, GMAC0_MDIO

      /* phy0 reset */
      GpioPinSetDirection (3, GPIO_PIN_PC7, GPIO_PIN_OUTPUT);
      break;
    case 1:
      /* gmac1 iomux */
      BUS_IOC->GPIO3B_IOMUX_SEL_H = (0x0FFFUL << 16) | 0x0111; /* GMAC1_MCLKINOUT, GMAC1_TXEN, GMAC1_TXD1 */
      BUS_IOC->GPIO3A_IOMUX_SEL_L = (0xFFFFUL << 16) | 0x1111; /* GMAC1_RXD3, GMAC1_RXD2, GMAC1_TXD3, GMAC1_TXD2 */
      BUS_IOC->GPIO3B_IOMUX_SEL_L = (0xF0FFUL << 16) | 0x1011; /* GMAC1_TXD0, GMAC1_RXDV_CRS, GMAC1_RXD1 */
      BUS_IOC->GPIO3A_IOMUX_SEL_H = (0xF0FFUL << 16) | 0x1011; /* GMAC1_RXD0, GMAC1_RXCLK, GMAC1_TXCLK */
      BUS_IOC->GPIO3C_IOMUX_SEL_L = (0xFF00UL << 16) | 0x1100; /* GMAC1_MDIO, GMAC1_MDC */

      /* phy1 reset */
      GpioPinSetDirection (3, GPIO_PIN_PB7, GPIO_PIN_OUTPUT);
      break;
    default:
      break;
  }
}

VOID
EFIAPI
GmacIoPhyReset (
  UINT32   Id,
  BOOLEAN  Enable
  )
{
  switch (Id) {
    case 0:
      /* phy0 reset */
      GpioPinWrite (3, GPIO_PIN_PC7, !Enable);
      break;
    case 1:
      /* phy1 reset */
      GpioPinWrite (3, GPIO_PIN_PB7, !Enable);
      break;
    default:
      break;
  }
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
      GpioPinSetFunction (0, GPIO_PIN_PD1, 3); // I2C0_SCL_M2
      GpioPinSetFunction (0, GPIO_PIN_PD2, 3); // I2C0_SDA_M2
      break;
    case 1:
      GpioPinSetFunction (0, GPIO_PIN_PD4, 9); // I2C1_SCL_M2
      GpioPinSetFunction (0, GPIO_PIN_PD5, 9); // I2C1_SDA_M2
      break;
    case 3:
      GpioPinSetFunction (1, GPIO_PIN_PC1, 9); // I2C3_SCL_M0
      GpioPinSetFunction (1, GPIO_PIN_PC0, 9); // I2C3_SDA_M0
      break;
    case 6:
      GpioPinSetFunction (0, GPIO_PIN_PD0, 9); // I2C6_SCL_M0
      GpioPinSetFunction (0, GPIO_PIN_PC7, 9); // I2C6_SDA_M0
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
  EFI_STATUS        Status = EFI_SUCCESS;
  PCA95XX_PROTOCOL  *Pca95xxProtocol;

  /* On Firefly AIO-3588Q this is controlled via the PCA9555. */
  Status = GetPca9555Protocol (&Pca95xxProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UsbPortPowerEnable failed to get PCA9555! (%d)\n", Status));
  } else {
    /* USB-C */
    Pca95xxProtocol->GpioProtocol.Set (
                                    &Pca95xxProtocol->GpioProtocol,
                                    12, /* vbus5v0_typec_pwr_en */
                                    GPIO_MODE_OUTPUT_0
                                    );

    gBS->Stall (1200000);

    Pca95xxProtocol->GpioProtocol.Set (
                                    &Pca95xxProtocol->GpioProtocol,
                                    12, /* vbus5v0_typec_pwr_en */
                                    GPIO_MODE_OUTPUT_1
                                    );

    /* other USB stuff */
    Pca95xxProtocol->GpioProtocol.Set (
                                    &Pca95xxProtocol->GpioProtocol,
                                    5, /* vcc5v0_host */
                                    GPIO_MODE_OUTPUT_1
                                    );

    Pca95xxProtocol->GpioProtocol.Set (
                                    &Pca95xxProtocol->GpioProtocol,
                                    4, /* vcc_hub_reset */
                                    GPIO_MODE_OUTPUT_1
                                    );

    Pca95xxProtocol->GpioProtocol.Set (
                                    &Pca95xxProtocol->GpioProtocol,
                                    6, /* vcc_hub3_reset */
                                    GPIO_MODE_OUTPUT_1
                                    );

    Pca95xxProtocol->GpioProtocol.Set (
                                    &Pca95xxProtocol->GpioProtocol,
                                    7, /* vcc5v0_host3 */
                                    GPIO_MODE_OUTPUT_1
                                    );
  }
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
  switch (Segment) {
    case PCIE_SEGMENT_PCIE30X4:
      GpioPinSetDirection (4, GPIO_PIN_PB6, GPIO_PIN_OUTPUT); // PCIE30X4_PERSTN_M1
      GpioPinSetDirection (4, GPIO_PIN_PC6, GPIO_PIN_OUTPUT); // vcc3v3_pcie30
      break;
    case PCIE_SEGMENT_PCIE20L0:
      GpioPinSetDirection (1, GPIO_PIN_PB4, GPIO_PIN_OUTPUT);
      break;
    case PCIE_SEGMENT_PCIE20L1:
      break;
    case PCIE_SEGMENT_PCIE20L2:
      break;
  }
}

VOID
EFIAPI
PciePowerEn (
  UINT32   Segment,
  BOOLEAN  Enable
  )
{
  switch (Segment) {
    case PCIE_SEGMENT_PCIE30X4:
      GpioPinWrite (4, GPIO_PIN_PC6, Enable); // vcc3v3_pcie30
      break;
    case PCIE_SEGMENT_PCIE20L0:
      break;
    case PCIE_SEGMENT_PCIE20L1:
      break;
    case PCIE_SEGMENT_PCIE20L2:
      break;
  }
}

VOID
EFIAPI
PciePeReset (
  UINT32   Segment,
  BOOLEAN  Enable
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
  PCA95XX_PROTOCOL  *Pca95xxProtocol;

  switch (Segment) {
    case PCIE_SEGMENT_PCIE30X4:
      GpioPinWrite (4, GPIO_PIN_PB6, !Enable); // PCIE30X4_PERSTN_M1
      break;
    case PCIE_SEGMENT_PCIE20L0:
      GpioPinWrite (1, GPIO_PIN_PB4, !Enable);
      break;
    case PCIE_SEGMENT_PCIE20L1:
      break;
    case PCIE_SEGMENT_PCIE20L2:
      Status = GetPca9555Protocol (&Pca95xxProtocol);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "PciePeReset(L2) failed to get PCA9555! (%d)\n", Status));
      } else {
        Pca95xxProtocol->GpioProtocol.Set (
                                        &Pca95xxProtocol->GpioProtocol,
                                        14, /* PCA_IO1_6 */
                                        Enable ? GPIO_MODE_OUTPUT_0 : GPIO_MODE_OUTPUT_1
                                        );
      }

      break;
  }
}

PWM_DATA  pwm_data = {
  .ControllerID = PWM_CONTROLLER3,
  .ChannelID    = PWM_CHANNEL3,
  .PeriodNs     = 50000,
  .DutyNs       = 50000,
  .Polarity     = FALSE,
}; // PWM15

VOID
EFIAPI
PwmFanIoSetup (
  VOID
  )
{
  GpioPinSetFunction (1, GPIO_PIN_PC6, 0xB); // PWM15_IR_M2
  RkPwmSetConfig (&pwm_data);
  RkPwmEnable (&pwm_data);
}

VOID
EFIAPI
PwmFanSetSpeed (
  IN UINT32  Percentage
  )
{
  pwm_data.DutyNs = pwm_data.PeriodNs * Percentage / 100;
  RkPwmSetConfig (&pwm_data);
}

VOID
EFIAPI
PlatformInitLeds (
  VOID
  )
{
  GpioPinWrite (3, GPIO_PIN_PB2, FALSE);
  GpioPinSetDirection (3, GPIO_PIN_PB2, GPIO_PIN_OUTPUT);
}

VOID
EFIAPI
PlatformSetStatusLed (
  IN BOOLEAN  Enable
  )
{
  GpioPinWrite (3, GPIO_PIN_PB2, Enable);
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

  switch (CompatMode) {
    case FDT_COMPAT_MODE_VENDOR:
      return &VendorDtbFileGuid;
  }

  return NULL;
}

VOID
EFIAPI
PlatformEarlyInit (
  VOID
  )
{
  GpioPinSetDirection (4, GPIO_PIN_PB0, GPIO_PIN_OUTPUT); // headphone enable
  GpioPinWrite (4, GPIO_PIN_PB0, TRUE);
  GpioPinSetFunction (1, GPIO_PIN_PC4, 0); // headphone detect
}
