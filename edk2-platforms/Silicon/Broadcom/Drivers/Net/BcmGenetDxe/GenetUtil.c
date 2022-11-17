/** @file

  Copyright (c) 2020 Jared McNeill. All rights reserved.
  Copyright (c) 2020 Andrey Warkentin <andrey.warkentin@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/DmaLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "BcmGenetDxe.h"

#define GENET_PHY_RETRY     1000

STATIC CONST
EFI_PHYSICAL_ADDRESS   mDmaAddressLimit = FixedPcdGet64 (PcdDmaDeviceLimit) -
                                          FixedPcdGet64 (PcdDmaDeviceOffset);

/**
  Read a memory-mapped device CSR.

  @param  Genet[in]   Pointer to GENERIC_PHY_PRIVATE_DATA instance.
  @param  Offset[in]  Register offset.

  @retval Value

**/
STATIC
UINT32
GenetMmioRead (
  IN GENET_PRIVATE_DATA *Genet,
  IN UINT32             Offset
  )
{
  ASSERT ((Offset & 3) == 0);

  return MmioRead32 (Genet->RegBase + Offset);
}

/**
  Write a memory-mapped device CSR.

  @param  Genet[in]   Pointer to GENERIC_PHY_PRIVATE_DATA instance.
  @param  Offset[in]  Register offset.
  @param  Data[in]    Data to write.

  @retval Value

**/
STATIC
VOID
GenetMmioWrite (
  IN GENET_PRIVATE_DATA *Genet,
  IN UINT32             Offset,
  IN UINT32             Data
  )
{
  ASSERT ((Offset & 3) == 0);

  MemoryFence ();
  MmioWrite32 (Genet->RegBase + Offset, Data);
}

/**
  Perform a GENET PHY register read.

  @param  Priv[in]     Pointer to GENET_PRIVATE_DATA.
  @param  PhyAddr[in]  PHY address.
  @param  Reg[in]      PHY register.
  @param  Data[out]    Pointer to register data read.

  @retval EFI_SUCCESS       Data read successfully.
  @retval EFI_DEVICE_ERROR  Failed to read data.

**/
EFI_STATUS
EFIAPI
GenetPhyRead (
  IN  VOID   *Priv,
  IN  UINT8  PhyAddr,
  IN  UINT8  Reg,
  OUT UINT16 *Data
  )
{
  GENET_PRIVATE_DATA   *Genet;
  UINTN                Retry;
  UINT32               Value;

  Genet = Priv;
  Value = GENET_MDIO_READ |
          GENET_MDIO_START_BUSY |
          SHIFTIN (PhyAddr, GENET_MDIO_PMD) |
          SHIFTIN (Reg, GENET_MDIO_REG);
  GenetMmioWrite (Genet, GENET_MDIO_CMD, Value);

  for (Retry = GENET_PHY_RETRY; Retry > 0; Retry--) {
    Value = GenetMmioRead (Genet, GENET_MDIO_CMD);
    if ((Value & GENET_MDIO_START_BUSY) == 0) {
      *Data = Value & 0xffff;
      break;
    }
    gBS->Stall (10);
  }

  if (Retry == 0) {
    DEBUG ((DEBUG_ERROR,
      "%a: Timeout reading PhyAddr %d, Reg %d\n", __FUNCTION__, PhyAddr, Reg));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Perform a GENET PHY register write.

  @param  Priv[in]     Pointer to GENET_PRIVATE_DATA.
  @param  PhyAddr[in]  PHY address.
  @param  Reg[in]      PHY register.
  @param  Data[in]     Pointer to register data to write.

  @retval EFI_SUCCESS       Data written successfully.
  @retval EFI_DEVICE_ERROR  Failed to write data.

**/
EFI_STATUS
EFIAPI
GenetPhyWrite (
  IN VOID   *Priv,
  IN UINT8  PhyAddr,
  IN UINT8  Reg,
  IN UINT16 Data
  )
{
  GENET_PRIVATE_DATA    *Genet;
  UINTN                 Retry;
  UINT32                Value;

  Genet = Priv;
  Value = GENET_MDIO_WRITE |
          GENET_MDIO_START_BUSY |
          SHIFTIN (PhyAddr, GENET_MDIO_PMD) |
          SHIFTIN (Reg, GENET_MDIO_REG);
  GenetMmioWrite (Genet, GENET_MDIO_CMD, Value | Data);

  for (Retry = GENET_PHY_RETRY; Retry > 0; Retry--) {
    Value = GenetMmioRead (Genet, GENET_MDIO_CMD);
    if ((Value & GENET_MDIO_START_BUSY) == 0) {
      break;
    }
    gBS->Stall (10);
  }

  if (Retry == 0) {
    DEBUG ((DEBUG_ERROR,
      "%a: Timeout writing PhyAddr %d, Reg %d\n", __FUNCTION__, PhyAddr, Reg));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Process a PHY link speed change (e.g. with MAC layer).

  @param  Priv[in]    Pointer to GENET_PRIVATE_DATA.
  @param  Speed[in]   Speed setting.
  @param  Duplex[in]  Duplex setting.

**/
VOID
EFIAPI
GenetPhyConfigure (
  IN VOID               *Priv,
  IN GENERIC_PHY_SPEED  Speed,
  IN GENERIC_PHY_DUPLEX Duplex
  )
{
  GENET_PRIVATE_DATA  *Genet;
  UINT32              Value;

  Genet = Priv;
  Value = GenetMmioRead (Genet, GENET_EXT_RGMII_OOB_CTRL);
  Value &= ~GENET_EXT_RGMII_OOB_OOB_DISABLE;
  Value |= GENET_EXT_RGMII_OOB_RGMII_LINK;
  Value |= GENET_EXT_RGMII_OOB_RGMII_MODE_EN;
  if (Genet->PhyMode == GENET_PHY_MODE_RGMII) {
    Value |= GENET_EXT_RGMII_OOB_ID_MODE_DISABLE;
  } else {
    Value &= ~GENET_EXT_RGMII_OOB_ID_MODE_DISABLE;
  }
  GenetMmioWrite (Genet, GENET_EXT_RGMII_OOB_CTRL, Value);

  Value = GenetMmioRead (Genet, GENET_UMAC_CMD);
  Value &= ~GENET_UMAC_CMD_SPEED;
  switch (Speed) {
    case PHY_SPEED_1000:
      Value |= SHIFTIN (GENET_UMAC_CMD_SPEED_1000, GENET_UMAC_CMD_SPEED);
      break;
    case PHY_SPEED_100:
      Value |= SHIFTIN (GENET_UMAC_CMD_SPEED_100, GENET_UMAC_CMD_SPEED);
      break;
    default:
      Value |= SHIFTIN (GENET_UMAC_CMD_SPEED_10, GENET_UMAC_CMD_SPEED);
      break;
  }
  if (Duplex == PHY_DUPLEX_FULL) {
    Value &= ~GENET_UMAC_CMD_HD_EN;
  } else {
    Value |= GENET_UMAC_CMD_HD_EN;
  }
  GenetMmioWrite (Genet, GENET_UMAC_CMD, Value);
}

/**
  Extra action to run after a PHY reset. This adds the appropriate clock
  delay based on the PHY mode.

  @param  Priv[in]  Pointer to GENET_PRIVATE_DATA.

**/
EFI_STATUS
EFIAPI
GenetPhyResetAction (
  IN VOID             *Priv
  )
{
  GENET_PRIVATE_DATA  *Genet;
  UINT16              Value;
  EFI_STATUS          Status;

  Genet = Priv;
  Status = GenetPhyWrite (Priv, Genet->Phy.PhyAddr, BRGPHY_MII_AUXCTL,
             BRGPHY_AUXCTL_SHADOW_MISC |
             (BRGPHY_AUXCTL_SHADOW_MISC << BRGPHY_AUXCTL_MISC_READ_SHIFT));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GenetPhyRead (Priv, Genet->Phy.PhyAddr, BRGPHY_MII_AUXCTL, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Value &= BRGPHY_AUXCTL_MISC_DATA_MASK;

  if (Genet->PhyMode == GENET_PHY_MODE_RGMII_RXID ||
      Genet->PhyMode == GENET_PHY_MODE_RGMII_ID) {
    Value |= BRGPHY_AUXCTL_MISC_RGMII_SKEW_EN;
  } else {
    Value &= ~BRGPHY_AUXCTL_MISC_RGMII_SKEW_EN;
  }

  Status = GenetPhyWrite (Priv, Genet->Phy.PhyAddr, BRGPHY_MII_AUXCTL,
             BRGPHY_AUXCTL_MISC_WRITE_EN | BRGPHY_AUXCTL_SHADOW_MISC | Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GenetPhyWrite (Priv, Genet->Phy.PhyAddr, BRGPHY_MII_SHADOW_1C,
                          BRGPHY_SHADOW_1C_CLK_CTRL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GenetPhyRead (Priv, Genet->Phy.PhyAddr, BRGPHY_MII_SHADOW_1C, &Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Value &= BRGPHY_SHADOW_1C_DATA_MASK;

  if (Genet->PhyMode == GENET_PHY_MODE_RGMII_TXID ||
      Genet->PhyMode == GENET_PHY_MODE_RGMII_ID) {
    Value |= BRGPHY_SHADOW_1C_GTXCLK_EN;
  } else {
    Value &= ~BRGPHY_SHADOW_1C_GTXCLK_EN;
  }

  Status = GenetPhyWrite (Priv, Genet->Phy.PhyAddr, BRGPHY_MII_SHADOW_1C,
             BRGPHY_SHADOW_1C_WRITE_EN | BRGPHY_SHADOW_1C_CLK_CTRL | Value);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Reset GENET.

  @param  Genet[in]  Pointer to GENET_PRIVATE_DATA.

**/
VOID
GenetReset (
  IN GENET_PRIVATE_DATA   *Genet
  )
{
  UINT32  Value;

  Value = GenetMmioRead (Genet, GENET_SYS_RBUF_FLUSH_CTRL);
  Value |= GENET_SYS_RBUF_FLUSH_RESET;
  GenetMmioWrite (Genet, GENET_SYS_RBUF_FLUSH_CTRL, Value);
  gBS->Stall (10);

  Value &= ~GENET_SYS_RBUF_FLUSH_RESET;
  GenetMmioWrite (Genet, GENET_SYS_RBUF_FLUSH_CTRL, Value);
  gBS->Stall (10);

  GenetMmioWrite (Genet, GENET_SYS_RBUF_FLUSH_CTRL, 0);
  gBS->Stall (10);

  GenetMmioWrite (Genet, GENET_UMAC_CMD, 0);
  GenetMmioWrite (Genet, GENET_UMAC_CMD,
    GENET_UMAC_CMD_LCL_LOOP_EN | GENET_UMAC_CMD_SW_RESET);
  gBS->Stall (10);
  GenetMmioWrite (Genet, GENET_UMAC_CMD, 0);

  GenetMmioWrite (Genet, GENET_UMAC_MIB_CTRL,
    GENET_UMAC_MIB_RESET_RUNT | GENET_UMAC_MIB_RESET_RX | GENET_UMAC_MIB_RESET_TX);
  GenetMmioWrite (Genet, GENET_UMAC_MIB_CTRL, 0);

  GenetMmioWrite (Genet, GENET_UMAC_MAX_FRAME_LEN, GENET_MAX_PACKET_SIZE);

  Value = GenetMmioRead (Genet, GENET_RBUF_CTRL);
  Value |= GENET_RBUF_ALIGN_2B;
  GenetMmioWrite (Genet, GENET_RBUF_CTRL, Value);

  GenetMmioWrite (Genet, GENET_RBUF_TBUF_SIZE_CTRL, 1);
}

/**
  Set the station address.

  @param  Genet[in]    Pointer to GENET_PRIVATE_DATA.
  @param  MacAddr[in]  MAC address to set.

**/
VOID
EFIAPI
GenetSetMacAddress (
  IN GENET_PRIVATE_DATA   *Genet,
  IN EFI_MAC_ADDRESS      *MacAddr
  )
{
  UINT32  Value;

  Value = MacAddr->Addr[3] |
          MacAddr->Addr[2] << 8 |
          MacAddr->Addr[1] << 16 |
          MacAddr->Addr[0] << 24;
  GenetMmioWrite (Genet, GENET_UMAC_MAC0, Value);
  Value = MacAddr->Addr[5] |
          MacAddr->Addr[4] << 8;
  GenetMmioWrite (Genet, GENET_UMAC_MAC1, Value);
}

/**
  Set a PHY mode.

  @param  Genet[in]    Pointer to GENET_PRIVATE_DATA.
  @param  PhyMode[in]  Mode to set.

**/
VOID
GenetSetPhyMode (
  IN GENET_PRIVATE_DATA   *Genet,
  IN GENET_PHY_MODE       PhyMode
  )
{
  UINT32  Value;

  switch (PhyMode) {
    case GENET_PHY_MODE_RGMII:
    case GENET_PHY_MODE_RGMII_RXID:
    case GENET_PHY_MODE_RGMII_TXID:
    case GENET_PHY_MODE_RGMII_ID:
      Value = GENET_SYS_PORT_MODE_EXT_GPHY;
      break;
    default:
      Value = 0;
      break;
  }
  GenetMmioWrite (Genet, GENET_SYS_PORT_CTRL, Value);
}

/**
  Enable TX/RX.

  @param  Genet[in]  Pointer to GENET_PRIVATE_DATA.

**/
VOID
GenetEnableTxRx (
  IN GENET_PRIVATE_DATA   *Genet
  )
{
  UINT32 Value;

  // Start TX DMA on default queue
  Value = GenetMmioRead (Genet, GENET_TX_DMA_CTRL);
  Value |= GENET_TX_DMA_CTRL_EN |
           GENET_TX_DMA_CTRL_RBUF_EN (GENET_DMA_DEFAULT_QUEUE);
  GenetMmioWrite (Genet, GENET_TX_DMA_CTRL, Value);

  // Start RX DMA on default queue
  Value = GenetMmioRead (Genet, GENET_RX_DMA_CTRL);
  Value |= GENET_RX_DMA_CTRL_EN |
           GENET_RX_DMA_CTRL_RBUF_EN (GENET_DMA_DEFAULT_QUEUE);
  GenetMmioWrite (Genet, GENET_RX_DMA_CTRL, Value);

  // Enable transmitter and receiver
  Value = GenetMmioRead (Genet, GENET_UMAC_CMD);
  Value |= GENET_UMAC_CMD_TXEN | GENET_UMAC_CMD_RXEN;
  GenetMmioWrite (Genet, GENET_UMAC_CMD, Value);

  // Enable interrupts
  GenetMmioWrite (Genet, GENET_INTRL2_CPU_CLEAR_MASK,
    GENET_IRQ_TXDMA_DONE | GENET_IRQ_RXDMA_DONE);
}

/**
  Disable TX/RX.

  @param  Genet[in]  Pointer to GENET_PRIVATE_DATA.

**/
VOID
GenetDisableTxRx (
  IN GENET_PRIVATE_DATA   *Genet
  )
{
  UINT32  Value;

  // Disable interrupts
  GenetMmioWrite (Genet, GENET_INTRL2_CPU_SET_MASK, 0xFFFFFFFF);
  GenetMmioWrite (Genet, GENET_INTRL2_CPU_CLEAR, 0xFFFFFFFF);

  // Disable receiver
  Value = GenetMmioRead (Genet, GENET_UMAC_CMD);
  Value &= ~GENET_UMAC_CMD_RXEN;
  GenetMmioWrite (Genet, GENET_UMAC_CMD, Value);

  // Stop RX DMA
  Value = GenetMmioRead (Genet, GENET_RX_DMA_CTRL);
  Value &= ~GENET_RX_DMA_CTRL_EN;
  Value &= ~GENET_RX_DMA_CTRL_RBUF_EN (GENET_DMA_DEFAULT_QUEUE);
  GenetMmioWrite (Genet, GENET_RX_DMA_CTRL, Value);

  // Stop TX DMA
  Value = GenetMmioRead (Genet, GENET_TX_DMA_CTRL);
  Value &= ~GENET_TX_DMA_CTRL_EN;
  Value &= ~GENET_TX_DMA_CTRL_RBUF_EN (GENET_DMA_DEFAULT_QUEUE);
  GenetMmioWrite (Genet, GENET_TX_DMA_CTRL, Value);

  // Flush data in the TX FIFO
  GenetMmioWrite (Genet, GENET_UMAC_TX_FLUSH, 1);
  gBS->Stall (10);
  GenetMmioWrite (Genet, GENET_UMAC_TX_FLUSH, 0);

  // Disable transmitter
  Value = GenetMmioRead (Genet, GENET_UMAC_CMD);
  Value &= ~GENET_UMAC_CMD_TXEN;
  GenetMmioWrite (Genet, GENET_UMAC_CMD, Value);
}

/**
  Change promiscuous mode state.

  @param  Genet[in]   Pointer to GENET_PRIVATE_DATA.
  @param  Enable[in]  Promiscuous mode state.

**/
VOID
GenetSetPromisc (
  IN GENET_PRIVATE_DATA   *Genet,
  IN BOOLEAN              Enable
  )
{
  UINT32 Value;

  Value = GenetMmioRead (Genet, GENET_UMAC_CMD);
  if (Enable) {
    Value |= GENET_UMAC_CMD_PROMISC;
  } else {
    Value &= ~GENET_UMAC_CMD_PROMISC;
  }
  GenetMmioWrite (Genet, GENET_UMAC_CMD, Value);
}

/**
  Enable the MAC filter for the Ethernet broadcast address

  @param  Genet[in]   Pointer to GENET_PRIVATE_DATA.
  @param  Enable[in]  Promiscuous mode state.

**/
VOID
GenetEnableBroadcastFilter (
  IN GENET_PRIVATE_DATA   *Genet,
  IN BOOLEAN              Enable
  )
{
  CONST EFI_MAC_ADDRESS   *MacAddr;
  UINT32                  Value;

  if (Enable) {
    MacAddr = &Genet->SnpMode.CurrentAddress;

    GenetMmioWrite (Genet, GENET_UMAC_MDF_ADDR0 (0),
      MacAddr->Addr[1] | MacAddr->Addr[0] << 8);
    GenetMmioWrite (Genet, GENET_UMAC_MDF_ADDR1 (0),
      MacAddr->Addr[5] | MacAddr->Addr[4] << 8 |
      MacAddr->Addr[3] << 16 | MacAddr->Addr[2] << 24);

    GenetMmioWrite (Genet, GENET_UMAC_MDF_ADDR0 (1), 0xffff);
    GenetMmioWrite (Genet, GENET_UMAC_MDF_ADDR1 (1), 0xffffffff);

    Value = BIT16 | BIT15; // enable filters 0 and 1
  } else {
    Value = 0;
  }
  GenetMmioWrite (Genet, GENET_UMAC_MDF_CTRL, Value);
}

/**
  Configure DMA TX and RX queues, enabling them.

  @param  Genet[in]  Pointer to GENET_PRIVATE_DATA.

**/
VOID
GenetDmaInitRings (
  IN GENET_PRIVATE_DATA *Genet
  )
{
  UINT8 Qid;

  Qid = GENET_DMA_DEFAULT_QUEUE;

  Genet->TxQueued = 0;
  Genet->TxNext = 0;
  Genet->TxConsIndex = 0;
  Genet->TxProdIndex = 0;

  Genet->RxConsIndex = 0;
  Genet->RxProdIndex = 0;

  // Configure TX queue
  GenetMmioWrite (Genet, GENET_TX_SCB_BURST_SIZE, 0x08);
  GenetMmioWrite (Genet, GENET_TX_DMA_READ_PTR_LO (Qid), 0);
  GenetMmioWrite (Genet, GENET_TX_DMA_READ_PTR_HI (Qid), 0);
  GenetMmioWrite (Genet, GENET_TX_DMA_CONS_INDEX (Qid), 0);
  GenetMmioWrite (Genet, GENET_TX_DMA_PROD_INDEX (Qid), 0);
  GenetMmioWrite (Genet, GENET_TX_DMA_RING_BUF_SIZE (Qid),
    SHIFTIN (GENET_DMA_DESC_COUNT, GENET_TX_DMA_RING_BUF_SIZE_DESC_COUNT) |
    SHIFTIN (GENET_MAX_PACKET_SIZE, GENET_TX_DMA_RING_BUF_SIZE_BUF_LENGTH));
  GenetMmioWrite (Genet, GENET_TX_DMA_START_ADDR_LO (Qid), 0);
  GenetMmioWrite (Genet, GENET_TX_DMA_START_ADDR_HI (Qid), 0);
  GenetMmioWrite (Genet, GENET_TX_DMA_END_ADDR_LO (Qid),
    GENET_DMA_DESC_COUNT * GENET_DMA_DESC_SIZE / 4 - 1);
  GenetMmioWrite (Genet, GENET_TX_DMA_END_ADDR_HI (Qid), 0);
  GenetMmioWrite (Genet, GENET_TX_DMA_MBUF_DONE_THRES (Qid), 1);
  GenetMmioWrite (Genet, GENET_TX_DMA_FLOW_PERIOD (Qid), 0);
  GenetMmioWrite (Genet, GENET_TX_DMA_WRITE_PTR_LO (Qid), 0);
  GenetMmioWrite (Genet, GENET_TX_DMA_WRITE_PTR_HI (Qid), 0);

  // Enable TX queue
  GenetMmioWrite (Genet, GENET_TX_DMA_RING_CFG, (1U << Qid));

  // Configure RX queue
  GenetMmioWrite (Genet, GENET_RX_SCB_BURST_SIZE, 0x08);
  GenetMmioWrite (Genet, GENET_RX_DMA_WRITE_PTR_LO (Qid), 0);
  GenetMmioWrite (Genet, GENET_RX_DMA_WRITE_PTR_HI (Qid), 0);
  GenetMmioWrite (Genet, GENET_RX_DMA_PROD_INDEX (Qid), 0);
  GenetMmioWrite (Genet, GENET_RX_DMA_CONS_INDEX (Qid), 0);
  GenetMmioWrite (Genet, GENET_RX_DMA_RING_BUF_SIZE (Qid),
    SHIFTIN (GENET_DMA_DESC_COUNT, GENET_RX_DMA_RING_BUF_SIZE_DESC_COUNT) |
    SHIFTIN (GENET_MAX_PACKET_SIZE, GENET_RX_DMA_RING_BUF_SIZE_BUF_LENGTH));
  GenetMmioWrite (Genet, GENET_RX_DMA_START_ADDR_LO (Qid), 0);
  GenetMmioWrite (Genet, GENET_RX_DMA_START_ADDR_HI (Qid), 0);
  GenetMmioWrite (Genet, GENET_RX_DMA_END_ADDR_LO (Qid),
    GENET_DMA_DESC_COUNT * GENET_DMA_DESC_SIZE / 4 - 1);
  GenetMmioWrite (Genet, GENET_RX_DMA_END_ADDR_HI (Qid), 0);
  GenetMmioWrite (Genet, GENET_RX_DMA_XON_XOFF_THRES (Qid),
    SHIFTIN (5, GENET_RX_DMA_XON_XOFF_THRES_LO) |
    SHIFTIN (GENET_DMA_DESC_COUNT >> 4, GENET_RX_DMA_XON_XOFF_THRES_HI));
  GenetMmioWrite (Genet, GENET_RX_DMA_READ_PTR_LO (Qid), 0);
  GenetMmioWrite (Genet, GENET_RX_DMA_READ_PTR_HI (Qid), 0);

  // Enable RX queue
  GenetMmioWrite (Genet, GENET_RX_DMA_RING_CFG, (1U << Qid));
}

/**
  Allocate DMA buffers for RX.

  @param  Genet[in]  Pointer to GENET_PRIVATE_DATA.

  @retval EFI_SUCCESS           DMA buffers allocated.
  @retval EFI_OUT_OF_RESOURCES  DMA buffers could not be allocated.
**/
EFI_STATUS
GenetDmaAlloc (
  IN GENET_PRIVATE_DATA   *Genet
  )
{
  EFI_STATUS              Status;

  Genet->RxBuffer = mDmaAddressLimit;
  Status = gBS->AllocatePages (AllocateMaxAddress, EfiBootServicesData,
                  EFI_SIZE_TO_PAGES (GENET_MAX_PACKET_SIZE * GENET_DMA_DESC_COUNT),
                  &Genet->RxBuffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "%a: Failed to allocate RX buffer: %r\n", __FUNCTION__, Status));
  }
  return Status;
}

/**
  Given an RX buffer descriptor index, program the IO address of the buffer into the hardware.

  @param  Genet[in]      Pointer to GENET_PRIVATE_DATA.
  @param  DescIndex[in]  Index of RX buffer descriptor.

  @retval EFI_SUCCESS  DMA buffers allocated.
  @retval Others       Programmatic errors, as buffers come from DmaAllocateBuffer, and thus
                       cannot fail DmaMap (for the expected NonCoherentDmaLib).
**/
EFI_STATUS
GenetDmaMapRxDescriptor (
  IN GENET_PRIVATE_DATA * Genet,
  IN UINT8                DescIndex
  )
{
  EFI_STATUS    Status;
  UINTN         DmaNumberOfBytes;

  ASSERT (Genet->RxBufferMap[DescIndex].Mapping == NULL);
  ASSERT (Genet->RxBuffer != 0);

  DmaNumberOfBytes = GENET_MAX_PACKET_SIZE;
  Status = DmaMap (MapOperationBusMasterWrite,
             GENET_RX_BUFFER (Genet, DescIndex),
             &DmaNumberOfBytes,
             &Genet->RxBufferMap[DescIndex].PhysAddress,
             &Genet->RxBufferMap[DescIndex].Mapping);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to map RX buffer: %r\n",
      __FUNCTION__, Status));
    return Status;
  }

  GenetMmioWrite (Genet, GENET_RX_DESC_ADDRESS_LO (DescIndex),
    Genet->RxBufferMap[DescIndex].PhysAddress & 0xFFFFFFFF);
  GenetMmioWrite (Genet, GENET_RX_DESC_ADDRESS_HI (DescIndex),
    (Genet->RxBufferMap[DescIndex].PhysAddress >> 32) & 0xFFFFFFFF);
  GenetMmioWrite (Genet, GENET_RX_DESC_STATUS (DescIndex), 0);

  return EFI_SUCCESS;
}

/**
  Given an RX buffer descriptor index, undo the DmaMap operation on the buffer.

  @param  Genet[in]      Pointer to GENET_PRIVATE_DATA.
  @param  DescIndex[in]  Index of RX buffer descriptor.

**/
VOID
GenetDmaUnmapRxDescriptor (
  IN GENET_PRIVATE_DATA * Genet,
  IN UINT8                DescIndex
  )
{
  if (Genet->RxBufferMap[DescIndex].Mapping != NULL) {
    DmaUnmap (Genet->RxBufferMap[DescIndex].Mapping);
    Genet->RxBufferMap[DescIndex].Mapping = NULL;
  }
}

/**
  Free DMA buffers for RX, undoing GenetDmaAlloc.

  @param  Genet[in]      Pointer to GENET_PRIVATE_DATA.
  @param  DescIndex[in]  Index of RX buffer descriptor.

**/
VOID
GenetDmaFree (
  IN GENET_PRIVATE_DATA *Genet
  )
{
  UINTN Idx;

  for (Idx = 0; Idx < GENET_DMA_DESC_COUNT; Idx++) {
    GenetDmaUnmapRxDescriptor (Genet, Idx);
  }
  gBS->FreePages (Genet->RxBuffer,
         EFI_SIZE_TO_PAGES (GENET_MAX_PACKET_SIZE * GENET_DMA_DESC_COUNT));
}

/**
  Queue TX transmission, given a buffer to transmit and a TX descriptor index.

  @param  Genet[in]          Pointer to GENET_PRIVATE_DATA.
  @param  DescIndex[in]      TX descriptor index.
  @param  PhysAddr[in]       Buffer to transmit.
  @param  NumberOfBytes[in]  Buffer length.

**/
VOID
GenetDmaTriggerTx (
  IN GENET_PRIVATE_DATA * Genet,
  IN UINT8                DescIndex,
  IN EFI_PHYSICAL_ADDRESS PhysAddr,
  IN UINTN                NumberOfBytes
  )
{
  UINT32    DescStatus;

  DescStatus = GENET_TX_DESC_STATUS_SOP |
               GENET_TX_DESC_STATUS_EOP |
               GENET_TX_DESC_STATUS_CRC |
               GENET_TX_DESC_STATUS_QTAG |
               SHIFTIN (NumberOfBytes, GENET_TX_DESC_STATUS_BUFLEN);

  GenetMmioWrite (Genet, GENET_TX_DESC_ADDRESS_LO (DescIndex),
    PhysAddr & 0xFFFFFFFF);
  GenetMmioWrite (Genet, GENET_TX_DESC_ADDRESS_HI (DescIndex),
    (PhysAddr >> 32) & 0xFFFFFFFF);
  GenetMmioWrite (Genet, GENET_TX_DESC_STATUS (DescIndex), DescStatus);

  GenetMmioWrite (Genet, GENET_TX_DMA_PROD_INDEX (GENET_DMA_DEFAULT_QUEUE),
    Genet->TxProdIndex);
}

/**
  Simulate a "TX interrupt", return the next (completed) TX buffer to recycle.

  @param  Genet[in]   Pointer to GENET_PRIVATE_DATA.
  @param  TxBuf[out]  Location to store pointer to next TX buffer to recycle.

**/
VOID
GenetTxIntr (
  IN  GENET_PRIVATE_DATA *Genet,
  OUT VOID               **TxBuf
  )
{
  UINT32 Total;

  Total = GenetTxPending (Genet);
  if (Genet->TxQueued > 0 && Total > 0) {
    DmaUnmap (Genet->TxBufferMap[Genet->TxNext]);
    *TxBuf = Genet->TxBuffer[Genet->TxNext];
    Genet->TxQueued--;
    Genet->TxNext = (Genet->TxNext + 1) % GENET_DMA_DESC_COUNT;
    Genet->TxConsIndex = (Genet->TxConsIndex + 1) & 0xFFFF;
  } else {
    *TxBuf = NULL;
  }
}

UINT32
GenetRxPending (
  IN  GENET_PRIVATE_DATA *Genet
  )
{
  UINT32 ProdIndex;
  UINT32 ConsIndex;

  ConsIndex = GenetMmioRead (Genet,
                GENET_RX_DMA_CONS_INDEX (GENET_DMA_DEFAULT_QUEUE)) & 0xFFFF;
  ASSERT (ConsIndex == Genet->RxConsIndex);

  ProdIndex = GenetMmioRead (Genet,
                GENET_RX_DMA_PROD_INDEX (GENET_DMA_DEFAULT_QUEUE)) & 0xFFFF;
  return (ProdIndex - Genet->RxConsIndex) & 0xFFFF;
}

UINT32
GenetTxPending (
  IN  GENET_PRIVATE_DATA *Genet
  )
{
  UINT32 ConsIndex;

  ConsIndex = GenetMmioRead (Genet,
                GENET_TX_DMA_CONS_INDEX (GENET_DMA_DEFAULT_QUEUE)) & 0xFFFF;

  return (ConsIndex - Genet->TxConsIndex) & 0xFFFF;
}

VOID
GenetRxComplete (
  IN GENET_PRIVATE_DATA *Genet
  )
{
  Genet->RxConsIndex = (Genet->RxConsIndex + 1) & 0xFFFF;
  GenetMmioWrite (Genet, GENET_RX_DMA_CONS_INDEX (GENET_DMA_DEFAULT_QUEUE),
                  Genet->RxConsIndex);
}

/**
  Simulate an "RX interrupt", returning the index of a completed RX buffer and
  corresponding frame length.

  @param  Genet[in]         Pointer to GENET_PRIVATE_DATA.
  @param  DescIndex[out]    Location to store completed RX buffer index.
  @param  FrameLength[out]  Location to store frame length.

  @retval EFI_SUCCESS    Data received.
  @retval EFI_NOT_READY  No RX buffers ready as no data received.

**/
EFI_STATUS
GenetRxIntr (
  IN  GENET_PRIVATE_DATA *Genet,
  OUT UINT8              *DescIndex,
  OUT UINTN              *FrameLength
  )
{
  EFI_STATUS    Status;
  UINT32        Total;
  UINT32        DescStatus;

  Total = GenetRxPending (Genet);
  if (Total > 0) {
    *DescIndex = Genet->RxConsIndex % GENET_DMA_DESC_COUNT;
    DescStatus = GenetMmioRead (Genet, GENET_RX_DESC_STATUS (*DescIndex));
    *FrameLength = SHIFTOUT (DescStatus, GENET_RX_DESC_STATUS_BUFLEN);
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_NOT_READY;
  }

  return Status;
}
