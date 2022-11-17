/** @file

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _KTI_SI_H_
#define _KTI_SI_H_
#ifdef _MSC_VER
#pragma warning (disable: 4127 4214 4100)     // disable C4127: constant conditional expression
#endif
#include "DataTypes.h"
#include "PlatformHost.h"

/*********************************************************
                KTI Topology Related
*********************************************************/
#define SI_MAX_CPU_SOCKETS  8    // Maximum CPU sockets  supported by Si
#define SI_MAX_KTI_PORTS    3    // Maximum KTI ports supported by Si

/*********************************************************
                      IIO Stacks
*********************************************************/
#define IIO_CSTACK          0
#define IIO_PSTACK0         1
#define IIO_PSTACK1         2
#define IIO_PSTACK2         3
#define IIO_PSTACK3         4
#define IIO_PSTACK4         5
#define MAX_IIO_STACK       6

/*********************************************************
                      M3KTI
*********************************************************/
#define MAX_M3KTI             2
#define MAX_PORT_IN_M3KTI     2

#endif // _KTI_SI_H_
