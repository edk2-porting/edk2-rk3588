/*******************************************************************************
Copyright (C) 2018 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/
#ifndef __MARVELL_BOARD_DESC_PROTOCOL_H__
#define __MARVELL_BOARD_DESC_PROTOCOL_H__

#include <Library/ArmadaSoCDescLib.h>
#include <Library/ArmadaBoardDescLib.h>

extern EFI_GUID gMarvellBoardDescProtocolGuid;

typedef struct _MARVELL_BOARD_DESC_PROTOCOL MARVELL_BOARD_DESC_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *MV_BOARD_DESC_COMPHY_GET) (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_COMPHY_DESC    **ComPhyDesc
  );

typedef
EFI_STATUS
(EFIAPI *MV_BOARD_GPIO_DESCRIPTION_GET) (
  IN MARVELL_BOARD_DESC_PROTOCOL    *This,
  IN OUT MV_BOARD_GPIO_DESCRIPTION **GpioDescription
  );

typedef
EFI_STATUS
(EFIAPI *MV_BOARD_DESC_I2C_GET) (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_I2C_DESC       **I2cDesc
  );

typedef
EFI_STATUS
(EFIAPI *MV_BOARD_DESC_MDIO_GET) (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_MDIO_DESC      **MdioDesc
  );

typedef
EFI_STATUS
(EFIAPI *MV_BOARD_DESC_AHCI_GET) (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_AHCI_DESC      **AhciDesc
  );

typedef
EFI_STATUS
(EFIAPI *MV_BOARD_DESC_SDMMC_GET) (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_SDMMC_DESC     **SdMmcDesc
  );

typedef
EFI_STATUS
(EFIAPI *MV_BOARD_DESC_XHCI_GET) (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_XHCI_DESC      **XhciDesc
  );

/**
  Return the description of PCIE controllers used on the platform.

  @param[in out]  *This                 Pointer to board description protocol.
  @param[in out] **PcieDescription      Array containing PCIE controllers'
                                        description.

  @retval EFI_SUCCESS                   The data were obtained successfully.
  @retval EFI_NOT_FOUND                 None of the controllers is used.
  @retval EFI_INVALID_PARAMETER         Description wrongly defined.
  @retval EFI_OUT_OF_RESOURCES          Lack of resources.
  @retval Other                         Return error status.

**/
typedef
EFI_STATUS
(EFIAPI *MV_BOARD_PCIE_DESCRIPTION_GET) (
  IN MARVELL_BOARD_DESC_PROTOCOL          *This,
  IN OUT MV_BOARD_PCIE_DESCRIPTION CONST **PcieDescription
  );

typedef
EFI_STATUS
(EFIAPI *MV_BOARD_DESC_PP2_GET) (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_PP2_DESC       **Pp2Desc
  );

typedef
EFI_STATUS
(EFIAPI *MV_BOARD_DESC_UTMI_GET) (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_UTMI_DESC      **UtmiDesc
  );

typedef
VOID
(EFIAPI *MV_BOARD_DESC_FREE) (
  IN VOID                         *BoardDesc
  );

struct _MARVELL_BOARD_DESC_PROTOCOL {
  MV_BOARD_DESC_AHCI_GET         BoardDescAhciGet;
  MV_BOARD_DESC_COMPHY_GET       BoardDescComPhyGet;
  MV_BOARD_DESC_I2C_GET          BoardDescI2cGet;
  MV_BOARD_DESC_MDIO_GET         BoardDescMdioGet;
  MV_BOARD_DESC_PP2_GET          BoardDescPp2Get;
  MV_BOARD_DESC_SDMMC_GET        BoardDescSdMmcGet;
  MV_BOARD_DESC_UTMI_GET         BoardDescUtmiGet;
  MV_BOARD_DESC_XHCI_GET         BoardDescXhciGet;
  MV_BOARD_DESC_FREE             BoardDescFree;
  MV_BOARD_GPIO_DESCRIPTION_GET  GpioDescriptionGet;
  MV_BOARD_PCIE_DESCRIPTION_GET  PcieDescriptionGet;
};

#endif // __MARVELL_BOARD_DESC_PROTOCOL_H__
