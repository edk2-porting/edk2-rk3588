/**
*
*  Copyright (C) 2018, Marvell International Ltd. and its affiliates
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#ifndef __ARMADA_BOARD_DESC_LIB_H__
#define __ARMADA_BOARD_DESC_LIB_H__

#include <Library/ArmadaSoCDescLib.h>
#include <Library/MvGpioLib.h>

//
// General purpose routine for per-board initalization
//
EFI_STATUS
EFIAPI
ArmadaBoardInit (
  VOID
  );

//
// COMPHY controllers per-board description
//
typedef struct {
  MV_SOC_COMPHY_DESC *SoC;
  UINTN               ComPhyDevCount;
} MV_BOARD_COMPHY_DESC;

//
// GPIO devices per-board description
//
typedef struct {
  UINTN ChipId;
  UINTN I2cAddress;
  UINTN I2cBus;
} MV_GPIO_EXPANDER;

typedef struct {
  GPIO_CONTROLLER  *SoCGpio;
  UINTN             GpioDeviceCount;
  MV_GPIO_EXPANDER *GpioExpanders;
  UINTN             GpioExpanderCount;
} MV_BOARD_GPIO_DESCRIPTION;

EFI_STATUS
EFIAPI
ArmadaBoardGpioExpanderGet (
  IN OUT MV_GPIO_EXPANDER **GpioExpanders,
  IN OUT UINTN             *GpioExpanderCount
  );

//
// I2C devices per-board description
//
typedef struct {
  MV_SOC_I2C_DESC *SoC;
  UINTN            I2cDevCount;
} MV_BOARD_I2C_DESC;

//
// MDIO devices per-board description
//
typedef struct {
  MV_SOC_MDIO_DESC *SoC;
  UINTN             MdioDevCount;
} MV_BOARD_MDIO_DESC;

//
// NonDiscoverableDevices per-board description
//

//
// AHCI devices per-board description
//
typedef struct {
  MV_SOC_AHCI_DESC *SoC;
  UINTN             AhciDevCount;
} MV_BOARD_AHCI_DESC;

//
// SDMMC devices per-board description
//
typedef enum {
  RemovableSlot,
  EmbeddedSlot,
  SharedBusSlot,
  UnknownSlot
} MV_SDMMC_SLOT_TYPE;

typedef struct {
  MV_SOC_SDMMC_DESC *SoC;
  UINTN    SdMmcDevCount;
  BOOLEAN  Xenon1v8Enabled;
  BOOLEAN  Xenon8BitBusEnabled;
  BOOLEAN  XenonSlowModeEnabled;
  UINT8    XenonTuningStepDivisor;
  MV_SDMMC_SLOT_TYPE SlotType;
} MV_BOARD_SDMMC_DESC;

EFI_STATUS
EFIAPI
ArmadaBoardDescSdMmcGet (
  OUT UINTN               *SdMmcDevCount,
  OUT MV_BOARD_SDMMC_DESC **SdMmcDesc
  );

//
// XHCI devices per-board description
//
typedef struct {
  MV_SOC_XHCI_DESC *SoC;
  UINTN             XhciDevCount;
} MV_BOARD_XHCI_DESC;

//
// PCIE controllers description
//
typedef struct {
  EFI_PHYSICAL_ADDRESS PcieDbiAddress;
  EFI_PHYSICAL_ADDRESS ConfigSpaceAddress;
  UINT64 ConfigSpaceSize;
  BOOLEAN HaveResetGpio;
  MV_GPIO_PIN PcieResetGpio;
  UINT64 PcieBusMin;
  UINT64 PcieBusMax;
  UINT64 PcieIoTranslation;
  UINT64 PcieIoWinBase;
  UINT64 PcieIoWinSize;
  UINT64 PcieMmio32Translation;
  UINT64 PcieMmio32WinBase;
  UINT64 PcieMmio32WinSize;
  UINT64 PcieMmio64Translation;
  UINT64 PcieMmio64WinBase;
  UINT64 PcieMmio64WinSize;
} MV_PCIE_CONTROLLER;

typedef struct {
  MV_PCIE_CONTROLLER CONST *PcieControllers;
  UINTN                     PcieControllerCount;
} MV_BOARD_PCIE_DESCRIPTION;

/**
  Return the number and description of PCIE controllers used on the platform.

  @param[in out] **PcieControllers      Array containing PCIE controllers'
                                        description.
  @param[in out]  *PcieControllerCount  Amount of used PCIE controllers.

  @retval EFI_SUCCESS                   The data were obtained successfully.
  @retval EFI_NOT_FOUND                 None of the controllers is used.
  @retval other                         Return error status.

**/
EFI_STATUS
EFIAPI
ArmadaBoardPcieControllerGet (
  IN OUT MV_PCIE_CONTROLLER CONST **PcieControllers,
  IN OUT UINTN                     *PcieControllerCount
  );

//
// PP2 NIC devices per-board description
//
typedef struct {
  MV_SOC_PP2_DESC *SoC;
  UINT8            Pp2DevCount;
} MV_BOARD_PP2_DESC;

//
// UTMI PHY devices per-board description
//
typedef struct {
  MV_SOC_UTMI_DESC *SoC;
  UINTN             UtmiDevCount;
  UINTN             UtmiPortType;
} MV_BOARD_UTMI_DESC;
#endif /* __ARMADA_SOC_DESC_LIB_H__ */
