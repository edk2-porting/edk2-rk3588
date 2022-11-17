/** @file
  This file provides services for Gpio policy function

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Protocol/PchPolicy.h>
#include <ConfigBlock/Gpio/GpioDevConfig.h>

/**
  Print GPIO_DXE_CONFIG and serial out.

  @param[in] PchPolicy            Pointer to a PCH_POLICY_PROTOCOL
**/
VOID
GpioDxePrintConfig (
  IN PCH_POLICY_PROTOCOL    *PchPolicy
  )
{
  EFI_STATUS        Status;
  GPIO_DXE_CONFIG   *GpioDxeConfig;

  Status = GetConfigBlock ((VOID *) PchPolicy, &gGpioDxeConfigGuid, (VOID *) &GpioDxeConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ GPIO DXE Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " HideGpioAcpiDevice : %d\n", GpioDxeConfig->HideGpioAcpiDevice));
}

/**
  Load DXE Config block default for GPIO

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
GpioDxeLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  GPIO_DXE_CONFIG  *GpioDxeConfig;
  GpioDxeConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "GpioDxeConfig->Header.GuidHob.Name = %g\n", &GpioDxeConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "GpioDxeConfig->Header.GuidHob.Header.HobLength = 0x%x\n", GpioDxeConfig->Header.GuidHob.Header.HobLength));

  GpioDxeConfig->HideGpioAcpiDevice = 0;
}

STATIC COMPONENT_BLOCK_ENTRY  mGpioBlocks = {
  &gGpioDxeConfigGuid,
  sizeof (GPIO_DXE_CONFIG),
  GPIO_DXE_CONFIG_REVISION,
  GpioDxeLoadConfigDefault
};

/**
  Get Gpio config block table size.

  @retval      Size of config block
**/
UINT16
GpioDxeGetConfigBlockTotalSize (
  VOID
  )
{
  return mGpioBlocks.Size;
}

/**
  Add Gpio ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
GpioDxeAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mGpioBlocks, 1);
}
