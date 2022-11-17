/********************************************************************************
Copyright (C) 2016 Marvell International Ltd.

SPDX-License-Identifier: BSD-2-Clause-Patent

*******************************************************************************/

#include "ComPhyLib.h"
#include <Library/MvComPhyLib.h>

CHAR16 * TypeStringTable [] = {L"unconnected", L"PCIE0", L"PCIE1", L"PCIE2",
                           L"PCIE3", L"SATA0", L"SATA1", L"SATA2", L"SATA3",
                           L"SGMII0", L"SGMII1", L"SGMII2", L"SGMII3",
                           L"QSGMII", L"USB3_HOST0", L"USB3_HOST1",
                           L"USB3_DEVICE", L"XAUI0", L"XAUI1", L"XAUI2",
                           L"XAUI3", L"RXAUI0", L"RXAUI1", L"SFI"};

CHAR16 * SpeedStringTable [] = {L"1.25 Gbps", L"2.5 Gbps", L"3.125 Gbps",
                                L"5 Gbps", L"5.156 Gbps", L"6 Gbps",
                                L"10.31 Gbps"};

CHIP_COMPHY_CONFIG ChipCfgTbl[] = {
  {
    .ChipType = MvComPhyTypeCp110,
    .Init = ComPhyCp110Init
  },
};

VOID
RegSet (
  IN EFI_PHYSICAL_ADDRESS Addr,
  IN UINT32 Data,
  IN UINT32 Mask
  )
{
  DEBUG((DEBUG_INFO, "Write to address = %10x, data = %10x (mask = %10x)"
    "- ", Addr, Data, Mask));
  DEBUG((DEBUG_INFO, "old value = %10x ==> ", MmioRead32 (Addr)));
  RegSetSilent (Addr, Data, Mask);
  DEBUG((DEBUG_INFO, "new value %10x\n", MmioRead32 (Addr)));
}

VOID
RegSetSilent (
  IN EFI_PHYSICAL_ADDRESS Addr,
  IN UINT32 Data,
  IN UINT32 Mask
  )
{
  UINT32 RegData;

  RegData = MmioRead32 (Addr);
  RegData &= ~Mask;
  RegData |= Data;
  MmioWrite32 (Addr, RegData);
}

CHAR16 *
GetTypeString (
  UINT32 Type
  )
{

  if (Type < 0 || Type > COMPHY_TYPE_MAX) {
    return L"invalid";
  }

  return TypeStringTable[Type];
}

CHAR16 *
GetSpeedString (
  UINT32 Speed
  )
{

  if (Speed == COMPHY_SPEED_DEFAULT) {
    return L"default";
  }

  if (Speed < 0 || Speed > COMPHY_SPEED_MAX) {
    return L"invalid";
  }

  return SpeedStringTable[Speed];
}

VOID
ComPhyPrint (
  IN CHIP_COMPHY_CONFIG *PtrChipCfg,
  IN UINT8 Index
  )
{
  UINT32 Lane;
  CHAR16 *SpeedStr, *TypeStr;

  for (Lane = 0; Lane < PtrChipCfg->LanesCount; Lane++) {
    SpeedStr = GetSpeedString(PtrChipCfg->MapData[Lane].Speed);
    TypeStr = GetTypeString(PtrChipCfg->MapData[Lane].Type);
    DEBUG ((DEBUG_ERROR, "Comphy%d-%d: %-13s %-10s\n", Index, Lane, TypeStr, SpeedStr));
  }

  DEBUG((DEBUG_ERROR, "\n"));
}

EFI_STATUS
GetChipComPhyInit (
  IN CHIP_COMPHY_CONFIG *PtrChipCfg
  )
{
  UINTN i, TblSize;


  TblSize = sizeof(ChipCfgTbl) / sizeof(ChipCfgTbl[0]);

  for (i = 0; i < TblSize ; i++) {
    if (PtrChipCfg->ChipType == ChipCfgTbl[i].ChipType) {
      PtrChipCfg->Init = ChipCfgTbl[i].Init;
      return EFI_SUCCESS;
    }
  }

  return EFI_D_ERROR;
}

STATIC
VOID
InitComPhyConfig (
  IN  OUT  CHIP_COMPHY_CONFIG *ChipConfig,
  IN  OUT  PCD_LANE_MAP       *LaneData,
  IN       MV_BOARD_COMPHY_DESC *Desc
  )
{
  ChipConfig->ChipType = Desc->SoC->ComPhyChipType;
  ChipConfig->ComPhyBaseAddr = Desc->SoC->ComPhyBaseAddress;
  ChipConfig->Hpipe3BaseAddr = Desc->SoC->ComPhyHpipe3BaseAddress;
  ChipConfig->LanesCount = Desc->SoC->ComPhyLaneCount;
  ChipConfig->MuxBitCount = Desc->SoC->ComPhyMuxBitCount;
  ChipConfig->ChipId = Desc->SoC->ComPhyId;

  /*
   * Below macro contains variable name concatenation (used to form PCD's name).
   */
  switch (ChipConfig->ChipId) {
  case 0:
    GetComPhyPcd (LaneData, 0);
    break;
  case 1:
    GetComPhyPcd (LaneData, 1);
    break;
  case 2:
    GetComPhyPcd (LaneData, 2);
    break;
  case 3:
    GetComPhyPcd (LaneData, 3);
    break;
  }
}

EFI_STATUS
MvComPhyInit (
  VOID
  )
{
  EFI_STATUS Status;
  CHIP_COMPHY_CONFIG *ChipConfig, *PtrChipCfg;
  MARVELL_BOARD_DESC_PROTOCOL *BoardDescProtocol;
  MV_BOARD_COMPHY_DESC *ComPhyBoardDesc;
  PCD_LANE_MAP *LaneData;
  UINT32 Lane, MaxComphyCount;
  UINTN Index;

  /* Obtain table with enabled ComPhy devices */
  Status = gBS->LocateProtocol (&gMarvellBoardDescProtocolGuid,
                  NULL,
                  (VOID **)&BoardDescProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = BoardDescProtocol->BoardDescComPhyGet (BoardDescProtocol,
                                &ComPhyBoardDesc);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ChipConfig = AllocateZeroPool (ComPhyBoardDesc->ComPhyDevCount *
                                 sizeof (CHIP_COMPHY_CONFIG));
  if (ChipConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    BoardDescProtocol->BoardDescFree (ComPhyBoardDesc);
    return EFI_OUT_OF_RESOURCES;
  }

  LaneData = AllocateZeroPool (ComPhyBoardDesc->ComPhyDevCount *
                               sizeof (PCD_LANE_MAP));
  if (ChipConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot allocate memory\n", __FUNCTION__));
    BoardDescProtocol->BoardDescFree (ComPhyBoardDesc);
    FreePool (ChipConfig);
    return EFI_OUT_OF_RESOURCES;
  }

  /* Initialize enabled chips */
  for (Index = 0; Index < ComPhyBoardDesc->ComPhyDevCount; Index++) {

    PtrChipCfg = &ChipConfig[Index];
    InitComPhyConfig (PtrChipCfg, LaneData, &ComPhyBoardDesc[Index]);

    /* Get the count of the SerDes of the specific chip */
    MaxComphyCount = PtrChipCfg->LanesCount;
    for (Lane = 0; Lane < MaxComphyCount; Lane++) {
      PtrChipCfg->MapData[Lane].Type = LaneData[Index].Type[Lane];
      PtrChipCfg->MapData[Lane].Speed = LaneData[Index].SpeedValue[Lane];
      PtrChipCfg->MapData[Lane].Invert = LaneData[Index].InvFlag[Lane];

      if ((PtrChipCfg->MapData[Lane].Speed == COMPHY_SPEED_INVALID) ||
          (PtrChipCfg->MapData[Lane].Type == COMPHY_TYPE_INVALID)) {
        DEBUG((DEBUG_ERROR, "ComPhy: No valid phy speed or type for lane %d, "
          "setting lane as unconnected\n", Lane + 1));
        PtrChipCfg->MapData[Lane].Type = COMPHY_TYPE_UNCONNECTED;
        PtrChipCfg->MapData[Lane].Speed = COMPHY_SPEED_INVALID;
      }
    };

    Status = GetChipComPhyInit (PtrChipCfg);
    if (EFI_ERROR(Status)) {
     DEBUG ((DEBUG_ERROR, "ComPhy: Invalid Chip%d type\n", Index));
     return Status;
    }

    ComPhyPrint (PtrChipCfg, Index);

    /* PHY power UP sequence */
    PtrChipCfg->Init (PtrChipCfg);
  }

  BoardDescProtocol->BoardDescFree (ComPhyBoardDesc);
  FreePool (ChipConfig);
  FreePool (LaneData);

  return EFI_SUCCESS;
}
