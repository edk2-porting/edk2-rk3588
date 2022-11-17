## @file
#  Component description file for the CN9132 Development Board (variant A)
#
#  Copyright (c) 2019 Marvell International Ltd.<BR>
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFixedAtBuild.common]
  # CP115 count
  gMarvellTokenSpaceGuid.PcdMaxCpCount|3

  # MPP
  gMarvellTokenSpaceGuid.PcdMppChipCount|4

  # CP115 #2 MPP
  gMarvellTokenSpaceGuid.PcdChip3MppReverseFlag|FALSE
  gMarvellTokenSpaceGuid.PcdChip3MppBaseAddress|0xF6440000
  gMarvellTokenSpaceGuid.PcdChip3MppPinCount|64
  gMarvellTokenSpaceGuid.PcdChip3MppSel0|{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  gMarvellTokenSpaceGuid.PcdChip3MppSel1|{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  gMarvellTokenSpaceGuid.PcdChip3MppSel2|{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x9, 0x9, 0x0 }
  gMarvellTokenSpaceGuid.PcdChip3MppSel3|{ 0x0, 0x0, 0x8, 0x0, 0x8, 0x0, 0x0, 0x2, 0x2, 0x0 }
  gMarvellTokenSpaceGuid.PcdChip3MppSel4|{ 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }
  gMarvellTokenSpaceGuid.PcdChip3MppSel5|{ 0x0, 0x0, 0x0, 0x0, 0xA, 0xB, 0xE, 0xE, 0xE, 0xE }
  gMarvellTokenSpaceGuid.PcdChip3MppSel6|{ 0xE, 0xE, 0xE, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }

  # ComPhy
  gMarvellTokenSpaceGuid.PcdComPhyDevices|{ 0x1, 0x1, 0x1 }
  # ComPhy1
  # 0: PCIE0         5 Gbps
  # 1: PCIE0         5 Gbps
  # 2: SATA0         5 Gbps
  # 3: USB3_HOST1    5 Gbps
  # 4: SFI           10.31 Gbps
  # 5: PCIE2         5 Gbps
  gMarvellTokenSpaceGuid.PcdChip2ComPhyTypes|{ $(CP_PCIE0), $(CP_PCIE0), $(CP_SATA0), $(CP_USB3_HOST1), $(CP_SFI), $(CP_PCIE2)}
  gMarvellTokenSpaceGuid.PcdChip2ComPhySpeeds|{ $(CP_5G), $(CP_5G), $(CP_5G), $(CP_5G), $(CP_10_3125G), $(CP_5G) }

  # UtmiPhy
  gMarvellTokenSpaceGuid.PcdUtmiControllersEnabled|{ 0x1, 0x1, 0x0, 0x1, 0x1, 0x1 }
  gMarvellTokenSpaceGuid.PcdUtmiPortType|{ $(UTMI_USB_HOST0), $(UTMI_USB_HOST1), $(UTMI_USB_HOST0), $(UTMI_USB_HOST1), $(UTMI_USB_HOST0), $(UTMI_USB_HOST1) }

  # MDIO
  gMarvellTokenSpaceGuid.PcdMdioControllersEnabled|{ 0x1, 0x0 }

  # PHY
  gMarvellTokenSpaceGuid.PcdPhy2MdioController|{ 0x0, 0x0 }
  gMarvellTokenSpaceGuid.PcdPhyDeviceIds|{ 0x0, 0x0 }
  gMarvellTokenSpaceGuid.PcdPhySmiAddresses|{ 0x0, 0x1 }
  gMarvellTokenSpaceGuid.PcdPhyStartupAutoneg|FALSE

  # NET
  gMarvellTokenSpaceGuid.PcdPp2GopIndexes|{ 0x0, 0x2, 0x3, 0x0, 0x0 }
  gMarvellTokenSpaceGuid.PcdPp2InterfaceAlwaysUp|{ 0x0, 0x0, 0x0, 0x0, 0x0 }
  gMarvellTokenSpaceGuid.PcdPp2InterfaceSpeed|{ $(PHY_SPEED_10000), $(PHY_SPEED_1000), $(PHY_SPEED_1000), $(PHY_SPEED_10000), $(PHY_SPEED_10000) }
  gMarvellTokenSpaceGuid.PcdPp2PhyConnectionTypes|{ $(PHY_SFI), $(PHY_RGMII), $(PHY_RGMII), $(PHY_SFI), $(PHY_SFI) }
  gMarvellTokenSpaceGuid.PcdPp2PhyIndexes|{ 0xFF, 0x0, 0x1, 0xFF, 0xFF }
  gMarvellTokenSpaceGuid.PcdPp2Port2Controller|{ 0x0, 0x0, 0x0, 0x1, 0x2 }
  gMarvellTokenSpaceGuid.PcdPp2PortIds|{ 0x0, 0x1, 0x2, 0x0, 0x0 }
  gMarvellTokenSpaceGuid.PcdPp2Controllers|{ 0x1, 0x1, 0x1 }

  # NonDiscoverableDevices
  gMarvellTokenSpaceGuid.PcdPciEXhci|{ 0x1, 0x1, 0x0, 0x1, 0x1, 0x1 }
  gMarvellTokenSpaceGuid.PcdPciEAhci|{ 0x1, 0x1, 0x1 }
  gMarvellTokenSpaceGuid.PcdPciESdhci|{ 0x1, 0x1, 0x0, 0x1 }
