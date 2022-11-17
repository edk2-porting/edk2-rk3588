/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _KTI_DISCOVERY_H_
#define _KTI_DISCOVERY_H_
#ifdef _MSC_VER
#pragma warning (disable: 4127 4214 4100)     // disable C4127: constant conditional expression
#endif
#include "DataTypes.h"
#include "PlatformHost.h"
#include "KtiSi.h"

#define MAX_TREE_NODES       (MAX_SOCKET + 2) // 2 additional nodes since a node will appear more than once in the tree when it is being constructed
#define MAX_RING_TREE_NODES  46  // A CPU with 3 links supported will have 1 + 1*3 + 3*2 + 6*2 + 12*2 = 46 nodes maximum in ring tree
#define MAX_RINGS            6   // Maximum number of rings possible in systems with upto 8 sockets (HyperCube)
#define CPUS_PER_RING        4   // # of CPUs in a CPU ring
#define VN0                  0
#define VN1                  1
#define TX                   0
#define RX                   1

#endif // _KTI_DISCOVERY_H_
