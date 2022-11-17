/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include <Protocol/DriverBinding.h>
#include <Protocol/Mdio.h>
#include <Protocol/MvPhy.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include "MvPhyDxe.h"

#define TIMEOUT   500

STATIC MARVELL_MDIO_PROTOCOL *Mdio;

//
// Table with available Mdio controllers
//
STATIC UINT8 * CONST MdioDeviceTable = PcdGetPtr (PcdMdioControllersEnabled);
//
// Table with PHY to Mdio controller mappings
//
STATIC UINT8 * CONST Phy2MdioController = PcdGetPtr (PcdPhy2MdioController);
//
// Table with PHYs' SMI addresses
//
STATIC UINT8 * CONST PhySmiAddresses = PcdGetPtr (PcdPhySmiAddresses);

STATIC MV_PHY_DEVICE MvPhyDevices[] = {
  { MV_PHY_DEVICE_1512, MvPhyInit1512 },
  { MV_PHY_DEVICE_1112, MvPhyInit1112 },
  { 0, NULL }
};

EFI_STATUS
MvPhyStatus (
  IN CONST MARVELL_PHY_PROTOCOL *This,
  IN PHY_DEVICE  *PhyDev
  );

EFI_STATUS
MvPhyReset (
  IN PHY_DEVICE *PhyDev
  )
{
  UINT32 Reg = 0;
  INTN timeout = TIMEOUT;

  Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MII_BMCR, &Reg);
  Reg |= BMCR_RESET;
  Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MII_BMCR, Reg);

  while ((Reg & BMCR_RESET) && timeout--) {
    Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MII_BMCR, &Reg);
    gBS->Stall(1000);
  }

  if (Reg & BMCR_RESET) {
    DEBUG((DEBUG_ERROR, "PHY reset timed out\n"));
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

/* Marvell 88E1111S */
EFI_STATUS
MvPhyM88e1111sConfig (
  IN PHY_DEVICE *PhyDev
  )
{
  UINT32 Reg;

  if ((PhyDev->Connection == PHY_CONNECTION_RGMII) ||
      (PhyDev->Connection == PHY_CONNECTION_RGMII_ID) ||
      (PhyDev->Connection == PHY_CONNECTION_RGMII_RXID) ||
      (PhyDev->Connection == PHY_CONNECTION_RGMII_TXID)) {
    Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1111_PHY_EXT_CR, &Reg);

    if ((PhyDev->Connection == PHY_CONNECTION_RGMII) ||
      (PhyDev->Connection == PHY_CONNECTION_RGMII_ID)) {
      Reg |= (MIIM_88E1111_RX_DELAY | MIIM_88E1111_TX_DELAY);
    } else if (PhyDev->Connection == PHY_CONNECTION_RGMII_RXID) {
      Reg &= ~MIIM_88E1111_TX_DELAY;
      Reg |= MIIM_88E1111_RX_DELAY;
    } else if (PhyDev->Connection == PHY_CONNECTION_RGMII_TXID) {
      Reg &= ~MIIM_88E1111_RX_DELAY;
      Reg |= MIIM_88E1111_TX_DELAY;
    }

    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1111_PHY_EXT_CR, Reg);

    Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1111_PHY_EXT_SR, &Reg);

    Reg &= ~(MIIM_88E1111_HWCFG_MODE_MASK);

    if (Reg & MIIM_88E1111_HWCFG_FIBER_COPPER_RES)
      Reg |= MIIM_88E1111_HWCFG_MODE_FIBER_RGMII;
    else
      Reg |= MIIM_88E1111_HWCFG_MODE_COPPER_RGMII;

    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1111_PHY_EXT_SR, Reg);
  }

  if (PhyDev->Connection == PHY_CONNECTION_SGMII) {
    Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1111_PHY_EXT_SR, &Reg);

    Reg &= ~(MIIM_88E1111_HWCFG_MODE_MASK);
    Reg |= MIIM_88E1111_HWCFG_MODE_SGMII_NO_CLK;
    Reg |= MIIM_88E1111_HWCFG_FIBER_COPPER_AUTO;

    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1111_PHY_EXT_SR, Reg);
  }

  if (PhyDev->Connection == PHY_CONNECTION_RTBI) {
    Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1111_PHY_EXT_CR, &Reg);
    Reg |= (MIIM_88E1111_RX_DELAY | MIIM_88E1111_TX_DELAY);
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1111_PHY_EXT_CR, Reg);

    Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1111_PHY_EXT_SR, &Reg);
    Reg &= ~(MIIM_88E1111_HWCFG_MODE_MASK |
      MIIM_88E1111_HWCFG_FIBER_COPPER_RES);
    Reg |= 0x7 | MIIM_88E1111_HWCFG_FIBER_COPPER_AUTO;
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1111_PHY_EXT_SR, Reg);

    /* Soft reset */
    MvPhyReset (PhyDev);

    Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1111_PHY_EXT_SR, &Reg);
    Reg &= ~(MIIM_88E1111_HWCFG_MODE_MASK |
      MIIM_88E1111_HWCFG_FIBER_COPPER_RES);
    Reg |= MIIM_88E1111_HWCFG_MODE_COPPER_RTBI |
      MIIM_88E1111_HWCFG_FIBER_COPPER_AUTO;
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1111_PHY_EXT_SR, Reg);
  }

  Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MII_BMCR, &Reg);
  Reg |= (BMCR_ANENABLE | BMCR_ANRESTART);
  Reg &= ~BMCR_ISOLATE;
  Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MII_BMCR, Reg);

  /* Soft reset */
  MvPhyReset (PhyDev);

  return EFI_SUCCESS;
}

EFI_STATUS
MvPhyParseStatus (
  IN PHY_DEVICE *PhyDev
  )
{
  UINT32 Data;
  UINT32 Speed;

  Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1xxx_PHY_STATUS, &Data);

  if ((Data & MIIM_88E1xxx_PHYSTAT_LINK) &&
    !(Data & MIIM_88E1xxx_PHYSTAT_SPDDONE)) {
    INTN i = 0;

    DEBUG((DEBUG_ERROR,"MvPhyDxe: Waiting for PHY realtime link"));
    while (!(Data & MIIM_88E1xxx_PHYSTAT_SPDDONE)) {
      if (i > PHY_AUTONEGOTIATE_TIMEOUT) {
        DEBUG((DEBUG_ERROR," TIMEOUT !\n"));
        PhyDev->LinkUp = FALSE;
        break;
      }

      if ((i++ % 1000) == 0)
        DEBUG((DEBUG_ERROR, "."));
      gBS->Stall(1000);
      Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MIIM_88E1xxx_PHY_STATUS, &Data);
    }
    DEBUG((DEBUG_ERROR," done\n"));
    gBS->Stall(500000);
  } else {
    if (Data & MIIM_88E1xxx_PHYSTAT_LINK) {
      DEBUG((DEBUG_ERROR, "MvPhyDxe: link up, "));
      PhyDev->LinkUp = TRUE;
    } else {
      DEBUG((DEBUG_ERROR, "MvPhyDxe: link down, "));
      PhyDev->LinkUp = FALSE;
    }
  }

  if (Data & MIIM_88E1xxx_PHYSTAT_DUPLEX) {
    DEBUG((DEBUG_ERROR, "full duplex, "));
    PhyDev->FullDuplex = TRUE;
  } else {
    DEBUG((DEBUG_ERROR, "half duplex, "));
    PhyDev->FullDuplex = FALSE;
  }

  Speed = Data & MIIM_88E1xxx_PHYSTAT_SPEED;

  switch (Speed) {
  case MIIM_88E1xxx_PHYSTAT_GBIT:
    DEBUG((DEBUG_ERROR, "speed 1000\n"));
    PhyDev->Speed = SPEED_1000;
    break;
  case MIIM_88E1xxx_PHYSTAT_100:
    DEBUG((DEBUG_ERROR, "speed 100\n"));
    PhyDev->Speed = SPEED_100;
    break;
  default:
    DEBUG((DEBUG_ERROR, "speed 10\n"));
    PhyDev->Speed = SPEED_10;
    break;
  }

  return EFI_SUCCESS;
}

/**
  Configure PHY device autonegotiation.

  @param[in out]   *PhyDevice      A pointer to configured PHY device structure.

**/
STATIC
EFI_STATUS
MvPhyConfigureAutonegotiation (
  IN OUT PHY_DEVICE *PhyDevice
  )
{
  UINT32 Data;
  INTN Index;

  /* Read BMSR register in order to check autoneg capabilities and status. */
  Mdio->Read (Mdio, PhyDevice->Addr, PhyDevice->MdioIndex, MII_BMSR, &Data);

  if ((Data & BMSR_ANEGCAPABLE) && !(Data & BMSR_ANEGCOMPLETE)) {

    DEBUG ((DEBUG_INFO,
      "%a: Waiting for PHY auto negotiation...",
      __FUNCTION__));

    /* Wait for autonegotiation to complete and read media status */
    for (Index = 0; !(Data & BMSR_ANEGCOMPLETE); Index++) {
      if (Index > PHY_AUTONEGOTIATE_TIMEOUT) {
        DEBUG ((DEBUG_ERROR, "%a: Timeout\n", __FUNCTION__));
        PhyDevice->LinkUp = FALSE;
        return EFI_TIMEOUT;
      }
      gBS->Stall (1000);  /* 1 ms */
      Mdio->Read (Mdio, PhyDevice->Addr, PhyDevice->MdioIndex, MII_BMSR, &Data);
    }

    PhyDevice->LinkUp = TRUE;
    DEBUG ((DEBUG_INFO, "%a: link up\n", __FUNCTION__));
  } else {
    Mdio->Read (Mdio, PhyDevice->Addr, PhyDevice->MdioIndex, MII_BMSR, &Data);

    if (Data & BMSR_LSTATUS) {
      PhyDevice->LinkUp = TRUE;
      DEBUG ((DEBUG_INFO, "%a: link up\n", __FUNCTION__));
    } else {
      PhyDevice->LinkUp = FALSE;
      DEBUG ((DEBUG_INFO, "%a: link down\n", __FUNCTION__));
    }
  }

  return EFI_SUCCESS;
}

STATIC
VOID
MvPhy1512WriteBits (
  IN PHY_DEVICE *PhyDev,
  IN UINT8 RegNum,
  IN UINT16 Offset,
  IN UINT16 Len,
  IN UINT16 Data)
{
  UINT32 Reg, Mask;

  if ((Len + Offset) >= 16)
    Mask = 0 - (1 << Offset);
  else
    Mask = (1 << (Len + Offset)) - (1 << Offset);

  Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, RegNum, &Reg);

  Reg &= ~Mask;
  Reg |= Data << Offset;

  Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, RegNum, Reg);
}

STATIC
EFI_STATUS
MvPhyInit1512 (
    IN CONST MARVELL_PHY_PROTOCOL *Snp,
    IN OUT PHY_DEVICE *PhyDev
    )
{
  EFI_STATUS Status;

  if (PhyDev->Connection == PHY_CONNECTION_SGMII) {
    /* Select page 0xff and update configuration registers according to
     * Marvell Release Notes - Alaska 88E1510/88E1518/88E1512 Rev A0,
     * Errata Section 3.1 - needed in SGMII mode.
     */
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, 22, 0x00ff);
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, 17, 0x214B);
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, 16, 0x2144);
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, 17, 0x0C28);
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, 16, 0x2146);
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, 17, 0xB233);
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, 16, 0x214D);
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, 17, 0xCC0C);
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, 16, 0x2159);

    /* Reset page selection and select page 0x12 */
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, 22, 0x0000);
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, 22, 0x0012);

    /* Write HWCFG_MODE = SGMII to Copper */
    MvPhy1512WriteBits(PhyDev, 20, 0, 3, 1);

    /* Phy reset - necessary after changing mode */
    MvPhy1512WriteBits(PhyDev, 20, 15, 1, 1);

    /* Reset page selection */
    Mdio->Write (Mdio, PhyDev->Addr, PhyDev->MdioIndex, 22, 0x0000);
    gBS->Stall(100);
  }

  MvPhyM88e1111sConfig (PhyDev);

  /* autonegotiation on startup is not always required */
  if (!PcdGetBool (PcdPhyStartupAutoneg))
    return EFI_SUCCESS;

  Status = MvPhyConfigureAutonegotiation (PhyDev);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MvPhyParseStatus (PhyDev);

  return EFI_SUCCESS;
}

/**
  Initialize Marvell 88E1112 PHY.

  @param[in]      MvPhyProtocol   Marvell PHY protocol instance.
  @param[in out] *PhyDevice       PHY device structure.

**/
STATIC
EFI_STATUS
MvPhyInit1112 (
  IN CONST MARVELL_PHY_PROTOCOL  *MvPhyProtocol,
  IN OUT PHY_DEVICE              *PhyDevice
  )
{
  EFI_STATUS Status;

  MvPhyM88e1111sConfig (PhyDevice);

  if (PcdGetBool (PcdPhyStartupAutoneg)) {
    Status = MvPhyConfigureAutonegotiation (PhyDevice);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    MvPhyParseStatus (PhyDevice);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
MvPhyInit (
  IN CONST MARVELL_PHY_PROTOCOL *Snp,
  IN UINT32 PhyIndex,
  IN PHY_CONNECTION PhyConnection,
  IN OUT PHY_DEVICE **OutPhyDev
  )
{
  EFI_STATUS Status;
  PHY_DEVICE *PhyDev;
  UINT8 *DeviceIds;
  UINT8 MdioIndex;
  UINT8 PhyId;

  Status = gBS->LocateProtocol (
      &gMarvellMdioProtocolGuid,
      NULL,
      (VOID **) &Mdio
      );
  if (EFI_ERROR(Status))
    return Status;

  MdioIndex = Phy2MdioController[PhyIndex];

  /* Verify correctness of PHY <-> MDIO assignment */
  if ((MdioDeviceTable[MdioIndex] == 0) ||
      (MdioIndex >= Mdio->ControllerCount)) {
    DEBUG ((DEBUG_ERROR, "MvPhyDxe: Incorrect Mdio controller assignment for PHY#%d", PhyIndex));
    return EFI_INVALID_PARAMETER;
  }

  DeviceIds = PcdGetPtr (PcdPhyDeviceIds);
  PhyId = DeviceIds[PhyIndex];
  if (PhyId >= MV_PHY_DEVICE_ID_MAX) {
    DEBUG ((DEBUG_ERROR,
      "%a, Incorrect PHY ID (0x%x) for PHY#%d\n",
      __FUNCTION__,
      PhyId,
      PhyIndex));
    return EFI_INVALID_PARAMETER;
  }

  /* perform setup common for all PHYs */
  PhyDev = AllocateZeroPool (sizeof (PHY_DEVICE));
  PhyDev->Addr = PhySmiAddresses[PhyIndex];
  PhyDev->Connection = PhyConnection;
  PhyDev->MdioIndex = MdioIndex;
  DEBUG ((DEBUG_INFO,
    "MvPhyDxe: MdioIndex is %d, PhyAddr is %d, connection %d\n",
    PhyDev->MdioIndex,
    PhyDev->Addr,
    PhyConnection));
  *OutPhyDev = PhyDev;

  return MvPhyDevices[PhyId].DevInit (Snp, PhyDev);
}

EFI_STATUS
MvPhyStatus (
  IN CONST MARVELL_PHY_PROTOCOL *This,
  IN PHY_DEVICE  *PhyDev
  )
{
  UINT32 Data;

  Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MII_BMSR, &Data);
  Mdio->Read (Mdio, PhyDev->Addr, PhyDev->MdioIndex, MII_BMSR, &Data);

  if ((Data & BMSR_LSTATUS) == 0) {
    PhyDev->LinkUp = FALSE;
  } else {
    PhyDev->LinkUp = TRUE;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MvPhyDxeInitialise (
  IN EFI_HANDLE  ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  MARVELL_PHY_PROTOCOL *Phy;
  EFI_STATUS Status;
  EFI_HANDLE Handle = NULL;

  Phy = AllocateZeroPool (sizeof (MARVELL_PHY_PROTOCOL));
  Phy->Status = MvPhyStatus;
  Phy->Init = MvPhyInit;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gMarvellPhyProtocolGuid, Phy,
                  NULL
                  );

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed to install interfaces\n"));
    return Status;
  }
  DEBUG((DEBUG_ERROR, "Succesfully installed protocol interfaces\n"));

  return EFI_SUCCESS;
}
