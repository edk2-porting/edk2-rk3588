/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include "ComPhyLib.h"

#include <Library/ArmSmcLib.h>
#include <Library/SampleAtResetLib.h>

#include <IndustryStandard/ArmStdSmc.h>

#define SD_LANE_ADDR_WIDTH          0x1000
#define HPIPE_ADDR_OFFSET           0x800
#define COMPHY_ADDR_LANE_WIDTH      0x28
#define SD_ADDR(base, Lane)         (base + SD_LANE_ADDR_WIDTH * Lane)
#define HPIPE_ADDR(base, Lane)      (SD_ADDR(base, Lane) + HPIPE_ADDR_OFFSET)
#define COMPHY_ADDR(base, Lane)     (base + COMPHY_ADDR_LANE_WIDTH * Lane)

STATIC
VOID
ComPhySataMacPowerDown (
  IN EFI_PHYSICAL_ADDRESS SataBase
)
{
  UINT32 Mask, Data;

  /*
   * MAC configuration - power down ComPhy
   * Use indirect address for vendor specific SATA control register
   */
  RegSet (SataBase + SATA3_VENDOR_ADDRESS,
    SATA_CONTROL_REG << SATA3_VENDOR_ADDR_OFSSET, SATA3_VENDOR_ADDR_MASK);

  /* SATA 0 power down */
  Mask = SATA3_CTRL_SATA0_PD_MASK;
  Data = 0x1 << SATA3_CTRL_SATA0_PD_OFFSET;

  /* SATA 1 power down */
  Mask |= SATA3_CTRL_SATA1_PD_MASK;
  Data |= 0x1 << SATA3_CTRL_SATA1_PD_OFFSET;

  /* SATA SSU disable */
  Mask |= SATA3_CTRL_SATA1_ENABLE_MASK;
  Data |= 0x0 << SATA3_CTRL_SATA1_ENABLE_OFFSET;

  /* SATA port 1 disable */
  Mask |= SATA3_CTRL_SATA_SSU_MASK;
  Data |= 0x0 << SATA3_CTRL_SATA_SSU_OFFSET;
  RegSet (SataBase + SATA3_VENDOR_DATA, Data, Mask);
}

STATIC
VOID
ComPhySataPhyPowerUp (
  IN EFI_PHYSICAL_ADDRESS SataBase
)
{
  UINT32 Data, Mask;

  /*
   * MAC configuration - power up ComPhy - power up PLL/TX/RX
   * Use indirect address for vendor specific SATA control register
   */
  RegSet (SataBase + SATA3_VENDOR_ADDRESS,
    SATA_CONTROL_REG << SATA3_VENDOR_ADDR_OFSSET, SATA3_VENDOR_ADDR_MASK);

  /* SATA 0 power up */
  Mask = SATA3_CTRL_SATA0_PD_MASK;
  Data = 0x0 << SATA3_CTRL_SATA0_PD_OFFSET;

  /* SATA 1 power up */
  Mask |= SATA3_CTRL_SATA1_PD_MASK;
  Data |= 0x0 << SATA3_CTRL_SATA1_PD_OFFSET;

  /* SATA SSU enable */
  Mask |= SATA3_CTRL_SATA1_ENABLE_MASK;
  Data |= 0x1 << SATA3_CTRL_SATA1_ENABLE_OFFSET;

  /* SATA port 1 enable */
  Mask |= SATA3_CTRL_SATA_SSU_MASK;
  Data |= 0x1 << SATA3_CTRL_SATA_SSU_OFFSET;
  RegSet (SataBase + SATA3_VENDOR_DATA, Data, Mask);

  /* MBUS request size and interface select register */
  RegSet (SataBase + SATA3_VENDOR_ADDRESS,
    SATA_MBUS_SIZE_SELECT_REG << SATA3_VENDOR_ADDR_OFSSET,
      SATA3_VENDOR_ADDR_MASK);

  /* Mbus regret enable */
  RegSet (SataBase + SATA3_VENDOR_DATA, 0x1 << SATA_MBUS_REGRET_EN_OFFSET,
    SATA_MBUS_REGRET_EN_MASK);
}

STATIC
EFI_STATUS
ComPhySmc (
    IN UINT32 FunctionId,
    EFI_PHYSICAL_ADDRESS ComPhyBaseAddr,
    IN UINT32 Lane,
    IN UINT32 Mode
    )
{
  ARM_SMC_ARGS  SmcRegs = {0};

  SmcRegs.Arg0 = FunctionId;
  SmcRegs.Arg1 = (UINTN)ComPhyBaseAddr;
  SmcRegs.Arg2 = Lane;
  SmcRegs.Arg3 = Mode;
  ArmCallSmc (&SmcRegs);

  if (SmcRegs.Arg0 != 0) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

STATIC
UINTN
ComPhySataPowerUp (
  IN UINTN ChipId,
  IN UINT32 Lane,
  IN EFI_PHYSICAL_ADDRESS HpipeBase,
  IN EFI_PHYSICAL_ADDRESS ComPhyBase,
  IN MV_BOARD_AHCI_DESC *Desc
  )
{
  EFI_STATUS Status;

  DEBUG ((DEBUG_INFO, "ComPhySata: Initialize SATA PHYs\n"));

  DEBUG((DEBUG_INFO, "ComPhySataPowerUp: stage: MAC configuration - power down ComPhy\n"));

  ComPhySataMacPowerDown (Desc[ChipId].SoC->AhciBaseAddress);

  Status = ComPhySmc (MV_SMC_ID_COMPHY_POWER_ON,
             ComPhyBase,
             Lane,
             COMPHY_FW_FORMAT (COMPHY_SATA_MODE,
               Desc[ChipId].SoC->AhciId,
               COMPHY_SPEED_DEFAULT));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ComPhySataPhyPowerUp (Desc[ChipId].SoC->AhciBaseAddress);

  Status = ComPhySmc (MV_SMC_ID_COMPHY_PLL_LOCK,
             ComPhyBase,
             Lane,
             COMPHY_FW_FORMAT (COMPHY_SATA_MODE,
               Desc[ChipId].SoC->AhciId,
               COMPHY_SPEED_DEFAULT));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return Status;
}

VOID
ComPhyCp110Init (
  IN CHIP_COMPHY_CONFIG *PtrChipCfg
  )
{
  EFI_STATUS Status;
  COMPHY_MAP *PtrComPhyMap, *SerdesMap;
  EFI_PHYSICAL_ADDRESS ComPhyBaseAddr, HpipeBaseAddr;
  MARVELL_BOARD_DESC_PROTOCOL *BoardDescProtocol;
  MV_BOARD_AHCI_DESC *AhciBoardDesc;
  UINT32 ComPhyMaxCount, Lane;
  UINT32 PcieWidth = 0;
  UINT8 ChipId;

  ComPhyMaxCount = PtrChipCfg->LanesCount;
  ComPhyBaseAddr = PtrChipCfg->ComPhyBaseAddr;
  HpipeBaseAddr = PtrChipCfg->Hpipe3BaseAddr;
  SerdesMap = PtrChipCfg->MapData;
  ChipId = PtrChipCfg->ChipId;

  /* Check if the first 4 Lanes configured as By-4 */
  for (Lane = 0, PtrComPhyMap = SerdesMap; Lane < 4; Lane++, PtrComPhyMap++) {
    if (PtrComPhyMap->Type != COMPHY_TYPE_PCIE0) {
      break;
    }
    PcieWidth++;
  }

  for (Lane = 0, PtrComPhyMap = SerdesMap; Lane < ComPhyMaxCount;
       Lane++, PtrComPhyMap++) {
    DEBUG((DEBUG_INFO, "ComPhy: Initialize serdes number %d\n", Lane));
    DEBUG((DEBUG_INFO, "ComPhy: Serdes Type = 0x%x\n", PtrComPhyMap->Type));

    if (Lane >= 4) {
      /* PCIe lanes above the first 4 lanes, can be only by1 */
      PcieWidth = 1;
    }

    switch (PtrComPhyMap->Type) {
    case COMPHY_TYPE_UNCONNECTED:
      continue;
      break;
    case COMPHY_TYPE_PCIE0:
    case COMPHY_TYPE_PCIE1:
    case COMPHY_TYPE_PCIE2:
    case COMPHY_TYPE_PCIE3:
      Status = ComPhySmc (MV_SMC_ID_COMPHY_POWER_ON,
                 PtrChipCfg->ComPhyBaseAddr,
                 Lane,
                 COMPHY_FW_PCIE_FORMAT (PcieWidth,
                   COMPHY_PCIE_MODE,
                   PtrComPhyMap->Speed));
      break;
    case COMPHY_TYPE_SATA0:
    case COMPHY_TYPE_SATA1:
    case COMPHY_TYPE_SATA2:
    case COMPHY_TYPE_SATA3:
      /* Obtain AHCI board description */
      Status = gBS->LocateProtocol (&gMarvellBoardDescProtocolGuid,
                      NULL,
                      (VOID **)&BoardDescProtocol);
      if (EFI_ERROR (Status)) {
        break;
      }

      Status = BoardDescProtocol->BoardDescAhciGet (BoardDescProtocol,
                                    &AhciBoardDesc);
      if (EFI_ERROR (Status)) {
        break;
      }

      Status = ComPhySataPowerUp (ChipId,
                 Lane,
                 HpipeBaseAddr,
                 ComPhyBaseAddr,
                 AhciBoardDesc);

      BoardDescProtocol->BoardDescFree (AhciBoardDesc);
      break;
    case COMPHY_TYPE_USB3_HOST0:
    case COMPHY_TYPE_USB3_HOST1:
      Status = ComPhySmc (MV_SMC_ID_COMPHY_POWER_ON,
                 PtrChipCfg->ComPhyBaseAddr,
                 Lane,
                 COMPHY_FW_MODE_FORMAT (COMPHY_USB3H_MODE));
      break;
    case COMPHY_TYPE_SGMII0:
    case COMPHY_TYPE_SGMII1:
    case COMPHY_TYPE_SGMII2:
    case COMPHY_TYPE_SGMII3:
      Status = ComPhySmc (MV_SMC_ID_COMPHY_POWER_ON,
                 PtrChipCfg->ComPhyBaseAddr,
                 Lane,
                 COMPHY_FW_FORMAT (COMPHY_SGMII_MODE,
                   (PtrComPhyMap->Type - COMPHY_TYPE_SGMII0),
                   PtrComPhyMap->Speed));
      break;
    case COMPHY_TYPE_SFI:
      Status = ComPhySmc (MV_SMC_ID_COMPHY_POWER_ON,
                 PtrChipCfg->ComPhyBaseAddr,
                 Lane,
                 COMPHY_FW_FORMAT (COMPHY_SFI_MODE,
                   COMPHY_UNIT_ID0,
                   PtrComPhyMap->Speed));
      break;
    case COMPHY_TYPE_RXAUI0:
    case COMPHY_TYPE_RXAUI1:
      Status = ComPhySmc (MV_SMC_ID_COMPHY_POWER_ON,
                 PtrChipCfg->ComPhyBaseAddr,
                 Lane,
                 COMPHY_FW_MODE_FORMAT (COMPHY_RXAUI_MODE));
      break;
    default:
      DEBUG((DEBUG_ERROR, "Unknown SerDes Type, skip initialize SerDes %d\n",
        Lane));
      Status = EFI_INVALID_PARAMETER;
      ASSERT (FALSE);
      break;
    }
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "Failed to initialize Lane %d\n with Status = 0x%x", Lane, Status));
      PtrComPhyMap->Type = COMPHY_TYPE_UNCONNECTED;
    }
  }
}
