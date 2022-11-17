/** SerDes.c
  Provides SoC specific SerDes interface

  Copyright 2017-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/ChassisLib.h>
#include <Library/DebugLib.h>
#include <Library/SerDesHelperLib.h>
#include <SocSerDes.h>
#include <Soc.h>
#include <Uefi.h>

// SerDes1 Protocol Mask in Reset Configuration Word (RCW) Status Register
#define SERDES1_PROTOCOL_MASK      0xffff0000

// SerDes1 Protocol Shift in Reset Configuration Word (RCW) Status Register
#define SERDES1_PROTOCOL_SHIFT     16

STATIC SERDES_CONFIG mSerDes1ConfigTable[] = {
  {0x1555, {XFI_FM1_MAC9, PCIE1, PCIE2, PCIE3 } },
  {0x2555, {SGMII_2500_FM1_DTSEC9, PCIE1, PCIE2, PCIE3 } },
  {0x4555, {QSGMII_FM1_A, PCIE1, PCIE2, PCIE3 } },
  {0x4558, {QSGMII_FM1_A,  PCIE1, PCIE2, SATA } },
  {0x1355, {XFI_FM1_MAC9, SGMII_FM1_DTSEC2, PCIE2, PCIE3 } },
  {0x2355, {SGMII_2500_FM1_DTSEC9, SGMII_FM1_DTSEC2, PCIE2, PCIE3 } },
  {0x3335, {SGMII_FM1_DTSEC9, SGMII_FM1_DTSEC2, SGMII_FM1_DTSEC5, PCIE3 } },
  {0x3355, {SGMII_FM1_DTSEC9, SGMII_FM1_DTSEC2, PCIE2, PCIE3 } },
  {0x3358, {SGMII_FM1_DTSEC9, SGMII_FM1_DTSEC2, PCIE2, SATA } },
  {0x3555, {SGMII_FM1_DTSEC9, PCIE1, PCIE2, PCIE3 } },
  {0x3558, {SGMII_FM1_DTSEC9, PCIE1, PCIE2, SATA } },
  {0x7000, {PCIE1, PCIE1, PCIE1, PCIE1 } },
  {0x9998, {PCIE1, PCIE2, PCIE3, SATA } },
  {0x6058, {PCIE1, PCIE1, PCIE2, SATA } },
  {0x1455, {XFI_FM1_MAC9, QSGMII_FM1_A, PCIE2, PCIE3 } },
  {0x2455, {SGMII_2500_FM1_DTSEC9, QSGMII_FM1_A, PCIE2, PCIE3 } },
  {0x2255, {SGMII_2500_FM1_DTSEC9, SGMII_2500_FM1_DTSEC2, PCIE2, PCIE3 } },
  {0x3333, {SGMII_FM1_DTSEC9, SGMII_FM1_DTSEC2, SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6 } },
  {0x1460, {XFI_FM1_MAC9, QSGMII_FM1_A, PCIE3, PCIE3 } },
  {0x2460, {SGMII_2500_FM1_DTSEC9, QSGMII_FM1_A, PCIE3, PCIE3 } },
  {0x3460, {SGMII_FM1_DTSEC9, QSGMII_FM1_A, PCIE3, PCIE3 } },
  {0x3455, {SGMII_FM1_DTSEC9, QSGMII_FM1_A, PCIE2, PCIE3 } },
  {0x9960, {PCIE1, PCIE2, PCIE3, PCIE3 } },
  {0x2233, {SGMII_2500_FM1_DTSEC9, SGMII_FM1_DTSEC2, SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6 } },
  {0x2533, {SGMII_2500_FM1_DTSEC9, PCIE1, SGMII_FM1_DTSEC5, SGMII_FM1_DTSEC6 } },
  {}
};

SERDES_CONFIG *gSerDesConfig[] = {
  mSerDes1ConfigTable
};

/**
  Probe all SerDes for lane protocol and execute provided callback function.

  @param  SerDesLaneProbeCallback Pointer Callback function to be called for Lane protocol
  @param  Arg                     Pointer to Arguments to be passed to callback function.

**/
VOID
SerDesProbeLanes (
  IN SERDES_PROBE_LANES_CALLBACK SerDesLaneProbeCallback,
  IN VOID                        *Arg
  )
{
  UINT32                 SerDesProtocol;
  LS1043A_DEVICE_CONFIG  *DeviceConfig;

  DeviceConfig = (LS1043A_DEVICE_CONFIG  *)LS1043A_DCFG_ADDRESS;
  SerDesProtocol = DcfgRead32 ((UINTN)&DeviceConfig->RcwSr[4]) & SERDES1_PROTOCOL_MASK;
  SerDesProtocol >>= SERDES1_PROTOCOL_SHIFT;

  SerDesInstanceProbeLanes (
    SERDES_1,
    SerDesProtocol,
    FixedPcdGet8 (PcdSerDesLanes),
    SERDES_PROTOCOL_COUNT,
    gSerDesConfig[SERDES_1],
    SerDesLaneProbeCallback,
    Arg
    );
}

/**
  Function to return SerDes protocol map for all SerDes available on board.

  @param  SerDesProtocolMap   Pointer to SerDes protocl map.

**/
VOID
GetSerDesProtocolMap (
  OUT UINT64   *SerDesProtocolMap
  )
{
  UINT32                 SerDesProtocol;
  LS1043A_DEVICE_CONFIG  *DeviceConfig;
  EFI_STATUS             Status;

  *SerDesProtocolMap = 0;
  DeviceConfig = (LS1043A_DEVICE_CONFIG  *)LS1043A_DCFG_ADDRESS;
  SerDesProtocol = DcfgRead32 ((UINTN)&DeviceConfig->RcwSr[4]) & SERDES1_PROTOCOL_MASK;
  SerDesProtocol >>= SERDES1_PROTOCOL_SHIFT;

  Status = GetSerDesMap (
             SERDES_1,
             SerDesProtocol,
             FixedPcdGet8 (PcdSerDesLanes),
             SERDES_PROTOCOL_COUNT,
             gSerDesConfig[SERDES_1],
             SerDesProtocolMap
             );

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: failed for SerDes1 \n",__FUNCTION__));
    *SerDesProtocolMap = 0;
  }
}
