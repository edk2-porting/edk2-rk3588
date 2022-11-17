/** @file
  Provides the Simple Network functions.

  Copyright (c) 2011 - 2016, Intel Corporation. All rights reserved.
  Copyright (c) 2020, ARM Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Ax88772.h"

/**
  This function updates the filtering on the receiver.

  This support routine calls ::Ax88772MacAddressSet to update
  the MAC address.  This routine then rebuilds the multicast
  hash by calling ::Ax88772MulticastClear and ::Ax88772MulticastSet.
  Finally this routine enables the receiver by calling
  ::Ax88772RxControl.

  @param [in] SimpleNetwork    Simple network mode pointer

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
ReceiveFilterUpdate (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork
  )
{
  EFI_SIMPLE_NETWORK_MODE *Mode;
  NIC_DEVICE              *NicDevice;
  EFI_STATUS              Status;
  UINT32                  Index;

  //
  // Set the MAC address
  //
  NicDevice = DEV_FROM_SIMPLE_NETWORK (SimpleNetwork);
  Mode = SimpleNetwork->Mode;
  //
  // Clear the multicast hash table
  //
  Ax88772MulticastClear (NicDevice);

  //
  // Load the multicast hash table
  //
  if ((Mode->ReceiveFilterSetting & EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST) != 0) {
    for (Index = 0; Index < Mode->MCastFilterCount; Index++) {
      //
      // Enable the next multicast address
      //
      Ax88772MulticastSet (NicDevice,
                            &Mode->MCastFilter[Index].Addr[0]);
    }
  }

    Status = Ax88772RxControl (NicDevice, Mode->ReceiveFilterSetting);

  //
  // Return the operation status
  //
  return Status;
}


/**
  This function updates the SNP driver status.

  This function gets the current interrupt and recycled transmit
  buffer status from the network interface.  The interrupt status
  and the media status are returned as a bit mask in InterruptStatus.
  If InterruptStatus is NULL, the interrupt status will not be read.
  Upon successful return of the media status, the MediaPresent field
  of EFI_SIMPLE_NETWORK_MODE will be updated to reflect any change
  of media status.  If TxBuf is not NULL, a recycled transmit buffer
  address will be retrived.  If a recycled transmit buffer address
  is returned in TxBuf, then the buffer has been successfully
  transmitted, and the status for that buffer is cleared.

  This function calls ::Ax88772Rx to update the media status and
  queue any receive packets.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [in] InterruptStatus  A pointer to the bit mask of the current active interrupts.
                                If this is NULL, the interrupt status will not be read from
                                the device.  If this is not NULL, the interrupt status will
                                be read from teh device.  When the interrupt status is read,
                                it will also be cleared.  Clearing the transmit interrupt
                                does not empty the recycled transmit buffer array.
  @param [out] TxBuf          Recycled transmit buffer address.  The network interface will
                                not transmit if its internal recycled transmit buffer array is
                                full.  Reading the transmit buffer does not clear the transmit
                                interrupt.  If this is NULL, then the transmit buffer status
                                will not be read.  If there are not transmit buffers to recycle
                                and TxBuf is not NULL, *TxBuf will be set to NULL.

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.

**/

EFI_STATUS
EFIAPI
SN_GetStatus (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork,
  OUT UINT32                      *InterruptStatus,
  OUT VOID                        **TxBuf
  )
{
  EFI_SIMPLE_NETWORK_MODE *Mode;
  NIC_DEVICE              *NicDevice = NULL;
  EFI_STATUS              Status = EFI_SUCCESS;
  EFI_TPL                 TplPrevious;

  TplPrevious = gBS->RaiseTPL(TPL_CALLBACK);
  //
  // Verify the parameters
  //
  if ((SimpleNetwork != NULL) && (SimpleNetwork->Mode != NULL)) {
    //
    // Return the transmit buffer
    //
    NicDevice = DEV_FROM_SIMPLE_NETWORK (SimpleNetwork);

    if ((TxBuf != NULL) && (NicDevice->TxBuffer != NULL)) {
      *TxBuf = NicDevice->TxBuffer;
      NicDevice->TxBuffer = NULL;
    }

    Mode = SimpleNetwork->Mode;
    if (EfiSimpleNetworkInitialized == Mode->State) {
      if ((TxBuf == NULL) && (InterruptStatus == NULL)) {
        Status = EFI_INVALID_PARAMETER;
        goto EXIT;
      }

#if REPORTLINK
#else
      if (!NicDevice->LinkUp || !NicDevice->Complete) {
#endif
        Status = Ax88772NegotiateLinkComplete (NicDevice,
                                                &NicDevice->PollCount,
                                                &NicDevice->Complete,
                                                &NicDevice->LinkUp,
                                                &NicDevice->LinkSpeed100Mbps,
                                                &NicDevice->FullDuplex);

        if (EFI_ERROR(Status))
          goto EXIT;
#if REPORTLINK
        if (NicDevice->LinkUp && NicDevice->Complete) {
          Mode->MediaPresent = TRUE;
          Status = ReceiveFilterUpdate (SimpleNetwork);
        } else {
          Mode->MediaPresent = FALSE;
        }
#else
        if (NicDevice->LinkUp && NicDevice->Complete) {
          Mode->MediaPresent = TRUE;
          Mode->MediaPresentSupported = FALSE;
          Status = ReceiveFilterUpdate (SimpleNetwork);
        }
      }
#endif
    } else {
      if (EfiSimpleNetworkStarted == Mode->State) {
        Status = EFI_DEVICE_ERROR;
      } else {
        Status = EFI_NOT_STARTED ;
      }
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
  }
  if (InterruptStatus != NULL) {
    *InterruptStatus = 0;
  }

EXIT:
  gBS->RestoreTPL(TplPrevious) ;

  return Status;
}

/**
  This function performs read and write operations on the NVRAM device
  attached to a network interface.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [in] ReadWrite         TRUE for read operations, FALSE for write operations.
  @param [in] Offset            Byte offset in the NVRAM device at which to start the
                                read or write operation.  This must be a multiple of
                                NvRamAccessSize and less than NvRamSize.
  @param [in] BufferSize        The number of bytes to read or write from the NVRAM device.
                                This must also be a multiple of NvramAccessSize.
  @param [in, out] Buffer      A pointer to the data buffer.

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_NvData (
  IN     EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork,
  IN     BOOLEAN                     ReadWrite,
  IN     UINTN                       Offset,
  IN     UINTN                       BufferSize,
  IN OUT VOID                        *Buffer
  )
{
  EFI_STATUS              Status = EFI_INVALID_PARAMETER;
  EFI_TPL                 TplPrevious;
  EFI_SIMPLE_NETWORK_MODE *Mode;
  NIC_DEVICE              *NicDevice;
  UINTN                   Index;

  TplPrevious = gBS->RaiseTPL(TPL_CALLBACK);
  if ((SimpleNetwork == NULL) || (SimpleNetwork->Mode == NULL)) {
    Status = EFI_INVALID_PARAMETER;
    goto  EXIT;
  }

  NicDevice = DEV_FROM_SIMPLE_NETWORK (SimpleNetwork);
  Mode = SimpleNetwork->Mode;

  if (EfiSimpleNetworkInitialized != Mode->State) {
    Status = EFI_NOT_STARTED;
    goto  EXIT;
  }

  if (Offset != 0) {
    if (((Offset % Mode->NvRamAccessSize) != 0) ||
        (Offset >= Mode->NvRamSize)) {
      Status = EFI_INVALID_PARAMETER;
      goto  EXIT;
    }
  }
  //
  // Offset must be a multiple of NvRamAccessSize and less than NvRamSize.
  //
  if ((BufferSize % Mode->NvRamAccessSize) != 0) {
    Status = EFI_INVALID_PARAMETER;
    goto  EXIT;
  }

  if (BufferSize + Offset > Mode->NvRamSize) {
    Status = EFI_INVALID_PARAMETER;
    goto  EXIT;
  }

  if (Buffer == NULL) {
    Status = EFI_INVALID_PARAMETER;
    goto  EXIT;
  }

  //
  // ReadWrite: TRUE FOR READ FALSE FOR WRITE
  //
  if (ReadWrite) {
    for (Index = 0; Index < BufferSize / 2; Index++) {
      Status = Ax88772SromRead (NicDevice,
                                 (UINT32)(Offset/2 + Index),
                                 (((UINT16*)Buffer) + Index));
    }
  } else {
    Status = Ax88772EnableSromWrite(NicDevice);
    if (EFI_ERROR(Status))
      goto EXIT;

    for (Index = 0; Index < BufferSize / 2; Index++) {
      Status = Ax88772SromWrite (NicDevice,
                                  (UINT32)(Offset/2 + Index),
                                  (((UINT16*)Buffer) + Index));
    }

    Status = Ax88772DisableSromWrite(NicDevice);

    if (BufferSize == 272)
      Status = Ax88772ReloadSrom(NicDevice);
  }

  //
  // Return the operation status
  //
EXIT:
  gBS->RestoreTPL (TplPrevious);
  return Status;
}

/**
  Resets the network adapter and allocates the transmit and receive buffers
  required by the network interface; optionally, also requests allocation of
  additional transmit and receive buffers.  This routine must be called before
  any other routine in the Simple Network protocol is called.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [in] ExtraRxBufferSize Size in bytes to add to the receive buffer allocation
  @param [in] ExtraTxBufferSize Size in bytes to add to the transmit buffer allocation

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_OUT_OF_RESOURCES  There was not enough memory for the transmit and receive buffers
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_Initialize (
  IN EFI_SIMPLE_NETWORK_PROTOCOL * SimpleNetwork,
  IN UINTN ExtraRxBufferSize,
  IN UINTN ExtraTxBufferSize
  )
{
  EFI_SIMPLE_NETWORK_MODE *Mode;
  EFI_STATUS              Status;
  UINT32                  TmpState;
  EFI_TPL                 TplPrevious;
  NIC_DEVICE              *NicDevice;

  TplPrevious = gBS->RaiseTPL (TPL_CALLBACK);

  //
  // Verify the parameters
  //
  if ((SimpleNetwork != NULL) && (SimpleNetwork->Mode != NULL)) {
    //
    // Determine if the interface is already started
    //
    Mode = SimpleNetwork->Mode;
    if (EfiSimpleNetworkStarted == Mode->State) {
      if ((0 == ExtraRxBufferSize) && (0 == ExtraTxBufferSize)) {
        //
        // Start the adapter
        //
        TmpState = Mode->State;
        Mode->State = EfiSimpleNetworkInitialized;
        Status = SN_Reset (SimpleNetwork, FALSE);
        if (EFI_ERROR (Status)) {
          //
          // Update the network state
          //
          Mode->State = TmpState; // EfiSimpleNetworkInitialized;
        } else {
          Mode->MediaPresentSupported = TRUE;
          NicDevice = DEV_FROM_SIMPLE_NETWORK (SimpleNetwork);
          Mode->MediaPresent = Ax88772GetLinkStatus (NicDevice);
        }
      } else {
        Status = EFI_UNSUPPORTED;
      }
    } else {
      Status = EFI_NOT_STARTED;
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
  }

  //
  // Return the operation status
  //
  gBS->RestoreTPL (TplPrevious);
  return Status;
}


/**
  This function converts a multicast IP address to a multicast HW MAC address
  for all packet transactions.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [in] IPv6             Set to TRUE if the multicast IP address is IPv6 [RFC2460].
                                Set to FALSE if the multicast IP address is IPv4 [RFC 791].
  @param [in] IP               The multicast IP address that is to be converted to a
                                multicast HW MAC address.
  @param [in] MAC              The multicast HW MAC address that is to be generated from IP.

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_MCastIPtoMAC (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork,
  IN BOOLEAN                     IPv6,
  IN EFI_IP_ADDRESS              *IP,
  IN EFI_MAC_ADDRESS             *MAC
  )
{
  EFI_STATUS              Status;
  EFI_TPL                 TplPrevious;
  EFI_SIMPLE_NETWORK_MODE *Mode;

  TplPrevious = gBS->RaiseTPL(TPL_CALLBACK);
  if ((SimpleNetwork != NULL) && (SimpleNetwork->Mode != NULL)) {
    //
    // The interface must be running
    //
    Mode = SimpleNetwork->Mode;
    if (EfiSimpleNetworkInitialized == Mode->State) {
      if (IP == NULL || MAC == NULL) {
        Status = EFI_INVALID_PARAMETER;
        goto EXIT;
      }
      if (IPv6) {
        Status = EFI_UNSUPPORTED;
        goto EXIT;
      } else {
        //
        // check if the ip given is a mcast IP
        //
        if ((IP->v4.Addr[0] & 0xF0) != 0xE0) {
          Status = EFI_INVALID_PARAMETER;
          goto EXIT;
        } else {
          MAC->Addr[0] = 0x01;
          MAC->Addr[1] = 0x00;
          MAC->Addr[2] = 0x5e;
          MAC->Addr[3] = (UINT8) (IP->v4.Addr[1] & 0x7f);
          MAC->Addr[4] = (UINT8) IP->v4.Addr[2];
          MAC->Addr[5] = (UINT8) IP->v4.Addr[3];
          Status = EFI_SUCCESS;
        }
      }
    } else {
      if (EfiSimpleNetworkStarted == Mode->State) {
        Status = EFI_DEVICE_ERROR;
      } else {
        Status = EFI_NOT_STARTED ;
      }
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
  }

EXIT:
  gBS->RestoreTPL(TplPrevious);
  return Status;
}

/**
  Attempt to receive a packet from the network adapter.

  This function retrieves one packet from the receive queue of the network
  interface.  If there are no packets on the receive queue, then EFI_NOT_READY
  will be returned.  If there is a packet on the receive queue, and the size
  of the packet is smaller than BufferSize, then the contents of the packet
  will be placed in Buffer, and BufferSize will be udpated with the actual
  size of the packet.  In addition, if SrcAddr, DestAddr, and Protocol are
  not NULL, then these values will be extracted from the media header and
  returned.  If BufferSize is smaller than the received packet, then the
  size of the receive packet will be placed in BufferSize and
  EFI_BUFFER_TOO_SMALL will be returned.

  This routine calls ::Ax88179Rx to update the media status and
  empty the network adapter of receive packets.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [out] HeaderSize      The size, in bytes, of the media header to be filled in by
                                the Transmit() function.  If HeaderSize is non-zero, then
                                it must be equal to SimpleNetwork->Mode->MediaHeaderSize
                                and DestAddr and Protocol parameters must not be NULL.
  @param [out] BufferSize      The size, in bytes, of the entire packet (media header and
                                data) to be transmitted through the network interface.
  @param [out] Buffer          A pointer to the packet (media header followed by data) to
                                to be transmitted.  This parameter can not be NULL.  If
                                HeaderSize is zero, then the media header is Buffer must
                                already be filled in by the caller.  If HeaderSize is nonzero,
                                then the media header will be filled in by the Transmit()
                                function.
  @param [out] SrcAddr         The source HW MAC address.  If HeaderSize is zero, then
                                this parameter is ignored.  If HeaderSize is nonzero and
                                SrcAddr is NULL, then SimpleNetwork->Mode->CurrentAddress
                                is used for the source HW MAC address.
  @param [out] DestAddr        The destination HW MAC address.  If HeaderSize is zero, then
                                this parameter is ignored.
  @param [out] Protocol        The type of header to build.  If HeaderSize is zero, then
                                this parameter is ignored.

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_NOT_READY         No packets have been received on the network interface.
  @retval EFI_BUFFER_TOO_SMALL  The packet is larger than BufferSize bytes.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.

**/
EFI_STATUS
EFIAPI
SN_Receive (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork,
  OUT UINTN                       *HeaderSize,
  OUT UINTN                       *BufferSize,
  OUT VOID                        *Buffer,
  OUT EFI_MAC_ADDRESS             *SrcAddr,
  OUT EFI_MAC_ADDRESS             *DestAddr,
  OUT UINT16                      *Protocol
  )
{
  ETHERNET_HEADER         *Header;
  EFI_SIMPLE_NETWORK_MODE *Mode;
  NIC_DEVICE              *NicDevice = NULL;
  EFI_STATUS              Status;
  EFI_TPL                 TplPrevious;
  UINT16                  Type;
  UINT16                  CurrentPktLen;


  TplPrevious = gBS->RaiseTPL (TPL_CALLBACK);
  //
  // Verify the parameters
  //
  if ((SimpleNetwork != NULL) && (SimpleNetwork->Mode != NULL)) {
    //
    // The interface must be running
    //
    Mode = SimpleNetwork->Mode;
    if (EfiSimpleNetworkInitialized == Mode->State) {
      if ((BufferSize == NULL) || (Buffer == NULL)) {
        Status = EFI_INVALID_PARAMETER;
        gBS->RestoreTPL (TplPrevious);
        return Status;
      }

      //
      // Update the link status
      //
      NicDevice = DEV_FROM_SIMPLE_NETWORK (SimpleNetwork);

      if (NicDevice->LinkUp && NicDevice->Complete) {
        if ((HeaderSize != NULL) && (*HeaderSize == 7720)) {
          NicDevice->Grub_f = TRUE;
        }

        if ((NicDevice->Grub_f) && (*HeaderSize != 7720)) {
          gBS->RestoreTPL (TplPrevious);
          return EFI_NOT_READY;
        }

        //
        //  Attempt to receive a packet
        //
        if (0 == NicDevice->PktCnt) {
          Status = Ax88772BulkIn(NicDevice);
          if (EFI_ERROR(Status)) {
            goto  no_pkt;
          }
        }

        CurrentPktLen = *((UINT16*) (NicDevice->CurPktHdrOff));
        CurrentPktLen &=  0x7ff;

        if ((60 <= CurrentPktLen) &&
            (CurrentPktLen - 14 <= MAX_ETHERNET_PKT_SIZE)) {
            if (*BufferSize < (UINTN)CurrentPktLen) {
              gBS->RestoreTPL (TplPrevious);
              return EFI_BUFFER_TOO_SMALL;
            }

            *BufferSize = CurrentPktLen;
            CopyMem (Buffer, NicDevice->CurPktOff, CurrentPktLen);
            Header = (ETHERNET_HEADER *) NicDevice->CurPktOff;

            if ((HeaderSize != NULL)  && (*HeaderSize != 7720)) {
              *HeaderSize = sizeof (*Header);
            }
            if (DestAddr != NULL) {
              CopyMem (DestAddr, &Header->DestAddr, PXE_HWADDR_LEN_ETHER);
            }
            if (SrcAddr != NULL) {
              CopyMem (SrcAddr, &Header->SrcAddr, PXE_HWADDR_LEN_ETHER);
            }
            if (Protocol != NULL) {
              Type = Header->Type;
              Type = (UINT16)((Type >> 8) | (Type << 8));
              *Protocol = Type;
            }
            NicDevice->PktCnt--;
            NicDevice->CurPktHdrOff += (CurrentPktLen + 4 + 1) & 0xfffe;
            NicDevice->CurPktOff = NicDevice->CurPktHdrOff + 4;
            Status = EFI_SUCCESS;
        } else {
          NicDevice->PktCnt = 0;
          Status = EFI_DEVICE_ERROR;
        }
      } else {
        //
        //  Link no up
        //
        Status = EFI_NOT_READY;
      }
    } else {
      if (EfiSimpleNetworkStarted == Mode->State) {
        Status = EFI_DEVICE_ERROR;
      } else {
        Status = EFI_NOT_STARTED;
      }
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
  }

  //
  // Return the operation status
  //
no_pkt:
  gBS->RestoreTPL (TplPrevious);
  return Status;
}

/**
  This function is used to enable and disable the hardware and software receive
  filters for the underlying network device.

  The receive filter change is broken down into three steps:

    1.  The filter mask bits that are set (ON) in the Enable parameter
        are added to the current receive filter settings.

    2.  The filter mask bits that are set (ON) in the Disable parameter
        are subtracted from the updated receive filter settins.

    3.  If the resulting filter settigns is not supported by the hardware
        a more liberal setting is selected.

  If the same bits are set in the Enable and Disable parameters, then the bits
  in the Disable parameter takes precedence.

  If the ResetMCastFilter parameter is TRUE, then the multicast address list
  filter is disabled (irregardless of what other multicast bits are set in
  the enable and Disable parameters).  The SNP->Mode->MCastFilterCount field
  is set to zero.  The SNP->Mode->MCastFilter contents are undefined.

  After enableing or disabling receive filter settings, software should
  verify the new settings by checking the SNP->Mode->ReceeiveFilterSettings,
  SNP->Mode->MCastFilterCount and SNP->Mode->MCastFilter fields.

  Note: Some network drivers and/or devices will automatically promote
  receive filter settings if the requested setting can not be honored.
  For example, if a request for four multicast addresses is made and
  the underlying hardware only supports two multicast addresses the
  driver might set the promiscuous or promiscuous multicast receive filters
  instead.  The receiving software is responsible for discarding any extra
  packets that get through the hardware receive filters.

  If ResetMCastFilter is TRUE, then the multicast receive filter list
  on the network interface will be reset to the default multicast receive
  filter list.  If ResetMCastFilter is FALSE, and this network interface
  allows the multicast receive filter list to be modified, then the
  MCastFilterCnt and MCastFilter are used to update the current multicast
  receive filter list.  The modified receive filter list settings can be
  found in the MCastFilter field of EFI_SIMPLE_NETWORK_MODE.

  This routine calls ::ReceiveFilterUpdate to update the receive
  state in the network adapter.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [in] Enable            A bit mask of receive filters to enable on the network interface.
  @param [in] Disable           A bit mask of receive filters to disable on the network interface.
                                For backward compatibility with EFI 1.1 platforms, the
                                EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST bit must be set
                                when the ResetMCastFilter parameter is TRUE.
  @param [in] ResetMCastFilter Set to TRUE to reset the contents of the multicast receive
                                filters on the network interface to their default values.
  @param [in] MCastFilterCnt    Number of multicast HW MAC address in the new MCastFilter list.
                                This value must be less than or equal to the MaxMCastFilterCnt
                                field of EFI_SIMPLE_NETWORK_MODE.  This field is optional if
                                ResetMCastFilter is TRUE.
  @param [in] MCastFilter      A pointer to a list of new multicast receive filter HW MAC
                                addresses.  This list will replace any existing multicast
                                HW MAC address list.  This field is optional if ResetMCastFilter
                                is TRUE.

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_ReceiveFilters (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork,
  IN UINT32                      Enable,
  IN UINT32                      Disable,
  IN BOOLEAN                     ResetMCastFilter,
  IN UINTN                       MCastFilterCnt,
  IN EFI_MAC_ADDRESS             *MCastFilter
  )
{
  EFI_SIMPLE_NETWORK_MODE *Mode;
  EFI_STATUS              Status = EFI_SUCCESS;
  EFI_TPL                 TplPrevious;
  UINTN                   Index;
  UINT8                   Temp;

  TplPrevious = gBS->RaiseTPL(TPL_CALLBACK);
  Mode = SimpleNetwork->Mode;

  if (SimpleNetwork == NULL) {
    gBS->RestoreTPL(TplPrevious);
    return EFI_INVALID_PARAMETER;
  }

  switch (Mode->State) {
  case EfiSimpleNetworkInitialized:
    break;

  case EfiSimpleNetworkStopped:
    Status = EFI_NOT_STARTED;
    gBS->RestoreTPL(TplPrevious);
    return Status;

  default:
    Status = EFI_DEVICE_ERROR;
    gBS->RestoreTPL(TplPrevious);
    return Status;
  }

  //
  // check if we are asked to enable or disable something that the SNP
  // does not even support!
  //
  if (((Enable &~Mode->ReceiveFilterMask) != 0) ||
      ((Disable &~Mode->ReceiveFilterMask) != 0)) {
    Status = EFI_INVALID_PARAMETER;
    gBS->RestoreTPL(TplPrevious);
    return Status;
  }

  if (ResetMCastFilter) {
    if ((0 == (Mode->ReceiveFilterSetting & EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST)) &&
          Enable == 0 &&
          Disable == 2) {
      gBS->RestoreTPL(TplPrevious);
      return EFI_SUCCESS;
    }
    Mode->MCastFilterCount = 0;
    SetMem (&Mode->MCastFilter[0],
             sizeof(EFI_MAC_ADDRESS) * MAX_MCAST_FILTER_CNT,
             0);
  } else {
    if (MCastFilterCnt != 0) {
      EFI_MAC_ADDRESS * MulticastAddress;
      MulticastAddress =  MCastFilter;

      if ((MCastFilterCnt > Mode->MaxMCastFilterCount) ||
          (MCastFilter == NULL)) {
        Status = EFI_INVALID_PARAMETER;
        gBS->RestoreTPL(TplPrevious);
        return Status;
      }

      for (Index = 0 ; Index < MCastFilterCnt ; Index++) {
          Temp = MulticastAddress->Addr[0];
          if ((Temp & 0x01) != 0x01) {
            gBS->RestoreTPL(TplPrevious);
            return EFI_INVALID_PARAMETER;
          }
          MulticastAddress++;
      }

      Mode->MCastFilterCount = (UINT32)MCastFilterCnt;
      CopyMem (&Mode->MCastFilter[0],
               MCastFilter,
               MCastFilterCnt * sizeof (EFI_MAC_ADDRESS));
    }
  }

  if (Enable == 0 && Disable == 0 && !ResetMCastFilter && MCastFilterCnt == 0) {
    Status = EFI_SUCCESS;
    gBS->RestoreTPL(TplPrevious);
    return Status;
  }

  if ((Enable & EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST) != 0 && MCastFilterCnt == 0) {
    Status = EFI_INVALID_PARAMETER;
    gBS->RestoreTPL(TplPrevious);
    return Status;
  }

  Mode->ReceiveFilterSetting |= Enable;
  Mode->ReceiveFilterSetting &= ~Disable;

  Status = ReceiveFilterUpdate (SimpleNetwork);

  if (EFI_DEVICE_ERROR == Status || EFI_INVALID_PARAMETER == Status)
    Status = EFI_SUCCESS;

  gBS->RestoreTPL(TplPrevious);

  return Status;
}

/**
  Reset the network adapter.

  Resets a network adapter and reinitializes it with the parameters that
  were provided in the previous call to Initialize ().  The transmit and
  receive queues are cleared.  Receive filters, the station address, the
  statistics, and the multicast-IP-to-HW MAC addresses are not reset by
  this call.

  This routine calls ::Ax88772Reset to perform the adapter specific
  reset operation.  This routine also starts the link negotiation
  by calling ::Ax88772NegotiateLinkStart.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [in] ExtendedVerification  Indicates that the driver may perform a more
                                exhaustive verification operation of the device
                                during reset.

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_Reset (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork,
  IN BOOLEAN                     ExtendedVerification
  )
{
  EFI_SIMPLE_NETWORK_MODE *Mode;
  NIC_DEVICE              *NicDevice;
  EFI_STATUS              Status;
  EFI_TPL                 TplPrevious;

  TplPrevious = gBS->RaiseTPL(TPL_CALLBACK);
  //
  //  Verify the parameters
  //
  if ((SimpleNetwork != NULL) && (SimpleNetwork->Mode != NULL)) {
    Mode = SimpleNetwork->Mode;
    if (EfiSimpleNetworkInitialized == Mode->State) {
      //
      //  Update the device state
      //
      NicDevice = DEV_FROM_SIMPLE_NETWORK (SimpleNetwork);

      //
      //  Reset the device
      //
      if (!NicDevice->FirstRst) {
        Status = EFI_SUCCESS;
      } else {
        Status = Ax88772Reset (NicDevice);
        if (!EFI_ERROR (Status)) {
          Status = ReceiveFilterUpdate (SimpleNetwork);
          if (!EFI_ERROR (Status) && !NicDevice->LinkUp && NicDevice->FirstRst) {
            Status = Ax88772NegotiateLinkStart (NicDevice);
            NicDevice->FirstRst = FALSE;
          }
        }
      }
    } else {
      Status = EFI_NOT_STARTED;
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
  }
  //
  // Return the operation status
  //
  gBS->RestoreTPL(TplPrevious);
  return Status;
}

/**
  Initialize the simple network protocol.

  This routine calls ::Ax88772MacAddressGet to obtain the
  MAC address.

  @param [in] NicDevice       NIC_DEVICE_INSTANCE pointer

  @retval EFI_SUCCESS     Setup was successful

**/
EFI_STATUS
SN_Setup (
  IN NIC_DEVICE *NicDevice
  )
{
  EFI_SIMPLE_NETWORK_MODE     *Mode;
  EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork;
  EFI_STATUS                  Status;

  //
  // Initialize the simple network protocol
  //
  SimpleNetwork = &NicDevice->SimpleNetwork;
  SimpleNetwork->Revision = EFI_SIMPLE_NETWORK_PROTOCOL_REVISION;
  SimpleNetwork->Start = SN_Start;
  SimpleNetwork->Stop = SN_Stop;
  SimpleNetwork->Initialize = SN_Initialize;
  SimpleNetwork->Reset = SN_Reset;
  SimpleNetwork->Shutdown = SN_Shutdown;
  SimpleNetwork->ReceiveFilters = SN_ReceiveFilters;
  SimpleNetwork->StationAddress = SN_StationAddress;
  SimpleNetwork->Statistics = SN_Statistics;
  SimpleNetwork->MCastIpToMac = SN_MCastIPtoMAC;
  SimpleNetwork->NvData = SN_NvData;
  SimpleNetwork->GetStatus = SN_GetStatus;
  SimpleNetwork->Transmit = SN_Transmit;
  SimpleNetwork->Receive = SN_Receive;
  SimpleNetwork->WaitForPacket = NULL;
  Mode = &NicDevice->SimpleNetworkData;
  SimpleNetwork->Mode = Mode;
  Mode->State = EfiSimpleNetworkStopped;
  Mode->HwAddressSize = PXE_HWADDR_LEN_ETHER;
  Mode->MediaHeaderSize = sizeof (ETHERNET_HEADER);
  Mode->MaxPacketSize = MAX_ETHERNET_PKT_SIZE;
  Mode->ReceiveFilterMask = EFI_SIMPLE_NETWORK_RECEIVE_UNICAST
                           | EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST
                           | EFI_SIMPLE_NETWORK_RECEIVE_BROADCAST
                           | EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS
                           | EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS_MULTICAST;
  Mode->ReceiveFilterSetting = EFI_SIMPLE_NETWORK_RECEIVE_UNICAST
                              | EFI_SIMPLE_NETWORK_RECEIVE_BROADCAST;
  Mode->MaxMCastFilterCount = ASIX_MCAST_FILTER_CNT;
  Mode->MCastFilterCount = 0;
  Mode->NvRamSize = 512;
  Mode->NvRamAccessSize = 2;
  SetMem (&Mode->BroadcastAddress,
           PXE_HWADDR_LEN_ETHER,
           0xff);

  SetMem (&Mode->MCastFilter[0],
           sizeof(EFI_MAC_ADDRESS) * MAX_MCAST_FILTER_CNT,
           0);

  Mode->IfType = NET_IFTYPE_ETHERNET;
  Mode->MacAddressChangeable = TRUE;
  Mode->MultipleTxSupported = FALSE;
  Mode->MediaPresentSupported = TRUE;
  Mode->MediaPresent = FALSE;

  //
  //  Read the MAC address
  //
  NicDevice->PhyId = PHY_ID_INTERNAL;
  NicDevice->LinkSpeed100Mbps = TRUE;
  NicDevice->FullDuplex = TRUE;
  NicDevice->Complete = FALSE;
  NicDevice->LinkUp = FALSE;
  NicDevice->Grub_f = FALSE;
  NicDevice->FirstRst = TRUE;
  NicDevice->PktCnt = 0;

  Status = Ax88772MacAddressGet (
                NicDevice,
                &Mode->PermanentAddress.Addr[0]);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //  Use the hardware address as the current address
  //
  CopyMem (&Mode->CurrentAddress,
            &Mode->PermanentAddress,
            PXE_HWADDR_LEN_ETHER);

   CopyMem (&NicDevice->MAC,
            &Mode->PermanentAddress,
            PXE_HWADDR_LEN_ETHER);

  Status = gBS->AllocatePool (EfiBootServicesData,
                                   AX88772_MAX_BULKIN_SIZE,
                                   (VOID **) &NicDevice->BulkInbuf);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->AllocatePool (EfiBootServicesData,
                                   sizeof (RX_TX_PACKET),
                                   (VOID **) &NicDevice->TxTest);

  if (EFI_ERROR (Status)) {
    gBS->FreePool (NicDevice->BulkInbuf);
    return Status;
  }

  //
  //  Return the setup status
  //
  return Status;
}


/**
  This routine starts the network interface.

  @param [in] SimpleNetwork    Protocol instance pointer

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_ALREADY_STARTED   The network interface was already started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_Start (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork
  )
{
  NIC_DEVICE              *NicDevice;
  EFI_SIMPLE_NETWORK_MODE *Mode;
  EFI_STATUS              Status;
  EFI_TPL                 TplPrevious;

  TplPrevious = gBS->RaiseTPL(TPL_CALLBACK);
  //
  // Verify the parameters
  //
  Status = EFI_INVALID_PARAMETER;
  if ((SimpleNetwork != NULL) && (SimpleNetwork->Mode != NULL)) {
    Mode = SimpleNetwork->Mode;
    if (EfiSimpleNetworkStopped == Mode->State) {
      //
      // Initialize the mode structure
      // NVRAM access is not supported
      //
      ZeroMem (Mode, sizeof (*Mode));

      Mode->State = EfiSimpleNetworkStarted;
      Mode->HwAddressSize = PXE_HWADDR_LEN_ETHER;
      Mode->MediaHeaderSize = sizeof (ETHERNET_HEADER);
      Mode->MaxPacketSize = MAX_ETHERNET_PKT_SIZE;
      Mode->ReceiveFilterMask = EFI_SIMPLE_NETWORK_RECEIVE_UNICAST
                               | EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST
                               | EFI_SIMPLE_NETWORK_RECEIVE_BROADCAST
                               | EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS
                               | EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS_MULTICAST;
      Mode->ReceiveFilterSetting = EFI_SIMPLE_NETWORK_RECEIVE_UNICAST;
      Mode->MaxMCastFilterCount = ASIX_MCAST_FILTER_CNT;
      Mode->MCastFilterCount = 0;

      SetMem (&Mode->MCastFilter[0],
               sizeof(EFI_MAC_ADDRESS) * MAX_MCAST_FILTER_CNT,
               0);

      Mode->NvRamSize = 512;
      Mode->NvRamAccessSize = 2;
      NicDevice = DEV_FROM_SIMPLE_NETWORK (SimpleNetwork);
      Status = Ax88772MacAddressGet (NicDevice, &Mode->PermanentAddress.Addr[0]);
      CopyMem (&Mode->CurrentAddress,
                &Mode->PermanentAddress,
                sizeof (Mode->CurrentAddress));
      SetMem(&Mode->BroadcastAddress, PXE_HWADDR_LEN_ETHER, 0xff);
      Mode->IfType = NET_IFTYPE_ETHERNET;
      Mode->MacAddressChangeable = TRUE;
      Mode->MultipleTxSupported = FALSE;
      Mode->MediaPresentSupported = TRUE;
      Mode->MediaPresent = FALSE;

    } else {
      Status = EFI_ALREADY_STARTED;
    }
  }

  //
  // Return the operation status
  //
  gBS->RestoreTPL(TplPrevious);
  return Status;
}


/**
  Set the MAC address.

  This function modifies or resets the current station address of a
  network interface.  If Reset is TRUE, then the current station address
  is set ot the network interface's permanent address.  If Reset if FALSE
  then the current station address is changed to the address specified by
  New.

  This routine calls ::Ax88772MacAddressSet to update the MAC address
  in the network adapter.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [in] Reset            Flag used to reset the station address to the
                                network interface's permanent address.
  @param [in] New              New station address to be used for the network
                                interface.

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_StationAddress (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork,
  IN BOOLEAN                     Reset,
  IN EFI_MAC_ADDRESS             *New
  )
{
  NIC_DEVICE              *NicDevice;
  EFI_SIMPLE_NETWORK_MODE *Mode;
  EFI_STATUS              Status;

  EFI_TPL TplPrevious;

  TplPrevious = gBS->RaiseTPL(TPL_CALLBACK);
  //
  // Verify the parameters
  //
  if ((SimpleNetwork != NULL) &&
      (SimpleNetwork->Mode != NULL) &&
      ((!Reset) || (Reset && (New != NULL)))) {
    //
    // Verify that the adapter is already started
    //
    NicDevice = DEV_FROM_SIMPLE_NETWORK (SimpleNetwork);
    Mode = SimpleNetwork->Mode;
    if (EfiSimpleNetworkInitialized == Mode->State) {
      //
      // Determine the adapter MAC address
      //
      if (Reset) {
        //
        // Use the permanent address
        //
        CopyMem (&Mode->CurrentAddress,
                  &Mode->PermanentAddress,
                  sizeof (Mode->CurrentAddress));
      } else {
        //
        // Use the specified address
        //
        CopyMem (&Mode->CurrentAddress,
                  New,
                  sizeof (Mode->CurrentAddress));
      }

      //
      // Update the address on the adapter
      //
      Status = Ax88772MacAddressSet (NicDevice, &Mode->CurrentAddress.Addr[0]);
    } else {
      if (EfiSimpleNetworkStarted == Mode->State) {
        Status = EFI_DEVICE_ERROR; ;
      } else {
        Status = EFI_NOT_STARTED ;
      }
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
  }

  //
  // Return the operation status
  //
  gBS->RestoreTPL(TplPrevious);
  return Status;
}


/**
  This function resets or collects the statistics on a network interface.
  If the size of the statistics table specified by StatisticsSize is not
  big enough for all of the statistics that are collected by the network
  interface, then a partial buffer of statistics is returned in
  StatisticsTable.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [in] Reset            Set to TRUE to reset the statistics for the network interface.
  @param [in, out] StatisticsSize  On input the size, in bytes, of StatisticsTable.  On output
                                the size, in bytes, of the resulting table of statistics.
  @param [out] StatisticsTable A pointer to the EFI_NETWORK_STATISTICS structure that
                                conains the statistics.

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_BUFFER_TOO_SMALL  The StatisticsTable is NULL or the buffer is too small.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.
**/
EFI_STATUS
EFIAPI
SN_Statistics (
  IN     EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork,
  IN     BOOLEAN                     Reset,
  IN OUT UINTN                       *StatisticsSize,
  OUT    EFI_NETWORK_STATISTICS      *StatisticsTable
  )
{
  EFI_STATUS              Status;
  EFI_TPL                 TplPrevious;
  EFI_SIMPLE_NETWORK_MODE *Mode;

  TplPrevious = gBS->RaiseTPL(TPL_CALLBACK);
  Mode = SimpleNetwork->Mode;

  if (EfiSimpleNetworkInitialized == Mode->State) {
    if ((StatisticsSize != NULL) && (*StatisticsSize == 0)) {
      Status = EFI_BUFFER_TOO_SMALL;
      goto EXIT;
    }

    if(Reset) {
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_SUCCESS;
    }
  } else {
    if (EfiSimpleNetworkStarted == Mode->State) {
      Status = EFI_DEVICE_ERROR; ;
    } else {
      Status = EFI_NOT_STARTED ;
    }
  }
  //
  // This is not currently supported
  //

EXIT:
  gBS->RestoreTPL(TplPrevious);
  return Status;
}


/**
  This function stops a network interface.  This call is only valid
  if the network interface is in the started state.

  @param [in] SimpleNetwork    Protocol instance pointer

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_Stop (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork
  )
{
  EFI_SIMPLE_NETWORK_MODE *Mode;
  EFI_STATUS              Status;
  EFI_TPL                 TplPrevious;

  TplPrevious = gBS->RaiseTPL(TPL_CALLBACK);
  //
  // Verify the parameters
  //
  if ((SimpleNetwork != NULL) && (SimpleNetwork->Mode != NULL)) {
    //
    // Determine if the interface is started
    //
    Mode = SimpleNetwork->Mode;

    if (EfiSimpleNetworkStarted == Mode->State) {
        Mode->State = EfiSimpleNetworkStopped;
        Status = EFI_SUCCESS;
    } else {
        Status = EFI_NOT_STARTED;
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
  }

  //
  // Return the operation status
  //
  gBS->RestoreTPL(TplPrevious);
  return Status;
}


/**
  This function releases the memory buffers assigned in the Initialize() call.
  Ending transmits and receives are lost, and interrupts are cleared and disabled.
  After this call, only Initialize() and Stop() calls may be used.

  @param [in] SimpleNetwork    Protocol instance pointer

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SN_Shutdown (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork
  )
{
  EFI_SIMPLE_NETWORK_MODE *Mode;
  UINT32                  RxFilter;
  EFI_STATUS              Status;
  EFI_TPL                 TplPrevious;

  TplPrevious = gBS->RaiseTPL(TPL_CALLBACK);
  //
  // Verify the parameters
  //
  if ((SimpleNetwork != NULL) && (SimpleNetwork->Mode != NULL)) {
    //
    // Determine if the interface is already started
    //
    Mode = SimpleNetwork->Mode;
    if (EfiSimpleNetworkInitialized == Mode->State) {
      //
      // Stop the adapter
      //
      RxFilter = Mode->ReceiveFilterSetting;
      Mode->ReceiveFilterSetting = 0;
      Status = SN_Reset (SimpleNetwork, FALSE);
      Mode->ReceiveFilterSetting = RxFilter;
      if (!EFI_ERROR (Status)) {
        //
        // Update the network state
        //
        Mode->State = EfiSimpleNetworkStarted;
      } else if (EFI_DEVICE_ERROR == Status) {
        Mode->State = EfiSimpleNetworkStopped;
      }

    } else {
      Status = EFI_NOT_STARTED;
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
  }

  //
  // Return the operation status
  //
  gBS->RestoreTPL(TplPrevious);
  return Status;
}


/**
  Send a packet over the network.

  This function places the packet specified by Header and Buffer on
  the transmit queue.  This function performs a non-blocking transmit
  operation.  When the transmit is complete, the buffer is returned
  via the GetStatus() call.

  This routine calls ::Ax88772Rx to empty the network adapter of
  receive packets.  The routine then passes the transmit packet
  to the network adapter.

  @param [in] SimpleNetwork    Protocol instance pointer
  @param [in] HeaderSize        The size, in bytes, of the media header to be filled in by
                                the Transmit() function.  If HeaderSize is non-zero, then
                                it must be equal to SimpleNetwork->Mode->MediaHeaderSize
                                and DestAddr and Protocol parameters must not be NULL.
  @param [in] BufferSize        The size, in bytes, of the entire packet (media header and
                                data) to be transmitted through the network interface.
  @param [in] Buffer           A pointer to the packet (media header followed by data) to
                                to be transmitted.  This parameter can not be NULL.  If
                                HeaderSize is zero, then the media header is Buffer must
                                already be filled in by the caller.  If HeaderSize is nonzero,
                                then the media header will be filled in by the Transmit()
                                function.
  @param [in] SrcAddr          The source HW MAC address.  If HeaderSize is zero, then
                                this parameter is ignored.  If HeaderSize is nonzero and
                                SrcAddr is NULL, then SimpleNetwork->Mode->CurrentAddress
                                is used for the source HW MAC address.
  @param [in] DestAddr         The destination HW MAC address.  If HeaderSize is zero, then
                                this parameter is ignored.
  @param [in] Protocol         The type of header to build.  If HeaderSize is zero, then
                                this parameter is ignored.

  @retval EFI_SUCCESS           This operation was successful.
  @retval EFI_NOT_STARTED       The network interface was not started.
  @retval EFI_NOT_READY         The network interface is too busy to accept this transmit request.
  @retval EFI_BUFFER_TOO_SMALL  The BufferSize parameter is too small.
  @retval EFI_INVALID_PARAMETER SimpleNetwork parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.

**/
EFI_STATUS
EFIAPI
SN_Transmit (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *SimpleNetwork,
  IN UINTN                       HeaderSize,
  IN UINTN                       BufferSize,
  IN VOID                        *Buffer,
  IN EFI_MAC_ADDRESS             *SrcAddr,
  IN EFI_MAC_ADDRESS             *DestAddr,
  IN UINT16                      *Protocol
  )
{
  ETHERNET_HEADER         *Header;
  EFI_SIMPLE_NETWORK_MODE *Mode;
  NIC_DEVICE              *NicDevice;
  EFI_USB_IO_PROTOCOL     *UsbIo;
  EFI_STATUS              Status;
  UINTN                   TransferLength;
  UINT32                  TransferStatus;
  UINT16                  Type;
  EFI_TPL                 TplPrevious;

  TplPrevious = gBS->RaiseTPL(TPL_CALLBACK);
  //
  // Verify the parameters
  //
  if ((SimpleNetwork != NULL) && (SimpleNetwork->Mode != NULL)) {
    //
    // The interface must be running
    //
    Mode = SimpleNetwork->Mode;
    if (EfiSimpleNetworkInitialized == Mode->State) {
      //
      // Update the link status
      //
      NicDevice = DEV_FROM_SIMPLE_NETWORK (SimpleNetwork);

      //
      //  Release the synchronization with Ax88772Timer
      //
      if (NicDevice->LinkUp && NicDevice->Complete) {
        //
        //  Copy the packet into the USB buffer
        //

        if ((HeaderSize != 0) && (Mode->MediaHeaderSize != HeaderSize))  {
          Status = EFI_INVALID_PARAMETER;
          goto EXIT;
        }
        if (BufferSize <  Mode->MediaHeaderSize) {
          Status = EFI_INVALID_PARAMETER;
          goto EXIT;
        }
        if (Buffer == NULL) {
          Status = EFI_INVALID_PARAMETER;
          goto EXIT;
        }
        if ((HeaderSize != 0) && (DestAddr == NULL)) {
          Status = EFI_INVALID_PARAMETER;
          goto EXIT;
        }
        if ((HeaderSize != 0) && (Protocol == NULL)) {
          Status = EFI_INVALID_PARAMETER;
          goto EXIT;
        }

        CopyMem (&NicDevice->TxTest->Data[0], Buffer, BufferSize);
        NicDevice->TxTest->Length = (UINT16) BufferSize;

        //
        //  Transmit the packet
        //
        Header = (ETHERNET_HEADER *) &NicDevice->TxTest->Data[0];
        if (HeaderSize != 0) {
          if (DestAddr != NULL) {
            CopyMem (&Header->DestAddr, DestAddr, PXE_HWADDR_LEN_ETHER);
          }
          if (SrcAddr != NULL) {
            CopyMem (&Header->SrcAddr, SrcAddr, PXE_HWADDR_LEN_ETHER);
          } else {
            CopyMem (&Header->SrcAddr, &Mode->CurrentAddress.Addr[0], PXE_HWADDR_LEN_ETHER);
          }
          if (Protocol != NULL) {
            Type = *Protocol;
          } else {
            Type = NicDevice->TxTest->Length;
          }
          Type = (UINT16)((Type >> 8) | (Type << 8));
          Header->Type = Type;
        }


        if (NicDevice->TxTest->Length < MIN_ETHERNET_PKT_SIZE) {
          NicDevice->TxTest->Length = MIN_ETHERNET_PKT_SIZE;
          ZeroMem (&NicDevice->TxTest->Data[BufferSize],
                    NicDevice->TxTest->Length - BufferSize);
        }

        NicDevice->TxTest->LengthBar = ~(NicDevice->TxTest->Length);
        TransferLength = sizeof (NicDevice->TxTest->Length)
                       + sizeof (NicDevice->TxTest->LengthBar)
                       + NicDevice->TxTest->Length;

        if (TransferLength % 512 == 0 || TransferLength % 1024 == 0)
          TransferLength +=4;
#if RXTHOU
        if (NicDevice->RxBurst == 1)
          NicDevice->RxBurst--;
#endif
        //
        //  Work around USB bus driver bug where a timeout set by receive
        //  succeeds but the timeout expires immediately after, causing the
        //  transmit operation to timeout.
        //
        UsbIo = NicDevice->UsbIo;
        Status = UsbIo->UsbBulkTransfer (UsbIo,
                                           BULK_OUT_ENDPOINT,
                                           &NicDevice->TxTest->Length,
                                           &TransferLength,
                                           0xfffffffe,
                                           &TransferStatus);
        if (EFI_SUCCESS == Status) {
          Status = TransferStatus;
        }
        if (EFI_SUCCESS == Status && EFI_SUCCESS == TransferStatus) {
          NicDevice->TxBuffer = Buffer;
        } else {
          if (EFI_DEVICE_ERROR == Status) {
            SN_Reset (SimpleNetwork, FALSE);
          }
          Status = EFI_NOT_READY;
        }
      } else {
        //
        // No packets available.
        //
        Status = EFI_NOT_READY;
      }
    } else {
      if (EfiSimpleNetworkStarted == Mode->State) {
        Status = EFI_DEVICE_ERROR;
      } else {
        Status = EFI_NOT_STARTED;
      }
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
  }

EXIT:
  gBS->RestoreTPL (TplPrevious);
  return Status;
}
