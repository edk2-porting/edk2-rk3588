/** @file
  IPMI Transport common layer driver head file

  @copyright
  Copyright 1999 - 2021 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _IPMI_BMC_H_
#define _IPMI_BMC_H_

#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/IpmiBaseLib.h>
#include <Protocol/IpmiTransportProtocol.h>

#include "IpmiBmcCommon.h"
#include "KcsBmc.h"


#define BMC_KCS_TIMEOUT  5   // [s] Single KSC request timeout

//
// IPMI Instance signature
//
#define SM_IPMI_BMC_SIGNATURE SIGNATURE_32 ('i', 'p', 'm', 'i')
#define IPMI_SEND_COMMAND_MAX_RETRY  3   // Number of retries
#define INSTANCE_FROM_SM_IPMI_BMC_THIS(a) \
  CR ( \
  a, \
  IPMI_BMC_INSTANCE_DATA, \
  IpmiTransport, \
  SM_IPMI_BMC_SIGNATURE \
  )

#endif // _IPMI_BMC_H_
