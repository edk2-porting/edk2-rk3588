/** @file
  Header file for SaPlatformLib.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _SA_PLATFORM_LIB_H_
#define _SA_PLATFORM_LIB_H_

#include <SaAccess.h>
#include <CpuAccess.h>

/**
  Determine if PCH Link is DMI/OPI

  @param[in] CpuModel             CPU model

  @retval TRUE                    DMI
  @retval FALSE                   OPI
**/
BOOLEAN
IsPchLinkDmi (
  IN CPU_FAMILY  CpuModel
  );

/**
  Returns the number of DMI lanes for current CPU

  @retval UINT8
**/
UINT8
GetMaxDmiLanes (
  );


/**
  Returns the number of DMI bundles for current CPU

  @retval UINT8
**/
UINT8
GetMaxDmiBundles (
  );


/**
  Returns the function numbers for current CPU

  @retval UINT8
**/
UINT8
GetMaxPegFuncs (
  );


/**
  Returns the number of DMI lanes for current CPU

  @retval UINT8
**/
UINT8
GetMaxPegLanes (
  );


/**
  Returns the number of DMI bundles for current CPU

  @retval UINT8
**/
UINT8
GetMaxPegBundles (
  );

/**
  Checks if PEG port is present

  @retval TRUE     PEG is presented
  @retval FALSE    PEG is not presented
**/
BOOLEAN
IsPegPresent (
  VOID
  );

#endif
