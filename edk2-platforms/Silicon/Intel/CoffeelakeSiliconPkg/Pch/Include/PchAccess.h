/** @file
  Macros that simplify accessing PCH devices's PCI registers.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PCH_ACCESS_H_
#define _PCH_ACCESS_H_

#include "PchLimits.h"
#include "PchReservedResources.h"

#ifndef STALL_ONE_MICRO_SECOND
#define STALL_ONE_MICRO_SECOND 1
#endif
#ifndef STALL_ONE_SECOND
#define STALL_ONE_SECOND 1000000
#endif

//
// Include device register definitions
//
#include "PcieRegs.h"
#include "Register/PchRegs.h"
#include "Register/PchRegsPcr.h"
#include "Register/PchRegsP2sb.h"
#include "Register/PchRegsHda.h"
#include "Register/PchRegsHsio.h"
#include "Register/PchRegsLan.h"
#include "Register/PchRegsLpc.h"
#include "Register/PchRegsPmc.h"
#include "Register/PchRegsPcie.h"
#include "Register/PchRegsSata.h"
#include "Register/PchRegsSmbus.h"
#include "Register/PchRegsSpi.h"
#include <Register/RegsUsb.h>
#include "Register/PchRegsGpio.h"
#include "Register/PchRegsThermalCnl.h"
#include "Register/PchRegsGpioCnl.h"
#include "Register/PchRegsSerialIoCnl.h"
#include "Register/PchRegsSerialIo.h"
#include "Register/PchRegsTraceHub.h"
#include "Register/PchRegsScsCnl.h"
#include "Register/PchRegsIsh.h"
#include "Register/PchRegsDmi.h"
#include "Register/PchRegsItss.h"
#include "Register/PchRegsPsth.h"
#include "Register/PchRegsFia.h"
#include "Register/PchRegsDci.h"

#endif

