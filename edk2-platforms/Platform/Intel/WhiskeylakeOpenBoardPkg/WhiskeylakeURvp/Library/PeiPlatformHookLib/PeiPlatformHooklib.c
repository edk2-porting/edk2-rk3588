/** @file
  PEI Library Functions. Initialize GPIOs


  Copyright (c) 2019, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <PiPei.h>
#include <PeiPlatformHookLib.h>
#include <SaPolicyCommon.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/HobLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PeiPlatformLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PmcLib.h>
#include <Library/PeiSaPolicyLib.h>
#include <PchAccess.h>
#include <Library/CpuPlatformLib.h>
#include <Library/GpioNativeLib.h>
#include <Library/GpioLib.h>
#include <GpioPinsCnlLp.h>
#include <GpioPinsCnlH.h>
#include <Library/PchInfoLib.h>
#include <Library/CnviLib.h>
#include <SioRegs.h>
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

  if (PcdGet32 (PcdGpioGroupToGpeDw0)) {
    GpioSetGroupToGpeDwX (PcdGet32 (PcdGpioGroupToGpeDw0),
                          PcdGet32 (PcdGpioGroupToGpeDw1),
                          PcdGet32 (PcdGpioGroupToGpeDw2));
  }
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
  GPIO_INIT_CONFIG*     TouchpanelPad;
  GPIO_PAD_OWN          PadOwnVal;

  PadOwnVal = 0;
  TouchpanelPad = (VOID *) (UINTN) PcdGet32 (PcdBoardGpioTableTouchPanel);
  if (TouchpanelPad != NULL) {
    GpioGetPadOwnership (TouchpanelPad->GpioPad, &PadOwnVal);
    if (PadOwnVal == GpioPadOwnHost) {
      GpioConfigurePads (1, TouchpanelPad);
    }
  }
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
  GPIO_CONFIG                     BbrstConfig;
  UINT32                          WwanBbrstGpio;

  WwanBbrstGpio = PcdGet32 (PcdWwanBbrstGpio);

  if (WwanBbrstGpio) {
    //
    // BIOS needs to put modem in OFF state for the two scenarios below.
    // 1. Modem RESET# is not asserted via PLTRST# in the previous sleep state
    // 2. Modem is disabled via setup option
    //
    GpioGetPadConfig (WwanBbrstGpio, &BbrstConfig);
    if ((PcdGetBool (PcdPcieWwanEnable) == FALSE) ||
        (PcdGetBool (PcdWwanResetWorkaround) == TRUE &&
        BbrstConfig.Direction == GpioDirOut &&
        BbrstConfig.OutputState == GpioOutHigh)) {
      //
      // Assert FULL_CARD_POWER_OFF#, RESET# and PERST# GPIOs
      //
      if (PcdGet32 (PcdBoardGpioTableWwanOffEarlyPreMem) != 0 && PcdGet16 (PcdBoardGpioTableWwanOffEarlyPreMemSize) != 0) {
        ConfigureGpio ((VOID *) (UINTN) PcdGet32 (PcdBoardGpioTableWwanOffEarlyPreMem), (UINTN) PcdGet16 (PcdBoardGpioTableWwanOffEarlyPreMemSize));
      }
      if (PcdGetBool (PcdPcieWwanEnable) == TRUE && PcdGetBool (PcdWwanResetWorkaround) == TRUE) {
        MicroSecondDelay (1 * 1000); // Delay by 1ms
      }
    }

    //
    // Turn ON modem power and de-assert RESET# and PERST# GPIOs
    //
    if (PcdGetBool (PcdPcieWwanEnable) == TRUE) {
      if (PcdGet32 (PcdBoardGpioTableWwanOnEarlyPreMem) != 0 && PcdGet16 (PcdBoardGpioTableWwanOnEarlyPreMemSize) != 0) {
        ConfigureGpio ((VOID *) (UINTN) PcdGet32 (PcdBoardGpioTableWwanOnEarlyPreMem), (UINTN) PcdGet16 (PcdBoardGpioTableWwanOnEarlyPreMemSize));
      }
    }
  }
}

/**
  Configure GPIO

**/
VOID
GpioInit (
  VOID
  )
{
  ConfigureGpio ((VOID *) (UINTN) PcdGet32 (PcdBoardGpioTable), (UINTN) PcdGet16 (PcdBoardGpioTableSize));

  if (PcdGet32 (PcdBoardGpioTable2)) {
    ConfigureGpio ((VOID *) (UINTN) PcdGet32 (PcdBoardGpioTable2), (UINTN) PcdGet16 (PcdBoardGpioTable2Size));
  }

  TouchpanelGpioInit();

  //
  // Lock pads after initializing platform GPIO.
  // Pads which were requested to be unlocked during configuration
  // will not be locked.
  //
  GpioLockPads ();

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
  GPIO_CONFIG                     GpioConfig;

  if (IsCnlPch ()) {

    //
    // Tristate unused pins by audio link mode.
    //
    ZeroMem(&GpioConfig, sizeof(GPIO_CONFIG));
    GpioConfig.PadMode = GpioPadModeGpio;
    GpioConfig.HostSoftPadOwn = GpioHostOwnGpio;
    GpioConfig.Direction = GpioDirNone;
    GpioConfig.OutputState = GpioOutDefault;
    GpioConfig.InterruptConfig = GpioIntDis;
    GpioConfig.PowerConfig = GpioPlatformReset;
    GpioConfig.ElectricalConfig = GpioTermNone;

    GpioSetPadConfig (GPIO_CNL_LP_SSP1_SFRM, &GpioConfig);
    GpioSetPadConfig (GPIO_CNL_LP_SSP1_TXD, &GpioConfig);

  }

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

