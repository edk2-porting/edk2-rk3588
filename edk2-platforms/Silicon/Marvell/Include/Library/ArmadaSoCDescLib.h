/**
*
*  Copyright (C) 2018, Marvell International Ltd. and its affiliates
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#ifndef __ARMADA_SOC_DESC_LIB_H__
#define __ARMADA_SOC_DESC_LIB_H__

#include <Library/MvComPhyLib.h>
#include <Library/NonDiscoverableDeviceRegistrationLib.h>

#include <Protocol/EmbeddedGpio.h>

//
// North Bridge description
//

/**

Routine Description:

  Get base address of the SoC North Bridge.

Arguments:

  ApBase  - Base address of the North Bridge.
  ApIndex - Index of the North Bridge.

Returns:

  EFI_SUCCESS           - Proper base address is returned.
  EFI_INVALID_PARAMETER - The index is out of range.

**/
EFI_STATUS
EFIAPI
ArmadaSoCAp8xxBaseGet (
  IN OUT EFI_PHYSICAL_ADDRESS  *ApBase,
  IN UINTN                      ApIndex
  );

//
// ComPhy SoC description
//
typedef struct {
  UINTN ComPhyId;
  UINTN ComPhyBaseAddress;
  UINTN ComPhyHpipe3BaseAddress;
  UINTN ComPhyLaneCount;
  UINTN ComPhyMuxBitCount;
  MV_COMPHY_CHIP_TYPE ComPhyChipType;
} MV_SOC_COMPHY_DESC;

EFI_STATUS
EFIAPI
ArmadaSoCDescComPhyGet (
  IN OUT MV_SOC_COMPHY_DESC  **ComPhyDesc,
  IN OUT UINTN                *DescCount
  );

//
// South Bridge description
//
EFI_PHYSICAL_ADDRESS
EFIAPI
ArmadaSoCDescCpBaseGet (
  IN UINTN  CpIndex
  );

//
// GPIO devices description template definition
//
EFI_STATUS
EFIAPI
ArmadaSoCGpioGet (
  IN OUT GPIO_CONTROLLER  **SoCGpioDescription,
  IN OUT UINTN             *Count
  );

//
// I2C
//
typedef struct {
  UINTN I2cId;
  UINTN I2cBaseAddress;
} MV_SOC_I2C_DESC;

EFI_STATUS
EFIAPI
ArmadaSoCDescI2cGet (
  IN OUT MV_SOC_I2C_DESC  **I2cDesc,
  IN OUT UINTN             *DescCount
  );

//
// ICU (Interrupt Consolidation Unit)
//
typedef enum {
  IcuGroupNsr  = 0,
  IcuGroupSr   = 1,
  IcuGroupLpi  = 2,
  IcuGroupVlpi = 3,
  IcuGroupSei  = 4,
  IcuGroupRei  = 5,
  IcuGroupMax,
} ICU_GROUP;

typedef struct {
  ICU_GROUP Group;
  EFI_PHYSICAL_ADDRESS SetSpiAddr;
  EFI_PHYSICAL_ADDRESS ClrSpiAddr;
} ICU_MSI;

typedef struct {
  UINTN    IcuSpiBase;
  ICU_MSI  IcuMsi[IcuGroupMax];
} MV_SOC_ICU_DESC;

EFI_STATUS
EFIAPI
ArmadaSoCDescIcuGet (
  IN OUT MV_SOC_ICU_DESC  **IcuDesc
  );

//
// MDIO
//
typedef struct {
  UINTN MdioId;
  UINTN MdioBaseAddress;
} MV_SOC_MDIO_DESC;

EFI_STATUS
EFIAPI
ArmadaSoCDescMdioGet (
  IN OUT MV_SOC_MDIO_DESC  **MdioDesc,
  IN OUT UINTN              *DescCount
  );

//
// NonDiscoverable devices SoC description
//
// AHCI
typedef struct {
  UINTN AhciId;
  UINTN AhciBaseAddress;
  UINTN AhciMemSize;
  NON_DISCOVERABLE_DEVICE_DMA_TYPE AhciDmaType;
} MV_SOC_AHCI_DESC;

EFI_STATUS
EFIAPI
ArmadaSoCDescAhciGet (
  IN OUT MV_SOC_AHCI_DESC  **AhciDesc,
  IN OUT UINTN              *DescCount
  );

// SDMMC
typedef struct {
  UINTN SdMmcBaseAddress;
  UINTN SdMmcMemSize;
  NON_DISCOVERABLE_DEVICE_DMA_TYPE SdMmcDmaType;
} MV_SOC_SDMMC_DESC;

EFI_STATUS
EFIAPI
ArmadaSoCDescSdMmcGet (
  IN OUT MV_SOC_SDMMC_DESC  **SdMmcDesc,
  IN OUT UINTN               *DescCount
  );

// XHCI
typedef struct {
  UINTN XhciBaseAddress;
  UINTN XhciMemSize;
  NON_DISCOVERABLE_DEVICE_DMA_TYPE XhciDmaType;
} MV_SOC_XHCI_DESC;

EFI_STATUS
EFIAPI
ArmadaSoCDescXhciGet (
  IN OUT MV_SOC_XHCI_DESC  **XhciDesc,
  IN OUT UINTN              *DescCount
  );

/**
  This function returns the total number of PCIE controllers and an array
  with their base addresses.

  @param[in out] **PcieDbiAddresses  Array containing PCIE controllers' base
                                     adresses.
  @param[in out]  *Count             Total amount of available PCIE controllers.

  @retval EFI_SUCCESS                The data were obtained successfully.
  @retval EFI_OUT_OF_RESOURCES       The request could not be completed due to a
                                     lack of resources.

**/
EFI_STATUS
EFIAPI
ArmadaSoCPcieGet (
  IN OUT EFI_PHYSICAL_ADDRESS  **PcieDbiAddresses,
  IN OUT UINTN                  *Count
  );

//
// PP2 NIC devices SoC description
//
typedef struct {
  UINTN Pp2BaseAddress;
  UINTN Pp2ClockFrequency;
} MV_SOC_PP2_DESC;

EFI_STATUS
EFIAPI
ArmadaSoCDescPp2Get (
  IN OUT MV_SOC_PP2_DESC  **Pp2Desc,
  IN OUT UINTN             *DescCount
  );

//
// UTMI PHY devices SoC description
//
typedef struct {
  UINT8 UtmiPhyId;
  UINTN UtmiBaseAddress;
  UINTN UtmiPllAddress;
  UINTN UtmiConfigAddress;
  UINTN UsbConfigAddress;
} MV_SOC_UTMI_DESC;

EFI_STATUS
EFIAPI
ArmadaSoCDescUtmiGet (
  IN OUT MV_SOC_UTMI_DESC  **UtmiDesc,
  IN OUT UINTN              *DescCount
  );
#endif /* __ARMADA_SOC_DESC_LIB_H__ */
