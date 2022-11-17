/** @file

  Copyright (c) 2017, Linaro, Ltd. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <PiPei.h>
#include <Library/IoLib.h>
#include <Library/PeiServicesLib.h>
#include <Platform/MemoryMap.h>
#include <Ppi/EmbeddedGpio.h>

#define PDR(x)            (SYNQUACER_GPIO_BASE + 4 * (GPIO_PIN (x) >> 3))
#define DDR(x)            (SYNQUACER_GPIO_BASE + 0x10 + 4 * (GPIO_PIN (x) >> 3))
#define PFR(x)            (SYNQUACER_GPIO_BASE + 0x20 + 4 * (GPIO_PIN (x) >> 3))

#define GPIO_BIT(x)       (1U << (GPIO_PIN (x) % 8))

STATIC CONST UINTN mGpioPinCount = 32;

/**

  Gets the state of a GPIO pin

  @param This                   Pointer to protocol
  @param Gpio                   Which pin to read
  @param Value                  State of the pin

  @retval EFI_SUCCESS           GPIO state returned in Value
  @retval EFI_INVALID_PARAMETER Value is NULL
  @retval EFI_NOT_FOUND         Pin does not exit

**/
STATIC
EFI_STATUS
EFIAPI
GpioGet (
  IN  EMBEDDED_GPIO_PPI     *This,
  IN  EMBEDDED_GPIO_PIN     Gpio,
  OUT UINTN                 *Value
  )
{
  if (Value == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (GPIO_PORT (Gpio) > 0 || GPIO_PIN (Gpio) >= mGpioPinCount) {
    return EFI_NOT_FOUND;
  }

  *Value = ((MmioRead32 (PDR (GPIO_PIN (Gpio))) & GPIO_BIT (Gpio)) != 0);

  return EFI_SUCCESS;
}

/**

  Sets the state of a GPIO pin

  @param This                   Pointer to protocol
  @param Gpio                   Which pin to modify
  @param Mode                   Mode to set

  @retval EFI_SUCCESS           GPIO set as requested
  @retval EFI_INVALID_PARAMETER Invalid mode
  @retval EFI_NOT_FOUND         Pin does not exit

**/
STATIC
EFI_STATUS
EFIAPI
GpioSet (
  IN EMBEDDED_GPIO_PPI      *This,
  IN EMBEDDED_GPIO_PIN      Gpio,
  IN EMBEDDED_GPIO_MODE     Mode
  )
{
  if (GPIO_PORT (Gpio) > 0 || GPIO_PIN (Gpio) >= mGpioPinCount) {
    return EFI_NOT_FOUND;
  }

  switch (Mode) {
  case GPIO_MODE_INPUT:
    MmioAnd32 (DDR (GPIO_PIN (Gpio)), ~GPIO_BIT (Gpio));
    break;

  case GPIO_MODE_OUTPUT_0:
    MmioOr32 (DDR (GPIO_PIN (Gpio)), GPIO_BIT (Gpio));
    MmioAnd32 (PDR (GPIO_PIN (Gpio)), ~GPIO_BIT (Gpio));
    break;

  case GPIO_MODE_OUTPUT_1:
    MmioOr32 (DDR (GPIO_PIN (Gpio)), GPIO_BIT (Gpio));
    MmioOr32 (PDR (GPIO_PIN (Gpio)), GPIO_BIT (Gpio));
    break;

  default:
    return EFI_INVALID_PARAMETER;
  }
  return EFI_SUCCESS;
}


/**

  Gets the mode (function) of a GPIO pin

  @param This                   Pointer to protocol
  @param Gpio                   Which pin
  @param Mode                   Pointer to output mode value

  @retval EFI_SUCCESS           Mode value retrieved
  @retval EFI_INVALID_PARAMETER Mode is NULL
  @retval EFI_NOT_FOUND         Pin does not exit

**/
STATIC
EFI_STATUS
EFIAPI
GpioGetMode (
  IN  EMBEDDED_GPIO_PPI     *This,
  IN  EMBEDDED_GPIO_PIN     Gpio,
  OUT EMBEDDED_GPIO_MODE    *Mode
  )
{
  if (Mode == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (GPIO_PORT (Gpio) > 0 || GPIO_PIN (Gpio) >= mGpioPinCount) {
    return EFI_NOT_FOUND;
  }

  if (!(MmioRead32 (DDR (GPIO_PIN (Gpio))) & GPIO_BIT (Gpio))) {
    *Mode = GPIO_MODE_INPUT;
  } else if (!(MmioRead32 (PDR (GPIO_PIN (Gpio))) & GPIO_BIT (Gpio))) {
    *Mode = GPIO_MODE_OUTPUT_0;
  } else {
    *Mode = GPIO_MODE_OUTPUT_1;
  }
  return EFI_SUCCESS;
}


/**

  Sets the pull-up / pull-down resistor of a GPIO pin

  @param This                   Pointer to PPI
  @param Gpio                   Port/pin index
  @param Pull                   The pullup/pulldown mode to set

  @retval EFI_SUCCESS           Mode was set
  @retval EFI_NOT_FOUND         Pin does not exist
  @retval EFI_UNSUPPORTED       Action not supported

**/
STATIC
EFI_STATUS
EFIAPI
GpioSetPull (
  IN  EMBEDDED_GPIO_PPI     *This,
  IN  EMBEDDED_GPIO_PIN     Gpio,
  IN  EMBEDDED_GPIO_PULL    Pull
  )
{
  if (Pull != GPIO_PULL_NONE) {
    return EFI_UNSUPPORTED;
  }
  if (GPIO_PORT (Gpio) > 0 || GPIO_PIN (Gpio) >= mGpioPinCount) {
    return EFI_NOT_FOUND;
  }
  return EFI_SUCCESS;
}

STATIC EMBEDDED_GPIO_PPI mGpioPpi = {
  GpioGet,
  GpioSet,
  GpioGetMode,
  GpioSetPull,
};

STATIC CONST EFI_PEI_PPI_DESCRIPTOR mEmbeddedGpioPpiDescriptor = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gEdkiiEmbeddedGpioPpiGuid,
  &mGpioPpi
};

EFI_STATUS
EFIAPI
SynQuacerGpioPeiEntryPoint (
  IN       EFI_PEI_FILE_HANDLE      FfsHeader,
  IN       CONST EFI_PEI_SERVICES   **PeiServices
  )
{
  return PeiServicesInstallPpi (&mEmbeddedGpioPpiDescriptor);
}
