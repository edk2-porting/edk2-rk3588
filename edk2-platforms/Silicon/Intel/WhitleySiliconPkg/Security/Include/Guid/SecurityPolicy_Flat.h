/** @file
  Provides data structure information used by ServerSecurity features in literally all products
  Header is flat and injected directly in SecurityPolicy structure and SOCKET_PROCESSORCORE_CONFIGURATION.

  @copyright
  Copyright 2020 - 2021 Intel Corporation. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

  // Header is flat and injected directly in SecurityPolicy structure and SOCKET_PROCESSORCORE_CONFIGURATION.
  // Put common definitons here either directly or via intermediate header file..

// SECURITY_IP_MKTME_1V0  MkTme;
#include "SecurityIp/SecurityIpMkTme1v0_Inputs.h"
#include "SecurityIp/SecurityIpMkTme1v0_Outputs.h"
// SECURITY_IP_SGXTEM_1V0 SgxTem;
#include "SecurityIp/SecurityIpSgxTem1v0_Inputs.h"
#include "SecurityIp/SecurityIpSgxTem1v0_Outputs.h"
// SECURITY_IP_TDX_1V0    Tdx;
#include "SecurityIp/SecurityIpTdx1v0_Inputs.h"
#include "SecurityIp/SecurityIpTdx1v0_Outputs.h"