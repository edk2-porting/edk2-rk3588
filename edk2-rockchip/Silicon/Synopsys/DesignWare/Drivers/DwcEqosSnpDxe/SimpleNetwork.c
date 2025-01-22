/** @file

  Copyright (c) 2025, Mario Bălănică <mariobalanica02@gmail.com>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Eqos.h"

/**
  Changes the state of a network interface from "stopped" to "started".

  @param  This Protocol instance pointer.

  @retval EFI_SUCCESS           The network interface was started.
  @retval EFI_ALREADY_STARTED   The network interface is already in the started state.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
STATIC
EFI_STATUS
EFIAPI
EqosSnpStart (
  IN EFI_SIMPLE_NETWORK_PROTOCOL  *This
  )
{
  EQOS_PRIVATE_DATA  *Eqos;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Eqos = EQOS_PRIVATE_DATA_FROM_SNP_THIS (This);
  if (Eqos->SnpMode.State == EfiSimpleNetworkStarted) {
    return EFI_ALREADY_STARTED;
  } else if (Eqos->SnpMode.State != EfiSimpleNetworkStopped) {
    return EFI_DEVICE_ERROR;
  }

  Eqos->SnpMode.State = EfiSimpleNetworkStarted;

  return EFI_SUCCESS;
}

/**
  Changes the state of a network interface from "started" to "stopped".

  @param  This Protocol instance pointer.

  @retval EFI_SUCCESS           The network interface was stopped.
  @retval EFI_ALREADY_STARTED   The network interface is already in the stopped state.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
STATIC
EFI_STATUS
EFIAPI
EqosSnpStop (
  IN EFI_SIMPLE_NETWORK_PROTOCOL  *This
  )
{
  EQOS_PRIVATE_DATA  *Eqos;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Eqos = EQOS_PRIVATE_DATA_FROM_SNP_THIS (This);
  if (Eqos->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  } else if (Eqos->SnpMode.State != EfiSimpleNetworkStarted) {
    return EFI_DEVICE_ERROR;
  }

  Eqos->SnpMode.State = EfiSimpleNetworkStopped;

  return EFI_SUCCESS;
}

/**
  Resets a network adapter and allocates the transmit and receive buffers
  required by the network interface; optionally, also requests allocation
  of additional transmit and receive buffers.

  @param  This              The protocol instance pointer.
  @param  ExtraRxBufferSize The size, in bytes, of the extra receive buffer space
                            that the driver should allocate for the network interface.
                            Some network interfaces will not be able to use the extra
                            buffer, and the caller will not know if it is actually
                            being used.
  @param  ExtraTxBufferSize The size, in bytes, of the extra transmit buffer space
                            that the driver should allocate for the network interface.
                            Some network interfaces will not be able to use the extra
                            buffer, and the caller will not know if it is actually
                            being used.

  @retval EFI_SUCCESS           The network interface was initialized.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_OUT_OF_RESOURCES  There was not enough memory for the transmit and
                                receive buffers.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
STATIC
EFI_STATUS
EFIAPI
EqosSnpInitialize (
  IN EFI_SIMPLE_NETWORK_PROTOCOL  *This,
  IN UINTN                        ExtraRxBufferSize  OPTIONAL,
  IN UINTN                        ExtraTxBufferSize  OPTIONAL
  )
{
  EQOS_PRIVATE_DATA  *Eqos;
  EFI_STATUS         Status;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Eqos = EQOS_PRIVATE_DATA_FROM_SNP_THIS (This);
  if (Eqos->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  } else if (Eqos->SnpMode.State != EfiSimpleNetworkStarted) {
    return EFI_DEVICE_ERROR;
  }

  Status = EqosStart (Eqos);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Eqos->SnpMode.State = EfiSimpleNetworkInitialized;

  return EFI_SUCCESS;
}

/**
  Resets a network adapter and re-initializes it with the parameters that were
  provided in the previous call to Initialize().

  @param  This                 The protocol instance pointer.
  @param  ExtendedVerification Indicates that the driver may perform a more
                               exhaustive verification operation of the device
                               during reset.

  @retval EFI_SUCCESS           The network interface was reset.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
STATIC
EFI_STATUS
EFIAPI
EqosSnpReset (
  IN EFI_SIMPLE_NETWORK_PROTOCOL  *This,
  IN BOOLEAN                      ExtendedVerification
  )
{
  EQOS_PRIVATE_DATA  *Eqos;
  EFI_STATUS         Status;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Eqos = EQOS_PRIVATE_DATA_FROM_SNP_THIS (This);
  if (Eqos->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  if (Eqos->SnpMode.State != EfiSimpleNetworkInitialized) {
    return EFI_DEVICE_ERROR;
  }

  Status = EqosStop (Eqos);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = EqosStart (Eqos);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = EqosSetRxFilters (
             Eqos,
             Eqos->SnpMode.ReceiveFilterSetting,
             Eqos->SnpMode.MCastFilterCount == 0,
             Eqos->SnpMode.MCastFilterCount,
             Eqos->SnpMode.MCastFilter
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Resets a network adapter and leaves it in a state that is safe for
  another driver to initialize.

  @param  This Protocol instance pointer.

  @retval EFI_SUCCESS           The network interface was shutdown.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
STATIC
EFI_STATUS
EFIAPI
EqosSnpShutdown (
  IN EFI_SIMPLE_NETWORK_PROTOCOL  *This
  )
{
  EQOS_PRIVATE_DATA  *Eqos;
  EFI_STATUS         Status;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Eqos = EQOS_PRIVATE_DATA_FROM_SNP_THIS (This);
  if (Eqos->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  if (Eqos->SnpMode.State != EfiSimpleNetworkInitialized) {
    return EFI_DEVICE_ERROR;
  }

  Status = EqosStop (Eqos);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Eqos->SnpMode.State = EfiSimpleNetworkStarted;

  return EFI_SUCCESS;
}

/**
  Manages the multicast receive filters of a network interface.

  @param  This             The protocol instance pointer.
  @param  Enable           A bit mask of receive filters to enable on the network interface.
  @param  Disable          A bit mask of receive filters to disable on the network interface.
  @param  ResetMCastFilter Set to TRUE to reset the contents of the multicast receive
                           filters on the network interface to their default values.
  @param  MCastFilterCnt   Number of multicast HW MAC addresses in the new
                           MCastFilter list. This value must be less than or equal to
                           the MCastFilterCnt field of EFI_SIMPLE_NETWORK_MODE. This
                           field is optional if ResetMCastFilter is TRUE.
  @param  MCastFilter      A pointer to a list of new multicast receive filter HW MAC
                           addresses. This list will replace any existing multicast
                           HW MAC address list. This field is optional if
                           ResetMCastFilter is TRUE.

  @retval EFI_SUCCESS           The multicast receive filter list was updated.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
STATIC
EFI_STATUS
EFIAPI
EqosSnpReceiveFilters (
  IN EFI_SIMPLE_NETWORK_PROTOCOL  *This,
  IN UINT32                       Enable,
  IN UINT32                       Disable,
  IN BOOLEAN                      ResetMCastFilter,
  IN UINTN                        MCastFilterCnt    OPTIONAL,
  IN EFI_MAC_ADDRESS              *MCastFilter      OPTIONAL
  )
{
  EQOS_PRIVATE_DATA  *Eqos;
  UINT32             ReceiveFilterSetting;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Eqos = EQOS_PRIVATE_DATA_FROM_SNP_THIS (This);

  if (Eqos->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  if (Eqos->SnpMode.State != EfiSimpleNetworkInitialized) {
    return EFI_DEVICE_ERROR;
  }

  if ((((Enable | Disable) & ~Eqos->SnpMode.ReceiveFilterMask) != 0)) {
    return EFI_INVALID_PARAMETER;
  }

  ReceiveFilterSetting = (Eqos->SnpMode.ReceiveFilterSetting | Enable) & (~Disable);

  if (ResetMCastFilter) {
    Eqos->SnpMode.MCastFilterCount = 0;
  } else if (ReceiveFilterSetting & EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST) {
    if ((MCastFilterCnt == 0) ||
        (MCastFilterCnt > Eqos->SnpMode.MaxMCastFilterCount) ||
        (MCastFilter == NULL))
    {
      return EFI_INVALID_PARAMETER;
    }

    Eqos->SnpMode.MCastFilterCount = MCastFilterCnt;
    CopyMem (Eqos->SnpMode.MCastFilter, MCastFilter, MCastFilterCnt * sizeof (EFI_MAC_ADDRESS));
  }

  Eqos->SnpMode.ReceiveFilterSetting = ReceiveFilterSetting;

  return EqosSetRxFilters (
           Eqos,
           ReceiveFilterSetting,
           ResetMCastFilter,
           MCastFilterCnt,
           MCastFilter
           );
}

/**
  Modifies or resets the current station address, if supported.

  @param  This  The protocol instance pointer.
  @param  Reset Flag used to reset the station address to the network interfaces
                permanent address.
  @param  New   The new station address to be used for the network interface.

  @retval EFI_SUCCESS           The network interfaces station address was updated.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
STATIC
EFI_STATUS
EFIAPI
EqosSnpStationAddress (
  IN EFI_SIMPLE_NETWORK_PROTOCOL  *This,
  IN BOOLEAN                      Reset,
  IN EFI_MAC_ADDRESS              *New    OPTIONAL
  )
{
  EQOS_PRIVATE_DATA  *Eqos;

  if ((This == NULL) || (This->Mode == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if ((Reset == FALSE) && (New == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Eqos = EQOS_PRIVATE_DATA_FROM_SNP_THIS (This);
  if (Eqos->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  if (Eqos->SnpMode.State != EfiSimpleNetworkInitialized) {
    return EFI_DEVICE_ERROR;
  }

  if (Reset) {
    CopyMem (
      &This->Mode->CurrentAddress,
      &This->Mode->PermanentAddress,
      sizeof (This->Mode->CurrentAddress)
      );
  } else {
    CopyMem (
      &This->Mode->CurrentAddress,
      New,
      sizeof (This->Mode->CurrentAddress)
      );
  }

  EqosSetMacAddress (Eqos->Base, &Eqos->SnpMode.CurrentAddress);

  return EFI_SUCCESS;
}

/**
  Resets or collects the statistics on a network interface.

  @param  This            Protocol instance pointer.
  @param  Reset           Set to TRUE to reset the statistics for the network interface.
  @param  StatisticsSize  On input the size, in bytes, of StatisticsTable. On
                          output the size, in bytes, of the resulting table of
                          statistics.
  @param  StatisticsTable A pointer to the EFI_NETWORK_STATISTICS structure that
                          contains the statistics.

  @retval EFI_SUCCESS           The statistics were collected from the network interface.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_BUFFER_TOO_SMALL  The Statistics buffer was too small. The current buffer
                                size needed to hold the statistics is returned in
                                StatisticsSize.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
STATIC
EFI_STATUS
EFIAPI
EqosSnpStatistics (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL  *This,
  IN  BOOLEAN                      Reset,
  IN  OUT UINTN                    *StatisticsSize  OPTIONAL,
  OUT EFI_NETWORK_STATISTICS       *StatisticsTable OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Converts a multicast IP address to a multicast HW MAC address.

  @param  This The protocol instance pointer.
  @param  IPv6 Set to TRUE if the multicast IP address is IPv6 [RFC 2460]. Set
               to FALSE if the multicast IP address is IPv4 [RFC 791].
  @param  IP   The multicast IP address that is to be converted to a multicast
               HW MAC address.
  @param  MAC  The multicast HW MAC address that is to be generated from IP.

  @retval EFI_SUCCESS           The multicast IP address was mapped to the multicast
                                HW MAC address.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_BUFFER_TOO_SMALL  The Statistics buffer was too small. The current buffer
                                size needed to hold the statistics is returned in
                                StatisticsSize.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
STATIC
EFI_STATUS
EFIAPI
EqosSnpMCastIptoMac (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL  *This,
  IN  BOOLEAN                      IPv6,
  IN  EFI_IP_ADDRESS               *IP,
  OUT EFI_MAC_ADDRESS              *MAC
  )
{
  EQOS_PRIVATE_DATA  *Eqos;

  if ((This == NULL) || (IP == NULL) || (MAC == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // https://en.wikipedia.org/wiki/Multicast_address
  //
  if ((IPv6 && ((IP->v6.Addr[0]) != 0xFF)) ||       // invalid IPv6 mcast addr
      (!IPv6 && ((IP->v4.Addr[0] & 0xF0) != 0xE0))  // invalid IPv4 mcast addr
      )
  {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Invalid IPv4/IPv6 multicast address!\n",
      __func__
      ));
    return EFI_INVALID_PARAMETER;
  }

  Eqos = EQOS_PRIVATE_DATA_FROM_SNP_THIS (This);
  if (Eqos->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  if (Eqos->SnpMode.State != EfiSimpleNetworkInitialized) {
    return EFI_DEVICE_ERROR;
  }

  ZeroMem (MAC, sizeof (EFI_MAC_ADDRESS));

  //
  // https://en.wikipedia.org/wiki/IP_multicast#Layer_2_delivery
  //
  if (IPv6) {
    MAC->Addr[0] = 0x33;
    MAC->Addr[1] = 0x33;
    MAC->Addr[2] = IP->v6.Addr[12];
    MAC->Addr[3] = IP->v6.Addr[13];
    MAC->Addr[4] = IP->v6.Addr[14];
    MAC->Addr[5] = IP->v6.Addr[15];
  } else {
    MAC->Addr[0] = 0x01;
    MAC->Addr[1] = 0x00;
    MAC->Addr[2] = 0x5E;
    MAC->Addr[3] = IP->v4.Addr[1] & 0x7F;
    MAC->Addr[4] = IP->v4.Addr[2];
    MAC->Addr[5] = IP->v4.Addr[3];
  }

  return EFI_SUCCESS;
}

/**
  Performs read and write operations on the NVRAM device attached to a
  network interface.

  @param  This       The protocol instance pointer.
  @param  ReadWrite  TRUE for read operations, FALSE for write operations.
  @param  Offset     Byte offset in the NVRAM device at which to start the read or
                     write operation. This must be a multiple of NvRamAccessSize and
                     less than NvRamSize.
  @param  BufferSize The number of bytes to read or write from the NVRAM device.
                     This must also be a multiple of NvramAccessSize.
  @param  Buffer     A pointer to the data buffer.

  @retval EFI_SUCCESS           The NVRAM access was performed.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
STATIC
EFI_STATUS
EFIAPI
EqosSnpNvData (
  IN EFI_SIMPLE_NETWORK_PROTOCOL  *This,
  IN BOOLEAN                      ReadWrite,
  IN UINTN                        Offset,
  IN UINTN                        BufferSize,
  IN OUT VOID                     *Buffer
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Reads the current interrupt status and recycled transmit buffer status from
  a network interface.

  @param  This            The protocol instance pointer.
  @param  InterruptStatus A pointer to the bit mask of the currently active interrupts
                          If this is NULL, the interrupt status will not be read from
                          the device. If this is not NULL, the interrupt status will
                          be read from the device. When the  interrupt status is read,
                          it will also be cleared. Clearing the transmit  interrupt
                          does not empty the recycled transmit buffer array.
  @param  TxBuf           Recycled transmit buffer address. The network interface will
                          not transmit if its internal recycled transmit buffer array
                          is full. Reading the transmit buffer does not clear the
                          transmit interrupt. If this is NULL, then the transmit buffer
                          status will not be read. If there are no transmit buffers to
                          recycle and TxBuf is not NULL, * TxBuf will be set to NULL.

  @retval EFI_SUCCESS           The status of the network interface was retrieved.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
STATIC
EFI_STATUS
EFIAPI
EqosSnpGetStatus (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL  *This,
  OUT UINT32                       *InterruptStatus  OPTIONAL,
  OUT VOID                         **TxBuf           OPTIONAL
  )
{
  EQOS_PRIVATE_DATA  *Eqos;
  EFI_STATUS         Status;
  UINT32             DescIndex;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Eqos = EQOS_PRIVATE_DATA_FROM_SNP_THIS (This);
  if (Eqos->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  if (Eqos->SnpMode.State != EfiSimpleNetworkInitialized) {
    return EFI_DEVICE_ERROR;
  }

  EqosUpdateLink (Eqos);

  if (TxBuf != NULL) {
    *TxBuf = NULL;

    if (Eqos->TxQueued > 0) {
      DescIndex = Eqos->TxCurrent;

      Status = EqosCheckTxDescriptor (Eqos, DescIndex);
      if (Status != EFI_NOT_READY) {
        ASSERT (Eqos->TxBuffersMap[DescIndex] != NULL);
        EqosDmaUnmapTxDescriptor (Eqos, DescIndex);

        *TxBuf = Eqos->TxBuffers[DescIndex];

        Eqos->TxCurrent = EQOS_TX_NEXT (DescIndex);
        Eqos->TxQueued--;
      }
    }
  }

  //
  // InterruptStatus is not currently consumed by the upper layers,
  // but we still read it for compliance and to log any detected
  // errors, which can be helpful for debugging.
  //
  EqosGetDmaInterruptStatus (Eqos, InterruptStatus);

  return EFI_SUCCESS;
}

/**
  Places a packet in the transmit queue of a network interface.

  @param  This       The protocol instance pointer.
  @param  HeaderSize The size, in bytes, of the media header to be filled in by
                     the Transmit() function. If HeaderSize is non-zero, then it
                     must be equal to This->Mode->MediaHeaderSize and the DestAddr
                     and Protocol parameters must not be NULL.
  @param  BufferSize The size, in bytes, of the entire packet (media header and
                     data) to be transmitted through the network interface.
  @param  Buffer     A pointer to the packet (media header followed by data) to be
                     transmitted. This parameter cannot be NULL. If HeaderSize is zero,
                     then the media header in Buffer must already be filled in by the
                     caller. If HeaderSize is non-zero, then the media header will be
                     filled in by the Transmit() function.
  @param  SrcAddr    The source HW MAC address. If HeaderSize is zero, then this parameter
                     is ignored. If HeaderSize is non-zero and SrcAddr is NULL, then
                     This->Mode->CurrentAddress is used for the source HW MAC address.
  @param  DestAddr   The destination HW MAC address. If HeaderSize is zero, then this
                     parameter is ignored.
  @param  Protocol   The type of header to build. If HeaderSize is zero, then this
                     parameter is ignored. See RFC 1700, section "Ether Types", for
                     examples.

  @retval EFI_SUCCESS           The packet was placed on the transmit queue.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_NOT_READY         The network interface is too busy to accept this transmit request.
  @retval EFI_BUFFER_TOO_SMALL  The BufferSize parameter is too small.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
STATIC
EFI_STATUS
EFIAPI
EqosSnpTransmit (
  IN EFI_SIMPLE_NETWORK_PROTOCOL  *This,
  IN UINTN                        HeaderSize,
  IN UINTN                        BufferSize,
  IN VOID                         *Buffer,
  IN EFI_MAC_ADDRESS              *SrcAddr   OPTIONAL,
  IN EFI_MAC_ADDRESS              *DestAddr  OPTIONAL,
  IN UINT16                       *Protocol  OPTIONAL
  )
{
  EQOS_PRIVATE_DATA  *Eqos;
  EFI_STATUS         Status;
  UINT8              *Frame;
  UINT32             DescIndex;

  if ((This == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Eqos = EQOS_PRIVATE_DATA_FROM_SNP_THIS (This);
  if (Eqos->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  if (Eqos->SnpMode.State != EfiSimpleNetworkInitialized) {
    return EFI_DEVICE_ERROR;
  }

  if (HeaderSize != 0) {
    if (HeaderSize != Eqos->SnpMode.MediaHeaderSize) {
      DEBUG ((
        DEBUG_ERROR,
        "%a: Header size mismatch: HeaderSize=0x%X, SnpMode.MediaHeaderSize=0x%X)\n",
        __func__,
        HeaderSize,
        Eqos->SnpMode.MediaHeaderSize
        ));
      return EFI_INVALID_PARAMETER;
    }

    if ((DestAddr == NULL) || (Protocol == NULL)) {
      return EFI_INVALID_PARAMETER;
    }
  }

  if (BufferSize < Eqos->SnpMode.MediaHeaderSize) {
    DEBUG ((DEBUG_ERROR, "%a: Buffer too small!\n", __func__));
    return EFI_BUFFER_TOO_SMALL;
  }

  Status = EfiAcquireLockOrFail (&Eqos->Lock);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to get lock. Status=%r\n", __func__, Status));
    return EFI_ACCESS_DENIED;
  }

  if (Eqos->TxQueued == EQOS_TX_DESC_COUNT - 1) {
    Status = EFI_NOT_READY;
    goto Exit;
  }

  Frame = Buffer;

  if (HeaderSize != 0) {
    CopyMem (&Frame[0], &DestAddr->Addr[0], NET_ETHER_ADDR_LEN);
    CopyMem (&Frame[6], &SrcAddr->Addr[0], NET_ETHER_ADDR_LEN);
    Frame[12] = (*Protocol & 0xFF00) >> 8;
    Frame[13] = *Protocol & 0xFF;
  }

  DescIndex = Eqos->TxNext;

  Eqos->TxBuffers[DescIndex] = Frame;

  Status = EqosDmaMapTxDescriptor (Eqos, DescIndex, BufferSize);
  if (EFI_ERROR (Status)) {
    goto Exit;
  }

  Eqos->TxNext = EQOS_TX_NEXT (DescIndex);
  Eqos->TxQueued++;

  Status = EFI_SUCCESS;

Exit:
  EfiReleaseLock (&Eqos->Lock);
  return Status;
}

/**
  Receives a packet from a network interface.

  @param  This       The protocol instance pointer.
  @param  HeaderSize The size, in bytes, of the media header received on the network
                     interface. If this parameter is NULL, then the media header size
                     will not be returned.
  @param  BufferSize On entry, the size, in bytes, of Buffer. On exit, the size, in
                     bytes, of the packet that was received on the network interface.
  @param  Buffer     A pointer to the data buffer to receive both the media header and
                     the data.
  @param  SrcAddr    The source HW MAC address. If this parameter is NULL, the
                     HW MAC source address will not be extracted from the media
                     header.
  @param  DestAddr   The destination HW MAC address. If this parameter is NULL,
                     the HW MAC destination address will not be extracted from the
                     media header.
  @param  Protocol   The media header type. If this parameter is NULL, then the
                     protocol will not be extracted from the media header. See
                     RFC 1700 section "Ether Types" for examples.

  @retval  EFI_SUCCESS           The received data was stored in Buffer, and BufferSize has
                                 been updated to the number of bytes received.
  @retval  EFI_NOT_STARTED       The network interface has not been started.
  @retval  EFI_NOT_READY         No packets have been received on the network interface.
  @retval  EFI_BUFFER_TOO_SMALL  The BufferSize parameter is too small.
  @retval  EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval  EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval  EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
STATIC
EFI_STATUS
EFIAPI
EqosSnpReceive (
  IN     EFI_SIMPLE_NETWORK_PROTOCOL  *This,
  OUT    UINTN                        *HeaderSize  OPTIONAL,
  IN OUT UINTN                        *BufferSize,
  OUT    VOID                         *Buffer,
  OUT    EFI_MAC_ADDRESS              *SrcAddr     OPTIONAL,
  OUT    EFI_MAC_ADDRESS              *DestAddr    OPTIONAL,
  OUT    UINT16                       *Protocol    OPTIONAL
  )
{
  EQOS_PRIVATE_DATA  *Eqos;
  EFI_STATUS         Status;
  EFI_STATUS         MapStatus;
  UINT32             DescIndex;
  UINT8              *Frame;
  UINTN              FrameLength;

  if ((This == NULL) || (Buffer == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Eqos = EQOS_PRIVATE_DATA_FROM_SNP_THIS (This);
  if (Eqos->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  if (Eqos->SnpMode.State != EfiSimpleNetworkInitialized) {
    return EFI_DEVICE_ERROR;
  }

  Status = EfiAcquireLockOrFail (&Eqos->Lock);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to get lock. Status=%r\n", __func__, Status));
    return EFI_ACCESS_DENIED;
  }

  DescIndex   = Eqos->RxCurrent;
  FrameLength = 0;

  Status = EqosCheckRxDescriptor (Eqos, DescIndex, &FrameLength);
  if (Status == EFI_NOT_READY) {
    goto Exit;
  } else if (EFI_ERROR (Status)) {
    ASSERT (Eqos->RxBuffersMap[DescIndex] != NULL);
    EqosDmaUnmapRxDescriptor (Eqos, DescIndex);
    goto ReleaseDesc;
  }

  if (*BufferSize < FrameLength) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Buffer size (0x%X) is too small for frame (0x%X)!\n",
      __func__,
      *BufferSize,
      FrameLength
      ));
    *BufferSize = FrameLength;
    Status      = EFI_BUFFER_TOO_SMALL;
    goto Exit;
  }

  ASSERT (Eqos->RxBuffersMap[DescIndex] != NULL);
  EqosDmaUnmapRxDescriptor (Eqos, DescIndex);

  Frame = EQOS_RX_BUFFER (Eqos, DescIndex);

  if (DestAddr != NULL) {
    CopyMem (&DestAddr->Addr[0], &Frame[0], NET_ETHER_ADDR_LEN);
  }

  if (SrcAddr != NULL) {
    CopyMem (&SrcAddr->Addr[0], &Frame[6], NET_ETHER_ADDR_LEN);
  }

  if (Protocol != NULL) {
    *Protocol = (UINT16)((Frame[12] << 8) | Frame[13]);
  }

  if (HeaderSize != NULL) {
    *HeaderSize = Eqos->SnpMode.MediaHeaderSize;
  }

  CopyMem (Buffer, Frame, FrameLength);
  *BufferSize = FrameLength;

  Status = EFI_SUCCESS;

ReleaseDesc:
  MapStatus = EqosDmaMapRxDescriptor (Eqos, DescIndex);
  if (EFI_ERROR (MapStatus)) {
    Status = MapStatus;
    goto Exit;
  }

  Eqos->RxCurrent = EQOS_RX_NEXT (Eqos->RxCurrent);

Exit:
  EfiReleaseLock (&Eqos->Lock);
  return Status;
}

///
/// Simple Network Protocol
///
CONST EFI_SIMPLE_NETWORK_PROTOCOL  gEqosSnpTemplate = {
  EFI_SIMPLE_NETWORK_PROTOCOL_REVISION, // Revision
  EqosSnpStart,                         // Start
  EqosSnpStop,                          // Stop
  EqosSnpInitialize,                    // Initialize
  EqosSnpReset,                         // Reset
  EqosSnpShutdown,                      // Shutdown
  EqosSnpReceiveFilters,                // ReceiveFilters
  EqosSnpStationAddress,                // StationAddress
  EqosSnpStatistics,                    // Statistics
  EqosSnpMCastIptoMac,                  // MCastIpToMac
  EqosSnpNvData,                        // NvData
  EqosSnpGetStatus,                     // GetStatus
  EqosSnpTransmit,                      // Transmit
  EqosSnpReceive,                       // Receive
  NULL,                                 // WaitForPacket
  NULL                                  // Mode
};
