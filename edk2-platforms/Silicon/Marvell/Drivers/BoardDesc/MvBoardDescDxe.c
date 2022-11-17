/*******************************************************************************
Copyright (C) 2018 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/
#include "MvBoardDescDxe.h"

MV_BOARD_DESC *mBoardDescInstance;

STATIC MV_BOARD_GPIO_DESCRIPTION *mGpioDescription;
STATIC MV_BOARD_PCIE_DESCRIPTION *mPcieDescription;

STATIC
EFI_STATUS
MvBoardDescComPhyGet (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_COMPHY_DESC    **ComPhyDesc
  )
{
  UINT8 *ComPhyDeviceEnabled;
  UINTN ComPhyCount, ComPhyDeviceTableSize, ComPhyIndex, Index;
  MV_BOARD_COMPHY_DESC *BoardDesc;
  MV_SOC_COMPHY_DESC *SoCDesc;
  EFI_STATUS Status;

  /* Get SoC data about all available ComPhy controllers */
  Status = ArmadaSoCDescComPhyGet (&SoCDesc, &ComPhyCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /*
   * Obtain table with enabled ComPhy controllers
   * which is represented as an array of UINT8 values
   * (0x0 - disabled, 0x1 enabled).
   */
  ComPhyDeviceEnabled = PcdGetPtr (PcdComPhyDevices);
  if (ComPhyDeviceEnabled == NULL) {
    /* No ComPhy controllers declared */
    return EFI_NOT_FOUND;
  }

  ComPhyDeviceTableSize = PcdGetSize (PcdComPhyDevices);

  /* Check if PCD with ComPhy is correctly defined */
  if (ComPhyDeviceTableSize > ComPhyCount) {
    DEBUG ((DEBUG_ERROR, "%a: Wrong PcdComPhyDevices format\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  /* Allocate and fill board description */
  BoardDesc = AllocateZeroPool (ComPhyDeviceTableSize * sizeof (MV_BOARD_COMPHY_DESC));
  if (BoardDesc == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  ComPhyIndex = 0;
  for (Index = 0; Index < ComPhyDeviceTableSize; Index++) {
    if (!ComPhyDeviceEnabled[Index]) {
      DEBUG ((DEBUG_ERROR, "%a: Skip ComPhy controller %d\n", __FUNCTION__, Index));
      continue;
    }

    BoardDesc[ComPhyIndex].SoC = &SoCDesc[Index];
    ComPhyIndex++;
  }

  BoardDesc->ComPhyDevCount = ComPhyIndex;

  *ComPhyDesc = BoardDesc;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvBoardGpioDescriptionGet (
  IN MARVELL_BOARD_DESC_PROTOCOL    *This,
  IN OUT MV_BOARD_GPIO_DESCRIPTION **GpioDescription
  )
{
  UINTN SoCGpioCount, GpioExpanderCount;
  MV_GPIO_EXPANDER *GpioExpanders;
  GPIO_CONTROLLER *SoCGpio;
  EFI_STATUS Status;

  /* Use existing structure if already created. */
  if (mGpioDescription != NULL) {
    *GpioDescription = mGpioDescription;
    return EFI_SUCCESS;
  }

  /* Get SoC data about all available GPIO controllers. */
  Status = ArmadaSoCGpioGet (&SoCGpio, &SoCGpioCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /* Get per-board information about all available GPIO expanders. */
  Status = ArmadaBoardGpioExpanderGet (&GpioExpanders, &GpioExpanderCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /* Allocate and fill board description. */
  mGpioDescription = AllocateZeroPool (sizeof (MV_BOARD_GPIO_DESCRIPTION));
  if (mGpioDescription == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  mGpioDescription->SoCGpio = SoCGpio;
  mGpioDescription->GpioDeviceCount = SoCGpioCount;
  mGpioDescription->GpioExpanders = GpioExpanders;
  mGpioDescription->GpioExpanderCount = GpioExpanderCount;

  *GpioDescription = mGpioDescription;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvBoardDescI2cGet (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_I2C_DESC       **I2cDesc
  )
{
  UINT8 *I2cDeviceEnabled;
  UINTN I2cCount, I2cDeviceEnabledSize, I2cIndex, Index;
  MV_BOARD_I2C_DESC *BoardDesc;
  MV_SOC_I2C_DESC *SoCDesc;
  EFI_STATUS Status;

  /* Get SoC data about all available I2C controllers */
  Status = ArmadaSoCDescI2cGet (&SoCDesc, &I2cCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /*
   * Obtain table with enabled I2C controllers
   * which is represented as an array of UINT8 values
   * (0x0 - disabled, 0x1 enabled).
   */
  I2cDeviceEnabled = PcdGetPtr (PcdI2cControllersEnabled);
  if (I2cDeviceEnabled == NULL) {
    /* No I2C on platform */
    return EFI_SUCCESS;
  }

  I2cDeviceEnabledSize = PcdGetSize (PcdI2cControllersEnabled);

  /* Check if PCD with I2C controllers is correctly defined */
  if (I2cDeviceEnabledSize > I2cCount) {
    DEBUG ((DEBUG_ERROR,
      "%a: Wrong PcdI2cControllersEnabled format\n",
      __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  /* Allocate and fill board description */
  BoardDesc = AllocateZeroPool (I2cDeviceEnabledSize * sizeof (MV_BOARD_I2C_DESC));
  if (BoardDesc == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  I2cIndex = 0;
  for (Index = 0; Index < I2cDeviceEnabledSize; Index++) {
    if (!I2cDeviceEnabled[Index]) {
      DEBUG ((DEBUG_INFO, "%a: Skip I2c controller %d\n", __FUNCTION__, Index));
      continue;
    }

    BoardDesc[I2cIndex].SoC = &SoCDesc[Index];
    I2cIndex++;
  }

  BoardDesc->I2cDevCount = I2cIndex;

  *I2cDesc = BoardDesc;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvBoardDescMdioGet (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_MDIO_DESC      **MdioDesc
  )
{
  MV_BOARD_MDIO_DESC *BoardDesc;
  MV_SOC_MDIO_DESC *SoCDesc;
  UINTN MdioCount, Index;
  EFI_STATUS Status;

  /* Get SoC data about all available MDIO controllers */
  Status = ArmadaSoCDescMdioGet (&SoCDesc, &MdioCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /* Allocate and fill board description */
  BoardDesc = AllocateZeroPool (MdioCount * sizeof (MV_BOARD_MDIO_DESC));
  if (BoardDesc == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < MdioCount; Index++) {
    BoardDesc[Index].SoC = &SoCDesc[Index];
  }

  BoardDesc->MdioDevCount = MdioCount;
  *MdioDesc = BoardDesc;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvBoardDescAhciGet (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_AHCI_DESC      **AhciDesc
  )
{
  UINT8 *AhciDeviceEnabled;
  UINTN AhciCount, AhciDeviceTableSize, AhciIndex, Index;
  MV_BOARD_AHCI_DESC *BoardDesc;
  MV_SOC_AHCI_DESC *SoCDesc;
  EFI_STATUS Status;

  /* Get SoC data about all available AHCI controllers */
  Status = ArmadaSoCDescAhciGet (&SoCDesc, &AhciCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /*
   * Obtain table with enabled AHCI controllers
   * which is represented as an array of UINT8 values
   * (0x0 - disabled, 0x1 enabled).
   */
  AhciDeviceEnabled = PcdGetPtr (PcdPciEAhci);
  if (AhciDeviceEnabled == NULL) {
    /* No AHCI on the platform */
    return EFI_SUCCESS;
  }

  AhciDeviceTableSize = PcdGetSize (PcdPciEAhci);

  /* Check if PCD with AHCI controllers is correctly defined */
  if (AhciDeviceTableSize > AhciCount) {
    DEBUG ((DEBUG_ERROR, "%a: Wrong PcdPciEAhci format\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  /* Allocate and fill board description */
  BoardDesc = AllocateZeroPool (AhciDeviceTableSize * sizeof (MV_BOARD_AHCI_DESC));
  if (BoardDesc == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  AhciIndex = 0;
  for (Index = 0; Index < AhciDeviceTableSize; Index++) {
    if (!AhciDeviceEnabled[Index]) {
      DEBUG ((DEBUG_INFO, "%a: Skip Ahci controller %d\n", __FUNCTION__, Index));
      continue;
    }

    BoardDesc[AhciIndex].SoC = &SoCDesc[Index];
    AhciIndex++;
  }

  BoardDesc->AhciDevCount = AhciIndex;

  *AhciDesc = BoardDesc;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvBoardDescSdMmcGet (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_SDMMC_DESC     **SdMmcDesc
  )
{
  UINT8 *SdMmcDeviceEnabled;
  UINTN SdMmcCount, SdMmcDeviceTableSize, SdMmcIndex, Index;
  UINTN SdMmcDevCount;
  MV_BOARD_SDMMC_DESC *BoardDesc;
  MV_SOC_SDMMC_DESC *SoCDesc;
  EFI_STATUS Status;

  /* Get SoC data about all available SDMMC controllers */
  Status = ArmadaSoCDescSdMmcGet (&SoCDesc, &SdMmcCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /* Get per-board configuration of the controllers */
  Status = ArmadaBoardDescSdMmcGet (&SdMmcDevCount, &BoardDesc);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ArmadaBoardDescSdMmcGet filed\n", __FUNCTION__));
    return Status;
  }

  /*
   * Obtain table with enabled SDMMC controllers
   * which is represented as an array of UINT8 values
   * (0x0 - disabled, 0x1 enabled).
   */
  SdMmcDeviceEnabled = PcdGetPtr (PcdPciESdhci);
  if (SdMmcDeviceEnabled == NULL) {
    /* No SDMMC on platform */
    return EFI_SUCCESS;
  }

  SdMmcDeviceTableSize = PcdGetSize (PcdPciESdhci);

  /* Check if PCD with SDMMC controllers is correctly defined */
  if ((SdMmcDeviceTableSize > SdMmcCount) ||
      (SdMmcDeviceTableSize < SdMmcDevCount)) {
    DEBUG ((DEBUG_ERROR, "%a: Wrong PcdPciESdhci format\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  SdMmcIndex = 0;
  for (Index = 0; Index < SdMmcDeviceTableSize; Index++) {
    if (!SdMmcDeviceEnabled[Index]) {
      DEBUG ((DEBUG_INFO, "%a: Skip SdMmc controller %d\n", __FUNCTION__, Index));
      continue;
    }

    if (SdMmcIndex >= SdMmcDevCount) {
      DEBUG ((DEBUG_ERROR,
        "%a: More enabled devices than returned by ArmadaBoardDescSdMmcGet\n",
        __FUNCTION__));
      return EFI_INVALID_PARAMETER;
    }
    BoardDesc[SdMmcIndex].SoC = &SoCDesc[Index];
    SdMmcIndex++;
  }

  BoardDesc->SdMmcDevCount = SdMmcIndex;

  *SdMmcDesc = BoardDesc;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvBoardDescXhciGet (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_XHCI_DESC      **XhciDesc
  )
{
  UINT8 *XhciDeviceEnabled;
  UINTN XhciCount, XhciDeviceTableSize, XhciIndex, Index;
  MV_BOARD_XHCI_DESC *BoardDesc;
  MV_SOC_XHCI_DESC *SoCDesc;
  EFI_STATUS Status;

  /* Get SoC data about all available XHCI controllers */
  Status = ArmadaSoCDescXhciGet (&SoCDesc, &XhciCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /*
   * Obtain table with enabled XHCI controllers
   * which is represented as an array of UINT8 values
   * (0x0 - disabled, 0x1 enabled).
   */
  XhciDeviceEnabled = PcdGetPtr (PcdPciEXhci);
  if (XhciDeviceEnabled == NULL) {
    /* No XHCI on platform */
    return EFI_SUCCESS;
  }

  XhciDeviceTableSize = PcdGetSize (PcdPciEXhci);

  /* Check if PCD with XHCI controllers is correctly defined */
  if (XhciDeviceTableSize > XhciCount) {
    DEBUG ((DEBUG_ERROR, "%a: Wrong PcdPciEXhci format\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  /* Allocate and fill board description */
  BoardDesc = AllocateZeroPool (XhciDeviceTableSize * sizeof (MV_BOARD_XHCI_DESC));
  if (BoardDesc == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  XhciIndex = 0;
  for (Index = 0; Index < XhciDeviceTableSize; Index++) {
    if (!XhciDeviceEnabled[Index]) {
      DEBUG ((DEBUG_INFO, "%a: Skip Xhci controller %d\n", __FUNCTION__, Index));
      continue;
    }

    BoardDesc[XhciIndex].SoC = &SoCDesc[Index];
    XhciIndex++;
  }

  BoardDesc->XhciDevCount = XhciIndex;

  *XhciDesc = BoardDesc;

  return EFI_SUCCESS;
}

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
STATIC
EFI_STATUS
MvBoardPcieDescriptionGet (
  IN MARVELL_BOARD_DESC_PROTOCOL          *This,
  IN OUT MV_BOARD_PCIE_DESCRIPTION CONST **PcieDescription
  )
{
  UINTN SoCPcieControllerCount, BoardPcieControllerCount, SoCIndex, BoardIndex;
  EFI_PHYSICAL_ADDRESS *PcieDbiAddresses;
  MV_PCIE_CONTROLLER CONST *PcieControllers;
  EFI_STATUS Status;

  /* Use existing structure if already created. */
  if (mPcieDescription != NULL) {
    *PcieDescription = mPcieDescription;
    return EFI_SUCCESS;
  }

  /* Get SoC data about all available PCIE controllers. */
  Status = ArmadaSoCPcieGet (&PcieDbiAddresses, &SoCPcieControllerCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /* Get per-board information about all used PCIE controllers. */
  Status = ArmadaBoardPcieControllerGet (&PcieControllers,
             &BoardPcieControllerCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /* Sanity check of the board description. */
  if (BoardPcieControllerCount > SoCPcieControllerCount) {
    DEBUG ((DEBUG_ERROR, "%a: Too many controllers described\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  for (BoardIndex = 0; BoardIndex < BoardPcieControllerCount; BoardIndex++) {
    for (SoCIndex = 0; SoCIndex < SoCPcieControllerCount; SoCIndex++) {
      if (PcieControllers[BoardIndex].PcieDbiAddress ==
          PcieDbiAddresses[SoCIndex]) {
          /* Match found */
          break;
      }
    }
    if (SoCIndex == SoCPcieControllerCount) {
      DEBUG ((DEBUG_ERROR,
        "%a: Controller #%d base address invalid: 0x%x\n",
        __FUNCTION__,
        BoardIndex,
        PcieControllers[BoardIndex].PcieDbiAddress));
      return EFI_INVALID_PARAMETER;
    }
  }

  /* Allocate and fill board description. */
  mPcieDescription = AllocateZeroPool (sizeof (MV_BOARD_PCIE_DESCRIPTION));
  if (mPcieDescription == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  mPcieDescription->PcieControllers = PcieControllers;
  mPcieDescription->PcieControllerCount = BoardPcieControllerCount;

  *PcieDescription = mPcieDescription;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvBoardDescPp2Get (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_PP2_DESC       **Pp2Desc
  )
{
  UINT8 *Pp2DeviceEnabled;
  UINTN Pp2Count, Pp2DeviceTableSize, Pp2Index, Index;
  MV_BOARD_PP2_DESC *BoardDesc;
  MV_SOC_PP2_DESC *SoCDesc;
  EFI_STATUS Status;

  /* Get SoC data about all available PP2 controllers */
  Status = ArmadaSoCDescPp2Get (&SoCDesc, &Pp2Count);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /*
   * Obtain table with enabled Pp2 controllers,
   * which is represented as an array of UINT8 values
   * (0x0 - disabled, 0x1 enabled).
   */
  Pp2DeviceEnabled = PcdGetPtr (PcdPp2Controllers);
  if (Pp2DeviceEnabled == NULL) {
    /* No PP2 NIC on platform */
    return EFI_SUCCESS;
  }

  Pp2DeviceTableSize = PcdGetSize (PcdPp2Controllers);

  /* Check if PCD with PP2 NICs is correctly defined */
  if (Pp2DeviceTableSize > Pp2Count) {
    DEBUG ((DEBUG_ERROR, "%a: Wrong PcdPp2Controllers format\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  /* Allocate and fill board description */
  BoardDesc = AllocateZeroPool (Pp2DeviceTableSize * sizeof (MV_BOARD_PP2_DESC));
  if (BoardDesc == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  Pp2Index = 0;
  for (Index = 0; Index < Pp2DeviceTableSize; Index++) {
    if (!Pp2DeviceEnabled[Index]) {
      continue;
    }

    BoardDesc[Pp2Index].SoC = &SoCDesc[Index];
    Pp2Index++;
  }

  BoardDesc->Pp2DevCount = Pp2Index;

  *Pp2Desc = BoardDesc;

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
MvBoardDescUtmiGet (
  IN MARVELL_BOARD_DESC_PROTOCOL  *This,
  IN OUT MV_BOARD_UTMI_DESC      **UtmiDesc
  )
{
  UINT8 *UtmiDeviceEnabled, *XhciDeviceEnabled, *UtmiPortType;
  UINTN UtmiCount, UtmiDeviceTableSize, UtmiIndex, Index;
  MV_BOARD_UTMI_DESC *BoardDesc;
  MV_SOC_UTMI_DESC *SoCDesc;
  EFI_STATUS Status;

  /* Get SoC data about all available UTMI controllers */
  Status = ArmadaSoCDescUtmiGet (&SoCDesc, &UtmiCount);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  /*
   * Obtain table with enabled Utmi PHY's,
   * which is represented as an array of UINT8 values
   * (0x0 - disabled, 0x1 enabled).
   */
  UtmiDeviceEnabled = PcdGetPtr (PcdUtmiControllersEnabled);
  if (UtmiDeviceEnabled == NULL) {
    /* No UTMI PHY on platform */
    return EFI_SUCCESS;
  }

  /* Make sure XHCI controllers table is present */
  XhciDeviceEnabled = PcdGetPtr (PcdPciEXhci);
  if (XhciDeviceEnabled == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Missing PcdPciEXhci\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  UtmiDeviceTableSize = PcdGetSize (PcdUtmiControllersEnabled);

  /* Check if PCD with UTMI PHYs is correctly defined */
  if ((UtmiDeviceTableSize > UtmiCount) ||
      (UtmiDeviceTableSize > PcdGetSize (PcdPciEXhci))) {
    DEBUG ((DEBUG_ERROR,
      "%a: Wrong PcdUtmiControllersEnabled format\n",
      __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  /* Obtain port type table - also stored as UINT8 array */
  UtmiPortType = PcdGetPtr (PcdUtmiPortType);
  if ((UtmiPortType == NULL) ||
      (PcdGetSize (PcdUtmiPortType) != UtmiDeviceTableSize)) {
    DEBUG ((DEBUG_ERROR, "%a: Wrong PcdUtmiPortType format\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  /* Allocate and fill board description */
  BoardDesc = AllocateZeroPool (UtmiDeviceTableSize * sizeof (MV_BOARD_UTMI_DESC));
  if (BoardDesc == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  }

  UtmiIndex = 0;
  for (Index = 0; Index < UtmiDeviceTableSize; Index++) {
    if (!UtmiDeviceEnabled[Index]) {
      continue;
    }

    /* UTMI PHY without enabled XHCI controller is useless */
    if (!XhciDeviceEnabled[Index]) {
      DEBUG ((DEBUG_ERROR,
             "%a: Disabled Xhci controller %d\n",
             Index,
             __FUNCTION__));
      return EFI_INVALID_PARAMETER;
    }

    BoardDesc[UtmiIndex].SoC = &SoCDesc[Index];
    BoardDesc[UtmiIndex].UtmiPortType = UtmiPortType[Index];
    UtmiIndex++;
  }

  BoardDesc->UtmiDevCount = UtmiIndex;

  *UtmiDesc = BoardDesc;

  return EFI_SUCCESS;
}

STATIC
VOID
MvBoardDescFree (
  IN VOID *BoardDesc
  )
{
  FreePool (BoardDesc);
}

STATIC
EFI_STATUS
MvBoardDescInitProtocol (
  IN MARVELL_BOARD_DESC_PROTOCOL *BoardDescProtocol
  )
{
  BoardDescProtocol->BoardDescAhciGet = MvBoardDescAhciGet;
  BoardDescProtocol->BoardDescComPhyGet = MvBoardDescComPhyGet;
  BoardDescProtocol->BoardDescI2cGet = MvBoardDescI2cGet;
  BoardDescProtocol->BoardDescMdioGet = MvBoardDescMdioGet;
  BoardDescProtocol->BoardDescPp2Get = MvBoardDescPp2Get;
  BoardDescProtocol->BoardDescSdMmcGet = MvBoardDescSdMmcGet;
  BoardDescProtocol->BoardDescUtmiGet = MvBoardDescUtmiGet;
  BoardDescProtocol->BoardDescXhciGet = MvBoardDescXhciGet;
  BoardDescProtocol->BoardDescFree = MvBoardDescFree;
  BoardDescProtocol->GpioDescriptionGet = MvBoardGpioDescriptionGet;
  BoardDescProtocol->PcieDescriptionGet = MvBoardPcieDescriptionGet;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MvBoardDescEntryPoint (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS Status;

  mBoardDescInstance = AllocateZeroPool (sizeof (MV_BOARD_DESC));
  if (mBoardDescInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  MvBoardDescInitProtocol (&mBoardDescInstance->BoardDescProtocol);

  mBoardDescInstance->Signature = MV_BOARD_DESC_SIGNATURE;

  Status = gBS->InstallMultipleProtocolInterfaces (&(mBoardDescInstance->Handle),
                  &gMarvellBoardDescProtocolGuid,
                  &(mBoardDescInstance->BoardDescProtocol));
  if (EFI_ERROR (Status)) {
    FreePool (mBoardDescInstance);
    return Status;
  }

  return EFI_SUCCESS;
}
