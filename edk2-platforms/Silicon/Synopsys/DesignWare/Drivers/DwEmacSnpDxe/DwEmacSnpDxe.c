/** @file
  DW EMAC SNP DXE driver

  SPDX-License-Identifier: BSD-2-Clause-Patent

  The original software modules are licensed as follows:

  Copyright (c) 2012 - 2020, Arm Limited. All rights reserved.<BR>
  Copyright (c) 2004 - 2010, Intel Corporation. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "DwEmacSnpDxe.h"
#include "EmacDxeUtil.h"
#include "PhyDxeUtil.h"

#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/NetLib.h>
#include <Library/DmaLib.h>

/**
  Change the state of a network interface from "stopped" to "started."

  This function starts a network interface. If the network interface successfully
  starts, then EFI_SUCCESS will be returned.

  @param  Snp                    A pointer to the EFI_SIMPLE_NETWORK_PROTOCOL instance.

  @retval EFI_SUCCESS            The network interface was started.
  @retval EFI_ALREADY_STARTED    The network interface is already in the started state.
  @retval EFI_INVALID_PARAMETER  This parameter was NULL or did not point to a valid
                                 EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR       The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED        This function is not supported by the network interface.

**/
EFI_STATUS
EFIAPI
SnpStart (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL   *This
 )
{
  SIMPLE_NETWORK_DRIVER    *Snp;

  DEBUG ((DEBUG_INFO,"SNP:DXE: %a ()\r\n", __FUNCTION__));

  // Check Snp instance
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Snp = INSTANCE_FROM_SNP_THIS (This);
  // Check state
  if ((Snp->SnpMode.State == EfiSimpleNetworkStarted)    ||
      (Snp->SnpMode.State == EfiSimpleNetworkInitialized)  ) {
    return EFI_ALREADY_STARTED;
  }

  // Change state
  Snp->SnpMode.State = EfiSimpleNetworkStarted;
  return EFI_SUCCESS;
}


/**
  Changes the state of a network interface from "started" to "stopped."

  This function stops a network interface. This call is only valid if the network
  interface is in the started state. If the network interface was successfully
  stopped, then EFI_SUCCESS will be returned.

  @param  Snp                     A pointer to the EFI_SIMPLE_NETWORK_PROTOCOL
                                  instance.


  @retval EFI_SUCCESS             The network interface was stopped.
  @retval EFI_NOT_STARTED         The network interface has not been started.
  @retval EFI_INVALID_PARAMETER   This parameter was NULL or did not point to a
                                  valid EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR        The command could not be sent to the network
                                  interface.
  @retval EFI_UNSUPPORTED         This function is not supported by the network
                                  interface.

**/
EFI_STATUS
EFIAPI
SnpStop (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL*   This
  )
{
  SIMPLE_NETWORK_DRIVER    *Snp;

  DEBUG ((DEBUG_INFO, "SNP:DXE: %a ()\r\n", __FUNCTION__));

  // Check Snp Instance
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Snp = INSTANCE_FROM_SNP_THIS (This);
  // Check state of the driver
  if (Snp->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  // Stop the Tx and Rx
  EmacStopTxRx (Snp->MacBase);
  // Change the state
  switch (Snp->SnpMode.State) {
    case EfiSimpleNetworkStarted:
    case EfiSimpleNetworkInitialized:
      Snp->SnpMode.State = EfiSimpleNetworkStopped;
      break;
    default:
      return EFI_DEVICE_ERROR;
  }

  // Put the device into a power saving mode
  return EFI_SUCCESS;
}


/**
  Resets a network adapter and allocates the transmit and receive buffers
  required by the network interface; optionally, also requests allocation of
  additional transmit and receive buffers.

  This function allocates the transmit and receive buffers required by the network
  interface. If this allocation fails, then EFI_OUT_OF_RESOURCES is returned.
  If the allocation succeeds and the network interface is successfully initialized,
  then EFI_SUCCESS will be returned.

  @param Snp                A pointer to the EFI_SIMPLE_NETWORK_PROTOCOL instance.

  @param ExtraRxBufferSize  The size, in bytes, of the extra receive buffer space
                            that the driver should allocate for the network interface.
                            Some network interfaces will not be able to use the
                            extra buffer, and the caller will not know if it is
                            actually being used.
  @param ExtraTxBufferSize  The size, in bytes, of the extra transmit buffer space
                            that the driver should allocate for the network interface.
                            Some network interfaces will not be able to use the
                            extra buffer, and the caller will not know if it is
                            actually being used.

  @retval EFI_SUCCESS           The network interface was initialized.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_OUT_OF_RESOURCES  There was not enough memory for the transmit and
                                receive buffers.
  @retval EFI_INVALID_PARAMETER This parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       The increased buffer size feature is not supported.

**/
EFI_STATUS
EFIAPI
SnpInitialize (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL   *This,
  IN  UINTN                         ExtraRxBufferSize OPTIONAL,
  IN  UINTN                         ExtraTxBufferSize OPTIONAL
  )
{
  EFI_STATUS                  Status;
  SIMPLE_NETWORK_DRIVER       *Snp;

  DEBUG ((DEBUG_INFO, "SNP:DXE: %a ()\r\n", __FUNCTION__));

  // Check Snp Instance
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Snp = INSTANCE_FROM_SNP_THIS (This);
  // First check that driver has not already been initialized
  if (Snp->SnpMode.State == EfiSimpleNetworkInitialized) {
    return EFI_SUCCESS;
  } else if (Snp->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  // Init PHY
  Status = PhyDxeInitialization (&Snp->PhyDriver, Snp->MacBase);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  // Init EMAC
  Status = EmacDxeInitialization (&Snp->MacDriver, Snp->MacBase);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  // Set MAC Address
  EmacSetMacAddress (&Snp->SnpMode.CurrentAddress, Snp->MacBase);
  EmacReadMacAddress (&Snp->SnpMode.CurrentAddress, Snp->MacBase);

  // Init Link
  DEBUG ((DEBUG_INFO, "SNP:DXE: Auto-Negotiating Ethernet PHY Link ...\n"));
  Status = PhyLinkAdjustEmacConfig (&Snp->PhyDriver, Snp->MacBase);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "SNP:DXE: Link is Down - Network Cable is not plugged in?\n"));
    return EFI_DEVICE_ERROR;
  }

  EmacStartTransmission (Snp->MacBase);

  // Declare the driver as initialized
  Snp->SnpMode.State = EfiSimpleNetworkInitialized;

  return EFI_SUCCESS;
}


/**
  Resets a network adapter and reinitializes it with the parameters that were
  provided in the previous call to Initialize().

  This function resets a network adapter and reinitializes it with the parameters
  that were provided in the previous call to Initialize(). The transmit and
  receive queues are emptied and all pending interrupts are cleared.
  Receive filters, the station address, the statistics, and the multicast-IP-to-HW
  MAC addresses are not reset by this call. If the network interface was
  successfully reset, then EFI_SUCCESS will be returned. If the driver has not
  been initialized, EFI_DEVICE_ERROR will be returned.

  @param Snp                  A pointer to the EFI_SIMPLE_NETWORK_PROTOCOL instance.
  @param ExtendedVerification Indicates that the driver may perform a more
                              exhaustive verification operation of the device
                              during reset.

  @retval EFI_SUCCESS           The network interface was reset.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.

**/
EFI_STATUS
EFIAPI
SnpReset (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL   *This,
  IN  BOOLEAN                       ExtendedVerification
  )
{
  EFI_STATUS                  Status;
  SIMPLE_NETWORK_DRIVER       *Snp;

  Snp = INSTANCE_FROM_SNP_THIS (This);

  DEBUG ((DEBUG_INFO, "SNP:DXE: %a ()\r\n", __FUNCTION__));

  // Check Snp Instance
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // First check that driver has not already been initialized
  if (Snp->SnpMode.State == EfiSimpleNetworkStarted) {
    return EFI_DEVICE_ERROR;
  } else if (Snp->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  // Initiate a PHY reset
  Status = PhySoftReset (&Snp->PhyDriver, Snp->MacBase);
  if (EFI_ERROR (Status)) {
    Snp->SnpMode.State = EfiSimpleNetworkStopped;
    return EFI_NOT_STARTED;
  }

  return EFI_SUCCESS;
}


/**
  Resets a network adapter and leaves it in a state that is safe for another
  driver to initialize.

  This function releases the memory buffers assigned in the Initialize() call.
  Pending transmits and receives are lost, and interrupts are cleared and disabled.
  After this call, only the Initialize() and Stop() calls may be used. If the
  network interface was successfully shutdown, then EFI_SUCCESS will be returned.
  If the driver has not been initialized, EFI_DEVICE_ERROR will be returned.

  @param  This  A pointer to the EFI_SIMPLE_NETWORK_PROTOCOL instance.

  @retval EFI_SUCCESS           The network interface was shutdown.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_INVALID_PARAMETER This parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.

**/
EFI_STATUS
EFIAPI
SnpShutdown (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL*   This
  )
{
  SIMPLE_NETWORK_DRIVER     *Snp;

  DEBUG ((DEBUG_INFO, "SNP:DXE: %a ()\r\n", __FUNCTION__));

  // Check Snp Instance
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Snp = INSTANCE_FROM_SNP_THIS (This);
  // First check that driver has not already been initialized
  if (Snp->SnpMode.State == EfiSimpleNetworkStarted) {
    return EFI_DEVICE_ERROR;
  } else if (Snp->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  EmacStopTxRx (Snp->MacBase);

  Snp->SnpMode.State = EfiSimpleNetworkStopped;

  return EFI_SUCCESS;
}


/**
  Manages the multicast receive filters of a network interface.

  This function is used enable and disable the hardware and software receive
  filters for the underlying network device.
  The receive filter change is broken down into three steps:
  * The filter mask bits that are set (ON) in the Enable parameter are added to
    the current receive filter settings.
  * The filter mask bits that are set (ON) in the Disable parameter are subtracted
    from the updated receive filter settings.
  * If the resulting receive filter setting is not supported by the hardware a
    more liberal setting is selected.
  If the same bits are set in the Enable and Disable parameters, then the bits
  in the Disable parameter takes precedence.
  If the ResetMCastFilter parameter is TRUE, then the multicast address list
  filter is disabled (irregardless of what other multicast bits are set in the
  Enable and Disable parameters). The SNP->Mode->MCastFilterCount field is set
  to zero. The Snp->Mode->MCastFilter contents are undefined.
  After enabling or disabling receive filter settings, software should verify
  the new settings by checking the Snp->Mode->ReceiveFilterSettings,
  Snp->Mode->MCastFilterCount and Snp->Mode->MCastFilter fields.
  Note: Some network drivers and/or devices will automatically promote receive
    filter settings if the requested setting can not be honored. For example, if
    a request for four multicast addresses is made and the underlying hardware
    only supports two multicast addresses the driver might set the promiscuous
    or promiscuous multicast receive filters instead. The receiving software is
    responsible for discarding any extra packets that get through the hardware
    receive filters.
    Note: Note: To disable all receive filter hardware, the network driver must
      be Shutdown() and Stopped(). Calling ReceiveFilters() with Disable set to
      Snp->Mode->ReceiveFilterSettings will make it so no more packets are
      returned by the Receive() function, but the receive hardware may still be
      moving packets into system memory before inspecting and discarding them.
      Unexpected system errors, reboots and hangs can occur if an OS is loaded
      and the network devices are not Shutdown() and Stopped().
  If ResetMCastFilter is TRUE, then the multicast receive filter list on the
  network interface will be reset to the default multicast receive filter list.
  If ResetMCastFilter is FALSE, and this network interface allows the multicast
  receive filter list to be modified, then the MCastFilterCnt and MCastFilter
  are used to update the current multicast receive filter list. The modified
  receive filter list settings can be found in the MCastFilter field of
  EFI_SIMPLE_NETWORK_MODE. If the network interface does not allow the multicast
  receive filter list to be modified, then EFI_INVALID_PARAMETER will be returned.
  If the driver has not been initialized, EFI_DEVICE_ERROR will be returned.
  If the receive filter mask and multicast receive filter list have been
  successfully updated on the network interface, EFI_SUCCESS will be returned.

  @param Snp              A pointer to the EFI_SIMPLE_NETWORK_PROTOCOL instance.
  @param Enable           A bit mask of receive filters to enable on the network
                          interface.
  @param Disable          A bit mask of receive filters to disable on the network
                          interface. For backward compatibility with EFI 1.1
                          platforms, the EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST bit
                          must be set when the ResetMCastFilter parameter is TRUE.
  @param ResetMCastFilter Set to TRUE to reset the contents of the multicast
                          receive filters on the network interface to their
                          default values.
  @param MCastFilterCnt   Number of multicast HW MAC addresses in the new MCastFilter
                          list. This value must be less than or equal to the
                          MCastFilterCnt field of EFI_SIMPLE_NETWORK_MODE.
                          This field is optional if ResetMCastFilter is TRUE.
  @param MCastFilter      A pointer to a list of new multicast receive filter HW
                          MAC addresses. This list will replace any existing
                          multicast HW MAC address list. This field is optional
                          if ResetMCastFilter is TRUE.

  @retval EFI_SUCCESS            The multicast receive filter list was updated.
  @retval EFI_NOT_STARTED        The network interface has not been started.
  @retval EFI_INVALID_PARAMETER  One or more of the following conditions is TRUE:
                                 * This is NULL
                                 * There are bits set in Enable that are not set
                                   in Snp->Mode->ReceiveFilterMask
                                 * There are bits set in Disable that are not set
                                   in Snp->Mode->ReceiveFilterMask
                                 * Multicast is being enabled (the
                                   EFI_SIMPLE_NETWORK_RECEIVE_MULTICAST bit is
                                   set in Enable, it is not set in Disable, and
                                   ResetMCastFilter is FALSE) and MCastFilterCount
                                   is zero
                                 * Multicast is being enabled and MCastFilterCount
                                   is greater than Snp->Mode->MaxMCastFilterCount
                                 * Multicast is being enabled and MCastFilter is NULL
                                 * Multicast is being enabled and one or more of
                                   the addresses in the MCastFilter list are not
                                   valid multicast MAC addresses
  @retval EFI_DEVICE_ERROR       One or more of the following conditions is TRUE:
                                 * The network interface has been started but has
                                   not been initialized
                                 * An unexpected error was returned by the
                                   underlying network driver or device
  @retval EFI_UNSUPPORTED        This function is not supported by the network
                                 interface.

**/
EFI_STATUS
EFIAPI
SnpReceiveFilters (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL   *This,
  IN  UINT32                        Enable,
  IN  UINT32                        Disable,
  IN  BOOLEAN                       ResetMCastFilter,
  IN  UINTN                         MCastFilterCnt  OPTIONAL,
  IN  EFI_MAC_ADDRESS               *MCastFilter    OPTIONAL
  )
{
  UINT32                  ReceiveFilterSetting;
  SIMPLE_NETWORK_DRIVER   *Snp;

  Snp = INSTANCE_FROM_SNP_THIS (This);

  // Check Snp Instance
  if (Snp == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Check that driver was started and initialised
  if (Snp->SnpMode.State == EfiSimpleNetworkStarted) {
    return EFI_DEVICE_ERROR;
  } else if (Snp->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  // Check that bits set in Enable/Disable are set in ReceiveFilterMask
   if ((Enable  & (~Snp->SnpMode.ReceiveFilterMask)) ||
      (Disable & (~Snp->SnpMode.ReceiveFilterMask))    ) {
    return EFI_INVALID_PARAMETER;
  }

  // Get the filter mask bits that are set in Enable parameter or Disable Parameter
  // Same bits that are set in Enable/Disable parameters, then bits in the Disable parameter takes precedance
  ReceiveFilterSetting = (Snp->SnpMode.ReceiveFilterSetting | Enable) & (~Disable);

  EmacRxFilters (ReceiveFilterSetting, ResetMCastFilter, MCastFilterCnt, MCastFilter, Snp->MacBase);

  return EFI_SUCCESS;
}


/**
  Modifies or resets the current station address, if supported.

  This function modifies or resets the current station address of a network
  interface, if supported. If Reset is TRUE, then the current station address is
  set to the network interface's permanent address. If Reset is FALSE, and the
  network interface allows its station address to be modified, then the current
  station address is changed to the address specified by New. If the network
  interface does not allow its station address to be modified, then
  EFI_INVALID_PARAMETER will be returned. If the station address is successfully
  updated on the network interface, EFI_SUCCESS will be returned. If the driver
  has not been initialized, EFI_DEVICE_ERROR will be returned.

  @param Snp      A pointer to the EFI_SIMPLE_NETWORK_PROTOCOL instance.
  @param Reset    Flag used to reset the station address to the network interface's
                  permanent address.
  @param NewMac   New station address to be used for the network interface.


  @retval EFI_SUCCESS           The network interface's station address was updated.
  @retval EFI_NOT_STARTED       The Simple Network Protocol interface has not been
                                started by calling Start().
  @retval EFI_INVALID_PARAMETER The New station address was not accepted by the NIC.
  @retval EFI_INVALID_PARAMETER Reset is FALSE and New is NULL.
  @retval EFI_DEVICE_ERROR      The Simple Network Protocol interface has not
                                been initialized by calling Initialize().
  @retval EFI_DEVICE_ERROR      An error occurred attempting to set the new
                                station address.
  @retval EFI_UNSUPPORTED       The NIC does not support changing the network
                                interface's station address.

**/
EFI_STATUS
EFIAPI
SnpStationAddress (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL   *This,
  IN  BOOLEAN                       Reset,
  IN  EFI_MAC_ADDRESS               *NewMac
)
{
  return EFI_UNSUPPORTED;
}



/**
  Resets or collects the statistics on a network interface.

  This function resets or collects the statistics on a network interface. If the
  size of the statistics table specified by StatisticsSize is not big enough for
  all the statistics that are collected by the network interface, then a partial
  buffer of statistics is returned in StatisticsTable, StatisticsSize is set to
  the size required to collect all the available statistics, and
  EFI_BUFFER_TOO_SMALL is returned.
  If StatisticsSize is big enough for all the statistics, then StatisticsTable
  will be filled, StatisticsSize will be set to the size of the returned
  StatisticsTable structure, and EFI_SUCCESS is returned.
  If the driver has not been initialized, EFI_DEVICE_ERROR will be returned.
  If Reset is FALSE, and both StatisticsSize and StatisticsTable are NULL, then
  no operations will be performed, and EFI_SUCCESS will be returned.
  If Reset is TRUE, then all of the supported statistics counters on this network
  interface will be reset to zero.

  @param Snp             A pointer to the EFI_SIMPLE_NETWORK_PROTOCOL instance.
  @param Reset           Set to TRUE to reset the statistics for the network interface.
  @param StatSize        On input the size, in bytes, of StatisticsTable. On output
                         the size, in bytes, of the resulting table of statistics.
  @param Statistics      A pointer to the EFI_NETWORK_STATISTICS structure that
                         contains the statistics. Type EFI_NETWORK_STATISTICS is
                         defined in "Related Definitions" below.

  @retval EFI_SUCCESS           The requested operation succeeded.
  @retval EFI_NOT_STARTED       The Simple Network Protocol interface has not been
                                started by calling Start().
  @retval EFI_BUFFER_TOO_SMALL  StatisticsSize is not NULL and StatisticsTable is
                                NULL. The current buffer size that is needed to
                                hold all the statistics is returned in StatisticsSize.
  @retval EFI_BUFFER_TOO_SMALL  StatisticsSize is not NULL and StatisticsTable is
                                not NULL. The current buffer size that is needed
                                to hold all the statistics is returned in
                                StatisticsSize. A partial set of statistics is
                                returned in StatisticsTable.
  @retval EFI_INVALID_PARAMETER StatisticsSize is NULL and StatisticsTable is not
                                NULL.
  @retval EFI_DEVICE_ERROR      The Simple Network Protocol interface has not
                                been initialized by calling Initialize().
  @retval EFI_DEVICE_ERROR      An error was encountered collecting statistics
                                from the NIC.
  @retval EFI_UNSUPPORTED       The NIC does not support collecting statistics
                                from the network interface.

**/
EFI_STATUS
EFIAPI
SnpStatistics (
  IN       EFI_SIMPLE_NETWORK_PROTOCOL   *This,
  IN       BOOLEAN                       Reset,
  IN  OUT  UINTN                         *StatSize,
      OUT  EFI_NETWORK_STATISTICS        *Statistics
  )
{

  SIMPLE_NETWORK_DRIVER   *Snp;

  Snp = INSTANCE_FROM_SNP_THIS (This);

  DEBUG ((DEBUG_INFO, "SNP:DXE: %a ()\r\n", __FUNCTION__));

  // Check Snp instance
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Check that driver was started and initialised
  if (Snp->SnpMode.State == EfiSimpleNetworkStarted) {
    return EFI_DEVICE_ERROR;
  } else if (Snp->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  // Check the parameters
  if ((StatSize == NULL) && (Statistics != NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Do a reset if required
  if (Reset) {
    ZeroMem (&Snp->Stats, sizeof(EFI_NETWORK_STATISTICS));
  }

  // Check buffer size
  if (*StatSize < sizeof(EFI_NETWORK_STATISTICS)) {
    *StatSize = sizeof(EFI_NETWORK_STATISTICS);
    return EFI_BUFFER_TOO_SMALL;
  }

  // read statistic counters
  EmacGetStatistic (&Snp->Stats, Snp->MacBase);

  // Fill in the statistics
  CopyMem (&Statistics, &Snp->Stats, sizeof(EFI_NETWORK_STATISTICS));

  return EFI_SUCCESS;
}

/**
  Converts a multicast IP address to a multicast HW MAC address.

  This function converts a multicast IP address to a multicast HW MAC address
  for all packet transactions. If the mapping is accepted, then EFI_SUCCESS will
  be returned.

  @param Snp         A pointer to the EFI_SIMPLE_NETWORK_PROTOCOL instance.
  @param IsIpv6      Set to TRUE if the multicast IP address is IPv6 [RFC 2460].
                     Set to FALSE if the multicast IP address is IPv4 [RFC 791].
  @param Ip          The multicast IP address that is to be converted to a multicast
                     HW MAC address.
  @param McastMac    The multicast HW MAC address that is to be generated from IP.

  @retval EFI_SUCCESS           The multicast IP address was mapped to the
                                multicast HW MAC address.
  @retval EFI_NOT_STARTED       The Simple Network Protocol interface has not
                                been started by calling Start().
  @retval EFI_INVALID_PARAMETER IP is NULL.
  @retval EFI_INVALID_PARAMETER MAC is NULL.
  @retval EFI_INVALID_PARAMETER IP does not point to a valid IPv4 or IPv6
                                multicast address.
  @retval EFI_DEVICE_ERROR      The Simple Network Protocol interface has not
                                been initialized by calling Initialize().
  @retval EFI_UNSUPPORTED       IPv6 is TRUE and the implementation does not
                                support IPv6 multicast to MAC address conversion.

**/
EFI_STATUS
EFIAPI
SnpMcastIptoMac (
  IN   EFI_SIMPLE_NETWORK_PROTOCOL   *This,
  IN   BOOLEAN                       IsIpv6,
  IN   EFI_IP_ADDRESS                *Ip,
  OUT  EFI_MAC_ADDRESS               *McastMac
  )
{

  DEBUG ((DEBUG_INFO, "SNP:DXE: %a ()\r\n", __FUNCTION__));

  // Check Snp instance
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SIMPLE_NETWORK_DRIVER  *Snp;

  Snp = INSTANCE_FROM_SNP_THIS (This);

  // Check that driver was started and initialised
  if (Snp->SnpMode.State == EfiSimpleNetworkStarted) {
    return EFI_DEVICE_ERROR;
  } else if (Snp->SnpMode.State == EfiSimpleNetworkStopped) {
    return EFI_NOT_STARTED;
  }

  // Check parameters
  if ((McastMac == NULL) || (Ip == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  // Make sure MAC address is empty
  ZeroMem (McastMac, sizeof(EFI_MAC_ADDRESS));

  // If we need ipv4 address
  if (!IsIpv6) {
    // Most significant 25 bits of a multicast HW address are set.
    // 01-00-5E is the IPv4 Ethernet Multicast Address (see RFC 1112)
    McastMac->Addr[0] = 0x01;
    McastMac->Addr[1] = 0x00;
    McastMac->Addr[2] = 0x5E;

    // Lower 23 bits from ipv4 address
    McastMac->Addr[3] = (Ip->v4.Addr[1] & 0x7F); // Clear the most significant bit (25th bit of MAC must be 0)
    McastMac->Addr[4] = Ip->v4.Addr[2];
    McastMac->Addr[5] = Ip->v4.Addr[3];
  } else {
    // Most significant 16 bits of multicast v6 HW address are set
    // 33-33 is the IPv6 Ethernet Multicast Address (see RFC 2464)
    McastMac->Addr[0] = 0x33;
    McastMac->Addr[1] = 0x33;

    // lower four octets are taken from ipv6 address
    McastMac->Addr[2] = Ip->v6.Addr[8];
    McastMac->Addr[3] = Ip->v6.Addr[9];
    McastMac->Addr[4] = Ip->v6.Addr[10];
    McastMac->Addr[5] = Ip->v6.Addr[11];
  }

  return EFI_SUCCESS;
}


/**
  Performs read and write operations on the NVRAM device attached to a network
  interface.

  This function performs read and write operations on the NVRAM device attached
  to a network interface. If ReadWrite is TRUE, a read operation is performed.
  If ReadWrite is FALSE, a write operation is performed. Offset specifies the
  byte offset at which to start either operation. Offset must be a multiple of
  NvRamAccessSize , and it must have a value between zero and NvRamSize.
  BufferSize specifies the length of the read or write operation. BufferSize must
  also be a multiple of NvRamAccessSize, and Offset + BufferSize must not exceed
  NvRamSize.
  If any of the above conditions is not met, then EFI_INVALID_PARAMETER will be
  returned.
  If all the conditions are met and the operation is "read," the NVRAM device
  attached to the network interface will be read into Buffer and EFI_SUCCESS
  will be returned. If this is a write operation, the contents of Buffer will be
  used to update the contents of the NVRAM device attached to the network
  interface and EFI_SUCCESS will be returned.

  It does the basic checking on the input parameters and retrieves snp structure
  and then calls the read_nvdata() call which does the actual reading

  @param Snp        A pointer to the EFI_SIMPLE_NETWORK_PROTOCOL instance.
  @param ReadWrite  TRUE for read operations, FALSE for write operations.
  @param Offset     Byte offset in the NVRAM device at which to start the read or
                    write operation. This must be a multiple of NvRamAccessSize
                    and less than NvRamSize. (See EFI_SIMPLE_NETWORK_MODE)
  @param BufferSize The number of bytes to read or write from the NVRAM device.
                    This must also be a multiple of NvramAccessSize.
  @param Buffer     A pointer to the data buffer.

  @retval EFI_SUCCESS           The NVRAM access was performed.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_INVALID_PARAMETER One or more of the following conditions is TRUE:
                                * The This parameter is NULL
                                * The This parameter does not point to a valid
                                  EFI_SIMPLE_NETWORK_PROTOCOL  structure
                                * The Offset parameter is not a multiple of
                                  EFI_SIMPLE_NETWORK_MODE.NvRamAccessSize
                                * The Offset parameter is not less than
                                  EFI_SIMPLE_NETWORK_MODE.NvRamSize
                                * The BufferSize parameter is not a multiple of
                                  EFI_SIMPLE_NETWORK_MODE.NvRamAccessSize
                                * The Buffer parameter is NULL
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network
                                interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network
                                interface.

**/
EFI_STATUS
EFIAPI
SnpNvData (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL   *This,
  IN  BOOLEAN                       ReadWrite,
  IN  UINTN                         Offset,
  IN  UINTN                         BufferSize,
  IN  OUT VOID                      *Buffer
  )
{
  return EFI_UNSUPPORTED;
}


/**
  Reads the current interrupt status and recycled transmit buffer status from a
  network interface.

  This function gets the current interrupt and recycled transmit buffer status
  from the network interface. The interrupt status is returned as a bit mask in
  InterruptStatus. If InterruptStatus is NULL, the interrupt status will not be
  read. If TxBuf is not NULL, a recycled transmit buffer address will be retrieved.
  If a recycled transmit buffer address is returned in TxBuf, then the buffer has
  been successfully transmitted, and the status for that buffer is cleared. If
  the status of the network interface is successfully collected, EFI_SUCCESS
  will be returned. If the driver has not been initialized, EFI_DEVICE_ERROR will
  be returned.

  @param Snp             A pointer to the EFI_SIMPLE_NETWORK_PROTOCOL instance.
  @param IrqStat         A pointer to the bit mask of the currently active
                         interrupts (see "Related Definitions"). If this is NULL,
                         the interrupt status will not be read from the device.
                         If this is not NULL, the interrupt status will be read
                         from the device. When the interrupt status is read, it
                         will also be cleared. Clearing the transmit interrupt does
                         not empty the recycled transmit buffer array.
  @param TxBuff          Recycled transmit buffer address. The network interface
                         will not transmit if its internal recycled transmit
                         buffer array is full. Reading the transmit buffer does
                         not clear the transmit interrupt. If this is NULL, then
                         the transmit buffer status will not be read. If there
                         are no transmit buffers to recycle and TxBuf is not NULL,
                         TxBuf will be set to NULL.

  @retval EFI_SUCCESS           The status of the network interface was retrieved.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_INVALID_PARAMETER This parameter was NULL or did not point to a valid
                                EFI_SIMPLE_NETWORK_PROTOCOL structure.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network
                                interface.

**/
EFI_STATUS
EFIAPI
SnpGetStatus (
  IN   EFI_SIMPLE_NETWORK_PROTOCOL   *This,
  OUT  UINT32                        *IrqStat  OPTIONAL,
  OUT  VOID                          **TxBuff  OPTIONAL
  )
{
  EFI_STATUS                 Status;
  SIMPLE_NETWORK_DRIVER      *Snp;

  Snp = INSTANCE_FROM_SNP_THIS (This);

  // Check preliminaries
  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (Snp->SnpMode.State != EfiSimpleNetworkInitialized) {
    return EFI_NOT_STARTED;
  }

  // Update the media status
  Status = PhyLinkAdjustEmacConfig (&Snp->PhyDriver, Snp->MacBase);
  if (EFI_ERROR(Status)) {
    Snp->SnpMode.MediaPresent = FALSE;
  } else {
    Snp->SnpMode.MediaPresent = TRUE;
  }

  // TxBuff
  if (TxBuff != NULL) {
    // Get a recycled buf from Snp->RecycledTxBuf
    if (Snp->RecycledTxBufCount == 0) {
      *TxBuff = NULL;
    } else {
      Snp->RecycledTxBufCount--;
      *TxBuff = (VOID *)(UINTN) Snp->RecycledTxBuf[Snp->RecycledTxBufCount];
    }
  }

  // Check DMA Irq status
  EmacGetDmaStatus (IrqStat, Snp->MacBase);

  return EFI_SUCCESS;
}


/**
  Places a packet in the transmit queue of a network interface.

  This function places the packet specified by Header and Buffer on the transmit
  queue. If HeaderSize is nonzero and HeaderSize is not equal to
  This->Mode->MediaHeaderSize, then EFI_INVALID_PARAMETER will be returned. If
  BufferSize is less than This->Mode->MediaHeaderSize, then EFI_BUFFER_TOO_SMALL
  will be returned. If Buffer is NULL, then EFI_INVALID_PARAMETER will be
  returned. If HeaderSize is nonzero and DestAddr or Protocol is NULL, then
  EFI_INVALID_PARAMETER will be returned. If the transmit engine of the network
  interface is busy, then EFI_NOT_READY will be returned. If this packet can be
  accepted by the transmit engine of the network interface, the packet contents
  specified by Buffer will be placed on the transmit queue of the network
  interface, and EFI_SUCCESS will be returned. GetStatus() can be used to
  determine when the packet has actually been transmitted. The contents of the
  Buffer must not be modified until the packet has actually been transmitted.
  The Transmit() function performs nonblocking I/O. A caller who wants to perform
  blocking I/O, should call Transmit(), and then GetStatus() until the
  transmitted buffer shows up in the recycled transmit buffer.
  If the driver has not been initialized, EFI_DEVICE_ERROR will be returned.

  @param Snp        A pointer to the EFI_SIMPLE_NETWORK_PROTOCOL instance.
  @param HdrSize    The size, in bytes, of the media header to be filled in by the
                    Transmit() function. If HeaderSize is nonzero, then it must
                    be equal to This->Mode->MediaHeaderSize and the DestAddr and
                    Protocol parameters must not be NULL.
  @param BuffSize   The size, in bytes, of the entire packet (media header and
                    data) to be transmitted through the network interface.
  @param Data       A pointer to the packet (media header followed by data) to be
                    transmitted. This parameter cannot be NULL. If HeaderSize is
                    zero, then the media header in Buffer must already be filled
                    in by the caller. If HeaderSize is nonzero, then the media
                    header will be filled in by the Transmit() function.
  @param SrcAddr    The source HW MAC address. If HeaderSize is zero, then this
                    parameter is ignored. If HeaderSize is nonzero and SrcAddr
                    is NULL, then This->Mode->CurrentAddress is used for the
                    source HW MAC address.
  @param DstAddr    The destination HW MAC address. If HeaderSize is zero, then
                    this parameter is ignored.
  @param Protocol   The type of header to build. If HeaderSize is zero, then this
                    parameter is ignored. See RFC 1700, section "Ether Types,"
                    for examples.

  @retval EFI_SUCCESS           The packet was placed on the transmit queue.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_NOT_READY         The network interface is too busy to accept this
                                transmit request.
  @retval EFI_BUFFER_TOO_SMALL  The BufferSize parameter is too small.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an unsupported
                                value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network interface.
  @retval EFI_ACCESS_DENIED     Error acquire global lock for operation.

**/
EFI_STATUS
EFIAPI
SnpTransmit (
  IN  EFI_SIMPLE_NETWORK_PROTOCOL   *This,
  IN  UINTN                         HdrSize,
  IN  UINTN                         BuffSize,
  IN  VOID                          *Data,
  IN  EFI_MAC_ADDRESS               *SrcAddr  OPTIONAL,
  IN  EFI_MAC_ADDRESS               *DstAddr  OPTIONAL,
  IN  UINT16                        *Protocol OPTIONAL
  )
{
  SIMPLE_NETWORK_DRIVER      *Snp;
  UINT32                     DescNum;
  DESIGNWARE_HW_DESCRIPTOR   *TxDescriptor;
  DESIGNWARE_HW_DESCRIPTOR   *TxDescriptorMap;
  UINT8                      *EthernetPacket;
  UINT64                     *Tmp;
  EFI_STATUS                 Status;
  UINTN                      BufferSizeBuf;
  EFI_PHYSICAL_ADDRESS       TxBufferAddrMap;

  BufferSizeBuf = ETH_BUFSIZE;
  EthernetPacket = Data;

  Snp = INSTANCE_FROM_SNP_THIS (This);

  if (EFI_ERROR (EfiAcquireLockOrFail (&Snp->Lock))) {
    return EFI_ACCESS_DENIED;
  }

  if ((Snp->MaxRecycledTxBuf + SNP_TX_BUFFER_INCREASE) >= SNP_MAX_TX_BUFFER_NUM) {
    return EFI_NOT_READY;
  }

  // Check preliminaries
  if ((This == NULL) || (Data == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (Snp->SnpMode.State != EfiSimpleNetworkInitialized) {
    return EFI_NOT_STARTED;
  }

  Snp->MacDriver.TxCurrentDescriptorNum = Snp->MacDriver.TxNextDescriptorNum;
  DescNum = Snp->MacDriver.TxCurrentDescriptorNum;

  TxDescriptor = Snp->MacDriver.TxdescRing[DescNum];
  TxDescriptorMap = (VOID *)(UINTN)Snp->MacDriver.TxdescRingMap[DescNum].AddrMap;

  // Ensure header is correct size if non-zero
  if (HdrSize) {
    if (HdrSize != Snp->SnpMode.MediaHeaderSize) {
      return EFI_INVALID_PARAMETER;
    }

    if ((DstAddr == NULL) || (Protocol == NULL)) {
      return EFI_INVALID_PARAMETER;
    }
  }

  // Ensure buffer size is valid
  if (BuffSize < Snp->SnpMode.MediaHeaderSize) {
    return EFI_BUFFER_TOO_SMALL;
  }

  if (HdrSize) {
    EthernetPacket[0] = DstAddr->Addr[0];
    EthernetPacket[1] = DstAddr->Addr[1];
    EthernetPacket[2] = DstAddr->Addr[2];
    EthernetPacket[3] = DstAddr->Addr[3];
    EthernetPacket[4] = DstAddr->Addr[4];
    EthernetPacket[5] = DstAddr->Addr[5];

    EthernetPacket[6] = SrcAddr->Addr[0];
    EthernetPacket[7] = SrcAddr->Addr[1];
    EthernetPacket[8] = SrcAddr->Addr[2];
    EthernetPacket[9] = SrcAddr->Addr[3];
    EthernetPacket[10] = SrcAddr->Addr[4];
    EthernetPacket[11] = SrcAddr->Addr[5];

    EthernetPacket[13] = *Protocol & 0xFF;
    EthernetPacket[12] = (*Protocol & 0xFF00) >> 8;
  }

  CopyMem ((VOID *)(UINTN)TxDescriptor->Addr, EthernetPacket, BuffSize);

  Status = DmaMap (MapOperationBusMasterRead, (VOID *)(UINTN)TxDescriptor->Addr,
             &BufferSizeBuf, &TxBufferAddrMap, &Snp->MappingTxbuf);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () for Txbuffer: %r\n", __FUNCTION__, Status));
    return Status;
  }
  TxDescriptorMap->Addr = TxBufferAddrMap;

  TxDescriptor->Tdes1 = (BuffSize << TDES1_SIZE1SHFT) &
                         TDES1_SIZE1MASK;

  TxDescriptor->Tdes0 |= (TDES0_TXFIRST |
                          TDES0_TXLAST |
                          TDES0_OWN);

  // Increase descriptor number
  DescNum++;

  if (DescNum >= CONFIG_TX_DESCR_NUM) {
    DescNum = 0;
  }

  Snp->MacDriver.TxNextDescriptorNum = DescNum;

  if (Snp->RecycledTxBufCount < Snp->MaxRecycledTxBuf) {
    Snp->RecycledTxBuf[Snp->RecycledTxBufCount] = (UINT64)(UINTN)Data;
    Snp->RecycledTxBufCount ++;
  } else {
    Tmp = AllocatePool (sizeof (UINT64) * (Snp->MaxRecycledTxBuf + SNP_TX_BUFFER_INCREASE));
    if (Tmp == NULL) {
      return EFI_DEVICE_ERROR;
    }
    CopyMem (Tmp, Snp->RecycledTxBuf, sizeof (UINT64) * Snp->RecycledTxBufCount);
    FreePool (Snp->RecycledTxBuf);
    Snp->RecycledTxBuf = Tmp;
    Snp->MaxRecycledTxBuf += SNP_TX_BUFFER_INCREASE;
  }

  // Start the transmission
  EmacDmaStart (Snp->MacBase);

  DmaUnmap (Snp->MappingTxbuf);
  EfiReleaseLock (&Snp->Lock);
  return EFI_SUCCESS;
}

/**
  Receives a packet from a network interface.

  This function retrieves one packet from the receive queue of a network interface.
  If there are no packets on the receive queue, then EFI_NOT_READY will be
  returned. If there is a packet on the receive queue, and the size of the packet
  is smaller than BufferSize, then the contents of the packet will be placed in
  Buffer, and BufferSize will be updated with the actual size of the packet.
  In addition, if SrcAddr, DestAddr, and Protocol are not NULL, then these values
  will be extracted from the media header and returned. EFI_SUCCESS will be
  returned if a packet was successfully received.
  If BufferSize is smaller than the received packet, then the size of the receive
  packet will be placed in BufferSize and EFI_BUFFER_TOO_SMALL will be returned.
  If the driver has not been initialized, EFI_DEVICE_ERROR will be returned.

  @param Snp        A pointer to the EFI_SIMPLE_NETWORK_PROTOCOL instance.
  @param HdrSize    The size, in bytes, of the media header received on the network
                    interface. If this parameter is NULL, then the media header size
                    will not be returned.
  @param BuffSize   On entry, the size, in bytes, of Buffer. On exit, the size, in
                    bytes, of the packet that was received on the network interface.
  @param Data       A pointer to the data buffer to receive both the media
                    header and the data.
  @param SrcAddr    The source HW MAC address. If this parameter is NULL, the HW
                    MAC source address will not be extracted from the media header.
  @param DstAddr   The destination HW MAC address. If this parameter is NULL,
                    the HW MAC destination address will not be extracted from
                    the media header.
  @param Protocol   The media header type. If this parameter is NULL, then the
                    protocol will not be extracted from the media header. See
                    RFC 1700 section "Ether Types" for examples.

  @retval EFI_SUCCESS           The received data was stored in Buffer, and
                                BufferSize has been updated to the number of
                                bytes received.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_NOT_READY         No packets have been received on the network interface.
  @retval EFI_BUFFER_TOO_SMALL  BufferSize is too small for the received packets.
                                BufferSize has been updated to the required size.
  @retval EFI_INVALID_PARAMETER One or more of the following conditions is TRUE:
                                * The This parameter is NULL
                                * The This parameter does not point to a valid
                                  EFI_SIMPLE_NETWORK_PROTOCOL structure.
                                * The BufferSize parameter is NULL
                                * The Buffer parameter is NULL
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network interface.
  @retval EFI_ACCESS_DENIED     Error acquire global lock for operation.

**/
EFI_STATUS
EFIAPI
SnpReceive (
  IN       EFI_SIMPLE_NETWORK_PROTOCOL   *This,
      OUT  UINTN                         *HdrSize      OPTIONAL,
  IN  OUT  UINTN                         *BuffSize,
      OUT  VOID                          *Data,
      OUT  EFI_MAC_ADDRESS               *SrcAddr      OPTIONAL,
      OUT  EFI_MAC_ADDRESS               *DstAddr      OPTIONAL,
      OUT  UINT16                        *Protocol     OPTIONAL
  )
{
  SIMPLE_NETWORK_DRIVER      *Snp;
  EFI_MAC_ADDRESS            Dst;
  EFI_MAC_ADDRESS            Src;
  UINT32                     Length;
  UINT32                     DescriptorStatus;
  UINT8                      *RawData;
  UINT32                     DescNum;
  DESIGNWARE_HW_DESCRIPTOR   *RxDescriptor;
  DESIGNWARE_HW_DESCRIPTOR   *RxDescriptorMap;
  UINTN                      BufferSizeBuf;
  UINTN                      *RxBufferAddr;
  EFI_PHYSICAL_ADDRESS       RxBufferAddrMap;
  EFI_STATUS                 Status;

  BufferSizeBuf = ETH_BUFSIZE;

  Snp = INSTANCE_FROM_SNP_THIS (This);

  // Check preliminaries
  if ((This == NULL) || (Data == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (Snp->SnpMode.State != EfiSimpleNetworkInitialized) {
    return EFI_NOT_STARTED;
  }

  if (EFI_ERROR (EfiAcquireLockOrFail (&Snp->Lock))) {
    return EFI_ACCESS_DENIED;
  }

  Snp->MacDriver.RxCurrentDescriptorNum = Snp->MacDriver.RxNextDescriptorNum;
  DescNum = Snp->MacDriver.RxCurrentDescriptorNum;
  RxDescriptor = Snp->MacDriver.RxdescRing[DescNum];
  RxBufferAddr = (UINTN*)((UINTN)Snp->MacDriver.RxBuffer +
                          (DescNum * BufferSizeBuf));
  RxDescriptorMap = (VOID *)(UINTN)Snp->MacDriver.RxdescRingMap[DescNum].AddrMap;

  RawData = (UINT8 *) Data;

  DescriptorStatus = RxDescriptor->Tdes0;
  if (DescriptorStatus & ((UINT32)RDES0_OWN)) {
    goto ReleaseLock;
  }

  if (DescriptorStatus & RDES0_SAF) {
    DEBUG ((DEBUG_WARN, "SNP:DXE: Rx Descritpor Status Error: Source Address Filter Fail\n"));
    return EFI_DEVICE_ERROR;
  }

  if (DescriptorStatus & RDES0_AFM) {
    DEBUG ((DEBUG_WARN, "SNP:DXE: Rx Descritpor Status Error: Destination Address Filter Fail\n"));
    return EFI_DEVICE_ERROR;
  }

  if (DescriptorStatus & RDES0_ES) {
    // Check for errors
    if (DescriptorStatus & RDES0_RE) {
      DEBUG ((DEBUG_WARN, "SNP:DXE: Rx Descritpor Status Error: Receive Error\n"));
    }
    if (DescriptorStatus & RDES0_DE) {
      DEBUG ((DEBUG_WARN, "SNP:DXE: Rx Descritpor Status Error: Receive Error\n"));
    }
    if (DescriptorStatus & RDES0_RWT) {
      DEBUG ((DEBUG_WARN, "SNP:DXE: Rx Descritpor Status Error: Watchdog Timeout\n"));
    }
    if (DescriptorStatus & RDES0_LC) {
      DEBUG ((DEBUG_WARN, "SNP:DXE: Rx Descritpor Status Error: Late Collision\n"));
    }
    if (DescriptorStatus & RDES0_GF) {
      DEBUG ((DEBUG_WARN, "SNP:DXE: Rx Descritpor Status Error: Giant Frame\n"));
    }
    if (DescriptorStatus & RDES0_OE) {
      DEBUG ((DEBUG_WARN, "SNP:DXE: Rx Descritpor Status Error: Overflow Error\n"));
    }
    if (DescriptorStatus & RDES0_LE) {
      DEBUG ((DEBUG_WARN, "SNP:DXE: Rx Descritpor Status Error:Length Error\n"));
    }
    if (DescriptorStatus & RDES0_DBE) {
      DEBUG ((DEBUG_WARN, "SNP:DXE: Rx Descritpor Status Error: Dribble Bit Error\n"));
    }

    // Check descriptor error status
    if (DescriptorStatus & RDES0_CE) {
      DEBUG ((DEBUG_WARN, "SNP:DXE: Rx Descritpor Status Error: CRC Error\n"));
    }
    return EFI_DEVICE_ERROR;
  }

  Length = (DescriptorStatus >> RDES0_FL_SHIFT) & RDES0_FL_MASK;
  if (!Length) {
    DEBUG ((DEBUG_WARN, "SNP:DXE: Error: Invalid Frame Packet length \r\n"));
    return EFI_NOT_READY;
  }
  // Check buffer size
  if (*BuffSize < Length) {
    DEBUG ((DEBUG_WARN, "SNP:DXE: Error: Buffer size is too small\n"));
    return EFI_BUFFER_TOO_SMALL;
  }
  *BuffSize = Length;

  if (HdrSize != NULL)
    *HdrSize = Snp->SnpMode.MediaHeaderSize;

  DmaUnmap (Snp->MacDriver.RxBufNum[DescNum].Mapping);
  Snp->MacDriver.RxBufNum[DescNum].Mapping = NULL;

  CopyMem (RawData, (VOID *)RxBufferAddr, *BuffSize);

  if (DstAddr != NULL) {
    Dst.Addr[0] = RawData[0];
    Dst.Addr[1] = RawData[1];
    Dst.Addr[2] = RawData[2];
    Dst.Addr[3] = RawData[3];
    Dst.Addr[4] = RawData[4];
    Dst.Addr[5] = RawData[5];
    CopyMem (DstAddr, &Dst, NET_ETHER_ADDR_LEN);
    DEBUG ((DEBUG_INFO, "received from source address %x %x\r\n", DstAddr, &Dst));
  }

  // Get the source address
  if (SrcAddr != NULL) {
    Src.Addr[0] = RawData[6];
    Src.Addr[1] = RawData[7];
    Src.Addr[2] = RawData[8];
    Src.Addr[3] = RawData[9];
    Src.Addr[4] = RawData[10];
    Src.Addr[5] = RawData[11];
    DEBUG ((DEBUG_INFO, "received from source address %x %x\r\n", SrcAddr, &Src));
    CopyMem (SrcAddr, &Src, NET_ETHER_ADDR_LEN);
  }

  // Get the protocol
  if (Protocol != NULL) {
    *Protocol = NTOHS (RawData[12] | (RawData[13] >> 8) | (RawData[14] >> 16) | (RawData[15] >> 24));
  }

  // DMA map for the current receive buffer
  Status = DmaMap (MapOperationBusMasterWrite,  (VOID *)RxBufferAddr,
             &BufferSizeBuf, &RxBufferAddrMap, &Snp->MacDriver.RxBufNum[DescNum].Mapping);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () for Rxbuffer: %r\n", __FUNCTION__, Status));
    return Status;
  }
  Snp->MacDriver.RxBufNum[DescNum].AddrMap = RxBufferAddrMap;
  RxDescriptorMap->Addr = Snp->MacDriver.RxBufNum[DescNum].AddrMap;

  RxDescriptor->Tdes0 |= (UINT32)RDES0_OWN;

  // Increase descriptor number
  DescNum++;

  if (DescNum >= CONFIG_RX_DESCR_NUM) {
    DescNum = 0;
  }
  Snp->MacDriver.RxNextDescriptorNum = DescNum;

  EfiReleaseLock (&Snp->Lock);
  return EFI_SUCCESS;

ReleaseLock:
  EfiReleaseLock (&Snp->Lock);
  return EFI_NOT_READY;
}

