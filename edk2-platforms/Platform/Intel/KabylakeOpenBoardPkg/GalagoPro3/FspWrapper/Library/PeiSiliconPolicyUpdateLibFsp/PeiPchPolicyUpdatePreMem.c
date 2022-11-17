/** @file
  Intel PCH PEI Policy initialization.

Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "PeiPchPolicyUpdate.h"
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Guid/GlobalVariable.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchHsioLib.h>
#include <Library/PchPcieRpLib.h>
#include <PchHsioPtssTables.h>
#include <Library/DebugLib.h>

VOID
InstallPlatformHsioPtssTable (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  HSIO_PTSS_TABLES            *UnknowPtssTables;
  HSIO_PTSS_TABLES            *SpecificPtssTables;
  HSIO_PTSS_TABLES            *PtssTables;
  UINT8                       PtssTableIndex;
  UINT32                      UnknowTableSize;
  UINT32                      SpecificTableSize;
  UINT32                      TableSize;
  UINT32                      Entry;
  UINT8                       LaneNum;
  UINT8                       Index;
  UINT8                       MaxSataPorts;
  UINT8                       MaxPciePorts;
  UINT8                       PcieTopologyReal[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8                       PciePort;
  UINTN                       RpBase;
  UINTN                       RpDevice;
  UINTN                       RpFunction;
  UINT32                      StrapFuseCfg;
  UINT8                       PcieControllerCfg;
  EFI_STATUS                  Status;

  UnknowPtssTables = NULL;
  UnknowTableSize = 0;
  SpecificPtssTables = NULL;
  SpecificTableSize = 0;

  if (GetPchGeneration () == SklPch) {
    switch (PchStepping ()) {
      case PchLpB0:
      case PchLpB1:
        UnknowPtssTables = (VOID *) (UINTN) PcdGet32 (PcdUnknowLpHsioPtssTable1);
        UnknowTableSize = PcdGet16 (PcdUnknowLpHsioPtssTable1Size);
        SpecificPtssTables = (VOID *) (UINTN) PcdGet32 (PcdSpecificLpHsioPtssTable1);
        SpecificTableSize = PcdGet16 (PcdSpecificLpHsioPtssTable1Size);
        break;
      case PchLpC0:
      case PchLpC1:
        UnknowPtssTables = (VOID *) (UINTN) PcdGet32 (PcdUnknowLpHsioPtssTable2);
        UnknowTableSize = PcdGet16 (PcdUnknowLpHsioPtssTable2Size);
        SpecificPtssTables = (VOID *) (UINTN) PcdGet32 (PcdSpecificLpHsioPtssTable2);
        SpecificTableSize = PcdGet16 (PcdSpecificLpHsioPtssTable2Size);
        break;
      case PchHB0:
      case PchHC0:
        UnknowPtssTables = (VOID *) (UINTN) PcdGet32 (PcdUnknowHHsioPtssTable1);
        UnknowTableSize = PcdGet16 (PcdUnknowHHsioPtssTable1Size);
        SpecificPtssTables = (VOID *) (UINTN) PcdGet32 (PcdSpecificHHsioPtssTable1);
        SpecificTableSize = PcdGet16 (PcdSpecificHHsioPtssTable1Size);
        break;
      case PchHD0:
      case PchHD1:
        UnknowPtssTables = (VOID *) (UINTN) PcdGet32 (PcdUnknowHHsioPtssTable2);
        UnknowTableSize = PcdGet16 (PcdUnknowHHsioPtssTable2Size);
        SpecificPtssTables = (VOID *) (UINTN) PcdGet32 (PcdSpecificHHsioPtssTable2);
        SpecificTableSize = PcdGet16 (PcdSpecificHHsioPtssTable2Size);
        break;
      default:
        UnknowPtssTables = NULL;
        UnknowTableSize = 0;
        SpecificPtssTables = NULL;
        SpecificTableSize = 0;
        DEBUG ((DEBUG_ERROR, "Unsupported PCH Stepping\n"));
    }
  } else {
    switch (PchStepping ()) {
      case KblPchHA0:
        UnknowPtssTables = (VOID *) (UINTN) PcdGet32 (PcdUnknowHHsioPtssTable2);
        UnknowTableSize = PcdGet16 (PcdUnknowHHsioPtssTable2Size);
        SpecificPtssTables = (VOID *) (UINTN) PcdGet32 (PcdSpecificHHsioPtssTable2);
        SpecificTableSize = PcdGet16 (PcdSpecificHHsioPtssTable2Size);
        break;
      default:
        UnknowPtssTables = NULL;
        UnknowTableSize = 0;
        SpecificPtssTables = NULL;
        SpecificTableSize = 0;
        DEBUG ((DEBUG_ERROR, "Unsupported PCH Stepping\n"));
    }
  }

  PtssTableIndex = 0;
  MaxSataPorts = GetPchMaxSataPortNum ();
  MaxPciePorts = GetPchMaxPciePortNum ();
  ZeroMem (PcieTopologyReal, sizeof (PcieTopologyReal));

  //Populate PCIe topology based on lane configuration
  for (PciePort = 0; PciePort < MaxPciePorts; PciePort += 4) {
    Status = GetPchPcieRpDevFun (PciePort, &RpDevice, &RpFunction);
    ASSERT_EFI_ERROR (Status);

    RpBase = MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, (UINT32) RpDevice, (UINT32) RpFunction);
    StrapFuseCfg = MmioRead32 (RpBase + R_PCH_PCIE_STRPFUSECFG);
    PcieControllerCfg = (UINT8) ((StrapFuseCfg & B_PCH_PCIE_STRPFUSECFG_RPC) >> N_PCH_PCIE_STRPFUSECFG_RPC);
    DEBUG ((DEBUG_INFO, "PCIE Port %d StrapFuseCfg Value = %d\n", PciePort, PcieControllerCfg));
  }
  for (Index = 0; Index < MaxPciePorts; Index++) {
    DEBUG ((DEBUG_INFO, "PCIE PTSS Assigned RP %d Topology = %d\n", Index, PcieTopologyReal[Index]));
  }

  //Case 1: BoardId is known, Topology is known/unknown
  //Case 1a: SATA
  PtssTables = SpecificPtssTables;
  TableSize = SpecificTableSize;
  for (Index = 0; Index < MaxSataPorts; Index++) {
    if (PchGetSataLaneNum (Index, &LaneNum) == EFI_SUCCESS) {
      for (Entry = 0; Entry < TableSize; Entry++) {
        if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
            (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_SATA)
          )
        {
          PtssTableIndex++;
          if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_PCH_HSIO_RX_DWORD20) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_PCH_HSIO_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0) == (UINT32) B_PCH_HSIO_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0)) {
            FspmUpd->FspmConfig.PchSataHsioRxGen3EqBoostMagEnable[Index] = TRUE;
            FspmUpd->FspmConfig.PchSataHsioRxGen3EqBoostMag[Index] = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_PCH_HSIO_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0;
          } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_PCH_HSIO_TX_DWORD8)) {
            if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0) == (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0) {
              FspmUpd->FspmConfig.PchSataHsioTxGen1DownscaleAmpEnable[Index] = TRUE;
              FspmUpd->FspmConfig.PchSataHsioTxGen1DownscaleAmp[Index] = (UINT8)((PtssTables[Entry].PtssTable.Value & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0) >> N_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0);
            }
            if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0) == (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0) {
              FspmUpd->FspmConfig.PchSataHsioTxGen2DownscaleAmpEnable[Index] = TRUE;
              FspmUpd->FspmConfig.PchSataHsioTxGen2DownscaleAmp[Index] = (UINT8)((PtssTables[Entry].PtssTable.Value & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0) >> N_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0);
            }
          } else {
            ASSERT (FALSE);
          }
        }
      }
    }
  }
  //Case 1b: PCIe
  for (Index = 0; Index < MaxPciePorts; Index++) {
    if (PchGetPcieLaneNum (Index, &LaneNum) == EFI_SUCCESS) {
      for (Entry = 0; Entry < TableSize; Entry++) {
        if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
            (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) &&
            (PcieTopologyReal[Index] == PtssTables[Entry].Topology)) {
          PtssTableIndex++;
          if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_PCH_HSIO_RX_DWORD25) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_PCH_HSIO_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0) == (UINT32) B_PCH_HSIO_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0)) {
            FspmUpd->FspmConfig.PchPcieHsioRxSetCtleEnable[Index] = TRUE;
            FspmUpd->FspmConfig.PchPcieHsioRxSetCtle[Index] = (UINT8)((PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_PCH_HSIO_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0);
          } else {
            ASSERT (FALSE);
          }
        }
      }
    }
  }
  //Case 2: BoardId is unknown, Topology is known/unknown
  if (PtssTableIndex == 0) {
    DEBUG ((DEBUG_INFO, "PTSS Settings for unknown board will be applied\n"));

    PtssTables = UnknowPtssTables;
    TableSize = UnknowTableSize;

    for (Index = 0; Index < MaxSataPorts; Index++) {
      if (PchGetSataLaneNum (Index, &LaneNum) == EFI_SUCCESS) {
        for (Entry = 0; Entry < TableSize; Entry++) {
          if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
              (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_SATA)
             )
          {
            if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_PCH_HSIO_RX_DWORD20) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_PCH_HSIO_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0) == (UINT32) B_PCH_HSIO_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0)) {
              FspmUpd->FspmConfig.PchSataHsioRxGen3EqBoostMagEnable[Index] = TRUE;
              FspmUpd->FspmConfig.PchSataHsioRxGen3EqBoostMag[Index] = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_PCH_HSIO_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0;
            } else if (PtssTables[Entry].PtssTable.Offset == (UINT32) R_PCH_HSIO_TX_DWORD8) {
              if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0) == (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0) {
                FspmUpd->FspmConfig.PchSataHsioTxGen1DownscaleAmpEnable[Index] = TRUE;
                FspmUpd->FspmConfig.PchSataHsioTxGen1DownscaleAmp[Index] = (UINT8)((PtssTables[Entry].PtssTable.Value & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0) >> N_PCH_HSIO_TX_DWORD8_ORATE00MARGIN_5_0);
              }
              if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0) == (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0) {
                FspmUpd->FspmConfig.PchSataHsioTxGen2DownscaleAmpEnable[Index] = TRUE;
                FspmUpd->FspmConfig.PchSataHsioTxGen2DownscaleAmp[Index] = (UINT8)((PtssTables[Entry].PtssTable.Value & (UINT32) B_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0) >> N_PCH_HSIO_TX_DWORD8_ORATE01MARGIN_5_0);
              }
            } else {
              ASSERT (FALSE);
            }
          }
        }
      }
    }
    for (Index = 0; Index < MaxPciePorts; Index++) {
      if (PchGetPcieLaneNum (Index, &LaneNum) == EFI_SUCCESS) {
        for (Entry = 0; Entry < TableSize; Entry++) {
          if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
              (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) &&
              (PcieTopologyReal[Index] == PtssTables[Entry].Topology)) {
            if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_PCH_HSIO_RX_DWORD25) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_PCH_HSIO_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0) == (UINT32) B_PCH_HSIO_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0)) {
              FspmUpd->FspmConfig.PchPcieHsioRxSetCtleEnable[Index] = TRUE;
              FspmUpd->FspmConfig.PchPcieHsioRxSetCtle[Index] = (UINT8)((PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_PCH_HSIO_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0);
            } else {
              ASSERT (FALSE);
            }
          }
        }
      }
    }
  }
}

/**
  Performs FSP PCH PEI Policy pre mem initialization.

  @param[in][out]  FspmUpd             Pointer to FSP UPD Data.

  @retval          EFI_SUCCESS         FSP UPD Data is updated.
  @retval          EFI_NOT_FOUND       Fail to locate required PPI.
  @retval          Other               FSP UPD Data update process fail.
**/
EFI_STATUS
EFIAPI
PeiFspPchPolicyUpdatePreMem (
  IN OUT FSPM_UPD    *FspmUpd
  )
{
  InstallPlatformHsioPtssTable (FspmUpd);
  return EFI_SUCCESS;
}

