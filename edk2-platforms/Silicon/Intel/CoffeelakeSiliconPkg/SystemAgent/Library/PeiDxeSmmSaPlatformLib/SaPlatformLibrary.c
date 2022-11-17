/** @file
  SA Platform Lib implementation.

  Copyright (c) 2019 Intel Corporation. All rights reserved. <BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include "SaPlatformLibrary.h"
#include <Library/PciSegmentLib.h>
#include <SaRegs.h>
#include <Library/CpuPlatformLib.h>

/**
  Determine if PCH Link is DMI/OPI

  @param[in] CpuModel             CPU model

  @retval TRUE                    DMI
  @retval FALSE                   OPI
**/
BOOLEAN
IsPchLinkDmi (
  IN CPU_FAMILY  CpuModel
  )
{
  if ((CpuModel == EnumCpuCflDtHalo) || (CpuModel == EnumCpuCnlDtHalo)) {
    return TRUE; // DMI
  }
  return FALSE;  // OPI
}


/**
  Returns the number of DMI lanes for current CPU

  @retval UINT8
**/
UINT8
GetMaxDmiLanes (
  )
{
    return SA_DMI_CFL_MAX_LANE;
}


/**
  Returns the number of DMI bundles for current CPU

  @retval UINT8
**/
UINT8
GetMaxDmiBundles (
  )
{
    return SA_DMI_CFL_MAX_BUNDLE;
}


/**
  Returns the function numbers for current CPU

  @retval UINT8
**/
UINT8
GetMaxPegFuncs (
  )
{
  if (GetCpuFamily() == EnumCpuCnlDtHalo) {
    return SA_PEG_CNL_H_MAX_FUN;
  } else {
    return SA_PEG_NON_CNL_H_MAX_FUN;
  }
}


/**
  Returns the number of PEG lanes for current CPU

  @retval UINT8
**/
UINT8
GetMaxPegLanes (
  )
{
  if (GetCpuFamily() == EnumCpuCnlDtHalo) {
    return SA_PEG_CNL_H_MAX_LANE;
  } else {
    return SA_PEG_NON_CNL_H_MAX_LANE;
  }
}


/**
  Returns the number of PEG bundles for current CPU

  @retval UINT8
**/
UINT8
GetMaxPegBundles (
  )
{
  if (GetCpuFamily() == EnumCpuCnlDtHalo) {
    return  SA_PEG_CNL_H_MAX_BUNDLE;
  } else {
    return  SA_PEG_NON_CNL_H_MAX_BUNDLE;
  }
}

/**
  Checks if PEG port is present

  @retval TRUE     PEG is presented
  @retval FALSE    PEG is not presented
**/
BOOLEAN
IsPegPresent (
  VOID
  )
{
  UINT64  PegBaseAddress;

  PegBaseAddress  = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_PEG_BUS_NUM, SA_PEG_DEV_NUM, 0, 0);
  if (PciSegmentRead16 (PegBaseAddress) != 0xFFFF) {
    return TRUE;
  }
  return FALSE;
}
