/**
 * @file
 *
 * Network Definitions.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      STYX
 * @e sub-project:  (TBD)
 * @e \$Revision$        @e  \$Date$
 *
 **/
/*****************************************************************************
*
*  Copyright 2013 - 2016 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
*
*  This program and the accompanying materials are licensed and made available
*  under the terms and conditions of the BSD License which accompanies this
*  distribution. The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR
*  IMPLIED.
*
***************************************************************************/


#ifndef __NETWORK_ADDRESS_H__
#define __NETWORK_ADDRESS_H__

/// Indicates the status of an IP address field within a structure
  typedef enum {
    DISABLED,                   ///< Disabled
    ENABLED                     ///< Enabled
  } IP_ADDRESS_STATUS;

/// Structure for an IPv4 address
  typedef struct {
    UINT32      Status;         ///< Indicates if the address is valid
    UINT8       IpAddress[4];   ///< IPv4 address data, if enabled (xxx.xxx.xxx.xxx)
  } ISCP_BMC_IPV4_ADDRESS;

/// Structure for an IPv6 address
  typedef struct {
    UINT32      Status;         ///< Indicates if the address is valid
    UINT8       IpAddress[16];  ///< IPv6 address data, if enabled (xxxx:xxxx:xxxx:xxx:xxxx:xxxx:xxxx:xxxx)
  } ISCP_BMC_IPV6_ADDRESS;

/// Structure for any combination of an IPv4 and an IPv6 address
  typedef struct {
    ISCP_BMC_IPV4_ADDRESS       Ipv4Address;  ///< IPv4 Network Address Structure
    ISCP_BMC_IPV6_ADDRESS       Ipv6Address;  ///< IPv6 Network Address Structure
  } IP_ADDRESS_INFO;

#endif /* __NETWORK_ADDRESS_H__ */
