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

// SerDes Protocol Mask and Shift in Reset Configuration Word (RCW) Status Register
#define SERDES1_PROTOCOL_MASK       0x001f0000
#define SERDES1_PROTOCOL_SHIFT      16
#define SERDES2_PROTOCOL_MASK       0x03E00000
#define SERDES2_PROTOCOL_SHIFT      21
#define SERDES3_PROTOCOL_MASK       0x7C000000
#define SERDES3_PROTOCOL_SHIFT      26

STATIC SERDES_CONFIG mSerDes1ConfigTable[] = {
  {  1, { PCIE2, PCIE2, PCIE2, PCIE2, PCIE1, PCIE1, PCIE1, PCIE1 } },
  {  2, { PCIE2, PCIE2, PCIE2, PCIE2, SGMII6, SGMII5, SGMII4, SGMII3 } },
  {  3, { PCIE2, PCIE2, PCIE2, PCIE2, XFI6, XFI5, XFI4, XFI3 } },
  {  4, { SGMII10, SGMII9, SGMII8, SGMII7, SGMII6, SGMII5, SGMII4, SGMII3 } },
  {  5, { XFI10, XFI9, XFI8, XFI7, PCIE1, PCIE1, PCIE1, PCIE1} },
  {  6, { SGMII10, SGMII9, SGMII8, SGMII7, SGMII6, SGMII5, XFI4, XFI3 } },
  {  7, { SGMII10, SGMII9, SGMII8, SGMII7, XFI6, XFI5, XFI4, XFI3  } },
  {  8, { XFI10, XFI9, XFI8, XFI7, XFI6, XFI5, XFI4, XFI3 } },
  {  9, { SGMII10, SGMII9, SGMII8, PCIE2, SGMII6, SGMII5, SGMII4, PCIE1 } },
  { 10, { XFI10, XFI9, XFI8, PCIE2, XFI6, XFI5, XFI4, PCIE1 } },
  { 11, { SGMII10, SGMII9, PCIE2, PCIE2, SGMII6, SGMII5, PCIE1, PCIE1 } },
  { 12, { SGMII10, SGMII9, PCIE2, PCIE2, PCIE1, PCIE1, PCIE1, PCIE1 } },
  { 13, { GE100_2, GE100_2, GE100_2, GE100_2, GE100_1, GE100_1, GE100_1, GE100_1 } },
  { 14, { PCIE2, PCIE2, PCIE2, PCIE2, GE100_1, GE100_1, GE100_1, GE100_1 } },
  { 15, { PCIE2, PCIE2, PCIE2, PCIE2, GE50_2, GE50_2, GE50_1, GE50_1 } },
  { 16, { PCIE2, PCIE2, PCIE2, PCIE2, GE25_6, GE25_5, GE50_1, GE50_1 } },
  { 17, { PCIE2, PCIE2, PCIE2, PCIE2, GE25_6, GE25_5, GE25_4, GE25_3 } },
  { 18, { XFI10, XFI9, XFI8, XFI7, GE25_6, GE25_5, XFI4, XFI3 } },
  { 19, { GE40_2, GE40_2, GE40_2, GE40_2, GE25_6, GE25_5, XFI4, XFI3 } },
  { 20, { GE40_2, GE40_2, GE40_2, GE40_2, GE40_1, GE40_1, GE40_1, GE40_1 } },
  { 21, { GE25_10, GE25_9, PCIE2, PCIE2, GE25_6, GE25_5, GE25_4, GE25_3 } },
  { 22, { XFI10, XFI9, PCIE2, PCIE2, XFI6, XFI5, XFI4, XFI3 } },
  {}
};

STATIC SERDES_CONFIG mSerDes2ConfigTable[] = {
  {  1, { PCIE3, PCIE3, SATA1, SATA2, PCIE4, PCIE4, PCIE4, PCIE4 } },
  {  2, { PCIE3, PCIE3, PCIE3, PCIE3, PCIE3, PCIE3, PCIE3, PCIE3 } },
  {  3, { PCIE3, PCIE3, PCIE3, PCIE3, PCIE4, PCIE4, PCIE4, PCIE4 } },
  {  4, { PCIE3, PCIE3, PCIE3, PCIE3, PCIE4, PCIE4, SATA1, SATA2 } },
  {  5, { PCIE3, PCIE3, PCIE3, PCIE3, SATA3, SATA4, SATA1, SATA2 } },
  {  6, { PCIE3, PCIE3, PCIE3, PCIE3, SGMII15, SGMII16, XFI13, XFI14} },
  {  7, { PCIE3, SGMII12, SGMII17, SGMII18, PCIE4, SGMII16, XFI13, XFI14} },
  {  8, { 0, 0, SATA1, SATA2, SATA3, SATA4, XFI13, XFI14} },
  {  9, { SGMII11, SGMII12, SGMII17, SGMII18, SGMII15, SGMII16, SGMII13, SGMII14 } },
  { 10, { SGMII11, SGMII12, SGMII17, SGMII18, PCIE4, PCIE4, PCIE4, PCIE4 } },
  { 11, { PCIE3, SGMII12, SGMII17, SGMII18, PCIE4, SGMII16, SGMII13, SGMII14 } },
  { 12, { SGMII11, SGMII12, SGMII17, SGMII18, PCIE4, PCIE4, SATA1, SATA2 } },
  { 13, { PCIE3, PCIE3, PCIE3, PCIE3, PCIE4, PCIE4, SGMII13, SGMII14 } },
  { 14, { PCIE3, PCIE3, SGMII17, SGMII18, PCIE4, PCIE4, SGMII13, SGMII14 } },
  {}
};

STATIC SERDES_CONFIG mSerDes3ConfigTable[] = {
  { 2, { PCIE5, PCIE5, PCIE5, PCIE5, PCIE5, PCIE5, PCIE5, PCIE5 } },
  { 3, { PCIE5, PCIE5, PCIE5, PCIE5, PCIE6, PCIE6, PCIE6, PCIE6 } },
  {}
};

SERDES_CONFIG *gSerDesConfig[] = {
  mSerDes1ConfigTable,
  mSerDes2ConfigTable,
  mSerDes3ConfigTable
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
  UINT32                 SerDes1Protocol;
  UINT32                 SerDes2Protocol;
  UINT32                 SerDes3Protocol;
  LX2160A_DEVICE_CONFIG  *DeviceConfig;

  DeviceConfig = (LX2160A_DEVICE_CONFIG  *)LX2160A_DCFG_ADDRESS;
  SerDes1Protocol = DcfgRead32 ((UINTN)&DeviceConfig->RcwSr[28]) & SERDES1_PROTOCOL_MASK;
  SerDes1Protocol >>= SERDES1_PROTOCOL_SHIFT;
  SerDes2Protocol = DcfgRead32 ((UINTN)&DeviceConfig->RcwSr[28]) & SERDES2_PROTOCOL_MASK;
  SerDes2Protocol >>= SERDES2_PROTOCOL_SHIFT;
  SerDes3Protocol = DcfgRead32 ((UINTN)&DeviceConfig->RcwSr[28]) & SERDES3_PROTOCOL_MASK;
  SerDes3Protocol >>= SERDES3_PROTOCOL_SHIFT;

  // SerDes1
  SerDesInstanceProbeLanes (
    SERDES_1,
    SerDes1Protocol,
    FixedPcdGet8 (PcdSerDesLanes),
    SERDES_PROTOCOL_COUNT,
    gSerDesConfig[SERDES_1],
    SerDesLaneProbeCallback,
    Arg
    );

  // SerDes2
  SerDesInstanceProbeLanes (
    SERDES_2,
    SerDes2Protocol,
    FixedPcdGet8 (PcdSerDesLanes),
    SERDES_PROTOCOL_COUNT,
    gSerDesConfig[SERDES_2],
    SerDesLaneProbeCallback,
    Arg
    );

  // SerDes3
  SerDesInstanceProbeLanes (
    SERDES_3,
    SerDes3Protocol,
    FixedPcdGet8 (PcdSerDesLanes),
    SERDES_PROTOCOL_COUNT,
    gSerDesConfig[SERDES_3],
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
  UINT32                 SerDes1Protocol;
  UINT32                 SerDes2Protocol;
  UINT32                 SerDes3Protocol;
  LX2160A_DEVICE_CONFIG  *DeviceConfig;
  EFI_STATUS             Status;

  DeviceConfig = (LX2160A_DEVICE_CONFIG  *)LX2160A_DCFG_ADDRESS;
  SerDes1Protocol = DcfgRead32 ((UINTN)&DeviceConfig->RcwSr[28]) & SERDES1_PROTOCOL_MASK;
  SerDes1Protocol >>= SERDES1_PROTOCOL_SHIFT;
  SerDes2Protocol = DcfgRead32 ((UINTN)&DeviceConfig->RcwSr[28]) & SERDES2_PROTOCOL_MASK;
  SerDes2Protocol >>= SERDES2_PROTOCOL_SHIFT;
  SerDes3Protocol = DcfgRead32 ((UINTN)&DeviceConfig->RcwSr[28]) & SERDES3_PROTOCOL_MASK;
  SerDes3Protocol >>= SERDES3_PROTOCOL_SHIFT;

  *SerDesProtocolMap = 0;

  // SerDes1
  Status = GetSerDesMap (
             SERDES_1,
             SerDes1Protocol,
             FixedPcdGet8 (PcdSerDesLanes),
             SERDES_PROTOCOL_COUNT,
             gSerDesConfig[SERDES_1],
             SerDesProtocolMap
             );

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: failed for SerDes1 \n",__FUNCTION__));
    *SerDesProtocolMap = 0;
  }

  // SerDes2
  Status = GetSerDesMap (
             SERDES_2,
             SerDes2Protocol,
             FixedPcdGet8 (PcdSerDesLanes),
             SERDES_PROTOCOL_COUNT,
             gSerDesConfig[SERDES_2],
             SerDesProtocolMap
             );

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: failed for SerDes2 \n",__FUNCTION__));
    *SerDesProtocolMap = 0;
  }

  // SerDes3
  Status = GetSerDesMap (
             SERDES_3,
             SerDes3Protocol,
             FixedPcdGet8 (PcdSerDesLanes),
             SERDES_PROTOCOL_COUNT,
             gSerDesConfig[SERDES_3],
             SerDesProtocolMap
             );

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "%a: failed for SerDes3 \n",__FUNCTION__));
    *SerDesProtocolMap = 0;
  }
}
