/** @file
  Vop2 DXE Driver, install RK_CRTC_PROTOCOL.

  Copyright (c) 2022 Rockchip Electronics Co. Ltd.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/RockchipDisplayLib.h>

#include <Library/MediaBusFormat.h>
#include <Library/DrmModes.h>

#include <string.h>

#include <Protocol/RockchipCrtcProtocol.h>

#include "Vop2Dxe.h"

STATIC VPS_CONFIG mVpsConfigs[][VOP2_VP_MAX] = {
  {
    {
      .PlaneMask = (1 << ROCKCHIP_VOP2_CLUSTER0 | 1 << ROCKCHIP_VOP2_ESMART0),
      .PrimaryPlane = ROCKCHIP_VOP2_ESMART0,
    },
    {
      .PlaneMask = (1 << ROCKCHIP_VOP2_CLUSTER1 | 1 << ROCKCHIP_VOP2_ESMART1),
      .PrimaryPlane = ROCKCHIP_VOP2_ESMART1,
    },
    {
      .PlaneMask = (1 << ROCKCHIP_VOP2_CLUSTER2 | 1 << ROCKCHIP_VOP2_ESMART2),
      .PrimaryPlane = ROCKCHIP_VOP2_ESMART2,
    },
    {
      .PlaneMask = (1 << ROCKCHIP_VOP2_CLUSTER3 | 1 << ROCKCHIP_VOP2_ESMART3),
      .PrimaryPlane = ROCKCHIP_VOP2_ESMART3,
    },
  },
  {
    {
      .PlaneMask = 0,
      .PrimaryPlane = 0,
    },
    {
      .PlaneMask = (1 << ROCKCHIP_VOP2_CLUSTER0 | 1 << ROCKCHIP_VOP2_ESMART0 |
                    1 << ROCKCHIP_VOP2_CLUSTER1 | 1 << ROCKCHIP_VOP2_ESMART1),
      .PrimaryPlane = ROCKCHIP_VOP2_ESMART1,
    },
    {
      .PlaneMask = (1 << ROCKCHIP_VOP2_CLUSTER2 | 1 << ROCKCHIP_VOP2_ESMART2 |
                    1 << ROCKCHIP_VOP2_CLUSTER3 | 1 << ROCKCHIP_VOP2_ESMART3),
      .PrimaryPlane = ROCKCHIP_VOP2_ESMART2,
    },
    {
      .PlaneMask = 0,
      .PrimaryPlane = 0,
    },
  },
};

STATIC UINT32 mVpsConfigsSize = ARRAY_SIZE (mVpsConfigs);

STATIC CONST INT32 mSinTable[] = {
  0x00000000, 0x023be165, 0x04779632, 0x06b2f1d2, 0x08edc7b6, 0x0b27eb5c,
  0x0d61304d, 0x0f996a26, 0x11d06c96, 0x14060b67, 0x163a1a7d, 0x186c6ddd,
  0x1a9cd9ac, 0x1ccb3236, 0x1ef74bf2, 0x2120fb82, 0x234815ba, 0x256c6f9e,
  0x278dde6e, 0x29ac379f, 0x2bc750e8, 0x2ddf003f, 0x2ff31bdd, 0x32037a44,
  0x340ff241, 0x36185aee, 0x381c8bb5, 0x3a1c5c56, 0x3c17a4e7, 0x3e0e3ddb,
  0x3fffffff, 0x41ecc483, 0x43d464fa, 0x45b6bb5d, 0x4793a20f, 0x496af3e1,
  0x4b3c8c11, 0x4d084650, 0x4ecdfec6, 0x508d9210, 0x5246dd48, 0x53f9be04,
  0x55a6125a, 0x574bb8e5, 0x58ea90c2, 0x5a827999, 0x5c135399, 0x5d9cff82,
  0x5f1f5ea0, 0x609a52d1, 0x620dbe8a, 0x637984d3, 0x64dd894f, 0x6639b039,
  0x678dde6d, 0x68d9f963, 0x6a1de735, 0x6b598ea1, 0x6c8cd70a, 0x6db7a879,
  0x6ed9eba0, 0x6ff389de, 0x71046d3c, 0x720c8074, 0x730baeec, 0x7401e4bf,
  0x74ef0ebb, 0x75d31a5f, 0x76adf5e5, 0x777f903b, 0x7847d908, 0x7906c0af,
  0x79bc384c, 0x7a6831b8, 0x7b0a9f8c, 0x7ba3751c, 0x7c32a67c, 0x7cb82884,
  0x7d33f0c8, 0x7da5f5a3, 0x7e0e2e31, 0x7e6c924f, 0x7ec11aa3, 0x7f0bc095,
  0x7f4c7e52, 0x7f834ecf, 0x7fb02dc4, 0x7fd317b3, 0x7fec09e1, 0x7ffb025e,
  0x7fffffff
};

STATIC VOP2_VP_PLANE_MASK mVpPlaneMaskRK3588[VOP2_VP_MAX][VOP2_VP_MAX] = {
  { /* one display policy */
    {/* main display */
      .PrimaryPlaneId = ROCKCHIP_VOP2_ESMART0,
      .AttachedLayersNr = 8,
      .AttachedLayers = {
        ROCKCHIP_VOP2_CLUSTER0, ROCKCHIP_VOP2_ESMART0,
        ROCKCHIP_VOP2_CLUSTER1, ROCKCHIP_VOP2_ESMART1,
        ROCKCHIP_VOP2_CLUSTER2, ROCKCHIP_VOP2_ESMART2,
        ROCKCHIP_VOP2_CLUSTER3, ROCKCHIP_VOP2_ESMART3
      },
    },
    {/* second display */},
    {/* third  display */},
    {/* fourth display */},
  },

  { /* two display policy */
    {/* main display */
      .PrimaryPlaneId = ROCKCHIP_VOP2_ESMART0,
      .AttachedLayersNr = 4,
      .AttachedLayers = {
        ROCKCHIP_VOP2_CLUSTER0, ROCKCHIP_VOP2_ESMART0,
        ROCKCHIP_VOP2_CLUSTER1, ROCKCHIP_VOP2_ESMART1
      },
    },

    {/* second display */
      .PrimaryPlaneId = ROCKCHIP_VOP2_ESMART2,
      .AttachedLayersNr = 4,
      .AttachedLayers = {
        ROCKCHIP_VOP2_CLUSTER2, ROCKCHIP_VOP2_ESMART2,
        ROCKCHIP_VOP2_CLUSTER3, ROCKCHIP_VOP2_ESMART3
      },
    },
    {/* third  display */},
    {/* fourth display */},
  },

  { /* three display policy */
    {/* main display */
      .PrimaryPlaneId = ROCKCHIP_VOP2_ESMART0,
      .AttachedLayersNr = 3,
      .AttachedLayers = {
        ROCKCHIP_VOP2_CLUSTER0, ROCKCHIP_VOP2_ESMART0,
        ROCKCHIP_VOP2_CLUSTER1
      },
    },

    {/* second display */
      .PrimaryPlaneId = ROCKCHIP_VOP2_ESMART1,
      .AttachedLayersNr = 3,
      .AttachedLayers = {
        ROCKCHIP_VOP2_CLUSTER2, ROCKCHIP_VOP2_ESMART1,
        ROCKCHIP_VOP2_CLUSTER3
      },
    },

    {/* third  display */
      .PrimaryPlaneId = ROCKCHIP_VOP2_ESMART2,
      .AttachedLayersNr = 2,
      .AttachedLayers = { ROCKCHIP_VOP2_ESMART2, ROCKCHIP_VOP2_ESMART3 },
    },

    {/* fourth display */},
  },

  { /* four display policy */
    {/* main display */
      .PrimaryPlaneId = ROCKCHIP_VOP2_ESMART0,
      .AttachedLayersNr = 2,
      .AttachedLayers = { ROCKCHIP_VOP2_CLUSTER0, ROCKCHIP_VOP2_ESMART0 },
    },

    {/* second display */
      .PrimaryPlaneId = ROCKCHIP_VOP2_ESMART1,
      .AttachedLayersNr = 2,
      .AttachedLayers = { ROCKCHIP_VOP2_CLUSTER1, ROCKCHIP_VOP2_ESMART1 },
    },

    {/* third  display */
      .PrimaryPlaneId = ROCKCHIP_VOP2_ESMART2,
      .AttachedLayersNr = 2,
      .AttachedLayers = { ROCKCHIP_VOP2_CLUSTER2, ROCKCHIP_VOP2_ESMART2 },
    },

    {/* fourth display */
      .PrimaryPlaneId = ROCKCHIP_VOP2_ESMART3,
      .AttachedLayersNr = 2,
      .AttachedLayers = { ROCKCHIP_VOP2_CLUSTER3, ROCKCHIP_VOP2_ESMART3 },
    },
  },
};

STATIC VOP2_VP_DATA mVpDataRK3588[4] = {
  {
    .Feature = VOP_FEATURE_OUTPUT_10BIT,
    .PreScanMaxDly = 42,
    .MaxDclk = 600000,
    .MaxOutput = {7680, 4320},
  },
  {
    .Feature = VOP_FEATURE_OUTPUT_10BIT,
    .PreScanMaxDly = 40,
    .MaxDclk = 600000,
    .MaxOutput = {4096, 2304},
  },
  {
    .Feature = VOP_FEATURE_OUTPUT_10BIT,
    .PreScanMaxDly = 52,
    .MaxDclk = 600000,
    .MaxOutput = {4096, 2304},
  },
  {
    .Feature = 0,
    .PreScanMaxDly = 52,
    .MaxDclk = 200000,
    .MaxOutput = {1920, 1080},
  },
};

static VOP2_POWER_DOMAIN_DATA mCluster0PdDataRK3588 = {
  .PdEnShift = RK3588_CLUSTER0_PD_EN_SHIFT,
  .PdStatusShift = RK3588_CLUSTER0_PD_STATUS_SHIFT,
  .PmuStatusShift = RK3588_PD_CLUSTER0_PWR_STAT_SHIFI,
  .BisrEnStatusShift = RK3588_PD_CLUSTER0_REPAIR_EN_SHIFT,
};

static VOP2_POWER_DOMAIN_DATA mCluster1PdDataRK3588 = {
  .IsParentNeeded = TRUE,
  .PdEnShift = RK3588_CLUSTER1_PD_EN_SHIFT,
  .PdStatusShift = RK3588_CLUSTER1_PD_STATUS_SHIFT,
  .PmuStatusShift = RK3588_PD_CLUSTER1_PWR_STAT_SHIFI,
  .BisrEnStatusShift = RK3588_PD_CLUSTER1_REPAIR_EN_SHIFT,
  .ParentPhyID = ROCKCHIP_VOP2_CLUSTER0,
};

static VOP2_POWER_DOMAIN_DATA mCluster2PdDataRK3588 = {
  .IsParentNeeded = TRUE,
  .PdEnShift = RK3588_CLUSTER2_PD_EN_SHIFT,
  .PdStatusShift = RK3588_CLUSTER2_PD_STATUS_SHIFT,
  .PmuStatusShift = RK3588_PD_CLUSTER2_PWR_STAT_SHIFI,
  .BisrEnStatusShift = RK3588_PD_CLUSTER2_REPAIR_EN_SHIFT,
  .ParentPhyID = ROCKCHIP_VOP2_CLUSTER0,
};

static VOP2_POWER_DOMAIN_DATA mCluster3PdDataRK3588 = {
  .IsParentNeeded = TRUE,
  .PdEnShift = RK3588_CLUSTER3_PD_EN_SHIFT,
  .PdStatusShift = RK3588_CLUSTER3_PD_STATUS_SHIFT,
  .PmuStatusShift = RK3588_PD_CLUSTER3_PWR_STAT_SHIFI,
  .BisrEnStatusShift = RK3588_PD_CLUSTER3_REPAIR_EN_SHIFT,
  .ParentPhyID = ROCKCHIP_VOP2_CLUSTER0,
};

static VOP2_POWER_DOMAIN_DATA mEsmartPdDataRK3588 = {
  .PdEnShift = RK3588_ESMART_PD_EN_SHIFT,
  .PdStatusShift = RK3588_ESMART_PD_STATUS_SHIFT,
  .PmuStatusShift = RK3588_PD_ESMART_PWR_STAT_SHIFI,
  .BisrEnStatusShift = RK3588_PD_ESMART_REPAIR_EN_SHIFT,
};

static VOP2_WIN_DATA mWinDataRK3588[8] = {
  {
    .Name = "Cluster0",
    .PhysID = ROCKCHIP_VOP2_CLUSTER0,
    .Type = CLUSTER_LAYER,
    .WinSelPortOffset = 0,
    .LayerSelWinID = 0,
    .RegOffset = 0,
    .PdData = &mCluster0PdDataRK3588,
  },

  {
    .Name = "Cluster1",
    .PhysID = ROCKCHIP_VOP2_CLUSTER1,
    .Type = CLUSTER_LAYER,
    .WinSelPortOffset = 1,
    .LayerSelWinID = 1,
    .RegOffset = 0x200,
    .PdData = &mCluster1PdDataRK3588,
  },

  {
    .Name = "Cluster2",
    .PhysID = ROCKCHIP_VOP2_CLUSTER2,
    .Type = CLUSTER_LAYER,
    .WinSelPortOffset = 2,
    .LayerSelWinID = 4,
    .RegOffset = 0x400,
    .PdData = &mCluster2PdDataRK3588,
  },

  {
    .Name = "Cluster3",
    .PhysID = ROCKCHIP_VOP2_CLUSTER3,
    .Type = CLUSTER_LAYER,
    .WinSelPortOffset = 3,
    .LayerSelWinID = 5,
    .RegOffset = 0x600,
    .PdData = &mCluster3PdDataRK3588,
  },

  {
    .Name = "Esmart0",
    .PhysID = ROCKCHIP_VOP2_ESMART0,
    .Type = ESMART_LAYER,
    .WinSelPortOffset = 4,
    .LayerSelWinID = 2,
    .RegOffset = 0,
    .PdData = &mEsmartPdDataRK3588,
  },

  {
    .Name = "Esmart1",
    .PhysID = ROCKCHIP_VOP2_ESMART1,
    .Type = ESMART_LAYER,
    .WinSelPortOffset = 5,
    .LayerSelWinID = 3,
    .RegOffset = 0x200,
    .PdData = &mEsmartPdDataRK3588,
  },

  {
    .Name = "Esmart2",
    .PhysID = ROCKCHIP_VOP2_ESMART2,
    .Type = ESMART_LAYER,
    .WinSelPortOffset = 6,
    .LayerSelWinID = 6,
    .RegOffset = 0x400,
    .PdData = &mEsmartPdDataRK3588,
  },

  {
    .Name = "Esmart3",
    .PhysID = ROCKCHIP_VOP2_ESMART3,
    .Type = ESMART_LAYER,
    .WinSelPortOffset = 7,
    .LayerSelWinID = 7,
    .RegOffset = 0x600,
    .PdData = &mEsmartPdDataRK3588,
  },
};


STATIC VOP2_DATA mVop2RK3588 = {
  .Version = VOP_VERSION_RK3588,
  .NrVps = 4,
  .VpData = mVpDataRK3588,
  .WinData = mWinDataRK3588,
/*
  .plane_table = rk3588_plane_table,
*/
  .PlaneMask = mVpPlaneMaskRK3588[0],
  .NrLayers = 8,
  .NrMixers = 7,
  .NrGammas = 4,
  .NrDscs = 2,
};

STATIC UINT8 RK3588Vop2VpPrimaryPlaneOrder[VOP2_VP_MAX] = {
  ROCKCHIP_VOP2_ESMART0,
  ROCKCHIP_VOP2_ESMART1,
  ROCKCHIP_VOP2_ESMART2,
  ROCKCHIP_VOP2_ESMART3,
};

STATIC UINT8 RK3568Vop2VpPrimaryPlaneOrder[VOP2_VP_MAX] = {
  ROCKCHIP_VOP2_SMART0,
  ROCKCHIP_VOP2_SMART1,
  ROCKCHIP_VOP2_ESMART1,
};

STATIC UINT32 mRegsBackup[RK3568_MAX_REG] = {0};

STATIC CHAR8* mDisplayIfName[] = {
  " RGB",
  " BT1120",
  " BT656",
  " LVDS0",
  " LVDS1",
  " MIPI0",
  " MIPI1",
  " eDP0",
  " eDP1",
  " DP0",
  " DP1",
  " HDMI0",
  " HDMI1"
};

STATIC VOP2 *RockchipVop2;

INLINE
UINT32
LogCalculate (
 IN UINT32                                  Input
  )
{
  INT32 Count = -1;

  if (Input == 0)
    return 0;

  while (Input) {
    Input >>= 1;
    Count++;
  }

  return Count;
}

INLINE
VOID
Vop2MaskWrite (
  IN  UINTN                                 Address,
  IN  UINT32                                Offset,
  IN  UINT32                                Mask,
  IN  UINT32                                Shift,
  IN  UINT32                                Value,
  IN  BOOLEAN                               WriteMask
  )
{
  UINT32 CachedVal;

  if (!Mask)
    return;

  if (WriteMask) {
    Value = ((Value & Mask) << Shift) | (Mask << (Shift + 16));
  } else {
    CachedVal = mRegsBackup[Offset >> 2];

    Value = (CachedVal & ~(Mask << Shift)) | ((Value & Mask) << Shift);
    mRegsBackup[Offset >> 2] = Value;
  }

  MmioWrite32(Address + Offset, Value);
}

INLINE
VOID
Vop2Writel (
  IN UINTN                                 Address,
  IN UINT32                                Offset,
  IN UINT32                                Value
)
{
  MmioWrite32(Address + Offset, Value);
  mRegsBackup[Offset >> 2] = Value;
}

INLINE
VOID
Vop2GrfWrite (
  IN  UINTN                                 Address,
  IN  UINT32                                Offset,
  IN  UINT32                                Mask,
  IN  UINT32                                Shift,
  IN  UINT32                                Value
  )
{
  UINT32 TempVal = 0;

  TempVal = (Value << Shift) | (Mask << (Shift + 16));
  MmioWrite32(Address + Offset, TempVal);
}

INLINE
UINT32 GenericHWeight32 (
  IN  UINT32                                 W
  )
{
  UINT32 Res = (W & 0x55555555) + ((W >> 1) & 0x55555555);
  Res = (Res & 0x33333333) + ((Res >> 2) & 0x33333333);
  Res = (Res & 0x0F0F0F0F) + ((Res >> 4) & 0x0F0F0F0F);
  Res = (Res & 0x00FF00FF) + ((Res >> 8) & 0x00FF00FF);
  return (Res & 0x0000FFFF) + ((Res >> 16) & 0x0000FFFF);
}

INLINE
INT32 FFS(int x)
{
  int r = 1;

  if (!x)
    return 0;
  if (!(x & 0xffff)) {
    x >>= 16;
    r += 16;
  }
  if (!(x & 0xff)) {
    x >>= 8;
    r += 8;
  }
  if (!(x & 0xf)) {
    x >>= 4;
    r += 4;
  }
  if (!(x & 3)) {
    x >>= 2;
    r += 2;
  }
  if (!(x & 1))
    r += 1;
  return r;
}

STATIC
INT32
Vop2GetPrimaryPlane (
  OUT VOP2                                  *Vop2,
  IN  UINT32                                PlaneMask
  )
{
  UINT32 i = 0;
  UINT8 *Vop2VpPrimaryPlaneOrder;
  UINT8 DefaultPrimaryPlane;

  if (Vop2->Version == VOP_VERSION_RK3588) {
    Vop2VpPrimaryPlaneOrder = RK3588Vop2VpPrimaryPlaneOrder;
    DefaultPrimaryPlane = ROCKCHIP_VOP2_ESMART0;
  } else {
    Vop2VpPrimaryPlaneOrder = RK3568Vop2VpPrimaryPlaneOrder;
    DefaultPrimaryPlane = ROCKCHIP_VOP2_SMART0;
  }

  for (i = 0; i < Vop2->Data->NrVps; i++) {
    if (PlaneMask & BIT(Vop2VpPrimaryPlaneOrder[i]))
      return Vop2VpPrimaryPlaneOrder[i];
  }

  return DefaultPrimaryPlane;
}

STATIC
CHAR8 *
GetPlaneName (
  IN UINT32                                PlaneID
  )
{
  CHAR8 *Name = NULL;

  switch (PlaneID) {
  case ROCKCHIP_VOP2_CLUSTER0:
    Name = "Cluster0";
    break;
  case ROCKCHIP_VOP2_CLUSTER1:
    Name = "Cluster1";
    break;
  case ROCKCHIP_VOP2_ESMART0:
    Name = "Esmart0";
    break;
  case ROCKCHIP_VOP2_ESMART1:
    Name = "Esmart1";
    break;
  case ROCKCHIP_VOP2_SMART0:
    Name = "Smart0";
    break;
  case ROCKCHIP_VOP2_SMART1:
    Name = "Smart1";
    break;
  case ROCKCHIP_VOP2_CLUSTER2:
    Name = "Cluster2";
    break;
  case ROCKCHIP_VOP2_CLUSTER3:
    Name = "Cluster3";
    break;
  case ROCKCHIP_VOP2_ESMART2:
    Name = "Esmart2";
    break;
  case ROCKCHIP_VOP2_ESMART3:
    Name = "Esmart3";
    break;
  }

  return Name;
}

VOID
Vop2DumpRegisters (
  OUT DISPLAY_STATE                        *DisplayState,
  OUT VOP2_WIN_DATA                        *WinData
  )
{
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  VOP2 *Vop2 = CrtcState->Private;
  UINT32 VPOffset = CrtcState->CrtcID * 0x100;
  UINT32 WinOffset = 0;
  UINT8 PrimaryPlaneID = 0;
  INT32 i = 0;
  UINT32 Reg = 0;

  /* sys registers */
  Reg = RK3588_VOP2_REG_BASE;
  DEBUG ((DEBUG_WARN, "SYS:"));
  for (i = 0; i < 0x100; i += 4) {
    if (i % 0x10 == 0) {
      DEBUG ((DEBUG_WARN, "\n"));
      DEBUG ((DEBUG_WARN, "%x:", Reg + i));
    }

    DEBUG ((DEBUG_WARN, " %08x", MmioRead32(Reg + i)));
  }
  DEBUG ((DEBUG_WARN, "\n"));
  DEBUG ((DEBUG_WARN, "\n"));

  /* ovl registers */
  Reg = RK3588_VOP2_REG_BASE + RK3568_OVL_CTRL;
  DEBUG ((DEBUG_WARN, "OVL:"));
  for (i = 0; i < 0x100; i += 4) {
    if (i % 0x10 == 0) {
      DEBUG ((DEBUG_WARN, "\n"));
      DEBUG ((DEBUG_WARN, "%x:", Reg + i));
    }

    DEBUG ((DEBUG_WARN, " %08x", MmioRead32(Reg + i)));
  }
  DEBUG ((DEBUG_WARN, "\n"));
  DEBUG ((DEBUG_WARN, "\n"));

  /* hdr registers */
  Reg = RK3588_VOP2_REG_BASE + 0x2000;
  DEBUG ((DEBUG_WARN, "HDR:"));
  for (i = 0; i < 0x40; i += 4) {
    if (i % 0x10 == 0) {
      DEBUG ((DEBUG_WARN, "\n"));
      DEBUG ((DEBUG_WARN, "%x:", Reg + i));
    }

    DEBUG ((DEBUG_WARN, " %08x", MmioRead32(Reg + i)));
  }
  DEBUG ((DEBUG_WARN, "\n"));
  DEBUG ((DEBUG_WARN, "\n"));

  /* vp registers */
  Reg = RK3588_VOP2_REG_BASE + RK3568_VP0_DSP_CTRL + VPOffset;
  DEBUG ((DEBUG_WARN, "VP%d:", CrtcState->CrtcID));
  for (i = 0; i < 0x100; i += 4) {
    if (i % 0x10 == 0) {
      DEBUG ((DEBUG_WARN, "\n"));
      DEBUG ((DEBUG_WARN, "%x:", Reg + i));
    }

    DEBUG ((DEBUG_WARN, " %08x", MmioRead32(Reg + i)));
  }
  DEBUG ((DEBUG_WARN, "\n"));
  DEBUG ((DEBUG_WARN, "\n"));

  /* plane registers */
  if (WinData) {
    WinOffset = WinData->RegOffset;
    if (WinData->Type == CLUSTER_LAYER)
      Reg = RK3588_VOP2_REG_BASE + RK3568_CLUSTER0_WIN0_CTRL0 + WinOffset;
    else
      Reg = RK3588_VOP2_REG_BASE + RK3568_ESMART0_CTRL0 + WinOffset;
    PrimaryPlaneID = Vop2->VpPlaneMask[CrtcState->CrtcID].PrimaryPlaneId;

    DEBUG ((DEBUG_WARN, "%a:", GetPlaneName(PrimaryPlaneID)));
    for (i = 0; i < 0x100; i += 4) {
      if (i % 0x10 == 0) {
        DEBUG ((DEBUG_WARN, "\n"));
        DEBUG ((DEBUG_WARN, "%x:", Reg + i));
      }

      DEBUG ((DEBUG_WARN, " %08x", MmioRead32(Reg + i)));
    }
    DEBUG ((DEBUG_WARN, "\n"));
    DEBUG ((DEBUG_WARN, "\n"));
  }
}

STATIC
VOP2_WIN_DATA *
Vop2FindWinByPhysID (
  OUT VOP2                                 *Vop2,
  IN  INT32                                PhysID
  )
{
  INT32 i = 0;
  for (i = 0; i < Vop2->Data->NrLayers; i++) {
    if (Vop2->Data->WinData[i].PhysID == PhysID)
      return &Vop2->Data->WinData[i];
  }

  return NULL;
}

INLINE
BOOLEAN
IsHotPlugDevices (
  IN  INT32                                OutputType
  )
{
	switch (OutputType) {
	case DRM_MODE_CONNECTOR_HDMIA:
	case DRM_MODE_CONNECTOR_HDMIB:
	case DRM_MODE_CONNECTOR_TV:
	case DRM_MODE_CONNECTOR_DisplayPort:
	case DRM_MODE_CONNECTOR_VGA:
	case DRM_MODE_CONNECTOR_Unknown:
		return TRUE;
	default:
		return FALSE;
	}
}

STATIC
VOID
Vop2GlobalInitial (
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  VOP2 *Vop2 = CrtcState->Private;
  ROCKCHIP_CRTC_PROTOCOL *Crtc = (ROCKCHIP_CRTC_PROTOCOL*)CrtcState->Crtc;
  VOP2_WIN_DATA *WinData;
  VOP2_VP_PLANE_MASK *PlaneMask;
  UINT32 BakIndex;
  UINT32 i = 0, j = 0;
  UINT32 LayerNr = 0;
  UINT8 Shift = 0, TotalUsedLayer = 0;
  INT32 PortMux = 0;
  INT32 LayerPhyID = 0;

  if (Vop2->GlobalInit)
    return;

  /* open the vop global pd */
  MmioWrite32 (0xfd8d8150, 0xffff0000);
  MicroSecondDelay (10);

  if (DisplayState->VpsConfigModeID < 0 || DisplayState->VpsConfigModeID >= mVpsConfigsSize) {
    INT32 MainVpIndex = -1;
    INT32 ActiveVpNum = 0;

    for (i = 0; i < Vop2->Data->NrVps; i++) {
      if (Crtc->Vps[i].Enable)
        ActiveVpNum++;
    }

    PlaneMask = Vop2->Data->PlaneMask;
    PlaneMask += (ActiveVpNum - 1) * VOP2_VP_MAX;

    /* find the first unplug devices and set it as main display */
    for (i = 0; i < Vop2->Data->NrVps; i++) {
      if (!IsHotPlugDevices (Crtc->Vps[i].OutputType)) {
        Vop2->VpPlaneMask[i] = PlaneMask[0];
        MainVpIndex = i;
        break;
      }
    }

    /* if no find unplug devices, use vp0 as main display */
    if (MainVpIndex < 0) {
      ActiveVpNum = 0;
      Vop2->VpPlaneMask[0] = PlaneMask[0];
    }

    /* plane_mask[0] store main display, so we from plane_mask[1] */
    j = 1;

    /* init other display except main display */
    for (i = 0; i < Vop2->Data->NrVps; i++) {
      if (i == MainVpIndex || !Crtc->Vps[i].Enable) {
        continue;
      }
      Vop2->VpPlaneMask[i] = PlaneMask[j++];
    }

    for (i = 0; i < Vop2->Data->NrVps; i++) {
      LayerNr = Vop2->VpPlaneMask[i].AttachedLayersNr;
      for (j = 0; j < LayerNr; j++) {
        LayerPhyID = Vop2->VpPlaneMask[i].AttachedLayers[j];
        Vop2->VpPlaneMask[i].PlaneMask |= BIT(LayerPhyID);
      }
    }
  } else {
    UINT32 PlaneMask;
    UINT32 PrimaryPlaneID;

    for (i = 0; i < Vop2->Data->NrVps; i++) {
      PlaneMask = mVpsConfigs[DisplayState->VpsConfigModeID][i].PlaneMask;
      Vop2->VpPlaneMask[i].PlaneMask = PlaneMask;
      LayerNr = GenericHWeight32 (PlaneMask);
      Vop2->VpPlaneMask[i].AttachedLayersNr = LayerNr;
      PrimaryPlaneID = Vop2GetPrimaryPlane (Vop2, PlaneMask);
      Vop2->VpPlaneMask[i].PrimaryPlaneId = PrimaryPlaneID;
      Vop2->VpPlaneMask[i].PlaneMask = PlaneMask;
      for (j = 0; j < LayerNr; j++) {
        Vop2->VpPlaneMask[i].AttachedLayers[j] = FFS (PlaneMask) - 1;
        PlaneMask &= ~BIT(Vop2->VpPlaneMask[i].AttachedLayers[j]);
      }
    }
  }

  /* open the vop plane pd(esmart) */
  /* status checkout --- todo */
  MmioWrite32 (0xfdd90034, 0x00000000);
  MicroSecondDelay (10);

  /* vop2 regs backup */
  for (BakIndex = 0; BakIndex < (RK3568_MAX_REG >> 2); BakIndex++) {
    mRegsBackup[BakIndex] = MmioRead32(Vop2->BaseAddress + 4 * BakIndex);
  }

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_OVL_CTRL, EN_MASK,
                OVL_PORT_MUX_REG_DONE_IMD_SHIFT, 1, FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_DSP_IF_POL, EN_MASK,
                IF_CTRL_REG_DONE_IMD_SHIFT, 1, FALSE);

  for (i = 0; i < Vop2->Data->NrVps; i++) {
    DEBUG ((DEBUG_INIT, "vp%d have layer nr:%d[", i, Vop2->VpPlaneMask[i].AttachedLayersNr));
    for (j = 0; j < Vop2->VpPlaneMask[i].AttachedLayersNr; j++)
      DEBUG ((DEBUG_INIT, "%d ", Vop2->VpPlaneMask[i].AttachedLayers[j]));
    DEBUG ((DEBUG_INIT, "], primary plane: %d\n", Vop2->VpPlaneMask[i].PrimaryPlaneId));
  }

  Shift = 0;
  /* layer sel win id */
  for (i = 0; i < Vop2->Data->NrVps; i++) {
    LayerNr = Vop2->VpPlaneMask[i].AttachedLayersNr;
    for (j = 0; j < LayerNr; j++) {
      LayerPhyID = Vop2->VpPlaneMask[i].AttachedLayers[j];
      WinData = Vop2FindWinByPhysID (Vop2, LayerPhyID);
      Vop2MaskWrite (Vop2->BaseAddress, RK3568_OVL_LAYER_SEL, LAYER_SEL_MASK,
                     Shift, WinData->LayerSelWinID, FALSE);
      Shift += 4;
    }
  }

  /* win sel port */
  for (i = 0; i < Vop2->Data->NrVps; i++) {
    LayerNr = Vop2->VpPlaneMask[i].AttachedLayersNr;
    for (j = 0; j < LayerNr; j++) {
      if (!Vop2->VpPlaneMask[i].AttachedLayers[j])
        continue;
      LayerPhyID = Vop2->VpPlaneMask[i].AttachedLayers[j];
      WinData = Vop2FindWinByPhysID (Vop2, LayerPhyID);
      Shift = WinData->WinSelPortOffset * 2;
      Vop2MaskWrite (Vop2->BaseAddress, RK3568_OVL_PORT_SEL, LAYER_SEL_PORT_MASK,
                     LAYER_SEL_PORT_SHIFT + Shift, i, FALSE);
    }
  }

  /*
   * port mux config
   */
  for (i = 0; i < Vop2->Data->NrVps; i++) {
    Shift = i * 4;
    if (Vop2->VpPlaneMask[i].AttachedLayersNr) {
      TotalUsedLayer += Vop2->VpPlaneMask[i].AttachedLayersNr;
      PortMux = TotalUsedLayer - 1;
    } else {
      PortMux = 8;
    }

    if (i == (Vop2->Data->NrVps - 1))
      PortMux = Vop2->Data->NrMixers;

    Crtc->Vps[i].BgOvlDly = (Vop2->Data->NrMixers - PortMux) << 1;
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_OVL_PORT_SEL, PORT_MUX_MASK,
                   PORT_MUX_SHIFT + Shift, PortMux, FALSE);
  }

  Vop2->GlobalInit = TRUE;
}

STATIC
UINT64
Vop2CalcDclk (
  IN UINT32                               ChildClk,
  IN UINT32                               MaxDclk
  )
{
  if (ChildClk * 4 <= MaxDclk)
    return ChildClk * 4;
  else if (ChildClk * 2 <= MaxDclk)
    return ChildClk * 2;
  else if (ChildClk <= MaxDclk)
    return ChildClk;
  else
    return 0;
}

STATIC
UINT64
Vop2CalcCruConfig (
  IN  DISPLAY_STATE                        *DisplayState,
  OUT UINT32*                              DclkCoreDiv,
  OUT UINT32*                              DclkOutDiv,
  OUT UINT32*                              IfPixclkDiv,
  OUT UINT32*                              IfDclkDiv
  )
{
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  DRM_DISPLAY_MODE *DisplayMode = &ConnectorState->DisplayMode;
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  VOP2 *Vop2 = CrtcState->Private;
  UINT64 VPixclk = DisplayMode->Clock;
  UINT64 DclkCoreRate = VPixclk >> 2;
  UINT64 DclkRate = VPixclk;
  UINT64 IfDclkRate;
  UINT64 IfPixclkRate;
  INT32 OutputType = ConnectorState->Type;
  INT32 OutputMode = ConnectorState->OutputMode;
  UINT8 K = 1;

  if (OutputType == DRM_MODE_CONNECTOR_HDMIA) {
    /*
     * K = 2: dclk_core = if_pixclk_rate > if_dclk_rate
     * K = 1: dclk_core = hdmie_edp_dclk > if_pixclk_rate
     */
    if (OutputMode == ROCKCHIP_OUT_MODE_YUV420)
      K = 2;
    //if (ConnectorState->DSCEnable)
    IfPixclkRate = (DclkCoreRate << 1) / K;
    IfDclkRate = DclkCoreRate / K;

    DclkRate = Vop2CalcDclk (IfPixclkRate, Vop2->Data->VpData->MaxDclk);
    if (!DclkRate) {
      DEBUG ((DEBUG_ERROR, "DP if_pixclk_rate out of range(max_dclk: %d KHZ, dclk_core: %lld KHZ)\n",
                           Vop2->Data->VpData->MaxDclk, IfPixclkRate));
      return -RETURN_INVALID_PARAMETER;
    }
    *IfPixclkDiv = DclkRate / IfPixclkRate;
    *IfDclkDiv = DclkRate / IfDclkRate;
  } else if (OutputType == DRM_MODE_CONNECTOR_eDP) {
    /* edp_pixclk = edp_dclk > dclk_core */
    IfPixclkRate = VPixclk / K;
    IfDclkRate = VPixclk / K;
    DclkRate = IfPixclkRate * K;
    *DclkCoreDiv = DclkRate / DclkCoreRate;
    *IfPixclkDiv = DclkRate / IfPixclkRate;
    *IfDclkDiv = *IfPixclkDiv;
  } else if (OutputType == DRM_MODE_CONNECTOR_DPI) {
    DclkRate = VPixclk;
    *DclkCoreDiv = DclkRate / DclkCoreRate;
  }

  *IfPixclkDiv = LogCalculate(*IfPixclkDiv);
  *IfDclkDiv = LogCalculate(*IfDclkDiv);
  *DclkCoreDiv = LogCalculate(*DclkCoreDiv);
  *DclkOutDiv = LogCalculate(*DclkOutDiv);

  DEBUG ((DEBUG_INFO, "[VOP2](CalcCru)DclkRate     = %ld(divide: %d)\n", DclkRate, *DclkOutDiv));
  DEBUG ((DEBUG_INFO, "               DclkCoreRate = %ld(divide: %d)\n", DclkCoreRate, *DclkCoreDiv));
  DEBUG ((DEBUG_INFO, "               IfDclkRate   = %ld(divide: %d)\n", IfDclkRate, *IfDclkDiv));
  DEBUG ((DEBUG_INFO, "               IfPixclkRate = %ld(divide: %d)\n", IfPixclkRate, *IfPixclkDiv));

  return DclkRate;
}

STATIC
UINT32
Vop2IfConfig (
  OUT DISPLAY_STATE                        *DisplayState,
  OUT VOP2                                 *Vop2
  )
{
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  UINT32 VPOffset = CrtcState->CrtcID * 0x100;
  UINT32 OutputIf = ConnectorState->OutputInterface;
  UINT32 DclkCoreDiv = 0;
  UINT32 DclkOutDiv = 0;
  UINT32 IfPixclkDiv = 0;
  UINT32 IfDclkDiv = 0;
  UINT32 DclkRate;

  DclkRate = Vop2CalcCruConfig(DisplayState, &DclkCoreDiv, &DclkOutDiv, &IfPixclkDiv, &IfDclkDiv);

  if (OutputIf & VOP_OUTPUT_IF_eDP0) {
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_DSP_IF_EN, EN_MASK,
                   RK3588_EDP0_EN_SHIFT, 1, FALSE);
    /* temp eDP0 fixed vp2 */
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_DSP_IF_EN, IF_MUX_MASK,
                   RK3588_HDMI_EDP0_MUX_SHIFT, CrtcState->CrtcID, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_DSP_IF_CTRL, 0x3,
                   HDMI_EDP0_DCLK_DIV_SHIFT, IfDclkDiv, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_DSP_IF_CTRL, 0x3,
                   HDMI_EDP0_PIXCLK_DIV_SHIFT, IfPixclkDiv, FALSE);
    Vop2GrfWrite (RK3588_VOP_GRF_BASE, RK3588_GRF_VOP_CON2, EN_MASK,
                  RK3588_GRF_EDP0_ENABLE_SHIFT, 1);
  }

  if (OutputIf & VOP_OUTPUT_IF_eDP1) {
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_DSP_IF_EN, EN_MASK,
                   RK3588_EDP1_EN_SHIFT, 1, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_DSP_IF_EN, IF_MUX_MASK,
                   RK3588_HDMI_EDP1_MUX_SHIFT, CrtcState->CrtcID, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_DSP_IF_CTRL, 0x3,
                   HDMI_EDP1_DCLK_DIV_SHIFT, IfDclkDiv, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_DSP_IF_CTRL, 0x3,
                   HDMI_EDP1_PIXCLK_DIV_SHIFT, IfPixclkDiv, FALSE);
    Vop2GrfWrite (RK3588_VOP_GRF_BASE, RK3588_GRF_VOP_CON2, EN_MASK,
                  RK3588_GRF_EDP1_ENABLE_SHIFT, 1);
  }

  /* temp eDP0 fixed vp2 */
  Vop2MaskWrite (Vop2->BaseAddress, RK3588_VP0_CLK_CTRL + VPOffset, 0x3,
                DCLK_CORE_DIV_SHIFT, DclkCoreDiv, FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3588_VP0_CLK_CTRL + VPOffset, 0x3,
                DCLK_OUT_DIV_SHIFT, DclkOutDiv, FALSE);

  MmioWrite32(0xfdd90e0c, 0xe);

  return DclkRate;
}

STATIC
BOOLEAN
IsUVSwap (
  IN UINT32                                BusFormat,
  IN UINT32                                OutputMode
  )
{
  /*
   * FIXME:
   *
   * There is no media type for YUV444 output,
   * so when out_mode is AAAA or P888, assume output is YUV444 on
   * yuv format.
   *
   * From H/W testing, YUV444 mode need a rb swap.
   */
  if ((BusFormat == MEDIA_BUS_FMT_YUV8_1X24 ||
       BusFormat == MEDIA_BUS_FMT_YUV10_1X30) &&
      (OutputMode == ROCKCHIP_OUT_MODE_AAAA ||
       OutputMode == ROCKCHIP_OUT_MODE_P888))
    return TRUE;
  else
    return FALSE;
}

STATIC
BOOLEAN
IsYUVOutput (
  IN UINT32                                BusFormat
  )
{
  switch (BusFormat) {
  case MEDIA_BUS_FMT_YUV8_1X24:
  case MEDIA_BUS_FMT_YUV10_1X30:
  case MEDIA_BUS_FMT_UYYVYY8_0_5X24:
  case MEDIA_BUS_FMT_UYYVYY10_0_5X30:
  case MEDIA_BUS_FMT_YUYV8_2X8:
  case MEDIA_BUS_FMT_YVYU8_2X8:
  case MEDIA_BUS_FMT_UYVY8_2X8:
  case MEDIA_BUS_FMT_VYUY8_2X8:
  case MEDIA_BUS_FMT_YUYV8_1X16:
  case MEDIA_BUS_FMT_YVYU8_1X16:
  case MEDIA_BUS_FMT_UYVY8_1X16:
  case MEDIA_BUS_FMT_VYUY8_1X16:
    return TRUE;
  default:
    return FALSE;
}
}

INLINE
UINT32
Vop2ConvertCSCMode (
  IN UINT32                                CSCMode
  )
{
  switch (CSCMode) {
  case V4L2_COLORSPACE_SMPTE170M:
  case V4L2_COLORSPACE_470_SYSTEM_M:
  case V4L2_COLORSPACE_470_SYSTEM_BG:
    return CSC_BT601L;
  case V4L2_COLORSPACE_REC709:
  case V4L2_COLORSPACE_SMPTE240M:
  case V4L2_COLORSPACE_DEFAULT:
    return CSC_BT709L;
  case V4L2_COLORSPACE_JPEG:
    return CSC_BT601F;
  case V4L2_COLORSPACE_BT2020:
    return CSC_BT2020;
  default:
    return CSC_BT709L;
  }
}

INLINE
INT32
Interpolate (
  IN INT32                                X1,
  IN INT32                                Y1,
  IN INT32                                X2,
  IN INT32                                Y2,
  IN INT32                                X
  )
{
  return Y1 + (Y2 - Y1) * (X - X1) / (X2 - X1);
}

INLINE
INT32
__Fixp_Sin32 (
  IN UINT32                                Degrees
  )
{
  INT32 Ret;
  BOOLEAN Negative = FALSE;

  if (Degrees > 180) {
    Negative = TRUE;
    Degrees -= 180;
  }
  if (Degrees > 90)
    Degrees = 180 - Degrees;

  Ret = mSinTable[Degrees];

  return Negative ? -Ret : Ret;
}

INLINE
INT32
FixpSin32 (
  IN UINT32                                Degrees
  )
{
  Degrees = (Degrees % 360 + 360) % 360;

  return __Fixp_Sin32(Degrees);
}
#define FixpCos32(v) FixpSin32((v) + 90)
#define FixpSin16(v) (FixpSin32(v) >> 16)
#define FixpCos16(v) (FixpCos32(v) >> 16)

STATIC
VOID
Vop2TVConfigUpdate (
  OUT DISPLAY_STATE                        *DisplayState,
  OUT VOP2                                 *Vop2
  )
{
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  BASE_BCSH_INFO *BCSHInfo;
  UINT32 Brightness, Contrast, Saturation, Hue, SinHue, CosHue;
  BOOLEAN BCSHEnable = FALSE;
  BOOLEAN R2YEnable = FALSE;
  BOOLEAN Y2REnable = FALSE;
  UINT32 VPOffset = CrtcState->CrtcID * 0x100;
  UINT32 PostCSCMode;

  /* base2_disp_info --- todo */
  if (!ConnectorState->DispInfo)
    return;

  BCSHInfo = &ConnectorState->DispInfo->BCSHInfo;
  if (!BCSHInfo)
    return;

  if (BCSHInfo->Brightness != 50 ||
      BCSHInfo->Contrast != 50 ||
      BCSHInfo->Saturation != 50 ||
      BCSHInfo->Hue != 50)
      BCSHEnable = TRUE;

  if (BCSHEnable) {
    if (!CrtcState->YUVOverlay)
      R2YEnable = TRUE;
    if (!IsYUVOutput(ConnectorState->BusFormat))
      Y2REnable = TRUE;
  } else {
    if (!CrtcState->YUVOverlay && IsYUVOutput(ConnectorState->BusFormat))
      R2YEnable = TRUE;
    if (CrtcState->YUVOverlay && !IsYUVOutput(ConnectorState->BusFormat))
      Y2REnable = TRUE;
  }

  PostCSCMode = Vop2ConvertCSCMode(ConnectorState->ColorSpace);

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_BCSH_CTRL + VPOffset, BCSH_CTRL_R2Y_MASK,
                BCSH_CTRL_R2Y_SHIFT, R2YEnable, FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_BCSH_CTRL + VPOffset, BCSH_CTRL_Y2R_MASK,
                BCSH_CTRL_Y2R_SHIFT, Y2REnable, FALSE);

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_BCSH_CTRL + VPOffset, BCSH_CTRL_R2Y_CSC_MODE_MASK,
                BCSH_CTRL_R2Y_CSC_MODE_SHIFT, PostCSCMode, FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_BCSH_CTRL + VPOffset, BCSH_CTRL_Y2R_CSC_MODE_MASK,
                BCSH_CTRL_Y2R_CSC_MODE_SHIFT, PostCSCMode, FALSE);
  if (!BCSHEnable) {
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_BCSH_COLOR + VPOffset, BCSH_EN_MASK,
                  BCSH_EN_SHIFT, 0, FALSE);
    return;
  }

  if (CrtcState->Feature & VOP_FEATURE_OUTPUT_10BIT)
    Brightness = Interpolate(0, -128, 100, 127, BCSHInfo->Brightness);
  else
    Brightness = Interpolate(0, -32, 100, 31, BCSHInfo->Brightness);
  Contrast = Interpolate(0, 0, 100, 511, BCSHInfo->Contrast);
  Saturation = Interpolate(0, 0, 100, 511, BCSHInfo->Saturation);
  Hue = Interpolate(0, -30, 100, 30, BCSHInfo->Hue);

  /*
   *  a:[-30~0):
   *    sin_hue = 0x100 - sin(a)*256;
   *    cos_hue = cos(a)*256;
   *  a:[0~30]
   *    sin_hue = sin(a)*256;
   *    cos_hue = cos(a)*256;
   */
  SinHue = FixpSin32(Hue) >> 23;
  CosHue = FixpCos32(Hue) >> 23;

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_BCSH_BCS + VPOffset, BCSH_BRIGHTNESS_MASK,
                BCSH_BRIGHTNESS_SHIFT, Brightness, FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_BCSH_BCS + VPOffset, BCSH_CONTRAST_MASK,
                BCSH_CONTRAST_SHIFT, Contrast, FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_BCSH_BCS + VPOffset, BCSH_SATURATION_MASK,
                BCSH_SATURATION_SHIFT, Saturation, FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_BCSH_H + VPOffset, BCSH_SIN_HUE_MASK,
                BCSH_SIN_HUE_SHIFT, SinHue, FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_BCSH_H + VPOffset, BCSH_COS_HUE_MASK,
                BCSH_COS_HUE_SHIFT, CosHue, FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_BCSH_BCS + VPOffset, BCSH_OUT_MODE_MASK,
                BCSH_OUT_MODE_SHIFT, BCSH_OUT_MODE_NORMAL_VIDEO, FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_BCSH_COLOR + VPOffset, BCSH_EN_MASK,
                BCSH_EN_SHIFT, 1, FALSE);
}

INLINE
UINT16 SclCalScale2 (
  IN  INT32                                Src,
  IN  INT32                                Dst
  )
{
  return ((Src - 1) << 12) / (Dst - 1);
}

STATIC
VOID
Vop2PostConfig (
  OUT DISPLAY_STATE                        *DisplayState,
  OUT VOP2                                 *Vop2
  )
{
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  ROCKCHIP_CRTC_PROTOCOL *Crtc = (ROCKCHIP_CRTC_PROTOCOL *)CrtcState->Crtc;
  DRM_DISPLAY_MODE *Mode = &ConnectorState->DisplayMode;
  UINT32 VPOffset = CrtcState->CrtcID * 0x100;
  UINT16 HDisplay = Mode->CrtcHDisplay;
  UINT16 HActStart = Mode->CrtcHTotal - Mode->CrtcHSyncStart;
  UINT16 VDisplay = Mode->CrtcVDisplay;
  UINT16 VTotal = Mode->CrtcVTotal;
  UINT16 VActStart = Mode->CrtcVTotal - Mode->CrtcVSyncStart;
  UINT16 HSize = HDisplay * (ConnectorState->OverScan.LeftMargin +
                             ConnectorState->OverScan.RightMargin) /200;
  UINT16 VSize = VDisplay * (ConnectorState->OverScan.TopMargin +
                             ConnectorState->OverScan.BottomMargin) /200;
  UINT16 HActEnd, VActEnd;
  UINT32 Val;

  UINT32 BgOvlDly, BgDly, PreScanDly;
  UINT16 HSyncLen = Mode->CrtcHSyncEnd - Mode->CrtcHSyncStart;

  HSize = ROUNDDOWN (HSize, 2);
  VSize = ROUNDDOWN (VSize, 2);

  HActStart += HDisplay * (100 - ConnectorState->OverScan.LeftMargin) / 200;
  HActEnd = HActStart + HSize;
  Val = HActStart << 16;
  Val |= HActEnd;
  Vop2Writel (Vop2->BaseAddress, RK3568_VP0_POST_DSP_HACT_INFO + VPOffset, Val);

  VActStart += VDisplay * (100 - ConnectorState->OverScan.TopMargin) / 200;
  VActEnd = VActStart + VSize;
  Val = VActStart << 16;
  Val |= VActEnd;
  Vop2Writel (Vop2->BaseAddress, RK3568_VP0_POST_DSP_VACT_INFO + VPOffset, Val);

  Val = SclCalScale2 (VDisplay, VSize) << 16;
  Val |= SclCalScale2 (VDisplay, VSize);
  Vop2Writel (Vop2->BaseAddress, RK3568_VP0_POST_SCL_FACTOR_YRGB + VPOffset, Val);

#define POST_HORIZONTAL_SCALEDOWN_EN(x)		((x) << 0)
#define POST_VERTICAL_SCALEDOWN_EN(x)		((x) << 1)
  Vop2Writel (Vop2->BaseAddress, RK3568_VP0_POST_SCL_CTRL + VPOffset,
             POST_HORIZONTAL_SCALEDOWN_EN(HDisplay != HSize) |
             POST_VERTICAL_SCALEDOWN_EN(VDisplay != VSize));

  if (Mode->Flags & DRM_MODE_FLAG_INTERLACE) {
    UINT16 VActStartF1 = VTotal + VActStart + 1;
    UINT16 VActEndF1 = VActStartF1 + VDisplay;

    Val = VActStartF1 << 16 | VActEndF1;
    Vop2Writel (Vop2->BaseAddress, RK3568_VP0_POST_DSP_VACT_INFO_F1 + VPOffset, Val);
  }

  BgOvlDly = Crtc->Vps[CrtcState->CrtcID].BgOvlDly;
  BgDly = Vop2->Data->VpData[CrtcState->CrtcID].PreScanMaxDly;
  BgDly -= BgOvlDly;
  PreScanDly = BgDly + (HDisplay >> 1) - 1;
  if (Vop2->Version == VOP_VERSION_RK3588 && HSyncLen < 8)
    HSyncLen = 8;
  PreScanDly = (PreScanDly << 16) | HSyncLen;
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_BG_MIX_CTRL + CrtcState->CrtcID * 4, BG_MIX_CTRL_MASK,
                 BG_MIX_CTRL_SHIFT, BgDly, FALSE);
  Vop2Writel (Vop2->BaseAddress, RK3568_VP0_PRE_SCAN_HTIMING + VPOffset, PreScanDly);

  MmioWrite32 (0xfdd906e8, 0x34000000);
}

STATIC
EFI_STATUS
Vop2SetClk (
  IN UINT64                                Rate
)
{
  if (Rate == 148500000) {
    MmioWrite32(0xfD7C0160, 0xFFFF00C6);
    MmioWrite32(0xfD7C0164, 0xFFFF0082);
    MmioWrite32(0xfD7C0280, 0xFFFF0155);
    MmioWrite32(0xfD7C04C0, 0xFFFF0043);
    MmioWrite32(0xfD7C04C4, 0xFFFF0005);
    } else if (Rate == 200000000) {
    MmioWrite32(0xfD7C0160, 0xFFFF00C8);
    MmioWrite32(0xfD7C0164, 0xFFFF0082);
    MmioWrite32(0xfD7C0280, 0xFFFF0155);
    MmioWrite32(0xfD7C04BC, 0xFFFF0201);
    MmioWrite32(0xfD7C04C0, 0xFFFF0042);
  } else {
    return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

STATIC
CHAR8 *
GetOutputIfName (
  IN UINT32                                OutputIf
  )
{
  INT32 i = 0;
  INT32 Shift = 0;

  for (i = 0; i < VOP_OUTPUT_IF_NUMS; i++) {
    Shift = 1 << i;
    if (OutputIf & Shift) {
      return mDisplayIfName[i];
    }
  }

  return mDisplayIfName[0];
}

EFI_STATUS
Vop2PreInit (
  IN  ROCKCHIP_CRTC_PROTOCOL               *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;

  if (!RockchipVop2) {
    RockchipVop2 = AllocatePool (sizeof(*RockchipVop2));
    RockchipVop2->BaseAddress = RK3588_VOP2_REG_BASE;
    RockchipVop2->Version = mVop2RK3588.Version;
    RockchipVop2->Data = &mVop2RK3588;
    RockchipVop2->GlobalInit = FALSE;
    memset(RockchipVop2->VpPlaneMask, 0, sizeof(VOP2_VP_PLANE_MASK) * VOP2_VP_MAX);
  }

  CrtcState->Private = RockchipVop2;
  CrtcState->MaxOutput = RockchipVop2->Data->VpData[CrtcState->CrtcID].MaxOutput;
  CrtcState->Feature = RockchipVop2->Data->VpData[CrtcState->CrtcID].Feature;

  /* fix the plane mask about */
  Vop2GlobalInitial (DisplayState);

  return EFI_SUCCESS;
}

#define OUTPUT_IF_IS_HDMI(x) ((x) & (VOP_OUTPUT_IF_HDMI0 | VOP_OUTPUT_IF_HDMI1))
#define OUTPUT_IF_IS_DP(x) ((x) & (VOP_OUTPUT_IF_DP0 | VOP_OUTPUT_IF_DP1))

STATIC
VOID
Vop2PostColorSwap (
  OUT DISPLAY_STATE                        *DisplayState,
  OUT VOP2                                 *Vop2
  )
{
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  UINT32 VPOffset = CrtcState->CrtcID * 0x100;
  UINT32 OutputIf = ConnectorState->OutputInterface;
  UINT32 DateSwap = 0;

  if (IsUVSwap(ConnectorState->BusFormat, ConnectorState->OutputMode))
    DateSwap = DSP_RB_SWAP;

  if (Vop2->Version == VOP_VERSION_RK3588 &&
      (OUTPUT_IF_IS_HDMI(OutputIf) || OUTPUT_IF_IS_DP(OutputIf)) &&
      (ConnectorState->BusFormat == MEDIA_BUS_FMT_YUV8_1X24 ||
       ConnectorState->BusFormat == MEDIA_BUS_FMT_YUV10_1X30))
    DateSwap |= DSP_RG_SWAP;

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, DATA_SWAP_MASK,
                 DATA_SWAP_SHIFT, DateSwap, FALSE);
}

EFI_STATUS
Vop2Init (
  IN  ROCKCHIP_CRTC_PROTOCOL               *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  DRM_DISPLAY_MODE *Mode = &ConnectorState->DisplayMode;
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  VOP2 *Vop2 = CrtcState->Private;;
  UINT32 VPOffset = CrtcState->CrtcID * 0x100;
  UINT32 LineFlagOffset = CrtcState->CrtcID * 0x4;
  UINT16 HSyncLen, HDisplay, HTotal, HActStart, HActEnd;
  UINT16 VSyncLen, VDisplay, VTotal, VActStart, VActEnd;
  UINT32 Val, ActEnd;
  UINT8 DitherDownEn;
  UINT8 PreDitherDownEn;
  BOOLEAN YUVOverlay;
  UINT64 DclkRate;

  HSyncLen = Mode->CrtcHSyncEnd - Mode->CrtcHSyncStart;
  HDisplay = Mode->CrtcHDisplay;
  HTotal = Mode->CrtcHTotal;
  HActStart = Mode->CrtcHTotal - Mode->CrtcHSyncStart;
  HActEnd = HActStart + HDisplay;

  VSyncLen = Mode->CrtcVSyncEnd - Mode->CrtcVSyncStart;
  VDisplay = Mode->CrtcVDisplay;
  VTotal = Mode->CrtcVTotal;
  VActStart = Mode->CrtcVTotal - Mode->CrtcVSyncStart;
  VActEnd = VActStart + VDisplay;

  DEBUG((DEBUG_INIT, "[INIT]VOP update mode to: %dx%d%a%d, type:%a for VP%d\n",
         Mode->HDisplay, Mode->VDisplay,
         Mode->Flags & DRM_MODE_FLAG_INTERLACE ? "i" : "p",
         Mode->VScan,
         GetOutputIfName(ConnectorState->OutputInterface),
         CrtcState->CrtcID));

  DclkRate = Vop2IfConfig (DisplayState, Vop2);

  if (ConnectorState->OutputMode == ROCKCHIP_OUT_MODE_AAAA &&
      !(CrtcState->Feature & VOP_FEATURE_OUTPUT_10BIT))
  ConnectorState->OutputMode = ROCKCHIP_OUT_MODE_P888;

  Vop2PostColorSwap (DisplayState, Vop2);

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, OUT_MODE_MASK,
                 OUT_MODE_SHIFT, ConnectorState->OutputMode, FALSE);

  switch (ConnectorState->BusFormat) {
  case MEDIA_BUS_FMT_RGB565_1X16:
    DitherDownEn = 1;
    break;
  case MEDIA_BUS_FMT_RGB666_1X18:
  case MEDIA_BUS_FMT_RGB666_1X24_CPADHI:
  case MEDIA_BUS_FMT_RGB666_1X7X3_SPWG:
  case MEDIA_BUS_FMT_RGB666_1X7X3_JEIDA:
    DitherDownEn = 1;
    break;
  case MEDIA_BUS_FMT_YUV8_1X24:
  case MEDIA_BUS_FMT_UYYVYY8_0_5X24:
    DitherDownEn = 0;
    PreDitherDownEn = 1;
    break;
  case MEDIA_BUS_FMT_YUV10_1X30:
  case MEDIA_BUS_FMT_UYYVYY10_0_5X30:
  case MEDIA_BUS_FMT_RGB888_1X24:
  case MEDIA_BUS_FMT_RGB888_1X7X4_SPWG:
  case MEDIA_BUS_FMT_RGB888_1X7X4_JEIDA:
  default:
    DitherDownEn = 0;
    PreDitherDownEn = 0;
    break;
  }

  if (ConnectorState->OutputMode == ROCKCHIP_OUT_MODE_AAAA)
    PreDitherDownEn = 0;
  else
    PreDitherDownEn = 1;
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, EN_MASK,
                 DITHER_DOWN_EN_SHIFT, DitherDownEn, FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, EN_MASK,
                 PRE_DITHER_DOWN_EN_SHIFT, PreDitherDownEn, FALSE);

  YUVOverlay = IsYUVOutput (ConnectorState->BusFormat) ? 1 : 0;
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_OVL_CTRL, EN_MASK,
                 CrtcState->CrtcID, YUVOverlay, FALSE);
  CrtcState->YUVOverlay = YUVOverlay;

  Vop2Writel (Vop2->BaseAddress, RK3568_VP0_DSP_HTOTAL_HS_END + VPOffset,
           (HTotal << 16) | HSyncLen);
  Val = HActStart << 16;
  Val |= HActEnd;

  Vop2Writel (Vop2->BaseAddress, RK3568_VP0_DSP_HACT_ST_END + VPOffset, Val);
  Val = VActStart << 16;
  Val |= VActEnd;
  Vop2Writel (Vop2->BaseAddress, RK3568_VP0_DSP_VACT_ST_END + VPOffset, Val);
  if (Mode->Flags & DRM_MODE_FLAG_INTERLACE) {
    UINT16 VActStartF1 = VTotal + VActStart + 1;
    UINT16 VActEndF1 = VActStartF1 + VDisplay;

    Val = VActStartF1 << 16 | VActEndF1;
    Vop2Writel (Vop2->BaseAddress, RK3568_VP0_DSP_VACT_ST_END_F1 + VPOffset, Val);
    Val = VTotal << 16 | (VTotal + VSyncLen);
    Vop2Writel (Vop2->BaseAddress, RK3568_VP0_DSP_VS_ST_END_F1 + VPOffset, Val);

    Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, EN_MASK,
                   INTERLACE_EN_SHIFT, 1, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, EN_MASK,
                   DSP_FILED_POL, 1, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, EN_MASK,
                   P2I_EN_SHIFT, 1, FALSE);
    VTotal += VTotal + 1;
    ActEnd = VActEndF1;
  }
  else {
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, EN_MASK,
                   INTERLACE_EN_SHIFT, 0, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, EN_MASK,
                   P2I_EN_SHIFT, 0, FALSE);
    ActEnd = VActEnd;
  }
  Vop2Writel (Vop2->BaseAddress, RK3568_VP0_DSP_VTOTAL_VS_END + VPOffset,
              (VTotal << 16) | VSyncLen);

  if (Vop2->Version == VOP_VERSION_RK3568) {
    if (Mode->Flags & DRM_MODE_FLAG_DBLCLK ||
        ConnectorState->OutputInterface & VOP_OUTPUT_IF_BT656)
      Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, EN_MASK,
                     CORE_DCLK_DIV_EN_SHIFT, 1, FALSE);
    else
      Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, EN_MASK,
                     CORE_DCLK_DIV_EN_SHIFT, 0, FALSE);
  }

  if (ConnectorState->OutputMode == ROCKCHIP_OUT_MODE_YUV420)
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_MIPI_CTRL + VPOffset, DCLK_DIV2_MASK,
                   DCLK_DIV2_SHIFT, 0x3, FALSE);
  else
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_MIPI_CTRL + VPOffset, DCLK_DIV2_MASK,
                   DCLK_DIV2_SHIFT, 0x0, FALSE);

  if (YUVOverlay)
    Val = 0x20010200;
  else
    Val = 0;

  /* RK3568_VP0_DSP_BG can't be changed --- todo now */
  Vop2Writel (Vop2->BaseAddress, RK3568_VP0_DSP_BG + VPOffset, 0);

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, EN_MASK,
                 POST_DSP_OUT_R2Y_SHIFT, YUVOverlay, FALSE);

  Vop2TVConfigUpdate (DisplayState, Vop2);
  Vop2PostConfig (DisplayState, Vop2);

  Vop2SetClk (DclkRate * 1000);

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_SYS_CTRL_LINE_FLAG0 + LineFlagOffset, LINE_FLAG_NUM_MASK,
                 RK3568_DSP_LINE_FLAG_NUM0_SHIFT, ActEnd, FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_SYS_CTRL_LINE_FLAG0 + LineFlagOffset, LINE_FLAG_NUM_MASK,
                 RK3568_DSP_LINE_FLAG_NUM0_SHIFT, ActEnd, FALSE);

  return EFI_SUCCESS;
}

INLINE
SCALE_MODE
GetSclMode (
  IN  UINT32                               Src,
  IN  UINT32                               Dst
  )
{
  if (Src < Dst)
    return SCALE_UP;
  else if (Src > Dst)
    return SCALE_DOWN;

  return SCALE_NONE;
}

/*
 * bli_sd_factor = (src - 1) / (dst - 1) << 12;
 * avg_sd_factor:
 * bli_su_factor:
 * bic_su_factor:
 * = (src - 1) / (dst - 1) << 16;
 *
 * gt2 enable: dst get one line from two line of the src
 * gt4 enable: dst get one line from four line of the src.
 *
 */
#define VOP2_BILI_SCL_DN(src, dst)	(((src - 1) << 12) / (dst - 1))
#define VOP2_COMMON_SCL(src, dst)	(((src - 1) << 16) / (dst - 1))

#define VOP2_BILI_SCL_FAC_CHECK(src, dst, fac)	 \
				(fac * (dst - 1) >> 12 < (src - 1))
#define VOP2_COMMON_SCL_FAC_CHECK(src, dst, fac) \
				(fac * (dst - 1) >> 16 < (src - 1))

STATIC
UINT16
Vop2ScaleFactor (
  IN  SCALE_MODE                           Mode,
  IN  VOP2_SCALE_DOWN_MODE                 FilterMode,
  IN  UINT32                               Src,
  IN  UINT32                               Dst
  )
{
  UINT32 Factor = 0;
  INT32 i = 0;

  if (Mode == SCALE_NONE)
    return 0;

  if ((Mode == SCALE_DOWN) && (FilterMode == VOP2_SCALE_DOWN_BIL)) {
    Factor = VOP2_BILI_SCL_DN(Src, Dst);
    for (i = 0; i < 100; i++) {
      if (VOP2_BILI_SCL_FAC_CHECK(Src, Dst, Factor))
        break;
      Factor -= 1;
      DEBUG ((DEBUG_INFO, "down fac cali: src:%d, dst:%d, fac:0x%x\n", Src, Dst, Factor));
    }
  } else {
    for (i = 0; i < 100; i++) {
      if (VOP2_COMMON_SCL_FAC_CHECK(Src, Dst, Factor))
        break;
      Factor -= 1;
      DEBUG ((DEBUG_INFO, "up fac cali:  src:%d, dst:%d, fac:0x%x\n", Src, Dst, Factor));
    }
  }

  return Factor;
}

STATIC
VOID
Vop2SetupScale (
  OUT VOP2                                 *Vop2,
  OUT VOP2_WIN_DATA                        *WinData,
  IN  UINT32                               SrcW,
  IN  UINT32                               SrcH,
  IN  UINT32                               DstW,
  IN  UINT32                               DstH
  )
{
  UINT16 YrgbHorSclMode, YrgbVerSclMode;
  UINT16 HsclFilterMode, VsclFilterMode;
  UINT8 GT2 = 0, GT4 =0;
  UINT32 XFac = 0, YFac = 0;
  VOP2_SCALE_UP_MODE HSUFilterMode = VOP2_SCALE_UP_BIC;
  VOP2_SCALE_DOWN_MODE HSDFilterMode = VOP2_SCALE_DOWN_BIL;
  VOP2_SCALE_UP_MODE VSUFilterMode = VOP2_SCALE_UP_BIL;
  VOP2_SCALE_DOWN_MODE VSDFilterMode = VOP2_SCALE_DOWN_BIL;
  UINT32 WinOffset = WinData->RegOffset;

  if (SrcH > (4 * DstH))
    GT4 = 1;
  else if (SrcH >= (2 * DstH))
    GT2 = 1;

  if (GT4)
    SrcH >>= 2;
  else if (GT2)
    SrcH >>= 1;

  YrgbHorSclMode = GetSclMode(SrcW, DstW);
  YrgbVerSclMode = GetSclMode(SrcH, DstH);

  if (YrgbHorSclMode == SCALE_UP)
    HsclFilterMode = HSUFilterMode;
  else
    HsclFilterMode = HSDFilterMode;

  if (YrgbVerSclMode == SCALE_UP)
    VsclFilterMode = VSUFilterMode;
  else
    VsclFilterMode = VSDFilterMode;

  /*
   * RK3568 VOP Esmart/Smart DstW should be even pixel
   * at scale down mode
   */
  if ((YrgbHorSclMode == SCALE_DOWN) && (DstW & 0x1)) {
    DEBUG ((DEBUG_INFO, "win dst_w[%d] should align as 2 pixel\n", DstW));
    DstW += 1;
  }

  XFac = Vop2ScaleFactor (YrgbHorSclMode, HsclFilterMode, SrcW, DstW);
  YFac = Vop2ScaleFactor (YrgbVerSclMode, VsclFilterMode, SrcH, DstH);

  if (WinData->Type == CLUSTER_LAYER) {
    Vop2Writel (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_SCL_FACTOR_YRGB + WinOffset, YFac << 16 | XFac);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_CTRL1 + WinOffset, YRGB_GT2_MASK,
                   CLUSTER_YRGB_GT2_SHIFT, GT2, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_CTRL1 + WinOffset, YRGB_GT4_MASK,
                   CLUSTER_YRGB_GT4_SHIFT, GT4, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_CTRL1 + WinOffset, YRGB_XSCL_MODE_MASK,
                   CLUSTER_YRGB_XSCL_MODE_SHIFT, YrgbHorSclMode, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_CTRL1 + WinOffset, YRGB_YSCL_MODE_MASK,
                   CLUSTER_YRGB_YSCL_MODE_SHIFT, YrgbVerSclMode, FALSE);
  } else {
    Vop2Writel (Vop2->BaseAddress, RK3568_ESMART0_REGION0_SCL_FACTOR_YRGB + WinOffset, YFac << 16 | XFac);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_ESMART0_REGION0_CTRL + WinOffset, YRGB_GT2_MASK,
                   YRGB_GT2_SHIFT, GT2, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_ESMART0_REGION0_CTRL + WinOffset, YRGB_GT4_MASK,
                   YRGB_GT4_SHIFT, GT4, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_ESMART0_REGION0_SCL_CTRL + WinOffset, YRGB_XSCL_MODE_MASK,
                   YRGB_XSCL_MODE_SHIFT, YrgbHorSclMode, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_ESMART0_REGION0_SCL_CTRL + WinOffset, YRGB_YSCL_MODE_MASK,
                   YRGB_YSCL_MODE_SHIFT, YrgbVerSclMode, FALSE);

    Vop2MaskWrite (Vop2->BaseAddress, RK3568_ESMART0_REGION0_SCL_CTRL + WinOffset, YRGB_XSCL_FILTER_MODE_MASK,
                   YRGB_XSCL_FILTER_MODE_SHIFT, HsclFilterMode, FALSE);
    Vop2MaskWrite (Vop2->BaseAddress, RK3568_ESMART0_REGION0_SCL_CTRL + WinOffset, YRGB_YSCL_FILTER_MODE_MASK,
                   YRGB_YSCL_FILTER_MODE_SHIFT, VsclFilterMode, FALSE);
  }
}

STATIC
VOID
Vop2SetClusterWin (
  OUT DISPLAY_STATE                        *DisplayState,
  OUT VOP2_WIN_DATA                        *WinData
  )
{
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  DRM_DISPLAY_MODE *Mode = &ConnectorState->DisplayMode;
  VOP2 *Vop2 = CrtcState->Private;
  INT32 SrcW = CrtcState->SrcW;
  INT32 SrcH = CrtcState->SrcH;
  INT32 CrtcW = CrtcState->CrtcW;
  INT32 CrtcH = CrtcState->CrtcH;
  INT32 CrtcX = CrtcState->CrtcX;
  INT32 CrtcY = CrtcState->CrtcY;
  INT32 XVirtual = CrtcState->XVirtual;
  INT32 YMirror = 0;
  INT32 CSCMode;
  UINT32 ActInfo, DspInfo, DspSt, DspStx, DspSty;
  UINT32 WinOffset = WinData->RegOffset;
  UINT32 CfgDone = CFG_DONE_EN | BIT(CrtcState->CrtcID) | (BIT(CrtcState->CrtcID) << 16);

  ActInfo = (SrcH - 1) << 16;
  ActInfo |= (SrcW - 1) & 0xffff;

  DspInfo = (CrtcH - 1) << 16;
  DspInfo |= (CrtcW - 1) & 0xffff;

  DspStx = CrtcX;
  DspSty = CrtcY;
  DspSt = DspSty << 16 | (DspStx & 0xffff);

  if (Mode->Flags & DRM_MODE_FLAG_YMIRROR)
    YMirror = 1;
  else
    YMirror = 0;

  Vop2SetupScale(Vop2, WinData, SrcW, SrcH, CrtcW, CrtcH);

  if (YMirror)
    DEBUG ((DEBUG_WARN, "WARN: y mirror is unsupported by cluster window\n"));

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_CTRL0 + WinOffset, WIN_FORMAT_MASK,
                 WIN_FORMAT_SHIFT, CrtcState->Format, FALSE);
  Vop2Writel (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_VIR + WinOffset, XVirtual);
  Vop2Writel (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_YRGB_MST + WinOffset, CrtcState->DMAAddress);

  Vop2Writel (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_ACT_INFO + WinOffset, ActInfo);
  Vop2Writel (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_DSP_INFO + WinOffset, DspInfo);
  Vop2Writel (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_DSP_ST + WinOffset, DspSt);

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_CTRL0 + WinOffset, EN_MASK,
                 WIN_EN_SHIFT, 1, FALSE);

  CSCMode = Vop2ConvertCSCMode(ConnectorState->ColorSpace);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_CTRL0 + WinOffset, EN_MASK,
                 CLUSTER_RGB2YUV_EN_SHIFT, IsYUVOutput(ConnectorState->BusFormat), FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_CLUSTER0_WIN0_CTRL0 + WinOffset, CSC_MODE_MASK,
                 CLUSTER_CSC_MODE_SHIFT, CSCMode, FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_CLUSTER0_CTRL + WinOffset, EN_MASK,
                 CLUSTER_EN_SHIFT, 1, FALSE);

  Vop2Writel (Vop2->BaseAddress, RK3568_REG_CFG_DONE, CfgDone);
}

STATIC
VOID
Vop2SetSmartWin (
  OUT DISPLAY_STATE                        *DisplayState,
  OUT VOP2_WIN_DATA                        *WinData
  )
{
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  CONNECTOR_STATE *ConnectorState = &DisplayState->ConnectorState;
  DRM_DISPLAY_MODE *Mode = &ConnectorState->DisplayMode;
  VOP2 *Vop2 = CrtcState->Private;
  INT32 SrcW = CrtcState->SrcW;
  INT32 SrcH = CrtcState->SrcH;
  INT32 CrtcW = CrtcState->CrtcW;
  INT32 CrtcH = CrtcState->CrtcH;
  INT32 CrtcX = CrtcState->CrtcX;
  INT32 CrtcY = CrtcState->CrtcY;
  INT32 XVirtual = CrtcState->XVirtual;
  INT32 YMirror = 0;
  INT32 CSCMode;
  UINT32 ActInfo, DspInfo, DspSt, DspStx, DspSty;
  UINT32 WinOffset = WinData->RegOffset;
  UINT32 CfgDone = CFG_DONE_EN | BIT(CrtcState->CrtcID) | (BIT(CrtcState->CrtcID) << 16);

  /*
   * This is workaround solution for IC design:
   * esmart can't support scale down when actual_w % 16 == 1.
   */
  if (SrcW > CrtcW && (SrcW & 0xf) == 1) {
    DEBUG ((DEBUG_WARN, "WARN: vp%d unsupported act_w[%d] mode 16 = 1 when scale down\n", CrtcState->CrtcID, SrcW));
    SrcW -= 1;
  }

  ActInfo = (SrcH - 1) << 16;
  ActInfo |= (SrcW - 1) & 0xffff;

  DspInfo = (CrtcH - 1) << 16;
  DspInfo |= (CrtcW - 1) & 0xffff;

  DspStx = CrtcX;
  DspSty = CrtcY;
  DspSt = DspSty << 16 | (DspStx & 0xffff);

  if (Mode->Flags & DRM_MODE_FLAG_YMIRROR)
    YMirror = 1;
  else
    YMirror = 0;

  Vop2SetupScale(Vop2, WinData, SrcW, SrcH, CrtcW, CrtcH);

  if (YMirror)
    CrtcState->DMAAddress += (SrcH - 1) * XVirtual * 4;
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_ESMART0_CTRL1 + WinOffset, EN_MASK,
                 YMIRROR_EN_SHIFT, YMirror, FALSE);

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_ESMART0_REGION0_CTRL + WinOffset, WIN_FORMAT_MASK,
                 WIN_FORMAT_SHIFT, CrtcState->Format, FALSE);
  Vop2Writel (Vop2->BaseAddress, RK3568_ESMART0_REGION0_VIR + WinOffset, XVirtual);
  Vop2Writel (Vop2->BaseAddress, RK3568_ESMART0_REGION0_YRGB_MST + WinOffset, CrtcState->DMAAddress);

  Vop2Writel (Vop2->BaseAddress, RK3568_ESMART0_REGION0_ACT_INFO + WinOffset, ActInfo);
  Vop2Writel (Vop2->BaseAddress, RK3568_ESMART0_REGION0_DSP_INFO + WinOffset, DspInfo);
  Vop2Writel (Vop2->BaseAddress, RK3568_ESMART0_REGION0_DSP_ST + WinOffset, DspSt);

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_ESMART0_REGION0_CTRL + WinOffset, EN_MASK,
                 WIN_EN_SHIFT, 1, FALSE);

  CSCMode = Vop2ConvertCSCMode(ConnectorState->ColorSpace);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_ESMART0_CTRL0 + WinOffset, EN_MASK,
                 RGB2YUV_EN_SHIFT, IsYUVOutput(ConnectorState->BusFormat), FALSE);
  Vop2MaskWrite (Vop2->BaseAddress, RK3568_ESMART0_CTRL0 + WinOffset, CSC_MODE_MASK,
                 CSC_MODE_SHIFT, CSCMode, FALSE);

  Vop2Writel (Vop2->BaseAddress, RK3568_REG_CFG_DONE, CfgDone);
}

EFI_STATUS
Vop2SetPlane (
  IN  ROCKCHIP_CRTC_PROTOCOL               *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  VOP2 *Vop2 = CrtcState->Private;
  VOP2_WIN_DATA *WinData;
  UINT8 PrimaryPlaneID = Vop2->VpPlaneMask[CrtcState->CrtcID].PrimaryPlaneId;

  if (CrtcState->CrtcW > CrtcState->MaxOutput.Width) {
    DEBUG ((DEBUG_ERROR, "ERROR: output w[%d] exceeded max width[%d]\n",
            CrtcState->CrtcW, CrtcState->MaxOutput.Width));
    return -EFI_INVALID_PARAMETER;
  }

  WinData = Vop2FindWinByPhysID (Vop2, PrimaryPlaneID);
  if (!WinData) {
    DEBUG ((DEBUG_ERROR, "invalid win id %d\n", PrimaryPlaneID));
    return -EFI_INVALID_PARAMETER;
  }

  if (WinData->Type == CLUSTER_LAYER)
    Vop2SetClusterWin (DisplayState, WinData);
  else
    Vop2SetSmartWin (DisplayState, WinData);

  DEBUG ((DEBUG_INFO, "VOP VP%d enable %a[%dx%d->%dx%d@%dx%d] fmt[%d] addr[0x%x]\n",
          CrtcState->CrtcID, GetPlaneName(PrimaryPlaneID),
          CrtcState->SrcW, CrtcState->SrcH, CrtcState->CrtcW, CrtcState->CrtcH,
          CrtcState->CrtcX, CrtcState->CrtcY, CrtcState->Format,
          CrtcState->DMAAddress));

  return EFI_SUCCESS;
}

EFI_STATUS
Vop2Enable (
  IN  ROCKCHIP_CRTC_PROTOCOL               *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  VOP2 *Vop2 = CrtcState->Private;
  UINT32 VPOffset = CrtcState->CrtcID * 0x100;
  UINT32 CfgDone = CFG_DONE_EN | BIT(CrtcState->CrtcID) | (BIT(CrtcState->CrtcID) << 16);

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, EN_MASK,
                 STANDBY_EN_SHIFT, 0, FALSE);
  Vop2Writel (Vop2->BaseAddress, RK3568_REG_CFG_DONE, CfgDone);

#ifdef DEBUG_DUMP_REG
  Vop2DumpRegisters (DisplayState, Vop2FindWinByPhysID (Vop2, Vop2->VpPlaneMask[CrtcState->CrtcID].PrimaryPlaneId));
#endif

  return EFI_SUCCESS;
}

EFI_STATUS
Vop2Disable (
  IN  ROCKCHIP_CRTC_PROTOCOL               *This,
  OUT DISPLAY_STATE                        *DisplayState
  )
{
  CRTC_STATE *CrtcState = &DisplayState->CrtcState;
  VOP2 *Vop2 = CrtcState->Private;
  UINT32 VPOffset = CrtcState->CrtcID * 0x100;
  UINT32 CfgDone = CFG_DONE_EN | BIT(CrtcState->CrtcID) | (BIT(CrtcState->CrtcID) << 16);

  Vop2MaskWrite (Vop2->BaseAddress, RK3568_VP0_DSP_CTRL + VPOffset, EN_MASK,
                 STANDBY_EN_SHIFT, 1, FALSE);
  Vop2Writel (Vop2->BaseAddress, RK3568_REG_CFG_DONE, CfgDone);

  return EFI_SUCCESS;
}

STATIC ROCKCHIP_CRTC_PROTOCOL mVop2 = {
  &mVop2RK3588,
  Vop2PreInit,
  Vop2Init,
  NULL,
  Vop2SetPlane,
  NULL,
  Vop2Enable,
  Vop2Disable,
  NULL,
  {
  },
  {},
  FALSE,
  FALSE,
  FALSE
};

EFI_STATUS
Vop2DxeInitialize (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_HANDLE    Handle;

  Handle = NULL;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gRockchipCrtcProtocolGuid,
                  &mVop2,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

