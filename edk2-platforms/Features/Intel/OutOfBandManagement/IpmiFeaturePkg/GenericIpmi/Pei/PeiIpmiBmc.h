/** @file
  Generic IPMI transport layer head file during PEI phase

  @copyright
  Copyright 2016 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _PEI_IPMI_BMC_H_
#define _PEI_IPMI_BMC_H_

#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/IpmiBaseLib.h>

#include <Ppi/IpmiTransportPpi.h>

#include "PeiIpmiBmcDef.h"
#include "KcsBmc.h"

//
// IPMI Instance signature
//
#define SM_IPMI_BMC_SIGNATURE SIGNATURE_32 ('i', 'p', 'm', 'i')

#define INSTANCE_FROM_PEI_SM_IPMI_BMC_THIS(a) \
  CR ( \
  a, \
  PEI_IPMI_BMC_INSTANCE_DATA, \
  IpmiTransportPpi, \
  SM_IPMI_BMC_SIGNATURE \
  )

#endif // _PEI_IPMI_BMC_H_
