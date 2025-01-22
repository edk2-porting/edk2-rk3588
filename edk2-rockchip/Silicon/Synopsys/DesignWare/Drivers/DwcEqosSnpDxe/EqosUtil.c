/** @file

  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>
  Copyright (c) 2022 Jared McNeill <jmcneill@invisible.ca>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/IoLib.h>

#include "Eqos.h"

#define DMA_DEVICE_ADDRESS_LIMIT \
  (FixedPcdGet64 (PcdDmaDeviceLimit) - FixedPcdGet64 (PcdDmaDeviceOffset))

EFI_STATUS
EqosIdentify (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  UINT32  Version;
  UINT32  UserVersion;
  UINT32  SnpsVersion;
  UINT32  Index;

  Version     = MmioRead32 (Eqos->Base + GMAC_MAC_VERSION);
  UserVersion = (Version & GMAC_MAC_VERSION_USERVER_MASK) >>
                GMAC_MAC_VERSION_USERVER_SHIFT;
  SnpsVersion = Version & GMAC_MAC_VERSION_SNPSVER_MASK;

  if ((SnpsVersion < 0x51) || (SnpsVersion > 0x52)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Ver 0x%02x not supported!\n",
      __func__,
      SnpsVersion
      ));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((
    DEBUG_ERROR,
    "%a: Synopsys DesignWare EQOS ver 0x%02x (0x%02x) @ 0x%lx\n",
    __func__,
    SnpsVersion,
    UserVersion,
    Eqos->Base
    ));

  for (Index = 0; Index < 4; Index++) {
    Eqos->HwFeatures[Index] = MmioRead32 (Eqos->Base + GMAC_MAC_HW_FEATURE (Index));
  }

  DEBUG ((
    DEBUG_ERROR,
    "%a: HW Features: 0x%08x 0x%08x 0x%08x 0x%08x\n",
    __func__,
    Eqos->HwFeatures[0],
    Eqos->HwFeatures[1],
    Eqos->HwFeatures[2],
    Eqos->HwFeatures[3]
    ));

  return EFI_SUCCESS;
}

VOID
EqosInitializeConfig (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  DWC_EQOS_CONFIG                    *Config;
  DWC_EQOS_PLATFORM_DEVICE_PROTOCOL  *Platform;

  Config = &Eqos->Config;

  //
  // Default values
  //
  Config->CsrClockRate  = 125000000;
  Config->AxiBusWidth   = EqosAxiBusWidth64;
  Config->AxiFixedBurst = FALSE;
  Config->AxiMixedBurst = FALSE;
  Config->AxiWrOsrLmt   = 1;
  Config->AxiRdOsrLmt   = 1;
  Config->AxiBlen       = 0;

  //
  // Allow platform to override defaults
  //
  Platform = Eqos->Platform;
  if (Platform->GetConfig != NULL) {
    Platform->GetConfig (Platform, Config);
  }
}

EFI_STATUS
EqosReset (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  UINT32  Value;
  UINT32  Retry;

  MmioWrite32 (Eqos->Base + GMAC_DMA_MODE, GMAC_DMA_MODE_SWR);
  for (Retry = 2000; Retry > 0; Retry--) {
    gBS->Stall (1000);
    Value = MmioRead32 (Eqos->Base + GMAC_DMA_MODE);
    if ((Value & GMAC_DMA_MODE_SWR) == 0) {
      return EFI_SUCCESS;
    }
  }

  DEBUG ((DEBUG_ERROR, "%a: Timeout!\n", __func__));
  return EFI_TIMEOUT;
}

EFI_STATUS
EqosInitialize (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  EFI_STATUS       Status;
  DWC_EQOS_CONFIG  *Config;
  UINT32           Value;
  UINT32           Tqs;
  UINT32           Rqs;

  Status = EqosReset (Eqos);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Config = &Eqos->Config;

  MmioWrite32 (
    Eqos->Base + GMAC_MAC_1US_TIC_COUNTER,
    (Config->CsrClockRate / 1000000) - 1
    );

  //
  // Set AXI configuration
  //
  Value = MmioRead32 (Eqos->Base + GMAC_DMA_SYSBUS_MODE);
  if (Config->AxiFixedBurst) {
    Value |= GMAC_DMA_SYSBUS_MODE_FB;
  }

  if (Config->AxiMixedBurst) {
    Value |= GMAC_DMA_SYSBUS_MODE_MB;
  }

  Value &= ~GMAC_DMA_SYSBUS_MODE_WR_OSR_LMT_MASK;
  Value |= Config->AxiWrOsrLmt << GMAC_DMA_SYSBUS_MODE_WR_OSR_LMT_SHIFT;
  Value &= ~GMAC_DMA_SYSBUS_MODE_RD_OSR_LMT_MASK;
  Value |= Config->AxiRdOsrLmt << GMAC_DMA_SYSBUS_MODE_RD_OSR_LMT_SHIFT;

  Value |= Config->AxiBlen;

  MmioWrite32 (Eqos->Base + GMAC_DMA_SYSBUS_MODE, Value);

  //
  // Configure transmit and receive DMA
  //
  Value  = MmioRead32 (Eqos->Base + GMAC_DMA_CHAN0_CONTROL);
  Value &= ~GMAC_DMA_CHAN0_CONTROL_DSL_MASK;
  Value |= ((EQOS_DESC_ALIGN - 16) / Config->AxiBusWidth) << GMAC_DMA_CHAN0_CONTROL_DSL_SHIFT;
  Value |= GMAC_DMA_CHAN0_CONTROL_PBLX8;
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_CONTROL, Value);
  Value  = MmioRead32 (Eqos->Base + GMAC_DMA_CHAN0_TX_CONTROL);
  Value |= GMAC_DMA_CHAN0_TX_CONTROL_OSP;
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_TX_CONTROL, Value);
  Value  = MmioRead32 (Eqos->Base + GMAC_DMA_CHAN0_RX_CONTROL);
  Value &= ~GMAC_DMA_CHAN0_RX_CONTROL_RBSZ_MASK;
  Value |= (EQOS_RX_BUFFER_SIZE << GMAC_DMA_CHAN0_RX_CONTROL_RBSZ_SHIFT);
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_RX_CONTROL, Value);

  //
  // Disable counters
  //
  MmioWrite32 (
    Eqos->Base + GMAC_MMC_CONTROL,
    GMAC_MMC_CONTROL_CNTFREEZ |
    GMAC_MMC_CONTROL_CNTPRST |
    GMAC_MMC_CONTROL_CNTPRSTLVL
    );

  //
  // Configure operation modes
  //
  MmioWrite32 (
    Eqos->Base + GMAC_MTL_TXQ0_OPERATION_MODE,
    GMAC_MTL_TXQ0_OPERATION_MODE_TSF |
    GMAC_MTL_TXQ0_OPERATION_MODE_TXQEN_EN
    );
  MmioWrite32 (
    Eqos->Base + GMAC_MTL_RXQ0_OPERATION_MODE,
    GMAC_MTL_RXQ0_OPERATION_MODE_RSF |
    GMAC_MTL_RXQ0_OPERATION_MODE_FEP |
    GMAC_MTL_RXQ0_OPERATION_MODE_FUP
    );

  //
  // TX/RX fifo size in hw_feature[1] are log2(n/128), and
  // TQS/RQS in TXQ0/RXQ0_OPERATION_MODE are n/256-1.
  //
  Tqs = (128 << SHIFTOUT (
                  Eqos->HwFeatures[1],
                  GMAC_MAC_HW_FEATURE1_TXFIFOSIZE
                  ) / 256) - 1;
  Value  = MmioRead32 (Eqos->Base + GMAC_MTL_TXQ0_OPERATION_MODE);
  Value &= ~GMAC_MTL_TXQ0_OPERATION_MODE_TQS;
  Value |= SHIFTIN (Tqs, GMAC_MTL_TXQ0_OPERATION_MODE_TQS);
  MmioWrite32 (Eqos->Base + GMAC_MTL_TXQ0_OPERATION_MODE, Value);

  Rqs = (128 << SHIFTOUT (
                  Eqos->HwFeatures[1],
                  GMAC_MAC_HW_FEATURE1_RXFIFOSIZE
                  ) / 256) - 1;
  Value  = MmioRead32 (Eqos->Base + GMAC_MTL_RXQ0_OPERATION_MODE);
  Value &= ~GMAC_MTL_RXQ0_OPERATION_MODE_RQS;
  Value |= SHIFTIN (Rqs, GMAC_MTL_RXQ0_OPERATION_MODE_RQS);
  MmioWrite32 (Eqos->Base + GMAC_MTL_RXQ0_OPERATION_MODE, Value);

  //
  // Enable flow control
  //
  Value  = MmioRead32 (Eqos->Base + GMAC_MAC_Q0_TX_FLOW_CTRL);
  Value |= 0xFFFFU << GMAC_MAC_Q0_TX_FLOW_CTRL_PT_SHIFT;
  Value |= GMAC_MAC_Q0_TX_FLOW_CTRL_TFE;
  MmioWrite32 (Eqos->Base + GMAC_MAC_Q0_TX_FLOW_CTRL, Value);
  Value  = MmioRead32 (Eqos->Base + GMAC_MAC_RX_FLOW_CTRL);
  Value |= GMAC_MAC_RX_FLOW_CTRL_RFE;
  MmioWrite32 (Eqos->Base + GMAC_MAC_RX_FLOW_CTRL, Value);

  //
  // Set RX queue mode to DCB
  //
  Value = SHIFTIN (GMAC_RXQ_CTRL0_EN_DCB, GMAC_RXQ_CTRL0_EN_MASK);
  MmioWrite32 (Eqos->Base + GMAC_RXQ_CTRL0, Value);

  //
  // Configure transmitter and receiver
  //
  Value  = MmioRead32 (Eqos->Base + GMAC_MAC_CONFIGURATION);
  Value |= GMAC_MAC_CONFIGURATION_BE;
  Value |= GMAC_MAC_CONFIGURATION_DCRS;
  MmioWrite32 (Eqos->Base + GMAC_MAC_CONFIGURATION, Value);

  EqosSetMacAddress (Eqos->Base, &Eqos->SnpMode.CurrentAddress);

  return EFI_SUCCESS;
}

VOID
EqosEnableTxRx (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  UINT32  Value;

  //
  // Start transmit DMA
  //
  Value  = MmioRead32 (Eqos->Base + GMAC_DMA_CHAN0_TX_CONTROL);
  Value |= GMAC_DMA_CHAN0_TX_CONTROL_START;
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_TX_CONTROL, Value);

  //
  // Start receive DMA
  //
  Value  = MmioRead32 (Eqos->Base + GMAC_DMA_CHAN0_RX_CONTROL);
  Value |= GMAC_DMA_CHAN0_RX_CONTROL_START;
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_RX_CONTROL, Value);

  //
  // Enable transmitter and receiver
  //
  Value  = MmioRead32 (Eqos->Base + GMAC_MAC_CONFIGURATION);
  Value |= GMAC_MAC_CONFIGURATION_TE;
  Value |= GMAC_MAC_CONFIGURATION_RE;
  MmioWrite32 (Eqos->Base + GMAC_MAC_CONFIGURATION, Value);

  //
  // Enable interrupts
  //
  MmioWrite32 (
    Eqos->Base + GMAC_DMA_CHAN0_INTR_ENABLE,
    GMAC_DMA_CHAN0_STATUS_NIS |
    GMAC_DMA_CHAN0_STATUS_AIS |
    GMAC_DMA_CHAN0_STATUS_CDE |
    GMAC_DMA_CHAN0_STATUS_FBE |
    GMAC_DMA_CHAN0_STATUS_RI |
    GMAC_DMA_CHAN0_STATUS_TI
    );
}

VOID
EqosDisableTxRx (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  UINT32  Value;
  INT32   Retry;

  //
  // Disable receiver
  //
  Value  = MmioRead32 (Eqos->Base + GMAC_MAC_CONFIGURATION);
  Value &= ~GMAC_MAC_CONFIGURATION_RE;
  MmioWrite32 (Eqos->Base + GMAC_MAC_CONFIGURATION, Value);

  //
  // Stop receive DMA
  //
  Value  = MmioRead32 (Eqos->Base + GMAC_DMA_CHAN0_RX_CONTROL);
  Value &= ~GMAC_DMA_CHAN0_RX_CONTROL_START;
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_RX_CONTROL, Value);

  //
  // Stop transmit DMA
  //
  Value  = MmioRead32 (Eqos->Base + GMAC_DMA_CHAN0_TX_CONTROL);
  Value &= ~GMAC_DMA_CHAN0_TX_CONTROL_START;
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_TX_CONTROL, Value);

  //
  // Flush data in the TX FIFO
  //
  Value  = MmioRead32 (Eqos->Base + GMAC_MTL_TXQ0_OPERATION_MODE);
  Value |= GMAC_MTL_TXQ0_OPERATION_MODE_FTQ;
  MmioWrite32 (Eqos->Base + GMAC_MTL_TXQ0_OPERATION_MODE, Value);

  // Wait for flush to complete
  for (Retry = 10000; Retry > 0; Retry--) {
    Value = MmioRead32 (Eqos->Base + GMAC_MTL_TXQ0_OPERATION_MODE);
    if ((Value & GMAC_MTL_TXQ0_OPERATION_MODE_FTQ) == 0) {
      break;
    }

    gBS->Stall (1);
  }

  if (Retry == 0) {
    DEBUG ((DEBUG_ERROR, "%a: Timeout flushing TX queue!\n", __func__));
  }

  //
  // Disable transmitter
  //
  Value  = MmioRead32 (Eqos->Base + GMAC_MAC_CONFIGURATION);
  Value &= ~GMAC_MAC_CONFIGURATION_TE;
  MmioWrite32 (Eqos->Base + GMAC_MAC_CONFIGURATION, Value);

  //
  // Disable interrupts
  //
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_INTR_ENABLE, 0);
}

VOID
EqosDmaInitDescriptorRings (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  Eqos->TxQueued  = 0;
  Eqos->TxNext    = 0;
  Eqos->TxCurrent = 0;
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_TX_BASE_ADDR_HI, (UINT32)(Eqos->TxDescsPhysAddr >> 32));
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_TX_BASE_ADDR, (UINT32)(Eqos->TxDescsPhysAddr));
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_TX_RING_LEN, EQOS_TX_DESC_COUNT - 1);

  Eqos->RxCurrent = 0;
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_RX_BASE_ADDR_HI, (UINT32)(Eqos->RxDescsPhysAddr >> 32));
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_RX_BASE_ADDR, (UINT32)Eqos->RxDescsPhysAddr);
  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_RX_RING_LEN, EQOS_RX_DESC_COUNT - 1);
}

EFI_STATUS
EqosStart (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  EFI_STATUS  Status;

  Status = EqosInitialize (Eqos);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = EqosDmaMapAllRxDescriptors (Eqos);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  EqosDmaInitDescriptorRings (Eqos);

  EqosEnableTxRx (Eqos);

  return EFI_SUCCESS;
}

EFI_STATUS
EqosStop (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  EqosDisableTxRx (Eqos);

  EqosDmaUnmapAllTxDescriptors (Eqos);
  EqosDmaUnmapAllRxDescriptors (Eqos);

  return EFI_SUCCESS;
}

EFI_STATUS
EqosDmaAllocate (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  EFI_STATUS  Status;
  UINTN       BufferSize;

  Eqos->RxBuffersAddr = DMA_DEVICE_ADDRESS_LIMIT;

  BufferSize = EQOS_RX_BUFFER_SIZE * EQOS_RX_DESC_COUNT;
  Status     = gBS->AllocatePages (
                      AllocateMaxAddress,
                      EfiBootServicesData,
                      EFI_SIZE_TO_PAGES (BufferSize),
                      &Eqos->RxBuffersAddr
                      );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to allocate RX DMA buffer. Status=%r\n",
      __func__,
      Status
      ));
    Eqos->RxBuffersAddr = 0;
    return Status;
  }

  BufferSize = EQOS_TX_DESC_RING_SIZE;
  Status     = DmaAllocateBuffer (
                 EfiBootServicesData,
                 EFI_SIZE_TO_PAGES (BufferSize),
                 &Eqos->TxDescs
                 );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to allocate TX DMA descriptors buffer. Status=%r\n",
      __func__,
      Status
      ));
    goto Fail;
  }

  Status = DmaMap (
             MapOperationBusMasterCommonBuffer,
             Eqos->TxDescs,
             &BufferSize,
             &Eqos->TxDescsPhysAddr,
             &Eqos->TxDescsMap
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to map TX DMA descriptors buffer. Status=%r\n",
      __func__,
      Status
      ));
    goto Fail;
  }

  ZeroMem (Eqos->TxDescs, BufferSize);

  BufferSize = EQOS_RX_DESC_RING_SIZE;
  Status     = DmaAllocateBuffer (
                 EfiBootServicesData,
                 EFI_SIZE_TO_PAGES (BufferSize),
                 &Eqos->RxDescs
                 );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to allocate RX DMA descriptors buffer. Status=%r\n",
      __func__,
      Status
      ));
    goto Fail;
  }

  Status = DmaMap (
             MapOperationBusMasterCommonBuffer,
             Eqos->RxDescs,
             &BufferSize,
             &Eqos->RxDescsPhysAddr,
             &Eqos->RxDescsMap
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Failed to map RX DMA descriptors buffer. Status=%r\n",
      __func__,
      Status
      ));
    goto Fail;
  }

  ZeroMem (Eqos->RxDescs, BufferSize);

  return EFI_SUCCESS;

Fail:
  EqosDmaFree (Eqos);

  return Status;
}

VOID
EqosDmaFree (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  if (Eqos->RxBuffersAddr != 0) {
    gBS->FreePages (
           Eqos->RxBuffersAddr,
           EFI_SIZE_TO_PAGES (EQOS_RX_BUFFER_SIZE * EQOS_RX_DESC_COUNT)
           );
    Eqos->RxBuffersAddr = 0;
  }

  if (Eqos->TxDescs != NULL) {
    DmaUnmap (Eqos->TxDescsMap);
    DmaFreeBuffer (EFI_SIZE_TO_PAGES (EQOS_TX_DESC_RING_SIZE), Eqos->TxDescs);
    Eqos->TxDescs = NULL;
  }

  if (Eqos->RxDescs != NULL) {
    DmaUnmap (Eqos->RxDescsMap);
    DmaFreeBuffer (EFI_SIZE_TO_PAGES (EQOS_RX_DESC_RING_SIZE), Eqos->RxDescs);
    Eqos->RxDescs = NULL;
  }
}

EFI_STATUS
EqosDmaMapTxDescriptor (
  IN EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32             DescIndex,
  IN UINTN              NumberOfBytes
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  BufferPhysAddr;
  EQOS_DMA_DESCRIPTOR   *Descriptor;

  ASSERT (Eqos->TxBuffersMap[DescIndex] == NULL);
  ASSERT (Eqos->TxBuffers[DescIndex] != NULL);

  Status = DmaMap (
             MapOperationBusMasterRead,
             Eqos->TxBuffers[DescIndex],
             &NumberOfBytes,
             &BufferPhysAddr,
             &Eqos->TxBuffersMap[DescIndex]
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to map TX buffer. Status=%r\n", __func__, Status));
    return Status;
  }

  Descriptor = EQOS_DESC (Eqos->TxDescs, DescIndex);

  Descriptor->Tdes0 = (UINT32)(BufferPhysAddr);
  Descriptor->Tdes1 = (UINT32)(BufferPhysAddr >> 32);
  Descriptor->Tdes2 = EQOS_TDES2_TX_IOC | NumberOfBytes;
  MemoryFence ();
  Descriptor->Tdes3 = EQOS_TDES3_TX_OWN | EQOS_TDES3_TX_FD | EQOS_TDES3_TX_LD | NumberOfBytes;

  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_TX_END_ADDR, (UINT32)(UINTN)Descriptor);

  return EFI_SUCCESS;
}

EFI_STATUS
EqosDmaMapRxDescriptor (
  IN EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32             DescIndex
  )
{
  EFI_STATUS            Status;
  UINTN                 NumberOfBytes;
  EFI_PHYSICAL_ADDRESS  BufferPhysAddr;
  EQOS_DMA_DESCRIPTOR   *Descriptor;

  ASSERT (Eqos->RxBuffersMap[DescIndex] == NULL);
  ASSERT (Eqos->RxBuffersAddr != 0);

  NumberOfBytes = EQOS_RX_BUFFER_SIZE;

  Status = DmaMap (
             MapOperationBusMasterWrite,
             EQOS_RX_BUFFER (Eqos, DescIndex),
             &NumberOfBytes,
             &BufferPhysAddr,
             &Eqos->RxBuffersMap[DescIndex]
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to map RX buffer. Status=%r\n", __func__, Status));
    return Status;
  }

  Descriptor = EQOS_DESC (Eqos->RxDescs, DescIndex);

  Descriptor->Tdes0 = (UINT32)(BufferPhysAddr);
  Descriptor->Tdes1 = (UINT32)(BufferPhysAddr >> 32);
  Descriptor->Tdes2 = 0;
  MemoryFence ();
  Descriptor->Tdes3 = EQOS_TDES3_RX_OWN | EQOS_TDES3_RX_IOC | EQOS_TDES3_RX_BUF1V;

  MmioWrite32 (Eqos->Base + GMAC_DMA_CHAN0_RX_END_ADDR, (UINT32)(UINTN)Descriptor);

  return EFI_SUCCESS;
}

VOID
EqosDmaUnmapTxDescriptor (
  IN  EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32              DescIndex
  )
{
  if (Eqos->TxBuffersMap[DescIndex] != NULL) {
    DmaUnmap (Eqos->TxBuffersMap[DescIndex]);
    Eqos->TxBuffersMap[DescIndex] = NULL;
  }
}

VOID
EqosDmaUnmapRxDescriptor (
  IN EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32             DescIndex
  )
{
  if (Eqos->RxBuffersMap[DescIndex] != NULL) {
    DmaUnmap (Eqos->RxBuffersMap[DescIndex]);
    Eqos->RxBuffersMap[DescIndex] = NULL;
  }
}

EFI_STATUS
EqosDmaMapAllRxDescriptors (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  EFI_STATUS  Status;
  UINT32      Index;

  for (Index = 0; Index < EQOS_RX_DESC_COUNT; Index++) {
    Status = EqosDmaMapRxDescriptor (Eqos, Index);
    if (EFI_ERROR (Status)) {
      EqosDmaUnmapAllRxDescriptors (Eqos);
      return Status;
    }
  }

  return EFI_SUCCESS;
}

VOID
EqosDmaUnmapAllTxDescriptors (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  UINT32  Index;

  for (Index = 0; Index < EQOS_TX_DESC_COUNT; Index++) {
    EqosDmaUnmapTxDescriptor (Eqos, Index);
  }
}

VOID
EqosDmaUnmapAllRxDescriptors (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  UINT32  Index;

  for (Index = 0; Index < EQOS_RX_DESC_COUNT; Index++) {
    EqosDmaUnmapRxDescriptor (Eqos, Index);
  }
}

EFI_STATUS
EqosCheckTxDescriptor (
  IN EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32             DescIndex
  )
{
  EQOS_DMA_DESCRIPTOR  *Descriptor;
  UINT32               Tdes3;

  Descriptor = EQOS_DESC (Eqos->TxDescs, DescIndex);
  Tdes3      = Descriptor->Tdes3;

  if (Tdes3 & EQOS_TDES3_TX_OWN) {
    return EFI_NOT_READY;
  }

  if (Tdes3 & EQOS_TDES3_TX_DE) {
    DEBUG ((DEBUG_ERROR, "%a: Descriptor Error\n", __func__));
    return EFI_DEVICE_ERROR;
  }

  if (Tdes3 & EQOS_TDES3_TX_ES) {
    if (Tdes3 & EQOS_TDES3_TX_EUE) {
      DEBUG ((DEBUG_ERROR, "%a: ECC Uncorrectable Error Status\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_TX_JT) {
      DEBUG ((DEBUG_ERROR, "%a: Jabber Timeout\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_TX_PF) {
      DEBUG ((DEBUG_ERROR, "%a: Packet Flushed\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_TX_PCE) {
      DEBUG ((DEBUG_ERROR, "%a: Payload Checksum Error\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_TX_LOC) {
      DEBUG ((DEBUG_ERROR, "%a: Loss of Carrier\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_TX_NC) {
      DEBUG ((DEBUG_ERROR, "%a: No Carrier\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_TX_LC) {
      DEBUG ((DEBUG_ERROR, "%a: Late Collision\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_TX_EC) {
      DEBUG ((DEBUG_ERROR, "%a: Excessive Collision\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_TX_ED) {
      DEBUG ((DEBUG_ERROR, "%a: Excessive Deferral\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_TX_UF) {
      DEBUG ((DEBUG_ERROR, "%a: Underflow Error\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_TX_IHE) {
      DEBUG ((DEBUG_ERROR, "%a: IP Header Error\n", __func__));
    }

    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EqosCheckRxDescriptor (
  IN  EQOS_PRIVATE_DATA  *Eqos,
  IN  UINT32             DescIndex,
  OUT UINTN              *FrameLength
  )
{
  EQOS_DMA_DESCRIPTOR  *Descriptor;
  UINT32               Tdes2;
  UINT32               Tdes3;

  Descriptor = EQOS_DESC (Eqos->RxDescs, DescIndex);
  Tdes2      = Descriptor->Tdes2;
  Tdes3      = Descriptor->Tdes3;

  if (Tdes3 & EQOS_TDES3_RX_OWN) {
    return EFI_NOT_READY;
  }

  if (Tdes3 & EQOS_TDES3_RX_ES) {
    if (Tdes3 & EQOS_TDES3_RX_CE) {
      DEBUG ((DEBUG_ERROR, "%a: CRC Error\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_RX_GP) {
      DEBUG ((DEBUG_ERROR, "%a: Giant Packet\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_RX_RWT) {
      DEBUG ((DEBUG_ERROR, "%a: Receive Watchdog Timeout\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_RX_OE) {
      DEBUG ((DEBUG_ERROR, "%a: Overflow Error\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_RX_RE) {
      DEBUG ((DEBUG_ERROR, "%a: Receive Error\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_RX_DE) {
      DEBUG ((DEBUG_ERROR, "%a: Dribble Bit Error\n", __func__));
    }

    if (Tdes3 & EQOS_TDES3_RX_OE) {
      DEBUG ((DEBUG_ERROR, "%a: Overflow Error\n", __func__));
    }

    if (Tdes2 & EQOS_TDES2_RX_DAF) {
      DEBUG ((DEBUG_ERROR, "%a: Destination Address Filter Fail\n", __func__));
    }

    if (Tdes2 & EQOS_TDES2_RX_SAF) {
      DEBUG ((DEBUG_ERROR, "%a: Source Address Filter Fail\n", __func__));
    }

    return EFI_DEVICE_ERROR;
  }

  *FrameLength = Tdes3 & EQOS_TDES3_RX_LENGTH_MASK;
  if (*FrameLength == 0) {
    DEBUG ((DEBUG_ERROR, "%a: Invalid Frame Length\n", __func__));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

VOID
EqosGetDmaInterruptStatus (
  IN  EQOS_PRIVATE_DATA  *Eqos,
  OUT UINT32             *InterruptStatus  OPTIONAL
  )
{
  UINT32  DmaStatus;
  UINT32  Mask;

  if (InterruptStatus != NULL) {
    *InterruptStatus = 0;
  }

  DmaStatus = MmioRead32 (Eqos->Base + GMAC_DMA_CHAN0_STATUS);
  Mask      = 0;

  if (DmaStatus & GMAC_DMA_CHAN0_STATUS_NIS) {
    Mask |= GMAC_DMA_CHAN0_STATUS_NIS;

    if (DmaStatus & GMAC_DMA_CHAN0_STATUS_RI) {
      if (InterruptStatus != NULL) {
        Mask            |= GMAC_DMA_CHAN0_STATUS_RI;
        *InterruptStatus = EFI_SIMPLE_NETWORK_RECEIVE_INTERRUPT;
      }
    }

    if (DmaStatus & GMAC_DMA_CHAN0_STATUS_TI) {
      if (InterruptStatus != NULL) {
        Mask            |= GMAC_DMA_CHAN0_STATUS_TI;
        *InterruptStatus = EFI_SIMPLE_NETWORK_TRANSMIT_INTERRUPT;
      }
    }
  }

  if (DmaStatus & GMAC_DMA_CHAN0_STATUS_AIS) {
    Mask |= GMAC_DMA_CHAN0_STATUS_AIS;

    if (DmaStatus & GMAC_DMA_CHAN0_STATUS_CDE) {
      Mask |= GMAC_DMA_CHAN0_STATUS_CDE;
      DEBUG ((DEBUG_ERROR, "%a: Context Descriptor Error\n", __func__));
    }

    if (DmaStatus & GMAC_DMA_CHAN0_STATUS_FBE) {
      Mask |= GMAC_DMA_CHAN0_STATUS_FBE;
      DEBUG ((DEBUG_ERROR, "%a: Fatal Bus Error\n", __func__));

      if (DmaStatus & GMAC_DMA_CHAN0_STATUS_REB_DATA_TRANS) {
        DEBUG ((DEBUG_ERROR, "%a: Rx DMA Error Bits: Error during data transfer by Rx DMA\n", __func__));
      }

      if (DmaStatus & GMAC_DMA_CHAN0_STATUS_REB_DESC_ACC) {
        DEBUG ((DEBUG_ERROR, "%a: Rx DMA Error Bits: Error during descriptor access\n", __func__));
      } else {
        DEBUG ((DEBUG_ERROR, "%a: Rx DMA Error Bits: Error during data buffer access\n", __func__));
      }

      if (DmaStatus & GMAC_DMA_CHAN0_STATUS_REB_READ_TRANS) {
        DEBUG ((DEBUG_ERROR, "%a: Rx DMA Error Bits: Error during read transfer\n", __func__));
      } else {
        DEBUG ((DEBUG_ERROR, "%a: Rx DMA Error Bits: Error during write transfer\n", __func__));
      }

      if (DmaStatus & GMAC_DMA_CHAN0_STATUS_TEB_DATA_TRANS) {
        DEBUG ((DEBUG_ERROR, "%a: Tx DMA Error Bits: Error during data transfer by Tx DMA\n", __func__));
      }

      if (DmaStatus & GMAC_DMA_CHAN0_STATUS_TEB_DESC_ACC) {
        DEBUG ((DEBUG_ERROR, "%a: Tx DMA Error Bits: Error during descriptor access\n", __func__));
      } else {
        DEBUG ((DEBUG_ERROR, "%a: Tx DMA Error Bits: Error during data buffer access\n", __func__));
      }

      if (DmaStatus & GMAC_DMA_CHAN0_STATUS_TEB_READ_TRANS) {
        DEBUG ((DEBUG_ERROR, "%a: Tx DMA Error Bits: Error during read transfer\n", __func__));
      } else {
        DEBUG ((DEBUG_ERROR, "%a: Tx DMA Error Bits: Error during write transfer\n", __func__));
      }
    }
  }

  MmioOr32 (Eqos->Base + GMAC_DMA_CHAN0_STATUS, Mask);
}

VOID
EqosGetPhyIfStatus (
  IN  EQOS_PRIVATE_DATA  *Eqos,
  OUT BOOLEAN            *LinkUp      OPTIONAL,
  OUT UINT32             *Speed       OPTIONAL,
  OUT BOOLEAN            *FullDuplex  OPTIONAL
  )
{
  UINT32  PhyIfStatus;

  PhyIfStatus = MmioRead32 (Eqos->Base + GMAC_MAC_PHYIF_CONTROL_STATUS);

  if (LinkUp != NULL) {
    *LinkUp = (PhyIfStatus & GMAC_MAC_PHYIF_CONTROL_STATUS_LNKSTS) != 0;
  }

  if (FullDuplex != NULL) {
    *FullDuplex = (PhyIfStatus & GMAC_MAC_PHYIF_CONTROL_STATUS_LNKMOD) != 0;
  }

  if (Speed != NULL) {
    switch (SHIFTOUT (PhyIfStatus, GMAC_MAC_PHYIF_CONTROL_STATUS_LNKSPEED)) {
      case GMAC_MAC_PHYIF_CONTROL_STATUS_LNKSPEED_2_5:
        *Speed = 10;
        break;
      case GMAC_MAC_PHYIF_CONTROL_STATUS_LNKSPEED_25:
        *Speed = 100;
        break;
      case GMAC_MAC_PHYIF_CONTROL_STATUS_LNKSPEED_125:
        *Speed = 1000;
        break;
      default:
        ASSERT (FALSE);
        *Speed = 0;
        break;
    }
  }
}

VOID
EqosSetLinkConfig (
  IN EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32             Speed,
  IN BOOLEAN            FullDuplex
  )
{
  UINT32  Config;

  Config = MmioRead32 (Eqos->Base + GMAC_MAC_CONFIGURATION);

  switch (Speed) {
    case 10:
      Config |= GMAC_MAC_CONFIGURATION_PS;
      Config &= ~GMAC_MAC_CONFIGURATION_FES;
      break;
    case 100:
      Config |= GMAC_MAC_CONFIGURATION_PS;
      Config |= GMAC_MAC_CONFIGURATION_FES;
      break;
    case 1000:
      Config &= ~GMAC_MAC_CONFIGURATION_PS;
      Config &= ~GMAC_MAC_CONFIGURATION_FES;
      break;
    default:
      ASSERT (FALSE);
      break;
  }

  if (FullDuplex) {
    Config |= GMAC_MAC_CONFIGURATION_DM;
  } else {
    Config &= ~GMAC_MAC_CONFIGURATION_DM;
  }

  MmioWrite32 (Eqos->Base + GMAC_MAC_CONFIGURATION, Config);
}

EFI_STATUS
EqosUpdateLink (
  IN EQOS_PRIVATE_DATA  *Eqos
  )
{
  EFI_STATUS                         Status;
  BOOLEAN                            LinkUp;
  UINT32                             Speed;
  BOOLEAN                            FullDuplex;
  DWC_EQOS_PLATFORM_DEVICE_PROTOCOL  *Platform;

  EqosGetPhyIfStatus (Eqos, &LinkUp, &Speed, &FullDuplex);

  if (Eqos->SnpMode.MediaPresent != LinkUp) {
    if (LinkUp) {
      DEBUG ((DEBUG_VERBOSE, "%a: Link is up\n", __func__));

      EqosSetLinkConfig (Eqos, Speed, FullDuplex);

      Platform = Eqos->Platform;
      if (Platform->SetInterfaceSpeed != NULL) {
        Status = Platform->SetInterfaceSpeed (Platform, Speed);
        if (EFI_ERROR (Status)) {
          DEBUG ((
            DEBUG_ERROR,
            "%a: Failed to set platform interface speed to %u. Status=%r\n",
            __func__,
            Speed,
            Status
            ));
        }
      }
    } else {
      DEBUG ((DEBUG_VERBOSE, "%a: Link is down\n", __func__));
    }

    Eqos->SnpMode.MediaPresent = LinkUp;
  }

  return LinkUp ? EFI_SUCCESS : EFI_NOT_READY;
}

EFI_STATUS
EqosSetRxFilters (
  IN EQOS_PRIVATE_DATA  *Eqos,
  IN UINT32             ReceiveFilterSetting,
  IN BOOLEAN            ResetMCastFilter,
  IN UINTN              MCastFilterCnt        OPTIONAL,
  IN EFI_MAC_ADDRESS    *MCastFilter          OPTIONAL
  )
{
  UINT32  Index;
  UINT32  PacketFilter;
  UINT32  Crc;
  UINT32  HashReg;
  UINT32  HashBit;
  UINT32  Hash;

  if (ResetMCastFilter) {
    for (HashReg = 0; HashReg < GMAC_MAC_HASH_TABLE_COUNT; HashReg++) {
      MmioWrite32 (Eqos->Base + GMAC_MAC_HASH_TABLE_REG (HashReg), 0x0);
    }
  }

  PacketFilter = MmioRead32 (Eqos->Base + GMAC_MAC_PACKET_FILTER);

  PacketFilter &= ~(GMAC_MAC_PACKET_FILTER_PCF_MASK |
                    GMAC_MAC_PACKET_FILTER_DBF |
                    GMAC_MAC_PACKET_FILTER_PM |
                    GMAC_MAC_PACKET_FILTER_HMC |
                    GMAC_MAC_PACKET_FILTER_PR);

  if (ReceiveFilterSetting & EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS) {
    PacketFilter |= GMAC_MAC_PACKET_FILTER_PR |
                    GMAC_MAC_PACKET_FILTER_PCF_ALL;
  } else if (ReceiveFilterSetting & EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS_MULTICAST) {
    PacketFilter |= GMAC_MAC_PACKET_FILTER_PM;
  } else if (ReceiveFilterSetting & EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST) {
    PacketFilter |= GMAC_MAC_PACKET_FILTER_HMC;

    for (Index = 0; Index < MCastFilterCnt; Index++) {
      Crc     = EqosEtherCrc32Le (MCastFilter[Index].Addr, NET_ETHER_ADDR_LEN);
      Crc    &= 0x7f;
      Crc     = EqosBitReverse32 (~Crc) >> 26;
      HashReg = Crc >> 5;
      HashBit = 1 << (Crc & 0x1f);

      Hash  = MmioRead32 (Eqos->Base + GMAC_MAC_HASH_TABLE_REG (HashReg));
      Hash |= HashBit;
      MmioWrite32 (Eqos->Base + GMAC_MAC_HASH_TABLE_REG (HashReg), Hash);
    }
  }

  if ((ReceiveFilterSetting & EFI_SIMPLE_NETWORK_RECEIVE_BROADCAST) == 0) {
    PacketFilter |= GMAC_MAC_PACKET_FILTER_DBF;
  }

  MmioWrite32 (Eqos->Base + GMAC_MAC_PACKET_FILTER, PacketFilter);

  return EFI_SUCCESS;
}

VOID
EqosGetMacAddress (
  IN  EFI_PHYSICAL_ADDRESS  Base,
  OUT EFI_MAC_ADDRESS       *MacAddress
  )
{
  UINT32  AddrLow;
  UINT32  AddrHigh;

  AddrLow  = MmioRead32 (Base + GMAC_MAC_ADDRESS0_LOW);
  AddrHigh = MmioRead32 (Base + GMAC_MAC_ADDRESS0_HIGH);

  MacAddress->Addr[0] = AddrLow & 0xff;
  MacAddress->Addr[1] = (AddrLow >> 8) & 0xff;
  MacAddress->Addr[2] = (AddrLow >> 16) & 0xff;
  MacAddress->Addr[3] = (AddrLow >> 24) & 0xff;
  MacAddress->Addr[4] = AddrHigh & 0xff;
  MacAddress->Addr[5] = (AddrHigh >> 8) & 0xff;
}

VOID
EqosSetMacAddress (
  IN EFI_PHYSICAL_ADDRESS  Base,
  IN EFI_MAC_ADDRESS       *MacAddress
  )
{
  UINT32  AddrLow;
  UINT32  AddrHigh;

  AddrLow   = MacAddress->Addr[0];
  AddrLow  |= MacAddress->Addr[1] << 8;
  AddrLow  |= MacAddress->Addr[2] << 16;
  AddrLow  |= MacAddress->Addr[3] << 24;
  AddrHigh  = MacAddress->Addr[4];
  AddrHigh |= MacAddress->Addr[5] << 8;
  AddrHigh |= GMAC_MAC_ADDRESS0_HIGH_AE;

  MmioWrite32 (Base + GMAC_MAC_ADDRESS0_LOW, AddrLow);
  MmioWrite32 (Base + GMAC_MAC_ADDRESS0_HIGH, AddrHigh);
}

UINT32
EqosEtherCrc32Le (
  IN UINT8  *Buffer,
  IN UINTN  Length
  )
{
  STATIC CONST UINT32  CrcTable[] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };

  UINT32  Crc;
  UINTN   Index;

  Crc = 0xffffffffU;

  for (Index = 0; Index < Length; Index++) {
    Crc ^= Buffer[Index];
    Crc  = (Crc >> 4) ^ CrcTable[Crc & 0xf];
    Crc  = (Crc >> 4) ^ CrcTable[Crc & 0xf];
  }

  return (Crc);
}

UINT32
EqosBitReverse32 (
  IN UINT32  Value
  )
{
  Value = (((Value & 0xaaaaaaaa) >> 1) | ((Value & 0x55555555) << 1));
  Value = (((Value & 0xcccccccc) >> 2) | ((Value & 0x33333333) << 2));
  Value = (((Value & 0xf0f0f0f0) >> 4) | ((Value & 0x0f0f0f0f) << 4));
  Value = (((Value & 0xff00ff00) >> 8) | ((Value & 0x00ff00ff) << 8));

  return (Value >> 16) | (Value << 16);
}
