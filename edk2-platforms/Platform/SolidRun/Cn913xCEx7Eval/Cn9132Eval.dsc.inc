## @file
#  Component description file for the CN9132 Development Board (variant A)
#
#  Copyright (c) 2019 Marvell International Ltd.<BR>
#  Copyright (c) 2021 Semihalf.<BR>
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
  gMarvellTokenSpaceGuid.PcdChip3MppSel0|{ 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0 }
  gMarvellTokenSpaceGuid.PcdChip3MppSel1|{ 0x0,  0x0,  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
  gMarvellTokenSpaceGuid.PcdChip3MppSel2|{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0,  0x7,  0x7 }
  gMarvellTokenSpaceGuid.PcdChip3MppSel3|{ 0x7,  0x0,  0x0,  0xFF, 0xFF, 0x2,  0x2,  0x8,  0x8,  0xFF }
  gMarvellTokenSpaceGuid.PcdChip3MppSel4|{ 0x0,  0xFF, 0x0,  0x0,  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }
  gMarvellTokenSpaceGuid.PcdChip3MppSel5|{ 0x0,  0xFF, 0xFF, 0xFF, 0xFF, 0x0,  0x0,  0xFF, 0xFF, 0xFF }
  gMarvellTokenSpaceGuid.PcdChip3MppSel6|{ 0xFF, 0xFF, 0xFF, 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x0 }

  # ComPhy
  gMarvellTokenSpaceGuid.PcdComPhyDevices|{ 0x1, 0x1, 0x1 }
  # ComPhy2
  # 0: PCIE0         5 Gbps
  # 1: USB3_HOST0    5 Gbps
  # 2: SFI           5.15625 Gbps
  # 3: SATA1         5 Gbps
  # 4: PCIE1         5 Gbps
  # 5: PCIE2         5 Gbps
  gMarvellTokenSpaceGuid.PcdChip2ComPhyTypes|{ $(CP_PCIE0), $(CP_USB3_HOST0), $(CP_SFI), $(CP_SATA1), $(CP_PCIE1), $(CP_PCIE2)}
  gMarvellTokenSpaceGuid.PcdChip2ComPhySpeeds|{ $(CP_5G), $(CP_5G), $(CP_5_15625G), $(CP_5G), $(CP_5G), $(CP_5G) }

  # UtmiPhy
  gMarvellTokenSpaceGuid.PcdUtmiControllersEnabled|{ 0x1, 0x1, 0x0, 0x1, 0x1, 0x1 }
  gMarvellTokenSpaceGuid.PcdUtmiPortType|{ $(UTMI_USB_HOST0), $(UTMI_USB_HOST1), $(UTMI_USB_HOST0), $(UTMI_USB_HOST1), $(UTMI_USB_HOST0), $(UTMI_USB_HOST1) }

  # NET
  gMarvellTokenSpaceGuid.PcdPp2GopIndexes|{ 0x0, 0x2, 0x3, 0x0, 0x0 }
  gMarvellTokenSpaceGuid.PcdPp2InterfaceAlwaysUp|{ 0x0, 0x0, 0x1, 0x0, 0x0 }
  gMarvellTokenSpaceGuid.PcdPp2InterfaceSpeed|{ $(PHY_SPEED_10000), $(PHY_SPEED_1000), $(PHY_SPEED_2500), $(PHY_SPEED_10000), $(PHY_SPEED_10000) }
  gMarvellTokenSpaceGuid.PcdPp2PhyConnectionTypes|{ $(PHY_SFI), $(PHY_RGMII), $(PHY_SGMII), $(PHY_SFI), $(PHY_SFI) }
  gMarvellTokenSpaceGuid.PcdPp2PhyIndexes|{ 0xFF, 0x0, 0xFF, 0xFF, 0xFF }
  gMarvellTokenSpaceGuid.PcdPp2Port2Controller|{ 0x0, 0x0, 0x0, 0x1, 0x2 }
  gMarvellTokenSpaceGuid.PcdPp2PortIds|{ 0x0, 0x1, 0x2, 0x0, 0x0 }
  gMarvellTokenSpaceGuid.PcdPp2Controllers|{ 0x1, 0x1, 0x1 }

  # NonDiscoverableDevices
  gMarvellTokenSpaceGuid.PcdPciEXhci|{ 0x1, 0x1, 0x0, 0x1, 0x1, 0x1 }
  gMarvellTokenSpaceGuid.PcdPciEAhci|{ 0x0, 0x1, 0x1 }
  gMarvellTokenSpaceGuid.PcdPciESdhci|{ 0x1, 0x1 }
