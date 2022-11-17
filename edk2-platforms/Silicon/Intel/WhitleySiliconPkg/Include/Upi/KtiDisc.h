/** @file

  @copyright
  Copyright 2004 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _KTI_DISCOVERY_H_
#define _KTI_DISCOVERY_H_

#include "DataTypes.h"
#include <Upi/KtiSi.h>

#pragma pack(1)

//
// Generic Data structure to describe Link Exchange Parameter (LEP) info
//
typedef struct {
  UINT32  Valid : 1;              ///< TRUE, if the link is valid (i.e trained successfully for low speed, no validation override that disables it)
  UINT32  PeerSocId : 3;          ///< Socket ID
  UINT32  PeerSocType : 2;        ///< Socket Type
  UINT32  PeerPort : 4;           ///< Port of the peer socket
  UINT32  DualLink : 1;           ///< TRUE, if there is a second link to the same neighbor
  UINT32  TwoSkt3Link : 1;        ///< TRUE, if there is a second and third link to the same neighbor
  UINT32  TwoSkt4Link : 1;        ///< TRUE, if there are 4 links between 2 sockets
  UINT32  DualLinkIndex : 3;      ///< Index of the second link that is connected to the same immediate neighbor
  UINT32  DisallowRouteThru : 1;  ///< TRUE if the link is not allowed to configure as route through traffic
  UINT32  SpokeOfPinwheel : 1;    ///< TRUE if the link is chosen as spoke of pinwheel
  UINT32  Rsvd1 : 14;
} KTI_LINK_DATA;

#pragma pack()

#endif // _KTI_DISCOVERY_H_
