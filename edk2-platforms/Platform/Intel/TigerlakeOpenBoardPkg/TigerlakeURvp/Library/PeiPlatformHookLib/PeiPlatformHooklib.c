/** @file
  PEI Library Functions. Initialize GPIOs


  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <PeiPlatformHookLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PmcLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioLib.h>
#include <PlatformBoardConfig.h>
#include <Library/PchPcrLib.h>
#include <Library/GpioCheckConflictLib.h>

#define SIO_RUNTIME_REG_BASE_ADDRESS                          0x0680

#define RECOVERY_MODE_GPIO_PIN                    0                    // Platform specific @todo use PCD

#define MANUFACTURE_MODE_GPIO_PIN                 0                    // Platform specific @todo use PCD

/**
  Configures GPIO

  @param[in]  GpioTable       Point to Platform Gpio table
  @param[in]  GpioTableCount  Number of Gpio table entries

**/
VOID
ConfigureGpio (
  IN GPIO_INIT_CONFIG                 *GpioDefinition,
  IN UINT16                           GpioTableCount
  )
{
  DEBUG ((DEBUG_INFO, "ConfigureGpio() Start\n"));


  CreateGpioCheckConflictHob (GpioDefinition, GpioTableCount);


  GpioConfigurePads (GpioTableCount, GpioDefinition);

  DEBUG ((DEBUG_INFO, "ConfigureGpio() End\n"));
}

/**
  Configure GPIO group GPE tier.

  @retval     none.
**/
VOID
GpioGroupTierInitHook(
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "GpioGroupTierInitHook Start\n"));

  DEBUG ((DEBUG_INFO, "GpioGroupTierInitHook End\n"));
}

/**
  Configure single GPIO pad for touchpanel interrupt
**/
VOID
TouchpanelGpioInit (
  VOID
  )
{

}

/**
  Configure GPIO Before Memory is not ready.

**/
VOID
GpioInitPreMem (
  VOID
  )
{
  if (PcdGet32 (PcdBoardGpioTablePreMem) != 0 && PcdGet16 (PcdBoardGpioTablePreMemSize) != 0) {
    DEBUG ((DEBUG_INFO, "Pre-mem Gpio Config\n"));
    ConfigureGpio ((VOID *) (UINTN) PcdGet32 (PcdBoardGpioTablePreMem), (UINTN) PcdGet16 (PcdBoardGpioTablePreMemSize));
  }
}

/**
  Basic GPIO configuration before memory is ready

**/
VOID
GpioInitEarlyPreMem (
  VOID
  )
{

}

/**
  Configure GPIO

**/

VOID
GpioInit (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "Post-mem Gpio Config\n"));
  ConfigureGpio ((VOID *) (UINTN) PcdGet32 (PcdBoardGpioTable), (UINTN) PcdGet16 (PcdBoardGpioTableSize));

  TouchpanelGpioInit();

  return;
}

/**
  Configure Super IO

**/
VOID
SioInit (
  VOID
  )
{
  //
  // Program and Enable Default Super IO Configuration Port Addresses and range
  //
  PchLpcGenIoRangeSet (PcdGet16 (PcdLpcSioConfigDefaultPort) & (~0xF), 0x10);

  PchLpcGenIoRangeSet (SIO_RUNTIME_REG_BASE_ADDRESS  & (~0x7F), 0x10);
  return;
}

/**
  Configure GPIO and SIO before memory ready

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
BoardInitPreMem (
  VOID
  )
{
  //
  // Obtain Platform Info from HOB.
  //
  GpioInitPreMem ();
  GpioGroupTierInitHook ();
  SioInit ();

  DEBUG ((DEBUG_INFO, "BoardInitPreMem Done\n"));

  return EFI_SUCCESS;
}

/**
  Configure GPIO and SIO

  @retval  EFI_SUCCESS   Operation success.
**/
EFI_STATUS
BoardInit (
  VOID
  )
{

  GpioInit ();

  return EFI_SUCCESS;
}

/**
  Do platform specific programming post-memory.

  @retval  EFI_SUCCESS       The function completed successfully.
**/

EFI_STATUS
PlatformSpecificInit (
  VOID
  )
{

  return EFI_SUCCESS;
}

/**
  Early Board Configuration before memory is ready

  @retval  EFI_SUCCESS  Operation success.
**/
EFI_STATUS
BoardInitEarlyPreMem (
  VOID
  )
{
  GpioInitEarlyPreMem ();

  return EFI_SUCCESS;
}
