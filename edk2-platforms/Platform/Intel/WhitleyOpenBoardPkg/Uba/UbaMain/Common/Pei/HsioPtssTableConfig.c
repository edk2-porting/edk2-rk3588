/** @file
  Install Platform Hsio Ptss Table Data.

  @copyright
  Copyright 2018 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "PeiCommonBoardInitLib.h"
#include <Library/UbaHsioPtssTableConfigLib.h>
#include <Library/SpsPeiLib.h>
#include <Library/PchInfoLib.h>
#include <Ppi/PchPolicy.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/PchHsioPtssTable.h>
#include "PchLbgHsioPtssTablesBx.h"
#include "PchLbgHsioPtssTablesBx_Ext.h"
#include "PchLbgHsioPtssTablesSx.h"
#include "PchLbgHsioPtssTablesSx_Ext.h"
#include <Guid/PlatformInfo.h>
#include <Library/IoLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/PchMultiPchBase.h>
#include <Ppi/DynamicSiLibraryPpi.h>


VOID
InstallPlatformHsioPtssTableCallback (
  IN          PCH_SETUP                    *PchSetup,
  IN OUT      PCH_POLICY_PPI               *PchPolicy
  )
{
  HSIO_PTSS_TABLES            *PtssTables;
  UINT8                       PtssTableIndex;
  UINT32                      TableSize;
  UINT32                      Entry;
  UINT8                       LaneNum;
  UINT8                       Index;
  UINT8                       MaxSataPorts;
  UINT8                       MaxsSataPorts;
  UINT8                       MaxPciePorts;
  UINT8                       PcieTopologyReal[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8                       PciePort;
  UINTN                       RpBase;
  UINTN                       RpDevice;
  UINTN                       RpFunction;
  UINT32                      StrapFuseCfg;
  UINT8                       PcieControllerCfg;
  EFI_STATUS                  Status;
  UINT16                      BoardId;
  EFI_HOB_GUID_TYPE                     *GuidHob;
  EFI_PLATFORM_INFO                     *PlatformInfo;
  DYNAMIC_SI_LIBARY_PPI                 *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  switch (DynamicSiLibraryPpi->PchStepping ()) {
    case LbgA0:
    case LbgB0:
    case LbgB1:
    case LbgB2:
    case LbgB3:
      {
        PtssTables = PchLbgHsioPtss_Bx;
        TableSize = PchLbgHsioPtss_Bx_Size;
      }
      if (DynamicSiLibraryPpi->HybridSystemLevelEmulationEnabled ()) {
        PtssTables = PchLbgHsioPtss_Bx;
        TableSize = PchLbgHsioPtss_Bx_Size;
      }
      break;
    case LbgS0:
    case LbgS1:
    case LbgS2:
      {
        PtssTables = PchLbgHsioPtss_Sx;
        TableSize = PchLbgHsioPtss_Sx_Size;
      }
      if (DynamicSiLibraryPpi->HybridSystemLevelEmulationEnabled ()) {
        PtssTables = PchLbgHsioPtss_Sx;
        TableSize = PchLbgHsioPtss_Sx_Size;
      }
      break;
    default:
      PtssTables = NULL;
      TableSize = 0;
      DEBUG ((DEBUG_ERROR, "Cannot find PTSS table for this PCH Stepping\n"));
      ASSERT (FALSE);
  }

  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT(GuidHob != NULL);
  PlatformInfo  = GET_GUID_HOB_DATA(GuidHob);
  BoardId = PlatformInfo->BoardId;
  PtssTableIndex = 0;
  MaxSataPorts = DynamicSiLibraryPpi->GetPchMaxSataPortNum ();
  MaxsSataPorts = DynamicSiLibraryPpi->GetPchMaxsSataPortNum ();
  MaxPciePorts = DynamicSiLibraryPpi->GetPchMaxPciePortNum ();
  ZeroMem (PcieTopologyReal, sizeof (PcieTopologyReal));
  //Populate PCIe topology based on lane configuration

  CopyMem (
    PcieTopologyReal,
    PchSetup->PcieTopology,
    sizeof (PcieTopologyReal)
    );
  for (PciePort = 0; PciePort < MaxPciePorts; PciePort += 4) {
    Status = DynamicSiLibraryPpi->GetPchPcieRpDevFunByPchId (PCH_LEGACY_ID, PciePort, &RpDevice, &RpFunction);
    RpBase = DynamicSiLibraryPpi->MmPciBase (0, (UINT32) RpDevice, (UINT32) RpFunction);
    StrapFuseCfg = MmioRead32 (RpBase + R_PCH_PCIE_STRPFUSECFG);
    PcieControllerCfg = (UINT8) ((StrapFuseCfg & B_PCH_PCIE_STRPFUSECFG_RPC) >> N_PCH_PCIE_STRPFUSECFG_RPC);
    DEBUG ((DEBUG_INFO, "PCIE Port %d StrapFuseCfg Value = %d\n", PciePort, PcieControllerCfg));
    PcieTopologyReal[PciePort] = PchSetup->PcieTopology[PciePort];
    if (PcieControllerCfg != V_PCH_PCIE_STRPFUSECFG_RPC_1_1_1_1) {
      PcieTopologyReal[PciePort + 1] = PchSetup->PcieTopology[PciePort];
    }
    if (PcieControllerCfg == V_PCH_PCIE_STRPFUSECFG_RPC_4) {
      PcieTopologyReal[PciePort + 2] = PchSetup->PcieTopology[PciePort];
      PcieTopologyReal[PciePort + 3] = PchSetup->PcieTopology[PciePort];
    }
    if (PcieControllerCfg == V_PCH_PCIE_STRPFUSECFG_RPC_2_2) {
      PcieTopologyReal[PciePort + 2] = PchSetup->PcieTopology[PciePort + 2];
      PcieTopologyReal[PciePort + 3] = PchSetup->PcieTopology[PciePort + 2];
    }
    if (PcieControllerCfg == V_PCH_PCIE_STRPFUSECFG_RPC_2_1_1) {
      PcieTopologyReal[PciePort + 2] = PchSetup->PcieTopology[PciePort + 2];
      PcieTopologyReal[PciePort + 3] = PchSetup->PcieTopology[PciePort + 3];
    }
  }
  for (Index = 0; Index < MaxPciePorts; Index++) {
    DEBUG ((DEBUG_INFO, "PCIE PTSS Setup RP %d Topology = %d\n", Index, PchSetup->PcieTopology[Index]));
    DEBUG ((DEBUG_INFO, "PCIE PTSS Assigned RP %d Topology = %d\n", Index, PcieTopologyReal[Index]));
  }
  //Case 1: BoardId is known, Topology is known/unknown
  //Case 1a: SATA
  for (Index = 0; Index < MaxSataPorts; Index++) {
    if (DynamicSiLibraryPpi->PchGetSataLaneNumByPchId (PCH_LEGACY_ID, Index, &LaneNum) == EFI_SUCCESS) {
      for (Entry = 0; Entry < TableSize; Entry++) {
        if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
          (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_SATA) &&
          (PchSetup->SataTopology[Index] == PtssTables[Entry].Topology) &&
          (BoardId == PtssTables[Entry].BoardId)) {
          PtssTableIndex++;
          if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD20) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0) == (UINT32) B_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0)) {
            PchPolicy->HsioSataConfig.PortLane[Index].HsioRxGen3EqBoostMagEnable = TRUE;
            PchPolicy->HsioSataConfig.PortLane[Index].HsioRxGen3EqBoostMag = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0;
          } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_TX_DWORD8)) {
            if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0) == (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0) {
              PchPolicy->HsioSataConfig.PortLane[Index].HsioTxGen1DownscaleAmpEnable = TRUE;
              PchPolicy->HsioSataConfig.PortLane[Index].HsioTxGen1DownscaleAmp = (PtssTables[Entry].PtssTable.Value & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0) >> N_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0;
            }
            if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0) == (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0) {
              PchPolicy->HsioSataConfig.PortLane[Index].HsioTxGen2DownscaleAmpEnable = TRUE;
              PchPolicy->HsioSataConfig.PortLane[Index].HsioTxGen2DownscaleAmp = (PtssTables[Entry].PtssTable.Value & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0) >> N_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0;
            }
          } else {
            DEBUG ((DEBUG_ERROR, "ERROR! PTSS programming: The PTSS table offset and/or mask are not compatible with the BIOS Code.\n"));
          }
        }
      }
    }
  }

  //Case 1a continued: Secondary SATA
  for (Index = 0; Index < MaxsSataPorts; Index++) {
    if (DynamicSiLibraryPpi->PchGetsSataLaneNumByPchId (PCH_LEGACY_ID, Index, &LaneNum) == EFI_SUCCESS) {
      for (Entry = 0; Entry < TableSize; Entry++) {
        if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
          (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_SATA) &&
          (PchSetup->sSataTopology[Index] == PtssTables[Entry].Topology) &&
          (BoardId == PtssTables[Entry].BoardId)) {
          PtssTableIndex++;
          if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD20) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0) == (UINT32) B_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0)) {
            PchPolicy->HsiosSataConfig.PortLane[Index].HsioRxGen3EqBoostMagEnable = TRUE;
            PchPolicy->HsiosSataConfig.PortLane[Index].HsioRxGen3EqBoostMag = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0;
          } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_TX_DWORD8)) {
            if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0) == (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0) {
              PchPolicy->HsiosSataConfig.PortLane[Index].HsioTxGen1DownscaleAmpEnable = TRUE;
              PchPolicy->HsiosSataConfig.PortLane[Index].HsioTxGen1DownscaleAmp = (PtssTables[Entry].PtssTable.Value & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0) >> N_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0;
            }
            if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0) == (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0) {
              PchPolicy->HsiosSataConfig.PortLane[Index].HsioTxGen2DownscaleAmpEnable = TRUE;
              PchPolicy->HsiosSataConfig.PortLane[Index].HsioTxGen2DownscaleAmp = (PtssTables[Entry].PtssTable.Value & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0) >> N_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0;
            }
          } else {
            DEBUG ((DEBUG_ERROR, "ERROR! PTSS programming: The PTSS table offset and/or mask are not compatible with the BIOS Code.\n"));
          }
        }
      }
    }
  }

  //Case 1b: PCIe
  for (Index = 0; Index < MaxPciePorts; Index++) {
    if (DynamicSiLibraryPpi->PchGetPcieLaneNumByPchId (PCH_LEGACY_ID, Index, &LaneNum) == EFI_SUCCESS) {
      for (Entry = 0; Entry < TableSize; Entry++) {
        // Skip matching Lanes when the table record has settings for WM20 FIA
        if ((PtssTables[Entry].PtssTable.SbPortID == PID_MODPHY4) ||
          (PtssTables[Entry].PtssTable.SbPortID == PID_MODPHY5)){
          continue;
        }
        if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
          (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) &&
          (PcieTopologyReal[Index] == PtssTables[Entry].Topology) &&
          (BoardId == PtssTables[Entry].BoardId)) {
          PtssTableIndex++;
          if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD25) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0) == (UINT32) B_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0)) {
            PchPolicy->HsioPcieConfig.Lane[Index].HsioRxSetCtleEnable = TRUE;
            PchPolicy->HsioPcieConfig.Lane[Index].HsioRxSetCtle = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0;
          }
          else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD39) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0) == (UINT32) B_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0)) {
            PchPolicy->HsioPcieConfig.Lane[Index].HsioIcfgAdjLimitLoEnable = TRUE;
            PchPolicy->HsioPcieConfig.Lane[Index].HsioIcfgAdjLimitLo = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0;
          } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD40) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP) == (UINT32) B_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP)) {
            PchPolicy->HsioPcieConfig.Lane[Index].HsioSampOffstEvenErrSpEnable = TRUE;
            PchPolicy->HsioPcieConfig.Lane[Index].HsioSampOffstEvenErrSp = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP;
          } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD41) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS) == (UINT32) B_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS)) {
            PchPolicy->HsioPcieConfig.Lane[Index].HsioRemainingSamplerOffEnable = TRUE;
            PchPolicy->HsioPcieConfig.Lane[Index].HsioRemainingSamplerOff = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS;
          } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD7) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL) == (UINT32) B_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL)) {
            PchPolicy->HsioPcieConfig.Lane[Index].HsioVgaGainCalEnable = TRUE;
            PchPolicy->HsioPcieConfig.Lane[Index].HsioVgaGainCal = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL;
          } else {
            DEBUG ((DEBUG_ERROR, "ERROR! PTSS programming: The PTSS table offset and/or mask are not compatible with the BIOS Code.\n"));
          }
        }
      }
    }
  }

  //Case 1b Continued: PCIe for WM20 FIA
  for (Index = 0; Index < PCH_MAX_WM20_LANES_NUMBER; Index++) {
      LaneNum = Index;
      for (Entry = 0; Entry < TableSize; Entry++) {
        // Skip entries which are not for WM20 FIA
        if ((PtssTables[Entry].PtssTable.SbPortID != PID_MODPHY4) &&
          (PtssTables[Entry].PtssTable.SbPortID != PID_MODPHY5)){
          continue;
        }
        if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
          (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) &&
          (BoardId == PtssTables[Entry].BoardId)) {
          PtssTableIndex++;
          if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD25) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0) == (UINT32) B_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0)) {
            PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioRxSetCtleEnable = TRUE;
            PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioRxSetCtle = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0;
          } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD39) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0) == (UINT32) B_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0)) {
            PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioIcfgAdjLimitLoEnable = TRUE;
            PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioIcfgAdjLimitLo = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0;
          } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD40) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP) == (UINT32) B_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP)) {
            PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioSampOffstEvenErrSpEnable = TRUE;
            PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioSampOffstEvenErrSp = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP;
          } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD41) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS) == (UINT32) B_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS)) {
            PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioRemainingSamplerOffEnable = TRUE;
            PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioRemainingSamplerOff = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS;
          } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD7) &&
            (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL) == (UINT32) B_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL)) {
            PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioVgaGainCalEnable = TRUE;
            PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioVgaGainCal = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL;
          } else {
            DEBUG ((DEBUG_ERROR, "ERROR! PTSS programming: The PTSS table offset and/or mask are not compatible with the BIOS Code.\n"));
          }
        }
      }
  }

  //Case 2: BoardId is unknown, Topology is known/unknown
  if (PtssTableIndex == 0) {
    for (Index = 0; Index < MaxSataPorts; Index++) {
      if (DynamicSiLibraryPpi->PchGetSataLaneNumByPchId (PCH_LEGACY_ID, Index, &LaneNum) == EFI_SUCCESS) {
        for (Entry = 0; Entry < TableSize; Entry++) {
          if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
            (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_SATA) &&
            (PchSetup->SataTopology[Index] == PtssTables[Entry].Topology) &&
            (PtssTables[Entry].BoardId == TypePlatformUnknown)) {
            if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD20) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0) == (UINT32) B_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0)) {
              PchPolicy->HsioSataConfig.PortLane[Index].HsioRxGen3EqBoostMagEnable = TRUE;
              PchPolicy->HsioSataConfig.PortLane[Index].HsioRxGen3EqBoostMag = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0;
            } else if (PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_TX_DWORD8) {
              if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0) == (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0) {
                PchPolicy->HsioSataConfig.PortLane[Index].HsioTxGen1DownscaleAmpEnable = TRUE;
                PchPolicy->HsioSataConfig.PortLane[Index].HsioTxGen1DownscaleAmp = (PtssTables[Entry].PtssTable.Value & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0) >> N_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0;
              }
              if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0) == (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0) {
                PchPolicy->HsioSataConfig.PortLane[Index].HsioTxGen2DownscaleAmpEnable = TRUE;
                PchPolicy->HsioSataConfig.PortLane[Index].HsioTxGen2DownscaleAmp = (PtssTables[Entry].PtssTable.Value & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0) >> N_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0;
              }
            } else {
              DEBUG ((DEBUG_ERROR, "ERROR! PTSS programming: The PTSS table offset and/or mask are not compatible with the BIOS Code.\n"));
            }
          }
        }
      }
    }

    // Case 2 Continued secondary SATA when BoardId is unknown, Topology is known/unknown
    for (Index = 0; Index < MaxsSataPorts; Index++) {
      if (DynamicSiLibraryPpi->PchGetsSataLaneNumByPchId (PCH_LEGACY_ID, Index, &LaneNum) == EFI_SUCCESS) {
        for (Entry = 0; Entry < TableSize; Entry++) {
          if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
            (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_SATA) &&
            (PchSetup->sSataTopology[Index] == PtssTables[Entry].Topology) &&
            (PtssTables[Entry].BoardId == TypePlatformUnknown)) {
            if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD20) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0) == (UINT32) B_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0)) {
              PchPolicy->HsiosSataConfig.PortLane[Index].HsioRxGen3EqBoostMagEnable = TRUE;
              PchPolicy->HsiosSataConfig.PortLane[Index].HsioRxGen3EqBoostMag = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD20_ICFGCTLEDATATAP_FULLRATE_5_0;
            } else if (PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_TX_DWORD8) {
              if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0) == (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0) {
                PchPolicy->HsiosSataConfig.PortLane[Index].HsioTxGen1DownscaleAmpEnable = TRUE;
                PchPolicy->HsiosSataConfig.PortLane[Index].HsioTxGen1DownscaleAmp = (PtssTables[Entry].PtssTable.Value & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0) >> N_HSIO_PCR_TX_DWORD8_ORATE00MARGIN_5_0;
              }
              if (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0) == (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0) {
                PchPolicy->HsiosSataConfig.PortLane[Index].HsioTxGen2DownscaleAmpEnable = TRUE;
                PchPolicy->HsiosSataConfig.PortLane[Index].HsioTxGen2DownscaleAmp = (PtssTables[Entry].PtssTable.Value & (UINT32) B_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0) >> N_HSIO_PCR_TX_DWORD8_ORATE01MARGIN_5_0;
              }
            }
            else {
              DEBUG ((DEBUG_ERROR, "ERROR! PTSS programming: The PTSS table offset and/or mask are not compatible with the BIOS Code.\n"));
            }
          }
        }
      }
    }

    for (Index = 0; Index < MaxPciePorts; Index++) {
      if (DynamicSiLibraryPpi->PchGetPcieLaneNumByPchId (PCH_LEGACY_ID, Index, &LaneNum) == EFI_SUCCESS) {
        for (Entry = 0; Entry < TableSize; Entry++) {
          // Skip matching Lanes when the table record has settings for WM20 FIA
          if ((PtssTables[Entry].PtssTable.SbPortID == PID_MODPHY4) ||
            (PtssTables[Entry].PtssTable.SbPortID == PID_MODPHY5)){
            continue;
          }
          if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
            (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) &&
            (PcieTopologyReal[Index] == PtssTables[Entry].Topology) &&
            (PtssTables[Entry].BoardId == TypePlatformUnknown)) {
            if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD25) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0) == (UINT32) B_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0)) {
              PchPolicy->HsioPcieConfig.Lane[Index].HsioRxSetCtleEnable = TRUE;
              PchPolicy->HsioPcieConfig.Lane[Index].HsioRxSetCtle = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0;
            } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD39) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0) == (UINT32) B_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0)) {
              PchPolicy->HsioPcieConfig.Lane[Index].HsioIcfgAdjLimitLoEnable = TRUE;
              PchPolicy->HsioPcieConfig.Lane[Index].HsioIcfgAdjLimitLo = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0;
            } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD40) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP) == (UINT32) B_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP)) {
              PchPolicy->HsioPcieConfig.Lane[Index].HsioSampOffstEvenErrSpEnable = TRUE;
              PchPolicy->HsioPcieConfig.Lane[Index].HsioSampOffstEvenErrSp = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP;
            } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD41) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS) == (UINT32) B_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS)) {
              PchPolicy->HsioPcieConfig.Lane[Index].HsioRemainingSamplerOffEnable = TRUE;
              PchPolicy->HsioPcieConfig.Lane[Index].HsioRemainingSamplerOff = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS;
            } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD7) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL) == (UINT32) B_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL)) {
              PchPolicy->HsioPcieConfig.Lane[Index].HsioVgaGainCalEnable = TRUE;
              PchPolicy->HsioPcieConfig.Lane[Index].HsioVgaGainCal = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL;
            } else {
              DEBUG ((DEBUG_ERROR, "ERROR! PTSS programming: The PTSS table offset and/or mask are not compatible with the BIOS Code.\n"));
            }
          }
        }
      }
    }

    //Continued for PCIe ports in WM20 FIA
    for (Index = 0; Index < PCH_MAX_WM20_LANES_NUMBER; Index++) {
        LaneNum = Index;
        for (Entry = 0; Entry < TableSize; Entry++) {
          // Skip entries which are not for WM20 FIA
          if ((PtssTables[Entry].PtssTable.SbPortID != PID_MODPHY4) &&
            (PtssTables[Entry].PtssTable.SbPortID != PID_MODPHY5)){
            continue;
          }
          if ((LaneNum == PtssTables[Entry].PtssTable.LaneNum) &&
            (PtssTables[Entry].PtssTable.PhyMode == V_PCH_PCR_FIA_LANE_OWN_PCIEDMI) &&
            (PtssTables[Entry].BoardId == TypePlatformUnknown)) {
            if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD25) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0) == (UINT32) B_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0)) {
              PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioRxSetCtleEnable = TRUE;
              PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioRxSetCtle = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD25_CTLE_ADAPT_OFFSET_CFG_4_0;
            } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD39) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0) == (UINT32) B_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0)) {
              PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioIcfgAdjLimitLoEnable = TRUE;
              PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioIcfgAdjLimitLo = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD39_ICFG_ADJ_LIMITL0;
            } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD40) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP) == (UINT32) B_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP)) {
              PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioSampOffstEvenErrSpEnable = TRUE;
              PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioSampOffstEvenErrSp = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD40_SAMP_OFFST_EVEN_ERRSP;
            } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD41) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS) == (UINT32) B_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS)) {
              PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioRemainingSamplerOffEnable = TRUE;
              PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioRemainingSamplerOff = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD41_REMAINING_SAMP_OFFSTS;
            } else if ((PtssTables[Entry].PtssTable.Offset == (UINT32) R_HSIO_PCR_RX_DWORD7) &&
              (((UINT32) ~PtssTables[Entry].PtssTable.BitMask & B_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL) == (UINT32) B_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL)) {
              PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioVgaGainCalEnable = TRUE;
              PchPolicy->HsioPcieConfigFIAWM20.Lane[Index].HsioVgaGainCal = (PtssTables[Entry].PtssTable.Value & (UINT32) ~PtssTables[Entry].PtssTable.BitMask) >> N_HSIO_PCR_RX_DWORD7_VGA_GAIN_CAL;
            } else {
              DEBUG ((DEBUG_ERROR, "ERROR! PTSS programming: The PTSS table offset and/or mask are not compatible with the BIOS Code.\n"));
            }
          }
        }
    }
  }
}

PLATFORM_HSIO_PTSS_CONFIG_TABLE    PlatformHsioPtssConfigTable =
{
  PLATFORM_HSIO_PTSS_TABLE_SIGNATURE,
  PLATFORM_HSIO_PTSS_TABLE_VERSION,
  InstallPlatformHsioPtssTableCallback
};

EFI_STATUS
InstallPlatformHsioPtssTableData (
  IN UBA_CONFIG_DATABASE_PPI    *UbaConfigPpi
)
{
  EFI_STATUS                            Status = EFI_SUCCESS;
  DYNAMIC_SI_LIBARY_PPI                 *DynamicSiLibraryPpi = NULL;

  Status = PeiServicesLocatePpi (&gDynamicSiLibraryPpiGuid, 0, NULL, &DynamicSiLibraryPpi);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  if (DynamicSiLibraryPpi->GetPchSeries() == PchMini) {
    return Status;
 }

   Status = UbaConfigPpi->AddData (
                                  UbaConfigPpi,
                                  &gPlatformHsioPtssTableGuid,
                                  &PlatformHsioPtssConfigTable,
                                  sizeof(PlatformHsioPtssConfigTable)
                                  );

  return Status;
}

