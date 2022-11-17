/** @file

  Copyright (c) 2011 - 2019, Intel Corporaton. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

  The original software modules are licensed as follows:

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.
  Copyright (c) 2011 - 2014, ARM Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#ifndef EMAC_DXE_UTIL_H__
#define EMAC_DXE_UTIL_H__

#include <Protocol/SimpleNetwork.h>

// Most common CRC32 Polynomial for little endian machines
#define CRC_POLYNOMIAL                                            0xEDB88320
#define HASH_TABLE_REG(n)                                         0x500 + (0x4 * n)
#define RX_MAX_PACKET                                             1600

#define CONFIG_ETH_BUFSIZE                                         2048
#define CONFIG_TX_DESCR_NUM                                        10
#define CONFIG_RX_DESCR_NUM                                        10
#define TX_TOTAL_BUFSIZE                                           (CONFIG_ETH_BUFSIZE * CONFIG_TX_DESCR_NUM)
#define RX_TOTAL_BUFSIZE                                           (CONFIG_ETH_BUFSIZE * CONFIG_RX_DESCR_NUM)

// DMA status error bit
#define RX_DMA_WRITE_DATA_TRANSFER_ERROR                           0x0
#define TX_DMA_READ_DATA_TRANSFER_ERROR                            0x3
#define RX_DMA_DESCRIPTOR_WRITE_ACCESS_ERROR                       0x4
#define TX_DMA_DESCRIPTOR_WRITE_ACCESS_ERROR                       0x5
#define RX_DMA_DESCRIPTOR_READ_ACCESS_ERROR                        0x6
#define TX_DMA_DESCRIPTOR_READ_ACCESS_ERROR                        0x7

// tx descriptor
#define TDES0_OWN                                                  BIT31
#define TDES0_TXINT                                                BIT30
#define TDES0_TXLAST                                               BIT29
#define TDES0_TXFIRST                                              BIT28
#define TDES0_TXCRCDIS                                             BIT27
#define TDES0_TXRINGEND                                            BIT21
#define TDES0_TXCHAIN                                              BIT20

#define TDES1_SIZE1MASK                                            (0x1FFF << 0)
#define TDES1_SIZE1SHFT                                            (0)
#define TDES1_SIZE2MASK                                            (0x1FFF << 16)
#define TDES1_SIZE2SHFT                                            (16)

// rx descriptor
#define RDES0_FL_MASK                                               0x3fff
#define RDES0_FL_SHIFT                                              16
#define RDES1_CHAINED                                               BIT14

#define RDES0_CE                                                    BIT1
#define RDES0_DBE                                                   BIT2
#define RDES0_RE                                                    BIT3
#define RDES0_RWT                                                   BIT4
#define RDES0_LC                                                    BIT6
#define RDES0_GF                                                    BIT7
#define RDES0_OE                                                    BIT11
#define RDES0_LE                                                    BIT12
#define RDES0_SAF                                                   BIT13
#define RDES0_DE                                                    BIT14
#define RDES0_ES                                                    BIT15
#define RDES0_AFM                                                   BIT30
#define RDES0_OWN                                                   BIT31


// emac config phy interface setting
#define PHY_INTERFACE_MODE_GMII                                     0
#define PHY_INTERFACE_MODE_MII                                      1
#define PHY_INTERFACE_MODE_RGMII                                    2
#define PHY_INTERFACE_MODE_RMII                                     3

// DW emac mask
#define DW_EMAC_DMAGRP_BUS_MODE_SWR_SET_MSK                         0x00000001
#define DW_EMAC_DMAGRP_BUS_MODE_FB_SET_MSK                          0x00010000
#define DW_EMAC_DMAGRP_BUS_MODE_PBL_SET_MSK                         0x00003f00
#define DW_EMAC_DMAGRP_BUS_MODE_PR_SET_MSK                          0x0000c000
#define DW_EMAC_DMAGRP_OPERATION_MODE_FTF_SET_MSK                   0x00100000
#define DW_EMAC_DMAGRP_OPERATION_MODE_TSF_SET_MSK                   0x00200000
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_TIE_SET_MSK                 0x00000001
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_RIE_SET_MSK                 0x00000040
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_NIE_SET_MSK                 0x00010000
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_AIE_SET_MSK                 0x00008000
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_FBE_SET_MSK                 0x00002000
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_UNE_SET_MSK                 0x00000020
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_TSE_SET_MSK                 0x00000002
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_TUE_SET_MSK                 0x00000004
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_TJE_SET_MSK                 0x00000008
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_OVE_SET_MSK                 0x00000010
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_RUE_SET_MSK                 0x00000080
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_RSE_SET_MSK                 0x00000100
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_RWE_SET_MSK                 0x00000200
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_ETE_SET_MSK                 0x00000400
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_ERE_SET_MSK                 0x00004000
#define DW_EMAC_DMAGRP_OPERATION_MODE_ST_SET_MSK                    0x00002000
#define DW_EMAC_DMAGRP_OPERATION_MODE_SR_SET_MSK                    0x00000002
#define DW_EMAC_GMACGRP_MAC_CONFIGURATION_RE_SET_MSK                0x00000004
#define DW_EMAC_GMACGRP_MAC_CONFIGURATION_TE_SET_MSK                0x00000008
#define DW_EMAC_GMACGRP_MAC_FRAME_FILTER_RESET                      0x00000000
#define DW_EMAC_GMACGRP_MAC_FRAME_FILTER_HMC_SET_MSK                0x00000004
#define DW_EMAC_GMACGRP_MAC_FRAME_FILTER_DBF_SET_MSK                0x00000020
#define DW_EMAC_GMACGRP_MAC_FRAME_FILTER_PR_SET_MSK                 0x00000001
#define DW_EMAC_GMACGRP_MAC_FRAME_FILTER_PM_SET_MSK                 0x00000010
#define DW_EMAC_DMAGRP_OPERATION_MODE_ST_CLR_MSK                    0xffffdfff
#define DW_EMAC_GMACGRP_MAC_CONFIGURATION_RE_CLR_MSK                0xfffffffb
#define DW_EMAC_GMACGRP_MAC_CONFIGURATION_TE_CLR_MSK                0xfffffff7
#define DW_EMAC_DMAGRP_OPERATION_MODE_SR_CLR_MSK                    0xfffffffd
#define DW_EMAC_DMAGRP_STATUS_NIS_SET_MSK                           0x00010000
#define DW_EMAC_DMAGRP_STATUS_RI_SET_MSK                            0x00000040
#define DW_EMAC_DMAGRP_STATUS_TI_SET_MSK                            0x00000001
#define DW_EMAC_DMAGRP_STATUS_TU_SET_MSK                            0x00000004
#define DW_EMAC_DMAGRP_STATUS_ERI_SET_MSK                           0x00004000
#define DW_EMAC_DMAGRP_STATUS_AIS_SET_MSK                           0x00008000
#define DW_EMAC_DMAGRP_STATUS_TPS_SET_MSK                           0x00000002
#define DW_EMAC_DMAGRP_STATUS_TJT_SET_MSK                           0x00000008
#define DW_EMAC_DMAGRP_STATUS_OVF_SET_MSK                           0x00000010
#define DW_EMAC_DMAGRP_STATUS_UNF_SET_MSK                           0x00000020
#define DW_EMAC_DMAGRP_STATUS_RU_SET_MSK                            0x00000080
#define DW_EMAC_DMAGRP_STATUS_RPS_SET_MSK                           0x00000100
#define DW_EMAC_DMAGRP_STATUS_RWT_SET_MSK                           0x00000200
#define DW_EMAC_DMAGRP_STATUS_ETI_SET_MSK                           0x00000400
#define DW_EMAC_DMAGRP_STATUS_FBI_SET_MSK                           0x00002000
#define DW_EMAC_GMACGRP_MAC_CONFIGURATION_PS_SET_MSK                0x00008000
#define DW_EMAC_GMACGRP_MAC_CONFIGURATION_FES_SET_MSK               0x00004000
#define DW_EMAC_GMACGRP_MAC_CONFIGURATION_DM_SET_MSK                0x00000800
#define DW_EMAC_GMACGRP_MAC_CONFIGURATION_BE_SET_MSK                0x00200000
#define DW_EMAC_GMACGRP_MAC_CONFIGURATION_DO_SET_MSK                0x00002000

#define DW_EMAC_DMAGRP_BUS_MODE_SWR_GET(value)                      (((value) & 0x00000001) >> 0)
#define DW_EMAC_DMAGRP_STATUS_EB_GET(value)                         (((value) & 0x03800000) >> 23)
#define DW_EMAC_GMACGRP_GMII_ADDRESS_GB_GET(value)                  (((value) & 0x00000001) >> 0)
#define DW_EMAC_GMACGRP_GMII_DATA_GD_GET(value)                     (((value) & 0x0000ffff) >> 0)
#define DW_EMAC_DMAGRP_OPERATION_MODE_FTF_GET(value)                (((value) & 0x00100000) >> 20)

// DW emac registers offset

#define DW_EMAC_GMACGRP_MAC_CONFIGURATION_OFST                       0x000
#define DW_EMAC_GMACGRP_MAC_FRAME_FILTER_OFST                        0x004
#define DW_EMAC_GMACGRP_GMII_ADDRESS_OFST                            0x010
#define DW_EMAC_GMACGRP_GMII_DATA_OFST                               0x014
#define DW_EMAC_GMACGRP_FLOW_CONTROL_OFST                            0x018
#define DW_EMAC_GMACGRP_VLAN_TAG_OFST                                0x01c
#define DW_EMAC_GMACGRP_VERSION_OFST                                 0x020
#define DW_EMAC_GMACGRP_DEBUG_OFST                                   0x024
#define DW_EMAC_GMACGRP_LPI_CONTROL_STATUS_OFST                      0x030
#define DW_EMAC_GMACGRP_LPI_TIMERS_CONTROL_OFST                      0x034
#define DW_EMAC_GMACGRP_INTERRUPT_STATUS_OFST                        0x038
#define DW_EMAC_GMACGRP_INTERRUPT_MASK_OFST                          0x03c
#define DW_EMAC_GMACGRP_MAC_ADDRESS0_HIGH_OFST                       0x040
#define DW_EMAC_GMACGRP_MAC_ADDRESS0_LOW_OFST                        0x044
#define DW_EMAC_GMACGRP_MAC_ADDRESS1_HIGH_OFST                       0x048
#define DW_EMAC_GMACGRP_MAC_ADDRESS1_LOW_OFST                        0x04c
#define DW_EMAC_GMACGRP_MAC_ADDRESS2_HIGH_OFST                       0x050
#define DW_EMAC_GMACGRP_MAC_ADDRESS2_LOW_OFST                        0x054
#define DW_EMAC_GMACGRP_MAC_ADDRESS3_HIGH_OFST                       0x058
#define DW_EMAC_GMACGRP_MAC_ADDRESS3_LOW_OFST                        0x05c
#define DW_EMAC_GMACGRP_MAC_ADDRESS4_HIGH_OFST                       0x060
#define DW_EMAC_GMACGRP_MAC_ADDRESS4_LOW_OFST                        0x064
#define DW_EMAC_GMACGRP_MAC_ADDRESS5_HIGH_OFST                       0x068
#define DW_EMAC_GMACGRP_MAC_ADDRESS5_LOW_OFST                        0x06c
#define DW_EMAC_GMACGRP_MAC_ADDRESS6_HIGH_OFST                       0x070
#define DW_EMAC_GMACGRP_MAC_ADDRESS6_LOW_OFST                        0x074
#define DW_EMAC_GMACGRP_MAC_ADDRESS7_HIGH_OFST                       0x078
#define DW_EMAC_GMACGRP_MAC_ADDRESS7_LOW_OFST                        0x07c
#define DW_EMAC_GMACGRP_MAC_ADDRESS8_HIGH_OFST                       0x080
#define DW_EMAC_GMACGRP_MAC_ADDRESS8_LOW_OFST                        0x084
#define DW_EMAC_GMACGRP_MAC_ADDRESS9_HIGH_OFST                       0x088
#define DW_EMAC_GMACGRP_MAC_ADDRESS9_LOW_OFST                        0x08c
#define DW_EMAC_GMACGRP_MAC_ADDRESS10_HIGH_OFST                      0x090
#define DW_EMAC_GMACGRP_MAC_ADDRESS10_LOW_OFST                       0x094
#define DW_EMAC_GMACGRP_MAC_ADDRESS11_HIGH_OFST                      0x098
#define DW_EMAC_GMACGRP_MAC_ADDRESS11_LOW_OFST                       0x09c
#define DW_EMAC_GMACGRP_MAC_ADDRESS12_HIGH_OFST                      0x0a0
#define DW_EMAC_GMACGRP_MAC_ADDRESS12_LOW_OFST                       0x0a4
#define DW_EMAC_GMACGRP_MAC_ADDRESS13_HIGH_OFST                      0x0a8
#define DW_EMAC_GMACGRP_MAC_ADDRESS13_LOW_OFST                       0x0ac
#define DW_EMAC_GMACGRP_MAC_ADDRESS14_HIGH_OFST                      0x0b0
#define DW_EMAC_GMACGRP_MAC_ADDRESS14_LOW_OFST                       0x0b4
#define DW_EMAC_GMACGRP_MAC_ADDRESS15_HIGH_OFST                      0x0b8
#define DW_EMAC_GMACGRP_MAC_ADDRESS15_LOW_OFST                       0x0bc
#define DW_EMAC_GMACGRP_SGMII_RGMII_SMII_CONTROL_STATUS_OFST         0x0d8
#define DW_EMAC_GMACGRP_WDOG_TIMEOUT_OFST                            0x0dc
#define DW_EMAC_GMACGRP_GENPIO_OFST                                  0x0e0
#define DW_EMAC_GMACGRP_MMC_CONTROL_OFST                             0x100
#define DW_EMAC_GMACGRP_MMC_RECEIVE_INTERRUPT_OFST                   0x104
#define DW_EMAC_GMACGRP_MMC_TRANSMIT_INTERRUPT_OFST                  0x108
#define DW_EMAC_GMACGRP_MMC_RECEIVE_INTERRUPT_MASK_OFST              0x10c
#define DW_EMAC_GMACGRP_MMC_TRANSMIT_INTERRUPT_MASK_OFST             0x110
#define DW_EMAC_GMACGRP_TXOCTETCOUNT_GB_OFST                         0x114
#define DW_EMAC_GMACGRP_TXFRAMECOUNT_GB_OFST                         0x118
#define DW_EMAC_GMACGRP_TXBROADCASTFRAMES_G_OFST                     0x11c
#define DW_EMAC_GMACGRP_TXMULTICASTFRAMES_G_OFST                     0x120
#define DW_EMAC_GMACGRP_TXUNICASTFRAMES_GB_OFST                      0x13c
#define DW_EMAC_GMACGRP_TXLATECOL_OFST                               0x158
#define DW_EMAC_GMACGRP_TXEXESSCOL_OFST                              0x15c
#define DW_EMAC_GMACGRP_TXFRAMECOUNT_G_OFST                          0x168
#define DW_EMAC_GMACGRP_TXOVERSIZE_G_OFST                            0x178
#define DW_EMAC_GMACGRP_RXFRAMECOUNT_GB_OFST                         0x180
#define DW_EMAC_GMACGRP_RXOCTETCOUNT_GB_OFST                         0x184
#define DW_EMAC_GMACGRP_RXBROADCASTFRAMES_G_OFST                     0x18c
#define DW_EMAC_GMACGRP_RXMULTICASTFRAMES_G_OFST                     0x190
#define DW_EMAC_GMACGRP_RXCRCERROR_OFST                              0x194
#define DW_EMAC_GMACGRP_RXUNDERSIZE_G_OFST                           0x1a4
#define DW_EMAC_GMACGRP_RXOVERSIZE_G_OFST                            0x1a8
#define DW_EMAC_GMACGRP_RXUNICASTFRAMES_G_OFST                       0x1c4
#define DW_EMAC_DMAGRP_BUS_MODE_OFST                                 0x1000
#define DW_EMAC_DMAGRP_TRANSMIT_POLL_DEMAND_OFST                     0x1004
#define DW_EMAC_DMAGRP_RECEIVE_POLL_DEMAND_OFST                      0x1008
#define DW_EMAC_DMAGRP_RECEIVE_DESCRIPTOR_LIST_ADDRESS_OFST          0x100c
#define DW_EMAC_DMAGRP_TRANSMIT_DESCRIPTOR_LIST_ADDRESS_OFST         0x1010
#define DW_EMAC_DMAGRP_STATUS_OFST                                   0x1014
#define DW_EMAC_DMAGRP_OPERATION_MODE_OFST                           0x1018
#define DW_EMAC_DMAGRP_INTERRUPT_ENABLE_OFST                         0x101c
#define DW_EMAC_DMAGRP_MISSED_FRAME_AND_BUFFER_OVERFLOW_COUNTER_OFST 0x1020
#define DW_EMAC_DMAGRP_RECEIVE_INTERRUPT_WATCHDOG_TIMER_OFST         0x1024
#define DW_EMAC_DMAGRP_AXI_BUS_MODE_OFST                             0x1028
#define DW_EMAC_DMAGRP_AHB_OR_AXI_STATUS_OFST                        0x102c
#define DW_EMAC_DMAGRP_CURRENT_HOST_TRANSMIT_DESCRIPTOR_OFST         0x1048
#define DW_EMAC_DMAGRP_CURRENT_HOST_RECEIVE_DESCRIPTOR_OFST          0x104c
#define DW_EMAC_DMAGRP_CURRENT_HOST_TRANSMIT_BUFFER_ADDRESS_OFST     0x1050
#define DW_EMAC_DMAGRP_CURRENT_HOST_RECEIVE_BUFFER_ADDRESS_OFST      0x1054
#define DW_EMAC_DMAGRP_HW_FEATURE_OFST                               0x1058

typedef struct {
  UINT32 Tdes0;
  UINT32 Tdes1;
  UINT32 Addr;
  UINT32 AddrNext;
} DESIGNWARE_HW_DESCRIPTOR;

typedef struct {
  EFI_PHYSICAL_ADDRESS        AddrMap;
  void                        *Mapping;
} MAP_INFO;

typedef struct {
  DESIGNWARE_HW_DESCRIPTOR    *TxdescRing[CONFIG_TX_DESCR_NUM];
  DESIGNWARE_HW_DESCRIPTOR    *RxdescRing[CONFIG_RX_DESCR_NUM];
  CHAR8                       TxBuffer[TX_TOTAL_BUFSIZE];
  CHAR8                       RxBuffer[RX_TOTAL_BUFSIZE];
  MAP_INFO                    TxdescRingMap[CONFIG_TX_DESCR_NUM ];
  MAP_INFO                    RxdescRingMap[CONFIG_RX_DESCR_NUM ];
  MAP_INFO                    RxBufNum[CONFIG_TX_DESCR_NUM];
  UINT32                      TxCurrentDescriptorNum;
  UINT32                      TxNextDescriptorNum;
  UINT32                      RxCurrentDescriptorNum;
  UINT32                      RxNextDescriptorNum;
} EMAC_DRIVER;

VOID
EFIAPI
EmacSetMacAddress (
  IN  EFI_MAC_ADDRESS         *MacAddress,
  IN  UINTN                   MacBaseAddress
  );

VOID
EFIAPI
EmacReadMacAddress (
  OUT EFI_MAC_ADDRESS         *MacAddress,
  IN  UINTN                   MacBaseAddress
  );

EFI_STATUS
EFIAPI
EmacDxeInitialization (
  IN  EMAC_DRIVER             *EmacDriver,
  IN  UINTN                   MacBaseAddress
  );

EFI_STATUS
EFIAPI
EmacDmaInit (
  IN  EMAC_DRIVER             *EmacDriver,
  IN  UINTN                   MacBaseAddress
  );

EFI_STATUS
EFIAPI
EmacSetupTxdesc (
  IN  EMAC_DRIVER             *EmacDriver,
  IN  UINTN                   MacBaseAddress
 );

EFI_STATUS
EFIAPI
EmacSetupRxdesc (
  IN  EMAC_DRIVER             *EmacDriver,
  IN  UINTN                   MacBaseAddress
  );

VOID
EFIAPI
EmacStartTransmission (
  IN  UINTN                   MacBaseAddress
  );

EFI_STATUS
EFIAPI
EmacRxFilters (
  IN  UINT32                  ReceiveFilterSetting,
  IN  BOOLEAN                 Reset,
  IN  UINTN                   NumMfilter             OPTIONAL,
  IN  EFI_MAC_ADDRESS         *Mfilter               OPTIONAL,
  IN  UINTN                   MacBaseAddress
  );

UINT32
EFIAPI
GenEtherCrc32 (
  IN  EFI_MAC_ADDRESS         *Mac,
  IN  UINT32 AddrLen
  );

UINT8
EFIAPI
BitReverse (
  UINT8                       Value
  );

VOID
EFIAPI
EmacStopTxRx (
  IN  UINTN                   MacBaseAddress
  );

EFI_STATUS
EFIAPI
EmacDmaStart (
  IN  UINTN                   MacBaseAddress
  );


VOID
EFIAPI
EmacGetDmaStatus (
  OUT UINT32                  *IrqStat  OPTIONAL,
  IN  UINTN                   MacBaseAddress
  );

VOID
EFIAPI
EmacGetStatistic (
  IN  EFI_NETWORK_STATISTICS *Stats,
  IN  UINTN                  MacBaseAddress
  );

VOID
EFIAPI
EmacConfigAdjust (
  IN  UINT32                 Speed,
  IN  UINT32                 Duplex,
  IN  UINTN                  MacBaseAddress
  );

#endif // EMAC_DXE_UTIL_H__
