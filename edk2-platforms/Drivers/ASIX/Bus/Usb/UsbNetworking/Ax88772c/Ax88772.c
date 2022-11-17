/** @file
  Implement the interface to the AX88772 Ethernet controller.

  This module implements the interface to the ASIX AX88772
  USB to Ethernet MAC with integrated 10/100 PHY.  Note that this implementation
  only supports the integrated PHY since no other test cases were available.

  Copyright (c) 2011, Intel Corporation. All rights reserved.
  Copyright (c) 2020, ARM Limited. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Ax88772.h"


/**
  Compute the CRC

  @param [in] MacAddress      Address of a six byte buffer to containing the MAC address.

  @returns The CRC-32 value associated with this MAC address

**/
UINT32
Ax88772Crc (
  IN UINT8 *MacAddress
  )
{
  UINT32 BitNumber;
  INT32  Carry;
  INT32  Crc;
  UINT32 Data;
  UINT8  *End;

  //
  //  Walk the MAC address
  //
  Crc = -1;
  End = &MacAddress[PXE_HWADDR_LEN_ETHER];
  while (End > MacAddress) {
    Data = *MacAddress++;


    //
    //  CRC32: x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
    //
    //          1 0000 0100 1100 0001 0001 1101 1011 0111
    //
    for (BitNumber = 0; 8 > BitNumber; BitNumber++) {
      Carry = ((Crc >> 31) & 1) ^ (Data & 1);
      Crc <<= 1;
      if (Carry != 0) {
        Crc ^= 0x04c11db7;
      }
      Data >>= 1;
    }
  }

  //
  //  Return the CRC value
  //
  return (UINT32) Crc;
}

/**
  Get the MAC address

  This routine calls ::Ax88772UsbCommand to request the MAC
  address from the network adapter.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [out] MacAddress      Address of a six byte buffer to receive the MAC address.

  @retval EFI_SUCCESS          The MAC address is available.
  @retval other                The MAC address is not valid.

**/
EFI_STATUS
Ax88772MacAddressGet (
  IN  NIC_DEVICE *NicDevice,
  OUT UINT8      *MacAddress
  )
{
  USB_DEVICE_REQUEST SetupMsg;
  EFI_STATUS Status;

  //
  //  Set the register address.
  //
  SetupMsg.RequestType = USB_ENDPOINT_DIR_IN
                       | USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_MAC_ADDRESS_READ;
  SetupMsg.Value = 0;
  SetupMsg.Index = 0;
  SetupMsg.Length = PXE_HWADDR_LEN_ETHER;

  //
  //  Read the PHY register
  //
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               MacAddress);

  //
  // Return the operation status
  //
  return Status;
}


/**
  Set the MAC address

  This routine calls ::Ax88772UsbCommand to set the MAC address
  in the network adapter.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in] MacAddress      Address of a six byte buffer to containing the new MAC address.

  @retval EFI_SUCCESS          The MAC address was set.
  @retval other                The MAC address was not set.

**/
EFI_STATUS
Ax88772MacAddressSet (
  IN NIC_DEVICE *NicDevice,
  IN UINT8      *MacAddress
  )
{
  USB_DEVICE_REQUEST SetupMsg;
  EFI_STATUS Status;

  //
  //  Set the register address.
  //
  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_MAC_ADDRESS_WRITE;
  SetupMsg.Value = 0;
  SetupMsg.Index = 0;
  SetupMsg.Length = PXE_HWADDR_LEN_ETHER;

  //
  //  Read the PHY register
  //
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               MacAddress);

  //
  // Return the operation status
  //
  return Status;
}

/**
  Clear the multicast hash table

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure

**/
VOID
Ax88772MulticastClear (
  IN NIC_DEVICE *NicDevice
  )
{
  int Index = 0;
  //
  // Clear the multicast hash table
  //
  for (Index = 0 ; Index < 8 ; Index ++)
    NicDevice->MulticastHash[Index] = 0;
}

/**
  Enable a multicast address in the multicast hash table

  This routine calls ::Ax88772Crc to compute the hash bit for
  this MAC address.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in] MacAddress      Address of a six byte buffer to containing the MAC address.

**/
VOID
Ax88772MulticastSet (
  IN NIC_DEVICE *NicDevice,
  IN UINT8      *MacAddress
  )
{
  UINT32 Crc;

  //
  // Compute the CRC on the destination address
  //
  Crc = Ax88772Crc (MacAddress) >> 26;

  //
  //  Set the bit corresponding to the destination address
  //
  NicDevice->MulticastHash [Crc >> 3] |= (1 << (Crc & 7));

}

/**
  Start the link negotiation

  This routine calls ::Ax88772PhyWrite to start the PHY's link
  negotiation.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure

  @retval EFI_SUCCESS          The link negotiation was started.
  @retval other                Failed to start the link negotiation.

**/
EFI_STATUS
Ax88772NegotiateLinkStart (
  IN NIC_DEVICE *NicDevice
  )
{
  UINT16     Control;
  EFI_STATUS Status;

  //
  // Set the supported capabilities.
  //
  Status = Ax88772PhyWrite (NicDevice,
                             PHY_ANAR,
                             AN_CSMA_CD
                             | AN_TX_FDX | AN_TX_HDX
                             | AN_10_FDX | AN_10_HDX | AN_FCS);
  if (!EFI_ERROR (Status)) {
    //
    // Set the link speed and duplex
    //
    Control = BMCR_AUTONEGOTIATION_ENABLE
            | BMCR_RESTART_AUTONEGOTIATION;
    if (NicDevice->LinkSpeed100Mbps) {
      Control |= BMCR_100MBPS;
    }
    if (NicDevice->FullDuplex) {
      Control |= BMCR_FULL_DUPLEX;
    }
    Status = Ax88772PhyWrite (NicDevice, PHY_BMCR, Control);

    if (!EFI_ERROR(Status))
        gBS->Stall(3000000);
  }
  return Status;
}



/**
  Complete the negotiation of the PHY link

  This routine calls ::Ax88772PhyRead to determine if the
  link negotiation is complete.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in, out] PollCount  Address of number of times this routine was polled
  @param [out] Complete       Address of boolean to receive complate status.
  @param [out] LinkUp         Address of boolean to receive link status, TRUE=up.
  @param [out] HiSpeed        Address of boolean to receive link speed, TRUE=100Mbps.
  @param [out] FullDuplex     Address of boolean to receive link duplex, TRUE=full.

  @retval EFI_SUCCESS          The MAC address is available.
  @retval other                The MAC address is not valid.

**/
EFI_STATUS
Ax88772NegotiateLinkComplete (
  IN     NIC_DEVICE *NicDevice,
  IN OUT UINTN      *PollCount,
  OUT    BOOLEAN    *Complete,
  OUT    BOOLEAN    *LinkUp,
  OUT    BOOLEAN    *HiSpeed,
  OUT    BOOLEAN    *FullDuplex
  )
{
  UINT16      Mask;
  UINT16      PhyData;
  EFI_STATUS  Status;

  //
  //  Determine if the link is up.
  //
  *Complete = FALSE;

  //
  //  Get the link status
  //
  Status = Ax88772PhyRead (NicDevice,
                            PHY_BMSR,
                            &PhyData);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  *LinkUp = ((PhyData & BMSR_LINKST) != 0);
  if (0 == *LinkUp) {
  } else {
    *Complete = ((PhyData & 0x20) != 0);
    if (0 == *Complete) {
    } else {
      Status = Ax88772PhyRead (NicDevice,
                                PHY_ANLPAR,
                                &PhyData);

      if (!EFI_ERROR (Status)) {
        //
        //  Autonegotiation is complete
        //  Determine the link speed.
        //
        *HiSpeed = ((PhyData & (AN_TX_FDX | AN_TX_HDX))!= 0);
         //
        //  Determine the link duplex.
        //
        Mask = (*HiSpeed) ? AN_TX_FDX : AN_10_FDX;
        *FullDuplex = (BOOLEAN)((PhyData & Mask) != 0);
      }
    }
  }

  return Status;
}


/**
  Read a register from the PHY

  This routine calls ::Ax88772UsbCommand to read a PHY register.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in] RegisterAddress  Number of the register to read.
  @param [in, out] PhyData    Address of a buffer to receive the PHY register value

  @retval EFI_SUCCESS          The PHY data is available.
  @retval other                The PHY data is not valid.

**/
EFI_STATUS
Ax88772PhyRead (
  IN     NIC_DEVICE *NicDevice,
  IN     UINT8      RegisterAddress,
  IN OUT UINT16     *PhyData
  )
{
  USB_DEVICE_REQUEST SetupMsg;
  EFI_STATUS         Status;

  //
  //  Request access to the PHY
  //
  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_PHY_ACCESS_SOFTWARE;
  SetupMsg.Value = 0;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //  Read the PHY register address.
  //
  SetupMsg.RequestType = USB_ENDPOINT_DIR_IN
                       | USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_PHY_REG_READ;
  SetupMsg.Value = NicDevice->PhyId;
  SetupMsg.Index = RegisterAddress;
  SetupMsg.Length = sizeof(*PhyData);
  Status = Ax88772UsbCommand(NicDevice,
                             &SetupMsg,
                             PhyData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //  Release the PHY to the hardware
  //
    SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                         | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_PHY_ACCESS_HARDWARE;
  SetupMsg.Value = 0;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand(NicDevice,
                             &SetupMsg,
                             NULL);

  //
  //  Return the operation status.
  //
  return Status;
}


/**
  Write to a PHY register

  This routine calls ::Ax88772UsbCommand to write a PHY register.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in] RegisterAddress  Number of the register to read.
  @param [in] PhyData          Address of a buffer to receive the PHY register value

  @retval EFI_SUCCESS          The PHY data was written.
  @retval other                Failed to wwrite the PHY register.

**/
EFI_STATUS
Ax88772PhyWrite (
  IN NIC_DEVICE *NicDevice,
  IN UINT8      RegisterAddress,
  IN UINT16     PhyData
  )
{
  USB_DEVICE_REQUEST SetupMsg;
  EFI_STATUS         Status;

  //
  //  Request access to the PHY
  //
  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_PHY_ACCESS_SOFTWARE;
  SetupMsg.Value = 0;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //  Write the PHY register
  //
  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_PHY_REG_WRITE;
  SetupMsg.Value = NicDevice->PhyId;
  SetupMsg.Index = RegisterAddress;
  SetupMsg.Length = sizeof (PhyData);
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               &PhyData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  //  Release the PHY to the hardware
  //
  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_PHY_ACCESS_HARDWARE;
  SetupMsg.Value = 0;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);

  //
  //  Return the operation status.
  //
  return Status;
}


/**
  Reset the AX88772

  This routine uses ::Ax88772UsbCommand to reset the network
  adapter.  This routine also uses ::Ax88772PhyWrite to reset
  the PHY.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure

  @retval EFI_SUCCESS          The MAC address is available.
  @retval other                The MAC address is not valid.

**/
EFI_STATUS
Ax88772Reset (
  IN NIC_DEVICE *NicDevice
  )
{
  USB_DEVICE_REQUEST  SetupMsg;
  EFI_STATUS          Status;

  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                           | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_PHY_ACCESS_HARDWARE;
  SetupMsg.Value = 0;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);

  if (EFI_ERROR(Status)) goto err;

  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                          | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_PHY_SELECT;
  SetupMsg.Value = SPHY_PSEL;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);

  if (EFI_ERROR(Status)) goto err;

  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_RESET;
  SetupMsg.Value = SRR_IPRL ;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);

  if (EFI_ERROR(Status)) goto err;

  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_RESET;
  SetupMsg.Value = SRR_IPPD | SRR_IPRL ;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);

  gBS->Stall (200000);

  if (EFI_ERROR(Status)) goto err;

  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_RESET;
  SetupMsg.Value =  SRR_IPRL  ;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);

  gBS->Stall (200000);

  if (EFI_ERROR(Status)) goto err;

  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_RESET;
  SetupMsg.Value = 0;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);

  if (EFI_ERROR(Status)) goto err;

  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_PHY_SELECT;
  SetupMsg.Value = SPHY_PSEL;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);

  if (EFI_ERROR(Status)) goto err;

  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_RESET;
  SetupMsg.Value =  SRR_IPRL | SRR_BZ | SRR_BZTYPE;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);

  if (EFI_ERROR(Status)) goto err;

  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_RX_CONTROL_WRITE;
  SetupMsg.Value = 0;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);

  if (EFI_ERROR(Status)) goto err;

  if (!NicDevice->Flag772A) {
    SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                         | USB_TARGET_DEVICE;
    SetupMsg.Request = CMD_RXQTC;
#if RXTHOU
    /*size cannot exceed 3K*/
    //SetupMsg.Value = 0x0;
    //SetupMsg.Index = 0x8001;
    /*size cannot exceed 16K*/
    SetupMsg.Value = 0x8300;
    SetupMsg.Index = 0x8500;
    /*size cannot exceed 32K*/
    //SetupMsg.Value = 0x8784;
    //SetupMsg.Index = 0x8A00;
    SetupMsg.Length = 0;
#else
    SetupMsg.Value = 0x8000;
    SetupMsg.Index = 0x8001;
#endif
    Status = Ax88772UsbCommand (NicDevice,
                                 &SetupMsg,
                                 NULL);
  }
err:
  return Status;
}

/**
  Enable or disable the receiver

  This routine calls ::Ax88772UsbCommand to update the
  receiver state.  This routine also calls ::Ax88772MacAddressSet
  to establish the MAC address for the network adapter.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in] RxFilter         Simple network RX filter mask value

  @retval EFI_SUCCESS          The MAC address was set.
  @retval other                The MAC address was not set.

**/
EFI_STATUS
Ax88772RxControl (
  IN NIC_DEVICE *NicDevice,
  IN UINT32     RxFilter
  )
{
  UINT16             MediumStatus;
  UINT16             RxControl;
  USB_DEVICE_REQUEST SetupMsg;
  EFI_STATUS         Status = EFI_SUCCESS;

  //
  // Enable the receiver if something is to be received
  //
  if (RxFilter != 0) {
    //
    //  Enable the receiver
    //
    SetupMsg.RequestType = USB_ENDPOINT_DIR_IN
                         | USB_REQ_TYPE_VENDOR
                         | USB_TARGET_DEVICE;
    SetupMsg.Request = CMD_MEDIUM_STATUS_READ;
    SetupMsg.Value = 0;
    SetupMsg.Index = 0;
    SetupMsg.Length = sizeof (MediumStatus);
    Status = Ax88772UsbCommand (NicDevice,
                                 &SetupMsg,
                                 &MediumStatus);
    if (!EFI_ERROR (Status)) {
      if (0 == (MediumStatus & MS_RE)) {
        MediumStatus |= MS_RE | MS_ONE;

        if (NicDevice->FullDuplex)
          MediumStatus |= MS_TFC | MS_RFC | MS_FD;
        else
          MediumStatus &= ~(MS_TFC | MS_RFC | MS_FD);

        if (NicDevice->LinkSpeed100Mbps)
          MediumStatus |= MS_PS;
        else
          MediumStatus &= ~MS_PS;

        SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                             | USB_TARGET_DEVICE;
        SetupMsg.Request = CMD_MEDIUM_STATUS_WRITE;
        SetupMsg.Value = MediumStatus;
        SetupMsg.Index = 0;
        SetupMsg.Length = 0;
        Status = Ax88772UsbCommand (NicDevice,
                                     &SetupMsg,
                                     NULL);

        if (EFI_ERROR(Status))
              goto EXIT;
      }
    } else {
        goto EXIT;
    }
  }
  RxControl = RXC_SO;
  if (!NicDevice->Flag772A)
    RxControl |= RXC_RH1M;

  //
  //  Enable multicast if requested
  //
  if ((RxFilter & EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST) != 0) {
      RxControl |= RXC_AM;
      //
      //  Update the multicast hash table
      //
      SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                           | USB_TARGET_DEVICE;
      SetupMsg.Request = CMD_MULTICAST_HASH_WRITE;
      SetupMsg.Value = 0;
      SetupMsg.Index = 0;
      SetupMsg.Length = sizeof (NicDevice ->MulticastHash);
      Status = Ax88772UsbCommand (NicDevice,
                                   &SetupMsg,
                                   &NicDevice->MulticastHash);

      if (EFI_ERROR(Status))
              goto EXIT;
  }

  //
  //  Enable all multicast if requested
  //
  if ((RxFilter & EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS_MULTICAST) != 0) {
      RxControl |= RXC_AMALL;
  }

  //
  //  Enable broadcast if requested
  //
  if ((RxFilter & EFI_SIMPLE_NETWORK_RECEIVE_BROADCAST) != 0) {
      RxControl |= RXC_AB;
  }

  //
  //  Enable promiscuous mode if requested
  //
  if ((RxFilter & EFI_SIMPLE_NETWORK_RECEIVE_PROMISCUOUS) != 0) {
      RxControl |= RXC_PRO;
  }

  //
  //  Update the receiver control
  //
  if (NicDevice->CurRxControl != RxControl) {
    SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                         | USB_TARGET_DEVICE;
    SetupMsg.Request = CMD_RX_CONTROL_WRITE;
#if RXTHOU
    if (NicDevice->Flag772A)
      RxControl |= 0x0300;
#endif
    if (NicDevice->Flag772A)
      RxControl &= ~(RXC_MFB);
    SetupMsg.Value = RxControl;
    SetupMsg.Index = 0;
    SetupMsg.Length = 0;
    Status = Ax88772UsbCommand (NicDevice,
                                 &SetupMsg,
                                 NULL);
    if (!EFI_ERROR (Status))
      NicDevice->CurRxControl = RxControl;
  }

  //
  // Return the operation status
  //
EXIT:
  return Status;
}



EFI_STATUS
Ax88772ReloadSrom  (
  IN NIC_DEVICE *NicDevice
  )
{
  USB_DEVICE_REQUEST SetupMsg;
  EFI_STATUS Status;

  //
  //  Read a value from the SROM
  //
  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;

  SetupMsg.Request = CMD_WRITE_GPIOS;
  SetupMsg.Value = 0x80;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0 ;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);

  if (EFI_SUCCESS == Status)
          gBS->Stall(500000);

  return Status;

}


/**
  Read an SROM location

  This routine calls ::Ax88772UsbCommand to read data from the
  SROM.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in] Address          SROM address
  @param [out] Data           Buffer to receive the data

  @retval EFI_SUCCESS          The read was successful
  @retval other                The read failed

**/
EFI_STATUS
Ax88772SromRead (
  IN  NIC_DEVICE *NicDevice,
  IN  UINT32     Address,
  OUT UINT16     *Data
  )
{

  USB_DEVICE_REQUEST SetupMsg;
  EFI_STATUS Status;

  //
  //  Read a value from the SROM
  //
  SetupMsg.RequestType = USB_ENDPOINT_DIR_IN
                       | USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;
  SetupMsg.Request = CMD_SROM_READ;
  SetupMsg.Value = (UINT16) Address;
  SetupMsg.Index = 0;
  SetupMsg.Length = sizeof (*Data);
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               Data);

  //
  // Return the operation status
  //
  return Status;
}

EFI_STATUS
Ax88772EnableSromWrite  (
  IN NIC_DEVICE * NicDevice
  )
{
  USB_DEVICE_REQUEST SetupMsg;
  EFI_STATUS Status;

  //
  //  Read a value from the SROM
  //
  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;

  SetupMsg.Request = CMD_SROM_WRITE_EN;
  SetupMsg.Value = 0;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0 ;
  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);

  if (EFI_SUCCESS == Status)
          gBS->Stall(500000);

  return Status;

}


EFI_STATUS
Ax88772DisableSromWrite  (
  IN NIC_DEVICE *NicDevice
  )
{
  USB_DEVICE_REQUEST SetupMsg;
  EFI_STATUS Status;

  //
  //  Read a value from the SROM
  //
  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;

  SetupMsg.Request = CMD_SROM_WRITE_DIS;
  SetupMsg.Value = 0;
  SetupMsg.Index = 0;
  SetupMsg.Length = 0;
  Status = Ax88772UsbCommand (NicDevice,
                      &SetupMsg,
                      NULL);

  if (EFI_SUCCESS == Status)
          gBS->Stall(500000);

  return Status;

}

/**
  Write an SROM location

  This routine calls ::Ax88772UsbCommand to write data from the
  SROM.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in] Address          SROM address
  @param [out] Data           Buffer of data to write

  @retval EFI_SUCCESS          The write was successful
  @retval other                The write failed

**/
EFI_STATUS
Ax88772SromWrite (
  IN NIC_DEVICE *NicDevice,
  IN UINT32     Address,
  IN UINT16     *Data
  )
{

  USB_DEVICE_REQUEST SetupMsg;
  EFI_STATUS Status;

  SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                       | USB_TARGET_DEVICE;

  SetupMsg.Request = CMD_SROM_WRITE;
  SetupMsg.Value = (UINT16) Address;
  SetupMsg.Index = (UINT16) (*Data);
  SetupMsg.Length = 0;

  Status = Ax88772UsbCommand (NicDevice,
                               &SetupMsg,
                               NULL);

  //
  // Return the operation status
  //
  return Status;
}

/**
  Send a command to the USB device.

  @param [in] NicDevice       Pointer to the NIC_DEVICE structure
  @param [in] Request         Pointer to the request structure
  @param [in, out] Buffer     Data buffer address

  @retval EFI_SUCCESS          The USB transfer was successful
  @retval other                The USB transfer failed

**/
EFI_STATUS
Ax88772UsbCommand (
  IN     NIC_DEVICE         *NicDevice,
  IN     USB_DEVICE_REQUEST *Request,
  IN OUT VOID               *Buffer
  )
{
  UINT32 CmdStatus;
  EFI_USB_DATA_DIRECTION Direction;
  EFI_USB_IO_PROTOCOL    *UsbIo;
  EFI_STATUS             Status;

  //
  // Determine the transfer direction
  //
  Direction = EfiUsbNoData;
  if (Request->Length != 0) {
    Direction = ((Request->RequestType & USB_ENDPOINT_DIR_IN) != 0)
              ? EfiUsbDataIn : EfiUsbDataOut;
  }

  //
  // Issue the command
  //
  UsbIo = NicDevice->UsbIo;
  Status = UsbIo->UsbControlTransfer (UsbIo,
                                        Request,
                                        Direction,
                                        USB_BUS_TIMEOUT,
                                        Buffer,
                                        Request->Length,
                                        &CmdStatus);

  //
  // Determine the operation status
  //
  if (!EFI_ERROR (Status)) {
    Status = CmdStatus;
  } else {
    //
    // Only use status values associated with the Simple Network protocol
    //
    if (EFI_TIMEOUT == Status) {
      Status = EFI_DEVICE_ERROR;
    }
  }

  //
  // Return the operation status
  //
  return Status;
}

BOOLEAN
Ax88772GetLinkStatus (
  IN NIC_DEVICE *NicDevice
)
{
  UINT32              CmdStatus;
  EFI_USB_IO_PROTOCOL *UsbIo;
  UINT64              IntData = 0;
  UINTN               IntDataLeng = 8;
  EFI_STATUS          Status;

  //
  // Issue the command
  //
  UsbIo = NicDevice->UsbIo;
  Status = UsbIo->UsbSyncInterruptTransfer(UsbIo,
                                        USB_ENDPOINT_DIR_IN | INTERRUPT_ENDPOINT,
                                        &IntData,
                                        &IntDataLeng,
                                        USB_BUS_TIMEOUT,
                                        &CmdStatus);

  if (EFI_ERROR(Status) || EFI_ERROR(CmdStatus) || 0 == IntDataLeng) {
      return FALSE;
  }
  return (IntData & 0x800000)? FALSE : TRUE;

}

#if RXTHOU
EFI_STATUS
Ax88772BulkIn(
  IN NIC_DEVICE * NicDevice
)
{
  UINTN               Index;
  UINTN               LengthInBytes = 0;
  UINTN               TmpLen = AX88772_MAX_BULKIN_SIZE;
  UINTN               OrigTmpLen = 0;
  UINT16              TmpLen2;
  UINT16              TmpLenBar;
  UINT16              TmpTotalLen = 0;
  UINTN               TotalLen = LengthInBytes;
  EFI_STATUS          Status = EFI_DEVICE_ERROR;
  EFI_USB_IO_PROTOCOL *UsbIo;
  UINT32              TransferStatus = 0;
  UINT16              TmpPktCnt = 0;
  UINT16              *TmpHdr = (UINT16 *)NicDevice->BulkInbuf;
  USB_DEVICE_REQUEST  SetupMsg;

  UsbIo = NicDevice->UsbIo;
  for (Index = 0 ; Index < (AX88772_MAX_BULKIN_SIZE / 512) && UsbIo != NULL; Index++) {
    VOID* TmpAddr = 0;

    TmpPktCnt = 0;
    TmpAddr = (VOID*) &NicDevice->BulkInbuf[LengthInBytes];
    OrigTmpLen = TmpLen;
    Status = UsbIo->UsbBulkTransfer (UsbIo,
                          USB_ENDPOINT_DIR_IN | BULK_IN_ENDPOINT,
                          TmpAddr,
                          &TmpLen,
                          BULKIN_TIMEOUT,
                          &TransferStatus);

    if (OrigTmpLen == TmpLen) {
      Status = EFI_NOT_READY;
      goto no_pkt;
    }

    if ((!EFI_ERROR (Status)) &&
        (!EFI_ERROR (TransferStatus)) &&
        TmpLen != 0) {
      LengthInBytes += TmpLen;
      if ((TmpLen % 512) != 0) {
        goto done;
      }
    } else if ((!EFI_ERROR (Status)) &&
               (!EFI_ERROR (TransferStatus)) &&
               (TmpLen == 0)) {
      Status = EFI_NOT_READY;
      goto done;
    } else if (EFI_TIMEOUT == Status && EFI_USB_ERR_TIMEOUT == TransferStatus) {
      SetupMsg.RequestType = USB_REQ_TYPE_STANDARD | 0x02;
      SetupMsg.Request = 0x01;
      SetupMsg.Value =  0;
      SetupMsg.Index = 0x82;
      SetupMsg.Length = 0;
      Status = Ax88772UsbCommand (NicDevice,
                                   &SetupMsg,
                                   NULL);
      Status = EFI_NOT_READY;
      goto done;
    } else {
      Status = EFI_DEVICE_ERROR;
      goto done;
    }
  }
done:
  if (LengthInBytes != 0) {

    do {
      TmpLen2 = (*TmpHdr) & 0x7FF;
      TmpLenBar = *(TmpHdr + 1);
      TmpTotalLen = ((TmpLen + 4 + 1) & 0xfffe);

      if ((TmpLen2 & 0x7FF) + (TmpLenBar & 0x7FF) == 0x7FF) {
        TmpPktCnt++;
      } else {
        if (TmpPktCnt != 0) {
          break;
        }
        Status = EFI_NOT_READY;
        goto no_pkt;
      }
      TmpHdr += (TmpTotalLen / 2);
      TotalLen -= TmpTotalLen;
    } while (TotalLen > 0);

  if (LengthInBytes >= 1000 && TmpPktCnt != 0) {
    if ((NicDevice->RxBurst) == 1) {
      SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                           | USB_TARGET_DEVICE;
      SetupMsg.Request = CMD_RESET;
      SetupMsg.Value =  SRR_IPRL;
      SetupMsg.Index = 0;
      SetupMsg.Length = 0;
      Ax88772UsbCommand (NicDevice,
                          &SetupMsg,
                          NULL);
    }

    if (NicDevice->RxBurst < 2)
    NicDevice->RxBurst++;

  } else {
    if (NicDevice->RxBurst >= 2) {
      SetupMsg.RequestType = USB_REQ_TYPE_VENDOR
                           | USB_TARGET_DEVICE;
      SetupMsg.Request = CMD_RESET;
      SetupMsg.Value =  SRR_IPRL| SRR_BZ | SRR_BZTYPE;
      SetupMsg.Index = 0;
      SetupMsg.Length = 0;
      Ax88772UsbCommand (NicDevice,
                          &SetupMsg,
                          NULL);
      }
      NicDevice->RxBurst = 0;
    }
  }

  if (TmpPktCnt != 0) {
    NicDevice->PktCnt = TmpPktCnt;
    NicDevice->CurPktHdrOff = NicDevice->BulkInbuf;
    NicDevice->CurPktOff = NicDevice->BulkInbuf + 4;
    Status = EFI_SUCCESS;
  }

no_pkt:
  return Status;
}
#else
EFI_STATUS
Ax88772BulkIn(
  IN NIC_DEVICE *NicDevice
)
{
  UINTN               Index;
  UINTN               LengthInBytes = 0;
  UINTN               TmpLen = AX88772_MAX_BULKIN_SIZE;
  UINTN               OrigTmpLen = 0;
  UINT16              TmpLen2;
  UINT16              TmpLenBar;
  UINT16              TmpTotalLen = 0;
  UINTN               CURBufSize = AX88772_MAX_BULKIN_SIZE;
  EFI_STATUS          Status = EFI_DEVICE_ERROR;
  EFI_USB_IO_PROTOCOL *UsbIo;
  UINT32              TransferStatus = 0;
  UINT16              TmpPktCnt = 0;
  UINT16              *TmpHdr = (UINT16 *)NicDevice->BulkInbuf;

  UsbIo = NicDevice->UsbIo;
  for (Index = 0 ; Index < (AX88772_MAX_BULKIN_SIZE / 512) && UsbIo != NULL; Index++) {
    VOID *TmpAddr = 0;

    TmpPktCnt = 0;
    TmpAddr = (VOID*) &NicDevice->BulkInbuf[LengthInBytes];
    OrigTmpLen = TmpLen;
    Status = UsbIo->UsbBulkTransfer (UsbIo,
                          USB_ENDPOINT_DIR_IN | BULK_IN_ENDPOINT,
                          TmpAddr,
                          &TmpLen,
                          BULKIN_TIMEOUT,
                          &TransferStatus);

    if (OrigTmpLen == TmpLen) {
      Status = EFI_NOT_READY;
      goto no_pkt;
    }

    if ((!EFI_ERROR (Status)) &&
        (!EFI_ERROR (TransferStatus)) &&
        TmpLen != 0) {

      LengthInBytes += TmpLen;
      CURBufSize = CURBufSize - TmpLen;
      TmpLen = CURBufSize;
      do {
        TmpLen2 = *TmpHdr;
        TmpLenBar = *(TmpHdr + 1);
        TmpTotalLen += ((TmpLen2 + 4 + 1) & 0xfffe);

        if (((TmpLen2 ^ TmpLenBar) == 0xffff))  {
          if (TmpTotalLen == LengthInBytes) {
            TmpPktCnt++;
            Status = EFI_SUCCESS;
            goto done;
          } else if (TmpTotalLen > LengthInBytes) {
            break;
          }
        } else if (((TmpLen2 ^ TmpLenBar) != 0xffff)) {
          if (TmpPktCnt != 0) {
            Status = EFI_SUCCESS;
            goto done;
          }
          Status = EFI_NOT_READY;
          goto no_pkt;
        }
        TmpHdr += (TmpTotalLen / 2);
        TmpPktCnt++;
      } while (TmpTotalLen < LengthInBytes);
    } else if ((!EFI_ERROR (Status)) &&
               (!EFI_ERROR (TransferStatus)) &&
               (TmpLen == 0)) {
      if (TmpPktCnt != 0) {
        Status = EFI_SUCCESS;
        goto done;
      }
      Status = EFI_NOT_READY;
      goto no_pkt;
    } else if (EFI_TIMEOUT == Status && EFI_USB_ERR_TIMEOUT == TransferStatus) {
      if (TmpPktCnt != 0) {
        Status = EFI_SUCCESS;
        goto done;
      }
      Status = EFI_NOT_READY;
      goto no_pkt;
    } else {
      if (TmpPktCnt != 0) {
        Status = EFI_SUCCESS;
        goto done;
      }
      Status = EFI_DEVICE_ERROR;
      goto no_pkt;
    }
  }
done:
  NicDevice->PktCnt = TmpPktCnt;
  NicDevice->CurPktHdrOff = NicDevice->BulkInbuf;
  NicDevice->CurPktOff = NicDevice->BulkInbuf + 4;
no_pkt:
  return Status;
}
#endif
