/** @file
 *
 *  RK3588 GMAC initializer
 *
 *  Copyright (c) 2021-2022, Jared McNeill <jmcneill@invisible.ca>
 *  Copyright (c) 2023, Mario Bălănică <mariobalanica02@gmail.com>
 *
 *  SPDX-License-Identifier: BSD-2-Clause-Patent
 *
 **/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseCryptLib.h>
#include <Library/OtpLib.h>
#include <Library/RockchipPlatformLib.h>
#include <Soc.h>

#include <EthernetPhy.h>

#define CRU_SOFTRST_CON32       (CRU_BASE + 0x0A80)

#define PHP_GRF_BASE            0xFD5B0000
#define PHP_GRF_CLK_CON1        (PHP_GRF_BASE + 0x0070)
#define PHP_GRF_GMAC_CON0       (PHP_GRF_BASE + 0x0008)

#define SYS_GRF_BASE            0xFD58C000
#define SYS_GRF_SOC_CON7        (SYS_GRF_BASE + 0x031C)
#define SYS_GRF_SOC_CON8        (SYS_GRF_BASE + 0x0320)
#define SYS_GRF_SOC_CON9        (SYS_GRF_BASE + 0x0324)
#define  CLK_RX_DL_CFG_SHIFT    8
#define  CLK_TX_DL_CFG_SHIFT    0

#define TX_DELAY_GMAC0          FixedPcdGet8 (PcdGmac0TxDelay)
#define RX_DELAY_GMAC0          FixedPcdGet8 (PcdGmac0RxDelay)
#define TX_DELAY_GMAC1          FixedPcdGet8 (PcdGmac1TxDelay)
#define RX_DELAY_GMAC1          FixedPcdGet8 (PcdGmac1RxDelay)

#define GMAC0_BASE                         0xfe1b0000
#define GMAC1_BASE                         0xfe1c0000

/* GMAC registers */
#define  GMAC_MAC_MDIO_ADDRESS               0x0200
#define   GMAC_MAC_MDIO_ADDRESS_PA_SHIFT     21
#define   GMAC_MAC_MDIO_ADDRESS_RDA_SHIFT    16
#define   GMAC_MAC_MDIO_ADDRESS_CR_SHIFT     8
#define   GMAC_MAC_MDIO_ADDRESS_CR_100_150   (1U << GMAC_MAC_MDIO_ADDRESS_CR_SHIFT)
#define   GMAC_MAC_MDIO_ADDRESS_GOC_SHIFT    2
#define   GMAC_MAC_MDIO_ADDRESS_GOC_READ     (3U << GMAC_MAC_MDIO_ADDRESS_GOC_SHIFT)
#define   GMAC_MAC_MDIO_ADDRESS_GOC_WRITE    (1U << GMAC_MAC_MDIO_ADDRESS_GOC_SHIFT)
#define   GMAC_MAC_MDIO_ADDRESS_GB           BIT0
#define  GMAC_MAC_MDIO_DATA                  0x0204

#define GMAC_MAC_ADDRESS0_LOW               0x0304
#define GMAC_MAC_ADDRESS0_HIGH              0x0300

/* MII registers */
#define MII_PHYIDR1                         0x02
#define MII_PHYIDR2                         0x03

STATIC ETHERNET_PHY_INIT mPhyInitList[] = {
  RealtekPhyInit,
  MotorcommPhyInit
};

VOID
PhyRead (
  IN  EFI_PHYSICAL_ADDRESS  GmacBase,
  IN  UINT8                 Phy,
  IN  UINT16                Reg,
  OUT UINT16                *Value
  )
{
  UINT32 Addr;
  UINTN Retry;

  Addr = GMAC_MAC_MDIO_ADDRESS_CR_100_150 |
          (Phy << GMAC_MAC_MDIO_ADDRESS_PA_SHIFT) |
          (Reg << GMAC_MAC_MDIO_ADDRESS_RDA_SHIFT) |
          GMAC_MAC_MDIO_ADDRESS_GOC_READ |
          GMAC_MAC_MDIO_ADDRESS_GB;
  MmioWrite32 (GmacBase + GMAC_MAC_MDIO_ADDRESS, Addr);

  MicroSecondDelay (10000);

  for (Retry = 1000; Retry > 0; Retry--) {
    Addr = MmioRead32 (GmacBase + GMAC_MAC_MDIO_ADDRESS);
    if ((Addr & GMAC_MAC_MDIO_ADDRESS_GB) == 0) {
      *Value = MmioRead32 (GmacBase + GMAC_MAC_MDIO_DATA) & 0xFFFFu;
      break;
    }
    MicroSecondDelay (10);
  }
  if (Retry == 0) {
    DEBUG ((DEBUG_WARN, "MDIO: PHY read timeout!\n"));
    *Value = 0xFFFFU;
    ASSERT (FALSE);
  }
}

VOID
PhyWrite (
  IN EFI_PHYSICAL_ADDRESS   GmacBase,
  IN UINT8                  Phy,
  IN UINT16                 Reg,
  IN UINT16                 Value
  )
{
  UINT32 Addr;
  UINTN Retry;

  MmioWrite32 (GmacBase + GMAC_MAC_MDIO_DATA, Value);

  Addr = GMAC_MAC_MDIO_ADDRESS_CR_100_150 |
          (Phy << GMAC_MAC_MDIO_ADDRESS_PA_SHIFT) |
          (Reg << GMAC_MAC_MDIO_ADDRESS_RDA_SHIFT) |
          GMAC_MAC_MDIO_ADDRESS_GOC_WRITE |
          GMAC_MAC_MDIO_ADDRESS_GB;
  MmioWrite32 (GmacBase + GMAC_MAC_MDIO_ADDRESS, Addr);

  MicroSecondDelay (10000);

  for (Retry = 1000; Retry > 0; Retry--) {
    Addr = MmioRead32 (GmacBase + GMAC_MAC_MDIO_ADDRESS);
    if ((Addr & GMAC_MAC_MDIO_ADDRESS_GB) == 0) {
      break;
    }
    MicroSecondDelay (10);
  }
  if (Retry == 0) {
    DEBUG ((DEBUG_WARN, "MDIO: PHY write timeout!\n"));
    ASSERT (FALSE);
  }
}

STATIC
VOID
EFIAPI
PhyInit (
  IN EFI_PHYSICAL_ADDRESS GmacBase
  )
{
  EFI_STATUS Status;
  UINT16 PhyIdReg;
  UINT32 PhyId;
  UINT32 Index;

  PhyRead (GmacBase, 0, MII_PHYIDR1, &PhyIdReg);
  PhyId = PhyIdReg << 16;
  PhyRead (GmacBase, 0, MII_PHYIDR2, &PhyIdReg);
  PhyId |= PhyIdReg;

  for (Index = 0; Index < ARRAY_SIZE (mPhyInitList); Index++) {
    Status = mPhyInitList[Index] (GmacBase, PhyId);
    if (Status == EFI_UNSUPPORTED) {
      continue;
    } else if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: mPhyInitList[%d]() failed: %r\n", __func__, Index, Status));
    }
    return;
  }

  DEBUG ((DEBUG_ERROR, "%a: Unknown PHY ID %08X\n", __func__, PhyId));
}

STATIC
VOID
EFIAPI
InitGmac0 (
  VOID
  )
{
  /* Assert reset */
  MmioWrite32 (CRU_SOFTRST_CON32, 0x04000400); // aresetn_gmac0 = 1

  /* Configure pins */
  GmacIomux (0);

  /* Setup clocks */
  MmioWrite32 (PHP_GRF_CLK_CON1, 0x001d0000);  // io_clksel_gmac0 = io
                                               // mii_tx_clk_sel_gamc0 = 125 MHz
                                               // rmii_mode_gmac0 = RGMII mode

  MmioWrite32 (PHP_GRF_GMAC_CON0, 0x00380008);  // gmac0_phy_intf_sel = RGMII

  /* Setup DLLs */
  if (TX_DELAY_GMAC0) {
    MmioWrite32 (SYS_GRF_SOC_CON7, 0x00040004); // gmac0_txclk_dly_ena = 1
    MmioWrite32 (SYS_GRF_SOC_CON8, 0x007F0000U |
                  (TX_DELAY_GMAC0 << CLK_TX_DL_CFG_SHIFT));
  }

  if (RX_DELAY_GMAC0) {
    MmioWrite32 (SYS_GRF_SOC_CON7, 0x00080008); // gmac0_rxclk_dly_ena = 1
    MmioWrite32 (SYS_GRF_SOC_CON8, 0x7F000000U |
                  (RX_DELAY_GMAC0 << CLK_RX_DL_CFG_SHIFT));
  }

  /* Reset PHY */
  GmacIoPhyReset (0, TRUE);
  MicroSecondDelay (20000);
  GmacIoPhyReset (0, FALSE);
  MicroSecondDelay (200000);

  /* Deassert reset */
  MmioWrite32 (CRU_SOFTRST_CON32, 0x04000000); // aresetn_gmac0 = 0

  PhyInit (GMAC0_BASE);
}

STATIC
VOID
EFIAPI
InitGmac1 (
  VOID
  )
{
  /* Assert reset */
  MmioWrite32 (CRU_SOFTRST_CON32, 0x08000800); // aresetn_gmac1 = 1

  /* Configure pins */
  GmacIomux (1);

  /* Setup clocks */
  MmioWrite32 (PHP_GRF_CLK_CON1, 0x03a00000);  // io_clksel_gmac1 = io
                                               // mii_tx_clk_sel_gamc1 = 125 MHz
                                               // rmii_mode_gmac1 = RGMII mode

  MmioWrite32 (PHP_GRF_GMAC_CON0, 0x0e000200);  // gmac1_phy_intf_sel = RGMII

  /* Setup DLLs */
  if (TX_DELAY_GMAC1) {
    MmioWrite32 (SYS_GRF_SOC_CON7, 0x00100010); // gmac1_txclk_dly_ena = 1
    MmioWrite32 (SYS_GRF_SOC_CON9, 0x007F0000U |
                  (TX_DELAY_GMAC1 << CLK_TX_DL_CFG_SHIFT));
  }

  if (RX_DELAY_GMAC1) {
    MmioWrite32 (SYS_GRF_SOC_CON7, 0x00200020); // gmac1_rxclk_dly_ena = 1
    MmioWrite32 (SYS_GRF_SOC_CON9, 0x7F000000U |
                  (RX_DELAY_GMAC1 << CLK_RX_DL_CFG_SHIFT));
  }

  /* Reset PHY */
  GmacIoPhyReset (1, TRUE);
  MicroSecondDelay (20000);
  GmacIoPhyReset (1, FALSE);
  MicroSecondDelay (200000);

  /* Deassert reset */
  MmioWrite32 (CRU_SOFTRST_CON32, 0x08000000); // aresetn_gmac1 = 0

  PhyInit (GMAC1_BASE);
}

EFI_STATUS
EFIAPI
GmacPlatformDxeInitialize (
  IN EFI_HANDLE          ImageHandle,
  IN EFI_SYSTEM_TABLE    *SystemTable
  )
{
  UINT8 OtpData[32];
  UINT8 Hash[SHA256_DIGEST_SIZE];
  UINT32 MacLo, MacHi;

  if (!FixedPcdGetBool (PcdGmac0Supported)
      && !FixedPcdGetBool (PcdGmac1Supported)) {
    return EFI_SUCCESS;
  }

  /* Generate MAC addresses from the first 32 bytes in the OTP and write it to GMAC0 and GMAC1 */
  /* Use sequential MAC addresses. Last byte is even for GMAC0, and odd for GMAC1. */
  OtpRead (0x00, sizeof (OtpData), OtpData);
  Sha256HashAll (OtpData, sizeof (OtpData), Hash);
  Hash[0] &= 0xFE;
  Hash[0] |= 0x02;

  if (FixedPcdGetBool (PcdGmac0Supported)) {
    InitGmac0 ();

    Hash[5] &= ~1;
    DEBUG ((DEBUG_INFO, "%a: GMAC0 MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
            __func__,
            Hash[0], Hash[1], Hash[2],
            Hash[3], Hash[4], Hash[5]));
    MacLo = Hash[3] | (Hash[2] << 8) | (Hash[1] << 16) | (Hash[0] << 24);
    MacHi = Hash[5] | (Hash[4] << 8);
    MmioWrite32 (GMAC0_BASE + GMAC_MAC_ADDRESS0_LOW, MacLo);
    MmioWrite32 (GMAC0_BASE + GMAC_MAC_ADDRESS0_HIGH, MacHi);
  }

  if (FixedPcdGetBool (PcdGmac1Supported)) {
    InitGmac1 ();

    Hash[5] |= 1;
    DEBUG ((DEBUG_INFO, "%a: GMAC1 MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
            __func__,
            Hash[0], Hash[1], Hash[2],
            Hash[3], Hash[4], Hash[5]));
    MacLo = Hash[3] | (Hash[2] << 8) | (Hash[1] << 16) | (Hash[0] << 24);
    MacHi = Hash[5] | (Hash[4] << 8);
    MmioWrite32 (GMAC1_BASE + GMAC_MAC_ADDRESS0_LOW, MacLo);
    MmioWrite32 (GMAC1_BASE + GMAC_MAC_ADDRESS0_HIGH, MacHi);
  }

  return EFI_SUCCESS;
}
