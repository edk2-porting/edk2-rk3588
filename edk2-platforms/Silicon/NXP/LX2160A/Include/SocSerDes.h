/** SocSerDes.h
  SoC Specific header file for SerDes

  Copyright 2017-2020 NXP

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef SOC_SERDES_H
#define SOC_SERDES_H

typedef enum {
  NONE = 0,
  PCIE1,
  PCIE2,
  PCIE3,
  PCIE4,
  PCIE5,
  PCIE6,
  SATA1,
  SATA2,
  SATA3,
  SATA4,
  XFI1,
  XFI2,
  XFI3,
  XFI4,
  XFI5,
  XFI6,
  XFI7,
  XFI8,
  XFI9,
  XFI10,
  XFI11,
  XFI12,
  XFI13,
  XFI14,
  SGMII1,
  SGMII2,
  SGMII3,
  SGMII4,
  SGMII5,
  SGMII6,
  SGMII7,
  SGMII8,
  SGMII9,
  SGMII10,
  SGMII11,
  SGMII12,
  SGMII13,
  SGMII14,
  SGMII15,
  SGMII16,
  SGMII17,
  SGMII18,
  GE100_1,
  GE100_2,
  GE50_1,
  GE50_2,
  GE40_1,
  GE40_2,
  GE25_1,
  GE25_2,
  GE25_3,
  GE25_4,
  GE25_5,
  GE25_6,
  GE25_7,
  GE25_8,
  GE25_9,
  GE25_10,
  SERDES_PROTOCOL_COUNT
} SERDES_PROTOCOL;
#endif
