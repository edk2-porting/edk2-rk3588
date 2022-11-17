/** @file
  PCI Host Bridge Library instance for Marvell 70x0/80x0

  Copyright (c) 2017, Linaro Ltd. All rights reserved.<BR>
  Copyright (c) 2019 Marvell International Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#ifndef __PCI_HOST_BRIDGE_LIB_CONSTRUCTOR_H__
#define __PCI_HOST_BRIDGE_LIB_CONSTRUCTOR_H__

#define IATU_VIEWPORT_OFF                                   0x900
#define IATU_VIEWPORT_INBOUND                               BIT31
#define IATU_VIEWPORT_OUTBOUND                              0
#define IATU_VIEWPORT_REGION_INDEX(Idx)                     ((Idx) & 7)

#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0                   0x904
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_MEM          0x0
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_IO           0x2
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG0         0x4
#define IATU_REGION_CTRL_1_OFF_OUTBOUND_0_TYPE_CFG1         0x5

#define IATU_REGION_CTRL_2_OFF_OUTBOUND_0                   0x908
#define IATU_REGION_CTRL_2_OFF_OUTBOUND_0_REGION_EN         BIT31
#define IATU_REGION_CTRL_2_OFF_OUTBOUND_0_CFG_SHIFT_MODE    BIT28

#define IATU_LWR_BASE_ADDR_OFF_OUTBOUND_0                   0x90C
#define IATU_UPPER_BASE_ADDR_OFF_OUTBOUND_0                 0x910
#define IATU_LIMIT_ADDR_OFF_OUTBOUND_0                      0x914
#define IATU_LWR_TARGET_ADDR_OFF_OUTBOUND_0                 0x918
#define IATU_UPPER_TARGET_ADDR_OFF_OUTBOUND_0               0x91C

#define PORT_LINK_CTRL_OFF                                  0x710
#define PORT_LINK_CTRL_OFF_LINK_CAPABLE_x1                  (0x01 << 16)
#define PORT_LINK_CTRL_OFF_LINK_CAPABLE_x2                  (0x03 << 16)
#define PORT_LINK_CTRL_OFF_LINK_CAPABLE_x4                  (0x07 << 16)
#define PORT_LINK_CTRL_OFF_LINK_CAPABLE_x8                  (0x0f << 16)
#define PORT_LINK_CTRL_OFF_LINK_CAPABLE_x16                 (0x1f << 16)
#define PORT_LINK_CTRL_OFF_LINK_CAPABLE_MASK                (0x3f << 16)

#define GEN2_CTRL_OFF                                       0x80c
#define GEN2_CTRL_OFF_NUM_OF_LANES(n)                       (((n) & 0x1f) << 8)
#define GEN2_CTRL_OFF_NUM_OF_LANES_MASK                     (0x1f << 8)
#define GEN2_CTRL_OFF_DIRECT_SPEED_CHANGE                   BIT17

#define PCIE_GLOBAL_CTRL_OFFSET                             0x8000
#define PCIE_GLOBAL_APP_LTSSM_EN                            BIT2
#define PCIE_GLOBAL_CTRL_DEVICE_TYPE_RC                     (0x4 << 4)
#define PCIE_GLOBAL_CTRL_DEVICE_TYPE_MASK                   (0xF << 4)

#define PCIE_GLOBAL_STATUS_REG                              0x8008
#define PCIE_GLOBAL_STATUS_RDLH_LINK_UP                     BIT1
#define PCIE_GLOBAL_STATUS_PHY_LINK_UP                      BIT9

#define PCIE_PM_STATUS                                      0x8014
#define PCIE_PM_LTSSM_STAT_MASK                             (0x3f << 3)

#define PCIE_GLOBAL_INT_MASK1_REG                           0x8020
#define PCIE_INT_A_ASSERT_MASK                              BIT9
#define PCIE_INT_B_ASSERT_MASK                              BIT10
#define PCIE_INT_C_ASSERT_MASK                              BIT11
#define PCIE_INT_D_ASSERT_MASK                              BIT12

#define PCIE_ARCACHE_TRC_REG                                0x8050
#define PCIE_AWCACHE_TRC_REG                                0x8054
#define PCIE_ARUSER_REG                                     0x805C
#define PCIE_AWUSER_REG                                     0x8060

#define ARCACHE_DEFAULT_VALUE                               0x3511
#define AWCACHE_DEFAULT_VALUE                               0x5311

#define AX_USER_DOMAIN_INNER_SHAREABLE                      (0x1 << 4)
#define AX_USER_DOMAIN_OUTER_SHAREABLE                      (0x2 << 4)
#define AX_USER_DOMAIN_MASK                                 (0x3 << 4)

#define PCIE_LINK_CAPABILITY                                0x7C
#define PCIE_LINK_CTL_2                                     0xA0
#define TARGET_LINK_SPEED_MASK                              0xF
#define LINK_SPEED_GEN_1                                    0x1
#define LINK_SPEED_GEN_2                                    0x2
#define LINK_SPEED_GEN_3                                    0x3

#define PCIE_GEN3_EQU_CTRL                                  0x8A8
#define GEN3_EQU_EVAL_2MS_DISABLE                           BIT5

#define PCIE_LINK_UP_TIMEOUT_US                             40000

#endif
