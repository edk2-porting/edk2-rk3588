/** @file
  SA Platform Lib implementation.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include "SaPlatformLibrary.h"
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <CpuPcieInfo.h>


/**
  Checks if SKU is Mobile

  @retval FALSE  SKU is not Mobile
  @retval TRUE   SKU is Mobile
**/
BOOLEAN
EFIAPI
IsMobileSku (
  VOID
  )
{
  UINT16           DeviceId;

  DeviceId = PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, R_SA_MC_DEVICE_ID));
  if (
      (DeviceId == V_SA_DEVICE_ID_MB_ULT_1) || \
      (DeviceId == V_SA_DEVICE_ID_MB_ULT_2) || \
      (DeviceId == V_SA_DEVICE_ID_MB_ULX_1) || \
      (DeviceId == V_SA_DEVICE_ID_MB_ULX_2) \
    ) {
    return TRUE;
  }
  return FALSE;
}

/**
  Checks if SKU is Desktop

  @retval FALSE  SKU is not Desktop
  @retval TRUE   SKU is Desktop
**/
BOOLEAN
EFIAPI
IsDesktopSku (
  VOID
  )
{
  return FALSE;
}

/**
  Checks if SKU is Server

  @retval FALSE  SKU is not Server
  @retval TRUE   SKU is Server
**/
BOOLEAN
EFIAPI
IsServerSku (
  VOID
  )
{
  return FALSE;
}

