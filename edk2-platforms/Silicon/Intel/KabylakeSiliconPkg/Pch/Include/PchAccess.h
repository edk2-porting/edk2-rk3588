/** @file
  Macros that simplify accessing PCH devices's PCI registers.

Copyright (c) 2017, Intel Corporation. All rights reserved.<BR>
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


///
/// The default PCH PCI bus number
///
#define DEFAULT_PCI_BUS_NUMBER_PCH  0

//
// Default Vendor ID and Subsystem ID
//
#define V_PCH_INTEL_VENDOR_ID   0x8086      ///< Default Intel PCH Vendor ID
#define V_PCH_DEFAULT_SID       0x7270      ///< Default Intel PCH Subsystem ID
#define V_PCH_DEFAULT_SVID_SID  (V_INTEL_VENDOR_ID + (V_PCH_DEFAULT_SID << 16))   ///< Default INTEL PCH Vendor ID and Subsystem ID

//
// Include device register definitions
//
#include "PcieRegs.h"
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
#include "Register/PchRegsThermal.h"
#include "Register/PchRegsUsb.h"
#include "Register/PchRegsGpio.h"
#include "Register/PchRegsSerialIo.h"
#include "Register/PchRegsCam.h"
#include "Register/PchRegsTraceHub.h"
#include "Register/PchRegsScs.h"
#include "Register/PchRegsIsh.h"
#include "Register/PchRegsDmi.h"
#include "Register/PchRegsItss.h"
#include "Register/PchRegsPsth.h"
#include "Register/PchRegsPsf.h"
#include "Register/PchRegsFia.h"
#include "Register/PchRegsDci.h"

#endif

