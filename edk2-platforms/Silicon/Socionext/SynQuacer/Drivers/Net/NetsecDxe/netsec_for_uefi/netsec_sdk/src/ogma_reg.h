/** @file

  Copyright (c) 2016 - 2017, Socionext Inc. All rights reserved.<BR>
  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef OGMA_REG_H
#define OGMA_REG_H

#include "ogma_reg_netsec.h"
#include "ogma_reg_f_gmac_4mt.h"

/* aliases */
#define OGMA_REG_ADDR_TOP_INTEN OGMA_REG_ADDR_TOP_INTEN_A
#define OGMA_REG_ADDR_TOP_INTEN_SET OGMA_REG_ADDR_TOP_INTEN_A_SET
#define OGMA_REG_ADDR_TOP_INTEN_CLR OGMA_REG_ADDR_TOP_INTEN_A_CLR
/* aliases end here */

/* need fixing */
#define OGMA_REG_ADDR_DMAC_MH_CMD_BUF            (0x87)


/*bit fields for PKT_CTRL*/
#define OGMA_PKT_CTRL_REG_EN_JUMBO          (1UL << 27)
#define OGMA_PKT_CTRL_REG_EN_BITPATF        (1UL << 4 )
#define OGMA_PKT_CTRL_REG_LOG_CHKSUM_ER     (1UL << 3 )
#define OGMA_PKT_CTRL_REG_LOG_HD_INCOMPLETE (1UL << 2 )
#define OGMA_PKT_CTRL_REG_LOG_HD_ER         (1UL << 1 )
#define OGMA_PKT_CTRL_REG_DRP_NO_MATCH      (1UL << 0 )

#define OGMA_CLK_EN_REG_DOM_G (1UL << 5)

/************************************************************
 * Bit fields
 ************************************************************/
/* bit fields for com_init */
#define OGMA_COM_INIT_REG_DB   (1UL << 2)
#define OGMA_COM_INIT_REG_CLS  (1UL << 1)
#define OGMA_COM_INIT_REG_ALL  ( OGMA_COM_INIT_REG_CLS | OGMA_COM_INIT_REG_DB)

/* bit fields for soft_rst */
#define OGMA_SOFT_RST_REG_RESET (0)
#define OGMA_SOFT_RST_REG_RUN   (1UL << 31)

/* bit fields for dma_hm_ctrl */
#define OGMA_DMA_CTRL_REG_STOP 1UL

/* bit fields for gmac_cmd */
#define OGMA_GMAC_CMD_ST_READ  (0)
#define OGMA_GMAC_CMD_ST_WRITE (1UL << 28)
#define OGMA_GMAC_CMD_ST_BUSY  (1UL << 31)

/* bit fields for F_GMAC4MT BMR*/
#define OGMA_GMAC_BMR_REG_COMMON (0x00412080)
#define OGMA_GMAC_BMR_REG_RESET  (0x00020181)
#define OGMA_GMAC_BMR_REG_SWR    (0x00000001)

/* bit fields for F_GMAC4MT OMR*/
#define OGMA_GMAC_OMR_REG_ST (1UL << 13)
#define OGMA_GMAC_OMR_REG_SR (1UL << 1)

/* bit fields for F_GMAC4MT MCR*/
#define OGMA_GMAC_MCR_REG_CST                (1UL << 25)
#define OGMA_GMAC_MCR_REG_JE                 (1UL << 20)
#define OGMA_GMAC_MCR_REG_PS                 (1UL << 15)
#define OGMA_GMAC_MCR_REG_FES                (1UL << 14)
#define OGMA_GMAC_MCR_REG_FULL_DUPLEX_COMMON (0x0000280c)
#define OGMA_GMAC_MCR_REG_HALF_DUPLEX_COMMON (0x0001a00c)

#define OGMA_GMAC_MCR_1G_FULL ( OGMA_GMAC_MCR_REG_CST | \
                                OGMA_GMAC_MCR_REG_FULL_DUPLEX_COMMON)

#define OGMA_GMAC_MCR_100M_FULL ( OGMA_GMAC_MCR_REG_CST | \
                                  OGMA_GMAC_MCR_REG_PS  | \
                                  OGMA_GMAC_MCR_REG_FES | \
                                  OGMA_GMAC_MCR_REG_FULL_DUPLEX_COMMON)

#define OGMA_GMAC_MCR_100M_HALF ( OGMA_GMAC_MCR_REG_CST | \
                                  OGMA_GMAC_MCR_REG_FES | \
                                  OGMA_GMAC_MCR_REG_HALF_DUPLEX_COMMON)

#define OGMA_GMAC_MCR_10M_FULL ( OGMA_GMAC_MCR_REG_CST | \
                                 OGMA_GMAC_MCR_REG_PS  | \
                                 OGMA_GMAC_MCR_REG_FULL_DUPLEX_COMMON)

#define OGMA_GMAC_MCR_10M_HALF ( OGMA_GMAC_MCR_REG_CST | \
                                 OGMA_GMAC_MCR_REG_HALF_DUPLEX_COMMON)

/*bit fields for F_GMAC4MT FCR*/
#define OGMA_GMAC_FCR_REG_RFE (1UL << 2)
#define OGMA_GMAC_FCR_REG_TFE (1UL << 1)

/* bit fields for F_GMAC4MT GAR */
#define OGMA_GMAC_GAR_REG_GW (1UL << 1)
#define OGMA_GMAC_GAR_REG_GB (1UL << 0)

/* bit fields for F_GMAC4MT RDLAR*/
#define OGMA_GMAC_RDLAR_REG_COMMON (0x00018000UL)

/* bit fields for F_GMAC4MT TDLAR*/
#define OGMA_GMAC_TDLAR_REG_COMMON (0x0001c000UL)


#define OGMA_GMAC_GAR_REG_SHIFT_PA (11)
#define OGMA_GMAC_GAR_REG_SHIFT_GR (6)
#define OGMA_GMAC_GAR_REG_SHIFT_CR (2)

#define OGMA_GMAC_GAR_REG_CR_25_35_MHZ   (2)
#define OGMA_GMAC_GAR_REG_CR_35_60_MHZ   (3)
#define OGMA_GMAC_GAR_REG_CR_60_100_MHZ  (0)
#define OGMA_GMAC_GAR_REG_CR_100_150_MHZ (1)
#define OGMA_GMAC_GAR_REG_CR_150_250_MHZ (4)
#define OGMA_GMAC_GAR_REG_CR_250_300_MHZ (5)

/* bit fields for F_GMAC4MT LPITCR */
#define OGMA_GMAC_LPITCR_REG_LIT (16)
#define OGMA_GMAC_LPITCR_REG_TWT (0)

/* bit fileds mask for F_GMAC4MT LPITCR */
#define OGMA_GMAC_LPITCR_REG_MASK_LIT (0x3ffU)
#define OGMA_GMAC_LPITCR_REG_MASK_TWT (0xffffU)

/**
 * PHY regtister Address
 */
#define OGMA_PHY_REG_ADDR_CONTROL                      0
#define OGMA_PHY_REG_ADDR_STATUS                       1U
#define OGMA_PHY_REG_ADDR_AUTO_NEGO_ABILTY             4U
#define OGMA_PHY_REG_ADDR_AUTO_NEGO_LINK_PATNER_ABILTY 5U
#define OGMA_PHY_REG_ADDR_MASTER_SLAVE_CONTROL         9U
#define OGMA_PHY_REG_ADDR_MASTER_SLAVE_STATUS          10U
#define OGMA_PHY_REG_ADDR_MMD_AC                       13U
#define OGMA_PHY_REG_ADDR_MMD_AAD                      14U

/* bit fields for PHY CONTROL Register */
#define OGMA_PHY_CONTROL_REG_SPEED_SELECTION_MSB (6)
#define OGMA_PHY_CONTROL_REG_DUPLEX_MODE         (8)
#define OGMA_PHY_CONTROL_REG_ISOLATE             (10)
#define OGMA_PHY_CONTROL_REG_POWER_DOWN          (11)
#define OGMA_PHY_CONTROL_REG_AUTO_NEGO_ENABLE    (12)
#define OGMA_PHY_CONTROL_REG_SPEED_SELECTION_LSB (13)
#define OGMA_PHY_CONTROL_REG_LOOPBACK            (14)
#define OGMA_PHY_CONTROL_REG_RESET               (15)

/* bit fields for PHY STATUS Register */
#define OGMA_PHY_STATUS_REG_LINK_STATUS       (2)
#define OGMA_PHY_STATUS_REG_AUTO_NEGO_ABILITY (3)
#define OGMA_PHY_STATUS_REG_AUTO_NEGO_COMP    (5)

/* bit fields for PHY MASTER-SLAVE Control Register */
#define OGMA_PHY_MSC_REG_1000BASE_FULL (9)
#define OGMA_PHY_MSC_REG_1000BASE_HALF (8)

/* bit fields for PHY MASTER-SLAVE Status Register */
#define OGMA_PHY_MSS_REG_LP_1000BASE_FULL (11)
#define OGMA_PHY_MSS_REG_LP_1000BASE_HALF (10)

/* bit fields for PHY Auto-Negotiation Advertisement Register */
#define OGMA_PHY_ANA_REG_TAF (5)

/* bit fileds mask for F_GMAC4MT LPITCR */
#define OGMA_PHY_ANA_REG_TAF_MASK (0x7fU)

/* bit fields for PHY Technology Ability Field */
#define OGMA_PHY_TAF_REG_100BASE_FULL (1U << 3)
#define OGMA_PHY_TAF_REG_100BASE_HALF (1U << 2)
#define OGMA_PHY_TAF_REG_10BASE_FULL  (1U << 1)
#define OGMA_PHY_TAF_REG_10BASE_HALF  (1U << 0)

/**
 * PHY Device Address
 */
#define OGMA_PHY_DEV_ADDR_AUTO_NEGO (7U)

/**
 * PHY Register Address for Device Address 7
 */
#define OGMA_PHY_AUTO_NEGO_REG_ADDR_EEE_ADVERTISE  (60U)
#define OGMA_PHY_AUTO_NEGO_REG_ADDR_EEE_LP_ABILITY (61U)

/* bit fields for PHY AutoNegotiation Field */
#define OGMA_PHY_AUTO_NEGO_1000BASE_EEE (1U << 2)
#define OGMA_PHY_AUTO_NEGO_100BASE_EEE  (1U << 1)

/* bit fields for DESC RING CONFIG */
#define OGMA_REG_DESC_RING_CONFIG_CFG_UP     (31)
#define OGMA_REG_DESC_RING_CONFIG_CH_RST     (30)
#define OGMA_REG_DESC_RING_CONFIG_TMR_MODE   (4)
#define OGMA_REG_DESC_RING_CONFIG_DAT_ENDIAN (0)


/* bit fields for mac_desc_soft_rst */
#define OGMA_MAC_DESC_SOFT_RST_SOFT_RST 1UL

/* bit fields for mac_desc_init */
#define OGMA_MAC_DESC_INIT_REG_INIT 1UL

/* bit fields for dis_core */
#define OGMA_DIS_CORE_REG_DIS_PCORE  (1UL << 2)
#define OGMA_DIS_CORE_REG_DIS_MHCORE (1UL << 1)
#define OGMA_DIS_CORE_REG_DIS_HMCORE (1UL << 0)

#define OGMA_DIS_CORE_REG_ALL ( OGMA_DIS_CORE_REG_DIS_HMCORE | \
                                OGMA_DIS_CORE_REG_DIS_MHCORE | \
                                OGMA_DIS_CORE_REG_DIS_PCORE)

/* bit fields for dma_info */
#define OGMA_DMA_INFO_REG_AXI_WRITE_BUSY (1UL << 3)
#define OGMA_DMA_INFO_REG_AXI_READ_BUSY  (1UL << 2)

#define OGMA_DMA_INFO_REG_AXI_BUSY ( OGMA_DMA_INFO_REG_AXI_WRITE_BUSY | \
                                     OGMA_DMA_INFO_REG_AXI_READ_BUSY)

#endif /*OGMA_REG_H*/
