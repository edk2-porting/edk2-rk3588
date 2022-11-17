/** @file

  Copyright (c) 2016 Socionext Inc. All rights reserved.<BR>
  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>
  Copyright (c) 2020, Arm Limited. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "NetsecDxe.h"

EFI_CPU_ARCH_PROTOCOL      *mCpu;

STATIC
VOID
GetCurrentMacAddress (
  IN  UINT64        EepromBase,
  OUT UINT8         *Mac)
{
  Mac[0] = MmioRead8 (EepromBase + MAC_ADDRESS + 3);
  Mac[1] = MmioRead8 (EepromBase + MAC_ADDRESS + 2);
  Mac[2] = MmioRead8 (EepromBase + MAC_ADDRESS + 1);
  Mac[3] = MmioRead8 (EepromBase + MAC_ADDRESS + 0);
  Mac[4] = MmioRead8 (EepromBase + MAC_ADDRESS + 7);
  Mac[5] = MmioRead8 (EepromBase + MAC_ADDRESS + 6);
}

/*
 *  Probe()
 */
STATIC
EFI_STATUS
Probe (
  IN  EFI_HANDLE          Handle,
  IN  NETSEC_DRIVER       *LanDriver
  )
{
  ogma_param_t  Param;
  ogma_err_t    ogma_err;
  UINT64        dmac_hm_cmd_base, dmac_mh_cmd_base, core_cmd_base;
  UINT32        dmac_hm_cmd_size, dmac_mh_cmd_size, core_cmd_size;
  UINT64        EepromBase;

  SetMem (&Param, sizeof (Param), 0);

  Param.use_gmac_flag = OGMA_TRUE;

  Param.use_jumbo_pkt_flag = FixedPcdGet8 (PcdJumboPacket);

  Param.desc_ring_param[OGMA_DESC_RING_ID_NRM_TX].valid_flag = OGMA_TRUE;
  Param.desc_ring_param[OGMA_DESC_RING_ID_NRM_TX].little_endian_flag = OGMA_TRUE;
  Param.desc_ring_param[OGMA_DESC_RING_ID_NRM_TX].tmr_mode_flag = OGMA_FALSE;
  Param.desc_ring_param[OGMA_DESC_RING_ID_NRM_TX].entry_num = FixedPcdGet16 (PcdEncTxDescNum);
  Param.desc_ring_param[OGMA_DESC_RING_ID_NRM_RX].valid_flag = OGMA_TRUE;
  Param.desc_ring_param[OGMA_DESC_RING_ID_NRM_RX].little_endian_flag = OGMA_TRUE;
  Param.desc_ring_param[OGMA_DESC_RING_ID_NRM_RX].tmr_mode_flag = OGMA_FALSE;
  Param.desc_ring_param[OGMA_DESC_RING_ID_NRM_RX].entry_num = FixedPcdGet16 (PcdDecRxDescNum);

  // phy-interface
  Param.gmac_config.phy_interface = OGMA_PHY_INTERFACE_RGMII;

  Param.phy_addr = LanDriver->Dev->Resources[2].AddrRangeMin;

  // Read and save the Permanent MAC Address
  EepromBase = LanDriver->Dev->Resources[1].AddrRangeMin;
  GetCurrentMacAddress (EepromBase, LanDriver->SnpMode.PermanentAddress.Addr);

  LanDriver->SnpMode.CurrentAddress = LanDriver->SnpMode.PermanentAddress;
  DEBUG ((DEBUG_NET | DEBUG_INFO,
    "Netsec: HW MAC Address: %02x-%02x-%02x-%02x-%02x-%02x\n",
    LanDriver->SnpMode.PermanentAddress.Addr[0],
    LanDriver->SnpMode.PermanentAddress.Addr[1],
    LanDriver->SnpMode.PermanentAddress.Addr[2],
    LanDriver->SnpMode.PermanentAddress.Addr[3],
    LanDriver->SnpMode.PermanentAddress.Addr[4],
    LanDriver->SnpMode.PermanentAddress.Addr[5]));

  //
  // The NETSEC microcode for the core engine and the TX and RX engines
  // is loaded from a separate memory mapped ROM.
  //

  // Get hm microcode's physical addresses
  dmac_hm_cmd_base = MmioRead32 (EepromBase + HM_ME_ADDRESS_H);
  dmac_hm_cmd_base <<= 32;
  dmac_hm_cmd_base |= MmioRead32 (EepromBase + HM_ME_ADDRESS_L);
  dmac_hm_cmd_size = MmioRead32 (EepromBase + HM_ME_SIZE);

  // Get mh microcode's physical addresses
  dmac_mh_cmd_base = MmioRead32 (EepromBase + MH_ME_ADDRESS_H);
  dmac_mh_cmd_base <<= 32;
  dmac_mh_cmd_base |= MmioRead32 (EepromBase + MH_ME_ADDRESS_L);
  dmac_mh_cmd_size = MmioRead32 (EepromBase + MH_ME_SIZE);

  // Get core microcode's physical addresses
  core_cmd_base = MmioRead32 (EepromBase + PACKET_ME_ADDRESS);
  core_cmd_size = MmioRead32 (EepromBase + PACKET_ME_SIZE);

  ogma_err = ogma_init (
               (VOID *)(UINTN)LanDriver->Dev->Resources[0].AddrRangeMin,
               Handle, &Param,
               (VOID *)(UINTN)dmac_hm_cmd_base, dmac_hm_cmd_size,
               (VOID *)(UINTN)dmac_mh_cmd_base, dmac_mh_cmd_size,
               (VOID *)(UINTN)core_cmd_base, core_cmd_size,
               &LanDriver->Handle);
  if (ogma_err != OGMA_ERR_OK) {
    DEBUG ((DEBUG_ERROR, "NETSEC: ogma_init() failed with error code %d\n",
      ogma_err));
    return EFI_DEVICE_ERROR;
  }

  ogma_enable_top_irq (LanDriver->Handle,
                       OGMA_TOP_IRQ_REG_NRM_RX | OGMA_TOP_IRQ_REG_NRM_TX);

  return EFI_SUCCESS;
}

/*
 *  UEFI Stop() function
 */
STATIC
EFI_STATUS
EFIAPI
SnpStop (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL    *Snp
  )
{
  NETSEC_DRIVER           *LanDriver;
  EFI_TPL                 SavedTpl;
  EFI_STATUS              Status;

  // Check Snp Instance
  if (Snp == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Serialize access to data and registers
  SavedTpl = gBS->RaiseTPL (TPL_CALLBACK);

  LanDriver = INSTANCE_FROM_SNP_THIS (Snp);

  // Check state of the driver
  switch (Snp->Mode->State) {
  case EfiSimpleNetworkStarted:
  case EfiSimpleNetworkInitialized:
    break;
  case EfiSimpleNetworkStopped:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver not started\n"));
    ReturnUnlock (EFI_NOT_STARTED);
  default:
    DEBUG ((DEBUG_ERROR, "NETSEC: Driver in an invalid state: %u\n",
      (UINTN)Snp->Mode->State));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  gBS->CloseEvent (LanDriver->ExitBootEvent);

  gBS->SetTimer (LanDriver->PhyStatusEvent, TimerCancel, 0);
  gBS->CloseEvent (LanDriver->PhyStatusEvent);
  LanDriver->PhyStatusEvent = NULL;

  // Change the state
  Snp->Mode->State = EfiSimpleNetworkStopped;
  Status = EFI_SUCCESS;

  // Restore TPL and return
ExitUnlock:
  gBS->RestoreTPL (SavedTpl);
  return Status;
}

EFI_STATUS
EFIAPI
NetsecUpdateLink (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL    *Snp
  )
{
  NETSEC_DRIVER                 *LanDriver;
  ogma_phy_link_status_t        phy_link_status;
  ogma_gmac_mode_t              ogma_gmac_mode;
  ogma_err_t                    ogma_err;
  BOOLEAN                       ValidFlag;
  ogma_gmac_mode_t              GmacMode;
  BOOLEAN                       RxRunningFlag;
  BOOLEAN                       TxRunningFlag;
  EFI_STATUS                    ErrorStatus;

  LanDriver = INSTANCE_FROM_SNP_THIS (Snp);

  // Update the media status
  ogma_err = ogma_get_phy_link_status (LanDriver->Handle,
               &phy_link_status);
  if (ogma_err != OGMA_ERR_OK) {
    DEBUG ((DEBUG_ERROR,
      "NETSEC: ogma_get_phy_link_status failed with error code: %d\n",
      (INT32)ogma_err));
    ErrorStatus = EFI_DEVICE_ERROR;
    goto Fail;
  }

  // Update the GMAC status
  ogma_err = ogma_get_gmac_status (LanDriver->Handle, &ValidFlag, &GmacMode,
               &RxRunningFlag, &TxRunningFlag);
  if (ogma_err != OGMA_ERR_OK) {
    DEBUG ((DEBUG_ERROR,
      "NETSEC: ogma_get_gmac_status failed with error code: %d\n",
      (INT32)ogma_err));
    ErrorStatus = EFI_DEVICE_ERROR;
    goto Fail;
  }

  // Stop GMAC when GMAC is running and physical link is down
  if (RxRunningFlag && TxRunningFlag && !phy_link_status.up_flag) {
    ogma_err = ogma_stop_gmac (LanDriver->Handle, OGMA_TRUE, OGMA_TRUE);
    if (ogma_err != OGMA_ERR_OK) {
      DEBUG ((DEBUG_ERROR,
        "NETSEC: ogma_stop_gmac() failed with error status %d\n",
        ogma_err));
      ErrorStatus = EFI_DEVICE_ERROR;
      goto Fail;
    }
  }

  // Start GMAC when GMAC is stopped and physical link is up
  if (!RxRunningFlag && !TxRunningFlag && phy_link_status.up_flag) {
    ZeroMem (&ogma_gmac_mode, sizeof (ogma_gmac_mode_t));
    ogma_gmac_mode.link_speed = phy_link_status.link_speed;
    ogma_gmac_mode.half_duplex_flag = (ogma_bool)phy_link_status.half_duplex_flag;
    if (!phy_link_status.half_duplex_flag && FixedPcdGet8 (PcdFlowCtrl)) {
      ogma_gmac_mode.flow_ctrl_enable_flag     = FixedPcdGet8 (PcdFlowCtrl);
      ogma_gmac_mode.flow_ctrl_start_threshold = FixedPcdGet16 (PcdFlowCtrlStartThreshold);
      ogma_gmac_mode.flow_ctrl_stop_threshold  = FixedPcdGet16 (PcdFlowCtrlStopThreshold);
      ogma_gmac_mode.pause_time                = FixedPcdGet16 (PcdPauseTime);
    }

    ogma_err = ogma_set_gmac_mode (LanDriver->Handle, &ogma_gmac_mode);
    if (ogma_err != OGMA_ERR_OK) {
      DEBUG ((DEBUG_ERROR,
        "NETSEC: ogma_set_gmac() failed with error status %d\n",
        (INT32)ogma_err));
      ErrorStatus = EFI_DEVICE_ERROR;
      goto Fail;
    }

    ogma_err = ogma_start_gmac (LanDriver->Handle, OGMA_TRUE, OGMA_TRUE);
    if (ogma_err != OGMA_ERR_OK) {
      DEBUG ((DEBUG_ERROR,
        "NETSEC: ogma_start_gmac() failed with error status %d\n",
        (INT32)ogma_err));
      ErrorStatus = EFI_DEVICE_ERROR;
      goto Fail;
    }
  }

  /* Updating link status for external guery */
  Snp->Mode->MediaPresent = phy_link_status.up_flag;
  return EFI_SUCCESS;

Fail:
  Snp->Mode->MediaPresent = FALSE;
  return ErrorStatus;
}

/*
 *  UEFI Initialize() function
 */
STATIC
EFI_STATUS
EFIAPI
SnpInitialize (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL   *Snp,
  IN  UINTN                         RxBufferSize  OPTIONAL,
  IN  UINTN                         TxBufferSize  OPTIONAL
  )
{
  NETSEC_DRIVER           *LanDriver;
  EFI_TPL                 SavedTpl;
  EFI_STATUS              Status;

  ogma_err_t              ogma_err;

  // Check Snp Instance
  if (Snp == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Serialize access to data and registers
  SavedTpl = gBS->RaiseTPL (TPL_CALLBACK);

  // Check that driver was started but not initialised
  switch (Snp->Mode->State) {
  case EfiSimpleNetworkStarted:
    break;
  case EfiSimpleNetworkInitialized:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver already initialized\n"));
    ReturnUnlock (EFI_SUCCESS);
  case EfiSimpleNetworkStopped:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver not started\n"));
    ReturnUnlock (EFI_NOT_STARTED);
  default:
    DEBUG ((DEBUG_ERROR, "NETSEC: Driver in an invalid state: %u\n",
      (UINTN)Snp->Mode->State));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  // Find the LanDriver structure
  LanDriver = INSTANCE_FROM_SNP_THIS (Snp);

  // Clean all descriptors on the RX ring.
  ogma_err = ogma_clean_rx_desc_ring (LanDriver->Handle,
                                      OGMA_DESC_RING_ID_NRM_RX);
  if (ogma_err != OGMA_ERR_OK) {
    DEBUG ((DEBUG_ERROR,
      "NETSEC: ogma_clean_rx_desc_ring() failed with error code %d\n",
      (INT32)ogma_err));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  ogma_err = ogma_clean_tx_desc_ring (LanDriver->Handle,
                                      OGMA_DESC_RING_ID_NRM_TX);
  if (ogma_err != OGMA_ERR_OK) {
    DEBUG ((DEBUG_ERROR,
      "NETSEC: ogma_clean_tx_desc_ring() failed with error code %d\n",
      (INT32)ogma_err));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  ogma_clear_desc_ring_irq_status (LanDriver->Handle, OGMA_DESC_RING_ID_NRM_TX,
                                   OGMA_CH_IRQ_REG_EMPTY);

  // Start the RX queue
  ogma_err = ogma_start_desc_ring (LanDriver->Handle, OGMA_DESC_RING_ID_NRM_RX);
  if (ogma_err != OGMA_ERR_OK) {
    DEBUG ((DEBUG_ERROR,
      "NETSEC: ogma_start_desc_ring(ring_id=%d) failed with error code %d\n",
      OGMA_DESC_RING_ID_NRM_RX,
      (INT32)ogma_err));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  ogma_err = ogma_set_irq_coalesce_param (LanDriver->Handle,
                                          OGMA_DESC_RING_ID_NRM_RX,
                                          RXINT_PKTCNT, OGMA_FALSE,
                                          RXINT_TMR_CNT_US);
  if (ogma_err != OGMA_ERR_OK) {
    DEBUG ((DEBUG_ERROR,
      "NETSEC: ogma_set_irq_coalesce_param() failed with error code %d\n",
      (INT32)ogma_err));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  ogma_err = ogma_start_desc_ring (LanDriver->Handle, OGMA_DESC_RING_ID_NRM_TX);
  if (ogma_err != OGMA_ERR_OK) {
    DEBUG ((DEBUG_ERROR,
      "NETSEC: ogma_start_desc_ring(ring_id=%d) failed with error code %d\n",
      OGMA_DESC_RING_ID_NRM_TX,
      (INT32)ogma_err));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  ogma_disable_desc_ring_irq (LanDriver->Handle, OGMA_DESC_RING_ID_NRM_TX,
                              OGMA_CH_IRQ_REG_EMPTY);

  // Declare the driver as initialized
  Snp->Mode->State = EfiSimpleNetworkInitialized;
  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO | DEBUG_LOAD, "NETSEC: Driver started\n"));

  // Restore TPL and return
ExitUnlock:
  gBS->RestoreTPL (SavedTpl);
  return Status;
}

/*
 *  UEFI Shutdown () function
 */
STATIC
EFI_STATUS
EFIAPI
SnpShutdown (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL    *Snp
  )
{
  NETSEC_DRIVER     *LanDriver;
  EFI_TPL           SavedTpl;
  EFI_STATUS        Status;

  // Check Snp Instance
  if (Snp == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Serialize access to data and registers
  SavedTpl = gBS->RaiseTPL (TPL_CALLBACK);

  // First check that driver has already been initialized
  switch (Snp->Mode->State) {
  case EfiSimpleNetworkInitialized:
    break;
  case EfiSimpleNetworkStarted:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver not yet initialized\n"));
    ReturnUnlock (EFI_DEVICE_ERROR);
  case EfiSimpleNetworkStopped:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver in stopped state\n"));
    ReturnUnlock (EFI_NOT_STARTED);
  default:
    DEBUG ((DEBUG_ERROR, "NETSEC: Driver in an invalid state: %u\n",
      (UINTN)Snp->Mode->State));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  // Find the LanDriver structure
  LanDriver = INSTANCE_FROM_SNP_THIS (Snp);

  ogma_stop_gmac (LanDriver->Handle, OGMA_TRUE, OGMA_TRUE);

  ogma_stop_desc_ring (LanDriver->Handle, OGMA_DESC_RING_ID_NRM_RX);
  ogma_stop_desc_ring (LanDriver->Handle, OGMA_DESC_RING_ID_NRM_TX);

  Snp->Mode->State = EfiSimpleNetworkStarted;
  Status = EFI_SUCCESS;

  // Restore TPL and return
ExitUnlock:
  gBS->RestoreTPL (SavedTpl);
  return Status;
}

STATIC
VOID
EFIAPI
NotifyExitBoot (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_SIMPLE_NETWORK_PROTOCOL     *Snp;
  EFI_STATUS                      Status;

  Snp = Context;

  if (Snp->Mode != EfiSimpleNetworkStopped) {
    Status = SnpShutdown (Snp);
    if (!EFI_ERROR (Status)) {
      SnpStop (Snp);
    }
  }
  gBS->CloseEvent (Event);
}

/**
  Polling function to check the physical link status with GMAC

  @param[in]  Timer              Event
  @param[in]  Context            Pointer to the Snp structure

**/
STATIC
VOID
EFIAPI
NetsecPollPhyStatus (
  IN EFI_EVENT    Timer,
  IN VOID         *Context
  )
{
  EFI_SIMPLE_NETWORK_PROTOCOL   *Snp;

  Snp = (EFI_SIMPLE_NETWORK_PROTOCOL *)Context;
  if (Snp == NULL) {
    DEBUG((DEBUG_ERROR, "NETSEC: PollPhyStatus() invalid Snp\n"));
    return;
  }

  NetsecUpdateLink (Snp);
}

/*
 *  UEFI Start() function
 */
STATIC
EFI_STATUS
EFIAPI
SnpStart (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL    *Snp
  )
{
  EFI_SIMPLE_NETWORK_MODE   *Mode;
  EFI_TPL                   SavedTpl;
  EFI_STATUS                Status;
  NETSEC_DRIVER             *LanDriver;

  LanDriver = INSTANCE_FROM_SNP_THIS (Snp);

  // Check Snp instance
  if (Snp == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Serialize access to data and registers
  SavedTpl = gBS->RaiseTPL (TPL_CALLBACK);
  Mode = Snp->Mode;

  // Check state of the driver
  switch (Mode->State) {
  case EfiSimpleNetworkStopped:
    break;
  case EfiSimpleNetworkStarted:
  case EfiSimpleNetworkInitialized:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver already started\n"));
    ReturnUnlock (EFI_ALREADY_STARTED);
  default:
    DEBUG ((DEBUG_ERROR, "NETSEC: Driver in an invalid state: %u\n",
      (UINTN)Snp->Mode->State));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  Status = gBS->CreateEvent (EVT_SIGNAL_EXIT_BOOT_SERVICES, TPL_CALLBACK,
                  NotifyExitBoot, Snp, &LanDriver->ExitBootEvent);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEvent (EVT_TIMER | EVT_NOTIFY_SIGNAL, TPL_CALLBACK,
                  NetsecPollPhyStatus, Snp, &LanDriver->PhyStatusEvent);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->SetTimer (
                  LanDriver->PhyStatusEvent,
                  TimerPeriodic,
                  NETSEC_PHY_STATUS_POLL_INTERVAL
                  );
  ASSERT_EFI_ERROR (Status);

  // Change state
  Mode->State = EfiSimpleNetworkStarted;
  Status = EFI_SUCCESS;

  // Restore TPL and return
ExitUnlock:
  gBS->RestoreTPL (SavedTpl);
  return Status;

}

/*
 *  UEFI ReceiveFilters() function
 */
STATIC
EFI_STATUS
EFIAPI
SnpReceiveFilters (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL     *Snp,
  IN  UINT32                          Enable,
  IN  UINT32                          Disable,
  IN  BOOLEAN                         Reset,
  IN  UINTN                           NumMfilter  OPTIONAL,
  IN  EFI_MAC_ADDRESS                 *Mfilter    OPTIONAL
  )
{
  EFI_TPL             SavedTpl;
  EFI_STATUS          Status;

  // Check Snp Instance
  if (Snp == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Serialize access to data and registers
  SavedTpl = gBS->RaiseTPL (TPL_CALLBACK);

  // First check that driver has already been initialized
  switch (Snp->Mode->State) {
  case EfiSimpleNetworkInitialized:
    break;
  case EfiSimpleNetworkStarted:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver not yet initialized\n"));
    ReturnUnlock (EFI_DEVICE_ERROR);
  case EfiSimpleNetworkStopped:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver not started\n"));
    ReturnUnlock (EFI_NOT_STARTED);
  default:
    DEBUG ((DEBUG_ERROR, "NETSEC: Driver in an invalid state: %u\n",
      (UINTN)Snp->Mode->State));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  Status = EFI_SUCCESS;

  // Restore TPL and return
ExitUnlock:
  gBS->RestoreTPL (SavedTpl);
  return Status;
}

/*
 *  UEFI GetStatus () function
 */
STATIC
EFI_STATUS
EFIAPI
SnpGetStatus (
  IN      EFI_SIMPLE_NETWORK_PROTOCOL   *Snp,
      OUT UINT32                        *IrqStat  OPTIONAL,
      OUT VOID                          **TxBuff  OPTIONAL
  )
{
  NETSEC_DRIVER             *LanDriver;
  EFI_TPL                   SavedTpl;
  EFI_STATUS                Status;
  pfdep_pkt_handle_t        pkt_handle;
  LIST_ENTRY                *Link;

  ogma_err_t              ogma_err;

  // Check preliminaries
  if (Snp == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Serialize access to data and registers
  SavedTpl = gBS->RaiseTPL (TPL_CALLBACK);

  // Check that driver was started and initialised
  switch (Snp->Mode->State) {
  case EfiSimpleNetworkInitialized:
    break;
  case EfiSimpleNetworkStarted:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver not yet initialized\n"));
    ReturnUnlock (EFI_DEVICE_ERROR);
  case EfiSimpleNetworkStopped:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver not started\n"));
    ReturnUnlock (EFI_NOT_STARTED);
  default:
    DEBUG ((DEBUG_ERROR, "NETSEC: Driver in an invalid state: %u\n",
      (UINTN)Snp->Mode->State));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  // Find the LanDriver structure
  LanDriver = INSTANCE_FROM_SNP_THIS (Snp);

  ogma_err = ogma_clean_tx_desc_ring (LanDriver->Handle,
                                      OGMA_DESC_RING_ID_NRM_TX);

  if (TxBuff != NULL) {
    *TxBuff = NULL;
    //
    // Find a buffer in the list that has been released
    //
    for (Link = GetFirstNode (&LanDriver->TxBufferList);
         !IsNull (&LanDriver->TxBufferList, Link);
         Link = GetNextNode (&LanDriver->TxBufferList, Link)) {

      pkt_handle = BASE_CR (Link, PACKET_HANDLE, Link);
      if (pkt_handle->Released) {
        *TxBuff = pkt_handle->Buffer;
        RemoveEntryList (Link);
        FreePool (pkt_handle);
        break;
      }
    }
  }

  if (IrqStat != 0) {
    *IrqStat = 0;
  }

  Status = EFI_SUCCESS;

  // Restore TPL and return
ExitUnlock:
  gBS->RestoreTPL (SavedTpl);
  return Status;
}

/*
 *  UEFI Transmit() function
 */
STATIC
EFI_STATUS
EFIAPI
SnpTransmit (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL   *Snp,
  IN  UINTN                         HdrSize,
  IN  UINTN                         BufSize,
  IN  VOID                          *BufAddr,
  IN  EFI_MAC_ADDRESS               *SrcAddr    OPTIONAL,
  IN  EFI_MAC_ADDRESS               *DstAddr    OPTIONAL,
  IN  UINT16                        *Protocol   OPTIONAL
  )
{
  NETSEC_DRIVER             *LanDriver;
  EFI_TPL                   SavedTpl;
  EFI_STATUS                Status;

  ogma_tx_pkt_ctrl_t  tx_pkt_ctrl;
  ogma_frag_info_t    scat_info;
  ogma_uint16         tx_avail_num;
  ogma_err_t          ogma_err;
  UINT16              Proto;
  pfdep_pkt_handle_t  pkt_handle;

  // Check preliminaries
  if ((Snp == NULL) || (BufAddr == NULL)) {
    DEBUG ((DEBUG_ERROR,
      "NETSEC: SnpTransmit(): NULL Snp (%p) or BufAddr (%p)\n", Snp, BufAddr));
    return EFI_DEVICE_ERROR;
  }

  pkt_handle = AllocateZeroPool (sizeof (*pkt_handle));
  if (pkt_handle == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  pkt_handle->Buffer = BufAddr;
  pkt_handle->RecycleForTx = TRUE;

  // Serialize access to data and registers
  SavedTpl = gBS->RaiseTPL (TPL_CALLBACK);

  // Check that driver was started and initialised
  switch (Snp->Mode->State) {
  case EfiSimpleNetworkInitialized:
    break;
  case EfiSimpleNetworkStarted:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver not yet initialized\n"));
    ReturnUnlock (EFI_DEVICE_ERROR);
  case EfiSimpleNetworkStopped:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver not started\n"));
    ReturnUnlock (EFI_NOT_STARTED);
  default:
    DEBUG ((DEBUG_ERROR, "NETSEC: Driver in an invalid state: %u\n",
      (UINTN)Snp->Mode->State));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  // Find the LanDriver structure
  LanDriver = INSTANCE_FROM_SNP_THIS (Snp);

  ogma_err = ogma_clear_desc_ring_irq_status (LanDriver->Handle,
                                              OGMA_DESC_RING_ID_NRM_TX,
                                              OGMA_CH_IRQ_REG_EMPTY);
  if (ogma_err != OGMA_ERR_OK) {
    DEBUG ((DEBUG_ERROR,
      "NETSEC: ogma_clear_desc_ring_irq_status failed with error code: %d\n",
      (INT32)ogma_err));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  ogma_err = ogma_clean_tx_desc_ring (LanDriver->Handle,
                                      OGMA_DESC_RING_ID_NRM_TX);
  if (ogma_err != OGMA_ERR_OK) {
    DEBUG ((DEBUG_ERROR,
      "NETSEC: ogma_clean_tx_desc_ring failed with error code: %d\n",
      (INT32)ogma_err));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  // Ensure header is correct size if non-zero
  if (HdrSize) {
    if (HdrSize != Snp->Mode->MediaHeaderSize) {
      DEBUG ((DEBUG_ERROR, "NETSEC: SnpTransmit(): Invalid HdrSize %d\n",
        HdrSize));
      ReturnUnlock (EFI_INVALID_PARAMETER);
    }

    if ((DstAddr == NULL) || (Protocol == NULL)) {
      DEBUG ((DEBUG_ERROR,
        "NETSEC: SnpTransmit(): NULL DstAddr %p or Protocol %p\n",
        DstAddr, Protocol));
      ReturnUnlock (EFI_INVALID_PARAMETER);
    }

    // Copy destination address
    CopyMem (BufAddr, (VOID *)DstAddr, NET_ETHER_ADDR_LEN);
    // Copy source address
    CopyMem (
      (VOID*)((UINTN)BufAddr + NET_ETHER_ADDR_LEN),
      (VOID*)SrcAddr,
      NET_ETHER_ADDR_LEN
      );
    // Copy protocol
    Proto = HTONS (*Protocol);
    CopyMem (
      (VOID*)((UINTN)BufAddr + (NET_ETHER_ADDR_LEN * 2)),
      (VOID*)&Proto,
      sizeof (UINT16)
      );
  }

  Status = DmaMap (MapOperationBusMasterRead, BufAddr, &BufSize,
             &scat_info.phys_addr, &pkt_handle->Mapping);
  if (EFI_ERROR (Status)) {
    goto ExitUnlock;
  }

  scat_info.addr        = BufAddr;
  scat_info.len         = BufSize;

  SetMem (&tx_pkt_ctrl, sizeof (ogma_tx_pkt_ctrl_t), 0);

  tx_pkt_ctrl.pass_through_flag     = OGMA_TRUE;
  tx_pkt_ctrl.target_desc_ring_id   = OGMA_DESC_RING_ID_GMAC;

  // check empty slot
  do {
    tx_avail_num = ogma_get_tx_avail_num (LanDriver->Handle,
                                          OGMA_DESC_RING_ID_NRM_TX);
  } while (tx_avail_num < SCAT_NUM);

  // send
  ogma_err = ogma_set_tx_pkt_data (LanDriver->Handle,
                                   OGMA_DESC_RING_ID_NRM_TX,
                                   &tx_pkt_ctrl,
                                   SCAT_NUM,
                                   &scat_info,
                                   pkt_handle);

  if (ogma_err != OGMA_ERR_OK) {
    DmaUnmap (pkt_handle->Mapping);
    FreePool (pkt_handle);
    DEBUG ((DEBUG_ERROR,
      "NETSEC: ogma_set_tx_pkt_data failed with error code: %d\n",
      (INT32)ogma_err));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  //
  // Queue the descriptor so we can release the buffer once it has been
  // consumed by the hardware.
  //
  InsertTailList (&LanDriver->TxBufferList, &pkt_handle->Link);

  gBS->RestoreTPL (SavedTpl);
  return EFI_SUCCESS;

  // Restore TPL and return
ExitUnlock:
  FreePool (pkt_handle);
  gBS->RestoreTPL (SavedTpl);
  return Status;
}

/*
 *  UEFI Receive() function
 */
EFI_STATUS
EFIAPI
SnpReceive (
  IN      EFI_SIMPLE_NETWORK_PROTOCOL   *Snp,
      OUT UINTN                         *HdrSize    OPTIONAL,
  IN  OUT UINTN                         *BuffSize,
      OUT VOID                          *Data,
      OUT EFI_MAC_ADDRESS               *SrcAddr    OPTIONAL,
      OUT EFI_MAC_ADDRESS               *DstAddr    OPTIONAL,
      OUT UINT16                        *Protocol   OPTIONAL
  )
{
  EFI_TPL             SavedTpl;
  EFI_STATUS          Status;
  NETSEC_DRIVER       *LanDriver;

  ogma_err_t          ogma_err;
  ogma_rx_pkt_info_t  rx_pkt_info;
  ogma_frag_info_t    rx_data;
  ogma_uint16         len;
  pfdep_pkt_handle_t  pkt_handle;

  // Check preliminaries
  if ((Snp == NULL) || (Data == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Serialize access to data and registers
  SavedTpl = gBS->RaiseTPL (TPL_CALLBACK);

  // Check that driver was started and initialised
  switch (Snp->Mode->State) {
  case EfiSimpleNetworkInitialized:
    break;
  case EfiSimpleNetworkStarted:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver not yet initialized\n"));
    ReturnUnlock (EFI_DEVICE_ERROR);
  case EfiSimpleNetworkStopped:
    DEBUG ((DEBUG_WARN, "NETSEC: Driver not started\n"));
    ReturnUnlock (EFI_NOT_STARTED);
  default:
    DEBUG ((DEBUG_ERROR, "NETSEC: Driver in an invalid state: %u\n",
      (UINTN)Snp->Mode->State));
    ReturnUnlock (EFI_DEVICE_ERROR);
  }

  // Find the LanDriver structure
  LanDriver = INSTANCE_FROM_SNP_THIS (Snp);

  if (ogma_get_rx_num (LanDriver->Handle, OGMA_DESC_RING_ID_NRM_RX) > 0) {

    ogma_err = ogma_get_rx_pkt_data (LanDriver->Handle,
                                    OGMA_DESC_RING_ID_NRM_RX,
                                    &rx_pkt_info, &rx_data, &len, &pkt_handle);
    if (ogma_err != OGMA_ERR_OK) {
      DEBUG ((DEBUG_ERROR,
        "NETSEC: ogma_get_rx_pkt_data failed with error code: %d\n",
        (INT32)ogma_err));
      ReturnUnlock (EFI_DEVICE_ERROR);
    }

    DmaUnmap (pkt_handle->Mapping);
    pkt_handle->Mapping = NULL;

    CopyMem (Data, (VOID *)rx_data.addr, len);
    *BuffSize = len;

    pfdep_free_pkt_buf (LanDriver->Handle, rx_data.len, rx_data.addr,
      rx_data.phys_addr, PFDEP_TRUE, pkt_handle);
  } else {
    // not received any packets
    ReturnUnlock (EFI_NOT_READY);
  }

  if (HdrSize != NULL) {
    *HdrSize = LanDriver->SnpMode.MediaHeaderSize;
  }

  ogma_clear_desc_ring_irq_status (LanDriver->Handle,
                                   OGMA_DESC_RING_ID_NRM_TX,
                                   OGMA_CH_IRQ_REG_EMPTY);

  ogma_clean_tx_desc_ring (LanDriver->Handle, OGMA_DESC_RING_ID_NRM_TX);

  ogma_enable_top_irq (LanDriver->Handle,
                       OGMA_TOP_IRQ_REG_NRM_TX | OGMA_TOP_IRQ_REG_NRM_RX);

  Status = EFI_SUCCESS;

  // Restore TPL and return
ExitUnlock:
  gBS->RestoreTPL (SavedTpl);
  return Status;
}

STATIC
EFI_STATUS
EFIAPI
NetsecAipGetInformation (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  OUT VOID                              **InformationBlock,
  OUT UINTN                             *InformationBlockSize
  )
{
  EFI_ADAPTER_INFO_MEDIA_STATE  *AdapterInfo;
  NETSEC_DRIVER                 *LanDriver;

  if (This == NULL || InformationBlock == NULL ||
      InformationBlockSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (!CompareGuid (InformationType, &gEfiAdapterInfoMediaStateGuid)) {
    return EFI_UNSUPPORTED;
  }

  AdapterInfo = AllocateZeroPool (sizeof (EFI_ADAPTER_INFO_MEDIA_STATE));
  if (AdapterInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *InformationBlock = AdapterInfo;
  *InformationBlockSize = sizeof (EFI_ADAPTER_INFO_MEDIA_STATE);

  LanDriver = INSTANCE_FROM_AIP_THIS (This);
  if (LanDriver->Snp.Mode->MediaPresent) {
    AdapterInfo->MediaState = EFI_SUCCESS;
  } else {
    AdapterInfo->MediaState = EFI_NOT_READY;
  }

  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
NetsecAipSetInformation (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  IN  VOID                              *InformationBlock,
  IN  UINTN                             InformationBlockSize
  )
{
  if (This == NULL || InformationBlock == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (CompareGuid (InformationType, &gEfiAdapterInfoMediaStateGuid)) {
    return EFI_WRITE_PROTECTED;
  }

  return EFI_UNSUPPORTED;
}

STATIC
EFI_STATUS
EFIAPI
NetsecAipGetSupportedTypes (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  OUT EFI_GUID                          **InfoTypesBuffer,
  OUT UINTN                             *InfoTypesBufferCount
  )
{
  EFI_GUID    *Guid;

  if (This == NULL || InfoTypesBuffer == NULL ||
      InfoTypesBufferCount == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Guid = AllocatePool (sizeof *Guid);
  if (Guid == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  CopyGuid (Guid, &gEfiAdapterInfoMediaStateGuid);

  *InfoTypesBuffer      = Guid;
  *InfoTypesBufferCount = 1;

  return EFI_SUCCESS;
}

EFI_STATUS
NetsecInit (
  IN      EFI_HANDLE        DriverBindingHandle,
  IN      EFI_HANDLE        ControllerHandle
  )
{
  EFI_STATUS                        Status;
  NETSEC_DRIVER                     *LanDriver;
  EFI_SIMPLE_NETWORK_PROTOCOL       *Snp;
  EFI_SIMPLE_NETWORK_MODE           *SnpMode;

  // Allocate Resources
  LanDriver = AllocateZeroPool (sizeof (NETSEC_DRIVER));
  if (LanDriver == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Status = gBS->OpenProtocol (ControllerHandle,
                              &gEdkiiNonDiscoverableDeviceProtocolGuid,
                              (VOID **)&LanDriver->Dev,
                              DriverBindingHandle,
                              ControllerHandle,
                              EFI_OPEN_PROTOCOL_BY_DRIVER);
  if (EFI_ERROR (Status)) {
    goto FreeDevice;
  }

  // Initialize pointers
  Snp = &(LanDriver->Snp);
  SnpMode = &(LanDriver->SnpMode);
  Snp->Mode = SnpMode;

  // Set the signature of the LAN Driver structure
  LanDriver->Signature = NETSEC_SIGNATURE;

  // Probe the device
  Status = Probe (DriverBindingHandle, LanDriver);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,
      "NETSEC:%a(): Probe failed with status %d\n", __FUNCTION__, Status));
    goto CloseDeviceProtocol;
  }

  // Assign fields and func pointers
  Snp->Revision = EFI_SIMPLE_NETWORK_PROTOCOL_REVISION;
  Snp->WaitForPacket = NULL;
  Snp->Initialize = SnpInitialize;
  Snp->Start = SnpStart;
  Snp->Stop = SnpStop;
  Snp->Reset = NULL;
  Snp->Shutdown = SnpShutdown;
  Snp->ReceiveFilters = SnpReceiveFilters;
  Snp->StationAddress = NULL;
  Snp->Statistics = NULL;
  Snp->MCastIpToMac = NULL;
  Snp->NvData = NULL;
  Snp->GetStatus = SnpGetStatus;
  Snp->Transmit = SnpTransmit;
  Snp->Receive = SnpReceive;

  // Fill in simple network mode structure
  SnpMode->State = EfiSimpleNetworkStopped;
  SnpMode->HwAddressSize = NET_ETHER_ADDR_LEN;
  SnpMode->MediaHeaderSize = sizeof (ETHER_HEAD);
  SnpMode->MaxPacketSize = EFI_PAGE_SIZE;

  // Supported receive filters
  SnpMode->ReceiveFilterMask = EFI_SIMPLE_NETWORK_RECEIVE_UNICAST |
                               EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST |
                               EFI_SIMPLE_NETWORK_RECEIVE_BROADCAST |
                               EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS |
                               EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS_MULTICAST;

  // Initially-enabled receive filters
  SnpMode->ReceiveFilterSetting = EFI_SIMPLE_NETWORK_RECEIVE_UNICAST |
                                  EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST |
                                  EFI_SIMPLE_NETWORK_RECEIVE_BROADCAST;

  // Netsec has 64bit hash table. We can filter an infinite MACs, but
  // higher-level software must filter out any hash collisions.
  SnpMode->MaxMCastFilterCount = MAX_MCAST_FILTER_CNT;
  SnpMode->MCastFilterCount = 0;
  ZeroMem (&SnpMode->MCastFilter,
           MAX_MCAST_FILTER_CNT * sizeof (EFI_MAC_ADDRESS));

  // Set the interface type (1: Ethernet or 6: IEEE 802 Networks)
  SnpMode->IfType = NET_IFTYPE_ETHERNET;

  // Mac address is changeable
  SnpMode->MacAddressChangeable = TRUE;

  // We can only transmit one packet at a time
  SnpMode->MultipleTxSupported = FALSE;

  // MediaPresent checks for cable connection and partner link
  SnpMode->MediaPresentSupported = TRUE;
  SnpMode->MediaPresent = FALSE;

  LanDriver->Aip.GetInformation     = NetsecAipGetInformation;
  LanDriver->Aip.SetInformation     = NetsecAipSetInformation;
  LanDriver->Aip.GetSupportedTypes  = NetsecAipGetSupportedTypes;

  //  Set broadcast address
  SetMem (&SnpMode->BroadcastAddress, sizeof (EFI_MAC_ADDRESS), 0xFF);

  InitializeListHead (&LanDriver->TxBufferList);

  // Initialise the protocol
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ControllerHandle,
                  &gEfiSimpleNetworkProtocolGuid, Snp,
                  &gEfiAdapterInformationProtocolGuid, &LanDriver->Aip,
                  NULL);

  LanDriver->ControllerHandle = ControllerHandle;

  // Say what the status of loading the protocol structure is
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: InstallMultipleProtocolInterfaces failed - %r\n",
      __FUNCTION__, Status));
    ogma_terminate (LanDriver->Handle);
    goto CloseDeviceProtocol;
  }
  return EFI_SUCCESS;

CloseDeviceProtocol:
  gBS->CloseProtocol (ControllerHandle,
         &gEdkiiNonDiscoverableDeviceProtocolGuid, DriverBindingHandle,
         ControllerHandle);

FreeDevice:
  FreePool (LanDriver);
  return Status;
}

EFI_STATUS
NetsecRelease (
  IN  EFI_HANDLE        DriverBindingHandle,
  IN  EFI_HANDLE        ControllerHandle
  )
{
  EFI_SIMPLE_NETWORK_PROTOCOL   *Snp;
  NETSEC_DRIVER                 *LanDriver;
  EFI_STATUS                    Status;

  Status = gBS->HandleProtocol (ControllerHandle,
                                &gEfiSimpleNetworkProtocolGuid,
                                (VOID **)&Snp);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  LanDriver = INSTANCE_FROM_SNP_THIS (Snp);

  Status = gBS->UninstallMultipleProtocolInterfaces (ControllerHandle,
                  &gEfiSimpleNetworkProtocolGuid, Snp,
                  &gEfiAdapterInformationProtocolGuid, &LanDriver->Aip,
                  NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (Snp->Mode->State == EfiSimpleNetworkInitialized) {
    SnpShutdown (Snp);
  }

  ogma_terminate (LanDriver->Handle);

  gBS->CloseEvent (LanDriver->ExitBootEvent);

  Status = gBS->CloseProtocol (ControllerHandle,
                               &gEdkiiNonDiscoverableDeviceProtocolGuid,
                               DriverBindingHandle,
                               ControllerHandle);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  gBS->FreePool (LanDriver);

  return EFI_SUCCESS;
}
